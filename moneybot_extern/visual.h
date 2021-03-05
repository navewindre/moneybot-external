#pragma once

namespace features
{
	class c_visuals {
	private:
		void glow( );
		void text_esp( );
		void update( );

	public:
		void operator()( ) {
			glow( );
			//text_esp( );
			update( );
		}
	};
}