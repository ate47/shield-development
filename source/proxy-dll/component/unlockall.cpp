#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "definitions/game.hpp"
#include "definitions/variables.hpp"
#include "command.hpp"
#include "dvars.hpp"

#include <utilities/hook.hpp>
#include <utilities/string.hpp>
#include <utilities/json_config.hpp>
#include <utilities/tools.hpp>

namespace game
{
	WEAK symbol<int(const int a1)> LobbySession_GetControllingLobbySession{0x1439066F0_g};
	WEAK symbol<int(const int LobbyModule, const int LobbyType)> LobbySession_GetLobbyMode{0x143906830_g};
	WEAK symbol<int(const game::ControllerIndex_t controllerIndex, const int statsLocation)> LiveStorage_DoWeHaveStats{0x1438B79D0_g};
	WEAK symbol<int(game::eModes mode, const game::ControllerIndex_t controllerIndex)> LiveStats_GetPrestige{0x1438A7390_g};
	WEAK symbol<int(game::eModes mode, const game::ControllerIndex_t controllerIndex)> LiveStats_GetRank{0x1438A7430_g};
	WEAK symbol<int(game::eModes mode, const game::ControllerIndex_t controllerIndex)> LiveStats_GetXp{0x1438A78B0_g};
	WEAK symbol<int(game::eModes mode, const game::ControllerIndex_t controllerIndex, int playerStatsKeyIndex_t)> LiveStats_GetIntPlayerStatByKey{0x1438A66A0_g};
	WEAK symbol<int(game::eModes mode, const int Xp)> CL_Rank_GetRankForXP{0x1422EF900_g};
	WEAK symbol<int(game::eModes mode, const int Xp)> CL_Rank_GetParagonRankForXP{0x1422EF5A0_g};

	WEAK symbol<bool(const int64_t mode, const int64_t a2)> CL_IsWeaponMasteryChallenge{0x1427EAD70_g};
	WEAK symbol<bool(game::eModes mode, unsigned int a2, uint16_t a3, unsigned int a4)> CheckPrerequisiteChallengeComplete{0x1438A5E50_g};
	WEAK symbol<int(game::eModes mode, uint16_t a2)> CL_GetChallengeRowByIndex{0x1427EA380_g};
	WEAK symbol<__int64*(__int64* a1, int a2, __int64 a3, int a4)> CL_GetChallengeStatName{0x1427EA440_g};
	WEAK symbol<__int64(unsigned int a1)> sub_1428A2740{0x1428A2740_g};
	WEAK symbol<void*(__int64 a1, __int64* a2, int a3, int a4)> StringTable_GetColumnValueForRow{0x1428A1F50_g};
	WEAK symbol<void*(__int64 a1, bool a2, int a3, int a4)> sub_1428A2340{0x1428A2340_g};

	WEAK game::symbol<int(__int64 a1)> LobbyActiveList_GetClientInfo{0x1438FF090_g};
	WEAK game::symbol<bool(game::eModes mode)> Com_SessionMode_IsMode{0x14289F1C0_g};

	WEAK symbol<bool(game::eModes mode)> Com_SessionMode_SupportsParagon{0x14289F4B0_g};
	WEAK symbol<int(game::eModes mode)> CL_Rank_GetPrestigeCap{0x1422EF690_g};
	WEAK symbol<int(int clientNum)> G_GetClientPrestige{0x142CE9410_g};
	WEAK symbol<int(int clientNum, int rank)> G_SetClientParagonRank{0x142D23B60_g};

	enum playerStatKeys : int
	{
		XP = 0,
		PLEVEL = 1,
		PARAGONXP = 4,
		VERSION = 17,
		KILLS = 18,
		WINS = 32,
	};
}

namespace unlockall
{
	namespace
	{
		int UPDATE_FREQ = 1; // recheck dvar every nth call
		bool unlock_dvars_enabled;
		std::unordered_map<std::string, bool> unlock;
		dvars::DvarContainer DvarContainer;

