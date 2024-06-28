#pragma once
#ifndef ENIGMA_CORE_H
#define ENIGMA_CORE_H

#include "Platform.hpp"
#include "Macros.hpp"
#include "Constants.hpp"
#include "Concepts.hpp"
#include "Types.hpp"
#include "Version.hpp"

/// Enable/Disable Catch2 Tests
#define ENIGMA_TESTING false
///

///Enable/Disable Profiling (N/I)
#define ENIGMA_PROFILE false
///


/// GL Version (using 3.3 for compatibility)
#define ENIGMA_GL_VERSION_MAJOR 3
#define ENIGMA_GL_VERSION_MINOR 3
///


/// API import & export macros
#if ENIGMA_DYNAMIC_LINK
#ifdef ENIGMA_BUILD_DLL
	#define ENIGMA_API __declspec(dllexport)
#else
	#define ENIGMA_API __declspec(dllimport)
#endif
#else
	#define ENIGMA_API
#endif
///



/// Multi platform break debug function
#ifdef ENIGMA_DEBUG
	//Break debug
#if defined(ENIGMA_PLATFORM_WINDOWS)
	#define ENIGMA_DEBUG_BREAK() ::__debugbreak()
#elif defined(ENIGMA_PLATFORM_LINUX)
#include <csignal>
	#define ENIGMA_DEBUG_BREAK() std::raise(SIGTRAP)
#else
	#define ENIGMA_DEBUG_BREAK() std::abort()
#endif
#else
	#define ENIGMA_DEBUG_BREAK()
#endif
///





//Windows
#if defined(_MSC_VER)
//Disable warning  4251 in MSC Microsoft Visual C++
//Warning	C4251 needs to have dll - interface to be used by clients of class 
#ifdef ENIGMA_DYNAMIC_LINK 
	#pragma warning( disable: 4251 )
#endif
/// _CRT_SECURE_NO_WARNINGS Disable Clang warnings
#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef NOMINMAX
	// On Windows, a header file defines two macros min and max which may result in 
	// conflicts with their counterparts in the standard library and therefore in errors during compilation.
	#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#endif
///


#endif // !ENIGMA_CORE_H
