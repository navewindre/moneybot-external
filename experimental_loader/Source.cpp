#include <fstream>
#include "get.h"

int main( ) {
	auth::c_auth auth;
	if ( !auth.is_user_valid( ) ) {
		MessageBoxA( 0, "user unknown", "unauthorized", MB_OK );
		return 0;
	}

	printf( "welcome, %s\n", auth.get_username( ).c_str( ) );
	printf( "checking for updates...\n" );
	std::this_thread::sleep_for( 300ms );

	std::string appdata = getenv( "APPDATA" );
	appdata.append( "\\coffee_external\\" );

	CreateDirectoryA( appdata.c_str( ), 0 );

	std::string exe_path = appdata + "external.exe";
	std::string dll_path = appdata + "menu.dll";
	std::string sciter_path = appdata + "sciter.dll";

	if ( !file_exists( sciter_path ) ) {
		if ( !acquire_and_save_file( sciter_path, "/auth/sciter" ) ) {
			MessageBoxA( 0, "failed to acquire library", "error", MB_OK );
			return 0;
		}
		printf( "acquired library...\n" );
	}
	else {
		printf( "library is up to date...\n" );
	}

	if ( !acquire_and_save_file( exe_path, "/auth/external" ) ) {
		MessageBoxA( 0, "failed to acquire menu", "error", MB_OK );
		return 0;
	}
	printf( "acquired base...\n" );
	if ( !acquire_and_save_file( dll_path, "/auth/menu" ) ) {
		MessageBoxA( 0, "failed to acquire base", "error", MB_OK );
		return 0;
	}
	printf( "acquired menu...\n" );

	STARTUPINFOA si{ };
	PROCESS_INFORMATION pi{ };
	si.cb = sizeof( si );

	if ( !CreateProcessA( exe_path.c_str( ), NULL, NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi ) ) {
		MessageBoxA( 0, "couldn't create process", "error", MB_OK );
		return 0;
	}

	CloseHandle( pi.hThread );
	CloseHandle( pi.hProcess );

	std::this_thread::sleep_for( 200ms );
	printf( "done\n" );
}