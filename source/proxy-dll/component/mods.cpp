#include <std_include.hpp>
#include "definitions/xassets.hpp"
#include "definitions/game.hpp"

#include "game_console.hpp"

#include "gsc_funcs.hpp"
#include "gsc_custom.hpp"
#include "loader/component_loader.hpp"

#include <utilities/io.hpp>
#include <utilities/hook.hpp>

namespace mods {
	// GSC File magic (8 bytes)
	constexpr uint64_t gsc_magic = 0x36000A0D43534780;
	// Serious' GSIC File Magic (4 bytes)
	constexpr const char* gsic_magic = "GSIC";

	constexpr const char* mod_metadata_file = "metadata.json";
	std::filesystem::path mod_dir = "project-bo4/mods";

	namespace {
		struct raw_file
		{
			xassets::raw_file_header header{};

			std::string data{};

			auto* get_header()
			{
				header.buffer = data.data();
				header.size = (uint32_t)data.length();

				return &header;
			}
		};
		struct scriptparsetree
		{
			xassets::scriptparsetree_header header{};

			std::string data{};
			size_t gsic_header_size{};
			std::unordered_set<uint64_t> hooks{};
			gsc_custom::gsic_info gsic{};

			auto* get_header()
			{
				header.buffer = reinterpret_cast<game::GSC_OBJ*>(data.data());
				header.size = (uint32_t)data.length();

				for (gsc_custom::gsic_detour& detour : gsic.detours)
				{
					detour.fixup_function = header.buffer->magic + detour.fixup_offset;
				}

				return &header;
			}

			bool can_read_gsic(size_t bytes)
			{
				return data.length() >= gsic_header_size + bytes;
			}

			bool load_gsic()
			{
				byte* ptr = (byte*)data.data();

				if (!can_read_gsic(4) || memcmp(gsic_magic, ptr, 4))
				{
					return true; // not a gsic file
				}
				gsic_header_size += 4;

				if (!can_read_gsic(4))
				{
					logger::write(logger::LOG_TYPE_ERROR, "can't read gsic fields");
					return false;
				}
				int32_t fields = *reinterpret_cast<int32_t*>(ptr + gsic_header_size);
				gsic_header_size += 4;

				for (size_t i = 0; i < fields; i++)
				{
					if (!can_read_gsic(4))
					{
						logger::write(logger::LOG_TYPE_ERROR, "can't read gsic field type");
						return false;
					}

					int32_t field_type = *reinterpret_cast<int32_t*>(ptr + gsic_header_size);
					gsic_header_size += 4;

					switch (field_type)
					{
					case gsc_custom::gsic_field_type::GSIC_FIELD_DETOUR:
					{
						// detours
						if (!can_read_gsic(4))
						{
							logger::write(logger::LOG_TYPE_ERROR, "can't read gsic detours count");
							return false;
						}
						int32_t detour_count = *reinterpret_cast<int32_t*>(ptr + gsic_header_size);
						gsic_header_size += 4;


						if (!can_read_gsic(detour_count * 256ull))
						{
							logger::write(logger::LOG_TYPE_ERROR, "can't read detours");
							return false;
						}

						for (size_t j = 0; j < detour_count; j++)
						{
							gsc_custom::gsic_detour& detour = gsic.detours.emplace_back();

							detour.fixup_name = *reinterpret_cast<uint32_t*>(ptr + gsic_header_size);
							detour.replace_namespace = *reinterpret_cast<uint32_t*>(ptr + gsic_header_size + 4);
							detour.replace_function = *reinterpret_cast<uint32_t*>(ptr + gsic_header_size + 8);
							detour.fixup_offset = *reinterpret_cast<uint32_t*>(ptr + gsic_header_size + 12);
							detour.fixup_size = *reinterpret_cast<uint32_t*>(ptr + gsic_header_size + 16);
							detour.target_script = *reinterpret_cast<uint64_t*>(ptr + gsic_header_size + 20);

							logger::write(logger::LOG_TYPE_DEBUG, std::format(
								"read detour {:x} : namespace_{:x}<script_{:x}>::function_{:x} / offset={:x}+{:x}",
								detour.fixup_name, detour.replace_namespace, detour.target_script, detour.replace_function,
								detour.fixup_offset, detour.fixup_size
							));

							gsic_header_size += 256;
						}
					}
					break;
					default:
						logger::write(logger::LOG_TYPE_ERROR, "bad gsic field type {}", field_type);
						return false;
					}
				}

				// we need to remove the header to keep the alignment
				data = data.substr(gsic_header_size, data.length() - gsic_header_size);

				return true;
			}
		};
		struct lua_file
		{
			xassets::lua_file_header header{};

