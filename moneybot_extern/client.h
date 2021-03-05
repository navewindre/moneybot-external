#pragma once
#include "csgo.h"
#include "player.h"
#include "engine.h"
#include "math.h"

class c_instance {
public:
	c_instance( ) : m_local( ) { };

public:
	c_base_player m_local{ };

public:
	csgo::c_entlist m_entlist{ };
	csgo::c_engine m_engine{ };

public:
	bool run_frame( ) {
		m_local = m_entlist.get_localplayer( );
		return !!m_local;
	}
};

extern c_instance g_instance;