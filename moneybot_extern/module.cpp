#include "factory.h"

namespace process
{
	namespace factory
	{
		c_module::c_module( std::string module_name, std::shared_ptr< c_process > proc ) {
			operator()( module_name, std::move( proc ) );
		}

		uintptr_t c_module::get( ) const {
			return m_base;
		}

		uintptr_t c_module::operator()( ) const {
			return m_base;
		}

		size_t c_module::size( ) const {
			return m_size;
		}

		std::string c_module::name( ) const {
			return m_image_name;
		}

		std::string c_module::path( ) const {
			return m_image_path;
		}

		void c_module::operator()( std::string module_name, std::shared_ptr< c_process > proc ) {
			m_process = std::move( proc );

			HANDLE			t32_snapshot;
			MODULEENTRY32	mod_entry;

			t32_snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, m_process->get_id( ) );
			mod_entry.dwSize = sizeof( mod_entry );

			char path[ MAX_PATH ];

			for ( Module32First( t32_snapshot, &mod_entry );
				Module32Next( t32_snapshot, &mod_entry );
				) {
				if ( !module_name.compare( mod_entry.szModule ) ) {
					CloseHandle( t32_snapshot );

					m_base = reinterpret_cast< uintptr_t >( mod_entry.modBaseAddr );
					m_size = static_cast< size_t >( mod_entry.modBaseSize );

					HANDLE proc = m_process->get_handle( );
					GetModuleFileNameEx( proc, mod_entry.hModule, path, MAX_PATH );

					m_image_path = path;
					m_image_name = mod_entry.szModule;
					break;
				}
			}
		}

		uintptr_t c_module::first_code_match( std::string sig ) {
			auto bin_match = []( uint8_t* code, uint8_t* pattern, size_t size ) -> bool {
				for ( size_t i{ }; i < size; ++i ) {
					if ( pattern[ i ] && code[ i ] != pattern[ i ] ) {
						return false;
					}
				}
				return true;
			};

			std::istringstream iss( sig );
			std::vector< std::string > tokens{ std::istream_iterator< std::string >{ iss }, std::istream_iterator< std::string >{ } };
			std::vector< uint8_t > sig_bytes;

			for ( auto hex_byte : tokens ) {
				sig_bytes.push_back( static_cast< uint8_t >( std::strtoul( hex_byte.c_str( ), nullptr, 16 ) ) );
			}

			if ( sig_bytes.size( ) < 2 ) return 0;

			uint8_t* pattern_bytes = sig_bytes.data( );
			MEMORY_BASIC_INFORMATION mbi{ 0 };

			for ( size_t offset{ }; offset < m_size; offset += mbi.RegionSize ) {
				m_process->mem( )->query( m_base + offset, &mbi, sizeof( MEMORY_BASIC_INFORMATION ) );
				if ( mbi.State != MEM_FREE ) {
					uint8_t* buffer = new uint8_t[ mbi.RegionSize ];
					m_process.get( )->mem( )->get( uintptr_t( mbi.BaseAddress ), buffer, mbi.RegionSize );
					for ( size_t i{ }; i < mbi.RegionSize; ++i ) {
						if ( bin_match( buffer + i, pattern_bytes, sig_bytes.size( ) ) ) {
							return ( uintptr_t )mbi.BaseAddress + i;
						}
					}
				}
				offset += mbi.RegionSize;
			}
			return 0;
		}

		HMODULE c_module::load_remote( ) {
			auto module_ = GetModuleHandleA( m_image_name.c_str( ) );
			if ( module_ ) {
				return module_;
			}

			module_ = LoadLibraryExA( m_image_path.c_str( ), nullptr, 0x00000010 );
			if ( !module_ ) {
				printf( "Unable to load module %s\n Path: %s\nresult: %08x\ncode: %x\n\n",
					m_image_name.c_str( ), m_image_path.c_str( ), module_, GetLastError( ) );
				return 0;
			}

			printf( "loaded %s\n", m_image_name.c_str( ) );
			return module_;
		}
	}
}