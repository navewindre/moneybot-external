#include "client.h"
#include "offsets.h"
#include "chl_client.h"
#include <thread>
#include <future>
#include <fstream>

csgo::c_netvars g_netvars;
csgo::c_offsets g_offsets;


namespace csgo
{
	void c_netvars::init( ) {
		auto chl = g_csgo.m_chl.as< csgo::i_chl_client >( );
		for ( auto client_class = chl->get_list( ); !!client_class; client_class = client_class->m_next ) {
			RecvTable* table = client_class->m_recv;
			m_tables.push_back( table );
		}
	}

	RecvTable* c_netvars::get_table( std::string table_name ) const {
		if ( m_tables.empty( ) ) return nullptr;

		for ( auto table : m_tables ) {
			if ( !table ) continue;

			if ( !table_name.compare( table->m_pNetTableName ) )
				return table;
		}

		return nullptr;
	}

	std::ptrdiff_t c_netvars::get_entry( std::string name, RecvTable* table ) const {
		std::ptrdiff_t ret{ };

		for ( int i{ }; i < table->m_nProps; ++i ) {
			RecvProp* prop = &table->m_pProps[ i ];
			RecvTable* child = prop->GetDataTable( );

			if ( child && ( child->m_nProps ) ) {
				auto tmp = get_entry( name, child );
				if ( tmp ) ret += prop->GetOffset( ) + tmp;
			}

			if ( name.compare( prop->m_pVarName ) )
				continue;

			return ( prop->GetOffset( ) + ret );
		}

		return ret;
	}

	std::ptrdiff_t c_netvars::get_netvar( std::string data, std::string name ) const {
		std::ptrdiff_t ret{ };
		auto table = get_table( data );

		ret = get_entry( name, table );
#ifdef DEBUG
		printf( "%s:\t\t 0x%05x\n", name.c_str( ), ret );
#endif
		return ret;
	}


	void c_offsets::init( ) {
		RUN_ASYNC( m_forcejump = g_proc.m_client.first_code_match( "89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 08" );
		m_forcejump = g_proc->get< uintptr_t >( m_forcejump + 0x2 ) - g_proc.m_client( ); );

		RUN_ASYNC( m_forceattack = g_proc.m_client.first_code_match( "89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 04" );
		m_forceattack = g_proc->get< uintptr_t >( m_forceattack + 0x2 ) - g_proc.m_client( ); );

		RUN_ASYNC( m_clientstate = g_proc.m_engine.first_code_match( "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0" );
		m_clientstate = g_proc->get< uintptr_t >( m_clientstate + 0x1 ) - g_proc.m_engine( ); );

		RUN_ASYNC( m_clientstate_localplayer = g_proc.m_engine.first_code_match( "8B 80 ? ? ? ? 40 C3" );
		m_clientstate_localplayer = g_proc->get< uintptr_t >( m_clientstate_localplayer + 0x2 ); );

		RUN_ASYNC( m_clientstate_viewangles = g_proc.m_engine.first_code_match( "F3 0F 11 80 ? ? ? ? D9 46 04 D9 05" );
		m_clientstate_viewangles = g_proc->get< uintptr_t >( m_clientstate_viewangles + 0x4 ); );

		RUN_ASYNC( m_clientstate_signon = g_proc.m_engine.first_code_match( "83 B8 ? ? ? ? ? 0F 94 C0 C3" );
		m_clientstate_signon = g_proc->get< uintptr_t >( m_clientstate_signon + 0x2 ); );

		RUN_ASYNC( m_entlist = g_proc.m_client.first_code_match( "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8" );
		m_entlist = g_proc->get< uintptr_t >( m_entlist + 0x1 ) - g_proc.m_client( ); );

		RUN_ASYNC( m_sendpacket = g_proc.m_engine.first_code_match( "B3 01 8B 01 8B 40 10 FF D0 84 C0 74 0F 80 BF ? ? ? ? ? 0F 84" );
		m_sendpacket -= g_proc.m_engine( ) - 0x1;
		m_sendpacket = 0xCAA6A; );

		RUN_ASYNC( m_glowobject_manager = g_proc.m_client.first_code_match( "A1 ? ? ? ? A8 01 75 4B" );
		m_glowobject_manager = g_proc->get< uintptr_t >( m_glowobject_manager + 0x1 );
		m_glowobject_manager -= g_proc.m_client( ) - 0x4; );

		RUN_ASYNC( m_lobby_screen = g_proc.m_client.first_code_match( "A1 ? ? ? ? 85 C0 74 0F 6A 00" );
		m_lobby_screen = g_proc->get< uintptr_t >( m_lobby_screen + 0x1 ) - g_proc.m_client( ); );

		RUN_ASYNC( m_accept_match = g_proc.m_client.first_code_match( "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 8B 8E ? ? ? ?" ); );

		RUN_ASYNC( m_match_accepted = g_proc.m_client.first_code_match( "89 B7 ? ? ? ? 8B 4F 04 85 C9" );
		m_match_accepted = g_proc->get< uintptr_t >( m_match_accepted + 0x2 ) - g_proc.m_client( ); );

		RUN_ASYNC( m_match_found = g_proc.m_client.first_code_match( "89 87 ? ? ? ? 8B 87 ? ? ? ? 3B F0" );
		m_match_found = g_proc->get< uintptr_t >( m_match_found + 0x2 ) - g_proc.m_client( ); );

		RUN_ASYNC( m_game_directory = g_proc.m_engine.first_code_match( "68 ? ? ? ? 8D 85 ? ? ? ? 50 68 ? ? ? ? 68" );
		m_game_directory = g_proc->get< uintptr_t >( m_game_directory + 0x1 ) - g_proc.m_engine( ); );

		RUN_ASYNC( m_input_addr = g_proc.m_engine.first_code_match( "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ); 
		m_input_addr = g_proc->get< uintptr_t >( m_input_addr + 0x1 ) - g_proc.m_client( ); );

		RUN_ASYNC( m_mouse_enabled = g_proc.m_client.first_code_match( "B9 ? ? ? ? FF 50 34 85 C0 75 10" );
		m_mouse_enabled = g_proc->get< uintptr_t >( m_mouse_enabled + 0x1 ) - g_proc.m_client( ); );
	}
}