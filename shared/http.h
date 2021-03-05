#include <WinInet.h>
#include <Windows.h>
#include <sstream>
#include <thread>
#include <vector>

#pragma comment( lib, "wininet.lib" )

using namespace std::chrono_literals;
using ulong_t = unsigned long;

namespace http {
	class inethandle_t {
	public:
		operator HINTERNET( ) { return m_handle; }
		inethandle_t( HINTERNET handle ) : m_handle( handle ) { };
		inethandle_t( ) : m_handle( nullptr ) { };
		~inethandle_t( ) {
			InternetCloseHandle( m_handle );
		}

	private:
		HINTERNET m_handle;
	};

	auto get_file( const std::string& server, const std::string& filename, std::string login, std::string pass, bool secure = false ) {
		inethandle_t connect = InternetOpenA( "null", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0 );
		inethandle_t address = InternetConnectA( connect, server.c_str( ), 0, login.c_str( ), pass.c_str( ), INTERNET_SERVICE_HTTP, 0, 0 );
		if ( !address ) {
			MessageBoxA( 0, "couldn't connect to server, check your internet connection", "error", MB_OK );
			exit( 0 );
		}

		inethandle_t request = HttpOpenRequestA( address, "GET", filename.c_str( ), 0, 0, 0, INTERNET_FLAG_KEEP_CONNECTION, 0 );
		if ( !request ) {
			MessageBoxA( 0, "couldn't connect to server, check your internet connection", "error", MB_OK );
			exit( 0 );
		}

		HttpSendRequestA( request, 0, 0, 0, 0 );

		std::this_thread::sleep_for( 10ms );

		ulong_t size = 256;
		ulong_t temp = 0;
		char response[ 256 ];

		HttpQueryInfoA( request, HTTP_QUERY_CONTENT_LENGTH, response, &size, 0 );
		size = atoi( response );
		auto buf = new uint8_t[ size ];

		ulong_t data_received;
		if ( !InternetReadFile( request, buf, size, &data_received ) || !data_received ) {
			MessageBoxA( 0, "couldn't connect to server, check your internet connection", "error", MB_OK );
			exit( 0 );
		}

		std::vector< uint8_t > data;
		for ( size_t i{ }; i < size; ++i ) {
			data.push_back( buf[ i ] );
		}

		delete[] buf;
		return data;
	}
}