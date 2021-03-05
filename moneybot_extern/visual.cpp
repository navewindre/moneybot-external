#include "visual.h"
#include "sdk.h"
#include "factory.h"
#include "offsets.h"
#include "client.h"
#include "settings.h"
#include "drawing.h"

namespace features
{
	void c_visuals::glow( ) {
		static bool glow_pressed{ };

		if ( util::key_down( g_settings.visuals.glowkey( ) ) ) {
			if ( !glow_pressed ) {
				g_settings.visuals.glowtoggle ^= 1;
				glow_pressed = true;
			}
		}
		else glow_pressed = false;

		if ( !g_settings.visuals.glow( ) ) return;
		if ( !g_settings.visuals.glowtoggle ) return;

		auto set_glow_clr = [ ]( glow_object_t* object, fclr_t color, float bloom = 1.0f ) {
			object->m_rwo	= true;
			object->m_rwuo	= false;
			object->m_color = color;

			object->m_bloom_amt	= bloom;
			object->m_full_bloom = false;
		};

		glow_obj_manager_t	obj_manager = g_proc->get< glow_obj_manager_t >( g_proc.m_client( ) + g_offsets.m_glowobject_manager );
		if ( !obj_manager.m_count || obj_manager.m_count > 4096 ) return;
		//push these bytes to the stack nigga fuck the heap nigga
		glow_object_t*		glow_object = static_cast< glow_object_t* >( _alloca( sizeof( glow_object_t ) * obj_manager.m_count ) );
		
		g_proc->get( uintptr_t( obj_manager.m_objects ), glow_object, sizeof( glow_object_t ) * obj_manager.m_count );

		for ( size_t i{ }; i <= obj_manager.m_count; ++i ) {
			auto& object = glow_object[ i ];

			if ( !object.m_ent || object.m_ent == g_instance.m_local( ) ) {
				continue;
			}

			c_entity ent( object.m_ent );
			if ( ent.m_iTeamNum( ) == g_instance.m_local.m_iTeamNum( ) && !g_settings.visuals.team( ) ) {
				continue;
			}

			int class_id = ent.get_client_class( ).m_index;

			if ( class_id == CCSPlayer ) {
				c_base_player player_ent( ent );
				fclr_t color;

				if ( !player_ent.is_valid( ) ) continue;

				if ( player_ent.m_iTeamNum( ) == 2 ) {
					color = player_ent.is_visible( ) ? fclr_t( 1.0f, 0.17f, 0.37f, 0.7f ) : fclr_t( 1.f, 1.f, 1.f, 0.7f );
				}
				if ( player_ent.m_iTeamNum( ) == 3 ) {
					color = player_ent.is_visible( ) ? fclr_t( 0.17f, 0.67f, 0.8f, 0.7f ) : fclr_t( 0.1f, 0.1f, 1.f, 0.7f );
				}

				set_glow_clr( &object, color );
			}
			
			
			uintptr_t	cur_obj_address	= uintptr_t( obj_manager.m_objects ) + ( i * sizeof( glow_object_t ) + 0x4 );
			void*		cur_obj_buffer	= reinterpret_cast< void* >( uintptr_t( glow_object ) + ( i * sizeof( glow_object_t ) + 0x4 ) );
			g_proc->write( cur_obj_address, cur_obj_buffer, sizeof( glow_object_t ) - 0x4 - 0x12 );
		}
	}

	void c_visuals::update( ) {
		static bool was_pressed{ };
		if ( util::key_down( g_settings.visuals.viskey( ) ) ) {
			if ( !was_pressed ) {
				g_settings.visuals.vistoggle ^= 1;
				was_pressed = true;
			}
		}
		else was_pressed = false;

		if ( !g_settings.visuals.vistoggle ) return;

		for ( int i{ 1 }; i < 64; ++i ) {
			auto ent = g_instance.m_entlist.get_entity< c_base_player >( i );
			if ( !ent.is_valid( ) ) continue;

			if ( ent.m_iTeamNum( ) == g_instance.m_local.m_iTeamNum( ) && !g_settings.visuals.team( ) )
				continue;

			if ( g_settings.visuals.chams( ) ) {
				ent.m_clrRender( ent.m_iTeamNum( ) == 2 ? byteclr_t{ 255, 40, 40, 255 } : byteclr_t{ 40, 40, 255 } );
			}

			if ( g_settings.visuals.radar( ) ) {
				ent.m_bSpotted( true );
			}
		}
	}

	void c_visuals::text_esp( ) {
		for ( int i{ }; i < 64; ++i ) {
			auto ent = g_instance.m_entlist.get_entity< c_base_player >( i );

			if ( ent.is_valid( ) ) {
				vec3_t		position{ };
				std::string txt{ };

				position = ent.m_vecOrigin( );
				txt += std::to_string( ent.m_iHealth( ) );

				g_drawing.set_text( i, position, txt.data( ) );
			}
		}
	}
}