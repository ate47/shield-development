#include <std_include.hpp>
#include "dynamic_data.hpp"
#include "component/gsc_funcs.hpp"

#include <utilities/string.hpp>

namespace dynamic_data
{

	void dd_container::load_dd(rapidjson::Value& object, void* buffer, void* mask) const
	{
		if (!object.IsObject())
		{
			throw std::runtime_error("loading from something that isn't a json object");
		}

		for (const auto& [key, value] : object.GetObj())
		{
			if (!key.IsString())
			{
				continue;
			}

			const char* key_val = key.GetString();
			std::string_view key_v{ key_val };

			const field* field{};

			if (!key_v.rfind("unk_", 0))
			{
				try
				{
					size_t loc = std::strtoull(&key_val[4], nullptr, 16);

					auto it = std::find_if(fields, fields + field_count, [loc](const dynamic_data::field& f) { return f.offset == loc; });

					if (it != fields + field_count)
					{
						field = it;
					}
					else
					{
						throw std::runtime_error(utilities::string::va("unknown field unk value '%s'", key_val));
					}
				}
				catch (const std::invalid_argument& e)
				{
					throw std::runtime_error(utilities::string::va("invalid field name '%s': '%s'", key_val, e.what()));
				}
			}
			else
			{
				uint32_t canon = gsc_funcs::canon_hash_pattern(key.GetString());

				auto it = std::find_if(fields, fields + field_count, [canon](const dynamic_data::field& f) { return f.canon_name == canon; });

				if (it != fields + field_count)
				{
					field = it;
				}
				else
				{
					throw std::runtime_error(utilities::string::va("unknown field name '%s'", key_val));
				}
			}

			logger::write(logger::LOG_TYPE_DEBUG, "load field %s (%lx) -> %llx", key_val, field->canon_name, field->size);

			// update the mask
			memset((byte*)mask + field->offset, 0xFF, field->size);

			byte* data = (byte*)buffer + field->offset;
			const dynamic_data::dd_container& container = *this;
			auto handle_field_val = [&container, key_val, field, data](const rapidjson::Value& val, size_t idx) {

				auto [type, name] = get_named_type(field->type);
				switch (type)
				{
				case FT_UINT:
				{
					uint64_t u;
					if (val.IsBool())
					{
						u = val.GetBool() ? 1 : 0;
					}
					else if (val.IsNumber())
					{
						u = val.GetUint64();
					}
					else
					{
						throw std::runtime_error(utilities::string::va("field '%s' should be a number", key_val));
					}

					size_t len = field->size / field->array_size;
					switch (len)
					{
					case 1:
						*(data + len * idx) = (byte)u;
						break;
					case 2:
						*(uint16_t*)(data + len * idx) = (uint16_t)u;
						break;
					case 4:
						*(uint32_t*)(data + len * idx) = (uint32_t)u;
						break;
					case 8:
						*(uint64_t*)(data + len * idx) = (uint64_t)u;
						break;
					default:
						assert(0 && "field definition with bad size");
						break;
					}
				}
					break;
				case FT_INT:
				{
					int64_t u;
					if (val.IsBool())
					{
						u = val.GetBool() ? 1 : 0;
					}
					else if (val.IsNumber())
					{
						u = val.GetInt64();
					}
					else
					{
						throw std::runtime_error(utilities::string::va("field '%s' should be a number", key_val));
					}

					size_t len = field->size / field->array_size;
					switch (len)
					{
					case 1:
						*(int8_t*)(data + len * idx) = (int8_t)u;
						break;
					case 2:
						*(int16_t*)(data + len * idx) = (int16_t)u;
						break;
					case 4:
						*(int32_t*)(data + len * idx) = (int32_t)u;
						break;
					case 8:
						*(int64_t*)(data + len * idx) = (int64_t)u;
						break;
					default:
						assert(0 && "field definition with bad size");
						break;
					}
				}
					break;
				case FT_FLOAT:
				{
					if (!val.IsNumber())
					{
						throw std::runtime_error(utilities::string::va("field '%s' should be a number", key_val));
					}
					double u = val.GetDouble();

					size_t len = field->size / field->array_size;
					switch (len)
					{
					case 4:
						*(float*)(data + len * idx) = (float)u;
						break;
					case 8:
						*(double*)(data + len * idx) = u;
						break;
					default:
						assert(0 && "field definition with bad size");
						break;
					}
				}
				break;
				case FT_XHASH:
				{
					if (!val.IsString())
					{
						throw std::runtime_error(utilities::string::va("field '%s' should be a string", key_val));
					}
					const char* s = val.GetString();

					if (*s == '#') s++; // keep a "relative" compatibility with the bo4-source dump

					uint64_t hash = fnv1a::generate_hash_pattern(s);

					*(uint64_t*)(data + 0x10 * idx) = hash;
				}
					break;
				case FT_ENUM:
				{
					if (!val.IsString())
					{
						throw std::runtime_error(utilities::string::va("field '%s' should be a string", key_val));
					}
					const char* enum_val_str = val.GetString();
					uint64_t enum_val = fnv1a::generate_hash_const(enum_val_str);
					auto it = std::find_if(container.enums, container.enums + container.enum_count, 
						[name, enum_val](const dynamic_data::enum_type& et) { return et.type_name == name && et.field_name == enum_val; });

					if (it == container.enums + container.enum_count)
					{
						throw std::runtime_error(utilities::string::va("unknown enum value for key '%s' = '%s'", key_val, enum_val_str));
					}

					int64_t u = it->value;

					size_t len = field->size / field->array_size;
					switch (len)
					{
					case 1:
						*(int8_t*)(data + len * idx) = (int8_t)u;
						break;
					case 2:
						*(int16_t*)(data + len * idx) = (int16_t)u;
						break;
					case 4:
						*(int32_t*)(data + len * idx) = (int32_t)u;
						break;
					case 8:
						*(int64_t*)(data + len * idx) = (int64_t)u;
						break;
					default:
						assert(0 && "field definition with bad size");
						break;
					}

				}
					break;
				case FT_STRING:
					// todo
				default:
					assert(0 && "unknown field type");
					throw std::runtime_error(utilities::string::va("unknown field type for key '%s' %d", key_val, type));
				}
			};

			if (value.IsArray())
			{
				auto arr = value.GetArray();

				if (field->array_size < arr.Size())
				{
					throw std::runtime_error(utilities::string::va("array definition too big for field '%s' (%lld < %lld)", key_val, field->array_size, arr.Size()));
				}

				size_t idx{};
				for (const auto& array_val : arr)
				{
					handle_field_val(array_val, idx++);
				}
			}
			else if (value.IsObject())
			{
				throw std::runtime_error(utilities::string::va("field '%s' can't be an object", key_val));
			}
			else
			{
				handle_field_val(value, 0);
			}
		}
	}
}