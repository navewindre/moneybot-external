#pragma once
#include <inttypes.h>

class byteclr_t {
	uint8_t R, G, B, A;
public:
	byteclr_t( ) : R( 0 ), G( 0 ), B( 0 ), A( 0 ) { }
	
	byteclr_t( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) : R( r ), G( g ), B( b ), A( a ) { }

	byteclr_t( uint8_t r, uint8_t g, uint8_t b ) : R( r ), G( g ), B( b ), A( 255 ) { }

	uint8_t r( ) { return R; }
	uint8_t g( ) { return G; }
	uint8_t b( ) { return B; }
	uint8_t a( ) { return A; }
	void r( uint8_t _r ) { R = _r; }
	void g( uint8_t _g ) { G = _g; }
	void b( uint8_t _b ) { B = _b; }
	void a( uint8_t _a ) { A = _a; }

	byteclr_t& operator =( byteclr_t& c ) {
		R = c.r( );
		G = c.g( );
		B = c.b( );
		A = c.a( );
		return *this;
	}

	byteclr_t operator+( const byteclr_t& v ) const {
		return byteclr_t( R + v.R, G + v.G, B + v.B, A + v.A );
	}

	explicit operator bool( ) const noexcept {
		return ( R > 0 || G > 0 || B > 0 || A > 0 );
	}

	bool operator==( byteclr_t& c ) const {
		return ( R == c.r( ) && G == c.g( ) && B == c.b( ) );
	}
};

class fclr_t {
	float R, G, B, A;
public:
	fclr_t( ) : R( 0 ), G( 0 ), B( 0 ), A( 0 ) { }

	fclr_t( float r, float g, float b, float a ) : R( r ), G( g ), B( b ), A( a ) { }

	fclr_t( float r, float g, float b ) : R( r ), G( g ), B( b ), A( 255 ) { }

	float r( ) { return R; }
	float g( ) { return G; }
	float b( ) { return B; }
	float a( ) { return A; }
	void r( uint8_t _r ) { R = _r; }
	void g( uint8_t _g ) { G = _g; }
	void b( uint8_t _b ) { B = _b; }
	void a( uint8_t _a ) { A = _a; }

	fclr_t& operator =( fclr_t& c ) {
		R = c.r( );
		G = c.g( );
		B = c.b( );
		A = c.a( );
		return *this;
	}

	fclr_t operator+( const fclr_t& v ) const {
		return fclr_t( R + v.R, G + v.G, B + v.B, A + v.A );
	}

	explicit operator bool( ) const noexcept {
		return ( R > 0 || G > 0 || B > 0 || A > 0 );
	}

	bool operator==( fclr_t& c ) const {
		return ( R == c.r( ) && G == c.g( ) && B == c.b( ) );
	}
};

class clr_t {
	int R, G, B, A;
public:
	clr_t( ) : R( 0 ), G( 0 ), B( 0 ), A( 0 ) { }

	clr_t( int r, int g, int b, int a ) : R( r ), G( g ), B( b ), A( a ) { }

	clr_t( int r, int g, int b ) : R( r ), G( g ), B( b ), A( 255 ) { }

	static clr_t white( ) { return clr_t( 255, 255, 255, 255 ); }
	static clr_t black( ) { return clr_t( 0, 0, 0, 255 ); }
	static clr_t red( ) { return clr_t( 255, 0, 0, 255 ); }
	static clr_t green( ) { return clr_t( 0, 255, 0, 255 ); }
	static clr_t blue( ) { return clr_t( 0, 0, 255, 255 ); }
	static clr_t yellow( ) { return clr_t( 255, 255, 0, 255 ); }

	int r( ) { return R; }
	int g( ) { return G; }
	int b( ) { return B; }
	int a( ) { return A; }
	void r( int _r ) { R = _r; }
	void g( int _g ) { G = _g; }
	void b( int _b ) { B = _b; }
	void a( int _a ) { A = _a; }

