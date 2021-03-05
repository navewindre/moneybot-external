#pragma once
#include <string>
#include <atldef.h>
#include "vector.h"

//guess ill just leave this here?
#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI_F / 180.f) )

namespace util {
	class c_math {
	public:
		float get_fov( vec3_t view, vec3_t dest );

		vec3_t vector_angles( vec3_t forward, vec3_t angles );
		__forceinline void __fastcall sin_cos( float radian, float* sine, float* cosine );
		void angle_vectors( vec3_t view, vec3_t* forward );
	};
}

extern util::c_math g_math;