#pragma once
#ifndef ENIGMA_CPU_INFO_H
#define ENIGMA_CPU_INFO_H


#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#if defined(ENIGMA_PLATFORM_WINDOWS)
// Windows Includes
#include <Windows.h>
#elif defined(ENIGMA_PLATFORM_LINUX)
// Linux Includes
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/times.h>
#elif defined(ENIGMA_PLATFORM_MACOS)
// MacOS Includes
#include <mach/mach_error.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/vm_map.h>
#endif

NS_ENIGMA_BEGIN
/**
*	CPUInfo class will gather informations about the CPU at runtime
*/
class CPUInfo {
  public:
    CPUInfo() noexcept;
    ~CPUInfo() noexcept = default;

    ENIGMA_NON_COPYABLE(CPUInfo);
    ENIGMA_NON_MOVEABLE(CPUInfo);


  public:
    /**
	*	Returns cpu usage (in percentage [0% -> 100%])
	*/
    float GetCPUUsage() noexcept;

    /**
	*	Returns cpu usage by current process (in percentage [0% -> 100%])
	*/
    float GetProcessCPUUsage() noexcept;

  private: /** Platform Functions */
#if defined(ENIGMA_PLATFORM_WINDOWS) || defined(ENIGMA_PLATFORM_MACOS)
    /**
	*	Calculates CPU Load percentage by idle and total ticks for (used for Windows & MacOS)
	*/
    float CalculateCPULoad(const std::uint64_t idle_ticks, const std::uint64_t total_ticks) {
      const std::uint64_t total_ticks_since_last_time = total_ticks - m_cpu_previous_total_ticks;
      const std::uint64_t idle_ticks_since_last_time = idle_ticks - m_cpu_previous_idle_ticks;

      const float rate = 1.0f - ((total_ticks_since_last_time > 0) ? static_cast<float>(idle_ticks_since_last_time) / total_ticks_since_last_time : 0.0f);

      m_cpu_previous_total_ticks = total_ticks;
      m_cpu_previous_idle_ticks = idle_ticks;

      return (rate * 100.0f);
    }
#endif


  private: /** Platform Variables */
#if defined(ENIGMA_PLATFORM_WINDOWS)

    FILETIME m_idle_time{}, m_kernel_time{}, m_user_time{};
    std::uint64_t m_cpu_previous_total_ticks{0};
    std::uint64_t m_cpu_previous_idle_ticks{0};

    std::uint64_t FileTimeToUInt64(const FILETIME& file_time) {
      return (static_cast<std::uint64_t>(file_time.dwHighDateTime) << 32) | static_cast<std::uint64_t>(file_time.dwLowDateTime);
    }

#elif defined(ENIGMA_PLATFORM_LINUX)

    std::uint64_t m_last_total_user{0};
    std::uint64_t m_last_total_user_low{0};
    std::uint64_t m_last_total_sys{0};
    std::uint64_t m_last_total_idle{0};

#elif defined(ENIGMA_PLATFORM_MACOS)

    std::uint64_t m_cpu_previous_total_ticks{0};
    std::uint64_t m_cpu_previous_idle_ticks{0};
    host_cpu_load_info_data_t m_cpu_info{};
    mach_msg_type_number_t m_count = HOST_CPU_LOAD_INFO_COUNT;

#endif
};


NS_ENIGMA_END


#endif // !ENIGMA_CPU_INFO_H


#if 0
#pragma once
#ifndef ENIGMA_CPU_INFO_H
#define ENIGMA_CPU_INFO_H

#undef ENIGMA_PLATFORM_WINDOWS
#define ENIGMA_PLATFORM_LINUX 1

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#if defined(ENIGMA_PLATFORM_WINDOWS)
// Windows Includes
#include <Windows.h>
#elif defined(ENIGMA_PLATFORM_LINUX)
// Linux Includes
#include <cstring>
#include <sys/times.h>
#include <sys/vtimes.h>
#elif defined(ENIGMA_PLATFORM_MACOS)
// MacOS Includes
#include <mach/mach_error.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/vm_map.h>
#endif

