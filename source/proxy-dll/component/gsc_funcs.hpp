#pragma once
#include <std_include.hpp>
#include "definitions/game.hpp"

namespace gsc_funcs
{
	constexpr auto serious_custom_func_name = "SeriousCustom";

	extern bool enable_dev_func;

	constexpr uint32_t canon_hash_const(const char* str)
	{
		uint32_t hash = 0x4B9ACE2F;

		for (const char* data = str; *data; data++)
		{
			char c = *data >= 'A' && *data <= 'Z' ? (*data - 'A' + 'a') : *data;
			hash = ((c + hash) ^ ((c + hash) << 10)) + (((c + hash) ^ ((c + hash) << 10)) >> 6);
		}

		uint32_t val = 0x8001 * ((9 * hash) ^ ((9 * hash) >> 11));

		return val;
	}

	uint32_t canon_hash(const char* str);
	uint32_t canon_hash_pattern(const char* str);
	
	void gsc_error(const char* message, game::scriptInstance_t inst, bool terminal, ...);
	void ScrVm_AddToArrayIntIndexed(game::scriptInstance_t inst, uint64_t index);
}