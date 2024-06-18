#include <std_include.hpp>

#include "weapons.hpp"
#include "gsc_funcs.hpp"
#include "hashes.hpp"

#include "definitions/game.hpp"
#include "definitions/variables.hpp"
#include "definitions/xassets.hpp"
#include "definitions/game.hpp"
#include "definitions/dynamic_data.hpp"

#include "loader/component_loader.hpp"
#include <utilities/hook.hpp>
#include <utilities/io.hpp>

namespace weapons
{
	namespace
	{
#define WEAPON_ENUM_DEF(name, title, val) { gsc_funcs::canon_hash_const(name), fnv1a::generate_hash_const(title), val }
        dynamic_data::enum_type enum_types[]
        {
            WEAPON_ENUM_DEF("EModes", "ZOMBIES", game::MODE_ZOMBIES),
            WEAPON_ENUM_DEF("EModes", "MULTIPLAYER", game::MODE_MULTIPLAYER),
            WEAPON_ENUM_DEF("EModes", "CAMPAIGN", game::MODE_CAMPAIGN),
            WEAPON_ENUM_DEF("EModes", "WARZONE", game::MODE_WARZONE),
            WEAPON_ENUM_DEF("EModes", "ALL", game::MODE_COUNT),

            WEAPON_ENUM_DEF("OffhandSlot", "NONE", xassets::OFFHAND_SLOT_NONE),
            WEAPON_ENUM_DEF("OffhandSlot", "LETHAL_GRENADE", xassets::OFFHAND_SLOT_LETHAL_GRENADE),
            WEAPON_ENUM_DEF("OffhandSlot", "TACTICAL_GRENADE", xassets::OFFHAND_SLOT_TACTICAL_GRENADE),
            WEAPON_ENUM_DEF("OffhandSlot", "EQUIPMENT", xassets::OFFHAND_SLOT_EQUIPMENT),
            WEAPON_ENUM_DEF("OffhandSlot", "SPECIFIC_USE", xassets::OFFHAND_SLOT_SPECIFIC_USE),
            WEAPON_ENUM_DEF("OffhandSlot", "GADGET", xassets::OFFHAND_SLOT_GADGET),
            WEAPON_ENUM_DEF("OffhandSlot", "SPECIAL", xassets::OFFHAND_SLOT_SPECIAL),
            WEAPON_ENUM_DEF("OffhandSlot", "HERO_WEAPON", xassets::OFFHAND_SLOT_HERO_WEAPON),
            WEAPON_ENUM_DEF("OffhandSlot", "TAUNT", xassets::OFFHAND_SLOT_TAUNT),
            WEAPON_ENUM_DEF("OffhandSlot", "SCRIPTED", xassets::OFFHAND_SLOT_SCRIPTED),
            WEAPON_ENUM_DEF("OffhandClass", "NONE", xassets::OFFHAND_CLASS_NONE),
            WEAPON_ENUM_DEF("OffhandClass", "FRAG_GRENADE", xassets::OFFHAND_CLASS_FRAG_GRENADE),
            WEAPON_ENUM_DEF("OffhandClass", "SMOKE_GRENADE", xassets::OFFHAND_CLASS_SMOKE_GRENADE),
            WEAPON_ENUM_DEF("OffhandClass", "FLASH_GRENADE", xassets::OFFHAND_CLASS_FLASH_GRENADE),
            WEAPON_ENUM_DEF("OffhandClass", "GEAR", xassets::OFFHAND_CLASS_GEAR),
            WEAPON_ENUM_DEF("OffhandClass", "SUPPLYDROP_MARKER", xassets::OFFHAND_CLASS_SUPPLYDROP_MARKER),
            WEAPON_ENUM_DEF("OffhandClass", "GADGET", xassets::OFFHAND_CLASS_GADGET),

            WEAPON_ENUM_DEF("ProjExplosionType", "NONE", xassets::PROJEXP_NONE),
            WEAPON_ENUM_DEF("ProjExplosionType", "GRENADE", xassets::PROJEXP_GRENADE),
            WEAPON_ENUM_DEF("ProjExplosionType", "ROCKET", xassets::PROJEXP_ROCKET),
            WEAPON_ENUM_DEF("ProjExplosionType", "FLASHBANG", xassets::PROJEXP_FLASHBANG),
            WEAPON_ENUM_DEF("ProjExplosionType", "DUD", xassets::PROJEXP_DUD),
            WEAPON_ENUM_DEF("ProjExplosionType", "SMOKE", xassets::PROJEXP_SMOKE),
            WEAPON_ENUM_DEF("ProjExplosionType", "HEAVY_EXPLOSIVE", xassets::PROJEXP_HEAVY_EXPLOSIVE),
            WEAPON_ENUM_DEF("ProjExplosionType", "FIRE", xassets::PROJEXP_FIRE),
            WEAPON_ENUM_DEF("ProjExplosionType", "NAPALM_BLOB", xassets::PROJEXP_NAPALM_BLOB),
            WEAPON_ENUM_DEF("ProjExplosionType", "BOLD", xassets::PROJEXP_BOLD),
            WEAPON_ENUM_DEF("ProjExplosionType", "SHRAPNEL_SPAN", xassets::PROJEXP_SHRAPNEL_SPAN),

            WEAPON_ENUM_DEF("WeapClass", "RIFLE", xassets::WEAPCLASS_RIFLE),
            WEAPON_ENUM_DEF("WeapClass", "MG", xassets::WEAPCLASS_MG),
            WEAPON_ENUM_DEF("WeapClass", "SMG", xassets::WEAPCLASS_SMG),
            WEAPON_ENUM_DEF("WeapClass", "SPREAD", xassets::WEAPCLASS_SPREAD),
            WEAPON_ENUM_DEF("WeapClass", "PISTOL", xassets::WEAPCLASS_PISTOL),
            WEAPON_ENUM_DEF("WeapClass", "GRENADE", xassets::WEAPCLASS_GRENADE),
            WEAPON_ENUM_DEF("WeapClass", "ROCKETLAUNCHER", xassets::WEAPCLASS_ROCKETLAUNCHER),
            WEAPON_ENUM_DEF("WeapClass", "TURRET", xassets::WEAPCLASS_TURRET),
            WEAPON_ENUM_DEF("WeapClass", "NON_PLAYER", xassets::WEAPCLASS_NON_PLAYER),
            WEAPON_ENUM_DEF("WeapClass", "GAS", xassets::WEAPCLASS_GAS),
            WEAPON_ENUM_DEF("WeapClass", "ITEM", xassets::WEAPCLASS_ITEM),
            WEAPON_ENUM_DEF("WeapClass", "MELEE", xassets::WEAPCLASS_MELEE),
            WEAPON_ENUM_DEF("WeapClass", "KILLSTREAK_ALT_STORED_WEAPON", xassets::WEAPCLASS_KILLSTREAK_ALT_STORED_WEAPON),
            WEAPON_ENUM_DEF("WeapClass", "PISTOL_SPREAD", xassets::WEAPCLASS_PISTOL_SPREAD),
            WEAPON_ENUM_DEF("WeapClass", "BALL", xassets::WEAPCLASS_BALL),

            WEAPON_ENUM_DEF("WeapInventoryType", "PRIMARY", xassets::WEAPINVENTORY_PRIMARY),
            WEAPON_ENUM_DEF("WeapInventoryType", "OFFHAND", xassets::WEAPINVENTORY_OFFHAND),
            WEAPON_ENUM_DEF("WeapInventoryType", "ITEM", xassets::WEAPINVENTORY_ITEM),
            WEAPON_ENUM_DEF("WeapInventoryType", "ALTMODE", xassets::WEAPINVENTORY_ALTMODE),
            WEAPON_ENUM_DEF("WeapInventoryType", "MELEE", xassets::WEAPINVENTORY_MELEE),
            WEAPON_ENUM_DEF("WeapInventoryType", "DWLEFTHAND", xassets::WEAPINVENTORY_DWLEFTHAND),
            WEAPON_ENUM_DEF("WeapInventoryType", "ABILITY", xassets::WEAPINVENTORY_ABILITY),
            WEAPON_ENUM_DEF("WeapInventoryType", "HEAVY", xassets::WEAPINVENTORY_HEAVY),

            WEAPON_ENUM_DEF("GadgetType", "NONE", xassets::GADGET_TYPE_NONE),
            WEAPON_ENUM_DEF("GadgetType", "OTHER", xassets::GADGET_TYPE_OTHER),
            WEAPON_ENUM_DEF("GadgetType", "OPTIC_CAMO", xassets::GADGET_TYPE_OPTIC_CAMO),
            WEAPON_ENUM_DEF("GadgetType", "ARMOR_REGEN", xassets::GADGET_TYPE_ARMOR_REGEN),
            WEAPON_ENUM_DEF("GadgetType", "ARMOR", xassets::GADGET_TYPE_ARMOR),
            WEAPON_ENUM_DEF("GadgetType", "DRONE", xassets::GADGET_TYPE_DRONE),
            WEAPON_ENUM_DEF("GadgetType", "VISION_PULSE", xassets::GADGET_TYPE_VISION_PULSE),
            WEAPON_ENUM_DEF("GadgetType", "MULTI_ROCKET", xassets::GADGET_TYPE_MULTI_ROCKET),
            WEAPON_ENUM_DEF("GadgetType", "TURRET_DEPLOY", xassets::GADGET_TYPE_TURRET_DEPLOY),
            WEAPON_ENUM_DEF("GadgetType", "GRENADE", xassets::GADGET_TYPE_GRENADE),
            WEAPON_ENUM_DEF("GadgetType", "JUKE", xassets::GADGET_TYPE_JUKE),
            WEAPON_ENUM_DEF("GadgetType", "HACKER", xassets::GADGET_TYPE_HACKER),
            WEAPON_ENUM_DEF("GadgetType", "INFRARED", xassets::GADGET_TYPE_INFRARED),
            WEAPON_ENUM_DEF("GadgetType", "SPEED_BURST", xassets::GADGET_TYPE_SPEED_BURST),
            WEAPON_ENUM_DEF("GadgetType", "HERO_WEAPON", xassets::GADGET_TYPE_HERO_WEAPON),
            WEAPON_ENUM_DEF("GadgetType", "COMBAT_EFFICIENCY", xassets::GADGET_TYPE_COMBAT_EFFICIENCY),
            WEAPON_ENUM_DEF("GadgetType", "FLASHBACK", xassets::GADGET_TYPE_FLASHBACK),
            WEAPON_ENUM_DEF("GadgetType", "CLEANSE", xassets::GADGET_TYPE_CLEANSE),
            WEAPON_ENUM_DEF("GadgetType", "SYSTEM_OVERLOAD", xassets::GADGET_TYPE_SYSTEM_OVERLOAD),
            WEAPON_ENUM_DEF("GadgetType", "SERVO_SHORTOUT", xassets::GADGET_TYPE_SERVO_SHORTOUT),
            WEAPON_ENUM_DEF("GadgetType", "EXO_BREAKDOWN", xassets::GADGET_TYPE_EXO_BREAKDOWN),
            WEAPON_ENUM_DEF("GadgetType", "SURGE", xassets::GADGET_TYPE_SURGE),
            WEAPON_ENUM_DEF("GadgetType", "RAVAGE_CORE", xassets::GADGET_TYPE_RAVAGE_CORE),
            WEAPON_ENUM_DEF("GadgetType", "REMOTE_HIJACK", xassets::GADGET_TYPE_REMOTE_HIJACK),
            WEAPON_ENUM_DEF("GadgetType", "IFF_OVERRIDE", xassets::GADGET_TYPE_IFF_OVERRIDE),
            WEAPON_ENUM_DEF("GadgetType", "CACOPHONY", xassets::GADGET_TYPE_CACOPHONY),
            WEAPON_ENUM_DEF("GadgetType", "FORCED_MALFUNCTION", xassets::GADGET_TYPE_FORCED_MALFUNCTION),
            WEAPON_ENUM_DEF("GadgetType", "CONCUSSIVE_WAVE", xassets::GADGET_TYPE_CONCUSSIVE_WAVE),
            WEAPON_ENUM_DEF("GadgetType", "OVERDRIVE", xassets::GADGET_TYPE_OVERDRIVE),
            WEAPON_ENUM_DEF("GadgetType", "UNSTOPPABLE_FORCE", xassets::GADGET_TYPE_UNSTOPPABLE_FORCE),
            WEAPON_ENUM_DEF("GadgetType", "RAPID_STRIKE", xassets::GADGET_TYPE_RAPID_STRIKE),
            WEAPON_ENUM_DEF("GadgetType", "ACTIVE_CAMO", xassets::GADGET_TYPE_ACTIVE_CAMO),
            WEAPON_ENUM_DEF("GadgetType", "SENSORY_OVERLOAD", xassets::GADGET_TYPE_SENSORY_OVERLOAD),
            WEAPON_ENUM_DEF("GadgetType", "ES_STRIKE", xassets::GADGET_TYPE_ES_STRIKE),
            WEAPON_ENUM_DEF("GadgetType", "IMMOLATION", xassets::GADGET_TYPE_IMMOLATION),
            WEAPON_ENUM_DEF("GadgetType", "FIREFLY_SWARM", xassets::GADGET_TYPE_FIREFLY_SWARM),
            WEAPON_ENUM_DEF("GadgetType", "SMOKESCREEN", xassets::GADGET_TYPE_SMOKESCREEN),
            WEAPON_ENUM_DEF("GadgetType", "MISDIRECTION", xassets::GADGET_TYPE_MISDIRECTION),
            WEAPON_ENUM_DEF("GadgetType", "MRPUKEY", xassets::GADGET_TYPE_MRPUKEY),
            WEAPON_ENUM_DEF("GadgetType", "SHOCK_FIELD", xassets::GADGET_TYPE_SHOCK_FIELD),
            WEAPON_ENUM_DEF("GadgetType", "RESURRECT", xassets::GADGET_TYPE_RESURRECT),
            WEAPON_ENUM_DEF("GadgetType", "HEAT_WAVE", xassets::GADGET_TYPE_HEAT_WAVE),
            WEAPON_ENUM_DEF("GadgetType", "CLONE", xassets::GADGET_TYPE_CLONE),
            WEAPON_ENUM_DEF("GadgetType", "ROULETTE", xassets::GADGET_TYPE_ROULETTE),
            WEAPON_ENUM_DEF("GadgetType", "THIEF", xassets::GADGET_TYPE_THIEF),
            WEAPON_ENUM_DEF("GadgetType", "DISRUPTOR", xassets::GADGET_TYPE_DISRUPTOR),
            WEAPON_ENUM_DEF("GadgetType", "HORNET_SWARM", xassets::GADGET_TYPE_HORNET_SWARM),
            WEAPON_ENUM_DEF("GadgetType", "GROUP_REVIVE", xassets::GADGET_TYPE_GROUP_REVIVE),
            WEAPON_ENUM_DEF("GadgetType", "XRAY_EYES", xassets::GADGET_TYPE_XRAY_EYES),
            WEAPON_ENUM_DEF("GadgetType", "EMERGENCY_MELEE", xassets::GADGET_TYPE_EMERGENCY_MELEE),
            WEAPON_ENUM_DEF("GadgetType", "SHOULDER_GUN", xassets::GADGET_TYPE_SHOULDER_GUN),
            WEAPON_ENUM_DEF("GadgetType", "GRAPPLE", xassets::GADGET_TYPE_GRAPPLE),
            WEAPON_ENUM_DEF("GadgetType", "INVULNERABLE", xassets::GADGET_TYPE_INVULNERABLE),
            WEAPON_ENUM_DEF("GadgetType", "SPRINT_BOOST", xassets::GADGET_TYPE_SPRINT_BOOST),
            WEAPON_ENUM_DEF("GadgetType", "HEALTH_REGEN", xassets::GADGET_TYPE_HEALTH_REGEN),
            WEAPON_ENUM_DEF("GadgetType", "SELF_DESTRUCT", xassets::GADGET_TYPE_SELF_DESTRUCT),
            WEAPON_ENUM_DEF("GadgetType", "BARRIER_BUILDER", xassets::GADGET_TYPE_BARRIER_BUILDER),
            WEAPON_ENUM_DEF("GadgetType", "SPAWN_BEACON", xassets::GADGET_TYPE_SPAWN_BEACON),

            WEAPON_ENUM_DEF("GuidedMissileType", "NONE", xassets::MISSILE_GUIDANCE_NONE),
            WEAPON_ENUM_DEF("GuidedMissileType", "SIDEWINDER", xassets::MISSILE_GUIDANCE_SIDEWINDER),
            WEAPON_ENUM_DEF("GuidedMissileType", "HELLFIRE", xassets::MISSILE_GUIDANCE_HELLFIRE),
            WEAPON_ENUM_DEF("GuidedMissileType", "JAVELIN", xassets::MISSILE_GUIDANCE_JAVELIN),
            WEAPON_ENUM_DEF("GuidedMissileType", "BALLISTIC", xassets::MISSILE_GUIDANCE_BALLISTIC),
            WEAPON_ENUM_DEF("GuidedMissileType", "WIREGUIDED", xassets::MISSILE_GUIDANCE_WIREGUIDED),
            WEAPON_ENUM_DEF("GuidedMissileType", "TVGUIDED", xassets::MISSILE_GUIDANCE_TVGUIDED),
            WEAPON_ENUM_DEF("GuidedMissileType", "DRONE", xassets::MISSILE_GUIDANCE_DRONE),
            WEAPON_ENUM_DEF("GuidedMissileType", "HEATSEEKING", xassets::MISSILE_GUIDANCE_HEATSEEKING),
            WEAPON_ENUM_DEF("GuidedMissileType", "ROBOTECH", xassets::MISSILE_GUIDANCE_ROBOTECH),
            WEAPON_ENUM_DEF("GuidedMissileType", "DYNAMICIMPACTPOINT", xassets::MISSILE_GUIDANCE_DYNAMICIMPACTPOINT),

            WEAPON_ENUM_DEF("WeapType", "BULLET", xassets::WEAPTYPE_BULLET),
            WEAPON_ENUM_DEF("WeapType", "GRENADE", xassets::WEAPTYPE_GRENADE),
            WEAPON_ENUM_DEF("WeapType", "PROJECTILE", xassets::WEAPTYPE_PROJECTILE),
            WEAPON_ENUM_DEF("WeapType", "BINOCULARS", xassets::WEAPTYPE_BINOCULARS),
            WEAPON_ENUM_DEF("WeapType", "GAS", xassets::WEAPTYPE_GAS),
            WEAPON_ENUM_DEF("WeapType", "BOMB", xassets::WEAPTYPE_BOMB),
            WEAPON_ENUM_DEF("WeapType", "MINE", xassets::WEAPTYPE_MINE),
            WEAPON_ENUM_DEF("WeapType", "MELEE", xassets::WEAPTYPE_MELEE),
            WEAPON_ENUM_DEF("WeapType", "RIOTSHIELD", xassets::WEAPTYPE_RIOTSHIELD),
            WEAPON_ENUM_DEF("WeapType", "SCRIPT", xassets::WEAPTYPE_SCRIPT),

            WEAPON_ENUM_DEF("LockOnType", "NONE", xassets::LOCK_ON_TYPE_NONE),
            WEAPON_ENUM_DEF("LockOnType", "LEGACY_SINGLE", xassets::LOCK_ON_TYPE_LEGACY_SINGLE),
            WEAPON_ENUM_DEF("LockOnType", "AP_SINGLE", xassets::LOCK_ON_TYPE_AP_SINGLE),
            WEAPON_ENUM_DEF("LockOnType", "AP_MULTI", xassets::LOCK_ON_TYPE_AP_MULTI),
        };

#define WEAPON_FIELD_DEF(name, field, type) { gsc_funcs::canon_hash_pattern(name), dynamic_data::type, sizeof(xassets::weapon_def::field), 1, offsetof(xassets::weapon_def, field) }
#define WEAPON_FIELD_DEFU(field, type) { 0, dynamic_data::type, sizeof(xassets::weapon_def::field), 1, offsetof(xassets::weapon_def, field) }
#define WEAPON_FIELD_DEFE(name, field, type) { gsc_funcs::canon_hash_pattern(name), dynamic_data::named_type(dynamic_data::FT_ENUM, type), sizeof(xassets::weapon_def::field), 1, offsetof(xassets::weapon_def, field) }

