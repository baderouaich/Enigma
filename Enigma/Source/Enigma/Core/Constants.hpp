#pragma once
#ifndef ENIGMA_CONSTANTS_H
#define ENIGMA_CONSTANTS_H

#include "Types.hpp"

namespace Enigma
{
	namespace Constants
	{
		namespace Graphics
		{
			/*
			*	OpenGL Default Version (Consider using version 4.3 as minimum to work properly)
			*/
			static constexpr const ui16 OpenGL_MAJOR_VERSION = 4;
			static constexpr const ui16 OpenGL_MINOR_VERSION = 3;
		}

		namespace Math
		{
			static constexpr const f64 PI = 3.14159265358979323846;			// PI
			static constexpr const f64 HALF_PI = 1.57079632679489661923;	// PI/2
			static constexpr const f64 E = 2.71828182845904523536;			// e
			static constexpr const f64 LOG2E = 1.44269504088896340736;		// log2(e)
			static constexpr const f64 LOG10E = 0.434294481903251827651;	// log10(e)
			static constexpr const f64 LN2 = 0.693147180559945309417;		// ln(2)
			static constexpr const f64 LN10 = 2.30258509299404568402;		// ln(10)
			static constexpr const f64 EPSILON = 2.2204460492503131e-016;	// Epsilon
		}
	}
}

#endif // !ENIGMA_CONSTANTS_H
