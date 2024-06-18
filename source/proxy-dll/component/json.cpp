#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "definitions/game.hpp"
#include "definitions/variables.hpp"
#include "command.hpp"
#include "dvars.hpp"

#include <utilities/hook.hpp>
#include <utilities/json_config.hpp>
#include <utilities/string.hpp>

#include <sstream>
#include <unordered_set>

namespace json
{
	namespace
	{
		const std::unordered_set<std::string> valid_types = {
			"string", "bool", "int32_t", "uint32_t", "int64_t", "uint64_t"
		};

		bool is_valid_type(const std::string& type) {
			return valid_types.find(type) != valid_types.end();
		}

		template <typename T>
		T convert(const std::string& str)
		{
			T result;
			std::istringstream iss(str);
			if (!(iss >> result))
			{
				iss.clear();
				iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				//logger::write(logger::LOG_TYPE_INFO, "Conversion failed for input: " + str);
				return T();
			}
			return result;
		}

		template <>
		bool convert<bool>(const std::string& str)
		{
			std::istringstream iss(str);
			bool result;
			if (!(iss >> std::boolalpha >> result))
			{
				if (str == "0" || str == "1")
				{
					return str == "1";
				}
				//logger::write(logger::LOG_TYPE_INFO, "Conversion failed for input: " + str);
				return false;
			}
			return result;
		}
	}

	void writejson(const command::params& params)
	{
		if (params.size() < 5)
		{
			logger::write(logger::LOG_TYPE_INFO, "usage: writejson <section> <key> <value> <type> (filename)");
			return;
		}
		std::string section = params[1];
		std::string key = params[2];
		std::string value = params[3];
		std::string type = utilities::string::to_lower(params[4]);
		std::string filename = (params.size() >= 6) ? params[5] : utilities::json_config::DEFAULTJSON;

		if (!is_valid_type(type))
		{
			//logger::write(logger::LOG_TYPE_INFO, "writejson failed: invalid type");
			return;
		}

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
	}

	void readjson2dvar(const command::params& params)
	{
		if (params.size() < 5)
		{
			logger::write(logger::LOG_TYPE_INFO, "usage: readjson <dvar> <section> <key> <type> (defaultvalue) (readonly) (filename)");
			return;
		}
		std::string val;

		std::string dvar = params[1];
		std::string section = params[2];
		std::string key = params[3];
		std::string type = utilities::string::to_lower(params[4]);
		std::string defaultValue = (params.size() >= 6) ? params[5] : val;
		bool readonly = (params.size() >= 7) ? convert<bool>(params[6]) : true;
		std::string filename = (params.size() >= 8) ? params[7] : utilities::json_config::DEFAULTJSON;

		if (!is_valid_type(type))
		{
			//logger::write(logger::LOG_TYPE_INFO, "readjson failed: invalid type");
			return;
		}

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

		const char* cmd = utilities::string::va("set %s %s;", dvar.c_str(), val.c_str());
		logger::write(logger::LOG_TYPE_INFO, cmd);
		game::Cbuf_AddText(0, cmd);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			command::add("writejson", [&](const command::params& params) { writejson(params); });
			command::add("readjson", [&](const command::params& params) { readjson2dvar(params); });
		}
	};
}

REGISTER_COMPONENT(json::component)
