#include "settings.h"
#include "client.h"
#include "extra.h"
#include "sdk.h"
#include "syscall.h"
#include <cstdlib>
#include <thread>
#include "weapon.h"
using namespace std;

namespace features
{
	void c_extra::bhop( ) {
		if ( !g_settings.misc.bhop( ) ) return;
		if ( g_instance.m_local.m_vecVelocity( ).length2d( ) < 20.f ) return;
		if ( g_instance.m_local.m_iMoveType( ) == ( int )e_move_type::MOVETYPE_LADDER ) return;

		bool air = !( g_instance.m_local.m_fFlags( ) & ( int )e_player_states::FL_ONGROUND );

		if ( g_settings.misc.legitbhop( ) ) {
			//how about we dont completely fuck our threading up?
			//you need to explain threading to me at some point, real shit
			static int counter{ };
			static int counter2{ };

			if ( util::key_down( VK_SPACE ) && !air ) {
				int output = rand( ) % ( g_settings.misc.bhopmax( ) );
				for ( int i = 0; i < output; ++i ) {
					if ( counter == 1 ) {
						g_instance.m_engine.force_jump( 5 );
					}
					else {
						g_instance.m_engine.force_jump( 4 );
					}
				}
				counter++;
				if ( counter > g_settings.misc.bhoprand( ) )
					counter = 0;
			}
		}
		else {
			if ( util::key_down( VK_SPACE ) ) {
				g_instance.m_engine.force_jump( air ? 4 : 6 );
			}
		}
	}

	void c_extra::nohands( ) {
		static bool updated{ };
		static bool updated_2{ };
		if ( !g_settings.misc.nohands( ) ) {
			updated = false;
			if ( !updated_2 ) {
				auto& cl = g_instance.m_engine.get_clientstate( );
				cl.force_update( );
				updated_2 = true;
			}
			return;
		}

		updated_2 = false;
		g_instance.m_local.m_nModelIndex( 0 );

		if ( !updated ) {
			auto& cl = g_instance.m_engine.get_clientstate( );
			cl.force_update( );
			updated = true;
		}
	}

	void c_extra::skinchanger( ) {
		auto weapon = g_instance.m_local.get_weapon( );

		if ( weapon( ) ) {
			/**/
		}
	}
	
	void c_extra::noflash( ) {
		if ( !g_settings.misc.noflash( ) ) return;
		if ( g_instance.m_local.is_valid( ) ) {
			if ( g_instance.m_local.m_flFlashMaxAlpha( ) >= g_settings.misc.noflashpercent( ) ) {
				g_instance.m_local.m_flFlashMaxAlpha( 0.f );
			}
		}
	}
	void c_extra::autoaccept( ){
		if ( !g_settings.misc.autoaccept( ) ) return;
		if ( g_instance.m_engine.match_found( ) ) {
			g_instance.m_engine.accept_match( );
		}

	}

	void c_extra::BSODondeath( ) {
	/*	if ( !g_settings.misc.bsod ) return;
		if ( !g_instance.run_frame( ) ) return;
		if ( g_instance.m_local.m_iLifeState == 0 )
		{
			g_proc->BSOD( STATUS_ASSERTION_FAILURE );
		}
		*/
	}


}