NS_ENIGMA_BEGIN
/*
*	CPUInfo class will gather informations about the CPU at runtime
*/
class CPUInfo
{
public: /* Constructors / Destructor */
	CPUInfo() noexcept
	{
#if defined(ENIGMA_PLATFORM_WINDOWS)
		::SYSTEM_INFO sys_info{};
		::FILETIME ftime{}, fsys{}, fuser{};

		::GetSystemInfo(&sys_info);
		m_num_processors = sys_info.dwNumberOfProcessors;

		::GetSystemTimeAsFileTime(&ftime);
		std::memcpy(&m_last_cpu, &ftime, sizeof(FILETIME));

		::GetProcessTimes(::GetCurrentProcess(), &ftime, &ftime, &fsys, &fuser);
		std::memcpy(&m_last_sys_cpu, &fsys, sizeof(FILETIME));
		std::memcpy(&m_last_user_cpu, &fuser, sizeof(FILETIME));
#elif defined(ENIGMA_PLATFORM_LINUX)
		std::FILE* file{};
		struct tms timeSample {};
		char line[128];

		lastCPU = times(&timeSample);
		lastSysCPU = timeSample.tms_stime;
		lastUserCPU = timeSample.tms_utime;

		file = fopen("/proc/cpuinfo", "r");
		numProcessors = 0;
		while (fgets(line, 128, file) != NULL) {
			if (strncmp(line, "processor", 9) == 0) numProcessors++;
		}
		fclose(file);
#endif

	}
	~CPUInfo() noexcept = default;

	ENIGMA_NON_COPYABLE(CPUInfo);
	ENIGMA_NON_MOVEABLE(CPUInfo);


public:
	/*
	*	Returns cpu usage (in percentage [0% -> 100%])
	*/
	float CPUInfo::GetCPUUsage() noexcept
	{
		float percentage{ 0.0f };

#if defined(ENIGMA_PLATFORM_WINDOWS)

		if (::GetSystemTimes(&m_idle_time, &m_kernel_time, &m_user_time))
		{
			const std::uint64_t idle_ticks = this->FileTimeToUInt64(m_idle_time);
			const std::uint64_t total_ticks = this->FileTimeToUInt64(m_kernel_time) + this->FileTimeToUInt64(m_user_time);
			percentage = this->CalculateCPULoad(idle_ticks, total_ticks);
		}
		else
			percentage = -100.0f;

#elif defined(ENIGMA_PLATFORM_LINUX)

		std::uint64_t total_user{}, total_user_low{}, total_sys{}, total_idle{}, total{};

		std::FILE* file = std::fopen("/proc/stat", "r");
		std::fscanf(file, "cpu %llu %llu %llu %llu",
			&total_user, &total_user_low,
			&total_sys, &total_idle);
		std::fclose(file);

		if (total_user < m_last_total_user || total_user_low < m_last_total_user_low ||
			total_sys < m_last_total_sys || total_idle < m_last_total_idle)
		{
			//Overflow detection. Just skip this value.
			percentage = -100.0f;
		}
		else
		{
			total = (total_user - m_last_total_user) + (total_user_low - m_last_total_user_low) + (total_sys - m_last_total_sys);
			percentage = static_cast<float>(total);
			total += (total_idle - m_last_total_idle);
			percentage /= total;
			percentage *= 100.0f;
		}

		m_last_total_user = total_user;
		m_last_total_user_low = total_user_low;
		m_last_total_sys = total_sys;
		m_last_total_idle = total_idle;

#elif defined(ENIGMA_PLATFORM_MACOS)
		if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&m_cpu_info, &m_count) == KERN_SUCCESS)
		{
			std::uint64_t total_ticks{ 0 };
			for (auto i = 0; i < CPU_STATE_MAX; ++i)
				total_ticks += m_cpu_info.cpu_ticks[i];
			const std::uint64_t idle_ticks = m_cpu_info.cpu_ticks[CPU_STATE_IDLE];
			percentage = this->CalculateCPULoad(idle_ticks, total_ticks);
		}
		else
			percentage = -100.0f;
