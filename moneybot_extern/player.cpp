#include "factory.h"
#include "client.h"
#include "player.h"
#include "weapon.h"
#include "bsp_map.h"

bool c_base_player::is_valid( ) {
	if ( !m_ptr ) return false;
	if ( m_ptr == g_instance.m_local( ) ) return false;
	if ( !!m_iLifeState( ) ) return false;
	if ( m_bDormant( ) ) return false;
	int team = m_iTeamNum( );
	if ( team != 2 && team != 3 ) return false;
	if ( get_client_class( ).m_index != CCSPlayer ) {
		return false;
	}
	return true;
}

bool c_base_player::is_visible( ) {
	vec3_t start = g_instance.m_local.get_eye_pos( );
	vec3_t end	 = get_eye_pos( );

	return g_bsp.is_visible( start, end, 4000 );
}

vec3_t c_base_player::get_eye_pos( ) {
	return vec3_t( m_vecOrigin( ) + m_vecViewOffset( ) );
}

vec3_t c_base_player::get_bone_pos( int bone ) {
	uintptr_t	bone_matrix_ptr = m_dwBoneMatrix( );
	matrix3x4	bone_matrix[ 128 ];

	g_proc->get( bone_matrix_ptr, bone_matrix, sizeof( bone_matrix ) );

	return vec3_t(
		bone_matrix[ bone ].data[ 0 ][ 3 ],
		bone_matrix[ bone ].data[ 1 ][ 3 ],
		bone_matrix[ bone ].data[ 2 ][ 3 ]
	);
}

vec3_t c_base_player::velocity_comp( int bone ) {
	vec3_t bone_pos = get_bone_pos( bone );
	vec3_t velocity = m_vecVelocity( );
	auto& globals 	= g_csgo.m_globals.get< globalvars_t >( );
	float ipt 		= globals.m_interval_per_tick;

	return bone_pos + velocity * ipt;
}

vec3_t c_base_player::get_rotated_pos( int bone, float rotation ) {
	vec3_t bone_pos;
	vec3_t origin;
	vec3_t bone_offset;

	bone_pos	= velocity_comp( bone );
	origin		= m_vecOrigin( );
	bone_offset = bone_pos - origin;

	float radius = bone_offset.length2d( );
	float radian = rotation * M_RADPI;

	bone_offset.x = sin( radian ) * radius;
	bone_offset.y = cos( radian ) * radius;

	bone_offset += origin;

	return bone_offset;
}

c_base_weapon c_base_player::get_weapon( ) {
	int weapon_index = m_hActiveWeapon( ) & 0xfff;

	return g_instance.m_entlist.get_entity< c_base_weapon >( weapon_index );
}