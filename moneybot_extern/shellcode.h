#pragma once
#include "factory.h"

namespace util {
	class c_shellcode {
	public:
		c_shellcode( std::shared_ptr< process::c_process > proc ) :
			m_process( proc ) { }

		size_t push( std::vector< uint8_t > vector ) {
			m_code.insert( std::end( m_code ), std::begin( vector ), std::end( vector ) );
			return m_code.size( );
		}

		size_t push( uint8_t value ) {
			m_code.push_back( value );
			return m_code.size( );
		}

		template < typename T = uintptr_t > size_t push( T value ) {
			m_code.insert( m_code.end( ), ( uint8_t* )&value, ( uint8_t * )&value + sizeof( T ) );
			return m_code.size( );
		}

		std::vector< uint8_t > code( ) {
			return m_code;
		}

		void bootstrap( std::string name, uintptr_t value ) {
			auto hash = std::hash< std::string >{ }( name );

			for ( size_t i = 0; i + sizeof( value ) <= m_code.size( ); ++i ) {
				auto data = ( uintptr_t * )( ( uintptr_t )m_code.data( ) + i );

				if ( *data == hash ) {
					*data = value;
				}
			}
		}

		void bootstrap( std::vector< std::pair< std::string, uintptr_t > > data ) {
			for ( auto item : data ) {
				bootstrap( item.first, item.second );
			}
		}

		uintptr_t allocate( ) {
			auto allocation = m_addr = m_process->mem( )->allocate( m_code.size( ), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
			m_process->mem( )->write( allocation, m_code.data( ), m_code.size( ) );

			return allocation;
		}

		void deallocate( ) {
			m_process->mem( )->free( m_addr, m_code.size( ), MEM_RELEASE );
		}

	protected:
		uintptr_t				m_addr{ };
		std::vector< uint8_t >  m_code;
		std::shared_ptr< process::c_process > m_process;
	};
}