        dynamic_data::field weapon_def_fields[]
        {
            WEAPON_FIELD_DEF("weaponheadobjectiveheight", weaponheadobjectiveheight, FT_FLOAT),
            WEAPON_FIELD_DEF("baseWeapon", baseWeapon, FT_XHASH),
            WEAPON_FIELD_DEF("displayname", displayname, FT_XHASH),
            WEAPON_FIELD_DEFE("sessionMode", sessionMode, "EModes"),
            WEAPON_FIELD_DEFU(unk30, FT_XHASH),
            WEAPON_FIELD_DEFU(unk40, FT_XHASH),
            WEAPON_FIELD_DEFU(unk50, FT_XHASH),
            WEAPON_FIELD_DEFU(unk60, FT_XHASH),
            WEAPON_FIELD_DEFU(unk70, FT_XHASH),
            WEAPON_FIELD_DEFU(unk80, FT_XHASH),
            WEAPON_FIELD_DEFU(sound90, FT_XHASH),
            WEAPON_FIELD_DEFU(sounda0, FT_XHASH),
            WEAPON_FIELD_DEFU(soundb0, FT_XHASH),
            WEAPON_FIELD_DEFU(soundc0, FT_XHASH),
            WEAPON_FIELD_DEF("firesounddistant", firesounddistant, FT_XHASH),
            WEAPON_FIELD_DEF("firesound", firesound, FT_XHASH),
            WEAPON_FIELD_DEF("firesoundplayer", firesoundplayer, FT_XHASH),
            WEAPON_FIELD_DEFU(sound100, FT_XHASH),
            WEAPON_FIELD_DEFU(sound110, FT_XHASH),
            WEAPON_FIELD_DEFU(sound120, FT_XHASH),
            WEAPON_FIELD_DEFU(sound130, FT_XHASH),
            WEAPON_FIELD_DEFU(sound140, FT_XHASH),
            WEAPON_FIELD_DEFU(sound150, FT_XHASH),
            WEAPON_FIELD_DEFU(sound160, FT_XHASH),
            WEAPON_FIELD_DEFU(sound170, FT_XHASH),
            WEAPON_FIELD_DEFU(sound180, FT_XHASH),
            WEAPON_FIELD_DEFU(sound190, FT_XHASH),
            WEAPON_FIELD_DEFU(sound1a0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound1b0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound1c0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound1d0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound1e0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound1f0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound200, FT_XHASH),
            WEAPON_FIELD_DEFU(sound210, FT_XHASH),
            WEAPON_FIELD_DEFU(sound220, FT_XHASH),
            WEAPON_FIELD_DEFU(sound230, FT_XHASH),
            WEAPON_FIELD_DEFU(sound240, FT_XHASH),
            WEAPON_FIELD_DEFU(sound250, FT_XHASH),
            WEAPON_FIELD_DEFU(sound260, FT_XHASH),
            WEAPON_FIELD_DEFU(sound270, FT_XHASH),
            WEAPON_FIELD_DEFU(sound280, FT_XHASH),
            WEAPON_FIELD_DEFU(sound290, FT_XHASH),
            WEAPON_FIELD_DEFU(sound2a0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound2b0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound2c0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound2d0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound2e0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound2f0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound300, FT_XHASH),
            WEAPON_FIELD_DEFU(sound310, FT_XHASH),
            WEAPON_FIELD_DEFU(sound320, FT_XHASH),
            WEAPON_FIELD_DEFU(sound330, FT_XHASH),
            WEAPON_FIELD_DEFU(sound340, FT_XHASH),
            WEAPON_FIELD_DEFU(sound350, FT_XHASH),
            WEAPON_FIELD_DEFU(sound360, FT_XHASH),
            WEAPON_FIELD_DEFU(sound370, FT_XHASH),
            WEAPON_FIELD_DEFU(sound380, FT_XHASH),
            WEAPON_FIELD_DEF("gadgetreadysound", gadgetreadysound, FT_XHASH),
            WEAPON_FIELD_DEF("var_1f7ccc3b", var_1f7ccc3b, FT_XHASH),
            WEAPON_FIELD_DEFU(sound3b0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound3c0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound3d0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound3e0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound3f0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound400, FT_XHASH),
            WEAPON_FIELD_DEFU(sound410, FT_XHASH),
            WEAPON_FIELD_DEF("gadgetreadysoundplayer", gadgetreadysoundplayer, FT_XHASH),
            WEAPON_FIELD_DEF("var_fb22040b", var_fb22040b, FT_XHASH),
            WEAPON_FIELD_DEFU(sound440, FT_XHASH),
            WEAPON_FIELD_DEFU(sound450, FT_XHASH),
            WEAPON_FIELD_DEFU(sound460, FT_XHASH),
            WEAPON_FIELD_DEFU(sound470, FT_XHASH),
            WEAPON_FIELD_DEFU(sound480, FT_XHASH),
            WEAPON_FIELD_DEFU(sound490, FT_XHASH),
            WEAPON_FIELD_DEFU(sound4a0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound4b0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound4c0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound4d0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound4e0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound4f0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound500, FT_XHASH),
            WEAPON_FIELD_DEFU(sound510, FT_XHASH),
            WEAPON_FIELD_DEFU(sound520, FT_XHASH),
            WEAPON_FIELD_DEFU(sound530, FT_XHASH),
            WEAPON_FIELD_DEFU(sound540, FT_XHASH),
            WEAPON_FIELD_DEFU(sound550, FT_XHASH),
            WEAPON_FIELD_DEFU(sound560, FT_XHASH),
            WEAPON_FIELD_DEFU(sound570, FT_XHASH),
            WEAPON_FIELD_DEFU(sound580, FT_XHASH),
            WEAPON_FIELD_DEFU(sound590, FT_XHASH),
            WEAPON_FIELD_DEFU(sound5a0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound5b0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound5c0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound5d0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound5e0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound5f0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound600, FT_XHASH),
            WEAPON_FIELD_DEFU(sound610, FT_XHASH),
            WEAPON_FIELD_DEFU(sound620, FT_XHASH),
            WEAPON_FIELD_DEFU(sound630, FT_XHASH),
            WEAPON_FIELD_DEFU(sound640, FT_XHASH),
            WEAPON_FIELD_DEFU(sound650, FT_XHASH),
            WEAPON_FIELD_DEFU(sound660, FT_XHASH),
            WEAPON_FIELD_DEFU(sound670, FT_XHASH),
            WEAPON_FIELD_DEFU(sound680, FT_XHASH),
            WEAPON_FIELD_DEFU(sound690, FT_XHASH),
            WEAPON_FIELD_DEFU(sound6a0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound6b0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound6c0, FT_XHASH),
            WEAPON_FIELD_DEF("lockonseekersearchsound", lockonseekersearchsound, FT_XHASH),
            WEAPON_FIELD_DEF("lockonseekerlockedsound", lockonseekerlockedsound, FT_XHASH),
            WEAPON_FIELD_DEF("lockontargetlockedsound", lockontargetlockedsound, FT_XHASH),
            WEAPON_FIELD_DEF("lockontargetfiredonsound", lockontargetfiredonsound, FT_XHASH),
            WEAPON_FIELD_DEFU(sound710, FT_XHASH),
            WEAPON_FIELD_DEFU(sound720, FT_XHASH),
            WEAPON_FIELD_DEFU(sound730, FT_XHASH),
            WEAPON_FIELD_DEFU(sound740, FT_XHASH),
            WEAPON_FIELD_DEF("var_8a03df2b", var_8a03df2b, FT_XHASH),
            WEAPON_FIELD_DEF("var_2f3ca476", var_2f3ca476, FT_XHASH),
            WEAPON_FIELD_DEF("var_5c29f743", var_5c29f743, FT_XHASH),
            WEAPON_FIELD_DEF("projexplosionsound", projexplosionsound, FT_XHASH),
            WEAPON_FIELD_DEF("projexplosionsoundplayer", projexplosionsoundplayer, FT_XHASH),
            WEAPON_FIELD_DEF("projsmokestartsound", projsmokestartsound, FT_XHASH),
            WEAPON_FIELD_DEF("projsmokeloopsound", projsmokeloopsound, FT_XHASH),
            WEAPON_FIELD_DEF("projsmokeendsound", projsmokeendsound, FT_XHASH),
            WEAPON_FIELD_DEFU(sound7d0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound7e0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound7f0, FT_XHASH),
            WEAPON_FIELD_DEFU(sound800, FT_XHASH),
            WEAPON_FIELD_DEFU(sound810, FT_XHASH),
            WEAPON_FIELD_DEFU(sound820, FT_XHASH),
            WEAPON_FIELD_DEFU(sound830, FT_XHASH),
            WEAPON_FIELD_DEFU(sound840, FT_XHASH),
            WEAPON_FIELD_DEFU(sound850, FT_XHASH),
            WEAPON_FIELD_DEFU(sound860, FT_XHASH),
            WEAPON_FIELD_DEF("hitsound", hitsound, FT_XHASH),
            WEAPON_FIELD_DEFU(unk880, FT_XHASH),
		};

#define WEAPON_FIELD_PROP(name, field, type) { gsc_funcs::canon_hash_pattern(name), dynamic_data::type, sizeof(xassets::weapon_def_properties::field), 1, offsetof(xassets::weapon_def_properties, field) }
#define WEAPON_FIELD_PROPU(field, type) { 0, dynamic_data::type, sizeof(xassets::weapon_def_properties::field), 1, offsetof(xassets::weapon_def_properties, field) }
#define WEAPON_FIELD_PROPE(name, field, type) { gsc_funcs::canon_hash_pattern(name), dynamic_data::named_type(dynamic_data::FT_ENUM, type), sizeof(xassets::weapon_def_properties::field), 1, offsetof(xassets::weapon_def_properties, field) }
		dynamic_data::field weapon_properties_fields[]
		{
            WEAPON_FIELD_PROP("var_77b46a8c", var_77b46a8c, FT_XHASH),
            WEAPON_FIELD_PROP("explosionouterdamage", explosionouterdamage, FT_INT),
            WEAPON_FIELD_PROP("gadget_shockfield_radius", gadget_shockfield_radius, FT_FLOAT),
            WEAPON_FIELD_PROP("gadget_shockfield_damage", gadget_shockfield_damage, FT_FLOAT),
            WEAPON_FIELD_PROP("gadget_flickerondamage", gadget_flickerondamage, FT_INT),
            WEAPON_FIELD_PROP("gadget_flickeronpowerloss", gadget_flickeronpowerloss, FT_INT),
            WEAPON_FIELD_PROP("var_e4109b63", var_e4109b63, FT_INT),
            WEAPON_FIELD_PROP("gadget_max_hitpoints", gadget_max_hitpoints, FT_INT),
            WEAPON_FIELD_PROP("var_c4d4d2a9", var_c4d4d2a9, FT_INT),
            WEAPON_FIELD_PROP("gadget_powerofflossondamage", gadget_powerofflossondamage, FT_FLOAT),
            WEAPON_FIELD_PROP("gadget_powergainonretrieve", gadget_powergainonretrieve, FT_INT),
            WEAPON_FIELD_PROP("gadget_powergainscorefactor", gadget_powergainscorefactor, FT_FLOAT),
            WEAPON_FIELD_PROP("var_c15c6b39", var_c15c6b39, FT_FLOAT),
            WEAPON_FIELD_PROP("var_6a864cad", var_6a864cad, FT_INT),
            WEAPON_FIELD_PROP("gadget_powermoveloss", gadget_powermoveloss, FT_INT),
            WEAPON_FIELD_PROP("gadget_powermovespeed", gadget_powermovespeed, FT_FLOAT),
            WEAPON_FIELD_PROP("gadget_poweronlossondamage", gadget_poweronlossondamage, FT_FLOAT),
            WEAPON_FIELD_PROP("var_58543a1c", var_58543a1c, FT_UINT),
            WEAPON_FIELD_PROP("var_68a0f062", var_68a0f062, FT_INT),
            WEAPON_FIELD_PROP("var_abb570e0", var_abb570e0, FT_FLOAT),
            WEAPON_FIELD_PROP("var_829de2ac", var_829de2ac, FT_INT),
            WEAPON_FIELD_PROP("var_e4d4fa7e", var_e4d4fa7e, FT_FLOAT),
            WEAPON_FIELD_PROP("gadget_powerreplenishfactor", gadget_powerreplenishfactor, FT_FLOAT),
            WEAPON_FIELD_PROP("gadget_power_round_end_active_penalty", gadget_power_round_end_active_penalty, FT_INT),
            WEAPON_FIELD_PROP("gadget_powersprintloss", gadget_powersprintloss, FT_INT),
            WEAPON_FIELD_PROP("var_d911d477", var_d911d477, FT_INT),
            WEAPON_FIELD_PROP("gadget_power_usage_rate", gadget_power_usage_rate, FT_FLOAT),
            WEAPON_FIELD_PROP("var_e1811962", var_e1811962, FT_FLOAT),
            WEAPON_FIELD_PROP("var_66103577", var_66103577, FT_FLOAT),
            WEAPON_FIELD_PROP("var_f9eec1ec", var_f9eec1ec, FT_FLOAT),
            WEAPON_FIELD_PROP("var_6821283d", var_6821283d, FT_INT),
            WEAPON_FIELD_PROP("var_81683931", var_81683931, FT_INT),
            WEAPON_FIELD_PROP("var_1e89f40", var_1e89f40, FT_INT),
            WEAPON_FIELD_PROP("gadget_pulse_duration", gadget_pulse_duration, FT_INT),
            WEAPON_FIELD_PROP("var_4d88a1ff", var_4d88a1ff, FT_INT),
            WEAPON_FIELD_PROP("var_9d776ba6", var_9d776ba6, FT_INT),
            WEAPON_FIELD_PROP("var_b9951041", var_b9951041, FT_INT),
            WEAPON_FIELD_PROP("var_8e0b0827", var_8e0b0827, FT_INT),
            WEAPON_FIELD_PROP("var_a2d7b97c", var_a2d7b97c, FT_FLOAT),
            WEAPON_FIELD_PROP("var_5be370e9", var_5be370e9, FT_FLOAT),
            WEAPON_FIELD_PROP("var_42a3cafc", var_42a3cafc, FT_INT),
            WEAPON_FIELD_PROP("gadget_pulse_max_range", gadget_pulse_max_range, FT_INT),
            WEAPON_FIELD_PROP("var_44c79c09", var_44c79c09, FT_INT),
            WEAPON_FIELD_PROP("var_2cb95b88", var_2cb95b88, FT_UINT),
            WEAPON_FIELD_PROP("gadget_turnoff_onempjammed", gadget_turnoff_onempjammed, FT_UINT),
            WEAPON_FIELD_PROP("gadget_heroversion_2_0", gadget_heroversion_2_0, FT_UINT),
            WEAPON_FIELD_PROP("var_256488f1", var_256488f1, FT_UINT),
            WEAPON_FIELD_PROP("gadget_powergainscoreignoreself", gadget_powergainscoreignoreself, FT_UINT),
            WEAPON_FIELD_PROP("var_f23e9d19", var_f23e9d19, FT_UINT),
            WEAPON_FIELD_PROP("gadget_powergainscoreignorewhenactive", gadget_powergainscoreignorewhenactive, FT_UINT),
            WEAPON_FIELD_PROP("var_ddaa57f2", var_ddaa57f2, FT_UINT),
            WEAPON_FIELD_PROP("gadget_power_reset_on_class_change", gadget_power_reset_on_class_change, FT_UINT),
            WEAPON_FIELD_PROP("gadget_power_reset_on_spawn", gadget_power_reset_on_spawn, FT_UINT),
            WEAPON_FIELD_PROP("gadget_power_reset_on_team_change", gadget_power_reset_on_team_change, FT_UINT),
            WEAPON_FIELD_PROP("gadget_power_reset_on_round_switch", gadget_power_reset_on_round_switch, FT_UINT),
            WEAPON_FIELD_PROP("gadget_power_consume_on_ammo_use", gadget_power_consume_on_ammo_use, FT_UINT),
            WEAPON_FIELD_PROP("var_5801b768", var_5801b768, FT_UINT),
            WEAPON_FIELD_PROP("var_bec5136b", var_bec5136b, FT_UINT),
            WEAPON_FIELD_PROP("var_28bb357c", var_28bb357c, FT_UINT),
            WEAPON_FIELD_PROP("var_f10d73e1", var_f10d73e1, FT_UINT),
            WEAPON_FIELD_PROP("var_7b5016a7", var_7b5016a7, FT_UINT),
            WEAPON_FIELD_PROP("var_ce34bb7e", var_ce34bb7e, FT_UINT),
            WEAPON_FIELD_PROP("var_27c94b15", var_27c94b15, FT_FLOAT),
            WEAPON_FIELD_PROP("var_16e90b80", var_16e90b80, FT_FLOAT),
            WEAPON_FIELD_PROP("var_6cb9946f", var_6cb9946f, FT_FLOAT),
            WEAPON_FIELD_PROP("var_40ffe7d2", var_40ffe7d2, FT_FLOAT),
            WEAPON_FIELD_PROP("aimeleerange", aimeleerange, FT_FLOAT),
            WEAPON_FIELD_PROP("var_6844746b", var_6844746b, FT_FLOAT),
            WEAPON_FIELD_PROP("var_e2b40cd5", var_e2b40cd5, FT_FLOAT),
            WEAPON_FIELD_PROP("damagetoownerscalar", damagetoownerscalar, FT_FLOAT),
            WEAPON_FIELD_PROP("var_c94f007c", var_c94f007c, FT_FLOAT),
            WEAPON_FIELD_PROP("var_3a00e7eb", var_3a00e7eb, FT_FLOAT),
            WEAPON_FIELD_PROP("var_2cf65b96", var_2cf65b96, FT_FLOAT),
            WEAPON_FIELD_PROP("adsspread", adsspread, FT_FLOAT),
            WEAPON_FIELD_PROP("fightdist", fightdist, FT_FLOAT),
            WEAPON_FIELD_PROP("var_1f13c7f1", var_1f13c7f1, FT_FLOAT),
            WEAPON_FIELD_PROP("var_5af80bb6", var_5af80bb6, FT_INT),
            WEAPON_FIELD_PROP("var_36c76157", var_36c76157, FT_FLOAT),
            WEAPON_FIELD_PROP("var_19f920eb", var_19f920eb, FT_FLOAT),
            WEAPON_FIELD_PROP("maxdist", maxdist, FT_FLOAT),
            WEAPON_FIELD_PROP("maxgibdistance", maxgibdistance, FT_FLOAT),
            WEAPON_FIELD_PROP("meleelungerange", meleelungerange, FT_FLOAT),
            WEAPON_FIELD_PROP("var_9aa1ef19", var_9aa1ef19, FT_FLOAT),
            WEAPON_FIELD_PROP("var_cd539cb2", var_cd539cb2, FT_FLOAT),
            WEAPON_FIELD_PROP("var_cb3d0f65", var_cb3d0f65, FT_FLOAT),
            WEAPON_FIELD_PROP("var_7872b3a", var_7872b3a, FT_FLOAT),
            WEAPON_FIELD_PROP("var_367c47fc", var_367c47fc, FT_FLOAT),
            WEAPON_FIELD_PROP("var_ccebc40", var_ccebc40, FT_FLOAT),
            WEAPON_FIELD_PROP("var_deb0b2fe", var_deb0b2fe, FT_FLOAT),
            WEAPON_FIELD_PROP("lifetime", lifetime, FT_FLOAT),
            WEAPON_FIELD_PROP("sprintboostduration", sprintboostduration, FT_FLOAT),
            WEAPON_FIELD_PROP("sprintboostradius", sprintboostradius, FT_FLOAT),
            WEAPON_FIELD_PROP("lockonlossanglehorizontal", lockonlossanglehorizontal, FT_FLOAT),
            WEAPON_FIELD_PROP("lockonlossanglevertical", lockonlossanglevertical, FT_FLOAT),
            WEAPON_FIELD_PROP("lockonanglehorizontal", lockonanglehorizontal, FT_FLOAT),
            WEAPON_FIELD_PROP("lockonanglevertical", lockonanglevertical, FT_FLOAT),
            WEAPON_FIELD_PROP("vehicleprojectiledamagescalar", vehicleprojectiledamagescalar, FT_FLOAT),
            WEAPON_FIELD_PROP("vehicleprojectilesplashdamagescalar", vehicleprojectilesplashdamagescalar, FT_FLOAT),
            WEAPON_FIELD_PROP("var_416021d8", var_416021d8, FT_FLOAT),
            WEAPON_FIELD_PROP("aiFuseTime", aiFuseTime, FT_INT),
            WEAPON_FIELD_PROP("burndamage", burndamage, FT_INT),
            WEAPON_FIELD_PROP("burndamageinterval", burndamageinterval, FT_INT),
            WEAPON_FIELD_PROP("burnduration", burnduration, FT_INT),
            WEAPON_FIELD_PROP("burstcount", burstcount, FT_INT),
            WEAPON_FIELD_PROP("var_98333ae", var_98333ae, FT_INT),
            WEAPON_FIELD_PROP("var_1c0e3cb7", var_1c0e3cb7, FT_INT),
            WEAPON_FIELD_PROP("var_4941de5", var_4941de5, FT_INT),
            WEAPON_FIELD_PROP("var_f7e67f28", var_f7e67f28, FT_INT),
            WEAPON_FIELD_PROP("var_227c90e1", var_227c90e1, FT_INT),
            WEAPON_FIELD_PROP("var_ab300840", var_ab300840, FT_INT),
            WEAPON_FIELD_PROP("var_849af6b4", var_849af6b4, FT_INT),
            WEAPON_FIELD_PROP("var_c4aae0fa", var_c4aae0fa, FT_INT),
            WEAPON_FIELD_PROP("var_5ac2e7a4", var_5ac2e7a4, FT_INT),
            WEAPON_FIELD_PROP("var_76127e14", var_76127e14, FT_INT),
            WEAPON_FIELD_PROP("var_df381b5d", var_df381b5d, FT_INT),
            WEAPON_FIELD_PROP("var_a8bd8bb2", var_a8bd8bb2, FT_INT),
            WEAPON_FIELD_PROP("var_8134b209", var_8134b209, FT_INT),
            WEAPON_FIELD_PROP("heal", heal, FT_INT),
            WEAPON_FIELD_PROP("var_4465ef1e", var_4465ef1e, FT_INT),
            WEAPON_FIELD_PROP("ammoindex", ammoindex, FT_INT),
            WEAPON_FIELD_PROP("iClipSize", iClipSize, FT_UINT),
            WEAPON_FIELD_PROP("var_ec2cbce2", var_ec2cbce2, FT_INT),
            WEAPON_FIELD_PROP("var_df0f9ce9", var_df0f9ce9, FT_INT),
            WEAPON_FIELD_PROP("var_1123a989", var_1123a989, FT_INT),
            WEAPON_FIELD_PROP("iMaxAmmo", iMaxAmmo, FT_UINT),
            WEAPON_FIELD_PROP("meleedamage", meleedamage, FT_UINT),
            WEAPON_FIELD_PROP("meleetime", meleetime, FT_UINT),
            WEAPON_FIELD_PROP("iStartAmmo", iStartAmmo, FT_UINT),
            WEAPON_FIELD_PROP("fuellife", fuellife, FT_INT),
            WEAPON_FIELD_PROP("lockonlossradius", lockonlossradius, FT_INT),
            WEAPON_FIELD_PROP("lockonminrange", lockonminrange, FT_INT),
            WEAPON_FIELD_PROP("lockonradius", lockonradius, FT_INT),
            WEAPON_FIELD_PROP("lockonscreenradius", lockonscreenradius, FT_INT),
            WEAPON_FIELD_PROP("maxheal", maxheal, FT_INT),
            WEAPON_FIELD_PROP("maxinstancesallowed", maxinstancesallowed, FT_INT),
            WEAPON_FIELD_PROP("meleechargetime", meleechargetime, FT_UINT),
            WEAPON_FIELD_PROP("meleepowertime", meleepowertime, FT_UINT),
            WEAPON_FIELD_PROP("meleepowertimeleft", meleepowertimeleft, FT_UINT),
            WEAPON_FIELD_PROP("var_cddb5cd0", var_cddb5cd0, FT_INT),
            WEAPON_FIELD_PROP("var_e5db3b95", var_e5db3b95, FT_INT),
            WEAPON_FIELD_PROP("var_61d29b2f", var_61d29b2f, FT_INT),
            WEAPON_FIELD_PROP("multidetonation", multidetonation, FT_INT),
            WEAPON_FIELD_PROP("var_c264efc6", var_c264efc6, FT_INT),
            WEAPON_FIELD_PROP("var_95d8fabf", var_95d8fabf, FT_INT),
            WEAPON_FIELD_PROP("playerdamage", playerdamage, FT_INT),
            WEAPON_FIELD_PROP("var_ac36c1db", var_ac36c1db, FT_INT),
            WEAPON_FIELD_PROP("var_60563796", var_60563796, FT_INT),
            WEAPON_FIELD_PROP("var_7a93ed37", var_7a93ed37, FT_INT),
            WEAPON_FIELD_PROP("ammoCountClipRelative", ammoCountClipRelative, FT_UINT),
            WEAPON_FIELD_PROP("var_8032088a", var_8032088a, FT_UINT),
            WEAPON_FIELD_PROP("ammoregen", ammoregen, FT_UINT),
            WEAPON_FIELD_PROP("var_f6dea63a", var_f6dea63a, FT_UINT),
            WEAPON_FIELD_PROP("altoffhand", altoffhand, FT_UINT),
            WEAPON_FIELD_PROP("bAltWeaponDisableSwitching", bAltWeaponDisableSwitching, FT_UINT),
            WEAPON_FIELD_PROP("anyplayercanretrieve", anyplayercanretrieve, FT_UINT),
            WEAPON_FIELD_PROP("isboltaction", isboltaction, FT_UINT),
            WEAPON_FIELD_PROP("bulletimpactexplode", bulletimpactexplode, FT_UINT),
            WEAPON_FIELD_PROP("canuseunderwater", canuseunderwater, FT_UINT),
            WEAPON_FIELD_PROP("iscliponly", iscliponly, FT_UINT),
            WEAPON_FIELD_PROP("var_337fc1cf", var_337fc1cf, FT_UINT),
            WEAPON_FIELD_PROP("craftitem", craftitem, FT_UINT),
            WEAPON_FIELD_PROP("var_9fffdcee", var_9fffdcee, FT_UINT),
            WEAPON_FIELD_PROP("decoy", decoy, FT_UINT),
            WEAPON_FIELD_PROP("destroyablebytrophysystem", destroyablebytrophysystem, FT_UINT),
            WEAPON_FIELD_PROP("bDieOnRespawn", bDieOnRespawn, FT_UINT),
            WEAPON_FIELD_PROP("disabledeploy", disabledeploy, FT_UINT),
            WEAPON_FIELD_PROP("disallowatmatchstart", disallowatmatchstart, FT_UINT),
            WEAPON_FIELD_PROP("dodamagefeedback", dodamagefeedback, FT_UINT),
            WEAPON_FIELD_PROP("doempdestroyfx", doempdestroyfx, FT_UINT),
            WEAPON_FIELD_PROP("doesfiredamage", doesfiredamage, FT_UINT),
            WEAPON_FIELD_PROP("dohackedstats", dohackedstats, FT_UINT),
            WEAPON_FIELD_PROP("dostun", dostun, FT_UINT),
            WEAPON_FIELD_PROP("isdualwield", isdualwield, FT_UINT),
            WEAPON_FIELD_PROP("var_251796e3", var_251796e3, FT_UINT),
            WEAPON_FIELD_PROP("ignoresflakjacket", ignoresflakjacket, FT_UINT),
            WEAPON_FIELD_PROP("ignoreslightarmor", ignoreslightarmor, FT_UINT),
            WEAPON_FIELD_PROP("ignorespowerarmor", ignorespowerarmor, FT_UINT),
            WEAPON_FIELD_PROP("ignoreteamkills", ignoreteamkills, FT_UINT),
            WEAPON_FIELD_PROP("isaikillstreakdamage", isaikillstreakdamage, FT_UINT),
            WEAPON_FIELD_PROP("isballisticknife", isballisticknife, FT_UINT),
            WEAPON_FIELD_PROP("iscarriedkillstreak", iscarriedkillstreak, FT_UINT),
            WEAPON_FIELD_PROP("isemp", isemp, FT_UINT),
            WEAPON_FIELD_PROP("isflash", isflash, FT_UINT),
            WEAPON_FIELD_PROP("isflourishweapon", isflourishweapon, FT_UINT),
            WEAPON_FIELD_PROP("isgameplayweapon", isgameplayweapon, FT_UINT),
            WEAPON_FIELD_PROP("ishacktoolweapon", ishacktoolweapon, FT_UINT),
            WEAPON_FIELD_PROP("bIsHybridWeapon", bIsHybridWeapon, FT_UINT),
            WEAPON_FIELD_PROP("isperkbottle", isperkbottle, FT_UINT),
            WEAPON_FIELD_PROP("isscavengable", isscavengable, FT_UINT),
            WEAPON_FIELD_PROP("issniperweapon", issniperweapon, FT_UINT),
            WEAPON_FIELD_PROP("isstun", isstun, FT_UINT),
            WEAPON_FIELD_PROP("issupplydropweapon", issupplydropweapon, FT_UINT),
            WEAPON_FIELD_PROP("var_130391b9", var_130391b9, FT_UINT),
            WEAPON_FIELD_PROP("var_6f12adba", var_6f12adba, FT_UINT),
            WEAPON_FIELD_PROP("blocksprone", blocksprone, FT_UINT),
            WEAPON_FIELD_PROP("meleeignoreslightarmor", meleeignoreslightarmor, FT_UINT),
            WEAPON_FIELD_PROP("nohitmarker", nohitmarker, FT_UINT),
            WEAPON_FIELD_PROP("nonstowedweapon", nonstowedweapon, FT_UINT),
            WEAPON_FIELD_PROP("notkillstreak", notkillstreak, FT_UINT),
            WEAPON_FIELD_PROP("setusedstat", setusedstat, FT_UINT),
            WEAPON_FIELD_PROP("shownenemyequip", shownenemyequip, FT_UINT),
            WEAPON_FIELD_PROP("shownenemyexplo", shownenemyexplo, FT_UINT),
            WEAPON_FIELD_PROP("shownretrievable", shownretrievable, FT_UINT),
            WEAPON_FIELD_PROP("skipbattlechatterkill", skipbattlechatterkill, FT_UINT),
            WEAPON_FIELD_PROP("skipbattlechatterreload", skipbattlechatterreload, FT_UINT),
            WEAPON_FIELD_PROP("skiplowammovox", skiplowammovox, FT_UINT),
            WEAPON_FIELD_PROP("specialpain", specialpain, FT_UINT),
            WEAPON_FIELD_PROP("isthrowback", isthrowback, FT_UINT),
            WEAPON_FIELD_PROP("var_9111ccc0", var_9111ccc0, FT_UINT),
            WEAPON_FIELD_PROP("damagealwayskillsplayer", damagealwayskillsplayer, FT_UINT),
            WEAPON_FIELD_PROP("deployable", deployable, FT_UINT),
            WEAPON_FIELD_PROP("var_e0d42861", var_e0d42861, FT_UINT),
            WEAPON_FIELD_PROP("var_3344c07e", var_3344c07e, FT_UINT),
            WEAPON_FIELD_PROP("var_70b09d5b", var_70b09d5b, FT_UINT),
            WEAPON_FIELD_PROP("destroysequipment", destroysequipment, FT_UINT),
            WEAPON_FIELD_PROP("var_8025ffca", var_8025ffca, FT_UINT),
            WEAPON_FIELD_PROP("var_f076a292", var_f076a292, FT_UINT),
            WEAPON_FIELD_PROP("doannihilate", doannihilate, FT_UINT),
            WEAPON_FIELD_PROP("var_6262fd11", var_6262fd11, FT_UINT),
            WEAPON_FIELD_PROP("doblowback", doblowback, FT_UINT),
            WEAPON_FIELD_PROP("dogibbing", dogibbing, FT_UINT),
            WEAPON_FIELD_PROP("dogibbingonmelee", dogibbingonmelee, FT_UINT),
            WEAPON_FIELD_PROP("donotdamageowner", donotdamageowner, FT_UINT),
            WEAPON_FIELD_PROP("forcedamagehitlocation", forcedamagehitlocation, FT_UINT),
            WEAPON_FIELD_PROP("var_33d50507", var_33d50507, FT_UINT),
            WEAPON_FIELD_PROP("grappleweapon", grappleweapon, FT_UINT),
            WEAPON_FIELD_PROP("var_fab9617b", var_fab9617b, FT_UINT),
            WEAPON_FIELD_PROP("var_ca947940", var_ca947940, FT_UINT),
            WEAPON_FIELD_PROP("isnotdroppable", isnotdroppable, FT_UINT),
            WEAPON_FIELD_PROP("issignatureweapon", issignatureweapon, FT_UINT),
            WEAPON_FIELD_PROP("istacticalinsertion", istacticalinsertion, FT_UINT),
            WEAPON_FIELD_PROP("var_76ce72e8", var_76ce72e8, FT_UINT),
            WEAPON_FIELD_PROP("var_ff0b00ba", var_ff0b00ba, FT_UINT),
            WEAPON_FIELD_PROP("isvaluable", isvaluable, FT_UINT),
            WEAPON_FIELD_PROP("var_29d24e37", var_29d24e37, FT_UINT),
            WEAPON_FIELD_PROP("mountable", mountable, FT_UINT),
            WEAPON_FIELD_PROP("var_ba335ef", var_ba335ef, FT_UINT),
            WEAPON_FIELD_PROP("noadslockoncheck", noadslockoncheck, FT_UINT),
            WEAPON_FIELD_PROP("var_d69ee9ed", var_d69ee9ed, FT_UINT),
            WEAPON_FIELD_PROP("requirelockontofire", requirelockontofire, FT_UINT),
            WEAPON_FIELD_PROP("var_8072cf0b", var_8072cf0b, FT_UINT),
            WEAPON_FIELD_PROP("istimeddetonation", istimeddetonation, FT_UINT),
            WEAPON_FIELD_PROP("unlimitedAmmo", unlimitedAmmo, FT_UINT),
            WEAPON_FIELD_PROP("var_18608bfe", var_18608bfe, FT_UINT),
            WEAPON_FIELD_PROP("var_775d2aad", var_775d2aad, FT_UINT),
            WEAPON_FIELD_PROP("var_b8a85edd", var_b8a85edd, FT_UINT),
            WEAPON_FIELD_PROP("var_dbbd4cec", var_dbbd4cec, FT_UINT),
            WEAPON_FIELD_PROPE("gadget_type", gadget_type, "GadgetType"),
            WEAPON_FIELD_PROPE("guidedMissileType", guidedMissileType, "GuidedMissileType"),
            WEAPON_FIELD_PROPE("offhandClass", offhandClass, "OffhandClass"),
            WEAPON_FIELD_PROPE("offhandSlot", offhandSlot, "OffhandSlot"),
            WEAPON_FIELD_PROPE("inventoryType", inventoryType, "WeapInventoryType"),
            WEAPON_FIELD_PROPE("lockontype", lockontype, "LockOnType"),
            WEAPON_FIELD_PROPE("projExplosionType", projExplosionType, "ProjExplosionType"),
            WEAPON_FIELD_PROPE("weapType", weapType, "WeapType"),
		};

