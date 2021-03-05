#pragma once
#include <windows.h>
#include <stdio.h>
#include "../shared/auth.h"

static const char* const SERVER_NAME = "www.navewindre.xyz";
static const char* const SERVER_LOGIN = "externalauth";
static const char* const SERVER_PASS = "test12";

bool file_exists( const std::string& name ) {
	struct stat buffer;
	return stat( name.c_str( ), &buffer ) == 0;
}

bool acquire_and_save_file( const std::string& path, const std::string& name ) {
	auto file = http::get_file( SERVER_NAME, name, SERVER_LOGIN, SERVER_PASS );
	if ( file.empty( ) ) {
		return false;
	}

	auto exe = fopen( path.c_str( ), "wb" );
	if ( !exe ) {
		printf( "error opening file directory\n" );
		return false;
	}
	fwrite( file.data( ), 1, file.size( ), exe );
	fclose( exe );

	return true;
}