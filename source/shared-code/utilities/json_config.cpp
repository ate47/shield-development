#include "json_config.hpp"
#include "io.hpp"

#include <unordered_map>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

// #define USE_MULTI_THREAD

#ifdef USE_MULTI_THREAD
#include <atomic>
#include <memory>
#include <mutex>
#endif

namespace utilities
{
	std::vector<std::string> split_string(const std::string& input, char delimiter)
	{
		std::vector<std::string> tokens;
		std::stringstream ss(input);
		std::string token;
		while (std::getline(ss, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}
}

namespace utilities::json_config
{
#ifdef USE_MULTI_THREAD
	std::mutex json_mutex;
	using DocumentPtr = std::shared_ptr<rapidjson::Document>;
#else
	using DocumentPtr = std::unique_ptr<rapidjson::Document>;
#endif
	std::unordered_map<std::string, DocumentPtr> json_documents;

	namespace
	{
		bool read_json_config(const std::string& filename, rapidjson::Document& doc)
		{
			std::string json_data;
			if (!io::read_file(filename, &json_data)) return false;

			doc.Parse(json_data.c_str());
			return !doc.HasParseError() && doc.IsObject();
		}

		bool write_json_config(const std::string& filename)
		{
#ifdef USE_MULTI_THREAD
			std::lock_guard<std::mutex> lock(json_mutex);
#endif
			auto it = json_documents.find(filename);
			if (it != json_documents.end())
			{
				rapidjson::StringBuffer buffer;
				rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
				it->second->Accept(writer);

				std::string json_data(buffer.GetString(), buffer.GetSize());
				return io::write_file(filename, json_data);
			}
			else
			{
				return false;
			}
		}
	}

	template <typename T>
	rapidjson::Value ToJsonValue(const T& value, rapidjson::Document::AllocatorType& allocator)
	{
		rapidjson::Value jsonValue(rapidjson::kNullType);

		if constexpr (std::is_same_v<T, bool>)
		{
			jsonValue.SetBool(value);
		}
		else if constexpr (std::is_integral_v<T>)
		{
			if constexpr (std::is_signed_v<T>)
			{
				if constexpr (sizeof(T) <= 4)
					jsonValue.SetInt(value);
				else
					jsonValue.SetInt64(value);
			}
			else
			{
				if constexpr (sizeof(T) <= 4)
					jsonValue.SetUint(value);
				else
					jsonValue.SetUint64(value);
			}
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			jsonValue.SetDouble(value);
		}
		else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, std::string>)
		{
			jsonValue.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), allocator);
		}
		else
		{
			static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, const char*> || std::is_same_v<T, std::string>, "Unsupported type for ToJsonValue");
		}

		return jsonValue;
	}

	rapidjson::Document& get_json_document(const std::string& filename, bool useCached = false)
	{
#ifdef USE_MULTI_THREAD
		std::lock_guard<std::mutex> lock(json_mutex);
#endif
		DocumentPtr& documentPtr = json_documents[filename];
		if (!documentPtr || !useCached)
		{
#ifdef USE_MULTI_THREAD
			documentPtr = std::make_shared<rapidjson::Document>();
#else
			documentPtr = std::make_unique<rapidjson::Document>();
#endif
			if (!read_json_config(filename, *documentPtr))
			{
				documentPtr->SetObject();
			}
		}

		return *documentPtr;
	}

	rapidjson::Value& get_json_section(rapidjson::Document& doc, const char* szSection, bool readonly = false)
	{
		if (!readonly && !doc.HasMember(szSection))
		{
			rapidjson::Value section(rapidjson::kObjectType);
			doc.AddMember(rapidjson::StringRef(szSection), section.Move(), doc.GetAllocator());
		}

		return doc[szSection];
	}

	rapidjson::Value& get_json_section_recursive(rapidjson::Document& doc, rapidjson::Value& current_section, const std::vector<std::string>& sections, bool readonly = false)
	{
		rapidjson::Value* current = &current_section;
		for (const auto& section : sections)
		{
			if (!current->IsObject())
			{
				current->SetObject();
			}

			if (!current->HasMember(section.c_str()))
			{
				if (!readonly)
				{
					rapidjson::Value new_section(rapidjson::kObjectType);
					rapidjson::Value section_name(section.c_str(), doc.GetAllocator());
					current->AddMember(section_name, new_section, doc.GetAllocator());
				}
				else
				{
					rapidjson::Value empty_section(rapidjson::kObjectType);
					rapidjson::Value section_name(section.c_str(), doc.GetAllocator());
					current->AddMember(section_name, empty_section, doc.GetAllocator());
				}
			}

			current = &((*current)[section.c_str()]);
		}

		return *current;
	}

	template <typename T>
	void WriteValue(const std::string& filename, const char* szSection, const char* szKey, const T& value)
	{
		rapidjson::Document& doc = get_json_document(filename);
		std::vector<std::string> sections = utilities::split_string(szSection, '/');

		rapidjson::Value& section = get_json_section_recursive(doc, doc.GetObject(), sections);

		auto it = section.FindMember(szKey);
		if (it != section.MemberEnd())
		{
			it->value = ToJsonValue(value, doc.GetAllocator());
		}
		else
		{
			rapidjson::Value key(szKey, doc.GetAllocator());
			rapidjson::Value jsonValue = ToJsonValue(value, doc.GetAllocator());
			section.AddMember(key, jsonValue, doc.GetAllocator());
		}
		write_json_config(filename);
	}

	template <typename T>
	T ReadValue(const std::string& filename, const char* szSection, const char* szKey, T defaultValue, bool readonly = false)
	{
		rapidjson::Document& doc = get_json_document(filename);
		std::vector<std::string> sections = utilities::split_string(szSection, '/');
		rapidjson::Value& section = get_json_section_recursive(doc, doc.GetObject(), sections, readonly);

		if (!section.HasMember(szKey) || !section[szKey].Is<T>())
		{
			if (!readonly)
			{
				rapidjson::Value key(szKey, doc.GetAllocator());
				rapidjson::Value jsonValue = ToJsonValue(defaultValue, doc.GetAllocator());

				if (!section.HasMember(szKey))
				{
					section.AddMember(key, jsonValue, doc.GetAllocator());
				}
				else
				{
					section[szKey] = jsonValue;
				}

				write_json_config(filename);
			}

			return defaultValue;
		}

		return section[szKey].Get<T>();
	}

	bool ReadBoolean(const char* szSection, const char* szKey, bool defaultValue, bool readonly, const std::string& filename)
	{
		return ReadValue<bool>(filename, szSection, szKey, defaultValue, readonly);
	}

	void WriteBoolean(const char* szSection, const char* szKey, bool value, const std::string& filename)
	{
		WriteValue<bool>(filename, szSection, szKey, value);
	}

	std::string ReadString(const char* szSection, const char* szKey, const std::string& defaultValue, bool readonly, const std::string& filename)
	{
		return ReadValue<std::string>(filename, szSection, szKey, defaultValue, readonly);
	}

	void WriteString(const char* szSection, const char* szKey, const std::string& value, const std::string& filename)
	{
		WriteValue<std::string>(filename, szSection, szKey, value);
	}

	int32_t ReadInteger(const char* szSection, const char* szKey, int defaultValue, bool readonly, const std::string& filename)
	{
		return ReadValue<int32_t>(filename, szSection, szKey, defaultValue, readonly);
	}

	void WriteInteger(const char* szSection, const char* szKey, int value, const std::string& filename)
	{
		WriteValue<int>(filename, szSection, szKey, value);
	}

	uint32_t ReadUnsignedInteger(const char* szSection, const char* szKey, uint32_t iDefaultValue, bool readonly, const std::string& filename)
	{
		return ReadValue<uint32_t>(filename, szSection, szKey, iDefaultValue, readonly);
	}

	void WriteUnsignedInteger(const char* szSection, const char* szKey, uint32_t iValue, const std::string& filename)
	{
		WriteValue<uint32_t>(filename, szSection, szKey, iValue);
	}

	int64_t ReadInteger64(const char* szSection, const char* szKey, int64_t iDefaultValue, bool readonly, const std::string& filename)
	{
		return ReadValue<int64_t>(filename, szSection, szKey, iDefaultValue, readonly);
	}

	void WriteInteger64(const char* szSection, const char* szKey, int64_t iValue, const std::string& filename)
	{
		WriteValue<int64_t>(filename, szSection, szKey, iValue);
	}

	uint64_t ReadUnsignedInteger64(const char* szSection, const char* szKey, uint64_t iDefaultValue, bool readonly, const std::string& filename)
	{
		return ReadValue<uint64_t>(filename, szSection, szKey, iDefaultValue, readonly);
	}

	void WriteUnsignedInteger64(const char* szSection, const char* szKey, uint64_t iValue, const std::string& filename)
	{
		WriteValue<int64_t>(filename, szSection, szKey, iValue);
	}

	// template <typename T>
	// T Read(const char* szSection, const char* szKey, T defaultValue, bool readonly, const std::string& filename)
	// {
	// 	return ReadValue<T>(filename, szSection, szKey, defaultValue, readonly);
	// }

	// template <typename T>
	// void Write(const char* szSection, const char* szKey, T value, const std::string& filename)
	// {
	// 	WriteValue<T>(filename, szSection, szKey, value);
	// }
}