		utilities::hook::detour bg_emblemisentitlementbackgroundgranted_hook;
		utilities::hook::detour bg_unlockablesemblemorbackinglockedbychallenge_hook;
		utilities::hook::detour bg_unlockablesgetcustomclasscount_hook;
		utilities::hook::detour bg_unlockablesisattachmentslotlocked_hook;
		utilities::hook::detour bg_unlockablesisitemattachmentlocked_hook;
		utilities::hook::detour bg_unlockablesisitemlocked_hook;
		utilities::hook::detour bg_unlockablesitemoptionlocked_hook;
		utilities::hook::detour bg_unlockedgetchallengeunlockedforindex_hook;
		utilities::hook::detour loot_getitemquantity_hook;

		utilities::hook::detour livestats_getxp_hook;
		utilities::hook::detour livestats_getrank_hook;
		utilities::hook::detour livestats_getprestige_hook;
		utilities::hook::detour livestats_getparagonrank_hook;
		utilities::hook::detour livestats_ismasterprestige_hook;
		utilities::hook::detour is_item_locked_for_challenge_hook;

		utilities::hook::detour bg_unlockablesismaxlevelfrombuffer_hook;
		utilities::hook::detour bg_unlockablescharactercustomizationitemlockedbychallenge_hook;
		utilities::hook::detour cl_ischallengelocked_hook;
		utilities::hook::detour gscr_isitempurchasedforclientnum_hook;

		uint32_t zm_loot_table[] =
		    {
		        1000001, // zm_bgb_ctrl_z
		        1000002, // zm_bgb_dead_of_nuclear_winter
		        1000003, // zm_bgb_in_plain_sight
		        1000004, // zm_bgb_licensed_contractor
		        1000005, // zm_bgb_phantom_reload
		        1000006, // zm_bgb_sword_flay
		        1000007, // zm_bgb_whos_keeping_score
		        1000008, // zm_bgb_alchemical_antithesis
		        1000009, // zm_bgb_blood_debt
		        1000010, // zm_bgb_extra_credit
		        1000011, // zm_bgb_immolation_liquidation
		        1000012, // zm_bgb_kill_joy
		        1000013, // zm_bgb_shields_up
		        1000014, // zm_bgb_free_fire
		        1000015, // zm_bgb_power_keg
		        1000016, // zm_bgb_undead_man_walking
		        1000017, // zm_bgb_wall_to_wall_clearance
		        1000018, // zm_bgb_cache_back
		        1000019, // zm_bgb_join_the_party
		        1000020, // zm_bgb_wall_power
		        1000021, // talisman_box_guarantee_box_only
		        1000022, // talisman_coagulant
		        1000023, // talisman_extra_claymore
		        1000024, // talisman_extra_frag
		        1000025, // talisman_extra_molotov
		        1000026, // talisman_extra_semtex
		        1000027, // talisman_impatient
		        1000028, // talisman_weapon_reducepapcost
		        1000029, // talisman_perk_reducecost_1
		        1000030, // talisman_perk_reducecost_2
		        1000031, // talisman_perk_reducecost_3
		        1000032, // talisman_perk_reducecost_4
		        1000033, // talisman_shield_price
		        1000034, // talisman_special_xp_rate
		        1000035, // talisman_start_weapon_smg
		        1000036, // talisman_box_guarantee_lmg
		        1000037, // talisman_extra_miniturret
		        1000038, // talisman_perk_start_1
		        1000039, // talisman_perk_start_2
		        1000040, // talisman_perk_start_3
		        1000041, // talisman_perk_start_4
		        1000042, // talisman_shield_durability_rare
		        1000043, // talisman_start_weapon_ar
		        1000044, // talisman_perk_permanent_1
		        1000045, // talisman_perk_permanent_2
		        1000046, // talisman_perk_permanent_3
		        1000047, // talisman_perk_permanent_4
		        1000048, // talisman_shield_durability_legendary
		        1000049, // talisman_special_startlv2
		        1000050, // talisman_start_weapon_lmg
		        1000051, // talisman_special_startlv3
		        1000052, // talisman_perk_mod_single
		        1000053, // zm_bgb_refresh_mint
		        1000054, // zm_bgb_perk_up
		        1000055, // zm_bgb_conflagration_liquidation
		        1000056, // zm_bgb_bullet_boost
		        1000057, // zm_bgb_talkin_bout_regeneration
		        1000058, // zm_bgb_dividend_yield
		        1000059, // zm_bgb_suit_up
		        1000060, // talisman_permanent_heroweap_armor
		        1000061, // talisman_extra_self_revive
		        1000062, // zm_bgb_perkaholic
		        1000063, // zm_bgb_near_death_experience
		        1000064, // zm_bgb_shopping_free
		        1000065, // zm_bgb_reign_drops
		        1000066, // zm_bgb_phoenix_up
		        1000067, // zm_bgb_head_drama
		        1000068, // zm_bgb_secret_shopper
		        1000069  // zm_bgb_power_vacuum
		};

