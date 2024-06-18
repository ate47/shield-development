#pragma once
#include <string>

namespace utilities::json_config
{
	static std::string DEFAULTJSON = "project-bo4.json";

	bool ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue, bool readonly = false, const std::string& filename = DEFAULTJSON);
	void WriteBoolean(const char* szSection, const char* szKey, bool bolValue, const std::string& filename = DEFAULTJSON);

	std::string ReadString(const char* szSection, const char* szKey, const std::string& strDefaultValue, bool readonly = false, const std::string& filename = DEFAULTJSON);
	void WriteString(const char* szSection, const char* szKey, const std::string& strValue, const std::string& filename = DEFAULTJSON);

	int32_t ReadInteger(const char* szSection, const char* szKey, int32_t iDefaultValue, bool readonly = false, const std::string& filename = DEFAULTJSON);
	void WriteInteger(const char* szSection, const char* szKey, int32_t iValue, const std::string& filename = DEFAULTJSON);
	uint32_t ReadUnsignedInteger(const char* szSection, const char* szKey, uint32_t iDefaultValue, bool readonly = false, const std::string& filename = DEFAULTJSON);
	void WriteUnsignedInteger(const char* szSection, const char* szKey, uint32_t iValue, const std::string& filename);

	int64_t ReadInteger64(const char* szSection, const char* szKey, int64_t iDefaultValue, bool readonly = false, const std::string& filename = DEFAULTJSON);
	void WriteInteger64(const char* szSection, const char* szKey, int64_t iValue, const std::string& filename = DEFAULTJSON);
	uint64_t ReadUnsignedInteger64(const char* szSection, const char* szKey, uint64_t iDefaultValue, bool readonly = false, const std::string& filename = DEFAULTJSON);
	void WriteUnsignedInteger64(const char* szSection, const char* szKey, uint64_t iValue, const std::string& filename = DEFAULTJSON);

	// template <typename T>
	// T Read(const char* szSection, const char* szKey, T defaultValue, bool readonly = false, const std::string& filename = DEFAULTJSON);

	// template <typename T>
	// void Write(const char* szSection, const char* szKey, T value, const std::string& filename = DEFAULTJSON);

	// template void Write<int>(const char* szSection, const char* szKey, int value, const std::string& filename);
	// template void Write<double>(const char* szSection, const char* szKey, double value, const std::string& filename);
}
