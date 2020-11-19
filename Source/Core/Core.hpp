#pragma once
#ifndef ENIGMA_CORE_H
#define ENIGMA_CORE_H

#include "Platform.hpp"
#include "Macros.hpp"
#include "Constants.hpp"
#include "Types.hpp"
#include "Version.hpp"

/// Enable/Disable gtest Testing
#define ENIGMA_TEST true
///


///API import & export macros
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
#define ENIGMA_DEBUG_BREAK() __debugbreak()
#elif defined(ENIGMA_PLATFORM_LINUX)
#include <signal.h>
#define ENIGMA_DEBUG_BREAK() raise(SIGTRAP)
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
#endif
///



/// __super keyword
#ifndef _MSC_VER 
#define __super  
#endif
/// 


// ENIGMA_LOG_CURRENT_FUNCTION() on debug
#ifdef ENIGMA_DEBUG
	#define ENIGMA_LOG_CURRENT_FUNCTION() ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION())
#else
	#define ENIGMA_LOG_CURRENT_FUNCTION()
#endif // ENIGMA_DEBUG


#endif // !ENIGMA_CORE_H