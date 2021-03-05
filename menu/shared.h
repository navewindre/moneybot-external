#pragma once
#ifdef MENU_EXPORTS
#define MENU_API __declspec( dllexport )
#else
#define MENU_API __declspec( dllimport )
#endif

namespace shared
{
	extern MENU_API bool was_init;
	extern MENU_API std::string cur_username;
}