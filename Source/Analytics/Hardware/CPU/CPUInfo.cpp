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
		if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&m_cpu_info, &m_count) == KERN_SUCCESS)
		{
			ui64 total_ticks{0};
			for (auto i = 0; i < CPU_STATE_MAX; ++i) 
				total_ticks += m_cpu_info.cpu_ticks[i];
			const ui64 idle_ticks = m_cpu_info.cpu_ticks[CPU_STATE_IDLE];
			percentage = this->CalculateCPULoad(idle_ticks, total_ticks);
		}
		else
			percentage = -100.0f;
#endif

	return percentage;
}

NS_ENIGMA_END
