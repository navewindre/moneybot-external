#pragma once
#include <deque>
#include <chrono>

#include "vector.h"

class c_base_player;

namespace features
{
	class c_aim {
	private:
		struct aim_record_t {
			vec3_t m_viewangles;
			std::chrono::time_point< std::chrono::steady_clock > m_time;
		};

		void	the_only_rational_triggerbot( );
		void	aimbot( );
		void	aim_at_target( c_base_player& );
		void	sample_angle_data( );
		float	get_avg_delta( );
		int		get_aim_target( );

		float m_deltatime;
		float m_curtime;

		std::deque< aim_record_t > m_angle_samples;
	public:
		void operator()( ) {
			the_only_rational_triggerbot( );
			aimbot( );
		}
	};
}