        dynamic_data::dd_container weapon_def_container{ weapon_def_fields, ARRAYSIZE(weapon_def_fields), enum_types, ARRAYSIZE(enum_types) };
        dynamic_data::dd_container weapon_properties_container{ weapon_properties_fields, ARRAYSIZE(weapon_properties_fields), enum_types, ARRAYSIZE(enum_types) };

		struct mod_weapon
		{
			xassets::weapon_def def{};
			xassets::weapon_def_properties props{};
			byte* def_mask[sizeof(xassets::weapon_def)]{};
			byte* props_mask[sizeof(xassets::weapon_def_properties)]{};
		};
		std::unordered_map<uint64_t, std::shared_ptr<mod_weapon>> defs_delta{};
	}

	void* get_or_patch_weapon(void* weapon_def)
	{
        if (!weapon_def)
        {
            return weapon_def;
        }
		// patch already existing weapon def
		xassets::weapon_def* def = (xassets::weapon_def*)weapon_def;
		uint64_t name = def->name.hash;

		auto it = defs_delta.find(name & 0x7FFFFFFFFFFFFFFFuLL);

		if (it == defs_delta.end())
		{
			return weapon_def;
		}

		const mod_weapon& weap = *it->second;

		// patch weapon
		utilities::hook::copy(def, &weap.def, weap.def_mask, sizeof(weap.def));
        utilities::hook::copy(def->properties, &weap.props, weap.props_mask, sizeof(weap.props));

		return def;
	}

