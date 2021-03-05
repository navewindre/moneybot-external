#include "factory.h"

namespace process
{
	namespace factory
	{
		bool c_interface_base::operator()( std::shared_ptr< c_module > mod, std::string interface_name ) {
			m_module = mod;
			m_module_handle = m_module->load_remote( );
			if ( !m_module_handle ) {
				return false;
			}

			auto interface_factory = reinterpret_cast< create_interface_fn >( GetProcAddress( m_module_handle, "CreateInterface" ) );
			if ( !interface_factory ) {
				printf( "couldnt find createinterface %s in %s",
					interface_name.c_str( ),
					mod->name( ).c_str( ) );
			}

			int interface_num = 0;

			//ultragay
			//should implement dynamic scanning for modules and seeing if they export CreateInterface
			//but im a lazy fucker so this will do lmfao
			auto grab_interface_dynamic = [ & ]( std::string name, int version ) {
				char* buf = ( char* )name.c_str( );
				sprintf( buf, "%s%03d", name.c_str( ), version );
				return interface_factory( buf, nullptr );
			};

			for ( m_ptr = 0; !m_ptr && interface_num < 999; interface_num++ ) {
				m_ptr = reinterpret_cast< uintptr_t >( grab_interface_dynamic( interface_name.c_str( ), interface_num ) );
				if ( m_ptr ) {
					m_base = m_ptr;
					m_ptr -= reinterpret_cast< size_t >( m_module_handle );
					m_ptr += m_module->get( );
					break;
				}
			}

			if ( !m_ptr ) { printf( "%s not found\n", interface_name.c_str( ) ); }

			std::cout << interface_name << std::setfill( '0' ) << std::setw( 3 ) << std::dec << interface_num << std::setfill( ' ' )
				<< std::setw( 31 - interface_name.length( ) ) << "0x" << std::hex << m_ptr << '\n';

			return true;
		}

		void c_interface_base::operator()( std::shared_ptr< c_module > mod, uintptr_t addr ) {
			m_module		= mod;
			m_module_handle = mod->load_remote( );

			m_ptr	= addr;
			m_base	= NULL;
		}

		uintptr_t c_interface_base::get_function( int index ) {
			static auto mem = m_module->get_parent( )->mem( );
			uintptr_t vtable = mem->get< uintptr_t >( m_ptr );
			return mem->get< uintptr_t >( vtable + ( index * sizeof( uintptr_t ) ) );
		}

		uintptr_t c_interface_base::get_function_ptr( int index ) {
			static auto mem = m_module->get_parent( )->mem( );
			uintptr_t vtable = mem->get< uintptr_t >( m_ptr );
			return vtable + sizeof( uintptr_t ) * index;
		}
	}
}