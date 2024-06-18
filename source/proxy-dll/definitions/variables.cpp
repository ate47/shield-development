#include <std_include.hpp>
#include "variables.hpp"
#include "game.hpp"
#include "xassets.hpp"
#include "component/hashes.hpp"

namespace fnv1a
{
	uint64_t generate_hash(const char* string, uint64_t start)
	{
		uint64_t val = generate_hash_const(string, start);

		if (start == 0xCBF29CE484222325)
		{
			hashes::add_hash(val, string);
		}

		return val;
	}

	uint64_t generate_hash_pattern(const char* string)
	{
		std::string_view v{ string };

		// basic notations hash_123, file_123, script_123
		if (!v.rfind("hash_", 0)) return std::strtoull(&string[5], nullptr, 16) & 0x7FFFFFFFFFFFFFFF;
		if (!v.rfind("file_", 0)) return std::strtoull(&string[5], nullptr, 16) & 0x7FFFFFFFFFFFFFFF;
		if (!v.rfind("script_", 0)) return std::strtoull(&string[7], nullptr, 16) & 0x7FFFFFFFFFFFFFFF;

		// lua notation x64:123
		if (!v.rfind("x64:", 0))
		{
			if (v.length() <= 0x18 && v.ends_with(".lua"))
			{
				// x64:123456789abcdf.lua
				// 
				// extract hash value
				char tmpbuffer[0x17] = {};

				memcpy(tmpbuffer, string + 4, v.length() - 8);

				// gen the hash and add .lua add the end
				return generate_hash(".lua", std::strtoull(&string[4], nullptr, 16) & 0x7FFFFFFFFFFFFFFF);
			}

			return std::strtoull(&string[4], nullptr, 16) & 0x7FFFFFFFFFFFFFFF;
		}

		// unknown, use hashed value
		return generate_hash(string);
	}
}

