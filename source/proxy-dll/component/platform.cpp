#include <std_include.hpp>
#include "platform.hpp"
#include "definitions/game.hpp"
#include "loader/component_loader.hpp"

#include <utilities/hook.hpp>
#include <utilities/string.hpp>
#include <demonware/byte_buffer.hpp>
#include <utilities/info_string.hpp>
#include <utilities/identity.hpp>
#include <utilities/json_config.hpp>
#include <utilities/cryptography.hpp>
#include <WinReg.hpp>

#include "command.hpp"

namespace auth
{
	namespace
	{
		std::string get_hdd_serial()
		{
			DWORD serial{};
			if (!GetVolumeInformationA("C:\\", nullptr, 0, &serial, nullptr, nullptr, nullptr, 0))
			{
				return {};
			}

			return utilities::string::va("%08X", serial);
		}

		std::string get_key_entropy()
		{
			std::string entropy{};
			entropy.append(get_hdd_serial());

			if (entropy.empty())
			{
				logger::write(logger::LOG_TYPE_INFO, "generating random xuid");
				entropy.resize(32);
				utilities::cryptography::random::get_data(entropy.data(), entropy.size());
			}

			return entropy;
		}

		utilities::cryptography::ecc::key& get_key()
		{
			static auto key = utilities::cryptography::ecc::generate_key(512, get_key_entropy());
			return key;
		}

		bool is_second_instance()
		{
			static const auto is_first = []
			{
				static utilities::nt::handle mutex = CreateMutexA(nullptr, FALSE, "shield_mutex");
				return mutex && GetLastError() != ERROR_ALREADY_EXISTS;
			}();

			return !is_first;
		}

		uint64_t get_guid()
		{
			static const auto guid = []() -> uint64_t
			{
				uint64_t hash = utilities::cryptography::xxh32::compute(get_key().get_public_key());
				if (is_second_instance())
				{
					logger::write(logger::LOG_TYPE_INFO, "second instance: generating random xuid");
					return hash + 1; // return 0x11000010 | (hash & ~0x80000000);
				}

				return hash;
			}();

			return guid;
		}
	}
}

namespace platform
{
	uint64_t bnet_get_userid()
	{
		static uint64_t userid = 0;
		if (!userid)
		{
			uint32_t default_xuid = auth::get_guid();
			userid = utilities::json_config::ReadUnsignedInteger64("identity", auth::is_second_instance() ? "xuid2" : "xuid", default_xuid, true);
		}

		return userid;
	}

	const char* bnet_get_username()
	{
		static std::string username{};
		if (username.empty())
		{
			std::string default_name = utilities::identity::get_sys_username();
			username = utilities::json_config::ReadString("identity", "name", default_name, auth::is_second_instance() ? true : false);
			if (auth::is_second_instance())
			{
				username += "(2)";
			}
		}

		return username.data();
	}

	std::string get_userdata_directory()
	{
		return std::format("players/bnet-{}", bnet_get_userid());
	}

	namespace
	{
		utilities::hook::detour PC_TextChat_Print_Hook;
		void PC_TextChat_Print_Stub(const char* text)
		{
#ifdef DEBUG
			logger::write(logger::LOG_TYPE_DEBUG, "PC_TextChat_Print(%s)", text);
#endif
		}
#ifdef ONLINEDLL
#endif

		void check_platform_registry()
		{
			winreg::RegKey key;
			winreg::RegResult result = key.TryOpen(HKEY_CURRENT_USER, L"SOFTWARE\\Blizzard Entertainment\\Battle.net");
			if (!result)
			{
				MessageBoxA(nullptr, "You need to have BlackOps4 from Battle.Net to use this product...", "Error", MB_ICONWARNING);
				ShellExecuteA(nullptr, "open", "http://battle.net/", nullptr, nullptr, SW_SHOWNORMAL);
				logger::write(logger::LOG_TYPE_INFO, "[ PLATFORM ]: Couldnt find Battle.Net Launcher; Shutting down...");

				TerminateProcess(GetCurrentProcess(), 1);
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void pre_start() override
		{
			check_platform_registry();
		}

		void post_unpack() override
		{
			utilities::hook::set<uint16_t>(0x1423271D0_g, 0x01B0);     // BattleNet_IsDisabled (patch to mov al,1)
			utilities::hook::set<uint32_t>(0x1423271E0_g, 0x90C301B0); // BattleNet_IsConnected (patch to mov al,1 retn)

			utilities::hook::set<uint8_t>(0x142325210_g, 0xC3);        // patch#1 Annoying function crashing game; related to BattleNet (TODO : Needs Further Investigation)
			utilities::hook::set<uint8_t>(0x142307B40_g, 0xC3);        // patch#2 Annoying function crashing game; related to BattleNet (TODO : Needs Further Investigation)
			utilities::hook::set<uint32_t>(0x143D08290_g, 0x90C301B0); // patch#3 BattleNet_IsModeAvailable? (patch to mov al,1 retn)

			utilities::hook::nop(0x1437DA454_g, 13);       // begin cross-auth even without platform being initialized [LiveConnect_BeginCrossAuthPlatform]
			utilities::hook::set(0x1444D2D60_g, 0xC301B0); // Auth3 Response RSA signature check [bdAuth::validateResponseSignature]
			utilities::hook::set(0x1444E34C0_g, 0xC301B0); // Auth3 Response platform extended data check [bdAuthPC::processPlatformData]

			utilities::hook::nop(0x1438994E9_g, 22); // get live name even without platform being initialized [Live_UserSignedIn]
			utilities::hook::nop(0x1438C3476_g, 22); // get live xuid even without platform being initialized [LiveUser_UserGetXuid]

			utilities::hook::jump(0x142325C70_g, bnet_get_username); // detour battlenet username
			utilities::hook::jump(0x142325CA0_g, bnet_get_userid);   // detour battlenet userid

			// PC_TextChat_Print_Hook.create(0x1422D4A20_g, PC_TextChat_Print_Stub); // Disable useless system messages passed into chat box
#ifdef ONLINEDLL
#endif
			logger::write(logger::LOG_TYPE_INFO, "[ PLATFORM ]: BattleTag: '%s', BattleID: '%llu'", bnet_get_username(), bnet_get_userid());
		}
	};
}

    REGISTER_COMPONENT(platform::component)