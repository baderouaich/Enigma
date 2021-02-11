#include "pch.hpp"
#include "CPUInfo.hpp"


NS_ENIGMA_BEGIN

CPUInfo::CPUInfo() noexcept
{
}

f32 CPUInfo::GetCPUUsage() noexcept
{
	f32 percentage{0.0f};

#if defined(ENIGMA_PLATFORM_WINDOWS)

	if (::GetSystemTimes(&m_idle_time, &m_kernel_time, &m_user_time))
	{
		const ui64 idle_ticks = this->FileTimeToUInt64(m_idle_time);
		const ui64 total_ticks = this->FileTimeToUInt64(m_kernel_time) + this->FileTimeToUInt64(m_user_time);
		percentage = this->CalculateCPULoad(idle_ticks, total_ticks);
	}
	else
		percentage = -100.0f;

#elif defined(ENIGMA_PLATFORM_LINUX)

	ui64 total_user{}, total_user_low{}, total_sys{}, total_idle{}, total{};

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
		total = (total_user - m_last_total_user) + (total_user_low - m_last_total_user_low) + (total_sys - m_last_total_sys) + (total_idle - m_last_total_idle);
		percentage = (percentage / total) * 100.0f;
	}

	m_last_total_user = total_user;
	m_last_total_user_low = total_user_low;
	m_last_total_sys = total_sys;
	m_last_total_idle = total_idle;

#elif defined(ENIGMA_PLATFORM_MACOS)

	//TODO

#endif

	return percentage;
}

NS_ENIGMA_END
