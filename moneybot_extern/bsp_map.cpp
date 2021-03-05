#include "factory.h"
#include "bsp_map.h"
#include "client.h"

bsp::c_bsp_map g_bsp;

namespace bsp
{
	c_bsp_map::~c_bsp_map( ) {
		unload( );
	}

	bool c_bsp_map::is_valid( ) {
		if ( !*m_path || !*m_map_name ) return false;
		
		return m_header && m_plane && m_node && m_leaf;
	}

	void c_bsp_map::unload( ) {
		if ( m_data ) {
			delete[ ] m_data;
			m_data = nullptr;
		}

		*m_path = '\0';
		*m_map_name = '\0';

		m_header = nullptr;
		m_plane = nullptr;
		m_node = nullptr;
		m_leaf = nullptr;
	}

	bool c_bsp_map::load( const char* path, const char* name ) {
		strcpy( m_path, path );
		strcpy( m_map_name, name );

		std::string full_path( m_path );
		full_path += "\\maps\\";
		full_path += m_map_name;
		full_path += ".bsp";

		if ( !util::file_exists( full_path ) ) {
			return false;
		}

		auto file = CreateFileA( full_path.c_str( ), GENERIC_READ, 0, 0, OPEN_ALWAYS, 0, 0 );
		if ( !file ) return false;

		m_file_size = GetFileSize( file, 0 );
		if ( !m_file_size ) {
			CloseHandle( file );
			return false;
		}

		m_data = new uint8_t[ m_file_size ];

		if ( !ReadFile( file, m_data, m_file_size, nullptr, 0 ) ) {
			CloseHandle( file );
			delete[ ] m_data;
			return false;
		}

		CloseHandle( file );

		m_header = reinterpret_cast< dheader_t* >( m_data );
		
		m_node = reinterpret_cast< dnode_t* >( m_data + m_header->lumps[ LUMP_NODES ].fileofs );
		m_plane = reinterpret_cast< dplane_t* >( m_data + m_header->lumps[ LUMP_PLANES ].fileofs );
		m_leaf = reinterpret_cast< dleaf_t* >( m_data + m_header->lumps[ LUMP_LEAVES ].fileofs );
		
		return true;
	}

	void c_bsp_map::print_info( ) {
		printf( "------BSP INFO------\n" );
		printf( "path: %s\n", m_path );
		printf( "name: %s\n", m_map_name );
		if ( m_header ) {
			printf( "version: %d\n", m_header->version );
			printf( "size: %d\n", m_file_size );
			printf( "revision: %d\n", m_header->mapRevision );
		}
		else {
			printf( "header is null\n" );
		}
		printf( "\n\n" );
	}

	dleaf_t* c_bsp_map::get_leaf_from_point( const vec3_t& point ) {
		int			node_num{ };
		dnode_t*	node{ };
		dplane_t*	plane{ };
		float		dot{ };

		if ( !m_node || !m_plane ) {
			return nullptr;
		}
		
		for ( node_num = 0; node_num >= 0; ) {
			node = &m_node[ node_num ];
			plane = &m_plane[ node->planenum ];
			
			dot = point.dot( plane->normal ) - plane->dist;
			if ( dot > 0 ) {
				node_num = node->children[ 0 ];
			}
			else {
				node_num = node->children[ 1 ];
			}
		}

		return &m_leaf[ -node_num - 1 ];
	}

	bool c_bsp_map::is_visible( const vec3_t& start, const vec3_t& end, int max_len ) {
		std::unique_lock< std::mutex >( m_lock );
		if ( !is_valid( ) ) {
			return false;
		}

		vec3_t		direction = end - start;
		vec3_t		point = start;
		dleaf_t*	leaf{ };
		int			steps = int( direction.length( ) );

		if ( steps > max_len ) {
			return false;
		}

		direction /= float( steps );

		for ( ; steps > 0; steps-- ) {
			point += direction;
			leaf = get_leaf_from_point( point );

			if ( leaf->contents & CONTENTS_SOLID )
				return false;
		}

		return true;
	}

	bool c_bsp_map::check_cache( ) {
		std::unique_lock< std::mutex >( m_lock );
		if ( !g_offsets.m_game_directory ) 
			return false;

		auto cl = g_instance.m_engine.get_clientstate( );
		if ( cl.get_signon_state( ) != 6 ) {
			unload( );
			return false;
		}

		auto game_dir = g_instance.m_engine.get_game_dir( );
		auto level_name = cl.get_level_name( );

		if ( ( *level_name && !is_valid( ) ) || strcmp( level_name, m_map_name ) ) {
			unload( );
			load( game_dir, cl.get_level_name( ) );
			print_info( );
		}

		return is_valid( );
	}
}