namespace variables
{
	constexpr varEntry create_var_record(const char* name, const char* description)
	{
		return { name, description, fnv1a::generate_hash_const(name) };
	}
	std::vector<varEntry> dvars_table =
	{
		create_var_record(
			"aim_slowdown_enabled",
			"Slowdown the turn rate when the cross hair passes over a target"
		),
		create_var_record(
			"aim_lockon_enabled",
			"Aim lock on helps the player to stay on target"
		),
		create_var_record(
			"arena_defaultPlaylist",
			"Default Arena Playlist"
		),
		create_var_record(
			"arena_seasonOverride",
			"Arena Season Override"
		),
		create_var_record(
			"arena_enableListenHosting",
			"Enable listen servers for arena matchmaking"
		),
		create_var_record(
			"arena_qosSkillStart",
			"Base arena point skill range, regardless of search duration"
		),
		create_var_record(
			"arena_qosBrodenSkillStep",
			"Arena points per interval to increase the distance between the skill and the qos skill"
		),
		create_var_record(
			"arena_qosBrodenSkillFrequency",
			"Frequency of which the the skill step is increased linearly"
		),
		create_var_record(
			"arena_minHostSkillRange",
			"Skill range from client's arena skill that must be qos'd before hosting lobby or unparking dedi"
		),
		create_var_record(
			"arena_giveBonusStar",
			"When enabled, one bonus star will be awarded every n consecutive wins."
		),
		create_var_record(
			"arena_bonusStarStreak",
			"Win streak required before awarding bonus star (if enabled)"
		),
		create_var_record(
			"arena_minPlayTime",
			"Time in seconds a player must play an arena match for a loss to count."
		),
		create_var_record(
			"arena_maintenance",
			"When accessing Arena mode, display a maintenance message."
		),
		create_var_record(
			"arena_lobbyReloadSearchDelayMin",
			"Minimum time in seconds to delay the search for a new arena lobby after a match."
		),
		create_var_record(
			"arena_lobbyReloadSearchDelayMax",
			"Maximum time in seconds to delay the search for a new arena lobby after a match."
		),
		create_var_record(
			"arena_enableArenaChallenges",
			"Enable arena challenges including Season Vet and Best Rank."
		),
		create_var_record(
			"arena_seasonVetChallengeWins",
			"Number of arena wins in a season to complete a season vet challenge."
		),
		create_var_record(
			"arena_unfairTeamGap",
			"This is the gap where if a team is down this many players compaired with the other team then the pentalty is waved."
		),
		create_var_record(
			"bg_movingPlatformPitchScale",
			"The scale to apply to pitch from the moving platform that should be applied to the player's pitch"
		),
		create_var_record(
			"player_view_pitch_up",
			"Maximum angle that the player can look up"
		),
		create_var_record(
			"player_view_pitch_down",
			"Maximum angle that the player can look down"
		),
		create_var_record(
			"player_view_swim_pitch_up",
			"Maximum angle that the player can look up while swimming"
		),
		create_var_record(
			"player_view_swim_pitch_down",
			"Maximum angle that the player can look up while swimming"
		),
		create_var_record(
			"player_lean_shift",
			"Amount to shift the player 3rd person model when leaning(x:left, y:right)"
		),
		create_var_record(
			"player_lean_shift_crouch",
			"Amount to shift the player 3rd person model when crouch leaning(x:left, y:right)"
		),
		create_var_record(
			"player_lean_rotate",
			"Amount to rotate the player 3rd person model when leaning(x:left, y:right)"
		),
		create_var_record(
			"player_lean_rotate_crouch",
			"Amount to rotate the player 3rd person model when crouch leaning(x:left, y:right)"
		),
		create_var_record(
			"bg_prone_yawcap",
			"The maximum angle that a player can look around quickly while prone"
		),
		create_var_record(
			"bg_aimSpreadMoveSpeedThreshold",
			"When player is moving faster than this speed, the aim spread will increase"
		),
		create_var_record(
			"bg_maxGrenadeIndicatorSpeed",
			"Maximum speed of grenade that will show up in indicator and can be thrown back."
		),
		create_var_record(
			"player_scopeExitOnDamage",
			"Exit the scope if the player takes damage"
		),
		create_var_record(
			"player_sustainAmmo",
			"Firing weapon will not decrease clip ammo."
		),
		create_var_record(
			"player_clipSizeMultiplier",
			"Changes the clip size of weapons with more than one bullet in their clip."
		),
		create_var_record(
			"player_lastStandSuicideDelay",
			"The amount of time that must pass before the player is allowed to suicide"
		),
		create_var_record(
			"player_sprintTime",
			"The base length of time a player can sprint"
		),
		create_var_record(
			"bg_gravity",
			"Gravity in inches per second per second"
		),
		create_var_record(
			"bg_lowGravity",
			"Low gravity for slow or floaty objects, in inches per second per second"
		),
		create_var_record(
			"bg_moonGravity",
			"Gravity on the moon, in inches per second per second"
		),
		create_var_record(
			"bg_waterGravity",
			"Gravity for missiles while in water that use tr_watergravity"
		),
		create_var_record(
			"player_viewLockEnt",
			"Set an entity that the player view will be locked too."
		),
		create_var_record(
			"cg_isGameplayActive",
			""
		),
		create_var_record(
			"vehLockTurretToPlayerView",
			"Locks the turret angles to the player angles and sets the players rotation speed to the turrets rotRate"
		),
		create_var_record(
			"mp_blackjack_consumable_wait",
			"For Blackjack, the time to wait between updating consumable time."
		),
		create_var_record(
			"tu11_enableClassicMode",
			""
		),
		create_var_record(
			"tu11_enableVehicleMode",
			""
		),
		create_var_record(
			"bg_shieldHitEncodeWidthWorld",
			"The encoding range, in width, of a client's world shield.  A hit in this range is encoded into one of 16 collumns."
		),
		create_var_record(
			"bg_shieldHitEncodeHeightWorld",
			"The encoding range, in height, of a client's world shield.  A hit in this range is encoded into one of 8 rows."
		),
		create_var_record(
			"bg_shieldHitEncodeWidthVM",
			"The decoding range, in width, of a client's viewmodel shield."
		),
		create_var_record(
			"bg_shieldHitEncodeHeightVM",
			"The decoding range, in height, of a client's viewmodel shield."
		),
		create_var_record(
			"bg_disableWeaponPlantingInWater",
			"Disables being able to plant mines in the water."
		),
		create_var_record(
			"disable_rope",
			"Disables the rope system"
		),
		create_var_record(
			"waterbrush_entity",
			"CM_GetWaterHeight function will test against this enitty. Can be used on brushmodels to move the water level"
		),
		create_var_record(
			"playerPushAmount",
			"If this value is set the player will get pushed away from AIs by the amount specified."
		),
		create_var_record(
			"bg_serverDelayDamageKickForPing",
			"Turn on delayed damage kick on server to allow for client ping"
		),
		create_var_record(
			"bg_useClientDamageKick",
			"Turn on to make the server use the damage kick angles from the client usercmd"
		),
		create_var_record(
			"slam_enabled",
			"Enable slam activation"
		),
		create_var_record(
			"playerWeaponRaisePostIGC",
			"Temp dvar to give script control over which wepaon anim plays after an IGC"
		),
		create_var_record(
			"bg_allowPlayerRoleTemplateButtonOverrides",
			"When true the PlayerRoleTemplates are allowed to define button overrides per the enum PlayerRoleButtonOverrideTypes."
		),
		create_var_record(
			"bg_aqs",
			""
		),
		create_var_record(
			"bg_aqsStyle",
			""
		),
		create_var_record(
			"bg_ads",
			""
		),
		create_var_record(
			"bg_bobcycleResetThreshold",
			"The difference between the new and old bob cycle before we treat as if the server reset its bob cycle."
		),
		create_var_record(
			"adsZeroSpread",
			"Immediately zero spreadAmount when fully ADS"
		),
		create_var_record(
			"bg_deferScriptMissileDetonation",
			"Defer G_ExplodeMissile() till after origin is updated in G_RunMissile()"
		),
		create_var_record(
			"bg_isolateDamageFlash",
			"Drive damage flash overlay from isolated variable instead of view kick pitch"
		),
		create_var_record(
			"bg_limitGrenadeImpacts",
			"Use missileTargetEnt to limit impact damage to the first impact"
		),
		create_var_record(
			"bg_zombiePlayerUsesUtilityClip",
			"Use utility clip for zombie player movement"
		),
		create_var_record(
			"bg_warmode_version",
			"Temp dvar to control the war mode gametype"
		),
		create_var_record(
			"gadgetPowerOverrideFactor",
			"Override power factor"
		),
		create_var_record(
			"gadgetPowerOverchargePerkTimeFactor",
			"Overcharge perk charge over time multiplier"
		),
		create_var_record(
			"gadgetPowerOverchargePerkScoreFactor",
			"Overcharge perk score multiplier"
		),
		create_var_record(
			"gadgetThiefShutdownFullCharge",
			"If this is set to true then the users will recieve a full gadget charge if they shutdown an enemy. "
		),
		create_var_record(
			"gadget_force_slots",
			"Force the gadget slots to be in a defined order: primary, secondary, ability, heavy"
		),
		create_var_record(
			"mm_keyframeUsageAnimIndex",
			"anim to watch usage graph of"
		),
		create_var_record(
			"cg_minimapPadding",
			"The amount of padding to apply to the circular minimap"
		),
		create_var_record(
			"shoutcastHighlightedClient",
			"Shoutcaster's currently highlighted clientNum."
		),
		create_var_record(
			"cg_hudMapFriendlyWidth",
			"The size of the friendly icon on the full map"
		),
		create_var_record(
			"cg_hudMapFriendlyHeight",
			"The size of the friendly icon on the full map"
		),
		create_var_record(
			"cg_hudMapPlayerWidth",
			"The size of the player's icon on the full map"
		),
		create_var_record(
			"cg_hudMapPlayerHeight",
			"The size of the player's icon on the full map"
		),
		create_var_record(
			"waypointIconWidth",
			"Width of the offscreen pointer."
		),
		create_var_record(
			"waypointIconHeight",
			"Height of the offscreen pointer."
		),
		create_var_record(
			"waypointOffscreenPointerDistance",
			"Distance from the center of the offscreen objective icon to the center its arrow."
		),
		create_var_record(
			"cg_threatDetectorRadius",
			"The radius of the threat detector scan."
		),
		create_var_record(
			"cg_usingClientScripts",
			"True, if client scripts are enabled."
		),
		create_var_record(
			"cg_drawGun",
			"Draw the view model"
		),
		create_var_record(
			"cg_weaponHintsCoD1Style",
			"Draw weapon hints in CoD1 style: with the weapon name, and with the icon below"
		),
		create_var_record(
			"cg_focalLength",
			"Lens focal length for 3-perf 16x9 super 35mm (24.89mm x 14mm)"
		),
		create_var_record(
			"cg_fov",
			"The field of view angle in degrees"
		),
		create_var_record(
			"cg_fovExtraCam",
			"The field of view angle in degrees for the extra cam"
		),
		create_var_record(
			"cg_useWeaponBasedVariableZoom",
			"Use weapon based variable zoom instead of player based."
		),
		create_var_record(
			"cg_viewVehicleInfluenceGunner",
			"The influence on the view from being a vehicle gunner"
		),
		create_var_record(
			"cg_viewVehicleInfluenceGunnerFiring",
			"The influence on the view from being a vehicle gunner while firing"
		),
		create_var_record(
			"cg_viewVehicleInfluenceGunner_mode",
			"Controls when the viewVehicleInfluenceGunnerFiring is effective. 0 - only on ADS and firing; 1 - on ADS, firing, or moving camera; 2 - always on"
		),
		create_var_record(
			"cg_draw2D",
			"Draw 2D screen elements"
		),
		create_var_record(
			"cg_drawLagometer",
			"Draw lagometer"
		),
		create_var_record(
			"cg_drawFPS",
			"Draw frames per second"
		),
		create_var_record(
			"cg_drawMaterialImageNum",
			"Use up/down on the dpad to select an image when cg_drawMaterial is enabled.  Press right for more info on that image."
		),
		create_var_record(
			"cg_drawMaterialImageName",
			"Do not use (set by cg_drawMaterialImageNum code)"
		),
		create_var_record(
			"cg_drawCrosshair",
			"Turn on weapon crosshair"
		),
		create_var_record(
			"cg_drawCrosshairCooker",
			"Turn on cook indicator for cooked grenades. Use 1 for cookOffHoldTime grenades and 2 cookOffHoldTime and cookOffHold ones."
		),
		create_var_record(
			"cg_drawCrosshairNames",
			"Draw the name of an enemy under the crosshair"
		),
		create_var_record(
			"cg_hudGrenadeIconMaxRangeFrag",
			"The minimum distance that a grenade has to be from a player in order to be shown on the grenade indicator"
		),
		create_var_record(
			"cg_lagometer_pos",
			"lagometer position"
		),
		create_var_record(
			"cg_thirdPersonRange",
			"The range of the camera from the player in third person view"
		),
		create_var_record(
			"cg_thirdPersonAngle",
			"The angle of the camera from the player in third person view"
		),
		create_var_record(
			"cg_thirdPersonRoll",
			"The roll of the camera from the player in third person view"
		),
		create_var_record(
			"cg_thirdPersonSideOffset",
			"The side offset for the camera from the player in third person view"
		),
		create_var_record(
			"cg_thirdPersonUpOffset",
			"The up offset for the camera from the player in third person view"
		),
		create_var_record(
			"cg_thirdPersonFocusDist",
			"The distance infront of the player to aim the 3rd person camera at"
		),
		create_var_record(
			"cg_thirdPersonFocusOffsetUp",
			"An offset to add to the position the camera is looking at"
		),
		create_var_record(
			"cg_thirdPersonCamOffsetUp",
			"An offset to add to the camera position"
		),
		create_var_record(
			"cg_thirdPersonCamLerpScale",
			"Lerp amount for the camera in 3rd person mode"
		),
		create_var_record(
			"cg_thirdPerson",
			"Use third person view"
		),
		create_var_record(
			"cg_thirdPersonMode",
			"How the camera behaves in third person"
		),
		create_var_record(
			"cg_subtitleWidthWidescreen",
			"The width of the subtitle on a wide-screen"
		),
		create_var_record(
			"cg_headIconMinScreenRadius",
			"The minumum radius of a head icon on the screen"
		),
		create_var_record(
			"cg_overheadNamesSize",
			"The maximum size to show overhead names"
		),
		create_var_record(
			"cg_overheadIconSize",
			"The maximum size to show overhead icons like 'rank'"
		),
		create_var_record(
			"cg_overheadRankSize",
			"The size to show rank text"
		),
		create_var_record(
			"cg_healthPerBar",
			"How much health is represented per health bar"
		),
		create_var_record(
			"cg_drawFriendlyNames",
			"Whether to show friendly names in game"
		),
		create_var_record(
			"cg_playerHighlightTargetSize",
			"Size of player target highlights."
		),
		create_var_record(
			"cg_ScoresColor_Gamertag_0",
			"player gamertag color on scoreboard"
		),
		create_var_record(
			"cg_ScoresColor_Gamertag_1",
			"player gamertag color on scoreboard"
		),
		create_var_record(
			"cg_ScoresColor_Gamertag_2",
			"player gamertag color on scoreboard"
		),
		create_var_record(
			"cg_ScoresColor_Gamertag_3",
			"player gamertag color on scoreboard"
		),
		create_var_record(
			"hud_healthOverlay_pulseStart",
			"The percentage of full health at which the low-health warning overlay begins flashing"
		),
		create_var_record(
			"cg_drawTalk",
			"Controls which icons CG_TALKER ownerdraw draws"
		),
		create_var_record(
			"cg_drawJobsPerf",
			"draw jobs performance information"
		),
		create_var_record(
			"flareDisableEffects",
			""
		),
		create_var_record(
			"showVisionSetDebugInfo",
			"Enables visionset debug info"
		),
		create_var_record(
			"cl_smoothSnapInterval",
			"Length of the buffer smoothing the snap interval when adjusting the time delta. Value must be a power of 2."
		),
		create_var_record(
			"con_typewriterColorBase",
			"Base color of typewritten objective text."
		),
		create_var_record(
			"cl_deathMessageWidth",
			"Pixel width of the obituary area"
		),
		create_var_record(
			"m_pitch",
			"Default pitch"
		),
		create_var_record(
			"m_filter",
			"Allow mouse movement smoothing"
		),
		create_var_record(
			"m_mouseFilter",
			"Mouse filter"
		),
		create_var_record(
			"m_mouseAcceleration",
			"Mouse acceleration"
		),
		create_var_record(
			"m_mouseSensitivity",
			"Mouse sensitivity"
		),
		create_var_record(
			"cl_freelook",
			"Enable looking with mouse"
		),
		create_var_record(
			"cl_motdString",
			"Message of the day"
		),
		create_var_record(
			"cl_ingame",
			"True if the game is active"
		),
		create_var_record(
			"player_name",
			"Player name"
		),
		create_var_record(
			"maxVoicePacketsPerFrame",
			"The max number of voice packets that a local client will process per frame"
		),
		create_var_record(
			"splitscreen_playerCount",
			"The number of players in a splitscreen game"
		),
		create_var_record(
			"splitscreen_horizontal",
			"Draw splitscreen views horizontally"
		),
		create_var_record(
			"cl_migrationPingTime",
			"how many seconds between client pings.  used to determine hosting suitability."
		),
		create_var_record(
			"con_label_filter_mask",
			"mask to filter tty by label, if flag set then visible (1<<label)"
		),
		create_var_record(
			"live_whitelistFatal",
			"Sys_error on whitelist failure"
		),
		create_var_record(
			"saveLocalMatchRecordBinaryFile",
			"If set to true, and development-build, save out local copy of match_record"
		),
		create_var_record(
			"boostcheatIntercept",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatHeadshotsTotalMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatHeadshotsTotalStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatHeadshotsTotalCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanDistanceVictimTraveledMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanDistanceVictimTraveledStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanDistanceVictimTraveledCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillsTotalMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillsTotalStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillsTotalCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillsRatioMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillsRatioStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillsRatioCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanDistanceBitchTraveledMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanDistanceBitchTraveledStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanDistanceBitchTraveledCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchHKRatioMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchHKRatioStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchHKRatioCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanBitchLifetimeMillisecondsMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanBitchLifetimeMillisecondsStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatMeanBitchLifetimeMillisecondsCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatKillerXAnomalyMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatKillerXAnomalyStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatKillerXAnomalyCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatKillerYAnomalyMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatKillerYAnomalyStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatKillerYAnomalyCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatVictimXAnomalyMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatVictimXAnomalyStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatVictimXAnomalyCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatVictimYAnomalyMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatVictimYAnomalyStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatVictimYAnomalyCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillTimestampsAnomalyMean",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillTimestampsAnomalyStddev",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"boostcheatBitchKillTimestampsAnomalyCoef",
			"Boosting detector parameter (64 bits should be cast to double)"
		),
		create_var_record(
			"fx_marks_draw",
			"Toggles whether decals are rendered"
		),
		create_var_record(
			"fx_debugBolt",
			"Debug effects bolt"
		),
		create_var_record(
			"fx_visMinTraceDist",
			"Minimum visibility trace size"
		),
		create_var_record(
			"fx_profile",
			"Turn on FX profiling (specify which local client, with '1' being the first.)"
		),
		create_var_record(
			"fx_mark_profile",
			"Turn on FX profiling for marks (specify which local client, with '1' being the first.)"
		),
		create_var_record(
			"fx_drawClouds",
			"Toggles the drawing of particle clouds"
		),
		create_var_record(
			"fx_occlusion_max_player_count",
			"Maximium number of local clients that FX is drawing sprite with occlusion test.)"
		),
		create_var_record(
			"ai_useTacticalInfluencers",
			"Tactical Influencers : Debug draw tactical influencers."
		),
		create_var_record(
			"ai_tacticalInfluencersDebug",
			"Tactical Influencers : Debug draw tactical influencers."
		),
		create_var_record(
			"ai_tacticalInfluencersThreatDebug",
			"Tactical Influencers : Debug draw tactical threat."
		),
		create_var_record(
			"ui_blocksaves",
			"prevents level progression in saves, does not block player progression"
		),
		create_var_record(
			"settings_defaultSubtitles",
			"Any language that allows subtitles (except English) defaults to turn them on"
		),
		create_var_record(
			"tgraph_debugDrawPoints",
			"Draw points in the graph"
		),
		create_var_record(
			"tgraph_debugShowStats",
			"Show stats information of tgraph"
		),
		create_var_record(
			"tgraph_debugDrawPointNavFace",
			"Draw the navmesh face points are linked to"
		),
		create_var_record(
			"tgraph_debugTargetPointId",
			"Debug the point with this ID"
		),
		create_var_record(
			"tgraph_debugVisMapPointId",
			"Debug the visibility map on this point"
		),
		create_var_record(
			"tgraph_debugVisMapDrawNonVisible",
			"When debugging vismap, should the blocked sight be drawn (with different color)"
		),
		create_var_record(
			"tgraph_debugClearancePointId",
			"Debug the clearance value on this point"
		),
		create_var_record(
			"tquery_drawQuery",
			"Draw debug information for specific querydef"
		),
		create_var_record(
			"bot_supported",
			"Whether bots are supported"
		),
		create_var_record(
			"bot_friends",
			"Number of friends allowed in basic training"
		),
		create_var_record(
			"bot_enemies",
			"Number of enemies allowed in basic training"
		),
		create_var_record(
			"bot_difficulty",
			"Difficulty level of the basic training bots"
		),
		create_var_record(
			"bot_maxFree",
			"Maximum number of bots for TEAM_FREE"
		),
		create_var_record(
			"bot_maxAllies",
			"Maximum number of bots for TEAM_ALLIES"
		),
		create_var_record(
			"bot_maxAxis",
			"Maximum number of bots for TEAM_AXIS"
		),
		create_var_record(
			"bot_maxMantleHeight",
			"Max height a bot will attempt a mantle traversal"
		),
		create_var_record(
			"bot_AllowMelee",
			"Allow bots to press the melee button"
		),
		create_var_record(
			"bot_AllowGrenades",
			"Allow bots to throw lethal and tactical grenades"
		),
		create_var_record(
			"bot_AllowHeroGadgets",
			"Allow bots to use hero gadgets"
		),
		create_var_record(
			"g_loadScripts",
			"Disable scripts from loading"
		),
		create_var_record(
			"ui_doa_unlocked",
			"unlock doa in the mission list"
		),
		create_var_record(
			"ui_codexindex_unlocked",
			"add the master index to the bookmarks list"
		),
		create_var_record(
			"g_password",
			"Password"
		),
		create_var_record(
			"g_speed",
			"Player speed"
		),
		create_var_record(
			"g_debugDamage",
			"Show debug information for damage"
		),
		create_var_record(
			"g_vehicleDrawSplines",
			"Draws the vehicles splines"
		),
		create_var_record(
			"g_vehicleBypassFriendlyFire",
			"Make vehicle always damagable regardless of team"
		),
		create_var_record(
			"ai_useLeanRunAnimations",
			"whether to use lean run animations instead of strafes"
		),
		create_var_record(
			"ai_showNodesDist",
			"Maximum distance from the camera at which AI nodes are shown"
		),
		create_var_record(
			"ai_showNearestNode",
			"Show nodes closest to AI"
		),
		create_var_record(
			"ai_debugVolumeTool",
			"Show AI navigation volume debug information"
		),
		create_var_record(
			"ai_showPaths",
			"Show AI navigation paths"
		),
		create_var_record(
			"ai_clampToNavMeshEnabled",
			"Periodically clamp actors to the navmesh if they get off it."
		),
		create_var_record(
			"ai_debugCoverEntityNum",
			"Display debug info for cover"
		),
		create_var_record(
			"ai_showDodge",
			"Display debug information for AI dodging"
		),
		create_var_record(
			"ai_debugAnimScripted",
			"Enable debugging information for anim scripted AI."
		),
		create_var_record(
			"ai_debugEntIndex",
			"Entity index of an entity to debug"
		),
		create_var_record(
			"ai_debugArrivals",
			"Record AI arrival and exit debug information"
		),
		create_var_record(
			"ai_meleeDamage",
			"The amount of damage dealt by AI's melee attack"
		),
		create_var_record(
			"ai_scaleSightUpdateLatency",
			"Scale actor sight update latency based on distance to the nearest player"
		),
		create_var_record(
			"ai_accuracy_attackerCountDecrease",
			"Accuracy multiplied by this number for each additional attacker upto ai_accuracy_attackerCountMax"
		),
		create_var_record(
			"g_motd",
			"The message of the day"
		),
		create_var_record(
			"g_allowVote",
			"Enable voting on this server"
		),
		create_var_record(
			"g_customTeamName_Allies",
			"User assigned allied team name"
		),
		create_var_record(
			"g_customTeamName_Axis",
			"User assigned axis team name"
		),
		create_var_record(
			"g_TeamName_Allies",
			"Allied team name"
		),
		create_var_record(
			"g_TeamName_Axis",
			"Axis team name"
		),
		create_var_record(
			"g_quadrotorFlyHeight",
			"Default fly height of the quadrotors"
		),
		create_var_record(
			"g_fogColorReadOnly",
			"Fog color that was set in the most recent call to \"setexpfog\""
		),
		create_var_record(
			"g_fogStartDistReadOnly",
			"Fog start distance that was set in the most recent call to \"setexpfog\""
		),
		create_var_record(
			"g_fogHalfDistReadOnly",
			"Fog start distance that was set in the most recent call to \"setexpfog\""
		),
		create_var_record(
			"spawnsystem_convert_spawns_to_structs",
			"parse all map placed spawns to structs"
		),
		create_var_record(
			"spawnsystem_player_explored_radius",
			"How close a spawn point has to be to be considered 'explored'"
		),
		create_var_record(
			"spawnsystem_use_code_point_enabled",
			"Use code to filter enabled and disabled spawn points"
		),
		create_var_record(
			"vehicle_collision_prediction_time",
			"How far ahead of the vehicle it should predict to do collision damage."
		),
		create_var_record(
			"vehicle_collision_prediction_crash_time",
			"How far ahead of the vehicle it should predict to do collision."
		),
		create_var_record(
			"r_lightingSunShadowCacheDynamicSiegeDistance",
			"Inches from camera to draw dynamic siege shadows"
		),
		create_var_record(
			"nv_textureReleaseFrameDelay",
			"nv_textureReleaseFrameDelay"
		),
		create_var_record(
			"nv_textureReleaseMaxPerFrame",
			"nv_textureReleaseMaxPerFrame"
		),
		create_var_record(
			"amd_useShaderExtension",
			"use read first lane"
		),
		create_var_record(
			"r_drawFrameDurationGraph",
			"Display a graph of the frame durations."
		),
		create_var_record(
			"r_lateAllocLimit",
			"Set the VS late alloc limit for parameter cache"
		),
		create_var_record(
			"r_OIT",
			"Enable Order Independant Transperancy."
		),
		create_var_record(
			"r_backBufferCount",
			"Number of backbuffer, set to 3 to enable triple buffering"
		),
		create_var_record(
			"r_fullscreen",
			"Game window mode (window, fullscreen, fullscreen window)"
		),
		create_var_record(
			"r_convergence",
			"Change 3D convergence"
		),
		create_var_record(
			"r_deferredIgnoreShadowUpdate",
			"Lighting: Ignore the shadow update flag"
		),
		create_var_record(
			"r_deferredForceShadowNeverUpdate",
			"Lighting: Always render shadow only once"
		),
		create_var_record(
			"r_viewmodelSelfShadow",
			"Enable viewmodel self shadowing"
		),
		create_var_record(
			"r_dedicatedPlayerShadow",
			"Enable player dedicated shadowing"
		),
		create_var_record(
			"r_dedicatedPlayerSunShadowPenumbraScale",
			"Change player shadow penumbra in sun"
		),
		create_var_record(
			"r_lightingSunShadowDisableDynamicDraw",
			"Disable rendering to the sun shadow splits"
		),
		create_var_record(
			"r_lightingSunShadowCacheEnable",
			"Enable caching for sun shadows"
		),
		create_var_record(
			"r_lightingSunShadowSSTMipDrop",
			"Drop mips from the SST (saving GPU memory)"
		),
		create_var_record(
			"r_lightingReflectionProbeMipDrop",
			"Number of mips to drop from the reflection probes"
		),
		create_var_record(
			"r_lightingSpotOmniShadowMaxQuality",
			"Set the maximum spot/omni shadow quality"
		),
		create_var_record(
			"r_lightingSpotShadowForceSize",
			"Set the maximum spot shadows size"
		),
		create_var_record(
			"r_lightingShadowFiltering",
			"Enable shadow filtering"
		),
		create_var_record(
			"r_lightingOmniShadowForceSize",
			"Set the maximum spot shadows size"
		),
		create_var_record(
			"r_dedicatedPlayerSunShadowResolution",
			"Set the sun dedicated shadow resolution"
		),
		create_var_record(
			"r_dedicatedPlayerSpotOmniShadowResolution",
			"Set the spot/omni dedicated shadow resolution"
		),
		create_var_record(
			"r_viewModelSunShadowResolution",
			"Set the sun view model shadow resolution"
		),
		create_var_record(
			"r_viewModelSpotOmniShadowResolution",
			"Set the spot/omni view mode shadow resolution"
		),
		create_var_record(
			"r_spotShadowRes",
			"Spot shadow resolution"
		),
		create_var_record(
			"r_omniShadowRes",
			"Omni shadow resolution"
		),
		create_var_record(
			"r_asyncCompute",
			"Enable all async compute that uses the GfxAsyncComputeScope class."
		),
		create_var_record(
			"r_extracamDisable",
			"Disable extracam rendering"
		),
		create_var_record(
			"r_forceTextureAniso",
			"Force Anisotropy filter level"
		),
		create_var_record(
			"r_forceMaxTextureSize",
			"Force max texture sizes"
		),
		create_var_record(
			"r_autoLodCullRadius",
			"Auto-lod culling radius in screen pixels."
		),
		create_var_record(
			"r_lodScaleRigid",
			"Scale the level of detail distance for rigid models (larger reduces detail)"
		),
		create_var_record(
			"r_lodBiasRigid",
			"Bias the level of detail distance for rigid models (negative increases detail)"
		),
		create_var_record(
			"r_modelLodBias",
			"LOD scale"
		),
		create_var_record(
			"r_modelLodLimit",
			"Hard limit to reject higher lod from being picked"
		),
		create_var_record(
			"r_lodThresholdPixelArea",
			"When screen-space average triangle area falls below this threshold, switch LODs"
		),
		create_var_record(
			"r_zfar",
			"Change the distance at which culling fog reaches 100% opacity; 0 is off"
		),
		create_var_record(
			"r_fog",
			"Set to 0 to disable fog"
		),
		create_var_record(
			"r_norefresh",
			"Skips all rendering.  Useful for benchmarking."
		),
		create_var_record(
			"r_scaleViewport",
			"Scale 3D viewports by this fraction.  Use this to see if framerate is pixel shader bound."
		),
		create_var_record(
			"r_vsync",
			"Enable v-sync before drawing the next frame to avoid 'tearing' artifacts."
		),
		create_var_record(
			"r_clearColor",
			"Color to clear the screen to when clearing the frame buffer"
		),
		create_var_record(
			"r_clearColor2",
			"Color to clear every second frame to (for use during development)"
		),
		create_var_record(
			"r_viewportBackingColor",
			"Color to clear the frame buffer with before compositing viewports"
		),
		create_var_record(
			"r_spotLightShadows",
			"Enable shadows for spot lights."
		),
		create_var_record(
			"r_spotLightSModelShadows",
			"Enable static model shadows for spot lights."
		),
		create_var_record(
			"r_spotLightEntityShadows",
			"Enable entity shadows for spot lights."
		),
		create_var_record(
			"r_lockPvsInCode",
			"Indicates if pvslock is currently on, in code. Only code should modified this dvar"
		),
		create_var_record(
			"r_lockFrameRateTo30Hz",
			"Locks Frame Rate to 30Hz: -1:Nochange 0:60Hz 1:30Hz"
		),
		create_var_record(
			"r_lockFrameRateTo30Hz_enable",
			"Enable 30Hz frame rate lock"
		),
		create_var_record(
			"r_lockFrameRateTo30Hz_dynResOff",
			"Disable dynamic resolution when 30Hz is forced"
		),
		create_var_record(
			"r_useStrict30HzConditions",
			""
		),
		create_var_record(
			"r_forceLod",
			"Force all level of detail to this level"
		),
		create_var_record(
			"r_zombieNameAllowFriendsList",
			"Allow zombie name to be from friends list"
		),
		create_var_record(
			"r_zombieNameAllowDevList",
			"Allow zombie name to be from dev list"
		),
		create_var_record(
			"r_tilingHighlight",
			""
		),
		create_var_record(
			"r_tilingLightCount",
			""
		),
		create_var_record(
			"r_tilingProbeCount",
			""
		),
		create_var_record(
			"r_tilingSunShadow",
			""
		),
		create_var_record(
			"r_streamReadLog",
			"Log image and mesh reading"
		),
		create_var_record(
			"r_streamTextureMemoryMax",
			"Limit size of stream buffer in MB (0 is no limit)"
		),
		create_var_record(
			"r_streamFreezeState",
			"Freeze the state of all streaming memory - don't allow streaming or un-streaming of any images."
		),
		create_var_record(
			"r_stereo3DAvailable",
			"3D Mode available"
		),
		create_var_record(
			"r_stereo3DOn",
			"3D on off toggle"
		),
		create_var_record(
			"r_stereo3DMode",
			"3D Rendering mode"
		),
		create_var_record(
			"r_gpuCullingDisableDepthTest",
			"disable the gpu culling depth part"
		),
		create_var_record(
			"r_dedicatedShadowsUmbra",
			"Cull dedicated player and viewmodel shadows"
		),
		create_var_record(
			"r_cachedSpotShadowCopyMode",
			"spot shadow copy mode"
		),
		create_var_record(
			"r_smaaQuincunx",
			"Enables quincunx temporal resolve (for T2x modes)"
		),
		create_var_record(
			"r_fxaaDebug",
			"FXAA debug mode"
		),
		create_var_record(
			"r_fxaaContrastThreshold",
			"FXAA contrast threshold"
		),
		create_var_record(
			"r_fxaaSubpixelRemoval",
			"FXAA subpixel removal"
		),
		create_var_record(
			"r_aaTechnique",
			"Anti-aliasing technique"
		),
		create_var_record(
			"r_aaAllowTemporalMultiGpu",
			"Anti-aliasing temporal overide for SLI"
		),
		create_var_record(
			"r_ssaoTechnique",
			"SSAO technique."
		),
		create_var_record(
			"r_smodel_partialSortWorkers",
			"Number of workers to sort the smodel"
		),
		create_var_record(
			"r_smodel_splitWorkers",
			"Number of workers to split smodel"
		),
		create_var_record(
			"r_smodel_combineWorkers",
			"Number of workers to combine smodel"
		),
		create_var_record(
			"r_ssao_gtaoTweak",
			"Turn on to tweak ssao"
		),
		create_var_record(
			"r_ssao_gtaoStrength",
			"GTAO strength value. The default (1.0) will produce ground truth values"
		),
		create_var_record(
			"r_ssao_gtaoLow",
			"GTAO shadow tone"
		),
		create_var_record(
			"r_ssao_gtaoMid",
			"GTAO mid tone"
		),
		create_var_record(
			"r_ssao_gtaoHigh",
			"GTAO high tone"
		),
		create_var_record(
			"r_sssblurEnable",
			"SSS Blur enable"
		),
		create_var_record(
			"r_flame_allowed",
			"Allow flame effect."
		),
		create_var_record(
			"r_filmTweakLut",
			"Tweak Film LUT Index."
		),
		create_var_record(
			"r_enablePlayerShadow",
			"Enable First Person Player Shadow."
		),
		create_var_record(
			"r_xcamsEnabled",
			"Enable/disable the xcam system controlling the camera (does not effect scene playback)."
		),
		create_var_record(
			"r_expAuto",
			"toggle auto exposure"
		),
		create_var_record(
			"r_bloomUseLutALT",
			"enable alternate lut"
		),
		create_var_record(
			"vc_LUT",
			"Lut index"
		),
		create_var_record(
			"r_adsWorldFocalDistanceMax",
			"Ads World Blur max focalDistance"
		),
		create_var_record(
			"r_adsWorldFocalDistanceMin",
			"Ads World Blur min focalDistance"
		),
		create_var_record(
			"r_adsWorldFocalDistanceTrackBackScale",
			"Ads World Blur track back distance scale"
		),
		create_var_record(
			"r_adsBloomDownsample",
			"Enables Ads Blur shared bloom downsample"
		),
		create_var_record(
			"r_flameFX_distortionScaleFactor",
			"Distortion uv scales (Default to 1)"
		),
		create_var_record(
			"r_flameFX_magnitude",
			"Distortion magnitude"
		),
		create_var_record(
			"r_flameFX_FPS",
			"fire frames per sec"
		),
		create_var_record(
			"r_flameFX_fadeDuration",
			"Sets fade duration in seconds"
		),
		create_var_record(
			"r_waterSheetingFX_allowed",
			"Enable the water sheeting effect"
		),
		create_var_record(
			"r_waterSheetingFX_enable",
			"Enable the water sheeting effect"
		),
		create_var_record(
			"r_waterSheetingFX_distortionScaleFactor",
			"Distortion uv scales (Default to 1)"
		),
		create_var_record(
			"r_waterSheetingFX_magnitude",
			"Distortion magnitude"
		),
		create_var_record(
			"r_waterSheetingFX_radius",
			"Tweak dev var; Glow radius in pixels at 640x480"
		),
		create_var_record(
			"r_anaglyphFX_enable",
			"Enable red/green Anaglyph 3DS"
		),
		create_var_record(
			"r_graphicContentBlur",
			"Enable Fullscreen Blur"
		),
		create_var_record(
			"r_postFxWobble",
			"Enable Fullscreen Wobble"
		),
		create_var_record(
			"r_eacPathFX_enable",
			"Enable EAC path overlay"
		),
		create_var_record(
			"r_eacPath_Posn",
			"EAC Path drone position"
		),
		create_var_record(
			"r_eacPath_Radius",
			"EAC Path Radius"
		),
		create_var_record(
			"r_eacPath_Step",
			"EAC Path Step"
		),
		create_var_record(
			"r_blurAndTintEnable",
			"Enable Blur and Tint"
		),
		create_var_record(
			"r_blurAndTintLevel",
			"Blur Level"
		),
		create_var_record(
			"r_circleMaskRadius",
			"Circle Mask Enable"
		),
		create_var_record(
			"r_splitScreenExpandFull",
			"Split Screen is expanded to Fullscreen"
		),
		create_var_record(
			"cl_secondaryPlayerMenuControlDisable",
			"Disable menu control for the secondary player"
		),
		create_var_record(
			"r_ev_distance",
			"EV distance"
		),
		create_var_record(
			"r_ev_width",
			"EV pulse width"
		),
		create_var_record(
			"r_ev_targetwidth",
			"EV pulse target width"
		),
		create_var_record(
			"r_ev_rate",
			"EV pulse rate"
		),
		create_var_record(
			"r_ev_geometryrange",
			"EV geometry range"
		),
		create_var_record(
			"r_ev_targetrange",
			"EV target range"
		),
		create_var_record(
			"r_ev_edgewidth",
			"EV edge pulse width"
		),
		create_var_record(
			"r_ev_edgethickness",
			"EV edge thickness"
		),
		create_var_record(
			"r_ev_testenable",
			"EV test enable"
		),
		create_var_record(
			"r_ev_screen_scale",
			"EV screen scale"
		),
		create_var_record(
			"r_ev_screen_threshold",
			"EV screen threshold"
		),
		create_var_record(
			"r_ev_random_ground",
			"EV random ground"
		),
		create_var_record(
			"r_ev_random_edge",
			"EV random edge"
		),
		create_var_record(
			"r_aberrationFX_enable",
			"Enable Aberration"
		),
		create_var_record(
			"r_postFxIndex",
			"Display PostFx with this index"
		),
		create_var_record(
			"r_postFxSubIndex",
			"Tweak PostFx sub index for keyline variants"
		),
		create_var_record(
			"r_postFxUseTweaks",
			"Override PostFx Params with dvars"
		),
		create_var_record(
			"r_catsEyeReset",
			"Reset CatsEye dvars to default"
		),
		create_var_record(
			"r_catsEyeDecolor",
			"Cats Eye: decolor"
		),
		create_var_record(
			"r_catsEyeNoise",
			"Cats Eye: noise"
		),
		create_var_record(
			"r_catsEyeTexture",
			"Cats Eye: texture level"
		),
		create_var_record(
			"r_catsEyeDistortion",
			"Cats Eye: distortion &scale"
		),
		create_var_record(
			"r_catsEyeAberation",
			"Cats Eye: aberation"
		),
		create_var_record(
			"r_catsEyeBlur",
			"Cats Eye: blur amount"
		),
		create_var_record(
			"r_catsEyeBlurTint",
			"Cats Eye: blur tint"
		),
		create_var_record(
			"r_chaserFX_enable",
			"Enable Chaser PostFX"
		),
		create_var_record(
			"r_heatPulseFX_enable",
			"Enable Heat Pulse Gun Effect"
		),
		create_var_record(
			"r_radioactiveFX_enable",
			"Enable Radiaoctive Effect"
		),
		create_var_record(
			"r_radioactiveBlur",
			"Radioactive: blur"
		),
		create_var_record(
			"r_radioactiveSpeed",
			"Radioactive: fire animation speed"
		),
		create_var_record(
			"r_radioactiveIntensity",
			"Radioactive: fire intensity"
		),
		create_var_record(
			"r_uiHudFX_enable",
			"Enable UI HUD Effect"
		),
		create_var_record(
			"r_maxPOMSamples",
			"Additional ceiling on maximum number of samples in POM raycast, scales performance for testing"
		),
		create_var_record(
			"r_POMLODStart",
			"Distance to begin scaling down POM effect"
		),
		create_var_record(
			"r_POMLODEnd",
			"Distance to finish scaling down POM effect"
		),
		create_var_record(
			"r_poisonFX_pulse",
			"pulse rate for distortion"
		),
		create_var_record(
			"r_poisonFX_blurMin",
			"blur min"
		),
		create_var_record(
			"r_poisonFX_blurMax",
			"blur max"
		),
		create_var_record(
			"r_fogTweak",
			"enable dvar tweaks"
		),
		create_var_record(
			"r_fogBaseDist",
			"start distance"
		),
		create_var_record(
			"r_fogHalfDist",
			"distance at which fog is 50%"
		),
		create_var_record(
			"r_fogBaseHeight",
			"start height"
		),
		create_var_record(
			"r_fogHalfHeight",
			"height at which fog is 50%"
		),
		create_var_record(
			"r_fogSkyHalfHeightOffset",
			"sky fog density half height offset"
		),
		create_var_record(
			"r_fogColor",
			"color"
		),
		create_var_record(
			"r_fogIntensity",
			"intensity"
		),
		create_var_record(
			"r_fogOpacity",
			"opacity"
		),
		create_var_record(
			"r_fogSunColor",
			"sun color"
		),
		create_var_record(
			"r_fogSunIntensity",
			"sun color"
		),
		create_var_record(
			"r_fogSunOpacity",
			"sun opacity"
		),
		create_var_record(
			"r_fogSunPitchOffset",
			"sun pitch offset"
		),
		create_var_record(
			"r_fogSunYawOffset",
			"sun yaw offset"
		),
		create_var_record(
			"r_fogSunInner",
			"sun angle start"
		),
		create_var_record(
			"r_fogSunOuter",
			"sun angle end"
		),
		create_var_record(
			"r_atmospherefogcolor",
			"atmosphere fog color (Raleigh)"
		),
		create_var_record(
			"r_atmospherefogdensity",
			"atmosphere fog density"
		),
		create_var_record(
			"r_atmospherehazecolor",
			"atmosphere haze color (Mie)"
		),
		create_var_record(
			"r_atmospherehazedensity",
			"atmosphere sun haze strength (overall density that is allocated to haze with the remainder assigned to fog)"
		),
		create_var_record(
			"r_atmospherehazespread",
			"atmosphere sun haze spread"
		),
		create_var_record(
			"r_atmosphereinscatterstrength",
			"atmosphere fog Brightnesss/In-scatter strength"
		),
		create_var_record(
			"r_atmosphereextinctionstrength",
			"atmosphere fog Opacity/Extinction strength"
		),
		create_var_record(
			"r_atmospheresunstrength",
			"atmosphere enable sun"
		),
		create_var_record(
			"r_atmospherehazebasedist",
			"Distance at which haze density is 0"
		),
		create_var_record(
			"r_atmospherehazefadedist",
			"Distance over which to fade in the haze density to 1.0"
		),
		create_var_record(
			"r_atmospherepbramount",
			"0 == Not PBR, 1 == PBR "
		),
		create_var_record(
			"r_worldfogskysize",
			"world fog sky distance"
		),
		create_var_record(
			"r_exposureTweak",
			"enable the exposure dvar tweak"
		),
		create_var_record(
			"r_exposureValue",
			"exposure ev stops"
		),
		create_var_record(
			"r_exposureEyeWeight",
			"exposure eye vs. probe ratio"
		),
		create_var_record(
			"r_num_viewports",
			"number of viewports to expect to render"
		),
		create_var_record(
			"r_takeScreenShot",
			"Takes a screenshot"
		),
		create_var_record(
			"r_skyRotation",
			"Sky rotation angle."
		),
		create_var_record(
			"r_skyBoxColorIDX",
			"SkyBox Color Index"
		),
		create_var_record(
			"r_sortDrawSurfsBsp",
			"Sort BSP draw surfaces."
		),
		create_var_record(
			"r_sortDrawSurfsStaticModel",
			"Sort static model draw surfaces."
		),
		create_var_record(
			"r_videoMode",
			"pc video output setting"
		),
		create_var_record(
			"r_foveaAcuityColorMin",
			"full color angle"
		),
		create_var_record(
			"r_foveaAcuityColorMax",
			"reduced color angle"
		),
		create_var_record(
			"r_FilmIsoMin",
			"sensor / film min ISO"
		),
		create_var_record(
			"r_FilmIsoMax",
			"sensor / film max ISO"
		),
		create_var_record(
			"r_FilmIsoNoise",
			"ISO noise scale"
		),
		create_var_record(
			"r_dof_aperture_override",
			"Overrides the aperture of the lens when the dofMode is set to DOF_MAX_BLUR"
		),
		create_var_record(
			"r_dof_max_override",
			"Overrides the dof max value when the dofMode is set to DOF_MAX_BLUR and the override is non-negative"
		),
		create_var_record(
			"r_dof_min_override",
			"Overrides the dof min value when the dofMode is set to DOF_MAX_BLUR and the override is non-negative"
		),
		create_var_record(
			"r_volumetric_lighting_blur_depth_threshold",
			"Set volumetrics blur edge threshold"
		),
		create_var_record(
			"r_volumetric_lighting_upsample_depth_threshold",
			"Set volumetrics upsample edge threshold"
		),
		create_var_record(
			"r_litfog_bank_select",
			"bank select"
		),
		create_var_record(
			"r_fx_backlighting_amount",
			""
		),
		create_var_record(
			"r_paperWhite",
			"SDR paperWhite nits value"
		),
		create_var_record(
			"r_newLensFlares_offscreen_buffer_size",
			"Set the size of the out of screen buffer (in pixels based on a 1080p resolution) "
		),
		create_var_record(
			"r_newLensFlares",
			"enable the new lens flare system"
		),
		create_var_record(
			"r_motionVectorGenerateEnable",
			"toggles motion vector generation"
		),
		create_var_record(
			"r_motionVectorStaticComputeAsync",
			"enables static motion vector generation to run async"
		),
		create_var_record(
			"r_motionBlurMode",
			"motion blur mode"
		),
		create_var_record(
			"r_motionBlurStrength",
			"control motion blur strength"
		),
		create_var_record(
			"r_motionBlurQuality",
			"motion blur quality preset"
		),
		create_var_record(
			"r_shaderDebug",
			"r_shaderDebug"
		),
		create_var_record(
			"r_useCachedSpotShadow",
			"Enable pre-built spot shadow map rendering"
		),
		create_var_record(
			"r_fxShadows",
			"Particle shadow support"
		),
		create_var_record(
			"r_xanim_disableExtraChannel",
			"Disable the extra channel (tension) calculation"
		),
		create_var_record(
			"r_xanim_disableCosmeticBones",
			"Disable the cosmetic bones calculation"
		),
		create_var_record(
			"r_dedicatedPlayerShadowCull",
			"Does not render shadow map if light is facing player"
		),
		create_var_record(
			"r_dedicatedPlayerShadowCullAngle",
			"Angle from the foot to the view forward axis where the view model culling cuts off"
		),
		create_var_record(
			"r_cmdbuf_worker",
			"Process command buffer in a separate thread"
		),
		create_var_record(
			"r_addLightWorker",
			"use a worker for the add lights and probes"
		),
		create_var_record(
			"r_useSimpleDObj",
			"use the simple dobj render path"
		),
		create_var_record(
			"hkai_pathfindIterationLimit",
			"The maximum number of iterations that the astar algorithm is allowed to execute before giving up."
		),
		create_var_record(
			"hkai_additionalPathfindIterationLimit",
			"Additional number of iterations added to hkai_pathfindIterationLimit only for GenerateNavmeshPath requests."
		),
		create_var_record(
			"hkai_showTimers",
			"Whether or not to show the hkai timer debug information."
		),
		create_var_record(
			"hkai_resetTimers",
			"Reset the hkai timer debug information."
		),
		create_var_record(
			"hkai_timerTestActive",
			"Whether we're currently engaged in a timer test."
		),
		create_var_record(
			"hkai_storeClearanceRecalcStats",
			"Store results of refilling teh clearance cache to file."
		),
		create_var_record(
			"hkai_dumpMemoryLeaks",
			"Dump the outstanding havok memory allocations the next time HKAI_FreeMapData is called."
		),
		create_var_record(
			"hkai_warnPathFindFailures",
			"Show on screen print-warning if an actor fails to find a path"
		),
		create_var_record(
			"dw_sendBufSize",
			"Size in bytes for socketrouter's socket OS send buffer"
		),
		create_var_record(
			"dwNetMaxWaitMs",
			"Milliseconds we'll wait in dwgetlocalcommonaddr before giving up"
		),
		create_var_record(
			"tu5_dwNetFatalErrors",
			"Should errors encountered during DTLS handling be fatal"
		),
		create_var_record(
			"live_social_quickjoin",
			"Enable quickjoin widget: 0-disabled, 1-friends, 2-friends+groups"
		),
		create_var_record(
			"live_social_quickjoin_count",
			"number of freinds/group members in the quickjoin list"
		),
		create_var_record(
			"live_social_quickjoin_cache",
			"QuickJoin presence cache autoupdate"
		),
		create_var_record(
			"contracts_enabled_mp",
			"enables contracts"
		),
		create_var_record(
			"noDW",
			"Use DW"
		),
		create_var_record(
			"totalSampleRateQoS",
			"Total sample rate for sandbox - random + static - for QoS channel"
		),
		create_var_record(
			"totalSampleRateBlackBox",
			"Total sample rate for sandbox - random + static - for BB channel"
		),
		create_var_record(
			"totalSampleRateSurvey",
			"Total sample rate for sandbox - random + static - for Survey channel"
		),
		create_var_record(
			"totalSampleClientTrack",
			"Total sample rate for sandbox - random + static - for Survey channel"
		),
		create_var_record(
			"dwConsideredConnectedTime",
			"Time in milliseconds between the disconnect from Demonware happens and a Com_Error for that reason is thrown."
		),
		create_var_record(
			"live_useUno",
			"Use Uno Account Features"
		),
		create_var_record(
			"live_useUmbrella",
			"Use Umbrella Account Features"
		),
		create_var_record(
			"live_connect_mode",
			"LiveConnect operation mode:0-disabled, 1:auto, 2:initial auto+manual reconnect, 3:manual"
		),
		create_var_record(
			"motdDelay",
			"Delay after which the MOTD is shown again."
		),
		create_var_record(
			"motd_enabled",
			"Dvar to enable/disable the Treyarch MOTD message."
		),
		create_var_record(
			"liveNeverHostServer",
			"Set dvar to true if you like the client to never host."
		),
		create_var_record(
			"liveDedicatedOnly",
			"Set dvar to true if dedicated-only playlist"
		),
		create_var_record(
			"allowAllNAT",
			""
		),
		create_var_record(
			"live_statscaching",
			"If true will cache statchanged msgs from server and apply atomically"
		),
		create_var_record(
			"ui_enableConnectionMetricGraphs",
			"Control showing the Connection Metrics graph in the game options menu. 1 will turn on mode 1, 2 mode 2, 3 both. 0 would turn both off"
		),
		create_var_record(
			"partyChatDisallowed",
			"Whether to disallow Xbox Live Party Chat"
		),
		create_var_record(
			"live_autoEventPumpDelay",
			"Update delay for auto events"
		),
		create_var_record(
			"live_autoEventPumpTime",
			"Next pump time"
		),
		create_var_record(
			"live_autoEventEnabled",
			"Enable/disable the autoevent system"
		),
		create_var_record(
			"survey_chance",
			"Chance that a match will have post-game surveys shown"
		),
		create_var_record(
			"survey_count",
			"Number of active surveys"
		),
		create_var_record(
			"qos_minProbes",
			"Minimum probe results required before early outing qos"
		),
		create_var_record(
			"qos_minPercent",
			"Minimum percentage of probe results required before early outing qos"
		),
		create_var_record(
			"qos_firstUpdateMS",
			"MS to wait before deciding to early out qos"
		),
		create_var_record(
			"qos_lastUpdateMS",
			"MS since last update required to early out qos"
		),
		create_var_record(
			"qos_maxProbeWait",
			"Max MS for matchmaking QoS. Used only for percentage estimation."
		),
		create_var_record(
			"qos_minEchoServers",
			"Minimum number of echo servers per pop. Code will pad echo servers to this number. Set to zero to disable padding."
		),
		create_var_record(
			"qos_echo_chance",
			"Percent chance to run and capture qos echo results"
		),
		create_var_record(
			"bandwidth_retry_interval",
			"Interval at which Bandwidth test will be retried"
		),
		create_var_record(
			"reportUserInterval",
			"The interval in minutes you wait before getting another vote on this console as long as the console is turned on."
		),
		create_var_record(
			"live_presence_platform",
			"Presence through first party, 0:none, 1:basic(online/offline), 2:intitle, 3:extended(in title details), 4:extended+party"
		),
		create_var_record(
			"live_presence_incremental_fail_delay",
			"Every time console presence fails to set, the retry delay is incremented by << 1ms. This will limit exponential delay growth."
		),
		create_var_record(
			"live_presence_party",
			"Support list of party members in the presence"
		),
		create_var_record(
			"live_presence_features",
			"Bitmask for various presence features"
		),
		create_var_record(
			"live_friends_enabled",
			"Platform Friends system is enabled"
		),
		create_var_record(
			"live_friends_max",
			"Friends/Recent sorrting, 0-unsorted, 1-Most REcent, 2-Alphabetical"
		),
		create_var_record(
			"live_friends_batch_size",
			"Platform Friends system batched fetch, batch size"
		),
		create_var_record(
			"live_friends_update_interval",
			"Minimum time required before friends can be updated again."
		),
		create_var_record(
			"live_friends_sort",
			"Friends sorrting, 0-unsorted, 1-Most Recent Ingame/Online/Offline, 2-Alphabetical"
		),
		create_var_record(
			"live_friends_features",
			"Bitmask for various presence features"
		),
		create_var_record(
			"groups_enabled",
			"This is used to enable the groups feature."
		),
		create_var_record(
			"groups_self_groups_refresh_time",
			"Time to periodically update our own groups."
		),
		create_var_record(
			"groups_admins_refresh_time",
			"Time to periodically update admins for each group."
		),
		create_var_record(
			"groups_invites_refresh_time",
			"Time to periodically update group invites."
		),
		create_var_record(
			"groups_applications_refresh_time",
			"Time to periodically update group join requests."
		),
		create_var_record(
			"groups_quickjoin_players_refresh_time",
			"Time to periodically update quickjoin list with group members."
		),
		create_var_record(
			"groups_service_failure_backoff_time",
			"Time to back off before sending off any more tasks in case teams service is off."
		),
		create_var_record(
			"groups_presence_refresh_time",
			"Interval between refreshing presence for fetched group members, 0 is disabled"
		),
		create_var_record(
			"ui_hideLeaderboards",
			"Hide leaderboard buttons in the UI"
		),
		create_var_record(
			"live_leaderboardResetTime",
			"Amount of time to wait for the leaderboard reset task to finish before canceling"
		),
		create_var_record(
			"liveVoteTaskDelay",
			"Delay between consequent vote history page tasks"
		),
		create_var_record(
			"liveVoteErrorBackoff",
			"Backoff time if a vote history page task fails"
		),
		create_var_record(
			"keyarchiveWriteDelay",
			"Minimum delay between writes to keyarchive"
		),
		create_var_record(
			"inventory_enabled",
			"This is used to enable the user inventory feature."
		),
		create_var_record(
			"inventory_maxPages",
			"Max pages to fetch for player inventory."
		),
		create_var_record(
			"inventory_itemsPerPage",
			"Number of items to fetch per inventory page."
		),
		create_var_record(
			"inventory_fetch_cooloff",
			"Time in milliseconds to wait between inventory pages."
		),
		create_var_record(
			"inventory_retry_delay",
			"Time in milliseconds to wait between inventory fetch retryies."
		),
		create_var_record(
			"inventory_retry_max",
			"Max inventory fetch retryies."
		),
		create_var_record(
			"balances_retry_delay",
			"Time in milliseconds to wait between balance fetch retryies."
		),
		create_var_record(
			"balances_retry_max",
			"Max balance fetch retryies."
		),
		create_var_record(
			"inventory_blocking",
			"if false, don't wait on the inventory before allowing online play"
		),
		create_var_record(
			"rare_crate_bundle_id",
			"Loot crate Bundle ID."
		),
		create_var_record(
			"weapon_contract_incentive_id",
			"Special contract incentive item/sku id."
		),
		create_var_record(
			"weapon_contract_max",
			"Weapon contract max inventory sentinel items."
		),
		create_var_record(
			"weapon_contract_target_value",
			"Target value needed to complete the weapon contract."
		),
		create_var_record(
			"enable_weapon_contract",
			"Special Contract is enabled/disabled"
		),
		create_var_record(
			"daily_contract_cryptokey_reward_count",
			"Number of cryptokeys to reward for completing a daily contract."
		),
		create_var_record(
			"weekly_contract_cryptokey_reward_count",
			"Number of cryptokeys to reward for completing a set of weekly contracts."
		),
		create_var_record(
			"weekly_contract_blackjack_contract_reward_count",
			"Number of Blackjack contracts to reward for completing a set of weekly contracts."
		),
		create_var_record(
			"skip_contract_rewards",
			"Skip giving rewards for the special contract and daily/weekly contracts for debug purposes."
		),
		create_var_record(
			"incentive_rare_drop_id",
			"ID for rare crates awarded by contract or season pass incentives"
		),
		create_var_record(
			"incentive_weapon_drop_id",
			"ID for weapon bribe crates awarded by contract or season pass incentives"
		),
		create_var_record(
			"platformSessionShowErrorCodes",
			"Show platform session error codes"
		),
		create_var_record(
			"platformSessionPartyPrivacy",
			"Show platform session party privacy"
		),
		create_var_record(
			"platformSessionLaunchInviteJoinProcessDelay",
			"Platform session launch invite join process delay"
		),
		create_var_record(
			"live_umbrella_maxUmbrellaLoginAttempts",
			"The maximum # of attempts we will make toauth Umbrella for a given controller"
		),
		create_var_record(
			"fshSearchTaskDelay",
			"Gap in milliseconds between file share search tasks."
		),
		create_var_record(
			"fshThrottleEnabled",
			"Whether the dcache upload throttling is enabled."
		),
		create_var_record(
			"fshThrottleKBytesPerSec",
			"Dcache upload throttle limit in K Bytes per second."
		),
		create_var_record(
			"fileshare_enabled",
			"Enable or disable the Fileshare feature."
		),
		create_var_record(
			"fileshare_tier",
			"current fileshare tier for all users on this console."
		),
		create_var_record(
			"fshRecentsXUID",
			"Override recent games to come from this player instead"
		),
		create_var_record(
			"fshSummaryDelay",
			"Delay between summary tasks. Tweak to ensure no BD_TOO_MANY_TASKS."
		),
		create_var_record(
			"fshRetryDelay",
			"Delay after which a failed fileshare fetch will be retried"
		),
		create_var_record(
			"fshEnableRender",
			"Turn on or off the ability to render clips."
		),
		create_var_record(
			"fileshareRetry",
			"Retry fetching fileshare data on failure."
		),
		create_var_record(
			"fileshareAllowDownload",
			"Allow Fileshare downloads"
		),
		create_var_record(
			"fileshareAllowDownloadingOthersFiles",
			"Allow downloading other users' Fileshare files"
		),
		create_var_record(
			"fileshareAllowPaintjobDownload",
			"Allow Fileshare Paintjob downloads"
		),
		create_var_record(
			"fileshareAllowEmblemDownload",
			"Allow Fileshare Emblem downloads"
		),
		create_var_record(
			"emblemVersion",
			"Version for Emblems."
		),
		create_var_record(
			"paintjobVersion",
			"Version for Paintjob."
		),
		create_var_record(
			"enable_camo_materials_tab",
			"Enable the camo materials tab."
		),
		create_var_record(
			"live_username",
			"DW userename"
		),
		create_var_record(
			"profileDirtyInterval",
			"minimum interval (in milliseconds) between updating our profile, except when an update is forced"
		),
		create_var_record(
			"profileGetInterval",
			"minimum interval between own profile retrieval"
		),
		create_var_record(
			"groupUploadInterval",
			"Minimum interval to wait before setting new group counts"
		),
		create_var_record(
			"groupDownloadInterval",
			"Minimum interval to wait before getting new group counts"
		),
		create_var_record(
			"groupCountsVisible",
			"Toggles the group counts in the lobby and playlist menus."
		),
		create_var_record(
			"pcache_privacy",
			"Controlls the support for fetching Privacy settings from 1st party through pcache"
		),
		create_var_record(
			"loot_cryptokeyCost",
			"lootxp required to buy one crypto key. Must match the DW backend modifier."
		),
		create_var_record(
			"loot_cryptokeySku",
			"SKU used to purchase crypto keys by spending lootxp. Exchange rate is governed by 'crypto_key_cost'. Must match sku setup on DW."
		),
		create_var_record(
			"lootxp_multiplier",
			"multiplier for double lootxp"
		),
		create_var_record(
			"loot_commonCrate_dwid",
			"common crate dw id"
		),
		create_var_record(
			"loot_bribeCrate_dwid",
			"bribe 1 crate dw id"
		),
		create_var_record(
			"loot_rareCrate_dwid",
			"common crate dw id"
		),
		create_var_record(
			"loot_mpItemVersions",
			"MP loot item version"
		),
		create_var_record(
			"loot_zmItemVersions",
			"ZM loot item version"
		),
		create_var_record(
			"loot_enabled",
			"Enabled or disables loot"
		),
		create_var_record(
			"loot_cryptoCheckDelay",
			"Cooloff period checking for lootxp->cryptokey conversion"
		),
		create_var_record(
			"tu11_lootCryptoAutoRetry",
			"Enabled or disables auto lootxp->cryptokey conversion"
		),
		create_var_record(
			"loot_burnBatchSize",
			"Batch size for burning duplicate items"
		),
		create_var_record(
			"loot_burnCooloff",
			"Cooloff period before burning the next batch of items"
		),
		create_var_record(
			"loot_burnRefetchOnSuccess",
			"Refetch inventory when a burn is successful"
		),
		create_var_record(
			"loot_burnCommonRefund",
			"Cryptokeys refunded when a Common item is burned"
		),
		create_var_record(
			"loot_burnRareRefund",
			"Cryptokeys refunded when a Rare item is burned"
		),
		create_var_record(
			"loot_burnLegendaryRefund",
			"Cryptokeys refunded when a Legendary item is burned"
		),
		create_var_record(
			"loot_burnEpicRefund",
			"Cryptokeys refunded when an Epic item is burned"
		),
		create_var_record(
			"loot_burnMinMegaRequired",
			"Minimum number of mega gobblegum required to burn"
		),
		create_var_record(
			"loot_burnMinRareRequired",
			"Minimum number of rare gobblegum required to burn"
		),
		create_var_record(
			"loot_burnMinUltraRequired",
			"Minimum number of ultra gobblegum required to burn"
		),
		create_var_record(
			"live_store_enable",
			"Enable/Disable Store button in UI."
		),
		create_var_record(
			"live_store_enable_inventory",
			"Enable/Disable inventory fetch."
		),
		create_var_record(
			"store_item_viewed_timer",
			"Minimum time (in ms) player should look at a store item to register a comscore event"
		),
		create_var_record(
			"live_store_show_details",
			"Enable/Disable product browse."
		),
		create_var_record(
			"live_store_disable_lang",
			"Disable store for the available language based on value of dvar loc_availableLanguages."
		),
		create_var_record(
			"live_store_disable_region",
			"Disable store for the given SKU region."
		),
		create_var_record(
			"live_enablePolls",
			"If true, polls will fire off demonware tasks"
		),
		create_var_record(
			"xblive_matchEndingSoon",
			"True if the match is ending soon"
		),
		create_var_record(
			"live_pubSemaphoreCheckIntervalSeconds",
			"Interval in seconds between checking the backend to see if the pubsemaphore timestamp has changed"
		),
		create_var_record(
			"live_pubSemaphoreJitterSeconds",
			"Jitter in seconds to apply to live_pubSemaphoreCheckIntervalSeconds"
		),
		create_var_record(
			"live_pubSemaphoreForceChange",
			"If true pubsemaphore will always signal as changed"
		),
		create_var_record(
			"live_pubSemaphoreUserTriggerTime",
			"time when the user confirmed a game settings changed popup"
		),
		create_var_record(
			"stat_version",
			"Stats version number"
		),
		create_var_record(
			"stats_version_check",
			"Reset stats if version numbers do not match"
		),
		create_var_record(
			"maxStatsBackupInterval",
			"Maximum number of days before performing stats backup to Demonware User Storage"
		),
		create_var_record(
			"zero_stats_check",
			"Check for zeroed out stats from Demonware"
		),
		create_var_record(
			"dwFileFetchTryIntervalBase",
			"Delay in seconds after file fetch fails to the next retry.  Exponential growth each retry."
		),
		create_var_record(
			"dwFileFetchTryIntervalMax",
			"Max delay in seconds between file fetch tries."
		),
		create_var_record(
			"dwFileFetchTryMaxAttempts",
			"Max retry attempts before stopping altogether."
		),
		create_var_record(
			"waitOnStatsTimeout",
			"Time in seconds to wait for stats to be fetched while dev mapping."
		),
		create_var_record(
			"minDelayForOtherPlayerStatsFetch",
			"Delay in milliseconds to refetch the stats of a player."
		),
		create_var_record(
			"statsLocationFatal",
			"If true, a zombie stats error will cause the game to end, if false a warning is printed to the console and the game continues"
		),
		create_var_record(
			"statsCheckXuidBeforeStatsUpload",
			"Checks the XUID of the stats before uploading. Used as bitmask for each eMode(1 << mode)"
		),
		create_var_record(
			"statsErrorNormal",
			"Error if an invalid stats access is requested on NORMAL stats."
		),
		create_var_record(
			"statsErrorStable",
			"Error if an invalid stats access is requested on STABLE stats."
		),
		create_var_record(
			"statsErrorOtherPlayer",
			"Error if an invalid stats access is requested on OTHERPLAYER stats."
		),
		create_var_record(
			"livestorage_fakeNoob",
			"Force this player to be treated like a noob"
		),
		create_var_record(
			"purchasedClassSetCount",
			"Number of class sets available when purchased"
		),
		create_var_record(
			"defaultClassSetCount",
			"Number of class sets available when not purchased"
		),
		create_var_record(
			"live_timewarp",
			"Offset calculated live time by this amount of days"
		),
		create_var_record(
			"tu9_backupBadDecompress",
			"Create an online backup when decompression fails"
		),
		create_var_record(
			"tu9_backupBadSpace",
			"Create an online backup when there is insufficient space for decompression"
		),
		create_var_record(
			"tu10_validationFatal",
			"Disconnect completely when a stats validation error is detected"
		),
		create_var_record(
			"tu12_delayFFOTD",
			"ffotd waits on the playlist to first be downloaded, the execs everything in the proper sequence."
		),
		create_var_record(
			"tu10_enableCyclicalDailyChallenges",
			"Enable cyclical daily challenges"
		),
		create_var_record(
			"skill_teamBeta",
			"Beta value for team based skill calculation"
		),
		create_var_record(
			"skill_onboardingEnter",
			"Initial skill for players entering the onboarding pool"
		),
		create_var_record(
			"skill_onboardingExit",
			"Initial skill for players leaving the onboarding pool"
		),
		create_var_record(
			"skill_onboardingThreshold",
			"Skill value that will force players out of the onboarding pool"
		),
		create_var_record(
			"skill_onboardingGames",
			"Number of games to player before moving the player out of the onboarding pool"
		),
		create_var_record(
			"skill_onboardingAdjustOnExit",
			"Whether of not skill values should be adjusted when exiting onboarding pool"
		),
		create_var_record(
			"skill_onboardingEnabled",
			"Whether or not onboarding is enabled for new players."
		),
		create_var_record(
			"allowHost_matchesHostedRatio",
			"Required match completion/hosted ratio. 0 is off."
		),
		create_var_record(
			"allowHost_matchesPlayedRatio",
			"Required match completion/played ratio. 0 is off."
		),
		create_var_record(
			"disableHost_matchesHostedRatio",
			"Required match hosted completion/failure ratio. 0 is off."
		),
		create_var_record(
			"disableHost_matchesPlayedRatio",
			"Required match played completion/failure ratio. 0 is off."
		),
		create_var_record(
			"behaviorTrackerVersionNumber",
			"Version number for the behavior tracker"
		),
		create_var_record(
			"probation_version",
			""
		),
		create_var_record(
			"probation_public_enabled",
			"Probation enabled"
		),
		create_var_record(
			"probation_public_quitScalar",
			"How much each quit is factored into the probation"
		),
		create_var_record(
			"probation_public_timeoutScalar",
			"How much each timeout is factored into the probation"
		),
		create_var_record(
			"probation_public_kickScalar",
			"How much each kick is factored into the probation"
		),
		create_var_record(
			"probation_public_matchHistoryWindow",
			"How many games are evaluated to determin probation."
		),
		create_var_record(
			"probation_public_probationThreshold",
			"How many violations puts the player over the limit."
		),
		create_var_record(
			"probation_public_warningThreshold",
			"How many violations causes the player to get warned."
		),
		create_var_record(
			"probation_public_probationTime",
			"Time the player gets punished in minutes."
		),
		create_var_record(
			"probation_public_forgiveCount",
			"How many violations to forgive when probation is given"
		),
		create_var_record(
			"probation_league_enabled",
			"Probation enabled"
		),
		create_var_record(
			"probation_league_quitScalar",
			"How much each quit is factored into the probation"
		),
		create_var_record(
			"probation_league_timeoutScalar",
			"How much each timeout is factored into the probation"
		),
		create_var_record(
			"probation_league_kickScalar",
			"How much each kick is factored into the probation"
		),
		create_var_record(
			"probation_league_dashboardScalar",
			"How much each dashboard is factored into the probation"
		),
		create_var_record(
			"probation_league_matchHistoryWindow",
			"How many games are evaluated to determin probation."
		),
		create_var_record(
			"probation_league_probationThreshold",
			"How many violations puts the player over the limit."
		),
		create_var_record(
			"probation_league_warningThreshold",
			"How many violations causes the player to get warned."
		),
		create_var_record(
			"probation_league_probationTime",
			"Time the player gets punished in minutes."
		),
		create_var_record(
			"probation_league_forgiveCount",
			"How many violations to forgive when probation is given"
		),
		create_var_record(
			"fixNegativeLosses",
			"Fix negative losses in combat record"
		),
		create_var_record(
			"livestats_skipFirstTime",
			"Disables first time flow for all session modes."
		),
		create_var_record(
			"loot_taskWindow",
			"time alloted to the reward task, after which a retry will occur"
		),
		create_var_record(
			"loot_taskMaxAttempts",
			"max retries before the reward task is considered a failure"
		),
		create_var_record(
			"tu4_enableCodPoints",
			"Enable display and spending of COD Points"
		),
		create_var_record(
			"local_media_tier",
			"current local media tier for all users on this console."
		),
		create_var_record(
			"statsVersionMP",
			"design version for MP stats"
		),
		create_var_record(
			"statsVersionCP",
			"design version for CP stats"
		),
		create_var_record(
			"statsVersionZM",
			"design version for ZM stats"
		),
		create_var_record(
			"statsVersionFR",
			"design version for FR stats"
		),
		create_var_record(
			"loadoutVersionMP",
			"design version for MP loadouts"
		),
		create_var_record(
			"loadoutVersionZM",
			"design version for ZM loadouts"
		),
		create_var_record(
			"lobby_hostBots",
			"Enables lobby bots"
		),
		create_var_record(
			"lobbyvm_forceGC",
			"Force garbage collection after lobby pump to keep memory usage in check"
		),
		create_var_record(
			"ui_lobbyDebugVis",
			"master switch for turning on or off lobby debug ui"
		),
		create_var_record(
			"ui_lobbyDebugClients",
			"Enables and sets the Lobby Type for Client Debugging"
		),
		create_var_record(
			"ui_lobbyDebugOverlay",
			"Turn Lobby Debug Overlay ON/OFF"
		),
		create_var_record(
			"ui_lobbyDebugBaseX",
			"debug ui base x"
		),
		create_var_record(
			"ui_lobbyDebugBaseY",
			"debug ui base x"
		),
		create_var_record(
			"ui_lobbyDebugExtraInfoLoc",
			"game mode loc"
		),
		create_var_record(
			"ui_lobbyDebugSessionSQJ",
			"Enables/Disables session search debugging"
		),
		create_var_record(
			"ui_lobbyDebugVoice",
			"Enables/Disables lobby voice debugging"
		),
		create_var_record(
			"ui_abortKeyboardOnUIChange",
			"Abort any onscreen keyboard operations when the lobby system changes the UI screen"
		),
		create_var_record(
			"tu11_matchmakingPriorityMP",
			"MP matchmaking priority setting"
		),
		create_var_record(
			"tu11_matchmakingPriorityMPIgnoreTime",
			"MP amount of time that a session will be ignored for"
		),
		create_var_record(
			"party_minplayers",
			"Minimum number of players in a party"
		),
		create_var_record(
			"party_maxplayers",
			"Maximum number of players in a party"
		),
		create_var_record(
			"party_autoteams",
			"Enables/Disables auto team functionality"
		),
		create_var_record(
			"lobby_maxLocalPlayers",
			"Enables/Disables auto team functionality"
		),
		create_var_record(
			"lobby_forceLAN",
			"Force LAN lobby if > 0, 1-LAN offline, 2-LAN and connected to dw"
		),
		create_var_record(
			"lobby_showSVDebug",
			"shows sv debug info in the frontend"
		),
		create_var_record(
			"lobbymigrate_MaxBWPackets",
			"number of bw test packets to send when we have the token"
		),
		create_var_record(
			"lobbymigrate_MaxPacketWaitTime",
			"ms to wait for test packet responses after last packet is sent"
		),
		create_var_record(
			"lobbymigrate_NomineeWaitMS",
			"ms to wait for each nominee to send us a partystate"
		),
		create_var_record(
			"lobbymigrate_HostWaitMS",
			"ms to send hostannounce packets before sending partystates"
		),
		create_var_record(
			"lobbymigrate_NomineeRecalcInterval",
			"ms to wait between recalculating migration nominees"
		),
		create_var_record(
			"lobbymigrate_TestInterval",
			"base ms to wait between test rounds, see lobbymigrate_TestIntervalJitter"
		),
		create_var_record(
			"lobbymigrate_TestIntervalJitter",
			"ms jitter between test rounds"
		),
		create_var_record(
			"lobbymigrate_MinScoreDiff",
			"minimum score nominee must differ by before rank will change"
		),
		create_var_record(
			"lobbymigrate_WirelessLatencyIncrease",
			"percentage increase for wireless client latency during host selection"
		),
		create_var_record(
			"lobbymigrate_useStdDev",
			"If true we'll use the standard devation of the clients latency as a tie-breaker instead of the clientnum"
		),
		create_var_record(
			"lobbyMigrate_Enabled",
			"Set to false to disable lobby migration"
		),
		create_var_record(
			"lobbyMigrate_EnabledLAN",
			"Set to false to disable lobby migration in LAN mode"
		),
		create_var_record(
			"lobbyMigrate_migrateToBestGameHostEnabled",
			"Set to false to disable migrate to best game host migration"
		),
		create_var_record(
			"lobbyMigrate_dedicatedOnly",
			"only allow dedicated servers to host"
		),
		create_var_record(
			"lobbyMigrate_considerStreamingForHostMigration",
			"When true, will consider the factor of whether the host is streaming or not, when nominating for host migration"
		),
		create_var_record(
			"lobby_clientTimeout",
			"Seconds with no received packets from host until a timeout occurs"
		),
		create_var_record(
			"lobby_clientWarnTimeout",
			"Seconds with no received packets from host until a host missing warning is displayed"
		),
		create_var_record(
			"lobby_clientContentTimeout",
			"Milli-Seconds to finish sending the active client data until a timeout occurs"
		),
		create_var_record(
			"lobby_clientEndCooloffTime",
			"Miliseconds before dropping net chan channels to host"
		),
		create_var_record(
			"lobby_clientEndFakeSendLoss",
			"When a match end and are returning to the front end, check if we have full loaded the UI level"
		),
		create_var_record(
			"lobby_clientLoadingIntoUICheck",
			"When a match end and are returning to the front end, check if we have full loaded the UI level"
		),
		create_var_record(
			"lobbyLaunch_fadeToBlackDelay",
			"Once the launch game timer has started, when the fade to black will happen"
		),
		create_var_record(
			"lobbyLaunch_gameLaunchDelay",
			"Once the launch game timer has started, when the launch will happen"
		),
		create_var_record(
			"lobbyLaunch_fadeToBlackDelayOnConnect",
			"Once the launch game timer has started, when the fade to black will happen"
		),
		create_var_record(
			"lobbyLaunch_gameLaunchDelayOnConnect",
			"Once the launch game timer has started, when the launch will happen"
		),
		create_var_record(
			"lobbyLaunch_waitForClientAckDelay",
			"Time to wait for acknowledgments from the clients before launching"
		),
		create_var_record(
			"lobby_hostTimeout",
			"seconds without any message from client before they are disconnected"
		),
		create_var_record(
			"lobby_hostIntervalState",
			"lobby state message intervals"
		),
		create_var_record(
			"lobby_hostIntervalHeartbeat",
			"lobby heartbeat message intervals"
		),
		create_var_record(
			"lobby_hostContentTimeout",
			"Seconds to finish sending the host content until a timeout occurs"
		),
		create_var_record(
			"lobby_enablePSDataStreamingInGame",
			"Turn on the in-game streaming of paintshop data to all clients marked dirty"
		),
		create_var_record(
			"lobby_enableLoadoutDataStreamingInGame",
			"Turn on the in-game streaming of loadout data to all clients marked dirty"
		),
		create_var_record(
			"lobby_hostPSFragmentRetryInterval",
			"Paintshop data fragment retry interval"
		),
		create_var_record(
			"lobby_enablePSDataStreamingInLobby",
			"Turn on the paintshop data to be shared in the frontend lobbies"
		),
		create_var_record(
			"lobby_ingamePSDataStreamingBW",
			"Control In-Game Paintshop data streaming Bandwidth(total bumber of paintshop bytes to be sent per snapshop)"
		),
		create_var_record(
			"lobby_moveLobby",
			"seconds before a lobby move is considered a failure"
		),
		create_var_record(
			"lobby_hostJoinRequestReject",
			"Have the host reject incomming connections with random reasons"
		),
		create_var_record(
			"join_agreementWindow",
			"all clients must agree to join the new host in this window"
		),
		create_var_record(
			"join_handshakeWindow",
			"server must respond to reservation request within this window"
		),
		create_var_record(
			"join_completionWindow",
			"once a join is complete, the old host remains active for this amount of time before deactivating itself"
		),
		create_var_record(
			"join_infoProbeWindow",
			"time to wait for an info probe IM response"
		),
		create_var_record(
			"join_retryDelay",
			"if the host is busy, we'll try re-joining after this cooloff"
		),
		create_var_record(
			"join_retryCount",
			"if the host is still busy after these many retries, move on"
		),
		create_var_record(
			"join_reservationWindow",
			"host join reservation window in ms"
		),
		create_var_record(
			"lobbymsg_prints",
			"Turns on and off the message debug prints"
		),
		create_var_record(
			"lobbyDebugLogJoins",
			"Logs join results to the kvs"
		),
		create_var_record(
			"lobbyDebugLogJoinSuccess",
			"Includes join success results while logging"
		),
		create_var_record(
			"lobby_ptptimeout",
			"Time out for connection test"
		),
		create_var_record(
			"partyPrivacyEnabled",
			"Lobby party privacy on/off"
		),
		create_var_record(
			"partyPrivacyPumpEnabled",
			"Lobby party privacy pump on/off"
		),
		create_var_record(
			"partyPrivacyCheckInterval",
			"Party privacy check interval"
		),
		create_var_record(
			"leaveWithPartyEnabled",
			"Lobby leave with party on/off"
		),
		create_var_record(
			"lobbyMergeEnabled",
			"Lobby merge on/off"
		),
		create_var_record(
			"lobbyMergeInterval",
			"Lobby merge interval"
		),
		create_var_record(
			"lobbyMergeDedicatedEnabled",
			"Lobby dedicated merge on/off"
		),
		create_var_record(
			"lobbyAdvertiseSkip",
			"Lobby Advertise: Skip advertisment"
		),
		create_var_record(
			"lobbyAdvertiseDirty",
			"Lobby Advertise: Session is dirty/needs updating"
		),
		create_var_record(
			"lobbyAdvertiseMaxPlayers",
			"Lobby Advertise Param: Max Players"
		),
		create_var_record(
			"lobbyAdvertiseNumPlayers",
			"Lobby Advertise Param: Num Players"
		),
		create_var_record(
			"lobbyAdvertiseServerType",
			"Lobby Advertise Param: server type"
		),
		create_var_record(
			"lobbyAdvertiseShowInMatchmaking",
			"Lobby Advertise Param: Show in matchmaking"
		),
		create_var_record(
			"lobbyAdvertiseNetcodeVersion",
			"Lobby Advertise Param: Netcode version"
		),
		create_var_record(
			"lobbyAdvertiseMapPacks",
			"Lobby Advertise Param: Map packs"
		),
		create_var_record(
			"lobbyAdvertisePlaylistVersion",
			"Lobby Advertise Param: Playlist Version"
		),
		create_var_record(
			"lobbyAdvertisePlaylistNumber",
			"Lobby Advertise Param: Playlist Number"
		),
		create_var_record(
			"lobbyAdvertiseIsEmpty",
			"Lobby Advertise Param: Is Empty"
		),
		create_var_record(
			"lobbyAdvertiseTeamSizeMax",
			"Lobby Advertise Param: Team Size Max"
		),
		create_var_record(
			"lobbyAdvertiseSkill",
			"Lobby Advertise Param: Skill level"
		),
		create_var_record(
			"lobbyAdvertiseGeo1",
			"Lobby Advertise Param: Geographical loc 1"
		),
		create_var_record(
			"lobbyAdvertiseGeo2",
			"Lobby Advertise Param: Geographical loc 2"
		),
		create_var_record(
			"lobbyAdvertiseGeo3",
			"Lobby Advertise Param: Geographical loc 3"
		),
		create_var_record(
			"lobbyAdvertiseGeo4",
			"Lobby Advertise Param: Geographical loc 4"
		),
		create_var_record(
			"lobbyAdvertiseServerLocation",
			"Server location to advertise"
		),
		create_var_record(
			"lobbyAdvertiseLatencyBand",
			"Server latency band to advertise"
		),
		create_var_record(
			"lobbyAdvertiseMap",
			"Lobby Advertise Param: Map"
		),
		create_var_record(
			"lobbySearchSkip",
			"Lobby Search: Skip searches"
		),
		create_var_record(
			"lobbyDedicatedSearchSkip",
			"Lobby Search: Skip dedicated server searches"
		),
		create_var_record(
			"lobbySearchDelay",
			"Lobby Search: Time delay between searches"
		),
		create_var_record(
			"lobbySearchGeoMin",
			"Lobby Search: Minimum number of geo tiers that must match"
		),
		create_var_record(
			"lobbySearchQueryId",
			"Lobby Search Param: Query ID"
		),
		create_var_record(
			"lobbySearchGameSecurityId",
			"Lobby Search Param: Game security id, used in communication encryption"
		),
		create_var_record(
			"lobbySearchShowInMatchmaking",
			"Lobby Search Param: Show in matchmaking, flag for shown or hidden games"
		),
		create_var_record(
			"lobbySearchNetcodeVersion",
			"Lobby Search Param: Netcode Version, of the code base"
		),
		create_var_record(
			"lobbySearchMapPacks",
			"Lobby Search Param: Search for emabled map packs"
		),
		create_var_record(
			"lobbySearchPlaylistVersion",
			"Lobby Search Param: Playlist version of session"
		),
		create_var_record(
			"lobbySearchPlaylistNumber",
			"Lobby Search Param: Playlist number of session"
		),
		create_var_record(
			"lobbySearchServerType",
			"Lobby Search Param: Server type of session"
		),
		create_var_record(
			"lobbySearchIsEmpty",
			"Lobby Search Param: If the session is empty"
		),
		create_var_record(
			"lobbySearchTeamSize",
			"Lobby Search Param: Team size"
		),
		create_var_record(
			"lobbySearchSkill",
			"Lobby Search Param: Session skill level"
		),
		create_var_record(
			"lobbySearchSkillWeight",
			"Lobby Search Param: Session skill level weight"
		),
		create_var_record(
			"lobbySearchGeo1",
			"Lobby Search Param: Geographical loc 1"
		),
		create_var_record(
			"lobbySearchGeo2",
			"Lobby Search Param: Geographical loc 2"
		),
		create_var_record(
			"lobbySearchGeo3",
			"Lobby Search Param: Geographical loc 3"
		),
		create_var_record(
			"lobbySearchGeo4",
			"Lobby Search Param: Geographical loc 4"
		),
		create_var_record(
			"lobbySearchPingBand",
			"Lobby search param: Ping band"
		),
		create_var_record(
			"lobbySearchDediUnparkPingLimit",
			"Limit on RTT in milliseconds to a given serverloc we'll consider for unparking"
		),
		create_var_record(
			"lobbySearchServerLocation1",
			"Lobby search param: Server location 1"
		),
		create_var_record(
			"lobbySearchServerLocation2",
			"Lobby search param: Server location 2"
		),
		create_var_record(
			"lobbySearchServerLocation3",
			"Lobby search param: Server location 3"
		),
		create_var_record(
			"lobbySearchServerLocation4",
			"Lobby search param: Server location 4"
		),
		create_var_record(
			"lobbySearchServerLocation5",
			"Lobby search param: Server location 5"
		),
		create_var_record(
			"lobbySearchPingBandWeight1",
			"Lobby Search Param: Ping band weight1 for dedicated matchmaking query"
		),
		create_var_record(
			"lobbySearchPingBandWeight2",
			"Lobby Search Param: Ping band weight2 for dedicated matchmaking query"
		),
		create_var_record(
			"lobbySearchPingBandWeight3",
			"Lobby Search Param: Ping band weight3 for dedicated matchmaking query"
		),
		create_var_record(
			"lobbySearchPingBandWeight4",
			"Lobby Search Param: Ping band weight4 for dedicated matchmaking query"
		),
		create_var_record(
			"lobbySearchPingBandWeight5",
			"Lobby Search Param: Ping band weight5 for dedicated matchmaking query"
		),
		create_var_record(
			"lobbySearchForceLocation",
			"If true we'll ignore qos results and only search for sessions in this location"
		),
		create_var_record(
			"lobbySearchDatacenterType",
			"Do we look for Gameservers or Thunderhead datacenters?"
		),
		create_var_record(
			"lobbySearchDatacenterTypeGeo",
			"Gameservers/Thunderhead geo-filter"
		),
		create_var_record(
			"lobbySearchDatacenterTypeOverride",
			"Do we look for Gameservers or Thunderhead datacenters - override if != ANY?"
		),
		create_var_record(
			"lobbySearchGeo1Weight",
			"Lobby Search Param: Geographical loc latitude weight for weighted distance matchmaking queries"
		),
		create_var_record(
			"lobbySearchGeo2Weight",
			"Lobby Search Param: Geographical loc latitude weight for weighted distance matchmaking queries"
		),
		create_var_record(
			"lobbySearchGeo3Weight",
			"Lobby Search Param: Geographical loc latitude weight for weighted distance matchmaking queries"
		),
		create_var_record(
			"lobbySearchGeo4Weight",
			"Lobby Search Param: Geographical loc latitude weight for weighted distance matchmaking queries"
		),
		create_var_record(
			"lobbySearchMaxLatencyBandDiff",
			"Max diff in latency before we reject this host as a possible host"
		),
		create_var_record(
			"lobbySearchMinDediSearchTime",
			"The minimum amount of time required to search/unpark dedi before any listen search"
		),
		create_var_record(
			"lobbySearchMinDediSearchClientAdd",
			"The amount of time added to minimum dedi search/unpark for each party member"
		),
		create_var_record(
			"lobbySearchBaseSkillRange",
			"Base skill range for public matchmaking"
		),
		create_var_record(
			"lobbySearchSkillRangeMultiplier",
			"Rate at which skill range increases for each stage retry"
		),
		create_var_record(
			"lobbySearchForceUnparkLobbySize",
			"Party size that will force a dedicated server unpark"
		),
		create_var_record(
			"lobbySearchSkipUnparkProbability",
			"Probability that we skip unparking"
		),
		create_var_record(
			"lobbySearchSkipDLCProbability",
			"Probability we'll search for original maps instead of DLC1"
		),
		create_var_record(
			"lobbySearchPingBandEnabled",
			"If true set the lobbySearchPingBand value before the search"
		),
		create_var_record(
			"lobbySearchExperimentDatacenter",
			"Datacenter to use for pingband experiment"
		),
		create_var_record(
			"lobbySearchForceXuid",
			"If set, all search results will be discarded except for specified xuid"
		),
		create_var_record(
			"qosPreferredPing",
			"QoS preferred ping"
		),
		create_var_record(
			"qosMaxAllowedPing",
			"QoS max allowed ping"
		),
		create_var_record(
			"qosLocklessQueue",
			"If true we'll queue up qos results locklessly and push them to the VM on the main thread later"
		),
		create_var_record(
			"excellentPing",
			"Anything less than this is consider excellent"
		),
		create_var_record(
			"goodPing",
			"Anything less thant this is considered decent"
		),
		create_var_record(
			"terriblePing",
			"Just awful, limit of playability"
		),
		create_var_record(
			"lobbyTimerStartInterval",
			"Lobby timer interval for start match"
		),
		create_var_record(
			"lobbyCPTimerStartInterval",
			"Lobby CP timer interval for start match"
		),
		create_var_record(
			"lobbyCPZMTimerStartInterval",
			"Lobby CPZM or Nightmares timer interval for start match"
		),
		create_var_record(
			"lobbyTimerStatusVotingInterval",
			"Lobby timer interval for voting"
		),
		create_var_record(
			"lobbyTimerStatusBeginInterval",
			"Lobby timer interval for match begin"
		),
		create_var_record(
			"lobbyTimerStatusStartInterval",
			"Lobby timer interval for match start"
		),
		create_var_record(
			"lobbyTimerStatusPostGameInterval",
			"Lobby timer interval for post match"
		),
		create_var_record(
			"lobbyTimerStatusVotingInterval_Arena",
			"Lobby timer interval for arena pro voting"
		),
		create_var_record(
			"lobbyTimerZMStartInterval",
			"Lobby timer interval for zm start match"
		),
		create_var_record(
			"lobbyMapVotePrevMapPlayCount",
			"Number of times a previous map cazn be voted for"
		),
		create_var_record(
			"lobby_beginPlay",
			"use to signal UI_CoD_KeyEvent to force beginPlay on the Main thread"
		),
		create_var_record(
			"lobby_readyUpPercentRequired",
			"The fraction of the waiting players who need to be ready for a party to start"
		),
		create_var_record(
			"lobbyProbedXuidOverride",
			"How long to check new connecting clients against the probed xuids of already connected clients"
		),
		create_var_record(
			"lobby_MatchmakingLoggingLevel",
			"0 = off, 1 = all on, etc."
		),
		create_var_record(
			"lobby_MatchmakingLoggingChance",
			"Probability of uploading a telemetry event record.  Throttle control."
		),
		create_var_record(
			"ui_lobbyDebugJoinLoc",
			"lobby Join debug loc"
		),
		create_var_record(
			"ui_lobbyDebugMsgLoc",
			"lobby Msg debug loc"
		),
		create_var_record(
			"ui_lobbyDebugReservationLoc",
			"lobby Reservation debug loc"
		),
		create_var_record(
			"ui_lobbyDebugResCountLoc",
			"lobby ResCount debug loc"
		),
		create_var_record(
			"ui_lobbyDebugAgreementLoc",
			"lobby Agreement debug loc"
		),
		create_var_record(
			"ui_lobbyDebugAgreeCountLoc",
			"lobby AgreeCount debug loc"
		),
		create_var_record(
			"ui_lobbyDebugPrivateHostSessionLoc",
			"lobby PrivateHostSession debug loc"
		),
		create_var_record(
			"ui_lobbyDebugPrivateClientSessionLoc",
			"lobby PrivateClientSession debug loc"
		),
		create_var_record(
			"ui_lobbyDebugGameHostSessionLoc",
			"lobby GameHostSession debug loc"
		),
		create_var_record(
			"ui_lobbyDebugGameClientSessionLoc",
			"lobby GameClientSession debug loc"
		),
		create_var_record(
			"ui_lobbyDebugDWSessionLoc",
			"lobby DWSession debug loc"
		),
		create_var_record(
			"ui_lobbyDebugLobbyTasksLoc",
			"lobby LobbyTasks debug loc"
		),
		create_var_record(
			"ui_lobbyDebugVoiceHostLoc",
			"lobby VoiceHost debug loc"
		),
		create_var_record(
			"ui_lobbyDebugVoiceClientLoc",
			"lobby VoiceClient debug loc"
		),
		create_var_record(
			"ui_lobbyDebugPlatformSessionLoc",
			"lobby PlatformSession debug loc"
		),
		create_var_record(
			"lpc_forceDownloadFiles",
			"Force downloading of files even when they pass the FSCK"
		),
		create_var_record(
			"marketing_enabled",
			"Used to turn off/on the marketing mtx comms system"
		),
		create_var_record(
			"marketing_refreshTime",
			"The refresh time for the marketing messsages."
		),
		create_var_record(
			"marketing_autorefresh",
			"Automatically download new messages after reporting any message read"
		),
		create_var_record(
			"marketing_waitTimeOnLogin",
			"Used to enable the wait on fetching marketing messages on login. Time value in ms."
		),
		create_var_record(
			"constBaseline_allow",
			"If true, enable use of const baselines"
		),
		create_var_record(
			"net_ip",
			"Network IP address"
		),
		create_var_record(
			"net_port",
			"Network port"
		),
		create_var_record(
			"playlist_debug",
			"Enable the debug display for additional playlist information"
		),
		create_var_record(
			"playlist_linearMapCycle",
			"We will cycle through the maps linearly if this is true."
		),
		create_var_record(
			"playlist_linearMapCycleRandom",
			"We will cycle through the maps linearly if this is true but picks a random start map."
		),
		create_var_record(
			"playlist_excludeMap",
			""
		),
		create_var_record(
			"playlist_excludeGametype",
			""
		),
		create_var_record(
			"playlist_allowdevmaps",
			"Allow maps that aren't in the map/presence table to be selected"
		),
		create_var_record(
			"playlist_forceEvent",
			"Force a particular playlist event to be active"
		),
		create_var_record(
			"playlist_autoEvent",
			"Playlist event currently active as set by the autoevent schedule"
		),
		create_var_record(
			"playlist_autoEventsEnabled",
			"Allow autoevent scheduler to run playlist events"
		),
		create_var_record(
			"com_profileLoadingForceSend",
			"Force Blackbox send after loading done"
		),
		create_var_record(
			"com_smoothFrames",
			"Enable frame times smoothing."
		),
		create_var_record(
			"com_maxclients",
			"Maximum amount of clients on the server"
		),
		create_var_record(
			"sv_maxclients",
			"The maximum number of clients that can connect to a server"
		),
		create_var_record(
			"ui_level_sv",
			"Load the UI level and start a listen server"
		),
		create_var_record(
			"ui_execdemo",
			"Show or hide certain options in the UI for demo purposes."
		),
		create_var_record(
			"ui_execdemo_cp",
			"Show or hide certain options in the UI for demo purposes, for campaign demos only."
		),
		create_var_record(
			"ui_execdemo_beta",
			"Show or hide certain options in the UI for demo purposes, for public beta only."
		),
		create_var_record(
			"ui_execdemo_gamescom",
			"Show or hide certain options in the UI for demo purposes, for gamescom only."
		),
		create_var_record(
			"ui_execdemo_gamescom_host",
			"Show or hide certain options in the UI for demo purposes, for gamescom host machine only."
		),
		create_var_record(
			"ui_disablePSControlWidgets",
			"Disable all the control widgets in PS. This dvar is added for recording video for DPS Demo."
		),
		create_var_record(
			"com_maxfps",
			"Cap frames per second"
		),
		create_var_record(
			"ui_error_callstack_ship",
			"Enable LUI error callstack reporting to show in the UI"
		),
		create_var_record(
			"ui_timescale",
			"Scale time of each frame of LUI animation"
		),
		create_var_record(
			"ui_disable_blur",
			"Disable LUI blur"
		),
		create_var_record(
			"zombie_devgui",
			""
		),
		create_var_record(
			"zm_private_rankedmatch",
			"Zombies private online match will be ranked and no join in progress is allowed"
		),
		create_var_record(
			"com_fixedtime_float",
			"Use a fixed time rate for each frame"
		),
		create_var_record(
			"long_blocking_call",
			"Enable SCR_DrawPleaseWait dialog"
		),
		create_var_record(
			"sv_paused",
			"Pause the server"
		),
		create_var_record(
			"cl_paused",
			"Pause the client"
		),
		create_var_record(
			"cl_gamepadCheatsEnabled",
			"Enable God and Demigod controller cheats"
		),
		create_var_record(
			"com_sv_running",
			"Server is running"
		),
		create_var_record(
			"com_forceSVLockStep",
			"Only allow the server frame to run when it gets told to by the client thread"
		),
		create_var_record(
			"com_wideScreen",
			"True if the game video is running in 16x9 aspect, false if 4x3."
		),
		create_var_record(
			"log_filename",
			"Open log file with a given filename"
		),
		create_var_record(
			"showMapErrors",
			"Show pointless error message after quitting level"
		),
		create_var_record(
			"ui_mapname",
			"Current map name"
		),
		create_var_record(
			"ui_enableAllHeroes",
			"Enables all Heroes."
		),
		create_var_record(
			"ui_tu11_splitscreen_scoreboard_snap_check",
			"Enables a force update of the scoreboard for splitscreen clients on the first snapshot."
		),
		create_var_record(
			"tu11_maxQuadCacheAlloc",
			"Splits up large quadCache allocations into smaller ones, to fix a UI out of memory error with long strings."
		),
		create_var_record(
			"ui_errorUndefLocals",
			"Turns on whether LUI will error or warning for undefined local variables."
		),
		create_var_record(
			"ui_deadquote",
			""
		),
		create_var_record(
			"ui_errorMessage",
			"Most recent error message"
		),
		create_var_record(
			"ui_autoContinue",
			"Automatically 'click to continue' after loading a level"
		),
		create_var_record(
			"ui_playLogoMovie",
			"Play the initial loading movie with the company logos"
		),
		create_var_record(
			"ui_allowDisplayContinue",
			"if true, allow the skip button to be displayed during loading cinematic"
		),
		create_var_record(
			"scr_hostmigrationtest",
			""
		),
		create_var_record(
			"debug_show_viewpos",
			""
		),
		create_var_record(
			"scr_killcam",
			""
		),
		create_var_record(
			"nextarena",
			""
		),
		create_var_record(
			"ClickToContinue",
			""
		),
		create_var_record(
			"saved_gameskill",
			""
		),
		create_var_record(
			"upload_bps_dedicated",
			"dedicated server bandwidth req'd"
		),
		create_var_record(
			"testMissingContentPacks",
			"When the DLC flag changes for a player, sent the result to Demonware."
		),
		create_var_record(
			"addMapPackFlagsUserInfo",
			"Send availableMappackFlags out with the User info."
		),
		create_var_record(
			"missingCommandWillError",
			"When true a missing command or config file will cause an ERR_DROP."
		),
		create_var_record(
			"doAutoExecDevConfig",
			"Exec autoexec developer config files."
		),
		create_var_record(
			"doAutoExecUserConfig",
			"Exec autoexec user config files."
		),
		create_var_record(
			"tu6_doublePistolSpreadAmmo",
			"Show double ammo in the HUD for pistol spread weapons"
		),
		create_var_record(
			"tu9_skipPerlocalClientAllocation",
			""
		),
		create_var_record(
			"tu10_ClearWeapNextHeldInputOnSwitchWeaponCmd",
			"clear cl->weapNextHeld in the SVSCMD_SWITCH_WEAPON response. We should not allow controller input to weapon cycling on the same frame that the server tells us to switch weapons."
		),
		create_var_record(
			"scr_xpscaleCP",
			"Controls the xp scaling for CP"
		),
		create_var_record(
			"scr_xpscaleMP",
			"Controls the xp scaling for MP"
		),
		create_var_record(
			"scr_xpscaleZM",
			"Controls the xp scaling for ZM"
		),
		create_var_record(
			"scr_xpscaleZMPromo",
			"Controls the xp scaling for the ZM DoubleXP Promo"
		),
		create_var_record(
			"scr_vialsAwardedScale",
			"Controls how many vials are awarded to the player"
		),
		create_var_record(
			"scr_gunxpscaleCP",
			"Controls the gun xp scaling for CP"
		),
		create_var_record(
			"scr_gunxpscaleMP",
			"Controls the gun xp scaling for MP"
		),
		create_var_record(
			"scr_gunxpscaleZM",
			"Controls the gun xp scaling for ZM"
		),
		create_var_record(
			"scr_scorestreaks",
			""
		),
		create_var_record(
			"scr_scorestreaks_maxstacking",
			""
		),
		create_var_record(
			"scr_rampagebonusscale",
			""
		),
		create_var_record(
			"scr_disableChallenges",
			""
		),
		create_var_record(
			"scr_disableStatTracking",
			""
		),
		create_var_record(
			"scr_disableSetDStat",
			""
		),
		create_var_record(
			"com_logfile",
			"Write to log file - 0 = disabled, 1 = async file write, 2 = Sync every write"
		),
		create_var_record(
			"tu9_highestAvailableDLC",
			"Highest DLC to show purchase prompts for.  Should be ovewritten in playlists.info when DLC is released"
		),
		create_var_record(
			"tu10_enableHighestAvailableDLCEntitlementCheck",
			"Enable/Disable the highest available DLC entitlement check."
		),
		create_var_record(
			"net_broadcast_ip",
			"IP address to use for broadcast packets"
		),
		create_var_record(
			"netstats_dummy_tracker_data",
			"Turn on dummy random data generation for the netstats tracker"
		),
		create_var_record(
			"netchan_debugSpew",
			"Print very verbose debug info about netchan traffic"
		),
		create_var_record(
			"netchan_debugSpewChan",
			"If turned on, debug spew for this channel will be displayed"
		),
		create_var_record(
			"netchan_fakeLoad",
			"Additional ms lard to add to netchan frame to simulate heavy load / context switch"
		),
		create_var_record(
			"net_emu_latency",
			"Emulated network latency in ms"
		),
		create_var_record(
			"net_emu_jitter",
			"Emulated network latency jitter in ms"
		),
		create_var_record(
			"net_emu_packet_loss",
			"Emulated network %% packet loss"
		),
		create_var_record(
			"net_emu_server",
			"Server network emulation info string"
		),
		create_var_record(
			"net_emu_client",
			"Client network emulation info string"
		),
		create_var_record(
			"streamMBPerSecond",
			"Throttle the streamer to the specified megabytes per second"
		),
		create_var_record(
			"sv_connectionLogProbability",
			"Probability of uploading connection log data."
		),
		create_var_record(
			"sv_connectionLogSamplesPerSecond",
			"Number of samples per second per client."
		),
		create_var_record(
			"sv_mapname",
			"Current map name"
		),
		create_var_record(
			"sv_mapcrc",
			"crc of the map file"
		),
		create_var_record(
			"cp_queued_level",
			"Queued level"
		),
		create_var_record(
			"sv_skipto",
			"Current skipto"
		),
		create_var_record(
			"sv_saveGameSkipto",
			"Skipto that gets saved to the save game"
		),
		create_var_record(
			"sv_privateClients",
			"Maximum number of private clients allowed on the server"
		),
		create_var_record(
			"sv_privateClientsForClients",
			"The # of private clients (we send this to clients)"
		),
		create_var_record(
			"sv_hostname",
			"Host name of the server"
		),
		create_var_record(
			"sv_noname",
			"Player name assigned to players that fail name validation"
		),
		create_var_record(
			"sv_penetrationCount",
			"Maximum number of private clients allowed on the server"
		),
		create_var_record(
			"sv_connectTimeout",
			"seconds without any message when a client is loading"
		),
		create_var_record(
			"sv_writeConfigStrings",
			"Write out the config string file"
		),
		create_var_record(
			"scr_writeConfigStrings",
			"Special script mode for writing config string files"
		),
		create_var_record(
			"sv_generateConstBaselines",
			"Whether or not to generate the const baseline files."
		),
		create_var_record(
			"sv_cheats",
			"Enable cheats on the server"
		),
		create_var_record(
			"sv_sendCmdAngles",
			"Send cmdAngles to clients for increased viewangles fidelity during killcam"
		),
		create_var_record(
			"sv_mapRotation",
			"List of maps for the server to play"
		),
		create_var_record(
			"sv_mapRotationCurrent",
			"Current map in the map rotation"
		),
		create_var_record(
			"sv_rateBoosting",
			"Determines whether we should allow rate boosting: 0 - disabled, 1 - enabled based on huristic, 2 - force enabled"
		),
		create_var_record(
			"sv_maxRate",
			"Maximum server->client rate"
		),
		create_var_record(
			"sv_zlib_threshold",
			"Message size threshold which triggers more aggressive compression"
		),
		create_var_record(
			"sv_timeout",
			"seconds without any message"
		),
		create_var_record(
			"sv_mapSwitch",
			"in-game map switching: 0,disabled|1,enabled|(default)2,enabled but devmap will turn it off"
		),
		create_var_record(
			"sv_potm_debug",
			"Debug info for the Play of the Match system"
		),
		create_var_record(
			"migration_blocksperframe",
			"Number of blocks to send out each server frame.  A value of 0 means base it off our tested bandwidth."
		),
		create_var_record(
			"migration_msgtimeout",
			"timeout time in ms of migration messages"
		),
		create_var_record(
			"migration_timeBetween",
			"milliseconds between migration_limit dvar's value of migrations."
		),
		create_var_record(
			"migration_forceHost",
			"force a particular client index to be host."
		),
		create_var_record(
			"migration_soak",
			"Soak test the migration. Migrate every migrationTimeBetween milliseconds"
		),
		create_var_record(
			"storage_offline",
			"Live features are stored locally instead of on Demownare."
		),
		create_var_record(
			"storage_maxfailures",
			"Number of failures before maxing out backoff increases."
		),
		create_var_record(
			"storage_backoff",
			"Base for backing off exponentially"
		),
		create_var_record(
			"loc_availableLanguages",
			""
		),
		create_var_record(
			"loc_language",
			"Language"
		),
		create_var_record(
			"loc_systemLanguage",
			"System language"
		),
		create_var_record(
			"loc_forceEnglish",
			"Force english localized strings"
		),
		create_var_record(
			"loc_warnings",
			"Enable localization warnings"
		),
		create_var_record(
			"loc_warningsAsErrors",
			"Throw an error for any unlocalized string"
		),
		create_var_record(
			"loc_languageSaved",
			"Has a language value been chosen and saved"
		),
		create_var_record(
			"st_LODDistScale",
			"Distance scale for terrain LOD - when this increases, detail at a given distance is reduced"
		),
		create_var_record(
			"ui_emblemDisableGroups",
			"Disable the emblem group feature."
		),
		create_var_record(
			"ui_creditSkipTo",
			"Line number to skip to in the credits?"
		),
		create_var_record(
			"ui_smallFont",
			"Small font scale"
		),
		create_var_record(
			"ui_bigFont",
			"Big font scale"
		),
		create_var_record(
			"ui_demoname",
			"Current selected demo name"
		),
		create_var_record(
			"ui_liveEventViewerOpen",
			"If the liveEventViewer menu is currently open"
		),
		create_var_record(
			"ui_keyboardtitle",
			""
		),
		create_var_record(
			"ui_keyboard_dvar_edit",
			""
		),
		create_var_record(
			"scr_hardcore",
			""
		),
		create_var_record(
			"ui_hud_showobjicons",
			""
		),
		create_var_record(
			"ui_hud_obituaries",
			""
		),
		create_var_record(
			"ui_currMenuNameList",
			"Current menu name list set in lua, for debug display."
		),
		create_var_record(
			"ui_currFocusList",
			"Current focus stack in lua, for debug display and ship crash dumps."
		),
		create_var_record(
			"ui_currFocusStates",
			"Current focus stack, with states, set in lua, for debug display."
		),
		create_var_record(
			"ui_usingFreeCursor",
			"Turns the free cursor on/off in console builds."
		),
		create_var_record(
			"ui_scrollSpeed",
			""
		),
		create_var_record(
			"ui_hud_hardcore",
			"Whether the HUD should be suppressed for hardcore mode"
		),
		create_var_record(
			"ui_allow_classchange",
			"Whether the UI should allow changing class"
		),
		create_var_record(
			"ui_hud_visible",
			"Whether the 2D HUD should be displayed or not."
		),
		create_var_record(
			"ls_gametype",
			""
		),
		create_var_record(
			"ls_mapname",
			""
		),
		create_var_record(
			"ui_busyBlockIngameMenu",
			"1 if ingame menu should be blocked for pending action"
		),
		create_var_record(
			"ui_heatMapColor",
			"Heat Map Blob Color"
		),
		create_var_record(
			"ui_heatMapColorForPlayer",
			"Heat Map Blob Color for player deaths only"
		),
		create_var_record(
			"ui_useloadingmovie",
			"Play and render a loading movie instead of static screen (mapname_load)"
		),
		create_var_record(
			"ui_badWordSeverity",
			"1 to block partial and full profanity matches, 2 to only block full matches"
		),
		create_var_record(
			"ui_badWordMaxTaskWaits",
			"The number of cycles *10 ms for a remoteTask before we decide to cancel the isBadWord task"
		),
		create_var_record(
			"ui_friendsListOpen",
			"True when viewing Friends List."
		),
		create_var_record(
			"mp_prototype",
			"Enabled T8 prototype features (int because this was used as a command line int-only property)"
		),
		create_var_record(
			"ui_autoControlledPlayer",
			"Enables the player to select a ui option to transfer the player's control to Bot"
		),
		create_var_record(
			"blackboxMode",
			""
		),
		create_var_record(
			"cg_drawFramePerformanceWarnings",
			"Draw onscreen timers for client, server, and gps if they exceed their frame budgets."
		),
		create_var_record(
			"devmaponline",
			"devmap into online mode"
		),
		create_var_record(
			"phys_player_step_on_actors",
			"phys_player_step_on_actors"
		),
		create_var_record(
			"snd_losOcclusion",
			"occlusion based off of LOS"
		),
		create_var_record(
			"ui_drawCrosshair",
			"Whether to draw crosshairs."
		),
		create_var_record(
			"voice_enabled",
			"Voice communication enabled"
		),
		create_var_record(
			"cg_fov_default",
			"User default field of view angle in degrees"
		),
		create_var_record(
			"compassFriendlyHeight",
			"The size of the friendly icon on the compass"
		),
		create_var_record(
			"compassFriendlyWidth",
			"The size of the friendly icon on the compass"
		),
		create_var_record(
			"cg_weaponCycleDelay",
			"The delay after cycling to a new weapon to prevent holding down the cycle weapon button from cycling too fast"
		),
		create_var_record(
			"cl_packetdup",
			"Enable packet duplication"
		),
		create_var_record(
			"debug_audio",
			"Enables debug rendering for audio."
		),
		create_var_record(
			"debug_mover",
			"Turns on mover debug mode"
		),
		create_var_record(
			"demo_drawdebuginformation",
			"Used to draw debug information."
		),
		create_var_record(
			"g_reviveTime",
			"The time it takes to revive a player in last stand (do not change as it is tied to the third person animations)"
		),
		create_var_record(
			"slide_subsequentSlideScale",
			"Percent the slide speed should be scaled down by per subsequent slide"
		),
		create_var_record(
			"player_swimSprintSpeedScale",
			"Scale the swim speed by this amount when sprinting"
		),
		create_var_record(
			"melee_attachmentMultiplier",
			"If the melee attachment is equipped, the melee base damage is multiplied by this amount"
		),
		create_var_record(
			"tm_taskTrackerOn",
			"If true, tasks will be tracked by the tracking system"
		),
		create_var_record(
			"tm_taskManagerDebug",
			"Shows more information as the number is increased"
		),
		create_var_record(
			"tm_startedThreshold",
			"If this many tasks of a single definition are started, a warning message will pop-up."
		),
		create_var_record(
			"tm_failsThreshold",
			"If this many tasks of a single definition fail, a warning message will pop-up."
		),
		create_var_record(
			"tm_succeedsThreshold",
			"If this many tasks of a single definition succeed, a warning message will pop-up."
		),
		create_var_record(
			"tm_pendingeThreshold",
			"If this many tasks of a single definition are concurrently pending, a warning message will pop-up."
		),
		create_var_record(
			"tm_typeToDisplay",
			"Chooses which type of task to display in the log"
		),
		create_var_record(
			"tm_logTimeLimit",
			"In seconds, how long should pass before an event is shown in the log"
		),
		create_var_record(
			"tm_currentErrorPage",
			"In seconds, how long should pass before an event is shown in the log"
		),
		create_var_record(
			"sv_cheats",
			"External Dvar"
		),
		create_var_record(
			"allClientDvarsEnabled",
			"Allow host to set all client dvars"
		),
		create_var_record(
			"profileDwUpdateInterval",
			"Interval (in seconds) between updating recorded performance information in DW (negative value disable recording)"
		),
		create_var_record(
			"reliableResendTime",
			"Amount of time (in ms) to wait before resending an unack'd reliable message"
		),
		create_var_record(
			"reliableTimeoutTime",
			"Amount of time (in ms) to wait before timing out a client for not ack'ing a message"
		),
		create_var_record(
			"phys_gravity",
			"Physics gravity in units/sec^2."
		),
		create_var_record(
			"phys_gravity_dir",
			"Sets the gravity direction."
		),
		create_var_record(
			"phys_buoyancy",
			"Used to turn on/off buoyancy"
		),
		create_var_record(
			"phys_ragdoll_buoyancy",
			"Used to turn on/off the buoyancy for he ragdolls"
		),
		create_var_record(
			"phys_disableEntsAndDynEntsCollision",
			"Disable rigid body collision with dyn ents and regular entities as an optimization.  Vehicles do not apply."
		),
		create_var_record(
			"enable_moving_paths",
			"enable moving paths"
		),
		create_var_record(
			"enable_new_prone_check",
			"enable new prone check"
		),
		create_var_record(
			"phys_wind_enabled",
			"Used to turn off/on physics wind sim"
		),
		create_var_record(
			"phys_wind_vehicle_scale",
			"Scale the vehicle wind reactions by this value"
		),
		create_var_record(
			"snd_speakerConfiguration",
			"Speaker configuration"
		),
		create_var_record(
			"glassDamageMultiplier",
			"Multiplier the damage applied to the glass (server)."
		),
		create_var_record(
			"maxAIGlassHits",
			"Maximum number of hits it takes an AI to break the glass."
		),
		create_var_record(
			"demo_enabled",
			"Used to turn the system on/off."
		),
		create_var_record(
			"demo_recordPrivateMatch",
			"Used to turn the private match recording on/off."
		),
		create_var_record(
			"demo_recordSystemlinkMatch",
			"Used to turn the systemlink match recording on/off."
		),
		create_var_record(
			"demo_recordOfflineMatch",
			"Used to turn the offline match recording on/off."
		),
		create_var_record(
			"demo_debug",
			"Debug info for the Server Snapshot Demo system"
		),
		create_var_record(
			"demo_client",
			"Current viewing player"
		),
		create_var_record(
			"demo_recordingrate",
			"Used to tweak the rate(in msec) at which we write a super snapshot"
		),
		create_var_record(
			"demo_fileblockWriteRate",
			"Used to tweak the rate(in sec) at which we write the file blocks during demo recording"
		),
		create_var_record(
			"demo_keyframerate",
			"Used to specify the rate(in sec) at which we generate a keyframe during playback."
		),
		create_var_record(
			"demo_pause",
			"Used to pause a demo playback."
		),
		create_var_record(
			"demo_pauseOnNextSnapshot",
			"Used to jump to the next snapshot during network analysis."
		),
		create_var_record(
			"demo_maxTimeScale",
			"The maximum timescale rate we will support."
		),
		create_var_record(
			"demo_downloadEntireFile",
			"Used toggle the ability to download the entire file instead of parts."
		),
		create_var_record(
			"demo_livestreaming",
			"Used to turn the live streaming system on/off"
		),
		create_var_record(
			"demo_downloadLiveStreamThrottleTime",
			"The maximum amount of time (in msec) we can starve the download of live stream callback."
		),
		create_var_record(
			"demo_summaryReadEnabled",
			"Used to turn meta summary read on/off"
		),
		create_var_record(
			"demo_writePaintShopInformation",
			"Used to turn on/off the paintshop data write into the demo file"
		),
		create_var_record(
			"demo_bookmarkEventThresholdTime",
			"The time duration for which we want to show the bookmark event image in the demo timeline."
		),
		create_var_record(
			"demo_freeCameraLockOnOrbitRadius",
			"Radius of the lockon orbit."
		),
		create_var_record(
			"demo_highlightReelPlayerFilter",
			"Demo Highlight Reel player filter control"
		),
		create_var_record(
			"demo_highlightReelTransitionFilter",
			"Demo Highlight Reel transition filter control"
		),
		create_var_record(
			"demo_highlightReelStylesFilter",
			"Demo Highlight Reel styles filter control"
		),
		create_var_record(
			"demo_highlightReelNumberOfSegments",
			"Demo Highlight Reel number of segments"
		),
		create_var_record(
			"demo_highlightReelMinimumStarsFilter",
			"Minimum stars used for the filtering of events in the highlight."
		),
		create_var_record(
			"demo_highlightReelGameResultFilter",
			"Demo highlight reel round outcome filter."
		),
		create_var_record(
			"demo_packetsPerSecondMin",
			"Min amount of packets to send per second before throttling."
		),
		create_var_record(
			"demo_bytesPerSecondMin",
			"Min amount of bytes to send per second before throttling."
		),
		create_var_record(
			"demo_packetsPerSecondMax",
			"Max amount of packets to send per second before throttling."
		),
		create_var_record(
			"demo_bytesPerSecondMax",
			"Max amount of bytes to send per second before throttling."
		),
		create_var_record(
			"demo_inLobbyThrottleBandwidthPercent",
			"Bandwidth percent to be released when we are not going to throttle in the lobby."
		),
		create_var_record(
			"demo_inGameThrottleBandwidthPercent",
			"Bandwidth percent to be released when we are not going to throttle inside the game."
		),
		create_var_record(
			"demo_streamingAdjustmentFactor",
			"The adjustment factor for howScrewedAreWe in the streaming."
		),
		create_var_record(
			"demo_enableSvBandwidthLimitThrottle",
			"Used to enable the server bandwidth based throttling system."
		),
		create_var_record(
			"demo_svBandwidthLimitThrottleMaxBytesPercent",
			"The percentage of the max. bandwidth per frame within which we want to apply the throttle."
		),
		create_var_record(
			"demo_svBandwidthLimitThrottleTimeout",
			"The maximum amount of time (in msec) we can starve the write to stream callback, due to the server hitting the maximum bandwidth limit per frame."
		),
		create_var_record(
			"demo_streamingSendSocketBuffSizeForRecording",
			"The send socket buffer size which will be used for recording"
		),
		create_var_record(
			"demo_streamingSendSocketBuffSizeForOtherUploads",
			"The send socket buffer size which will be used for other uploads"
		),
		create_var_record(
			"demo_downloadStreamThrottleTime",
			"The maximum amount of time (in msec) we can starve the download stream callback."
		),
		create_var_record(
			"demo_downloadStreamRetryWaitTime",
			"The maximum amount of time (in msec) we will have to wait before we retry the download."
		),
		create_var_record(
			"demo_downloadStreamMaxRetryAttemps",
			"The maximum number of retry attempts before we mark the downloadstream as failed.."
		),
		create_var_record(
			"perk_weapReloadMultiplier",
			"Percentage of weapon reload time to use"
		),
		create_var_record(
			"perk_killstreakReduction",
			"Reduced number of kills required to obtain a killstreak."
		),
		create_var_record(
			"perk_killstreakMomentumMultiplier",
			"Multiplies the amount of momentum received by the player"
		),
		create_var_record(
			"perk_killstreakAnteUpResetValue",
			"Scorestreak reset value when the ante up perk is in use."
		),
		create_var_record(
			"perk_delayExplosiveTime",
			"Time in seconds to delay a proximity explosive"
		),
		create_var_record(
			"perk_disarmExplosiveTime",
			"Time in seconds to disarm a proximity explosive"
		),
		create_var_record(
			"perk_deathStreakCountRequired",
			"Number of deaths required to obtain a deathstreak."
		),
		create_var_record(
			"perk_healthRegenMultiplier",
			"Multiplier to the time which the player starts to regenerate health after damage"
		),
		create_var_record(
			"perk_flakJacket",
			"Percent of explosive damage done that will apply to player."
		),
		create_var_record(
			"perk_flakJacket_hardcore",
			"Percent of explosive damage done that will apply to player in hardcore."
		),
		create_var_record(
			"perk_fireproof",
			"Percent to reduce flame damage done to the player."
		),
		create_var_record(
			"perk_gpsjammer_min_speed",
			"Minimum speed a player must be moving for gpsjammer to protect them from UAV sweeps.  Based on player speed used in g_speed (I believe this is units/sec).  If 0, player will always be protected from UAV sweeps with this specialty."
		),
		create_var_record(
			"perk_gpsjammer_min_distance",
			"Minimum distance a player must have moved in a time period for gpsjammer to protect them from UAV sweeps.  If 0, player will always be protected from UAV sweeps with this specialty."
		),
		create_var_record(
			"perk_gpsjammer_time_period",
			"Time period inbetween checks, in MSec. Please use multiples of 50 msec as that is the period of server script "
		),
		create_var_record(
			"perk_gpsjammer_graceperiods",
			"Number of time period a player can be below the required velocity before removing protection."
		),
		create_var_record(
			"perk_sgjammer_min_speed",
			"Minimum speed a player must be moving for gpsjammer to protect them from Sensor Grenade sweeps.  Based on player speed used in g_speed (I believe this is units/sec).  If 0, player will always be protected from Sensor Grenade sweeps with this specialty."
		),
		create_var_record(
			"perk_sgjammer_min_distance",
			"Minimum distance a player must have moved in a time period for gpsjammer to protect them from Sensor Grenade sweeps.  If 0, player will always be protected from UAV sweeps with this specialty."
		),
		create_var_record(
			"perk_sgjammer_time_period",
			"Time period inbetween checks, in MSec. Please use multiples of 50 msec as that is the period of server script "
		),
		create_var_record(
			"perk_sgjammer_graceperiods",
			"Number of time period a player can be below the required velocity before removing protection."
		),
		create_var_record(
			"perk_nottargetedbyai_min_speed",
			"Minimum speed a player must be moving for nottargetedbyai to protect them from AI controlled killstreaks. If 0, player will always be protected from AI controlled killstreaks with this specialty."
		),
		create_var_record(
			"perk_nottargetedbyai_graceperiod",
			"Time after which the player drops below min_gpsjammer_speed that they are still protected from ai support targetting.  0-10000 int (in milliseconds), 0 default."
		),
		create_var_record(
			"perk_tracker_fx_foot_height",
			"Height of footsteps for tracker perk."
		),
		create_var_record(
			"perk_tracker_fx_fly_height",
			"Height of flying markers for tracker perk."
		),
		create_var_record(
			"enable_global_wind",
			"enable wind effect"
		),
		create_var_record(
			"enable_grass_wind",
			"enable grass wind effect"
		),
		create_var_record(
			"wind_global_vector",
			"global wind force in units/sec. This is also the hi-level wind"
		),
		create_var_record(
			"wind_global_low_altitude",
			"altitude of global low-level wind"
		),
		create_var_record(
			"wind_global_hi_altitude",
			"altitude of global hi-level wind"
		),
		create_var_record(
			"wind_global_low_strength_percent",
			"strength of global low-level wind, expressed as a percentage of wind_global_vector"
		),
		create_var_record(
			"wind_leaf_scale",
			"global wind grass tension (stiffness)"
		),
		create_var_record(
			"wind_grass_gustinterval",
			"global wind force gust interval"
		),
		create_var_record(
			"wind_grass_scale",
			"global wind force scaler"
		),
		create_var_record(
			"wind_grass_tension",
			"global wind grass tension (stiffness)"
		),
		create_var_record(
			"wind_grass_gust_speed",
			"speed multiplier for gusts"
		),
		create_var_record(
			"wind_grass_gust_radius",
			"size of global wind gust effector"
		),
		create_var_record(
			"wind_grass_gust_distance",
			"max distance at which to place gusts"
		),
		create_var_record(
			"aim_target_closest_first",
			"Target the closest AI based on 3d distance"
		),
		create_var_record(
			"aim_autoaim_enabled",
			"Turn on auto aim"
		),
		create_var_record(
			"bg_viewKickScale",
			"The scale to apply to the damage done to caluclate damage view kick"
		),
		create_var_record(
			"bg_viewKickMax",
			"The maximum view kick"
		),
		create_var_record(
			"bg_viewKickMin",
			"The minimum view kick"
		),
		create_var_record(
			"bg_viewKickDeflectTime",
			"The random direction scale view kick"
		),
		create_var_record(
			"bg_viewKickRecoverTime",
			"The random direction scale view kick"
		),
		create_var_record(
			"bg_viewKickScaleAds",
			"The scale to apply to the damage done to caluclate damage view kick"
		),
		create_var_record(
			"bg_viewKickMaxAds",
			"The maximum view kick"
		),
		create_var_record(
			"bg_viewKickMinAds",
			"The minimum view kick"
		),
		create_var_record(
			"bg_viewKickDeflectTimeAds",
			"The random direction scale view kick"
		),
		create_var_record(
			"bg_viewKickRecoverTimeAds",
			"The random direction scale view kick"
		),
		create_var_record(
			"bg_scriptCanDisableStances",
			"Whether script is allowed to disable stances"
		),
		create_var_record(
			"bg_useThrowButtonForDetonatorGrenades",
			"Whether the throw button should be returned as the fire button for grenades and mines that have a detonator"
		),
		create_var_record(
			"bg_heavyWeaponsAlwaysPlayFirstRaise",
			"Always play the first raise animation when switching to heavy weapons"
		),
		create_var_record(
			"player_useWaterFriction",
			"Change the players walk friction when in water, using water friction scales"
		),
		create_var_record(
			"player_useWaterWadeScale",
			"Scale the players command based on their depth in the water"
		),
		create_var_record(
			"bg_friendlyFire",
			"Friendly fire status"
		),
		create_var_record(
			"bg_friendlyFireMode",
			"Enables friendly fire mode, cannot shoot down friendlies at close range"
		),
		create_var_record(
			"bg_grenadeMinimumThrowbackTime",
			"Time added to grenade countdown to stop it from blowing up during throwback."
		),
		create_var_record(
			"bg_weaponCompatibleIfSharedAmmo",
			"Allow leeching ammo from weapons touched on the ground if the player has a weapon with matching shared ammo"
		),
		create_var_record(
			"disableLookAtEntityLogic",
			"Disables the Player_UpdateLookAtEntity logic (expensive)"
		),
		create_var_record(
			"player_lastStandBleedoutTime",
			"The time taken by the player to completely bleedout when in last stand (pro version)."
		),
		create_var_record(
			"player_lastStandDistScale",
			"Tweaks some of the movement in last stand."
		),
		create_var_record(
			"player_ProneMinPitchDiff",
			"Minimum pitch difference allowed to move in prone."
		),
		create_var_record(
			"player_ProneLaststandFreeMove",
			"Allow freer movement in last stand than normal prone movement"
		),
		create_var_record(
			"player_lastStandCrawlTransition",
			"Transition to the last stand crawl set, and not regular last stand animation"
		),
		create_var_record(
			"bg_useT7Melee",
			"use the T7 melee system."
		),
		create_var_record(
			"player_meleeChargeMaxSpeed",
			"Max speed to clamp when doing a charge melee"
		),
		create_var_record(
			"player_meleeChargeMinSpeed",
			"Min speed to clamp when doing a charge melee"
		),
		create_var_record(
			"player_meleeLungeAcceleration",
			"Dvar to control the acceleration for the lunge attack"
		),
		create_var_record(
			"player_meleeLungeUnderwaterSpeedScale",
			"Scaler for the underwater lunge speed"
		),
		create_var_record(
			"player_meleeLungeSpeedCutoff",
			"Cutoff the lunge attacks if the velocity falls bellow this value"
		),
		create_var_record(
			"player_meleeLungeTimeCutoff",
			"Max duration of the lunge attack"
		),
		create_var_record(
			"vehicle_riding",
			"Turning this on enables players to walk around on top of vehicles"
		),
		create_var_record(
			"vehicle_selfCollision",
			"Enabled the vehicle to shoot itself with it's own turret."
		),
		create_var_record(
			"tu11_IgnoreItemsWithPLevel100InIsItemLockedForChallenge",
			"Treats items with PLevel 100 or more as never unlocked in IsItemLockedForChallenge()."
		),
		create_var_record(
			"bg_disableWeaponPlantingGroundTrace",
			"Disables the ground trace check for weapon planting."
		),
		create_var_record(
			"slide_forceBaseSlide",
			"Force the player to use the base slide, do not allow boost reduced or boost"
		),
		create_var_record(
			"actors_walkable",
			"Players can walk on AI without sliding off"
		),
		create_var_record(
			"trm_enabled",
			"Enable or disable the player's mantle"
		),
		create_var_record(
			"juke_enabled",
			"Enable player evasive move"
		),
		create_var_record(
			"sprint_capSpeedEnabled",
			"Enable physics cap on speed during sprinting"
		),
		create_var_record(
			"weaponrest_enabled",
			"Enable weapon rest, player turns gun so that it does not appear to intersect geo"
		),
		create_var_record(
			"traverse_mode",
			"1 = new traversal 2 = pre-BO3 mantle system"
		),
		create_var_record(
			"sprint_allowRestore",
			"Restore the players sprint state after certain actions, wallrun, traverse etc.  "
		),
		create_var_record(
			"bg_weaponSwitchHero",
			"The HeroWeaponType to switch to on a press of button WEAPON_SWITCH_HERO."
		),
		create_var_record(
			"bg_specialOffhandInventorySwitch",
			"The HeroWeaponType to switch to on a press of button OFFHAND_SPECIAL."
		),
		create_var_record(
			"bg_specialOffhandInventoryAllowBallSwitch",
			"When true the special offhand input switch to inventory weapon allows ball switch."
		),
		create_var_record(
			"friendlyContentOutlines",
			"Use this to enable the outlines on friendlies and disable the icons"
		),
		create_var_record(
			"player_dpad_gadget_scheme",
			"Dpad gadget control scheme,  0 - mp system, 1 - career mode (dapd controls 3 gadgets)"
		),
		create_var_record(
			"compassObjectiveIconWidth",
			"The size of the objective on the full map"
		),
		create_var_record(
			"compassObjectiveIconHeight",
			"The size of the objective on the full map"
		),
		create_var_record(
			"compassDrawLastStandIcon",
			"Draw the last stand icon on the minimap"
		),
		create_var_record(
			"waypointOffscreenPadLeft",
			"Offset from the edge."
		),
		create_var_record(
			"waypointOffscreenPadRight",
			"Offset from the edge."
		),
		create_var_record(
			"waypointOffscreenPadTop",
			"Offset from the edge."
		),
		create_var_record(
			"waypointOffscreenPadBottom",
			"Offset from the edge."
		),
		create_var_record(
			"cg_aggressiveCullRadius",
			"The radius to use for entity CG_ProcessEntity frustrus culling. 0 means disabled."
		),
		create_var_record(
			"slam_blur_enabled",
			"Is screen blur enabled for slam"
		),
		create_var_record(
			"cg_drawBreathHint",
			"Draw a 'hold breath to steady' hint"
		),
		create_var_record(
			"cg_hudDamageIconTime",
			"The amount of time for the damage icon to stay on screen after damage is taken"
		),
		create_var_record(
			"cg_crosshairVehicleSentientCheck",
			"Whether to show colored crosshairs when targetting vehicles iff sentient exists"
		),
		create_var_record(
			"cg_updateScoreboardAfterGameEnded",
			"Controls whether to update or freeze the scoreboard when the game ends."
		),
		create_var_record(
			"cg_DrawRemoteVehiclePlayerNames",
			"Player's gamertag displayed above remote controlled vehicles"
		),
		create_var_record(
			"cg_DrawPlayerNamesWhileInVehicle",
			"Player's gamertags are displayed to player who is in vehicle"
		),
		create_var_record(
			"cg_DrawOnlyFriendlyPlayerNamesWhileInVehicle",
			"Only Friendly Player's gamertags are displayed to player who is in vehicle"
		),
		create_var_record(
			"cg_DrawOnScreenFriendlyAI",
			"Draws all friendly AI names who are currently on screen."
		),
		create_var_record(
			"cg_thirdPersonLastStand",
			"Uses the third person view when the player is in laststand."
		),
		create_var_record(
			"cg_WeaponButtVictimCameraShakeScale",
			"Scale of the camera Shake applied to a weapon butt Victim."
		),
		create_var_record(
			"cg_WeaponButtVictimCameraShakeDuration",
			"Duration in milliseconds of the camera Shake applied to a weapon butt Victim."
		),
		create_var_record(
			"cl_dpadLeftHeavyWeapon",
			"When true player switches to the inventory weapon when pressing dpad left."
		),
		create_var_record(
			"cl_bspmismatchFatal",
			"If true client will ERR_DROP if its BSP doesn't match the server's"
		),
		create_var_record(
			"cl_bitfieldmismatchFatal",
			"If true client will ERR_DROP if its bitfield doesn't match the server's"
		),
		create_var_record(
			"dynEnt_disable_rb_collision",
			"Disable rigid body collision for dyn ents with point or hinge constraints."
		),
		create_var_record(
			"dynEnt_small_cylinder_dimension",
			"Treats dyn ents with small cylinders less than this number in the X or Y as small for the purposes of angular velocity. Applies to dyn ents with point or hinge constraints. Set to 0 to disable."
		),
		create_var_record(
			"dynEnt_delete_expensive",
			"Delete expensive dyn ents."
		),
		create_var_record(
			"g_pickupPromptsForDroppedWeapons",
			"Whether dropped weapons allow a pickup prompt"
		),
		create_var_record(
			"ai_avoidNavBoundaries",
			"enables/disables the nearby boundary avoidance"
		),
		create_var_record(
			"ai_codeGameskill",
			"Use code based gameskill buildup accuracy instead of script's implementation."
		),
		create_var_record(
			"ai_secondaryThreats",
			"Allow secondary targets"
		),
		create_var_record(
			"player_throwbackOuterRadius",
			"The radius player is allow to throwback a grenade once the player has been in the inner radius"
		),
		create_var_record(
			"player_useRadius",
			"The radius within which a player can use things"
		),
		create_var_record(
			"player_useTouchScore",
			"The touch score for player use checks"
		),
		create_var_record(
			"g_avoidEnabled",
			"Disable the vehicle avoidance"
		),
		create_var_record(
			"g_throttleTempEnts",
			"Whether to throttle the number of temp ents created"
		),
		create_var_record(
			"g_allowLastStandForActiveClients",
			"Whether last stand is possible by default for active clients"
		),
		create_var_record(
			"dynEnt_shouldCullEntitiesForSplitscreen",
			"Cull alternating entities in splitscreen"
		),
		create_var_record(
			"spawnsystem_sight_check_sentient_ai",
			"Consider AI sentients while evaluating spawnpoint visibility"
		),
		create_var_record(
			"r_splitscreenBlurEdges",
			"Turns on blurring the side edges in splitscreen"
		),
		create_var_record(
			"r_maxSpotShadowUpdates",
			"Set the number of spot &omni shadow updates per frame (each omni light account for 6 updates)"
		),
		create_var_record(
			"ai_generateNavmeshRegions",
			"Generate navmesh regions."
		),
		create_var_record(
			"useClassSets",
			"Enable class sets"
		),
		create_var_record(
			"com_pauseSupported",
			"Whether is pause is ever supported by the game mode"
		),
		create_var_record(
			"gts_validation_enabled",
			"Enables gametype settings validation in online modes"
		),
		create_var_record(
			"path_checkDangerousNodes",
			"Whether to mark and check dangerous nodes"
		),
		create_var_record(
			"path_alwaysFindNearestNode",
			"Use to force finding some nearest node in, generally to try to ensure getting close enough to melee"
		),
		create_var_record(
			"path_boundsFudge",
			"Fudge the size of the ai's bounds when finding the nearest node"
		),
		create_var_record(
			"path_minzBias",
			"Bias to prevent missing valid cells below the origin"
		),
		create_var_record(
			"path_double_wide_checks",
			"Whether to do double wide checks when finding a start node for a pathfinding check"
		),
		create_var_record(
			"ui_keepLoadingScreenUntilAllPlayersConnected",
			"Whether to keep the loading screen up until all players have initially connected"
		),
		create_var_record(
			"upload_bps_demosystem",
			"demo system bandwidth req'd"
		),
		create_var_record(
			"upload_bps_perplayer",
			"per player bandwidth req'd"
		),
		create_var_record(
			"sv_antilagAnim",
			"Rewind animations during antilag rewind"
		),
		create_var_record(
			"g_gametype",
			"Current game type"
		),
		create_var_record(
			"migration_minclientcount",
			"Minimum real client count, below which the host will not migrate."
		),
		create_var_record(
			"migration_limit",
			"maximum number of migrations within migration_timeBetween dvar's value in milliseconds."
		),
		create_var_record(
			"phys_usePhysicsForLocalClientIfPossible",
			"Use the client physics simulation for the local vehicle if possible instead of the server snapshots"
		),
		create_var_record(
			"tu10_cleanupRagdollsOnExit",
			"Cleanup ragdolls on shutdown."
		),
		create_var_record(
			"tu11_cleanupRagdollsOnDemoJump",
			"Cleanup ragdolls on demo jump."
		),
		create_var_record(
			"demo_dollycamLeaveAtPreviousMarker",
			"Whether to place the camera at the previous dolly cam marker when leaving the mode."
		),
		create_var_record(
			"demo_freeCameraUseHeliHeightLockAsCeiling",
			"Whether to treat the heli_height_lock as a ceiling."
		),
		create_var_record(
			"demo_freeCameraLockOnMissileAllowed",
			"Whether missile entities are eligible for lockon"
		),
		create_var_record(
			"demo_streamUploadKeepAliveFrequency",
			"Specifies the frequency that we send the keep alive on the upload connection if we are otherwise starved for data to send (0 means not in use), used mainly to account for pause."
		),
		create_var_record(
			"demo_oldposInsteadOfMapCenter",
			"Use the oldpos instead of CL_GetMapCenter() when in freecam"
		),
		create_var_record(
			"demo_filmStartInformationScreenStayTime",
			"How long the film start information screen will stay on screen in ms."
		),
		create_var_record(
			"demo_viewTraceMask",
			"The contents mask to use for view traces."
		),
		create_var_record(
			"demo_useMapNameAsDefaultName",
			"Use the map name instead of the gametype as the default recording name."
		),
		create_var_record(
			"perk_requireScavengerPerk",
			"Should the player interact with the scavenger pickup without the perk"
		),
		create_var_record(
			"allCollectiblesUnlocked",
			"Allows all Collectibles to be available in the Safehouse"
		),
		create_var_record(
			"equipmentAsGadgets",
			"Enable equipment as gadgets in CAC"
		),
	};

