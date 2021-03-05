#pragma once
#include "factory.h"

namespace util {
	class i_hook {
	public:
		virtual void restore( ) = 0;
		virtual void register_process( std::shared_ptr< decltype( g_proc ) > proc ) = 0;
	};
	class c_hook;
	class c_hook_manager;
}

extern util::c_hook_manager g_hooks;

namespace util
{
	class c_hook_manager {
	protected:
		std::shared_ptr< process::c_process > m_process;
		std::vector< i_hook* > m_hooks;

	public:
		c_hook_manager( decltype( m_process ) process ) :
			m_process( process ) { }

		~c_hook_manager( ) {
			for ( auto& it : m_hooks ) {
				it->restore( );
			}
		}

		void register_hook( i_hook* hook ) {
			hook->register_process( m_process );
			m_hooks.push_back( hook );
		}
	};

	class c_hook : public i_hook {
		friend class c_hook_manager;
	protected:
		std::shared_ptr< process::c_process > m_process;

	private:
		uintptr_t m_original;
		uintptr_t m_func;
		uintptr_t m_address;

	public:
		c_hook( ) { }

		c_hook( process::factory::c_interface_base* iface, int function ) {
			g_hooks.register_hook( this );
			m_func = iface->get_function( function );
			m_original = iface->get_function( function );
		}

		~c_hook( ) {
			restore( );
		}

		void register_process( decltype( m_process ) proc ) {
			m_process = proc;
		}

		void hook( uintptr_t hook_addr ) {
			static auto mem = m_process->mem( );

			ulong_t old_protect;
			mem->protect( m_func, sizeof( uintptr_t ), PAGE_EXECUTE_READWRITE, &old_protect );
			mem->write< uintptr_t >( m_func, m_address );
			mem->protect( m_func, sizeof( uintptr_t ), old_protect, nullptr );
		}

		void restore( ) {
			static auto mem = m_process->mem( );

			ulong_t old_protect;
			mem->protect( m_func, sizeof( uintptr_t ), PAGE_EXECUTE_READWRITE, &old_protect );
			mem->write< uintptr_t >( m_func, m_original );
			mem->protect( m_func, sizeof( uintptr_t ), old_protect, nullptr );
		}

		uintptr_t get_original( ) {
			return m_original;
		}
	};
}