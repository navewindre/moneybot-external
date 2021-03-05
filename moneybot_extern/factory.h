#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include <vector>
#include <functional>
#include <iterator>
#include <Psapi.h>
#include "util.h"
#include <string>
#include "nt.h"
#include <sstream>
#include <iomanip>

using namespace std::chrono_literals;
static std::chrono::high_resolution_clock g_timer;

class c_instance;
namespace csgo { class c_csgo; }

namespace process
{
	class c_process;
	namespace factory
	{
		class c_module {
		public:
			c_module( ) { };
			c_module( std::string module_name, std::shared_ptr< c_process > proc );
			void operator()( std::string module_name, std::shared_ptr< c_process > proc );

			uintptr_t get( ) const;
			uintptr_t operator()( ) const;
			size_t size( ) const;
			std::string name( ) const;
			std::string path( ) const;

			uintptr_t first_code_match( std::string sig );
			HMODULE load_remote( );
			auto get_parent( ) { return m_process; };

		protected:
			uintptr_t	m_base;
			size_t		m_size;
			std::string	m_image_name;
			std::string	m_image_path;
			std::shared_ptr< c_process > m_process;
		};


		class c_interface_base {
		public:
			using create_interface_fn = void*( *)( const char*, int* );
			c_interface_base( ) {}
			bool operator()( std::shared_ptr< c_module > mod, std::string interface_name );
			void operator()( std::shared_ptr< c_module > mod, uintptr_t addr );
			uintptr_t get_function( int index );
			uintptr_t get_function_ptr( int index );

			template < typename t1 = uintptr_t > t1 operator()( ) const {
				return m_ptr;
			}

			template < typename t > t* as( ) {
				return reinterpret_cast< t* >( m_base );
			}

			template < typename t > t get( ) {
				return g_proc->get< t >( m_ptr );
			}

			uintptr_t get_ptr( ) { return m_ptr; }

		public:
			static HMODULE
				sm_steamapi,
				sm_tier0,
				sm_vst,
				sm_phonon3d,
				sm_netsock;

		protected:
			std::shared_ptr< c_module >	m_module;
			uintptr_t	m_ptr{ };
			uintptr_t	m_base{ };
			HMODULE		m_module_handle{ };
		};


		extern bool commit( c_process*, c_instance*, csgo::c_csgo* );
	}

	class c_memory {
	public:
		c_memory( ) { };

		c_memory( std::shared_ptr< c_process > process );
		void operator()( std::shared_ptr< c_process > process );
		NTSTATUS write( uintptr_t address, void* value, size_t size );
		NTSTATUS get( uintptr_t address, void* buffer, size_t size );
		NTSTATUS free( uintptr_t address, size_t size, ulong_t type );
		NTSTATUS protect( uintptr_t address, size_t size, ulong_t new_prot, ulong_t* old_prot );
		NTSTATUS query( uintptr_t address, MEMORY_BASIC_INFORMATION* info, size_t len );
		NTSTATUS close( HANDLE h );
		uintptr_t allocate( size_t size, ulong_t type, ulong_t protect );
		HANDLE create_thread( uintptr_t fn, uintptr_t params );

		template < typename t > NTSTATUS write( uintptr_t address, t value ) { 
			return write( address, reinterpret_cast< void* >( &value ), sizeof( t ) ); 
		}

		template < typename t > t get( uintptr_t address ) {
			t temp{ };
			if ( get( address, &temp, sizeof( t ) ) != nt::STATUS_SUCCESS ) return t{ };
			return temp;
		}

	private:
		std::shared_ptr< c_process > m_process;
	};


	class c_process : public std::enable_shared_from_this< c_process > {
		friend class c_memory;
	public:
		c_process( );
		~c_process( ) { close_process( m_proc_handle ); }

		void init( std::string name );
		void get_module( factory::c_module* mod, std::string modulename );
		HANDLE operator()( ) const;
		HANDLE get_handle( ) const;
		int get_id( ) const;
		c_memory* operator->( );
		c_memory* mem( ) { return &m_mem; };

		explicit operator bool( ) const noexcept {
			return !!( m_proc_handle );
		}

		static NTSTATUS open_process( HANDLE* handle, uint32_t id );
		static NTSTATUS close_process( HANDLE );

	public:
		factory::c_module m_client;
		factory::c_module m_engine;
		factory::c_module m_steam_api;
		factory::c_module m_tier0;
		factory::c_module m_vst;
		factory::c_module m_phonon3d;
		factory::c_module m_surface;
		factory::c_module m_vgui;
		factory::c_module m_netsock;

	protected:
		int m_id;
		HANDLE m_proc_handle;
		c_memory m_mem;
		std::vector< factory::c_module* > m_module_list; //this will have a use someday
	};
}

extern process::c_process g_proc;