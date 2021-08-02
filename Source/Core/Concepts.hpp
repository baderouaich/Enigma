#pragma once
#ifndef ENIGMA_CONCEPTS_H
#define ENIGMA_CONCEPTS_H
#include <yvals_core.h> // for __cpp_lib_concepts

#ifdef __cpp_lib_concepts
#include <concepts>

namespace Enigma
{
	namespace Concepts
	{
		/** Require a type to be an integer (bool excluded) */
		template<typename T>
		concept IsInteger = std::is_integral<T>::value && !std::is_same<T, bool>::value;

		/** Require a type to be a floating point */
		template<typename T>
		concept IsFloatingPoint = std::is_floating_point<T>::value;

		/** Require a type to be a boolean */
		template<typename T>
		concept IsBoolean = std::is_same<T, bool>::value;
	}
}
#else 
	#pragma message("Concepts are not supported by the compiler")
#endif

#endif // !ENIGMA_CONCEPTS_H
