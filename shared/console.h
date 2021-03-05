#pragma once
#include <Windows.h>

#define STD_HANDLE GetStdHandle( STD_OUTPUT_HANDLE )

namespace con
{
	enum class concol : int {
		black = 0,
		dark_blue = 1,
		dark_green = 2,
		dark_aqua, dark_cyan = 3,
		dark_red = 4,
		dark_purple = 5, dark_pink = 5, dark_magenta = 5,
		dark_yellow = 6,
		dark_white = 7,
		gray = 8,
		blue = 9,
		green = 10,
		aqua = 11, cyan = 11,
		red = 12,
		purple = 13, pink = 13, magenta = 13,
		yellow = 14,
		white = 15
	};

	inline void set_console_cursor( bool cursor ) {
		CONSOLE_CURSOR_INFO cursor_info;
		cursor_info.bVisible = cursor;
		cursor_info.dwSize = 1;
		SetConsoleCursorInfo( STD_HANDLE, &cursor_info );
	}

	inline void set_cursor_pos( int x, int y ) {
		COORD cursor_pos = { static_cast< short >( x ), static_cast< short >( y ) };
		SetConsoleCursorPosition( STD_HANDLE, cursor_pos );
	}

	inline void set_console_color( concol text, concol bg ) {
		SetConsoleTextAttribute( STD_HANDLE, ( ( int )text + ( ( int )bg * 16 ) ) );
	}

	inline void clear_console( ) {
		CONSOLE_SCREEN_BUFFER_INFO buf_info;
		HANDLE std_out;
		DWORD count;
		DWORD cell_count;
		COORD null_coord{ };

		std_out = STD_HANDLE;
		if ( std_out == INVALID_HANDLE_VALUE ) return;

		if ( !GetConsoleScreenBufferInfo( std_out, &buf_info ) ) return;
		cell_count = buf_info.dwSize.X * buf_info.dwSize.Y;

		if ( !FillConsoleOutputCharacter( std_out, ( TCHAR )' ', cell_count, null_coord, &count ) ) return;
		if ( !FillConsoleOutputAttribute( std_out, buf_info.wAttributes, cell_count, null_coord, &count ) ) return;
		
		set_cursor_pos( 0, 0 );
	}

	inline void set_size( int w, int h ) {
		auto con = GetConsoleWindow( );
		RECT r;

		GetWindowRect( con, &r );
		MoveWindow( con, r.left, r.top, w, h, 1 );
	}
}