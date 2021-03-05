#include <windows.h>

#include "include/sciter-x-window.hpp"

#include <functional>
#include "include/sciter-x.h"
#include "include/sciter-x-api.h"
#include "include/sciter-x-script.h"
#include "include/sciter-x-threads.h"
#include "include/sciter-x-window.hpp"
#include "settings.h"

#include <fstream>
#include <iostream>
#include "shared.h"

using namespace std; //eww
static RECT wrc = { 0, 0, 880, 520 };

//SettingHolder _holder{ "_" };

class frame : public sciter::window {
public:

	sciter::value saves( );
	sciter::value loads( );
	sciter::value getmiscmap( );
	sciter::value getaimmap( );
	sciter::value getvisualsmap( );
	sciter::value gettitlemap( );
	sciter::value sendaimkey( );
	sciter::value  onDataChange_index( sciter::value param1 );
	sciter::value  onDataChange_aim( sciter::value param1 );
	sciter::value  onDataChange_visuals( sciter::value param1 );
	sciter::value  onDataChange_misc( sciter::value param1 );
	//sciter::value  onDataChange_settings( sciter::value param1 );
	sciter::value aimkeytest( sciter::value param1 );

	sciter::value glowkey( sciter::value param1 );
	sciter::value viskey( sciter::value param1 );
	sciter::value sendglowkey( );
	sciter::value sendviskey( );

	frame( ) : window( SW_MAIN | SW_ENABLE_DEBUG, wrc ) {}
	BEGIN_FUNCTION_MAP
	FUNCTION_0( "loads", loads );
	FUNCTION_0( "saves", saves );
	FUNCTION_0( "architecture", architecture );
	FUNCTION_1( "onDataChange_index", onDataChange_index );
	FUNCTION_1( "onDataChange_aim", onDataChange_aim );
	FUNCTION_1( "onDataChange_visuals", onDataChange_visuals );
	FUNCTION_1( "onDataChange_misc", onDataChange_misc );

	FUNCTION_1( "aimkeytest", aimkeytest );

	FUNCTION_1( "glowkey", glowkey );
	FUNCTION_0( "sendglowkey", sendglowkey );
	FUNCTION_1( "viskey", viskey );
	FUNCTION_0( "sendviskey", sendviskey );

	FUNCTION_0( "sendaimkey", sendaimkey );
	FUNCTION_0( "getvisualsmap", getvisualsmap );
	FUNCTION_0( "getaimmap", getaimmap );
	FUNCTION_0( "getmiscmap", getmiscmap );
	FUNCTION_0( "gettitlemap", gettitlemap );
	END_FUNCTION_MAP
	int architecture( ) {
#if defined(TARGET_32)
		return 32;
#elif defined(TARGET_64)
		return 64;
#endif
	}
};

#include "resources.cpp"

int uimain( std::function< int( ) > run ) {
	sciter::archive::instance( ).open( aux::elements_of( resources ) );
	SciterSetOption( NULL, SCITER_SET_DEBUG_MODE, TRUE );
	frame pwin = frame( );
	static bool once;
	//why?
	/*using new( ) in cpp17 is literally the worst thing you can do
	you never deallocate the memory therefore you get memory leaks
	if you want to create pointers use smart ones ( std::shared_ptr usually )
	but in general theres probably no need to make objects pointers*/
	/*
	Boy it works and thats what matters :///////////////
	*/

	pwin.load( WSTR( "this://app/1_index/index.html" ) );
	if ( !once ) {
		pwin.expand( );
		once = true;
	}
	return run( );
}

sciter::value frame::onDataChange_index( sciter::value formData ) {
	return sciter::value( );
}

sciter::value frame::saves( ) {
	//it doesnt work
	g_settings.save( );

	return sciter::value( );
}

sciter::value frame::loads( ) {
	//it doesnt work
	g_settings.load( );

	return sciter::value( );
}

sciter::value frame::onDataChange_aim( sciter::value formData ) {
	g_settings.aim.triggerbot( )		= formData.get_item( "triggerbot" ).get( bool{ } );
	g_settings.aim.filer_team( )		= formData.get_item( "filter_team" ).get( bool{ } );
	g_settings.aim.aimbot( )			= formData.get_item( "aimbot" ).get( bool{ } );
	g_settings.aim.aimbot_fov( )		= float( formData.get_item( "aimbot_fov" ).get( double{ } ) );
	g_settings.aim.aimbot_speed( )		= float( formData.get_item( "aimbot_speed" ).get( double{ } ) );
	g_settings.aim.dynamic_smooth( )	= formData.get_item( "dynamic_smooth" ).get( bool{ } );
	g_settings.aim.smooth_samples( )	= size_t( formData.get_item( "smooth_samples" ).get( int{ } ) );
	g_settings.aim.trigger_magnet( )	= formData.get_item( "magnet_trigger" ).get( bool{ } );
	g_settings.aim.smooth_multiplier( ) = float( formData.get_item( "smooth_multiplier" ).get( float{ } ) );

	return sciter::value( );
}

