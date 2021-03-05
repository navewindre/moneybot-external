#pragma once
#include <thread>
#include <vector>
#include "../shared/console.h"
#include "client.h"
#include "cheat.h"
#include "offsets.h"
#include "sdk.h"
#include "nt.h"
#include "syscall.h"
#include "shellcode.h"
#include "settings.h"
#include "backtracking.h"
#include "../menu/shared.h"
#include "weapon.h"
#include "bsp_map.h"

bool game_check( ) {
	std::string console_title = xorstr( "CSGO External --- " ) + std::string( __TIME__ ) + xorstr( " " ) + std::string( __DATE__ );
	SetConsoleTitleA( console_title.c_str( ) );

	con::clear_console( );
	con::set_console_color( con::concol::aqua, con::concol::black );

	auto retry = [ & ]( const char* msg ) {
		con::clear_console( );
		con::set_cursor_pos( 30, 15 );

		std::cout << msg;
		con::set_cursor_pos( 30, 16 );
		std::cout << xorstr( "retry? (y/n)" );
		auto c = getchar( );

		return c == 'y';
	};

	if ( !process::factory::commit( &::g_proc, &::g_instance, &::g_csgo ) ) {
#ifndef DEBUG
		MessageBoxA( 0, xorstr( "csgo not running" ), xorstr( "game not found" ), MB_OK );
		abort( );
#else
		printf( "%s", xorstr( "failed to init\n" ) );
		bool found{ };
		for ( ;; ) {
			if ( retry( xorstr( "csgo not found" ) ) ) {
				found = process::factory::commit( &::g_proc, &::g_instance, &::g_csgo );
			}
			else break;
		}

		if ( !found ) {
			abort( );
		}
#endif
	}

	return true;
}