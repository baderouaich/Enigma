#pragma once
#ifndef ENIGMA_CPU_INFO_H
#define ENIGMA_CPU_INFO_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <numeric>	// std::accumulate

#if defined(ENIGMA_PLATFORM_WINDOWS)
#include <Windows.h>
#elif defined(ENIGMA_PLATFORM_LINUX)

typedef struct sysinfo memory_status_t;
#elif defined(ENIGMA_PLATFORM_MACOS)
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


private:
#if defined(ENIGMA_PLATFORM_WINDOWS)

	FILETIME m_idle_time{}, m_kernel_time{}, m_user_time{};
	ui64 m_cpu_previous_total_ticks{0};
	ui64 m_cpu_previous_idle_ticks{0};

	f32 CalculateCPULoad(const ui64& idle_ticks, const ui64& total_ticks)
	{
		const ui64 total_ticks_since_last_time = total_ticks - m_cpu_previous_total_ticks;
		const ui64 idle_ticks_since_last_time = idle_ticks - m_cpu_previous_idle_ticks;

		f32 ret = 1.0f - ((total_ticks_since_last_time > 0) ? ((f32)idle_ticks_since_last_time) / total_ticks_since_last_time : 0);

		m_cpu_previous_total_ticks = total_ticks;
		m_cpu_previous_idle_ticks = idle_ticks;

		return (ret * 100.0f);
	}
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

	//TODO

#endif
};


NS_ENIGMA_END


#endif // !ENIGMA_CPU_INFO_H
