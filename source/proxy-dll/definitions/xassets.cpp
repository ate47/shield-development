#include <std_include.hpp>
#include "xassets.hpp"

namespace xassets
{
	namespace
	{
		const char* cache_names[]
		{
			"invalid",
			"vehicle",
			"model",
			"aitype",
			"character",
			"xmodelalias",
			"weapon",
			"gesture",
			"gesturetable",
			"zbarrier",
			"rumble",
			"shellshock",
			"statuseffect",
			"xcam",
			"destructible",
			"streamerhint",
			"flowgraph",
			"xanim",
			"sanim",
			"scriptbundle",
			"talent",
			"statusicon",
			"locationselector",
			"menu",
			"material",
			"string",
			"eventstring",
			"moviefile",
			"objective",
			"fx",
			"lui_menu_data",
			"lui_elem",
			"lui_elem_uid",
			"radiant_exploder",
			"soundalias",
			"client_fx",
			"client_tagfxset",
			"client_lui_elem",
			"client_lui_elem_uid",
			"requires_implements"
		};
        const char* weap_inventory_type_names[]
        {
            "primary",
            "offhand",
            "item",
            "altmode",
            "melee",
            "dwlefthand",
            "ability",
            "heavy",
        };
        const char* weap_type_names[]
        {
            "bullet",
            "grenade",
            "projectile",
            "binoculars",
            "gas",
            "bomb",
            "mine",
            "melee",
            "riotshield",
            "script",
        };
        const char* gadget_type_names[]
        {
            "none",
            "other",
            "optic_camo",
            "armor_regen",
            "armor",
            "drone",
            "vision_pulse",
            "multi_rocket",
            "turret_deploy",
            "grenade",
            "juke",
            "hacker",
            "infrared",
            "speed_burst",
            "hero_weapon",
            "combat_efficiency",
            "flashback",
            "cleanse",
            "system_overload",
            "servo_shortout",
            "exo_breakdown",
            "surge",
            "ravage_core",
            "remote_hijack",
            "iff_override",
            "cacophony",
            "forced_malfunction",
            "concussive_wave",
            "overdrive",
            "unstoppable_force",
            "rapid_strike",
            "active_camo",
            "sensory_overload",
            "es_strike",
            "immolation",
            "firefly_swarm",
            "smokescreen",
            "misdirection",
            "mrpukey",
            "shock_field",
            "resurrect",
            "heat_wave",
            "clone",
            "roulette",
            "thief",
            "disruptor",
            "hornet_swarm",
            "group_revive",
            "xray_eyes",
            "emergency_melee",
            "shoulder_gun",
            "grapple",
            "invulnerable",
            "sprint_boost",
            "health_regen",
            "self_destruct",
            "barrier_builder",
            "spawn_beacon",
        };
        const char* guided_missile_type_names[]
        {
            "none",
            "sidewinder",
            "hellfire",
            "javelin",
            "ballistic",
            "wireguided",
            "tvguided",
            "drone",
            "heatseeking",
            "robotech",
            "dynamicimpactpoint",
        };
        const char* offhand_class_names[]
        {
            "none",
            "frag_grenade",
            "smoke_grenade",
            "flash_grenade",
            "gear",
            "supplydrop_marker",
            "gadget",
        };
        const char* offhand_slot_names[]
        {
            "none",
            "lethal_grenade",
            "tactical_grenade",
            "equipment",
            "specific_use",
            "gadget",
            "special",
            "hero_weapon",
            "taunt",
            "scripted",
        };
        const char* lock_on_type_names[]
        {
            "none",
            "legacy_single",
            "ap_single",
            "ap_multi",
        };
        const char* weap_class_names[]
        {
            "rifle",
            "mg",
            "smg",
            "spread",
            "pistol",
            "grenade",
            "rocketlauncher",
            "turret",
            "non_player",
            "gas",
            "item",
            "melee",
            "killstreak_alt_stored_weapon",
            "pistol_spread",
            "ball",
        };
        const char* proj_explosion_type_names[]
        {
            "none",
            "grenade",
            "rocket",
            "flashbang",
            "dud",
            "smoke",
            "heavy_explosive",
            "fire",
            "napalm_blob",
            "bold",
            "shrapnel_span",
        };
	}

	BGCacheTypes BG_Cache_GetTypeIndex(const char* name)
	{
		//BG_Cache_GetTypeIndexInternal base isn't available after unpack
		for (size_t i = 0; i < BGCacheTypes::BG_CACHE_TYPE_COUNT; i++)
		{
			if (!_strcmpi(name, cache_names[i]))
			{
				return (BGCacheTypes)i;
			}
		}
		return BG_CACHE_TYPE_INVALID;
	}

	const char* BG_Cache_GetTypeName(BGCacheTypes type)
	{
		return cache_names[type];
	}

