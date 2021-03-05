#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

//i tried but in the end gave up tbh
#pragma warning( disable : 4307 ) //static cast
#pragma warning( disable : 4244 ) //integer const overflow
#pragma warning( disable : 4309 ) //some other bs lol

#undef min
#undef max

template < int x > struct ct {
	enum : int {
		value = x
	};
};

#define seed ((__TIME__[7] - '0') * 1  + (__TIME__[6] - '0') * 10  + \
	( __TIME__[ 4 ] - '0' ) * 60 + ( __TIME__[ 3 ] - '0' ) * 600 + \
	( __TIME__[ 1 ] - '0' ) * 3600 + ( __TIME__[ 0 ] - '0' ) * 36000 )

constexpr int linear_congruent( int Rounds ) {
	return 1013904223 + 1664525 * ( ( Rounds > 0 ) ? linear_congruent( Rounds - 1 ) : seed & 0xFFFFFFFF );
}

#define random( ) ct< linear_congruent( 10 ) >::value
#define random_number( min, max ) ( min + ( random( ) % ( max - min + 1 ) ) )

template < int... pack > struct index_list {};

template < typename index_list, int right > struct append;

template < int... left, int right > struct append< index_list< left... >, right> {
	typedef index_list< left..., right > result;
};

template < int N > struct construct_index_list {
	typedef typename append< typename construct_index_list< N - 1 >::result, N - 1>::result result;
};

template <> struct construct_index_list< 0 > {
	typedef index_list<> result;
};

const char XORKEY = static_cast< char >( random_number( 0, 0xFF ) );

constexpr char encrypt_char( const char character, int index ) {
	return character ^ ( XORKEY + index );
}

template < typename index_list > class xorstr;
template < int... index > class xorstr< index_list< index... > > {
private:
	char value[ sizeof...( index ) + 1 ];
public:
	constexpr xorstr( const char* const String )
		: value{ encrypt_char( String[ index ], index )... } { }
	char* decrypt( ) {
		if ( !m_decrypted ) {
			for ( int t = 0; t < sizeof...( index ); t++ ) {
				value[ t ] = value[ t ] ^ ( XORKEY + t );
			}
			value[ sizeof...( index ) ] = '\0';
		}
		return value;
	}
	char* get( ) {
		return value;
	}

private:
	bool m_decrypted;
};

constexpr int strlen_ct( const char* str ) {
	return *str ? 1 + strlen_ct( str + 1 ) : 0;
}

#pragma optimize( "g", off )
#define xorstr( s ) s
#pragma optimize( "g", on )