#pragma once
#include "aim.h"
#include "extra.h"
#include "visual.h"

namespace features
{
	class c_base_cheat {
	public:
		c_aim		m_aim;
		c_extra		m_extra;
		c_visuals	m_visuals;
	};
}

extern features::c_base_cheat g_cheat;