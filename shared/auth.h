#pragma once
#include <string>
#include "http.h"



using ulong_t = unsigned long;

using namespace std::chrono_literals;

namespace auth
{
	/*for now we will just have a gay hwid check*/
	/*type in vol c: to get your hdd serial from cmd*/
	struct hwid_info_t {
		std::string m_username;
		ulong_t		m_hdd_serial;
	};

	const char* const SERVER_NAME = "www.navewindre.xyz";
	const char* const AUTH_DIRECTORY = "/auth/external_auth_test.txt";

	class c_auth {
	public:
		c_auth( ) {
			if ( !parse_data( ) || !parse_info( ) ) {
				MessageBoxA( 0, "couldn't parse user info, check your internet connection", "error", MB_OK );
				exit( 0 );
			}
		}

	private:
		bool parse_data( );
		bool parse_info( );

	public:
		bool is_user_valid( );

		std::string get_username( ) const {
			if ( m_cur_user == -1 ) return std::string( );
			return m_users[ size_t( m_cur_user ) ].m_username;
		}

		std::string get_data( ) const {
			return std::string( ( char* )m_data.data( ) );
		}

	private:
		//gay
		std::vector< uint8_t > m_data;
		int  m_cur_user{ -1 };
		std::vector< hwid_info_t > m_users;
	};
}

#include "authdef.h"