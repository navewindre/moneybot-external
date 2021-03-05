#include "math.h"

namespace util
{
	float c_math::get_fov( vec3_t view, vec3_t dest ) {
		auto delta = view - dest;

		return delta.length2d( );
	}

	vec3_t c_math::vector_angles( vec3_t start, vec3_t end ) {
		vec3_t delta = end - start;

		float magnitude = sqrtf( delta.x * delta.x + delta.y * delta.y );
		float pitch = atan2f( -delta.z, magnitude ) * M_RADPI;
		float yaw = atan2f( delta.y, delta.x ) * M_RADPI;

		vec3_t angle( pitch, yaw, 0.0f );
		return angle.clamp( );
	}

	//gotta get the turbofast register args dude
	__forceinline void __fastcall c_math::sin_cos( float radians, float* sine, float* cosine  ) {
		__asm { //haha
			fld dword ptr[ radians ]
			fsincos
			mov edx, dword ptr[ cosine ]
			mov eax, dword ptr[ sine ]
			fstp dword ptr[ edx ]
			fstp dword ptr[ eax ]
		}
	}

	__forceinline void c_math::angle_vectors( vec3_t view, vec3_t* forward ) {
		float sp, sy, cp, cy;

		sin_cos( DEG2RAD( view[ 1 ] ), &sy, &cy );
		sin_cos( DEG2RAD( view[ 0 ] ), &sp, &cp );

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}
}