			std::string data{};

			auto* get_header()
			{
				header.buffer = reinterpret_cast<byte*>(data.data());
				header.size = (uint32_t)data.length();

				return &header;
			}
		};
		struct string_table_file
		{
			xassets::stringtable_header header{};

			std::string data{};
			std::vector<xassets::stringtable_cell> cells{};

			auto* get_header()
			{
				header.values = cells.data();

				return &header;
			}
		};


		class mod_storage
		{
		public:
			std::mutex load_mutex{};
			std::vector<char*> allocated_strings{};
			std::vector<scriptparsetree> gsc_files{};
			std::vector<raw_file> raw_files{};
			std::vector<lua_file> lua_files{};
			std::vector<string_table_file> csv_files{};

			~mod_storage()
			{
				for (char* str : allocated_strings)
				{
					delete str;
				}
			}

			void clear()
			{
				// clear previously loaded files
				raw_files.clear();
				gsc_files.clear();
				lua_files.clear();
				csv_files.clear();

				for (char* str : allocated_strings)
				{
					delete str;
				}
				allocated_strings.clear();
			}

			char* allocate_string(const std::string& string)
			{
				char* str = new char[string.length() + 1];
				memcpy(str, string.c_str(), string.length() + 1);

				allocated_strings.emplace_back(str);

				return str;
			}

			void* get_xasset(xassets::XAssetType type, uint64_t name)
			{
				std::lock_guard lg{ load_mutex };
				switch (type)
				{
				case xassets::ASSET_TYPE_SCRIPTPARSETREE:
				{
					auto it = std::find_if(gsc_files.begin(), gsc_files.end(), [name](const scriptparsetree& file) { return file.header.name == name; });

					if (it == gsc_files.end()) return nullptr;

					return it->get_header();
				}
				case xassets::ASSET_TYPE_RAWFILE:
				{
					auto it = std::find_if(raw_files.begin(), raw_files.end(), [name](const raw_file& file) { return file.header.name == name; });

					if (it == raw_files.end()) return nullptr;

					return it->get_header();
				}
				case xassets::ASSET_TYPE_LUAFILE:
				{
					auto it = std::find_if(lua_files.begin(), lua_files.end(), [name](const lua_file& file) { return file.header.name == name; });

					if (it == lua_files.end()) return nullptr;

					return it->get_header();
				}
				case xassets::ASSET_TYPE_STRINGTABLE:
				{
					auto it = std::find_if(csv_files.begin(), csv_files.end(), [name](const string_table_file& file) { return file.header.name == name; });

					if (it == csv_files.end()) return nullptr;

					return it->get_header();
				}
				default:
					return nullptr; // unknown resource type
				}
			}

