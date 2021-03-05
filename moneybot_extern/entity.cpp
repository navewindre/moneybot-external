#include "entity.h"
#include "client.h"
#include "sdk.h"
#include "offsets.h"
#include "weapon.h"

c_client_class c_entity::get_client_class( ) {
	auto client_networkable = get< uintptr_t >( 0x8 );
	auto create_fn			= g_proc->get< uintptr_t >( client_networkable + 2 * sizeof( uintptr_t ) );
	auto clientclass_ptr	= g_proc->get< uintptr_t >( create_fn + 0x1 );

	return g_proc->get< c_client_class >( clientclass_ptr );
}

//----------------------------------------------------------------------//

c_base_player csgo::c_entlist::get_localplayer( ) {
	int local_index = g_instance.m_engine.get_clientstate( ).get_localplayer( );
	return get_entity< c_base_player >( local_index );
}