	void* get_weapon_asset(uint64_t name)
	{
		return nullptr; // todo: not implemented
	}

	void clear_weapon_defs() {
		defs_delta.clear();
	}

	bool load_weapon_def(rapidjson::Value& member, const char* mod_name, const std::filesystem::path& mod_path) {
		auto name_mb = member.FindMember("name");
		auto path_mb = member.FindMember("path");
        auto format_mb = member.FindMember("format");

		if (
			name_mb == member.MemberEnd() || path_mb == member.MemberEnd() || format_mb == member.MemberEnd()
			|| !name_mb->value.IsString() || !path_mb->value.IsString() || !format_mb->value.IsString()
			)
		{
			logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a bad weapon def, missing/bad name, format or path", mod_name));
			return false;
		}

		const char* format_st = format_mb->value.GetString();

		bool delta{};
		if (!_strcmpi(format_st, "delta"))
		{
			delta = true;
		}
		else if (!_strcmpi(format_st, "full"))
		{
			logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing a full weapon, it is not implemented yet.", mod_name));
			return false;
		}
		else
		{
			logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing weapon with an unknown format '{}'.", mod_name, format_st));
			return false;
		}

		auto def = std::make_shared<mod_weapon>();

		std::filesystem::path path_cfg = path_mb->value.GetString();
		auto weapon_file_path = path_cfg.is_absolute() ? path_cfg : (mod_path / path_cfg);


        std::string filename = weapon_file_path.string();
        std::string weapon_data{};
        if (!utilities::io::read_file(filename, &weapon_data))
        {
            logger::write(logger::LOG_TYPE_ERROR, std::format("can't read mod weapon file '{}'", filename));
            return false;
        }

        rapidjson::Document wnfo{};

        wnfo.Parse(weapon_data);

        if (wnfo.HasParseError()) {
            logger::write(logger::LOG_TYPE_ERROR, std::format("can't parse weapon json data '{}'", filename));
            return false;
        }

		uint64_t hash_name = fnv1a::generate_hash_pattern(name_mb->value.GetString());
        def->def.name.hash = hash_name;
		// load def

        auto weapon_def = wnfo.FindMember("def");
        auto weapon_props = wnfo.FindMember("properties");

        try
        {
            if (weapon_def != wnfo.MemberEnd() && weapon_def->value.IsObject())
            {
                weapon_def_container.load_dd(weapon_def->value, &def->def, def->def_mask);
            }

            if (weapon_props != wnfo.MemberEnd() && weapon_props->value.IsObject())
            {
                weapon_properties_container.load_dd(weapon_props->value, &def->props, def->props_mask);
            }
        }
        catch (std::runtime_error& err)
        {
            logger::write(logger::LOG_TYPE_WARN, std::format("mod {} is containing weapon with a bad definition: {}", mod_name, err.what()));
            return false;
        }

        assert(def->def.name.hash == hash_name && "DD error, redefined name");

		if (delta) {
			defs_delta[hash_name & 0x7FFFFFFFFFFFFFFFuLL] = def;
		}
		else
		{
			assert(0 && "Not implemented");
		}

		return true;
	}
}