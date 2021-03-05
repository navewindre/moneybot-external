namespace auth
{
	bool c_auth::parse_data( ) {
		m_data = http::get_file( SERVER_NAME, AUTH_DIRECTORY, "externalauth", "test12" );
		return !m_data.empty( );
	}

	bool c_auth::parse_info( ) {
		if ( m_data.empty( ) ) {
			return false;
		}

		std::string str( ( char* )m_data.data( ) );
		size_t last_pos = str.find( "USER: {" );
		size_t users_count = str[ 0 ] - '0';

		if ( !last_pos ) {
			return false;
		}

		while ( last_pos != std::string::npos ) {
			hwid_info_t cur_info;
			std::string current_section;
			size_t cur_read{ };

			while ( last_pos + cur_read < str.size( ) && str[ last_pos + cur_read ] != '}' ) {
				current_section += str[ last_pos + cur_read ];
				cur_read++;
			}

			auto read_section = [ & ]( const std::string& name ) {
				size_t pos = current_section.find( name ) + name.length( );
				std::string result;

				size_t read{ };
				while ( current_section[ pos + read ] != ';' ) {
					result += current_section[ pos + read ];
					read++;
				}

				return result;
			};

			std::string login_str = read_section( "login: " );
			std::string hwid_str = read_section( "hwid: " );
			std::string disable_str = read_section( "disable: " );

			bool disable = !!atoi( disable_str.c_str( ) );

			if ( !disable ) {
				cur_info = hwid_info_t{ login_str, std::strtoul( hwid_str.c_str( ), 0, 16 ) };
				m_users.push_back( cur_info );
			}

			last_pos = str.find( "USER: {", last_pos + 1 );
		}

		return true;
	}

	bool c_auth::is_user_valid( ) {
		if ( m_users.empty( ) ) {
			return false;
		}

		ulong_t hdd_serial;
		GetVolumeInformationA( "C:\\", 0, 0, &hdd_serial, 0, 0, 0, 0 );

		for ( size_t user_index = 0; user_index < m_users.size( ); user_index++ ) {
			if ( m_users[ user_index ].m_hdd_serial == hdd_serial ) {
				m_cur_user = user_index;
				return true;
			}
		}

		return false;
	}
}