#include "drawing.h"
#include "client.h"

drawing::c_drawing g_drawing;

namespace drawing
{
	c_drawing::~c_drawing( ) {
		g_proc->free( m_vector_array, STRING_COUNT * sizeof( vec3_t ), MEM_RELEASE );
		g_proc->free( m_string_array, STRING_COUNT * STRING_LENGTH, MEM_RELEASE );
		m_shellcode.deallocate( );
	}

	bool c_drawing::hook_hudupdate( ) {
		m_hook = util::c_hook( &g_csgo.m_chl, 11 );
		if ( !m_hook.get_original( ) ) {
			return false;
		}

		m_string_array = g_proc->allocate( STRING_COUNT * STRING_LENGTH, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
		g_proc->write( m_string_array, 0, STRING_COUNT * STRING_LENGTH );

		m_vector_array = g_proc->allocate( STRING_COUNT * sizeof( vec3_t ), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
		g_proc->write( m_string_array, 0, STRING_COUNT * sizeof( vec3_t ) );

		auto add_text_overlay = g_csgo.m_debug_overlay.get_function( 7 );
		if ( !add_text_overlay ) return false;

		m_shellcode.push( { 0x55, 0x89, 0xe5, 0xff, 0x75, 0x08 } );
		m_shellcode.push( { uint8_t( 0xb8 ) } );
		m_shellcode.push( { uintptr_t( m_hook.get_original( ) ) } );
		m_shellcode.push( { 0xff, 0xd0 } );
		for ( int i{ }; i <= STRING_COUNT; ++i ) {
			m_shellcode.push( { uint8_t( 0xb8 ) } );
			m_shellcode.push( { uintptr_t( add_text_overlay ) } );
			m_shellcode.push( { m_string_array + STRING_LENGTH * i } );
			m_shellcode.push( { 0x51 } );
			m_shellcode.push( { 0xc7, 0x04, 0x24, 0x6f, 0x12, 0x83, 0x3a } );
			m_shellcode.push( { uint8_t( 0x68 ) } );
			m_shellcode.push( { uintptr_t( m_vector_array + sizeof( vec3_t ) * i ) } );
			m_shellcode.push( { uint8_t( 0x68 ) } );
			m_shellcode.push( { uintptr_t( g_csgo.m_debug_overlay( ) ) } );
			m_shellcode.push( { 0xff, 0xd0 } );
			m_shellcode.push( { 0x83, 0xc4, 0x10 } );
		}
		m_shellcode.push( { uint8_t( 0x5d ) } );
		m_shellcode.push( { 0xc2, 0x04, 0x00 } );

		m_hook.hook( m_shellcode.allocate( ) );
		return true;
	}

	void c_drawing::set_text( int array_index, vec3_t position, char* text ) {
		g_proc->write( m_string_array, text, STRING_LENGTH * array_index );
		g_proc->write< vec3_t >( m_vector_array + array_index * sizeof( vec3_t ), position );
	}
}