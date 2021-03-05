#pragma once
#include <array>
#include "entity.h"
#include "client.h"
#include "sdk.h"

constexpr const size_t BACKTRACKING_BACKUP = 64;

namespace lagcomp
{
	class c_backtracking {
	public:
		c_backtracking( ) { };
		~c_backtracking( ) { };

		void update( ) {
			for ( int i{ }; i < 64; ++i ) {
				auto ent = g_instance.m_entlist.get_entity< c_base_player >( i );
				if ( !ent.is_valid( ) ) continue;

				auto& record = m_records[ i ];
				record.store( i );
			}
		}

		vec3_t backtrack_entity( int i, int tick ) {
			return m_records[ i ].restore( tick );
		}

	protected:
		struct lag_record {
			void store( int ent_number ) {
				//store entity's simulation time to backtrack to later
				//cache it by your own tickbase, we cant access usercmd reliably
				// fucking nerd
				m_index = ent_number;
				auto ent = g_instance.m_entlist.get_entity< c_base_player >( m_index );
				m_simtime[ g_instance.m_local.m_nTickBase( ) % BACKTRACKING_BACKUP ] = ent.m_flSimulationTime( );
				m_headpos[ g_instance.m_local.m_nTickBase( ) % BACKTRACKING_BACKUP ] = ent.get_bone_pos( 8 );
			}

			vec3_t restore( int tick ) {
				//simply set the simtime back and the tickcount
				int 	bt_tick;
				int		tick_delta;
				auto 	ent = g_instance.m_entlist.get_entity( m_index );
				
				bt_tick 	= TIME_TO_TICKS( m_simtime[ tick % BACKTRACKING_BACKUP ] + util::get_lerp( ) );
				tick_delta 	= g_instance.m_engine.set_tickcount( bt_tick );
				
				ent.m_flSimulationTime( m_simtime[ tick + tick_delta ] );
				return m_headpos[ ( tick + tick_delta ) % BACKTRACKING_BACKUP ];
			}

			int m_index{ };
			std::array< float, BACKTRACKING_BACKUP >	m_simtime;
			std::array< vec3_t, BACKTRACKING_BACKUP >	m_headpos;
			matrix3x4 m_bonedata[ 128 ];
		};

		std::array< lag_record, 64 > m_records;
	};
}

extern lagcomp::c_backtracking g_backtracking;