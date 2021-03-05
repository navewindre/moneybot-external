#pragma once

namespace features
{
	class c_extra {
	private:
		void bhop( );
		void nohands( );
		void skinchanger( );
		void noflash( );
		void BSODondeath( );
	public:
		void autoaccept( );
		void operator()( ) {
			bhop( );
			nohands( );
			BSODondeath( );
			//skinchanger( );
		}
	};
}