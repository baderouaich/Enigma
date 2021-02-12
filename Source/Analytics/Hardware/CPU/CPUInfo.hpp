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
#elif defined(ENIGMA_PLATFORM_MACOS)
// MacOS Includes
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach_host.h>
#include <mach/vm_map.h>
#endif

NS_ENIGMA_BEGIN
/*
*	CPUInfo class will gather informations about the CPU at runtime
*/
class ENIGMA_API CPUInfo
{
public: /* Constructors / Destructor */
	CPUInfo() noexcept;
	~CPUInfo() noexcept = default;

	ENIGMA_NON_COPYABLE(CPUInfo);
	ENIGMA_NON_MOVEABLE(CPUInfo);


public:
	/*
	*	Returns cpu usage (in percentage [0% -> 100%])
	*/
	f32 GetCPUUsage() noexcept;



private: /* Platform Functions */
#if defined(ENIGMA_PLATFORM_WINDOWS) || defined(ENIGMA_PLATFORM_MACOS)
	/*
	*	Calculates CPU Load percentage by idle and total ticks for (used for Windows & MacOS)
	*/
	f32 CalculateCPULoad(const ui64& idle_ticks, const ui64& total_ticks)
	{
		const ui64 total_ticks_since_last_time = total_ticks - m_cpu_previous_total_ticks;
		const ui64 idle_ticks_since_last_time = idle_ticks - m_cpu_previous_idle_ticks;

		f32 ret = 1.0f - ((total_ticks_since_last_time > 0) ? ((f32)idle_ticks_since_last_time) / total_ticks_since_last_time : 0);

		m_cpu_previous_total_ticks = total_ticks;
		m_cpu_previous_idle_ticks = idle_ticks;

		return (ret * 100.0f);
	}
#endif



private: /* Platform Variables */
#if defined(ENIGMA_PLATFORM_WINDOWS)

	FILETIME m_idle_time{}, m_kernel_time{}, m_user_time{};
	ui64 m_cpu_previous_total_ticks{0};
	ui64 m_cpu_previous_idle_ticks{0};

	ui64 FileTimeToUInt64(const FILETIME& file_time)
	{
		return (static_cast<ui64>(file_time.dwHighDateTime) << 32) | static_cast<ui64>(file_time.dwLowDateTime);
	}

#elif defined(ENIGMA_PLATFORM_LINUX)

	ui64 m_last_total_user{0};
	ui64 m_last_total_user_low{0};
	ui64 m_last_total_sys{0};
	ui64 m_last_total_idle{0};

#elif defined(ENIGMA_PLATFORM_MACOS)

	ui64 m_cpu_previous_total_ticks{0};
	ui64 m_cpu_previous_idle_ticks{0};
	host_cpu_load_info_data_t m_cpu_info{};
	mach_msg_type_number_t m_count = HOST_CPU_LOAD_INFO_COUNT;

#endif
};


NS_ENIGMA_END


#endif // !ENIGMA_CPU_INFO_H
