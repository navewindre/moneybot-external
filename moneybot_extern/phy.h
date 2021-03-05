#pragma once
#include "vector.h"

namespace phy
{
	struct phyheader_t {
		int size;
		int id;
		int solid_count;
		int checksum;
	};

	struct compactsurfaceheader_t {
		int size;
		char vphysics_id[ 4 ];
		short version;
		short model_type;
		int surface_size;
		vec3_t drag_axis_areas;
		//int   axis_map_size;
	};

	struct legacysurfaceheader_t {
		int size;
		vec3_t mass_center;
		vec3_t rotation_inertia;
		float upper_limit_radius;
		int max_deviation : 8;
		int byte_size : 24;
		int offset_ledgetree_root;
		int dummy[ 6 ];
		int count_tri_faces;
	};

	struct trianglefaceheader_t {
		int m_offset_to_vertices;
		int dummy[ 2 ];
		int m_count_faces;
	};

	struct triangleface_t {
		uint8_t id;
		uint8_t _dummy[ 3 ];
		uint8_t v1;
		uint8_t _dummy2[ 3 ];
		uint8_t v2;
		uint8_t _dummy3[ 3 ];
		uint8_t v3;
		uint8_t _dummy4[ 3 ];
	};

	class c_phy_file {
	public:
		c_phy_file( ) { }
		~c_phy_file( );

	public:
		void load( const char* path, const char* file );
		void unload( );

	public:
		compactsurfaceheader_t* get_surface_header( ) const;
		legacysurfaceheader_t*	get_legacy_header( );
		trianglefaceheader_t*	get_triangle_headers( );
		triangleface_t*			get_triangle_faces( );
		phyheader_t*			get_header( ) const;

	private:
		uint8_t* m_data;
		phyheader_t* m_header;
		compactsurfaceheader_t* m_surface_header;
		legacysurfaceheader_t*	m_legacy_header;
		trianglefaceheader_t*	m_triangle_headers;
		triangleface_t*			m_triangle_faces;

	private:
		size_t m_num_triangle_faces;
	};
}