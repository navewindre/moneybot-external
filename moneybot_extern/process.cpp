#include "factory.h"
#include "syscall.h"

namespace process
{
	c_process::c_process( ) : m_mem( std::shared_ptr< c_process >( &g_proc ) ) { };

	NTSTATUS c_process::open_process( HANDLE* handle, uint32_t id ) {
		OBJECT_ATTRIBUTES attr;
		nt::CLIENT_ID cid;

		cid.UniqueProcess = ULongToHandle( id );
		cid.UniqueThread = nullptr;

		attr.Length = sizeof( OBJECT_ATTRIBUTES );
		attr.RootDirectory = 0;
		attr.Attributes = 0;
		attr.SecurityDescriptor = 0;
		attr.SecurityQualityOfService = 0;
		attr.ObjectName = 0;
		
		static auto nt_op = g_syscalls.get_syscall_func< nt::NTOpenProcess >( xorstr( "NtOpenProcess" ) );
		return nt_op( handle, PROCESS_ALL_ACCESS, &attr, &cid );
	}

	NTSTATUS c_process::close_process( HANDLE handle ) {
		static auto nt_c = g_syscalls.get_syscall_func< nt::NTClose >( xorstr( "NtClose" ) );
		return nt_c( handle );
	}

	void c_process::init( std::string name ) {
		HANDLE			t32_snapshot;
		PROCESSENTRY32	proc_entry;

		t32_snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		proc_entry.dwSize = sizeof( proc_entry );
		
		for ( Process32First( t32_snapshot, &proc_entry );
			Process32Next( t32_snapshot, &proc_entry );
			) {
			if ( !std::string( proc_entry.szExeFile ).compare( name ) ) {
				m_id = proc_entry.th32ProcessID;
				CloseHandle( t32_snapshot );

				open_process( &m_proc_handle, m_id );
			}
		}

		printf( "%s%s%s", xorstr( "PROCESS [ " ), name.c_str( ), xorstr( " ]" ) );
		printf( "\n\t%s%d", xorstr( "->ID: " ), m_id );
		printf( "\n\t%s0x%08x\n", xorstr( "->Handle: " ), uintptr_t( m_proc_handle ) );
		printf( xorstr( "\n--------------------------\n" ) );
	}

	void c_process::get_module( factory::c_module* mod, std::string modulename ) {
		mod->operator()( modulename, shared_from_this( ) );
		m_module_list.push_back( mod );
	}

	HANDLE c_process::get_handle( ) const {
		return m_proc_handle;
	}

	HANDLE c_process::operator()( ) const {
		return m_proc_handle;
	}

	int c_process::get_id( ) const {
		return m_id;
	}
	
	c_memory* c_process::operator->( ) {
		return &m_mem;
	}
}