#pragma once
#include "entity.h"

class c_base_weapon : public c_entity {
public:
	c_base_weapon( ) : c_entity( ) { };
	c_base_weapon( c_entity ent ) : c_entity( ent ) { };

	NETVAR( m_iItemIDHigh, xorstr( "m_iItemIDHigh" ), xorstr( "DT_BaseAttributableItem" ), 0, int );
	NETVAR( m_iItemIDLow, xorstr( "m_iItemIDLow" ), xorstr( "DT_BaseAttributableItem" ), 0, int );
	NETVAR( m_iEntityQuality, xorstr( "m_iEntityQuality" ), xorstr( "DT_BaseAttributableItem" ), 0, int );
	NETVAR( m_OriginalOwnerXuidLow, xorstr( "m_OriginalOwnerXuidLow" ), xorstr( "DT_BaseAttributableItem" ), 0, int );
	NETVAR( m_OriginalOwnerXuidHigh, xorstr( "m_OriginalOwnerXuidHigh" ), xorstr( "DT_BaseAttributableItem" ), 0, int );
	NETVAR( m_nFallbackPaintKit, xorstr( "m_nFallbackPaintKit" ), xorstr( "DT_BaseAttributableItem" ), 0, int );
	NETVAR( m_nFallbackSeed, xorstr( "m_nFallbackSeed" ), xorstr( "DT_BaseAttributableItem" ), 0, int );
	NETVAR( m_flFallbackWear, xorstr( "m_flFallbackWear" ), xorstr( "DT_BaseAttributableItem" ), 0, float );
	NETVAR( m_nFallbackStatTrak, xorstr( "m_nFallbackStatTrak" ), xorstr( "DT_BaseAttributableItem" ), 0, int );
	NETVAR( m_iClip1, xorstr( "m_iClip1" ), xorstr( "DT_BaseCombatWeapon" ), 0, int );

	OFFSET( m_iItemDefinitionIndex, 0x2f88, int );

	bool is_pistol( );
	bool is_non_aim( );
	bool is_shotgun( );
	bool is_sniper( );
	bool is_rcs( );
	bool is_knife( );
	bool is_grenade( );
	bool is_taser( );
	bool is_bomb( );
};