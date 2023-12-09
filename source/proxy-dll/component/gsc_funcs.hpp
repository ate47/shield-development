#pragma once
#include <std_include.hpp>
#include "definitions/game.hpp"

namespace gsc_funcs
{
	constexpr auto serious_custom_func_name = "SeriousCustom";

	extern bool enable_dev_func;

	uint32_t canon_hash(const char* str);

	constexpr uint64_t custom_error_id = 0x42693201;

	void gsc_error(const char* message, game::scriptInstance_t inst, bool terminal, ...);
}