	XAssetType DB_GetXAssetTypeIndex(const char* name)
	{
		for (size_t i = 0; i < XAssetType::ASSET_TYPE_COUNT; i++)
		{
			if (!_strcmpi(name, s_XAssetTypeInfo[i].name))
			{
				return (XAssetType)i;
			}
		}
		return XAssetType::ASSET_TYPE_COUNT;
	}

	const char* DB_GetXAssetTypeName(XAssetType type)
	{
		if (type >= ASSET_TYPE_COUNT)
		{
			return "invalid";
		}
		return s_XAssetTypeInfo[type].name;
	}

    const char* weap_inventory_type_get_name(weapInventoryType_t t) {
        return t >= ARRAYSIZE(weap_inventory_type_names) ? "invalid" : weap_inventory_type_names[t];
    }

    const char* weap_type_get_name(weapType_t t) {
        return t >= ARRAYSIZE(weap_type_names) ? "invalid" : weap_type_names[t];
    }

    const char* gadget_type_get_name(gadgetType_e t) {
        return t >= ARRAYSIZE(gadget_type_names) ? "invalid" : gadget_type_names[t];
    }

    const char* guided_missile_type_get_name(guidedMissileType_t t) {
        return t >= ARRAYSIZE(guided_missile_type_names) ? "invalid" : guided_missile_type_names[t];
    }

    const char* offhand_class_get_name(OffhandClass t) {
        return t >= ARRAYSIZE(offhand_class_names) ? "invalid" : offhand_class_names[t];
    }

    const char* offhand_slot_get_name(OffhandSlot t) {
        return t >= ARRAYSIZE(offhand_slot_names) ? "invalid" : offhand_slot_names[t];
    }

    const char* lock_on_type_get_name(lockOnType_t t) {
        return t >= ARRAYSIZE(lock_on_type_names) ? "invalid" : lock_on_type_names[t];
    }

    const char* weap_class_get_name(weapClass_t t) {
        return t >= ARRAYSIZE(weap_class_names) ? "invalid" : weap_class_names[t];
    }

    const char* proj_explosion_type_get_name(projExplosionType_t t) {
        return t >= ARRAYSIZE(proj_explosion_type_names) ? "invalid" : proj_explosion_type_names[t];
    }

    weapInventoryType_t weap_inventory_type_from_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(weap_inventory_type_names); i++)
        {
            if (!_strcmpi(name, weap_inventory_type_names[i]))
            {
                return (weapInventoryType_t)i;
            }
        }
        return weapInventoryType_t::WEAPINVENTORYCOUNT;
    }

    weapType_t weap_type_from_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(weap_type_names); i++)
        {
            if (!_strcmpi(name, weap_type_names[i]))
            {
                return (weapType_t)i;
            }
        }
        return weapType_t::WEAPTYPE_NUM;
    }

    gadgetType_e gadget_type_from_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(gadget_type_names); i++)
        {
            if (!_strcmpi(name, gadget_type_names[i]))
            {
                return (gadgetType_e)i;
            }
        }
        return gadgetType_e::GADGET_TYPE_COUNT;
    }

    guidedMissileType_t guided_missile_type_from_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(guided_missile_type_names); i++)
        {
            if (!_strcmpi(name, guided_missile_type_names[i]))
            {
                return (guidedMissileType_t)i;
            }
        }
        return guidedMissileType_t::MISSILE_GUIDANCE_COUNT;
    }

    OffhandClass offhand_class_from_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(offhand_class_names); i++)
        {
            if (!_strcmpi(name, offhand_class_names[i]))
            {
                return (OffhandClass)i;
            }
        }
        return OffhandClass::OFFHAND_CLASS_COUNT;
    }

    OffhandSlot offhand_slot_from_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(offhand_slot_names); i++)
        {
            if (!_strcmpi(name, offhand_slot_names[i]))
            {
                return (OffhandSlot)i;
            }
        }
        return OffhandSlot::OFFHAND_SLOT_COUNT;
    }

    lockOnType_t lock_on_type_from_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(lock_on_type_names); i++)
        {
            if (!_strcmpi(name, lock_on_type_names[i]))
            {
                return (lockOnType_t)i;
            }
        }
        return lockOnType_t::LOCK_ON_TYPE_COUNT;
    }

    weapClass_t weap_class_from_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(weap_class_names); i++)
        {
            if (!_strcmpi(name, weap_class_names[i]))
            {
                return (weapClass_t)i;
            }
        }
        return weapClass_t::WEAPCLASS_NUM;
    }

    projExplosionType_t proj_explosion_type_name(const char* name) {
        for (size_t i = 0; i < ARRAYSIZE(proj_explosion_type_names); i++)
        {
            if (!_strcmpi(name, proj_explosion_type_names[i]))
            {
                return (projExplosionType_t)i;
            }
        }
        return projExplosionType_t::PROJEXP_NUM;
    }

}