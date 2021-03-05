#pragma once
#include <string>

#define NETVAR( funcname, name, table, extra, type ) \
__forceinline auto funcname( ) { \
	static std::ptrdiff_t offset = g_netvars.get_netvar( table, name ) + extra; \
	return get< type >( offset ); \
}; \
\
__forceinline auto funcname( type v ) { \
	static std::ptrdiff_t offset = g_netvars.get_netvar( table, name ) + extra; \
	set< type >( offset, v ); \
}; \

#define OFFSET( name, offset, type ) \
__forceinline type name( ) { \
	return get< type >( offset ); \
} \
__forceinline void name( type v ) { \
	set< type >( offset, v ); \
} \

class RecvTable;

namespace csgo
{
	class c_netvars {
	public:
		void init( );
		std::ptrdiff_t get_netvar( std::string table, std::string name ) const; //search through the netvar list
	private:
		std::ptrdiff_t get_entry( std::string name, RecvTable* table ) const;
		RecvTable* get_table( std::string table_name ) const;
	private:
		std::vector< RecvTable* > m_tables;
		//should we store all the vars in here or use the offsets class?
		//same
	};

	class c_offsets {
	public:
		void init( );

	public:
		std::ptrdiff_t m_forcejump;
		std::ptrdiff_t m_forceattack;
		std::ptrdiff_t m_clientstate;
		std::ptrdiff_t m_clientstate_localplayer;
		std::ptrdiff_t m_clientstate_viewangles;
		std::ptrdiff_t m_clientstate_signon;
		std::ptrdiff_t m_entlist;
		std::ptrdiff_t m_sendpacket;
		std::ptrdiff_t m_glowobject_manager;
		std::ptrdiff_t m_lobby_screen;
		std::ptrdiff_t m_accept_match;
		std::ptrdiff_t m_match_accepted;
		std::ptrdiff_t m_match_found;
		std::ptrdiff_t m_game_directory;
		std::ptrdiff_t m_input_addr;
		std::ptrdiff_t m_mouse_enabled;
	};
}

extern csgo::c_netvars g_netvars;
extern csgo::c_offsets g_offsets;