	std::vector<varEntry> commands_table =
	{
		// game functions
		create_var_record(
			"quit",
			"Shutdown the Game [Com_Quit_f]"
		),
		create_var_record(
			"gts",
			"Set gametype setting, Usage: gts <path> [<value>]"
		),
		create_var_record(
			"gametype_setting",
			"Set gametype setting, Usage: gametype_setting <path> [<value>]"
		),
		create_var_record(
			"set",
			"Set dvar value, Usage: set <variable> <value>"
		),
		create_var_record(
			"setdvartotime",
			"Set dvar value to time, Usage: setdvartotime <variable>"
		),
		create_var_record(
			"reset",
			"Reset dvar value, Usage: reset <variable>"
		),
		create_var_record(
			"lobby_errorshutdown",
			"Shutdown lobby with error"
		),
		create_var_record(
			"lobby_reload",
			"Reload lobby"
		),
		create_var_record(
			"map",
			"Load map, Usage: map <map>"
		),
		create_var_record(
			"devmap",
			"Load map in dev mode, Usage: devmap <map>"
		),
		create_var_record(
			"gametype",
			"Set the gametype, Usage: gametype <gametype>"
		),
		create_var_record(
			"exec",
			"Exec file, Usage: exec <config>"
		),
		create_var_record(
			"disconnect",
			"Disconnect from server"
		),
		create_var_record(
			"reconnect",
			"Reconnect to localhost"
		),
		create_var_record(
			"hostmigration_start",
			"Start host migration"
		),
		create_var_record(
			"setperk",
			"Set a perk, Usage: setperk <perk>"
		),
		create_var_record(
			"killserver",
			"Shutdown the server"
		),
		create_var_record(
			"switchmaps",
			"Switch now to the map"
		),
		create_var_record(
			"msload",
			"Start loading map switch, Usage: msload <map>"
		),
		create_var_record(
			"mspreload",
			"Start loading map switch, Usage: mspreload <map>"
		),
		create_var_record(
			"fast_restart",
			"Fast restart"
		),
		create_var_record(
			"full_restart",
			"Full restart"
		),
		create_var_record(
			"clientkick_for_reason",
			"Kick client with a reason, Usage: clientkick_for_reason <client number> <reason loc string>"
		),
		create_var_record(
			"clientkick",
			"Kick client, Usage: clientkick <client number>"
		),
		create_var_record(
			"clearpsdata",
			"clearpsdata"
		),
		// shield functions
		create_var_record(
			"reload_mods",
			"Reload the shield mods"
		),
		create_var_record(
			"hash_var",
			"Hash and print a variable, Usage: hash_var <var>"
		),
	};

