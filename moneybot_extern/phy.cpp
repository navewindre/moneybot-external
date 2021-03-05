#include "phy.h"
#include "factory.h"

namespace phy
{
	c_phy_file::~c_phy_file( ) {
		unload( );
	}

	void c_phy_file::unload( ) {
		delete[ ] m_data;
	}

	void c_phy_file::load( const char* path, const char* name ) {
		std::string full_path( path );
		full_path += '\\';
		full_path += name;

		auto file = CreateFileA( full_path.c_str( ), GENERIC_READ, 0, 0, OPEN_ALWAYS, 0, 0 );
		if ( !file ) return;

		size_t size = GetFileSize( file, 0 );
		if ( !size ) {
			CloseHandle( file );
			return;
		}

		m_data = new uint8_t[ size ];

		if ( !ReadFile( file, m_data, size, 0, 0 ) ) {
			CloseHandle( file );
			delete[ ] m_data;
			return;
		}

		CloseHandle( file );

		m_header = reinterpret_cast< phyheader_t* >( m_data );
		if ( !m_header->size ) {
			delete[ ] m_data;
			return;
		}

		m_surface_header = reinterpret_cast< decltype( m_surface_header ) >( m_data + sizeof( phyheader_t ) );
	}

	phyheader_t* c_phy_file::get_header( ) const {
		return m_header;
	}

	compactsurfaceheader_t* c_phy_file::get_surface_header( ) const {
		return m_surface_header;
	}

	legacysurfaceheader_t* c_phy_file::get_legacy_header( ) {
		return m_legacy_header;
	}

	trianglefaceheader_t* c_phy_file::get_triangle_headers( ) {
		return m_triangle_headers;
	}

	triangleface_t* c_phy_file::get_triangle_faces( ) {
		return m_triangle_faces;
	}
}