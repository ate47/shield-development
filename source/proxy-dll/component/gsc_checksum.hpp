#pragma once
#include "definitions/game.hpp"

namespace gsc::checksum
{
	uint32_t get_script_checksum(game::scriptInstance_t inst);
	void compute_script_checksum(game::scriptInstance_t inst);
}