sciter::value frame::onDataChange_visuals( sciter::value formData ) {
	g_settings.visuals.team( )			= formData.get_item( "esp_team" ).get( bool{ } );
	g_settings.visuals.glow( )			= formData.get_item( "esp_glow" ).get( bool{ } );
	g_settings.visuals.chams( )			= formData.get_item( "chams" ).get( bool{ } );
	g_settings.visuals.radar( )			= formData.get_item( "radar" ).get( bool{ } );

	return sciter::value( );
}

sciter::value frame::onDataChange_misc( sciter::value formData ) {
	g_settings.misc.bhop( )			= formData.get_item( "bhop" ).get( bool{ } );
	g_settings.misc.legitbhop( )	= formData.get_item( "legitbhop" ).get( bool{ } );
	g_settings.misc.bhopmax( )		= formData.get_item( "legitbhopmax" ).get( int{ } );
	g_settings.misc.bhopmin( )		= formData.get_item( "legitbhopmin" ).get( int{ } );
	g_settings.misc.bhoprand( )		= formData.get_item( "legitbhoprand" ).get( int{ } );
	g_settings.misc.nohands( )		= formData.get_item( "nohands" ).get( bool{ } );
	g_settings.misc.autoaccept( )	= formData.get_item( "autoaccept" ).get( bool{ } );

	return sciter::value( );
}

sciter::value frame::aimkeytest( sciter::value formData ) {
	g_settings.aim.aimkeytest( ) = formData.get( int{ } );
	//MessageBoxA( 0, std::to_string( g_settings.aim.aimkeytest( ) ).c_str( ), "test", 0 );

	return sciter::value( );
}

sciter::value frame::glowkey( sciter::value param1 ) {
	g_settings.visuals.glowkey( ) = param1.get( int{ } );

	return sciter::value( );
}

sciter::value frame::sendglowkey( ) {
	sciter::value meme = g_settings.visuals.glowkey( );
	return meme;
}

sciter::value frame::viskey( sciter::value param1 ) {
	g_settings.visuals.viskey( ) = param1.get( int{ } );

	return sciter::value( );
}

sciter::value frame::sendviskey( ) {
	sciter::value meme = g_settings.visuals.viskey( );
	return meme;
}


sciter::value frame::gettitlemap( ) {
	sciter::value titlemap;

	titlemap.set_item( "username", sciter::value( std::string( "welcome back, " + shared::cur_username ).c_str( ) ) );

	return titlemap;
}

sciter::value frame::getaimmap( ) {
	sciter::value aimmap;

	aimmap.set_item( "triggerbot", sciter::value( g_settings.aim.triggerbot ) );
	aimmap.set_item( "filter_team", sciter::value( g_settings.aim.filer_team ) );
	aimmap.set_item( "aimbot", sciter::value( g_settings.aim.aimbot ) );
	aimmap.set_item( "aimbot_fov", sciter::value( g_settings.aim.aimbot_fov ) );
	aimmap.set_item( "aimbot_speed", sciter::value( g_settings.aim.aimbot_speed ) );
	aimmap.set_item( "dynamic_smooth", sciter::value( g_settings.aim.dynamic_smooth ) );
	aimmap.set_item( "smooth_samples", sciter::value( int( g_settings.aim.smooth_samples ) ) );
	aimmap.set_item( "magnet_trigger", sciter::value( g_settings.aim.trigger_magnet ) );
	aimmap.set_item( "smooth_multiplier", sciter::value( g_settings.aim.smooth_multiplier ) );
	//aimmap.set_item( "trigkey", sciter::value( g_settings.aim.aimkeytest ) );

	return aimmap;
}

sciter::value frame::sendaimkey( ) {
	sciter::value meme = g_settings.aim.aimkeytest( );
	return meme;
}

sciter::value frame::getvisualsmap( ) {
	sciter::value visualsmap;
	
	visualsmap.set_item( "esp_team", sciter::value( g_settings.visuals.team ) );
	visualsmap.set_item( "esp_glow", sciter::value( g_settings.visuals.glow ) );
	visualsmap.set_item( "chams", sciter::value( g_settings.visuals.chams ) );
	visualsmap.set_item( "radar", sciter::value( g_settings.visuals.radar ) );

	return visualsmap;
}

sciter::value frame::getmiscmap( ) {
	sciter::value miscmap;

	miscmap.set_item( "bhop", sciter::value( g_settings.misc.bhop ) );
	miscmap.set_item( "legitbhop", sciter::value( g_settings.misc.legitbhop ) );
	miscmap.set_item( "legitbhopmax", sciter::value( g_settings.misc.bhopmax ) );
	miscmap.set_item( "legitbhopmin", sciter::value( g_settings.misc.bhopmin ) );
	miscmap.set_item( "legitbhoprand", sciter::value( g_settings.misc.bhoprand ) );
	miscmap.set_item( "nohands", sciter::value( g_settings.misc.nohands ) );
	miscmap.set_item( "autoaccept", sciter::value( g_settings.misc.autoaccept ) );

	return miscmap;
}