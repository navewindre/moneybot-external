#pragma once
#include "hook.h"
#include "shellcode.h"
#include "factory.h"

namespace drawing
{
	constexpr size_t STRING_LENGTH	= 64;
	constexpr size_t STRING_COUNT	= 128;
	//first 64 strings reserved for players
	//rest can be used for whatever, guns etc
	//you can always change this number
	//and allocate more if necessary

	class c_drawing {
	protected:
		uintptr_t m_string_array;
		uintptr_t m_vector_array;

	protected:
		util::c_hook m_hook;
		util::c_shellcode m_shellcode;

	public:
		c_drawing( ) : m_shellcode( std::make_shared< decltype( g_proc ) >( g_proc ) ) { };
		~c_drawing( );

		bool hook_hudupdate( );
		void set_text( int array_index, vec3_t position, char* string );
	};
}

extern drawing::c_drawing g_drawing;
