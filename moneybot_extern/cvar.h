#pragma once
#include "factory.h"
namespace csgo
{
	class c_convar {
	public:
		c_convar( uintptr_t addr ) : m_address( addr ) { };

		float get_float( ) {
			uintptr_t xored_val = g_proc->get< uintptr_t >( m_address + 44 );
			xored_val ^= m_address;
			return *reinterpret_cast< float* >( &xored_val );
		}

		int get_int( ) {
			uintptr_t xored_val = g_proc->get< uintptr_t >( m_address + 48 );
			xored_val ^= m_address;
			return *reinterpret_cast< float* >( &xored_val );
		}

	private:
		uintptr_t m_address;
	};

	class c_cvar : public process::factory::c_interface_base {
	public:
		void init( ) {

		}

		uintptr_t find_var( std::string name ) {
			auto hash = decrypt_cvar( name );

			uintptr_t base = g_proc->get< uintptr_t >( m_thisptr + 0x34 );
			uintptr_t ptr = g_proc->get< uintptr_t >( base + ( uint8_t )hash * 4 );

			while ( ptr ) {
				if ( g_proc->get< int >( ptr ) == hash ) {
					uintptr_t cvar_ptr = g_proc->get< uintptr_t >( ptr + 0x4 );
					char buf[ 10 ];
					if ( g_proc->get( cvar_ptr + 0xc, buf, sizeof( buf ) ) == *name.c_str( ) ) {
						return cvar_ptr;
					}
				}

				ptr = g_proc->get< uintptr_t >( m_ptr + 0xc );
			}

			return 0;
		}

	private:
		int decrypt_cvar( std::string name ) {
			int v2{ };
			int v3{ };

			for ( int i = 0; i < name.length( ); i += 2 ) {
				v3 = m_char_code[ v2 ^ toupper( name[ i ] ) ];
				if ( i + 1 == name.length( ) )
					break;
				v2 = m_char_code[ v3 ^ toupper( name[ i ] + 1 ) ];
			}

			return v2 | ( v3 << 8 );
		}

	private:
		int m_char_code[ 255 ];
		uintptr_t m_base;
		uintptr_t m_thisptr;
	};
}