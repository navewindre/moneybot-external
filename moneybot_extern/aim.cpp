#include "aim.h"
#include "client.h"
#include "../menu/settings.h"
#include "backtracking.h"
#include "player.h"
#include "weapon.h"

lagcomp::c_backtracking g_backtracking;

namespace features
{
	void c_aim::the_only_rational_triggerbot( ) {
		if ( !g_settings.aim.triggerbot( ) ) return;
		if ( !util::key_down( g_settings.aim.aimkeytest( ) ) ) return;

		auto weapon = g_instance.m_local.get_weapon( );
		if ( !weapon || weapon.is_non_aim( ) ) return;

		int  incross = g_instance.m_local.m_iCrosshairId( );
		auto entity = g_instance.m_entlist.get_entity< c_base_player >( incross );

		if ( !entity.is_valid( ) ) return;
		if ( entity.m_iTeamNum( ) == g_instance.m_local.m_iTeamNum( ) && !g_settings.aim.filer_team( ) )
			return;

		g_instance.m_engine.force_attack( );
	}

	int c_aim::get_aim_target( ) {
		int		target{ -1 };
		float	best_fov = float( g_settings.aim.aimbot_fov( ) ); //pls
		vec3_t	aim_angle{ };
		vec3_t	cur_angle{ };
		auto&	cl = g_instance.m_engine.get_clientstate( );

		cur_angle = cl.get_viewangles( );

		for ( int i{ 1 }; i <= 64; ++i ) {
			auto entity = g_instance.m_entlist.get_entity< c_base_player >( i );

			if ( !entity.is_valid( ) ) continue;
			if ( !entity.is_visible( ) ) continue; //uffja

			aim_angle = g_math.vector_angles( g_instance.m_local.get_eye_pos( ), entity.get_bone_pos( 8 ) );

			float fov = g_math.get_fov( cur_angle, aim_angle );
			if ( fov < best_fov ) {
				best_fov = fov;
				target = i;
			}
		}

		return target;
	}

	void c_aim::sample_angle_data( ) {
		auto	cl = g_instance.m_engine.get_clientstate( );
		vec3_t	cur_angles = cl.get_viewangles( );

		auto time = g_timer.now( );
		
		m_angle_samples.push_front( { cur_angles, time } );

		int size_diff = m_angle_samples.size( ) - g_settings.aim.smooth_samples( );
		if ( size_diff > 0 ) {
			for ( int i{ }; i <= size_diff; ++i ) {
				m_angle_samples.pop_back( );
			}
		}
	}

	float c_aim::get_avg_delta( ) {
		if ( m_angle_samples.empty( ) )
			return 0.f;

		float avg_delta{ };

		for ( auto& it : m_angle_samples ) {
			static vec3_t last_angle = it.m_viewangles;

			//doing this implements nonsticky aswell
			//once you already are at the target
			//after lets say a flick
			//it will reduce the aim speed
			//making it not "stick" to the bone
			//as we dont store records when aiming
			std::chrono::duration< float > time_delta = g_timer.now( ) - it.m_time;
			auto delta_diff = m_deltatime / time_delta.count( );

			vec3_t	angle_delta = ( last_angle - it.m_viewangles ).clamp( );
			float	delta_length = angle_delta.length( ) * delta_diff;
			
			avg_delta += delta_length;
			last_angle = it.m_viewangles;
		}

		return avg_delta / float( std::min< size_t >( g_settings.aim.smooth_samples( ), m_angle_samples.size( ) ) );
	}

	void c_aim::aim_at_target( c_base_player& target ) {
		vec3_t	aim_pos;
		vec3_t	aim_ang;
		vec3_t	cur_ang;
		vec3_t	delta;
		auto	cl = g_instance.m_engine.get_clientstate( );

		auto weapon = g_instance.m_local.get_weapon( );
		int  wep_id = weapon.m_iItemDefinitionIndex( );

		cur_ang = cl.get_viewangles( );
		aim_pos = target.get_bone_pos( wep_id == WEAPON_AWP ? 6 : 8 );
		aim_ang = g_math.vector_angles( g_instance.m_local.get_eye_pos( ), aim_pos );

		if ( g_instance.m_local.m_iShotsFired( ) > 1 ) {
			aim_ang -= g_instance.m_local.m_vecPunch( ) * 2.f;
		}

		delta = ( aim_ang.clamp( ) - cur_ang ).clamp( );

		float delta_length = delta.length( );

		if ( delta_length ) {
			float final_time = delta_length / ( float( g_settings.aim.aimbot_speed( ) ) / 100.f );
			m_curtime += m_deltatime;

			if ( m_curtime > final_time ) {
				m_curtime = final_time;
			}

			float aim_progress = m_curtime / final_time;

			delta *= aim_progress;
			aim_ang = cur_ang + delta;

			cl.set_viewangles( aim_ang.clamp( ) );
		}
	}

	void c_aim::aimbot( ) {
		if ( !g_settings.aim.aimbot( ) ) return;
		
		static auto old_time = g_timer.now( );
		auto current_time = g_timer.now( );

		std::chrono::duration< float > delta_time = current_time - old_time;

		auto weapon = g_instance.m_local.get_weapon( );
		if ( !weapon || weapon.is_non_aim( ) ) return;
		
		m_deltatime = delta_time.count( );
		if ( g_settings.aim.dynamic_smooth( ) ) {
			m_deltatime += get_avg_delta( ) * g_settings.aim.smooth_multiplier( );
		}

		old_time = current_time;

		bool in_attack = false;

		if ( g_proc->get< int >( g_proc.m_client( ) + g_offsets.m_forceattack ) == 5 )
			in_attack = true;
		if ( g_settings.aim.triggerbot( ) && g_settings.aim.trigger_magnet( ) && util::key_down( g_settings.aim.aimkeytest( ) ) )
			in_attack = true;

		int  target_index = get_aim_target( );
		auto target = g_instance.m_entlist.get_entity< c_base_player >( target_index );

		if ( in_attack && target.is_valid( ) && ( target.m_iTeamNum( ) != g_instance.m_local.m_iTeamNum( ) || g_settings.aim.filer_team( ) ) ) {
			aim_at_target( target );
		}
		else {
			sample_angle_data( );
			m_curtime = 0.f;
		}
	}
}