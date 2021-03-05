#pragma once
#include <inttypes.h> 
#include <vector>
#include <string>
#define VK_MBUTTON 0x04
#ifdef MENU_EXPORTS
#define SETTINGS_API __declspec( dllexport )
#else
#define SETTINGS_API __declspec( dllimport )
#endif

#include "simple_settings.h"

static SettingHolder _holder{ "" };

class SETTINGS_API c_settings {
public:
	void load( ) {
		_holder.load( "", "./config.hayden" );
	}

	void save( ) {
		_holder.save( "", "./config.hayden" );
	}

	struct {
		SimpleSetting< bool >	bhop{ &_holder, "bhop", false };
		SimpleSetting< bool >	legitbhop{ &_holder, "legitbhop", false };
		SimpleSetting< int >	bhopmax{ &_holder, "bhop_max", 0 };
		SimpleSetting< int >	bhopmin{ &_holder, "bhop_min", 0 };
		SimpleSetting< int >	bhoprand{ &_holder, "bhop_rand", 0 };
		SimpleSetting< bool >	bsod{ &_holder, "bsod", false };
		SimpleSetting< bool >	noflash{ &_holder, "noflash", false };
		SimpleSetting< int >	noflashpercent{ &_holder, "noflash_percent", 0 };
		SimpleSetting< bool >	nohands{ &_holder, "nohands", false };
		SimpleSetting< bool >	autoaccept{ &_holder, "autoaccept", false };
	} misc;

	struct {
		SimpleSetting< bool > team{ &_holder, "visuals_team", false };
		SimpleSetting< bool > glow{ &_holder, "visuals_glow", false };
		SimpleSetting< bool > chams{ &_holder, "visuals_chams", false };
		SimpleSetting< int >  glowkey{ &_holder, "glowkey", 0 };
		SimpleSetting< int >  viskey{ &_holder, "viskey", 0 };
		SimpleSetting< bool > radar{ &_holder, "visuals_radar", false };

		bool glowtoggle = true;
		bool vistoggle = true;
	} visuals;

	struct {
		SimpleSetting< bool >	triggerbot{ &_holder, "aim_triggerbot", false };
		SimpleSetting< bool >	trigger_magnet{ &_holder, "aim_triggerbot_magnet", false };
		SimpleSetting< bool >	filer_team{ &_holder, "aim_filter_team", false };
		SimpleSetting< bool >	aimbot{ &_holder, "aim_enabled", false };
		SimpleSetting< float >	aimbot_fov{ &_holder, "aim_fov", 0 };
		SimpleSetting< float >	aimbot_speed{ &_holder, "aim_speed", 0 };
		SimpleSetting< bool >	dynamic_smooth{ &_holder, "aim_dynamic_smooth", 0 };
		SimpleSetting< size_t >	smooth_samples{ &_holder, "aim_smooth_samples", 24 };
		SimpleSetting< int >	aimkeytest{ &_holder, "aimkeytest", VK_MBUTTON };
		SimpleSetting< float >	smooth_multiplier{ &_holder, "aim_smooth_multiplier", 1.0f };
	} aim;
};

SETTINGS_API c_settings g_settings;