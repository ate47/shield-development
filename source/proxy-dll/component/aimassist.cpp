#include <std_include.hpp>
#include "definitions/game.hpp"
#include "loader/component_loader.hpp"

#include "command.hpp"
#include "dvars.hpp"

#include <utilities/hook.hpp>
#include <utilities/string.hpp>
#include <utilities/json_config.hpp>

namespace aimassist
{
	namespace
	{
		utilities::hook::detour aimassist_init_hook;

		static uint8_t patch_dvar_true[] = {0x41, 0xB0, 0x01};
		static uint8_t patch_dvar_false[] = {0x45, 0x33, 0xC0};
		std::unordered_map<std::string, bool> dvar;

		void toggle_aimassist(uint8_t val)
		{
			if (val != 0x00 || val != 0x01) { val = 0x01; }
			uint8_t aimassist_mp[] = {0xB2, val};
			utilities::hook::set<uint8_t>(0x14055CCA8_g + 2, val); // 0x52A4767BD6DA84F1 default value
			utilities::hook::set<uint8_t>(0x14055CCBE_g + 2, val); // 0x4C6B94016130324D default value
			utilities::hook::copy(0x14055CCD8_g, aimassist_mp, 2); // 0x113C416D28FD0CE2 non ads aim assist for mp
			// utilities::hook::set<uint8_t>(0x14055CCD8_g + 1, 0x00); // 0x113C416D28FD0CE2 default value
		}

		void patch_aim_dvar(std::string dvarname)
		{
			if (dvarname == "aim_autoaim_enabled") utilities::hook::copy(0x14055C955_g, dvar[dvarname] ? patch_dvar_true : patch_dvar_false, 3);
			if (dvarname == "aim_lockon_enabled") utilities::hook::copy(0x14055CAB3_g, dvar[dvarname] ? patch_dvar_true : patch_dvar_false, 3);
			if (dvarname == "aim_slowdown_enabled") utilities::hook::copy(0x14055CB88_g, dvar[dvarname] ? patch_dvar_true : patch_dvar_false, 3);
			if (dvarname == "aim_target_closest_first") utilities::hook::copy(0x14055CB4C_g, dvar[dvarname] ? patch_dvar_true : patch_dvar_false, 3);
		}

		void patch_aim_dvars()
		{
			for (auto& pair : dvar)
			{
				patch_aim_dvar(pair.first);
			}
		}

		void init_dvars()
		{
			for (auto& pair : dvar)
			{
				const char* cmd = utilities::string::va("set shield_%s %i;", pair.first.c_str(), pair.second);
				game::Cbuf_AddText(0, cmd);
			}
		}

		void aimassist(const command::params& params)
		{
			bool TF = true;
			if (params.size() < 2) return;

			std::string arg1 = params[1];

			if (params.size() == 3)
			{
				std::string arg2 = params[2];
				TF = (arg2 == "true" || arg2 == "True" || arg2 == "1");
			}
			dvar[arg1] = TF;

			utilities::json_config::WriteInteger("dvars", arg1.c_str(), TF);
			const char* cmd = utilities::string::va("set shield_%s %i;", arg1.c_str(), TF);
			game::Cbuf_AddText(0, cmd);

			patch_aim_dvar(arg1);
			if (arg1 == "aim_autoaim_enabled") toggle_aimassist((uint8_t)TF);
		}

		void update_aimassist()
		{
			logger::write(logger::LOG_TYPE_INFO, "Updating AimAssist");
			aimassist_init_hook.invoke<int>(0);
			// utilities::hook::invoke<int>(0x14055C910_g); //crash AimAssist_RegisterDvars
		}

		void aimassist_init_stub(int localclientnum)
		{
			aimassist_init_hook.invoke<int>(0);
			init_dvars();
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvar["aim_autoaim_enabled"] = utilities::json_config::ReadInteger("dvars", "aim_autoaim_enabled", 1);
			dvar["aim_lockon_enabled"] = utilities::json_config::ReadInteger("dvars", "aim_lockon_enabled", 1);
			dvar["aim_slowdown_enabled"] = utilities::json_config::ReadInteger("dvars", "aim_slowdown_enabled", 1);
			dvar["aim_target_closest_first"] = utilities::json_config::ReadInteger("dvars", "aim_target_closest_first", 0);

			patch_aim_dvars();
			toggle_aimassist((uint8_t)dvar["aim_autoaim_enabled"]);

			command::add("aimassist", [&](const command::params& params){ aimassist(params); });
			command::add("aa", update_aimassist);

			aimassist_init_hook.create(0x14054CE50_g, aimassist_init_stub);
		}
	};
}

REGISTER_COMPONENT(aimassist::component)
