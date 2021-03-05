#include "main.h"
#include "../shared/auth.h"
#include <intrin.h>
#include "drawing.h"

auth::c_auth	g_auth;
nt::c_nt		g_nt;
::c_syscalls	g_syscalls;

#ifdef DEBUG
int main( ) 
#else
int __stdcall WinMain( HINSTANCE inst, HINSTANCE prev, char* cmdline, int cmdshow = 0 )
#endif 
{
	shared::was_init = false;

	printf( "running authentication...\n" );
	if ( !g_auth.is_user_valid( ) ) {
		MessageBoxA( 0, "User unknown", "error", MB_OK );
		return 0;
	}

	shared::cur_username = g_auth.get_username( );

	while ( !game_check( ) ) {
		Sleep( 1 );
	}

	g_settings.load( );

	srand( static_cast< uint32_t >( time( nullptr ) ) );

	g_nt.dump_exports( );
	g_syscalls.init( );

	g_offsets.init( );
	g_netvars.init( );

	std::thread bsp_thread( [ & ]( ) {
		for ( ;; ) {
			g_bsp.check_cache( );
			std::this_thread::sleep_for( 10s );
		}
	} );

	bsp_thread.detach( );

	//g_drawing.hook_hudupdate( );

	printf( "%s", xorstr( "done!\n" ) );
	shared::was_init = true;

	for ( ;; ) {
		if ( g_instance.run_frame( ) ) {
			g_cheat.m_aim( );
			g_cheat.m_extra( );
		}

		//g_cheat.m_extra.autoaccept( );
		g_cheat.m_visuals( );

		//printf( "%d\n", g_settings.aim.aimkeytest( ) );
		//printf( "%d\n",g_instance.m_local.m_iCrosshairId( ) );
		//experimental -- thirdperson
		/*auto input = g_csgo.m_input( );
		g_proc->write< bool >( input + 0xa5, true );
		g_proc->write< float >( input + 0xa8 + 0x8, 150.f );
		g_instance.m_local.set< int >( 0x336c, 3 );*/


		std::this_thread::sleep_for( 1ms );
	}

	return 0;
}