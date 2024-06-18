#pragma once

namespace weapons
{
	void* get_or_patch_weapon(void* weapon_def);
	void* get_weapon_asset(uint64_t name);
	void clear_weapon_defs();
	bool load_weapon_def(rapidjson::Value& definition, const char* mod_name, const std::filesystem::path& mod_path);
}