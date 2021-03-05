#pragma once
#include "bsp_file.h"
#include <mutex>

namespace bsp
{
	class c_bsp_map {
	public:
		c_bsp_map( ) : m_data( nullptr ) { };
		~c_bsp_map( );

		bool load( const char* path, const char* mapName );
		void unload( );

		bool is_valid( );
		bool check_cache( );

		void		print_info( );

		dleaf_t*	get_leaf_from_point( const vec3_t& point );
		bool		is_visible( const vec3_t& start, const vec3_t& end, int max_len );

	private:
		char m_path[ 255 ];
		char m_map_name[ 128 ];

		size_t m_file_size;
	private:
		uint8_t*	m_data;
		dheader_t*	m_header;
		dnode_t*	m_node;
		dplane_t*	m_plane;
		dleaf_t*	m_leaf;

		std::mutex	m_lock;
	};
}

extern bsp::c_bsp_map g_bsp;