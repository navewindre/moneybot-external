#include "factory.h"
#include "syscall.h"

namespace process
{
	c_memory::c_memory( std::shared_ptr< c_process > process ) {
		operator()( process );
	}

	void c_memory::operator()( std::shared_ptr< c_process > process ) {
		m_process = process;
	};

	NTSTATUS c_memory::write( uintptr_t address, void* value, size_t size ) {
		static auto nt_wvm = g_syscalls.get_syscall_func< nt::NTWriteVirtualMemory >( "NtWriteVirtualMemory" );
		return nt_wvm( m_process->get_handle( ), ( void* )address, value, size, nullptr );
	}

	NTSTATUS c_memory::get( uintptr_t address, void* buf, size_t size ) {
		static auto nt_rvm = g_syscalls.get_syscall_func< nt::NTReadVirtualMemory >( "NtReadVirtualMemory" );
		return nt_rvm( m_process->get_handle( ), reinterpret_cast< void* >( address ), buf, size, 0 );
	}

	uintptr_t c_memory::allocate( size_t size, ulong_t type, ulong_t protect ) {
		static auto nt_avm = g_syscalls.get_syscall_func< nt::NTAllocateVirtualMemory >( "NtAllocateVirtualMemory" );
		void*		alloc_address = nullptr;
		NTSTATUS	status;

		status = nt_avm( m_process->get_handle( ), &alloc_address, 0, &size, type, protect );
		if ( status != nt::STATUS_SUCCESS ) return uintptr_t{ };

		return reinterpret_cast< uintptr_t >( alloc_address );
	}

	NTSTATUS c_memory::free( uintptr_t address, size_t size, ulong_t type ) {
		static auto nt_fvm = g_syscalls.get_syscall_func< nt::NTFreeVirtualMemory >( "NtFreeVirtualMemory" );
		void* addr = reinterpret_cast< void* >( address );
		return nt_fvm( m_process->get_handle( ), &addr, &size, type );
	}

	NTSTATUS c_memory::protect( uintptr_t address, size_t size, ulong_t new_prot, ulong_t* old_prot ) {
		static auto nt_pvm = g_syscalls.get_syscall_func< nt::NTProtectVirtualMemory >( "NtProtectVirtualMemory" );
		void* addr = reinterpret_cast< void* >( address );
		return nt_pvm( m_process->get_handle( ), &addr, &size, new_prot, old_prot );
	}

	NTSTATUS c_memory::query( uintptr_t address, MEMORY_BASIC_INFORMATION* info, size_t len ) {
		static auto nt_qvm = g_syscalls.get_syscall_func< nt::NTQueryVirtualMemory >( "NtQueryVirtualMemory" );
		void* addr = reinterpret_cast< void* >( address );
		return nt_qvm( m_process->get_handle( ), addr, nt::MemoryBasicInformation, info, len, 0 );
	}

	HANDLE c_memory::create_thread( uintptr_t fn, uintptr_t params ) {
		HANDLE		ret;
		ACCESS_MASK access;
		LPVOID		parameter;
		int			suspended;
		ulong_t		stack_zero_bits = 0;
		ulong_t		stack_commit = 0;
		ulong_t		stack_reserve = 0;

		LPTHREAD_START_ROUTINE	start;
		OBJECT_ATTRIBUTES		attr;

		attr.Length = sizeof( OBJECT_ATTRIBUTES );
		attr.RootDirectory = 0;
		attr.Attributes = 0;
		attr.SecurityDescriptor = 0;
		attr.SecurityQualityOfService = 0;
		attr.ObjectName = 0;

		start = reinterpret_cast< decltype( start ) >( fn );
		access = 0xffff;
		parameter = reinterpret_cast< LPVOID >( params );
		suspended = false;

		static auto nt_cte = g_syscalls.get_syscall_func< nt::NTCreateThreadEx >( "NtCreateThreadEx" );
		nt_cte( &ret, access, &attr, m_process->get_handle( ), start, parameter, suspended, stack_zero_bits, stack_commit, stack_reserve, nullptr );

		return ret;
	}

	NTSTATUS c_memory::close( HANDLE h ) {
		return util::nt_close( h );
	}
}