		void init_dvars()
		{
			for (auto& pair : unlock)
			{
				const char* cmd = utilities::string::va("set shield_unlock_%s %i;", pair.first.c_str(), pair.second);
				game::Cbuf_AddText(0, cmd);
				// logger::write(logger::LOG_TYPE_INFO, cmd);
			}
		}

		void enable_unlock_hooks()
		{
			init_dvars();
			game::Cbuf_AddText(0, utilities::string::va("set allitemsunlocked  %i;", (unlock["all"] || unlock["itemoptions"])));
			unlock_dvars_enabled = true;
		}

		std::string get_moddvar_string(std::string dvarstring)
		{
			game::dvar_t* dvar = DvarContainer.get(dvarstring);
			return dvars::get_value_string(dvar, &dvar->value->current);
		}

		void update_unlock_toggle_from_dvars()
		{
			for (auto& pair : unlock)
			{
				unlock[pair.first.c_str()] = get_moddvar_string("shield_unlock_" + pair.second) == "1";
			}
		}

		void unlock_refresh_rate(const command::params& params)
		{
			if (params.size() < 2) return;
			std::string arg1 = params[1];
			if (!utilities::string::is_integer(arg1)) return;
			int num = std::stoi(arg1);
			UPDATE_FREQ = num;
		}

		int unlock_active(std::string key)
		{
			if (unlock_dvars_enabled)
			{
				if (get_moddvar_string("shield_unlock_all") == "1" ||
				    get_moddvar_string("shield_unlock_" + key) == "1")
				{
					return 1;
				}
			}
			else
			{
				if (unlock["all"] || unlock[key]) return 1;
			}
			return 0;
		}

		inline bool is_zm_loot(int item_id)
		{
			auto it = std::find(
			    std::begin(zm_loot_table), std::end(zm_loot_table), item_id);
			if (it != std::end(zm_loot_table))
			{
				return true;
			}
			return false;
		}

		int liveinventory_getitemquantity(const game::ControllerIndex_t controllerIndex, const int item_id)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			unlock_active_periodic.setThreshold(50);
			if (unlock_active_periodic("zm_loot"))
			{
				int result = is_zm_loot(item_id) ? 999 : 1;
				return result;
			}
			return loot_getitemquantity_hook.invoke<int>(controllerIndex, item_id);
		}

