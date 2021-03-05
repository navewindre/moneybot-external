#pragma once
#include <inttypes.h>
#include <Windows.h>
#include <functional>
#include <thread>
#include "sdk.h"
#include "cx_strenc.h"

#define function_printf(x) printf( "->in function %s : \n%s\n", __FUNCSIG__, x )
#define RUN_ASYNC( x ) \
std::thread( \
	[ & ]{ x } \
).detach( ); \
std::this_thread::sleep_for( 10ms ); \

#define TICK_INTERVAL g_csgo.m_globals.get< globalvars_t >( ).m_interval_per_tick
#define TIME_TO_TICKS( x ) static_cast< int >( 0.5f + x / TICK_INTERVAL )
#define TICKS_TO_TIME( x ) ( TICK_INTERVAL * float( x ) )

namespace util
{
	extern float get_lerp( );

	__forceinline void set( void* address, uint8_t value, size_t size ) {
		memset( address, value, size );
	}

	__forceinline void copy( void* address, void* value, size_t size ) {
		memcpy( address, value, size );
	}

	__forceinline bool key_down( int key ) {
		return GetAsyncKeyState( key ) & 0x8000;
	}

	template < typename fn > __forceinline fn call_vfunc( void* base, int index ) {
		if ( !base ) return fn{ };
		return reinterpret_cast< fn >( ( ( uintptr_t* )*( uintptr_t** )base )[ index ] );
	}

	__forceinline bool file_exists( const std::string& path ) {
		struct stat buf;
		return stat( path.c_str( ), &buf ) == 0;
	}

	extern NTSTATUS nt_close( HANDLE h );

	/*__forceinline void bsod( ) {
		static auto nt_rhe = g_syscalls.get_syscall_func< nt::NTRaiseHardError >( xorstr( "NTRaiseHardError" ) );
		nt_rhe( 0, 0, nullptr, 0, nullptr );
	}*/ //cus fuck u lol
}