			bool read_data_entry(rapidjson::Value& member, const char* mod_name, const std::filesystem::path& mod_path)
			{
				auto type = member.FindMember("type");

				if (type == member.MemberEnd() || !type->value.IsString())
				{
					logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a data member without a valid type", mod_name));
					return false;
				}

				const char* type_val = type->value.GetString();

				if (!_strcmpi("scriptparsetree", type_val))
				{
					auto name_mb = member.FindMember("name");
					auto path_mb = member.FindMember("path");

					if (
						name_mb == member.MemberEnd() || path_mb == member.MemberEnd()
						|| !name_mb->value.IsString() || !path_mb->value.IsString()
						)
					{
						logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a bad scriptparsetree def, missing/bad name or path", mod_name));
						return false;
					}

					scriptparsetree tmp{};
					std::filesystem::path path_cfg = path_mb->value.GetString();
					auto spt_path = path_cfg.is_absolute() ? path_cfg : (mod_path / path_cfg);
					tmp.header.name = fnv1a::generate_hash_pattern(name_mb->value.GetString());

					auto hooks = member.FindMember("hooks");

					if (hooks != member.MemberEnd())
					{
						// no hooks might not be an error, to replace a script for example

						if (!hooks->value.IsArray())
						{
							logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a bad scriptparsetree hook def, not an array for {}", mod_name, spt_path.string()));
							return false;
						}

						for (auto& hook : hooks->value.GetArray())
						{
							if (!hook.IsString())
							{
								logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a bad scriptparsetree hook def, not a string for {}", mod_name, spt_path.string()));
								return false;
							}

							tmp.hooks.insert(fnv1a::generate_hash_pattern(hook.GetString()));
						}
					}

					if (!utilities::io::read_file(spt_path.string(), &tmp.data))
					{
						logger::write(logger::LOG_TYPE_ERROR, std::format("can't read scriptparsetree {} for mod {}", spt_path.string(), mod_name));
						return false;
					}

					if (!tmp.load_gsic())
					{
						logger::write(logger::LOG_TYPE_ERROR, std::format("error when reading GSIC header of {} for mod {}", spt_path.string(), mod_name));
						return false;
					}

					if (tmp.gsic.detours.size())
					{
						logger::write(logger::LOG_TYPE_DEBUG, std::format("loaded {} detours", tmp.gsic.detours.size()));
					}

					if (tmp.data.length() < sizeof(game::GSC_OBJ) || *reinterpret_cast<uint64_t*>(tmp.data.data()) != gsc_magic)
					{
						logger::write(logger::LOG_TYPE_ERROR, std::format("bad scriptparsetree magic in {} for mod {}", spt_path.string(), mod_name));
						return false;
					}

					// after this point we assume that the GSC file is well formatted
					
					game::GSC_OBJ* script_obj = reinterpret_cast<game::GSC_OBJ*>(tmp.data.data());

					// fix compiler script name
					script_obj->name = tmp.header.name;

					// fix compiler custom namespace
					game::GSC_IMPORT_ITEM* imports = script_obj->get_imports();

					static uint32_t isprofilebuild_hash = gsc_funcs::canon_hash("IsProfileBuild");
					static uint32_t serious_custom_func_name_hash = gsc_funcs::canon_hash(gsc_funcs::serious_custom_func_name);
					for (size_t imp = 0; imp < script_obj->imports_count; imp++)
					{
						if (imports->name == isprofilebuild_hash && imports->param_count != 0)
						{
							// compiler:: calls, replace the call to our custom function
							imports->name = serious_custom_func_name_hash;
						}

						imports = reinterpret_cast<game::GSC_IMPORT_ITEM*>((uint32_t*)&imports[1] + imports->num_address);
					}

					logger::write(logger::LOG_TYPE_DEBUG, std::format("mod {}: loaded scriptparsetree {} -> {:x}", mod_name, spt_path.string(), tmp.header.name));
					gsc_files.emplace_back(tmp);
				}
				else if (!_strcmpi("rawfile", type_val))
				{
					auto name_mb = member.FindMember("name");
					auto path_mb = member.FindMember("path");

					if (
						name_mb == member.MemberEnd() || path_mb == member.MemberEnd()
						|| !name_mb->value.IsString() || !path_mb->value.IsString()
						)
					{
						logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a bad rawfile def, missing/bad name or path", mod_name));
						return false;
					}

					raw_file tmp{};
					std::filesystem::path path_cfg = path_mb->value.GetString();
					auto raw_file_path = path_cfg.is_absolute() ? path_cfg : (mod_path / path_cfg);
					tmp.header.name = fnv1a::generate_hash_pattern(name_mb->value.GetString());

					if (!utilities::io::read_file(raw_file_path.string(), &tmp.data))
					{
						logger::write(logger::LOG_TYPE_ERROR, std::format("can't read raw file {} for mod {}", raw_file_path.string(), mod_name));
						return false;
					}

					logger::write(logger::LOG_TYPE_DEBUG, std::format("mod {}: loaded raw file {} -> {:x}", mod_name, raw_file_path.string(), tmp.header.name));
					raw_files.emplace_back(tmp);
				}
				else if (!_strcmpi("luafile", type_val))
				{
					auto name_mb = member.FindMember("name");
					auto path_mb = member.FindMember("path");

					if (
						name_mb == member.MemberEnd() || path_mb == member.MemberEnd()
						|| !name_mb->value.IsString() || !path_mb->value.IsString()
						)
					{
						logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a bad luafile def, missing/bad name or path", mod_name));
						return false;
					}

					lua_file tmp{};
					std::filesystem::path path_cfg = path_mb->value.GetString();
					auto lua_file_path = path_cfg.is_absolute() ? path_cfg : (mod_path / path_cfg);
					tmp.header.name = fnv1a::generate_hash_pattern(name_mb->value.GetString());

					if (!utilities::io::read_file(lua_file_path.string(), &tmp.data))
					{
						logger::write(logger::LOG_TYPE_ERROR, std::format("can't read lua file {} for mod {}", lua_file_path.string(), mod_name));
						return false;
					}

					logger::write(logger::LOG_TYPE_DEBUG, std::format("mod {}: loaded lua file {} -> {:x}", mod_name, lua_file_path.string(), tmp.header.name));
					lua_files.emplace_back(tmp);
				}
				else if (!_strcmpi("stringtable", type_val))
				{
					auto name_mb = member.FindMember("name");
					auto path_mb = member.FindMember("path");

					if (
						name_mb == member.MemberEnd() || path_mb == member.MemberEnd()
						|| !name_mb->value.IsString() || !path_mb->value.IsString()
						)
					{
						logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a bad stringtable def, missing/bad name or path", mod_name));
						return false;
					}

					string_table_file tmp{};
					std::filesystem::path path_cfg = path_mb->value.GetString();
					auto stringtable_file_path = path_cfg.is_absolute() ? path_cfg : (mod_path / path_cfg);
					tmp.header.name = fnv1a::generate_hash_pattern(name_mb->value.GetString());

					if (!utilities::io::read_file(stringtable_file_path.string(), &tmp.data))
					{
						logger::write(logger::LOG_TYPE_ERROR, std::format("can't read stringtable file {} for mod {}", stringtable_file_path.string(), mod_name));
						return false;
					}

					rapidcsv::Document doc{};

					std::stringstream stream{ tmp.data };

					doc.Load(stream, rapidcsv::LabelParams(-1, -1));

					size_t rows_count_tmp = doc.GetRowCount();
					tmp.header.rows_count = rows_count_tmp != 0 ? (int32_t)(rows_count_tmp - 1) : 0;
					tmp.header.columns_count = (int32_t)doc.GetColumnCount();

					std::vector<xassets::stringtable_cell_type> cell_types{};

					for (size_t i = 0; i < tmp.header.columns_count; i++)
					{
						// read cell types
						const std::string cell = doc.GetCell<std::string>(i, 0);

						xassets::stringtable_cell_type cell_type = xassets::STC_TYPE_STRING;
						if (cell == "undefined")
						{
							cell_type = xassets::STC_TYPE_UNDEFINED;
						}
						else if (cell == "string")
						{
							cell_type = xassets::STC_TYPE_STRING;
						}
						else if (cell == "int")
						{
							cell_type = xassets::STC_TYPE_INT;
						}
						else if (cell == "float")
						{
							cell_type = xassets::STC_TYPE_FLOAT;
						}
						else if (cell == "hash")
						{
							cell_type = xassets::STC_TYPE_HASHED2;
						}
						else if (cell == "hash7")
						{
							cell_type = xassets::STC_TYPE_HASHED7;
						}
						else if (cell == "hash8")
						{
							cell_type = xassets::STC_TYPE_HASHED8;
						}
						else if (cell == "bool")
						{
							cell_type = xassets::STC_TYPE_BOOL;
						}
						else
						{
							logger::write(logger::LOG_TYPE_ERROR, std::format("mod {} : can't read stringtable {} type of column {} : '{}'", mod_name, stringtable_file_path.string(), i, cell));
							return false;
						}

						cell_types.emplace_back(cell_type);
					}

					for (size_t row = 1; row <= tmp.header.rows_count; row++)
					{
						// read cells
						for (size_t column = 0; column < tmp.header.columns_count; column++)
						{
							xassets::stringtable_cell_type cell_type = cell_types[column];

							const std::string cell_str = doc.GetCell<std::string>(column, row);

							xassets::stringtable_cell& cell = tmp.cells.emplace_back();
							cell.type = cell_type;

							try
							{
								switch (cell_type)
								{
								case xassets::STC_TYPE_UNDEFINED:
									cell.value.int_value = 0;
									break;
								case xassets::STC_TYPE_BOOL:
									cell.value.bool_value = cell_str == "true";
									break;
								case xassets::STC_TYPE_HASHED2:
								case xassets::STC_TYPE_HASHED7:
								case xassets::STC_TYPE_HASHED8:
									cell.value.hash_value = fnv1a::generate_hash_pattern(cell_str.c_str());
									break;
								case xassets::STC_TYPE_INT:
									if (cell_str.starts_with("0x"))
									{
										cell.value.int_value = std::stoull(cell_str.substr(2), nullptr, 16);
									}
									else
									{
										cell.value.int_value = std::stoll(cell_str);
									}
									break;
								case xassets::STC_TYPE_FLOAT:
									cell.value.float_value = std::stof(cell_str);
									break;
								case xassets::STC_TYPE_STRING:
									cell.value.string_value = allocate_string(cell_str);
									break;
								}
							}
							catch (const std::invalid_argument& e)
							{
								logger::write(logger::LOG_TYPE_DEBUG, std::format("mod {}: error when loading stringtable file {} : {} [line {} col {}] '{}'", mod_name, stringtable_file_path.string(), e.what(), row, column, cell_str));
								return false;
							}
						}
					}
					
					logger::write(logger::LOG_TYPE_DEBUG, std::format("mod {}: loaded stringtable file {} -> {:x}", mod_name, stringtable_file_path.string(), tmp.header.name));
					csv_files.emplace_back(tmp);
				}
				else
				{
					logger::write(logger::LOG_TYPE_ERROR, std::format("mod {} is load data member with an unknown type '{}'", mod_name, type_val));
					return false;
				}

				return true;
			}

