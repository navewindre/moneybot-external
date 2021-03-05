#pragma once
#include <inttypes.h>
#include "vector.h"
#include "sdk.h"

namespace csgo
{
	class c_clientstate {
	public:
		c_clientstate( uintptr_t addr ) : m_address( addr ) {};
		c_clientstate( ) : m_address( ) {};

		int		get_localplayer( );
		int		get_last_outgoing_cmd( );
		
		vec3_t	get_viewangles( );
		void	set_viewangles( vec3_t viewangle );
		float	get_latency( int flow );
		char*	get_channel_name( );
		void	force_update( );
		int		get_signon_state( );
		char*	get_level_name( );
		

	protected:
		template < typename t = uintptr_t > t get( uintptr_t address );
		template < typename t = uintptr_t > void write( uintptr_t addr, t val );

		client_state_t get( ) { return get< client_state_t >( 0 ); }

		uintptr_t m_address;
	};

	class c_engine {
	public:
		bool match_found( );
		void accept_match( );
		void force_attack( int );
		void force_attack( );
		void force_jump( int );
		void force_jump( );
		c_clientstate get_clientstate( );
		void set_sendpacket( bool );
		bool can_access_cmd( );
		int set_tickcount( int );
		char* get_game_dir( );
	};
}