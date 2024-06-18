#pragma once
#include "component/gsc_funcs.hpp"

namespace dynamic_data
{
	enum field_type : uint64_t
	{
		FT_UINT = 0,
		FT_INT,
		FT_FLOAT,
		FT_ENUM,
		FT_STRING,
		FT_XHASH,
		FT_COUNT
	};

	constexpr uint64_t named_type(field_type type, const char* name)
	{
		return type | ((uint64_t)gsc_funcs::canon_hash_const(name) << 32);
	}

	constexpr std::pair<field_type, uint32_t> get_named_type(uint64_t type)
	{
		return std::make_pair<field_type, uint32_t>((field_type)(type & (~0ull >> 32)), type >> 32);
	}

	struct field
	{
		uint32_t canon_name;
		uint64_t type{};
		size_t size{};
		size_t array_size{};
		size_t offset{};
	};

	struct enum_type
	{
		uint32_t type_name;
		uint64_t field_name;
		int64_t value;
	};

	struct dd_container
	{
		const field* fields;
		size_t field_count;
		const enum_type* enums;
		size_t enum_count;

		void load_dd(rapidjson::Value& object, void* buffer, void* mask) const;
	};
}