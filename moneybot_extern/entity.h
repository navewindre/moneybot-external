#pragma once
#include <stdint.h>
#include "vector.h"
#include "factory.h"
#include "offsets.h"

class c_base_player;
class c_entity {
public:
	c_entity( ) { }
	c_entity( uintptr_t ptr ) : m_ptr( ptr ) { }
	c_entity operator=( uintptr_t ptr ) { m_ptr = ptr; }
	explicit operator bool( ) const noexcept { return !!m_ptr; }
	uintptr_t operator()( ) const { return m_ptr; }

	template < typename t > __forceinline t get( std::ptrdiff_t offset ) {
		return g_proc->get< t >( m_ptr + offset );
	}
	template < typename t > __forceinline void set( std::ptrdiff_t offset, t value ) {
		g_proc->write< t >( m_ptr + offset, value );
	}

	NETVAR( m_vecVelocity, xorstr( "m_vecVelocity" ), xorstr( "DT_BaseGrenade" ), 0, vec3_t );
	NETVAR( m_dwBoneMatrix, xorstr( "m_nForceBone" ), xorstr( "DT_BaseAnimating" ), 28, uintptr_t );
	NETVAR( m_fFlags, xorstr( "m_fFlags" ), xorstr( "DT_BasePlayer" ), 0, int );
	NETVAR( m_vecOrigin, xorstr( "m_vecOrigin" ), xorstr( "DT_BaseEntity" ), 0, vec3_t );
	NETVAR( m_iTeamNum, xorstr( "m_iTeamNum" ), xorstr( "DT_BaseEntity" ), 0, int );
	NETVAR( m_flSimulationTime, xorstr( "m_flSimulationTime" ), xorstr( "DT_BaseEntity" ), 0, float );
	NETVAR( m_nModelIndex, xorstr( "m_nModelIndex" ), xorstr( "DT_BaseViewModel" ), 0, int );
	NETVAR( m_clrRender, xorstr( "m_clrRender" ), xorstr( "DT_BaseCombatCharacter" ), 0, byteclr_t );

	OFFSET( m_vecViewOffset, 0x104, vec3_t );
	OFFSET( m_bDormant, 0xe9, bool );
	OFFSET( m_iLifeState, 0x025b, int );
	OFFSET( m_nTickBase, 0x3424, int );
	OFFSET( m_nIndex, 0x64, int );


	c_client_class get_client_class( );

protected:
	uintptr_t m_ptr{ };
};

namespace csgo
{
	class c_entlist {
	public:
		template < typename t = c_entity > t get_entity( int i ) {
			return t( g_proc->get< uintptr_t >( g_proc.m_client( ) + g_offsets.m_entlist + ( i - 1 ) * 0x10 ) );
		}
		c_base_player get_localplayer( );
	};
}