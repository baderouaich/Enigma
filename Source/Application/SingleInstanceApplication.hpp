#pragma once
#ifndef ENIGMA_SINGLE_INSTANCE_H
#define ENIGMA_SINGLE_INSTANCE_H


#if defined(ENIGMA_PLATFORM_WINDOWS)
#include <Windows.h>
// On Windows case, we're using CreateMutex to register our unique app id
#elif defined(ENIGMA_PLATFORM_LINUX)
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
// On Linux case, we're using a file lock/unlock to register our unique app id
#else
#error Unsupported platform!
#endif

#include <Core/Core.hpp>
#include <Utility/DialogUtils.hpp>


NS_ENIGMA_BEGIN
/*
*	Single instance application interface
*	if you inherrit from this class, your app will have only one instance at a time.
*	Some of this code is inspired from https://rosettacode.org/wiki/Determine_if_only_one_instance_is_running
*/
class SingleInstanceApplication
{
public:
	/*
	*	Register a unique instance
	*	@param package_name: application's unique package name for example com.badereddineouaich.enigma
	*/
	explicit SingleInstanceApplication(const String& package_name)
		:
		m_package_name(package_name)
	{
		Initialize();
		ENIGMA_INFO("Registered single application instance");
	}


	void Initialize()
	{
#if defined(ENIGMA_PLATFORM_WINDOWS)
		m_handle = ::CreateMutexA(NULL, TRUE, m_package_name.c_str());
		if (m_handle == nullptr || ::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// another instance is running
			WarnAndExit();
		}
#elif defined(ENIGMA_PLATFORM_LINUX)
#endif
	}

	void WarnAndExit()
	{	
		DialogUtils::Warn("Another instance of Enigma is already running.");
		Shutdown();
		std::exit(EXIT_SUCCESS);
	}

	void Shutdown()
	{
#if defined(ENIGMA_PLATFORM_WINDOWS)
		::ReleaseMutex(m_handle); // Explicitly release mutex
		::CloseHandle(m_handle); // close handle before terminating
#elif defined(ENIGMA_PLATFORM_LINUX)
#endif

		ENIGMA_INFO("Unregistered single application instance");
	}


	/*
	*	Unregister unique instance
	*/
	virtual ~SingleInstanceApplication()
	{
		Shutdown();
	}

private:
	String m_package_name;
#if defined(ENIGMA_PLATFORM_WINDOWS)
	HANDLE m_handle{};
#elif defined(ENIGMA_PLATFORM_LINUX)
#endif
};

NS_ENIGMA_END

#endif	// !ENIGMA_SINGLE_INSTANCE_H