		bool bg_unlockedgetchallengeunlockedforindex(game::eModes mode, const game::ControllerIndex_t controllerIndex, uint16_t index, int32_t itemIndex)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			unlock_active_periodic.setThreshold(UPDATE_FREQ);
			if (unlock_active_periodic("challenges")) return true;
			return bg_unlockedgetchallengeunlockedforindex_hook.invoke<bool>(mode, controllerIndex, index, itemIndex);
		}

		bool bg_unlockablesitemoptionlocked(game::eModes mode, const game::ControllerIndex_t controllerIndex, int itemIndex, int32_t optionIndex)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			if (unlock_active_periodic("itemoptions")) return false;
			return bg_unlockablesitemoptionlocked_hook.invoke<bool>(mode, controllerIndex, itemIndex, optionIndex);
		}

		bool bg_unlockablesisitemattachmentlocked(game::eModes mode, const game::ControllerIndex_t controllerIndex, int32_t itemIndex, int32_t attachmentNum)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			if (unlock_active_periodic("attachments")) return false;
			return bg_unlockablesisitemattachmentlocked_hook.invoke<bool>(mode, controllerIndex, itemIndex, attachmentNum);
		}

		bool bg_unlockablesisattachmentslotlocked(game::eModes mode, const game::ControllerIndex_t controllerIndex, int32_t itemIndex, int32_t attachmentSlotIndex)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			if (unlock_active_periodic("attachmentslot")) return false;
			return bg_unlockablesisattachmentslotlocked_hook.invoke<bool>(mode, controllerIndex, itemIndex, attachmentSlotIndex);
		}

		bool gscr_isitempurchasedforclientnum_stub([[maybe_unused]] unsigned int clientNum, [[maybe_unused]] int itemIndex)
		{
			return true;
		}

		bool bg_unlockablesemblemorbackinglockedbychallenge(game::eModes mode, const game::ControllerIndex_t controllerIndex, void* challengeLookup, bool otherPlayer)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			if (unlock_active_periodic("emblems")) return false;
			return bg_unlockablesemblemorbackinglockedbychallenge_hook.invoke<bool>(mode, controllerIndex, challengeLookup, otherPlayer);
		}

		bool bg_emblemisentitlementbackgroundgranted(const game::ControllerIndex_t controllerIndex, uint16_t backgroundId)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			if (unlock_active_periodic("classes")) return true;
			return bg_emblemisentitlementbackgroundgranted_hook.invoke<bool>(controllerIndex, backgroundId);
		}

		bool bg_unlockablesisitemlocked(game::eModes mode, const game::ControllerIndex_t controllerIndex, int32_t itemIndex)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			if (unlock_active_periodic("items")) return false;
			return bg_unlockablesisitemlocked_hook.invoke<bool>(mode, controllerIndex, itemIndex);
		}

		int is_item_locked_for_challenge(void* a1, int a2, int64_t a3, int a4, int a5) // unlock camos + more
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			if (unlock_active_periodic("items")) return false;
			return is_item_locked_for_challenge_hook.invoke<int>(a1, a2, a3, a4, a5);
		}

		int32_t bg_unlockablesgetcustomclasscount(game::eModes mode, const game::ControllerIndex_t controllerIndex)
		{
			static utilities::tools::PeriodicUpdate<int, std::string> unlock_active_periodic(unlock_active, UPDATE_FREQ);
			if (unlock_active_periodic("classes")) return 12;
			return bg_unlockablesgetcustomclasscount_hook.invoke<int>(mode, controllerIndex);
		}

		void unlock_func(const command::params& params)
		{
			bool TF = true;
			if (params.size() < 2) return;

			std::string arg1 = params[1];

			if (params.size() == 3)
			{
				std::string arg2 = params[2];
				TF = (arg2 == "true" || arg2 == "True" || arg2 == "1");
			}
			unlock[arg1] = TF;

			utilities::json_config::WriteBoolean("unlock", arg1.c_str(), TF);
			const char* cmd = utilities::string::va("set shield_unlock_%s %i;", arg1.c_str(), TF);
			game::Cbuf_AddText(0, cmd);

			if (arg1 == "all" || arg1 == "itemoptions")
			{
				game::Cbuf_AddText(0, utilities::string::va("set allitemsunlocked %i;", TF));
			}
		}

		void print_livestat()
		{
			game::eModes mode = game::Com_SessionMode_GetMode();
			const auto controller = game::ControllerIndex_t::CONTROLLER_INDEX_FIRST;

			int prestige = game::LiveStats_GetPrestige(mode, controller);
			int rank = game::LiveStats_GetRank(mode, controller);
			logger::write(logger::LOG_TYPE_INFO, "player rank(%i) prestige(%i)", rank, prestige);
		}

		int getstat(int keyIndex)
		{
			int IntPlayerStat = 0;
			if (!game::Com_SessionMode_IsMode(game::eModes::MODE_INVALID))
			{
				const auto Mode = game::Com_SessionMode_GetMode();
				const auto controller = game::ControllerIndex_t::CONTROLLER_INDEX_FIRST;

				if ((game::LiveStorage_DoWeHaveStats(controller, 0) & 1) != 0)
				{
					IntPlayerStat = game::LiveStats_GetIntPlayerStatByKey(Mode, controller, keyIndex);
				}
			}
			return IntPlayerStat;
		}

		void print_paragon()
		{
			game::eModes mode = game::Com_SessionMode_GetMode();
			const auto controller = game::ControllerIndex_t::CONTROLLER_INDEX_FIRST;

			int xp = getstat(game::playerStatKeys::PARAGONXP);
			int rank = game::CL_Rank_GetParagonRankForXP(mode, xp);
			int prestige = game::G_GetClientPrestige(0);
			int cap = game::CL_Rank_GetPrestigeCap(mode);

			logger::write(logger::LOG_TYPE_INFO, "player xp(%i) rank(%i) prestige(%i) cap(%i)", xp, rank, prestige, cap);
		}

		void printstat(const command::params& params)
		{
			if (params.size() < 2) return;
			std::string numstr = params[1];
			int num = std::stoi(numstr);
			
			const int stat = getstat(num);
			logger::write(logger::LOG_TYPE_INFO, utilities::string::va("STAT %i: %i", num, stat));
		}

		int livestats_getxp_stub(game::eModes mode, const game::ControllerIndex_t controllerIndex)
		{
			const int xp = getstat(game::playerStatKeys::XP);
			return xp;
		}

		int livestats_getrank_stub(game::eModes mode, const game::ControllerIndex_t controllerIndex)
		{
			const int xp = getstat(game::playerStatKeys::XP);
			const int rank = game::CL_Rank_GetRankForXP(mode, xp);	
			return rank;
		}

		int livestats_getparagonrank_stub(game::eModes mode, const game::ControllerIndex_t controllerIndex)
		{
			const int xp = getstat(game::playerStatKeys::PARAGONXP);
			const int rank = game::CL_Rank_GetParagonRankForXP(mode, xp);
			return rank;
		}

		int livestats_getprestige_stub(game::eModes mode, const game::ControllerIndex_t controllerIndex)
		{
			return getstat(game::playerStatKeys::PLEVEL);
		}

		bool livestats_ismasterprestige_stub(game::eModes mode, const game::ControllerIndex_t controllerIndex)
		{
			if (game::G_GetClientPrestige(controllerIndex) >= (game::CL_Rank_GetPrestigeCap(mode) - 1))
			{
				return true;
			}
			return livestats_ismasterprestige_hook.invoke<bool>(mode, controllerIndex);
		}

		bool cl_ischallengelocked_stub(__int64 a1, __int64 a2, int a3, int a4)
		{
			return false;
		}

		bool sub_1438AE2F0_stub(int a1)
		{
			// const auto res = utilities::hook::invoke<bool>(0x1438AE2F0_g, a1); // sub_1438AE2F0 returns false
			return true;
		}
	}

	namespace test
	{
		// TODO check sub_1438A3C20 for online stat? make true?

		void printrank()
		{
			const auto mode = game::Com_SessionMode_GetMode();
			const int xp = getstat(game::playerStatKeys::XP);
			const int rank = game::CL_Rank_GetRankForXP(mode, xp) + 1;
			const int plevel = getstat(game::playerStatKeys::PLEVEL);

			logger::write(logger::LOG_TYPE_INFO, utilities::string::va("LUI STATS: xp %i rank %i prestige %i", xp, rank, plevel));
		}

		BYTE gscr_checkweaponmastery_stub(unsigned int a1, int a2, uint16_t a3, uint64_t* a4, int a5)
		{
			return 1;
		}

		bool bg_unlockablescharactercustomizationitemlockedbychallenge(game::eModes mode, const game::ControllerIndex_t controllerIndex, uint32_t characterIndex, int itemType, int itemIndex, int a6)
		{
			return false;
		}

		int sub_1406B2730_stub(unsigned int a1, unsigned int a2)
		{
			return 0;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			unlock["all"] = utilities::json_config::ReadBoolean("unlock", "all", false);
			unlock["attachments"] = utilities::json_config::ReadBoolean("unlock", "attachments", false);
			unlock["attachmentslot"] = utilities::json_config::ReadBoolean("unlock", "attachmentslot", false);
			unlock["backgrounds"] = utilities::json_config::ReadBoolean("unlock", "backgrounds", false);
			unlock["challenges"] = utilities::json_config::ReadBoolean("unlock", "challenges", false);
			unlock["classes"] = utilities::json_config::ReadBoolean("unlock", "classes", false);
			unlock["emblems"] = utilities::json_config::ReadBoolean("unlock", "emblems", false);
			unlock["itemoptions"] = utilities::json_config::ReadBoolean("unlock", "itemoptions", false);
			unlock["items"] = utilities::json_config::ReadBoolean("unlock", "items", false);
			unlock["zm_loot"] = utilities::json_config::ReadBoolean("unlock", "zm_loot", false);


			bg_emblemisentitlementbackgroundgranted_hook.create(0x144184D20_g, bg_emblemisentitlementbackgroundgranted);
			bg_unlockablesemblemorbackinglockedbychallenge_hook.create(0x1406AC010_g, bg_unlockablesemblemorbackinglockedbychallenge);
			bg_unlockablesgetcustomclasscount_hook.create(0x1406ae060_g, bg_unlockablesgetcustomclasscount);
			bg_unlockablesisattachmentslotlocked_hook.create(0x1406B3290_g, bg_unlockablesisattachmentslotlocked);
			bg_unlockablesisitemattachmentlocked_hook.create(0x1406B34D0_g, bg_unlockablesisitemattachmentlocked);
			bg_unlockablesisitemlocked_hook.create(0x1406B3AA0_g, bg_unlockablesisitemlocked);
			bg_unlockablesitemoptionlocked_hook.create(0x1406B5530_g, bg_unlockablesitemoptionlocked);
			bg_unlockedgetchallengeunlockedforindex_hook.create(0x1406BB410_g, bg_unlockedgetchallengeunlockedforindex);
			loot_getitemquantity_hook.create(0x1437F6ED0_g, liveinventory_getitemquantity);

			// utilities::hook::call(0x1438A78E7_g, sub_1438AE2F0_stub);
			livestats_getxp_hook.create(0x1438A78B0_g, livestats_getxp_stub);
			livestats_getrank_hook.create(0x1438A7430_g, livestats_getrank_stub);
			livestats_getprestige_hook.create(0x1438A7390_g, livestats_getprestige_stub);
			livestats_getparagonrank_hook.create(0x1438A70E0_g, livestats_getparagonrank_stub);
			// livestats_ismasterprestige_hook.create(0x144186730_g, livestats_ismasterprestige_stub);

			{
				// is_item_locked_for_challenge_hook.create(0x1431976E0_g, is_item_locked_for_challenge);
				// gscr_isitempurchasedforclientnum_hook.create(0x14318EF80_g, gscr_isitempurchasedforclientnum_stub);
				// bg_unlockablesismaxlevelfrombuffer_hook.create(0x1406B4760_g, test::bg_unlockablesismaxlevelfrombuffer);
				// bg_unlockablescharactercustomizationitemlockedbychallenge_hook.create(0x1406AA190_g, test::bg_unlockablescharactercustomizationitemlockedbychallenge);
				// utilities::hook::call(0x1431933B0_g, test::gscr_checkweaponmastery_stub);
				// cl_ischallengelocked_hook.create(0x1427EAD40_g, test::cl_ischallengelocked_stub);
			}

			command::add("unlock", [&](const command::params& params) { unlock_func(params); });
			command::add("enable_unlock_hooks", enable_unlock_hooks);
			// command::add("unlock_refresh_rate", unlock_refresh_rate);
			// command::add("update_unlock_toggle_from_dvars", update_unlock_toggle_from_dvars);
			command::add("printstat", [&](const command::params& params) { printstat(params); });
			command::add("printlivestat", print_livestat);
			command::add("print_paragon", print_paragon);
			command::add("printrank", test::printrank);
		}
	};
}

REGISTER_COMPONENT(unlockall::component)
