#include "engine.h"
#include "client.h"
#include "offsets.h"
#include <thread>

namespace csgo
{
	bool c_engine::match_found( ) {
		int match_found{ };
		int match_accepted{ };
		auto lobby_screen = g_proc->get< uintptr_t >( g_proc.m_client( ) + g_offsets.m_lobby_screen );

		if( lobby_screen ) {
			match_accepted	= g_proc->get< int >( lobby_screen + g_offsets.m_match_accepted );
			match_found		= g_proc->get< int >( lobby_screen + g_offsets.m_match_found );
		}
		
		return match_found && !match_accepted;
	}

	void c_engine::accept_match( ) {
		auto thread = g_proc->create_thread( g_offsets.m_accept_match, uintptr_t( 0 ) );
		WaitForSingleObject( thread, INFINITE );

		g_proc->close( thread );
	}

	void c_engine::force_attack( int v ) {
		g_proc->write< int >( g_proc.m_client( ) + g_offsets.m_forceattack, v );
	}

	void c_engine::force_attack( ) {
		force_attack( 4 );
		std::this_thread::sleep_for( 1ms );
		force_attack( 5 );
		std::this_thread::sleep_for( 10ms );
		force_attack( 4 );
	}

	void c_engine::force_jump( int v ) {
		auto status = g_proc->write< int >( g_proc.m_client( ) + g_offsets.m_forcejump, v );
	}

	void c_engine::force_jump( ) {
		force_jump( 6 );
	}

	void c_engine::set_sendpacket( bool v ) {
		ulong_t old_protect;
		g_proc->protect( g_proc.m_engine( ) + g_offsets.m_sendpacket, sizeof( byte ), PAGE_EXECUTE_READWRITE, &old_protect );
		g_proc->write< byte >( g_proc.m_engine( ) + g_offsets.m_sendpacket, v );
		g_proc->protect( g_proc.m_engine( ) + g_offsets.m_sendpacket, sizeof( byte ), old_protect, nullptr );
	}

	 bool c_engine::can_access_cmd( ) {
		auto& cl	= g_instance.m_engine.get_clientstate( );
		auto input	= g_csgo.m_input.get< input_t >( );

		int desired_cmd_number = cl.get_last_outgoing_cmd( );
		auto cmd = reinterpret_cast< uintptr_t >( input.m_commands ) + ( desired_cmd_number % 150 ) * sizeof( user_cmd_t );

		return !( g_proc->get< user_cmd_t >( cmd ).command_number < desired_cmd_number );
	}

	int c_engine::set_tickcount( int ticks ) {
		 set_sendpacket( false );

		 int old_tickbase = g_instance.m_local.m_nTickBase( );
		 while ( !can_access_cmd( ) ) {
			printf( "%d can't access usercmd!\n", ticks );
			 std::this_thread::yield( );
		 }

		 int tickbase_delta = g_instance.m_local.m_nTickBase( ) - old_tickbase;

		 auto input = g_csgo.m_input.get< input_t >( );
		 auto& cl	= get_clientstate( );

		 int desired_cmd_number = cl.get_last_outgoing_cmd( );
		 int old_cmd_number		= desired_cmd_number - 1;

		 uintptr_t old_cmd			= reinterpret_cast< uintptr_t >( input.m_commands ) + ( old_cmd_number % 150 ) * sizeof( user_cmd_t );
		 uintptr_t verified_old_cmd = reinterpret_cast< uintptr_t >( input.m_verified_commands ) + ( old_cmd_number % 150 ) * sizeof( verified_user_cmd_t );

		 user_cmd_t old_ucmd = g_proc->get< user_cmd_t >( old_cmd );
		 old_ucmd.tick_count = ticks;
		 g_proc->write< user_cmd_t >( old_cmd, old_ucmd );
		 g_proc->write< user_cmd_t >( verified_old_cmd, old_ucmd );
		 set_sendpacket( true );

		 return tickbase_delta;
	 }

	char* c_engine::get_game_dir( ) {
		static char path[ 256 ] = { 0 };
		g_proc->get( g_proc.m_engine( ) + g_offsets.m_game_directory, path, sizeof( path ) );

		return path;
	}

	c_clientstate c_engine::get_clientstate( ) {
		return c_clientstate( g_proc->get< uintptr_t >( g_proc.m_engine( ) + g_offsets.m_clientstate ) );
	}
	
	int c_clientstate::get_signon_state( ) {
		return get< int >( g_offsets.m_clientstate_signon );
	}

	template < typename t > t c_clientstate::get( uintptr_t addr ) {
		return g_proc->get< t >( m_address + addr );
	}

	template < typename t > void c_clientstate::write( uintptr_t addr, t val ) {
		g_proc->write< t >( m_address + addr, val );
	}

	int c_clientstate::get_localplayer( ) {
		return get< int >( g_offsets.m_clientstate_localplayer ) + 1;
	}

	vec3_t c_clientstate::get_viewangles( ) {
		return get< vec3_t >( g_offsets.m_clientstate_viewangles );
	}
	
	void c_clientstate::set_viewangles( vec3_t viewangle ) {
		write< vec3_t >( g_offsets.m_clientstate_viewangles, viewangle );
	}

	int c_clientstate::get_last_outgoing_cmd( ) {
		return get( ).m_lastoutgoingcommand;
	}

	float c_clientstate::get_latency( int flow ) {
		auto netchannel = get< uintptr_t >( 0x9c );
		netflow_t buf[ 2 ];
		g_proc->get( netchannel + 0x78b, buf, sizeof( netflow_t ) * 2 );
		return buf[ flow ].latency;
		//1931 * a2 + 372
	}

	char* c_clientstate::get_channel_name( ) {
		auto netchannel = get< uintptr_t >( 0x9c );
		return g_proc->get< char* >( netchannel + 0x4254 );
	}

	char* c_clientstate::get_level_name( ) {
		static char name[ 40 ];
		strcpy( name, get( ).m_level_name_short );
		return name;
	}

	void c_clientstate::force_update( ) {
		write< int >( 0x174, -1 );
	}
}