			bool load_mods()
			{
				std::lock_guard lg{ load_mutex };
				clear();
				rapidjson::Document info{};
				std::string mod_metadata{};

				bool err = false;

				std::filesystem::create_directories(mod_dir);
				for (const auto& mod : std::filesystem::directory_iterator{ mod_dir })
				{
					if (!mod.is_directory()) continue; // not a directory

					std::filesystem::path mod_path = mod.path();
					std::filesystem::path mod_metadata_path = mod_path / mod_metadata_file;

					if (!std::filesystem::exists(mod_metadata_path)) continue; // doesn't contain the metadata file


					std::string filename = mod_metadata_path.string();
					if (!utilities::io::read_file(filename, &mod_metadata))
					{
						logger::write(logger::LOG_TYPE_ERROR, std::format("can't read mod metadata file '{}'", filename));
						err = true;
						continue;
					}

					info.Parse(mod_metadata);

					if (info.HasParseError()) {
						logger::write(logger::LOG_TYPE_ERROR, std::format("can't parse mod json metadata '{}'", filename));
						err = true;
						continue;
					}

					auto name_member = info.FindMember("name");

					const char* mod_name;

					if (name_member != info.MemberEnd() && name_member->value.IsString())
					{
						mod_name = name_member->value.GetString();
					}
					else
					{
						mod_name = filename.c_str();
					}
					logger::write(logger::LOG_TYPE_INFO, std::format("loading mod {}...", mod_name));

					int mod_errors = 0;
					auto data_member = info.FindMember("data");

					if (data_member != info.MemberEnd() && data_member->value.IsArray())
					{
						auto data_array = data_member->value.GetArray();

						for (rapidjson::Value& member : data_array)
						{
							if (!member.IsObject())
							{
								logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a bad data member", mod_name));
								mod_errors++;
								continue;
							}

							if (!read_data_entry(member, mod_name, mod_path))
							{
								mod_errors++;
							}
						}
					}

					if (mod_errors)
					{
						logger::write(logger::LOG_TYPE_WARN, std::format("mod {} loaded with {} error{}.", mod_name, mod_errors, mod_errors > 1 ? "s" : ""));
						err = true;
					}
				}
				return err;
			}
		};

