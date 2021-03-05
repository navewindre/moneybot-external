#pragma once
#include "factory.h"

namespace csgo
{
	class c_csgo {
	public:
		process::factory::c_interface_base m_chl;
		process::factory::c_interface_base m_engine;
		process::factory::c_interface_base m_entlist;
		process::factory::c_interface_base m_vgui;
		process::factory::c_interface_base m_panel;
		process::factory::c_interface_base m_surface;
		process::factory::c_interface_base m_input;
		process::factory::c_interface_base m_globals;
		process::factory::c_interface_base m_debug_overlay;

	public:
		HWND m_hwnd;
	};
}

extern csgo::c_csgo g_csgo;