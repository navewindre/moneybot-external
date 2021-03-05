#include "factory.h"
#include "client.h"
#include "cheat.h"
#include "math.h"
#include "hook.h"

using namespace process;

HMODULE
	factory::c_interface_base::sm_steamapi,
	factory::c_interface_base::sm_tier0,
	factory::c_interface_base::sm_vst,
	factory::c_interface_base::sm_phonon3d,
	factory::c_interface_base::sm_netsock;


features::c_base_cheat	g_cheat;
csgo::c_csgo			g_csgo;
util::c_math			g_math;
c_instance				g_instance;
c_process				g_proc;
util::c_hook_manager	g_hooks( std::make_shared< decltype( g_proc ) >( g_proc ) );

bool factory::commit( c_process* proc, c_instance* instance, csgo::c_csgo* game ) {
	proc->init( xorstr( "csgo.exe" ) );

	if ( !proc->get_id( ) ) {
		function_printf( xorstr( "process id not found" ) );
		return false;
	}

	if ( !proc->get_handle( ) ) {
		function_printf( xorstr( "process handle unavailable" ) );
		return false;
	}

	proc->get_module( &proc->m_client,		xorstr( "client.dll" ) );
	proc->get_module( &proc->m_steam_api,	xorstr( "steam_api.dll" ) );
	proc->get_module( &proc->m_tier0,		xorstr( "tier0.dll" ) );
	proc->get_module( &proc->m_vst,			xorstr( "vstdlib.dll" ) );
	proc->get_module( &proc->m_phonon3d,	xorstr( "phonon.dll" ) ); //yaaa fuckeer
	proc->get_module( &proc->m_surface,		xorstr( "vguimatsurface.dll" ) );
	proc->get_module( &proc->m_vgui,		xorstr( "vgui2.dll" ) );
	proc->get_module( &proc->m_engine,		xorstr( "engine.dll" ) );
	proc->get_module( &proc->m_netsock,		xorstr( "steamnetworkingsockets.dll" ) );

	factory::c_interface_base::sm_steamapi =	proc->m_steam_api.load_remote( );
	factory::c_interface_base::sm_tier0 =		proc->m_tier0.load_remote( );
	factory::c_interface_base::sm_vst =			proc->m_vst.load_remote( );
	factory::c_interface_base::sm_phonon3d =	proc->m_phonon3d.load_remote( );
	factory::c_interface_base::sm_netsock =		proc->m_netsock.load_remote( );

	game->m_chl( std::make_shared< c_module >( proc->m_client ), xorstr( "VClient" ) );
	game->m_entlist( std::make_shared< c_module >( proc->m_client ), xorstr( "VClientEntityList" ) );
	game->m_surface( std::make_shared< c_module >( proc->m_surface ), xorstr( "VGUI_Surface" ) );
	game->m_vgui( std::make_shared< c_module >( proc->m_vgui ), xorstr( "VGUI_Panel" ) );
	game->m_debug_overlay( std::make_shared< c_module >( proc->m_engine ), xorstr( "VDebugOverlay" ) );
	game->m_engine( std::make_shared< c_module >( proc->m_engine ), xorstr( "VEngineClient" ) );

	auto mem = proc->mem( );

	uintptr_t cdll_in_activate = game->m_chl.get_function( 15 );
	uintptr_t input = mem->get< uintptr_t >( cdll_in_activate + 0x1 );
	game->m_input( std::make_shared< c_module >( proc->m_client ), input );

	uintptr_t cdll_init = game->m_chl.get_function( 0 );
	uintptr_t globals = mem->get< uintptr_t >( mem->get< uintptr_t >( cdll_init + 0x1b ) );
	game->m_globals( std::make_shared< c_module >( proc->m_client ), globals );

	game->m_hwnd = FindWindowA( 0, xorstr( "Counter-Strike: Global Offensive" ) );

	return true;
}