#endif

		return percentage;
	}


	/*
	*	Returns cpu usage by current process (in percentage [0% -> 100%])
	*/
	float GetProcessCPUUsage() noexcept
	{
#if defined(ENIGMA_PLATFORM_WINDOWS)

		ULARGE_INTEGER now{}, sys{}, user{};

		::GetSystemTimeAsFileTime(&m_idle_time);
		std::memcpy(&now, &m_idle_time, sizeof(FILETIME));

		::GetProcessTimes(::GetCurrentProcess(), &m_idle_time, &m_idle_time, &m_kernel_time, &m_user_time);
		std::memcpy(&sys, &m_kernel_time, sizeof(FILETIME));
		std::memcpy(&user, &m_user_time, sizeof(FILETIME));

		float percent = static_cast<float>(sys.QuadPart - m_last_sys_cpu.QuadPart) + static_cast<float>(user.QuadPart - m_last_user_cpu.QuadPart);
		percent /= static_cast<float>(now.QuadPart - m_last_cpu.QuadPart);
		percent /= static_cast<float>(m_num_processors);
		
		m_last_cpu = now;
		m_last_user_cpu = user;
		m_last_sys_cpu = sys;

		return percent * 100.0f;

#elif defined(ENIGMA_PLATFORM_LINUX)
#elif defined(ENIGMA_PLATFORM_MACOS)
#else
		return 0.0f;
#endif
	}


private: /* Platform Functions */
#if defined(ENIGMA_PLATFORM_WINDOWS) || defined(ENIGMA_PLATFORM_MACOS)
		/*
		*	Calculates CPU Load percentage by idle and total ticks for (used for Windows & MacOS)
		*/
		float CalculateCPULoad(const std::uint64_t idle_ticks, const std::uint64_t total_ticks)
		{
			const std::uint64_t total_ticks_since_last_time = total_ticks - m_cpu_previous_total_ticks;
			const std::uint64_t idle_ticks_since_last_time = idle_ticks - m_cpu_previous_idle_ticks;

			const float rate = 1.0f - ((total_ticks_since_last_time > 0) ? static_cast<float>(idle_ticks_since_last_time) / total_ticks_since_last_time : 0.0f);

			m_cpu_previous_total_ticks = total_ticks;
			m_cpu_previous_idle_ticks = idle_ticks;

			return (rate * 100.0f);
		}
#endif


private: /* Platform Variables */
#if defined(ENIGMA_PLATFORM_WINDOWS)

	FILETIME m_idle_time{}, m_kernel_time{}, m_user_time{};
	ULARGE_INTEGER m_last_cpu{}, m_last_user_cpu{}, m_last_sys_cpu{};
	std::int32_t m_num_processors{};

	std::uint64_t m_cpu_previous_total_ticks{ 0 };
	std::uint64_t m_cpu_previous_idle_ticks{ 0 };

	std::uint64_t FileTimeToUInt64(const FILETIME& file_time)
	{
		return (static_cast<std::uint64_t>(file_time.dwHighDateTime) << 32) | static_cast<std::uint64_t>(file_time.dwLowDateTime);
	}

#elif defined(ENIGMA_PLATFORM_LINUX)

	std::uint64_t m_last_total_user{ 0 };
	std::uint64_t m_last_total_user_low{ 0 };
	std::uint64_t m_last_total_sys{ 0 };
	std::uint64_t m_last_total_idle{ 0 };

	std::int32_t m_num_processors{};

#elif defined(ENIGMA_PLATFORM_MACOS)

	std::uint64_t m_cpu_previous_total_ticks{ 0 };
	std::uint64_t m_cpu_previous_idle_ticks{ 0 };
	host_cpu_load_info_data_t m_cpu_info{};
	mach_msg_type_number_t m_count = HOST_CPU_LOAD_INFO_COUNT;

#endif
};


NS_ENIGMA_END


#endif // !ENIGMA_CPU_INFO_H
#endif
