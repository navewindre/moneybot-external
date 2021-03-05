#pragma once
#include "entity.h"

class c_base_player : public c_entity {
public:
	c_base_player( ) : c_entity( ) { };
	c_base_player( c_entity ent ) : c_entity( ent ) { };

	NETVAR( m_flLowerBodyYawTarget, xorstr( "m_flLowerBodyYawTarget" ), xorstr( "DT_CSPlayer" ), 0, float );
	NETVAR( m_hActiveWeapon, xorstr( "m_hActiveWeapon" ), xorstr( "DT_BaseCombatCharacter" ), 0, ulong_t );
	NETVAR( m_bGunGameImmunity, xorstr( "m_bGunGameImmunity" ), xorstr( "DT_CSPlayer" ), 0, int );
	NETVAR( m_iHealth, xorstr( "m_iHealth" ), xorstr( "DT_BasePlayer" ), 0, int );
	NETVAR( m_flFlashMaxAlpha, xorstr( "m_flFlashMaxAlpha" ), xorstr( "DT_BasePlayer" ), 0, float );
	NETVAR( m_iShotsFired, xorstr( "m_iShotsFired" ), xorstr( "DT_CSPlayer" ), 0, int );
	NETVAR( m_iCrosshairId, xorstr( "m_bHasDefuser" ), xorstr( "DT_CSPlayer" ), 92, int );
	NETVAR( m_iMoveType, xorstr( "m_nRenderMode" ), xorstr( "DT_BaseEntity" ), 0x1, int );
	NETVAR( m_bSpotted, xorstr( "m_bSpotted" ), xorstr( "DT_BaseEntity" ), 0, bool );

	OFFSET( m_angEyeAngles, 0xaa08, vec3_t );
	OFFSET( m_vecPunch, 0x301c, vec3_t );

	class c_base_weapon get_weapon( );
	vec3_t get_eye_pos( );
	vec3_t get_bone_pos( int bone );
	vec3_t velocity_comp( int bone );
	vec3_t get_rotated_pos( int bone, float rotation );

	bool is_valid( );
	bool is_visible( );
};