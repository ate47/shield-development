#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "definitions/game.hpp"
#include "definitions/variables.hpp"
#include "command.hpp"
#include "dvars.hpp"

#include <utilities/hook.hpp>
#include <utilities/json_config.hpp>
#include <utilities/string.hpp>

namespace json
{
	namespace
	{
		template <typename T>
		T convert(const std::string& str) noexcept
		{
			T result;
			std::istringstream iss(str);

			if (!(iss >> result))
			{
				iss.clear();

				if constexpr (!std::is_integral_v<T>)
				{
					iss >> std::ws;
				}

				if constexpr (std::is_same_v<T, bool>)
				{
					std::string lstr = utilities::string::to_lower(str);
					if (lstr == "true" || lstr == "1")
					{
						result = true;
					}
					else if (lstr == "false" || lstr == "0")
					{
						result = false;
					}
					else
					{
						logger::write(logger::LOG_TYPE_DEBUG, utilities::string::va("Invalid boolean representation. (%s)", str.c_str()));
						result = T();
					}
				}
				else
				{
					logger::write(logger::LOG_TYPE_DEBUG, utilities::string::va("Cannot convert string to desired type. (%s)", str.c_str()));
					result = T();
				}
			}
			return result;
		}
	}

	void writejson(const command::params& params)
	{
		if (params.size() < 5)
		{
			logger::write(logger::LOG_TYPE_DEBUG, "usage: writejson <section> <key> <value> <type> (filename)");
			return;
		}
		std::string section = params[1];
		std::string key = params[2];
		std::string value = params[3];
		std::string type = utilities::string::to_lower(params[4]);
		std::string filename = (params.size() >= 6) ? params[5] : utilities::json_config::DEFAULTJSON;

		if (type == "string")
		{
			utilities::json_config::WriteString(section.c_str(), key.c_str(), value, filename);
		}
		else if (type == "bool")
		{
			utilities::json_config::WriteBoolean(section.c_str(), key.c_str(), convert<bool>(value), filename);
		}
		else if (type == "int32_t")
		{
			utilities::json_config::WriteInteger(section.c_str(), key.c_str(), convert<int32_t>(value), filename);
		}
		else if (type == "uint32_t")
		{
			utilities::json_config::WriteUnsignedInteger(section.c_str(), key.c_str(), convert<uint32_t>(value), filename);
		}
		else if (type == "int64_t")
		{
			utilities::json_config::WriteInteger64(section.c_str(), key.c_str(), convert<int64_t>(value), filename);
		}
		else if (type == "uint64_t")
		{
			utilities::json_config::WriteUnsignedInteger64(section.c_str(), key.c_str(), convert<uint64_t>(value), filename);
		}
		else
		{
			logger::write(logger::LOG_TYPE_DEBUG, "writejson failed: invalid type");
		}
	}

	void readjson2dvar(const command::params& params)
	{
		if (params.size() < 5)
		{
			logger::write(logger::LOG_TYPE_DEBUG, "usage: readjson <dvar> <section> <key> <type> (defaultvalue) (readonly) (filename)");
			return;
		}
		std::string val = "0";

		std::string dvar = params[1];
		std::string section = params[2];
		std::string key = params[3];
		std::string type = utilities::string::to_lower(params[4]);
		std::string defaultValue = (params.size() >= 6) ? params[5] : val;
		bool readonly = (params.size() >= 7) ? convert<bool>(params[6]) : true;
		std::string filename = (params.size() >= 8) ? params[7] : utilities::json_config::DEFAULTJSON;

		if (type == "string")
		{
			val = utilities::json_config::ReadString(section.c_str(), key.c_str(), defaultValue, readonly, filename);
		}
		else if (type == "bool")
		{
			val = std::to_string(utilities::json_config::ReadBoolean(section.c_str(), key.c_str(), convert<bool>(defaultValue), readonly, filename));
		}
		else if (type == "int32_t")
		{
			val = std::to_string(utilities::json_config::ReadInteger(section.c_str(), key.c_str(), convert<int32_t>(defaultValue), readonly, filename));
		}
		else if (type == "uint32_t")
		{
			val = std::to_string(utilities::json_config::ReadUnsignedInteger(section.c_str(), key.c_str(), convert<uint32_t>(defaultValue), readonly, filename));
		}
		else if (type == "int64_t")
		{
			val = std::to_string(utilities::json_config::ReadInteger64(section.c_str(), key.c_str(), convert<int64_t>(defaultValue), readonly, filename));
		}
		else if (type == "uint64_t")
		{
			val = std::to_string(utilities::json_config::ReadUnsignedInteger64(section.c_str(), key.c_str(), convert<uint64_t>(defaultValue), readonly, filename));
		}
		else
		{
			logger::write(logger::LOG_TYPE_DEBUG, "readjson failed: invalid type");
			return;
		}

		const char* cmd = utilities::string::va("set %s %s;", dvar.c_str(), val.c_str());
		logger::write(logger::LOG_TYPE_DEBUG, cmd);
		game::Cbuf_AddText(0, cmd);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			command::add("writejson", [&](const command::params& params){ writejson(params); });
			command::add("readjson", [&](const command::params& params){ readjson2dvar(params); });
		}
	};
}

REGISTER_COMPONENT(json::component)