		mod_storage storage{};


		void load_mods_cmd()
		{
			if (!game::Com_IsRunningUILevel())
			{
				// avoid gsc issues, but if a script is loaded in the frontend, it will still crash
				game_console::print("can't load mods while in-game!"); 
				return;
			}

			if (!storage.load_mods())
			{
				game_console::print("mods reloaded.");
			}
			else
			{
				game_console::print("mods reloaded with errors, see logs.");
			}
		}
	}

	utilities::hook::detour db_find_xasset_header_hook;
	utilities::hook::detour scr_gsc_obj_link_hook;

	void* db_find_xasset_header_stub(xassets::XAssetType type, game::BO4_AssetRef_t* name, bool errorIfMissing, int waitTime)
	{
		void* header = storage.get_xasset(type, name->hash);

		if (header)
		{
			return header; // overwrite/load custom data
		}

		return db_find_xasset_header_hook.invoke<void*>(type, name, errorIfMissing, waitTime);
	}

	int scr_gsc_obj_link_stub(game::scriptInstance_t inst, game::GSC_OBJ* prime_obj, bool runScript)
	{
		// link the injected scripts if we find a hook, sync the gsic fields at the same time 
		// because we know the instance.
		for (auto& spt : storage.gsc_files)
		{
			if (spt.hooks.find(prime_obj->name) != spt.hooks.end())
			{
				gsc_custom::sync_gsic(inst, spt.gsic);
				int err = scr_gsc_obj_link_hook.invoke<int>(inst, spt.get_header()->buffer, runScript);

				if (err < 0)
				{
					return err; // error when linking
				}
			}
		}

		auto custom_replaced_it = std::find_if(storage.gsc_files.begin(), storage.gsc_files.end(),
			[prime_obj](scriptparsetree& e){ return e.get_header()->buffer == prime_obj; });

		if (custom_replaced_it != storage.gsc_files.end())
		{
			// replaced gsc file
			gsc_custom::sync_gsic(inst, custom_replaced_it->gsic);
		}

		return scr_gsc_obj_link_hook.invoke<int>(inst, prime_obj, runScript);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// custom assets loading
			db_find_xasset_header_hook.create(0x142EB75B0_g, db_find_xasset_header_stub);

			scr_gsc_obj_link_hook.create(0x142748F10_g, scr_gsc_obj_link_stub);

			// register load mods command
			Cmd_AddCommand("reload_mods", load_mods_cmd);
		}

		void pre_start() override
		{
			storage.load_mods();
		}
	};
}

REGISTER_COMPONENT(mods::component)