	std::vector<const char*> get_dvars_list()
	{
		static std::vector<const char*> dvars;
		if (!dvars.empty()) return dvars;

		for (const auto& dvar : dvars_table)
		{
			dvars.push_back(dvar.name.data());
		}

		return dvars;
	}

	std::vector<const char*> get_commands_list()
	{
		static std::vector<const char*> commands;
		if (!commands.empty()) return commands;

		for (const auto& cmd : commands_table)
		{
			commands.push_back(cmd.name.data());
		}

		return commands;
	}
}

namespace runtime_errors
{
		std::unordered_map<uint64_t, const char*> errors = 
		{
			{ 1045192683, "Scr_RandomFloatRange's second parameter must be greater than the first." },
			{ 1047729873, "exitlevel already called" },
			{ 104978404, "cannot cast type to canon" },
			{ 1072585573, "Raw file is not a file of the right type" },
			{ 1088278929, "Raw file is not a file of the right type" },
			{ 1099145600, "Can't find asset" },
			{ 1132507782, "bad opcode" },
			{ 1137123674, "GScr_LUINotifyEvent: Expected Istrings, hash or integers only" },
			{ 1252503459, "caller is not an entity" },
			{ 1273214009, "CScr_PlayFX: invalid effect" },
			{ 1333649786, "IsMature can only be called on a player." },
			{ 1364331101, "Object must be an array" },
			{ 1377489376, "Gesture key can't have the higher bit set" },
			{ 1385570291, "assert fail (with message)" },
			{ 1402557361, "bad opcode" },
			{ 1403832952, "Attempt to register ClientField failed.  Client Field set either already contains a field, or a hash collision has occurred." },
			{ 1412130119, "parameter can't be cast to a hash" },
			{ 1427704235, "expected struct type to add value pair" },
			{ 1480037573, "Invalid canonical name hash" },
			{ 1480821566, "Error registering client field.  Attempted field size is not acceptable bit number range 1->32" },
			{ 1517473035, "Value out of range.  Allowed values: 0 to 2" },
			{ 1609894461, "bad entity" },
			{ 1670707254, "linking error" },
			{ 1850691545, "Debug Break" },
			{ 1895566756, "dvar is not a 3d vector, but GetDvarVector3D has been called on it" },
			{ 1909233687, "Optional argument must be a vector type" },
			{ 1915758537, "RegisterClientField can only accept bit ranges between 1 and 32." },
			{ 1957162421, "Can't find bgCache entry" },
			{ 1999906612, "type is not a integer or float" },
			{ 2078816051, "not a valid name for a clientfield set." },
			{ 209668787, "RandomInt parm must be positive integer." },
			{ 2116335949, "function called with too many parameters" },
			{ 219569925, "hasperk() can only be called on local players" },
			{ 219686544, "object is not an array index" },
			{ 2253722136, "CamAnimScripted can only be called on a player." },
			{ 2269096660, "vector scale expecting vector" },
			{ 2279633554, "SV_SetConfigstring: bad index" },
			{ 2331090760, "stack does not have the pool" },
			{ 2344222932, "assert fail" },
			{ 2355618801, "Gesture table key can't have the higher bit set" },
			{ 2408700928, "not a valid name for a clientfield set." },
			{ 2448966512, "string too long" },
			{ 245612264, "foreach should be used with an array" },
			{ 247375020, "Attempting to register client field in client field set using bits, but system is out of space." },
			{ 2479879368, "RegisterClientField can only accept bit ranges between 1 and 32." },
			{ 249068885, "Failed to allocate from state pool" },
			{ 2517242050, "parameter does not exist" },
			{ 2532286589, "error message" },
			{ 2538360978, "not a function pointer" },
			{ 2572009355, "vector scale expecting vector" },
			{ 2606724305, "parameter does not exist" },
			{ 2626909173, "exceeded maximum number of script strings" },
			{ 2681972741, "bad clientfield for name" },
			{ 2687742442, "Forced script exception." },
			{ 269518924, "Ammo count must not be negative" },
			{ 2706458388, "Trying to get a local client index for a client that is not a local client." },
			{ 2734491973, "Error need at least one argument for LUINotifyEvent." },
			{ 2737681163, "assert fail (with message)" },
			{ 2751867714, "self isn't a field object" },
			{ 2792722947, "RegisterClientField can only accept 5 parameters" },
			{ 280703902, "parameter does not exist" },
			{ 2838301872, "Gesture table key can't have the higher bit set" },
			{ 2855209542, "Cannot call IncrementClientField on a non-'counter' type clientfield." },
			{ 2940210599, "Invalid Version Handling. Grab Bat !!!" },
			{ 3015158315, "getperks() can only be called on local players" },
			{ 3016026156, "Can't find asset" },
			{ 3030895505, "Whitelist failure for title" },
			{ 3059411687, "Argument and parameter count mismatch for LUINotifyEvent" },
			{ 3086288875, "playFx called with (0 0 0) forward direction" },
			{ 3122940489, "caller is not an entity" },
			{ 312545010, "not a vector" },
			{ 3143575744, "parameter does not exist" },
			{ 317100267, "unmatching types" },
			{ 3189465155, "Invalid bgCache type" },
			{ 3221522156, "Failed to alloc client field" },
			{ 3222417139, "size cannot be applied to type" },
			{ 3251676101, "Could not load raw file" },
			{ 3255107847, "LUINotifyEvent: entity must be a player entity" },
			{ 3288551912, "expected array type to add value pair" },
			{ 3385008561, "cannot set field on type" },
			{ 3459949409, "Failed to alloc client field - MAX_CLIENTFIELD_FIELDS_IN_SET=512 exceeded." },
			{ 3523382186, "ScrEvent map is full, unable to register new event" },
			{ 3524483616, "Ammo count must not be negative" },
			{ 3592841213, "cannot directly set the origin on AI.  Use the teleport command instead." },
			{ 359760836, "G_Spawn: no free entities" },
			{ 3654063291, "ScrEvent map is full, unable to register new event" },
			{ 3679846953, "LUINotifyEvent: entity must be a player entity" },
			{ 3699844858, "parameter does not exist" },
			{ 3761634992, "not a pointer" },
			{ 3894031202, "Can't find gamedata/playeranim/playeranimtypes.txt" },
			{ 3967909977, "Trying to get version of a demo when the demo system is idle." },
			{ 3990130335, "player getperks(<localClientNum>): localClientNum out of range" },
			{ 4039057166, "LUINotifyEvent: entity must be a player entity" },
			{ 4047738848, "Invalid opcode (Recovery)" },
			{ 409067247, "No clientfield named found in set." },
			{ 4100218247, "Error sending LUI notify event: LUI event name is not precached." },
			{ 4103906837, "Entity is not an item." },
			{ 4104994143, "can't allocate more script variables" },
			{ 4106063796, "key value provided for array is not valid" },
			{ 4163774148, "Optional argument must be a float or integer type" },
			{ 4178589916, "Model was not cached by the linker." },
			{ 4196473479, "parameter does not exist" },
			{ 4213634562, "precacheLeaderboards must be called before any wait statements in the gametype or level script" },
			{ 4220857104, "Cannot call IncrementClientField on a 'counter' type clientfield on the frame it is spawned, since newEnts on the clientside will not process 'counter' type clientfields." },
			{ 467754466, "Error registering client field.  bCallbacksFor0WhenNew (CF_CALLBACK_ZERO_ON_NEW_ENT) is disallowed for CF_SCRIPTMOVERFIELDS." },
			{ 512306404, "not a function pointer" },
			{ 57350207, "Unknown perk" },
			{ 580674660, "Unknown ent type in GScr_GetFieldSetForEntityType." },
			{ 619241173, "Failed to allocate from element pool" },
			{ 647662103, "parameter can't be cast to a string" },
			{ 657813230, "Error registering client field.  bCallbacksFor0WhenNew (CF_CALLBACK_ZERO_ON_NEW_ENT) is disallowed for counter type clientfields. Due to it's treatment of the old and new val as a ring buffer, the counter type is not valid on a new snapshot, new ent, or demojump" },
			{ 665902298, "Parameter must be an array" },
			{ 71894325, "CamAnimScripted or ExtraCamAnimScripted can only be called on a player." },
			{ 732489269, "Non-player entity passed to UploadStats()" },
			{ 744499668, "too many vehicles" },
			{ 750896894, "Invalid bgCache type" },
			{ 753495682, "RandomIntRange's second parameter must be greater than the first." },
			{ 754846421, "invalid vehicle spawn origin" },
			{ 829015102, "var isn't a field object" },
			{ 876169112, "key value provided for struct is not valid" },
			{ 887228744, "origin being set to NAN." },
			{ 904544970, "Attempt to register Client Field post finalization of Client Field registration period failed." },
			{ 941828720, "exitlevel already called" },
			{ 946363963, "Invalid opcode" },
			{ 949934674, "Invalid Version Handling. Grab Bat !!!" },
			{ 952690413, "parameter can't be cast to a boolean" },
			{ 962032109, "Entity does not exist." },
			{ 968521323, "player hasperk(<localClientNum>, <perk>): localClientNum out of range" },
			{ 4088624643, "Can't find asset" },
			{ custom_error_id, "Shield Error" }
		};

	const char* get_error_message(uint64_t code)
	{
		auto it = errors.find(code);
		// known error
		if (it != errors.end())
		{
			return it->second;
		}

		// read from the csv
		static game::BO4_AssetRef_t custom_errors_file = game::AssetRef(fnv1a::generate_hash("gamedata/shield/custom_errors.csv"));

		xassets::stringtable_header* table = xassets::DB_FindXAssetHeader(xassets::ASSET_TYPE_STRINGTABLE, &custom_errors_file, false, -1).stringtable;

		if (!table || !table->columns_count || table->columns_count < 2)
		{
			return nullptr;
		}

		for (size_t i = 0; i < table->rows_count; i++)
		{
			auto* rows = &table->values[i * table->columns_count];

			if (rows[0].type != xassets::STC_TYPE_INT || rows[1].type != xassets::STC_TYPE_STRING)
			{
				continue; // bad types
			}

			if (rows[0].value.hash_value == code)
			{
				return rows[1].value.string_value;
			}
		}

		// unknown
		return nullptr;
	}
}
