#pragma once
#include "definitions/game.hpp"

namespace gsc_custom
{
	constexpr uint32_t linking_error = 1670707254;
	constexpr uint16_t lazylink_opcode = 0x16;
	constexpr uint16_t shield_devblock_opcode = 0x21;

	enum gsic_field_type
	{
		GSIC_FIELD_DETOUR = 0
	};

	struct gsic_detour
	{
		uint32_t fixup_name{};
		uint32_t replace_namespace{};
		uint32_t replace_function{};
		uint32_t fixup_offset{};
		uint32_t fixup_size{};
		uint64_t target_script{};
		byte* fixup_function{};
	};

	struct gsic_info
	{
		bool sync{};
		bool dev_blocks{};
		std::vector<gsic_detour> detours{};
	};

	void sync_gsic(game::scriptInstance_t inst, gsic_info& info);
	void find_linking_issues();
}