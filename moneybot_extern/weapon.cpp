#include "weapon.h"
#include "sdk.h"

bool c_base_weapon::is_non_aim( ) {
	return is_bomb( ) || is_taser( ) || is_knife( ) || is_grenade( ) || m_iClip1( ) == 0;
}

bool c_base_weapon::is_rcs( ) {
	return !is_non_aim( ) && !( is_pistol( ) || is_sniper( ) || is_shotgun( ) );
}

bool c_base_weapon::is_bomb( ) {
	return m_iItemDefinitionIndex( ) == WEAPON_C4;
}

bool c_base_weapon::is_taser( ) {
	return m_iItemDefinitionIndex( ) == WEAPON_TASER;
}

bool c_base_weapon::is_knife( ) {
	int def_index = m_iItemDefinitionIndex( );

	switch ( def_index ) {
	case WEAPON_KNIFE:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFEGG:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_BOWIE:
		return true;
	default:
		return false;
	}
}

bool c_base_weapon::is_grenade( ) {
	int def_index = m_iItemDefinitionIndex( );

	switch ( def_index ) {
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
	case WEAPON_DECOY:
		return true;
	default:
		return false;
	}

}

bool c_base_weapon::is_pistol( ) {
	int def_index = m_iItemDefinitionIndex( );

	switch ( def_index ) {
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_HKP2000:
	case WEAPON_P250:
	case WEAPON_TEC9:
	case WEAPON_USP_SILENCER:
	case WEAPON_R8REVOLVER:
		return true;
	default: return false;
	}
}

bool c_base_weapon::is_shotgun( ) {
	int def_index = m_iItemDefinitionIndex( );

	switch ( def_index ) {
	case WEAPON_NOVA:
	case WEAPON_SAWEDOFF:
	case WEAPON_XM1014:
	case WEAPON_MAG7:
		return true;
	default:
		return false;
	}
}

bool c_base_weapon::is_sniper( ) {
	int def_index = m_iItemDefinitionIndex( );

	switch ( def_index ) {
	case WEAPON_AWP:
	case WEAPON_G3SG1:
	case WEAPON_SCAR20:
	case WEAPON_SSG08:
		return true;
	default:
		return false;
	}
}