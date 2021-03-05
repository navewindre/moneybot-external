#pragma once
#include "util.h"
#include "sdk.h"

namespace csgo
{
	class i_chl_client {
	public:
		c_client_class* get_list( ) {
			return util::call_vfunc< c_client_class*( __thiscall* )( void* ) >( this, 8 )( this );
		}
	};
}