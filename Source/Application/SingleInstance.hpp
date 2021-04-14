#pragma once
#ifndef ENIGMA_SINGLE_INSTANCE_H
#define ENIGMA_SINGLE_INSTANCE_H


#if defined(ENIGMA_PLATFORM_WINDOWS)
#include <Windows.h>
// On Windows case, we're using CreateMutex to register our unique app id
#elif defined(ENIGMA_PLATFORM_LINUX)
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
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
class SingleInstance
{
public:
	/*
	*	Register a unique instance
	*	@param package_name: application's unique package name for example com.badereddineouaich.enigma
	*/
	SingleInstance(const String& package_name)
		:
		m_package_name(package_name)
	{
		const auto warn_and_exit_on_multiple_instance = [this, &package_name]
		{
			Terminate();
			DialogUtils::Warn("Another instance of " + package_name + " is already running.");
			std::exit(EXIT_SUCCESS);
		};
#if defined(ENIGMA_PLATFORM_WINDOWS)
		m_handle = ::CreateMutexA(NULL, TRUE, package_name.c_str());
		if (m_handle == nullptr || ::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// another instance is running
			warn_and_exit_on_multiple_instance();
		}
#elif defined(ENIGMA_PLATFORM_LINUX)
		auto fname = "/var/run/" + package_name + ".pid";
		m_handle = open(fname.c_str(), O_CREAT | O_RDWR, 0666);
		auto rc = flock(m_handle, LOCK_EX | LOCK_NB);
		if (rc && errno == EWOULDBLOCK)
		{
			// another instance is running
			warn_and_exit_on_multiple_instance();
		}
#endif

		ENIGMA_INFO("Registered single application instance");
	}

	void Terminate()
	{
#if defined(ENIGMA_PLATFORM_WINDOWS)
		::ReleaseMutex(m_handle); // Explicitly release mutex
		::CloseHandle(m_handle); // close handle before terminating
#elif defined(ENIGMA_PLATFORM_LINUX)
		if (m_handle < 0)
			return;
		auto fname = "/var/run/" + m_package_name + ".pid";
		remove(fname.c_str());
		close(m_handle);
#endif

		ENIGMA_INFO("Unregistered single application instance");
	}


	/*
	*	Unregister unique instance
	*/
	virtual ~SingleInstance()
	{
		Terminate();
	}

private:
	String m_package_name;
#if defined(ENIGMA_PLATFORM_WINDOWS)
	HANDLE m_handle{};
#elif defined(ENIGMA_PLATFORM_LINUX)
	i32 m_handle{};
#endif
};

NS_ENIGMA_END

#endif	// !ENIGMA_SINGLE_INSTANCE_H