	clr_t& operator =( clr_t& c ) {
		R = c.r( );
		G = c.g( );
		B = c.b( );
		A = c.a( );
		return *this;
	}

	clr_t operator+( const clr_t& v ) const {
		return clr_t( R + v.R, G + v.G, B + v.B, A + v.A );
	}

	explicit operator bool( ) const noexcept {
		return ( R > 0 || G > 0 || B > 0 || A > 0 );
	}

	bool operator==( clr_t& c ) const {
		return ( R == c.r( ) && G == c.g( ) && B == c.b( ) );
	}

	static clr_t from_hsb( float hue, float saturation, float brightness ) {
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - ( int )h;
		float p = brightness * ( 1.0f - saturation );
		float q = brightness * ( 1.0f - saturation * f );
		float t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) );

		if ( h < 1 ) {
			return clr_t(
				( unsigned char )( brightness * 255 ),
				( unsigned char )( t * 255 ),
				( unsigned char )( p * 255 )
			);
		}
		else if ( h < 2 ) {
			return clr_t(
				( unsigned char )( q * 255 ),
				( unsigned char )( brightness * 255 ),
				( unsigned char )( p * 255 )
			);
		}
		else if ( h < 3 ) {
			return clr_t(
				( unsigned char )( p * 255 ),
				( unsigned char )( brightness * 255 ),
				( unsigned char )( t * 255 )
			);
		}
		else if ( h < 4 ) {
			return clr_t(
				( unsigned char )( p * 255 ),
				( unsigned char )( q * 255 ),
				( unsigned char )( brightness * 255 )
			);
		}
		else if ( h < 5 ) {
			return clr_t(
				( unsigned char )( t * 255 ),
				( unsigned char )( p * 255 ),
				( unsigned char )( brightness * 255 )
			);
		}
		else {
			return clr_t(
				( unsigned char )( brightness * 255 ),
				( unsigned char )( p * 255 ),
				( unsigned char )( q * 255 )
			);
		}
	}

	static clr_t blend( clr_t first, clr_t second, float t ) {
		return clr_t(
			first.r( ) + static_cast< int >( t * ( second.r( ) - first.r( ) ) ),
			first.g( ) + static_cast< int >( t * ( second.g( ) - first.g( ) ) ),
			first.b( ) + static_cast< int >( t * ( second.b( ) - first.b( ) ) ),
			first.a( ) + static_cast< int >( t * ( second.a( ) - first.a( ) ) )
		);
	}

	float hue( ) {
		typedef struct {
			float h, s, v;
		} hsv;
		hsv         out;
		float       min, max, delta;

		min = static_cast< float >( R < G ? R : G );
		min = static_cast< float >( min < B ? min : B );

		max = static_cast< float >( R > G ? R : G );
		max = static_cast< float >( max > B ? max : B );

		out.v = max;
		delta = max - min;
		if ( delta < 0.0010f ) {
			out.s = 0.f;
			out.h = 0.f;
			return out.h;
		}
		if ( max > 0.0f ) {
			out.s = ( delta / max );
		}
		else {
			out.s = 0.0f;
			out.h = ( float )( uintptr_t )( "lol nan" );
			return out.h;
		}
		if ( R >= max )
			out.h = static_cast< float >( G - B ) / delta;
		else
			if ( G >= max )
				out.h = 2.0f + static_cast< float >( B - R ) / delta;
			else
				out.h = 4.0f + static_cast< float >( R - G ) / delta;

		out.h *= 60.0f;
		out.h /= 360.f;

		if ( out.h < 0.0f )
			out.h += 360.0f;

		return out.h;
	}

	byteclr_t to_byteclr( ) {
		return byteclr_t{ ( uint8_t )R, ( uint8_t )G, ( uint8_t )B, ( uint8_t )A };
	}

	fclr_t to_fclr( ) {
		return fclr_t{ ( float )R / 255, ( float )G / 255, ( float )B / 255, ( float )A / 255 };
	}
};