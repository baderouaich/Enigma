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
	SingleInstanceApplication(const String& package_name)
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
		int cond_id, mutex_id;
		int mode = S_IRWXU | S_IRWXG;
		int readMutex = false;
		/* Initialise attribute to mutex. */
		pthread_mutexattr_init(&attrmutex);
		pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);

		/* Allocate memory to pmutex here. */
		mutex_id = shm_open(MYMUTEX, O_RDWR, mode);
		if (mutex_id < 0) {

			std::cout << "Creating New mutex" << std::endl;
			mutex_id = shm_open(MYMUTEX, O_CREAT | O_RDWR | O_TRUNC, mode);
			if (mutex_id < 0) {
				std::cout << "shm_open failed with " << MYMUTEX << " ,Error: " << mutex_id << std::endl;
				return false;
			}
	}
		else {
			std::cout << "Reading existing mutex" << std::endl;
			readMutex = true;
		}

		if (ftruncate(mutex_id, sizeof(pthread_mutex_t)) == -1) {
			std::cout << "ftruncate failed with " << MYMUTEX << std::endl;
			return false;
		}
		pmutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex_id, 0);
		if (pmutex == MAP_FAILED) {
			std::cout << "mmap failed with " << MYMUTEX << std::endl;
			return false;
		}

		/* Initialise mutex. */
		if (!readMutex) {
			pthread_mutex_init(pmutex, &attrmutex);
		}

		if (pthread_mutex_trylock(pmutex) == 0) {
			lockflag = true;
			RunEndless();
		}
		else {
			std::cout << "Application instance already running" << std::endl;
			return false;
		}
		/* Use the mutex. */
		/*auto fname = "/var/run/" + package_name + ".pid";
		m_handle = open(fname.c_str(), O_CREAT | O_RDWR, 0666);
		auto rc = flock(m_handle, LOCK_EX | LOCK_NB);
		if (rc && errno == EWOULDBLOCK)
		{
			// another instance is running
			WarnAndExit();
		}*/
#endif
	}

	void WarnAndExit()
	{	
		Shutdown();
		DialogUtils::Warn("Another instance of " + m_package_name + " is already running.");
		std::exit(EXIT_SUCCESS);
	}

	void Shutdown()
	{
#if defined(ENIGMA_PLATFORM_WINDOWS)
		::ReleaseMutex(m_handle); // Explicitly release mutex
		::CloseHandle(m_handle); // close handle before terminating
#elif defined(ENIGMA_PLATFORM_LINUX)
		shm_unlink(MYMUTEX);
		if (lockflag)
			pthread_mutex_unlock(pmutex);
		pthread_mutex_destroy(pmutex);
		pthread_mutexattr_destroy(&attrmutex);
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
	i32 m_handle{};

	pthread_mutex_t* pmutex;
	pthread_mutexattr_t attrmutex;
	bool lockflag = false;
#define MYMUTEX "mymutex"

#endif
};

NS_ENIGMA_END

#endif	// !ENIGMA_SINGLE_INSTANCE_H
