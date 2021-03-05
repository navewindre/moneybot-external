#include "util.h"
#include "factory.h"
#include "syscall.h"
#include "csgo.h"

namespace util
{
	float get_lerp( ) {
		return TICKS_TO_TIME( 2 );
	}

	NTSTATUS nt_close( HANDLE h ) {
		static auto nt_c = g_syscalls.get_syscall_func< nt::NTClose >( "NtClose" );

		return nt_c( h );
	}
}