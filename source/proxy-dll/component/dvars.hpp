#pragma once
#include "definitions/game.hpp"

namespace dvars
{
	std::string get_value_string(const game::dvar_t* dvar, game::DvarValue* value = nullptr);
	bool get_value_bool(const game::dvar_t* dvar, game::DvarValue* value = nullptr);
	std::string get_domain_string(const game::dvarType_t type, const game::DvarLimits& domain);

	game::DvarData* get_session_mode_dvar(const game::dvar_t* dvar);

	game::dvar_t* find_dvar(uint64_t hashRef);
	game::dvar_t* find_dvar(const char* nameRef);
	game::dvar_t* find_dvar(const std::string& nameRef);

	class DvarContainer
	{
	private:
		std::unordered_map<std::string, game::dvar_t*> dvar;
	public:
		game::dvar_t*& get(const std::string& key);
	};
}
