#pragma once

namespace fnv1a
{
	constexpr uint64_t generate_hash_const(const char* string, uint64_t start = 0xCBF29CE484222325, uint64_t prime = 0x100000001B3)
	{
		uint64_t res = start;

		for (const char* c = string; *c; c++)
		{
			if (*c == '\\')
			{
				res ^= '/';
			}
			else if (*c >= 'A' && *c <= 'Z')
			{
				res ^= *c - 'A' + 'a';
			}
			else
			{
				res ^= *c;
			}

			res *= prime;
		}

		return res & 0x7FFFFFFFFFFFFFFF;
	}
	uint64_t generate_hash(const char* string, uint64_t start = 0xCBF29CE484222325);

	uint64_t generate_hash_pattern(const char* string);
}

namespace variables
{
	struct varInfo
	{
		std::string name;
		std::string desc;
		uint64_t fnv1a;
	};

	struct varEntry : varInfo
	{
		uintptr_t pointer = 0;
	};

	extern std::vector<varEntry> dvars_table;
	extern std::vector<varEntry> commands_table;

	std::vector<const char*> get_dvars_list();
	std::vector<const char*> get_commands_list();
}

namespace runtime_errors
{
	constexpr uint64_t custom_error_id = 0x42693201;
	const char* get_error_message(uint64_t code);
}