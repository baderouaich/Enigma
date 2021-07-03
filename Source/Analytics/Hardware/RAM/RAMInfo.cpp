#include <pch.hpp>
#include "RAMInfo.hpp"

NS_ENIGMA_BEGIN

RAMInfo::RAMInfo() noexcept
{
#if defined(ENIGMA_PLATFORM_WINDOWS)
	m_memory_status.dwLength = sizeof(m_memory_status);
#elif defined(ENIGMA_PLATFORM_LINUX)
#elif defined(ENIGMA_PLATFORM_MACOS)
#else
#endif
}

void RAMInfo::Update()
{
	static bool success = false;

#if defined(ENIGMA_PLATFORM_WINDOWS)
	success = ::GlobalMemoryStatusEx(&m_memory_status);
#elif defined(ENIGMA_PLATFORM_LINUX)
	success = sysinfo(&m_memory_status) == 0;
#elif defined(ENIGMA_PLATFORM_MACOS)
	static mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
	success = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&m_memory_status, &count) == KERN_SUCCESS;
#endif

	ENIGMA_ASSERT(success, "Failed to Update RAM Info");
}

size_t RAMInfo::GetUsedRAM() const noexcept
{
	return this->GetAvailableRAM() - this->GetFreeRAM();
}

size_t RAMInfo::GetProcessUsedRAM() const noexcept
{
#if defined(ENIGMA_PLATFORM_WINDOWS)

	::PROCESS_MEMORY_COUNTERS pmc{ 0 };
	const bool success = !!::GetProcessMemoryInfo(::GetCurrentProcess(), &pmc, sizeof(pmc));
	ENIGMA_ASSERT(success, "Failed to get process used ram");
	return pmc.WorkingSetSize;


#elif defined(ENIGMA_PLATFORM_LINUX)

	std::FILE* file = std::fopen("/proc/self/statm", "r");
	ENIGMA_ASSERT_OR_RETURN(file, "Failed to open file /proc/self/statm", 0);
	long pages{ 0 };
	i32 items = std::fscanf(file, "%*s%ld", &pages);
	std::fclose(file);
	return  items == 1 ? size_t(pages * sysconf(_SC_PAGESIZE)) : 0;

#elif defined(ENIGMA_PLATFORM_MACOS)

	#if defined(MACH_TASK_BASIC_INFO)
		mach_task_basic_info info{};
		mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;

		const i32 result = task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount);
	#else
		task_basic_info info{};
		mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;

		const i32 result = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount);
	#endif // defined(MACH_TASK_BASIC_INFO)
	if (result != KERN_SUCCESS)
	{
		return 0;
	}
	else
		return info.resident_size;

#else
	return 0;
#endif
}

size_t RAMInfo::GetFreeRAM() const noexcept
{
#if defined(ENIGMA_PLATFORM_WINDOWS)
	return m_memory_status.ullAvailPhys;
#elif defined(ENIGMA_PLATFORM_LINUX)
	return m_memory_status.freeram;
#elif defined(ENIGMA_PLATFORM_MACOS)
	return m_memory_status.free_count;
#else
	return 0;
#endif
}

size_t RAMInfo::GetAvailableRAM() const noexcept
{	
#if defined(ENIGMA_PLATFORM_WINDOWS)
	return m_memory_status.ullTotalPhys;
#elif defined(ENIGMA_PLATFORM_LINUX)
	return m_memory_status.totalram;
#elif defined(ENIGMA_PLATFORM_MACOS)
	return m_memory_status.wire_count + m_memory_status.active_count + m_memory_status.inactive_count + m_memory_status.free_count;
#else
	return 0;
#endif
}

f32 RAMInfo::GetRAMUsage() noexcept
{
	f32 percentage = MathUtils::Map(
		static_cast<f32>(this->GetUsedRAM()), // value
		0.0f, static_cast<f32>(this->GetAvailableRAM()), // from 0 to whatever ram available
		0.0f, 100.0f // to 0 to 100 percentage range
	);
	return percentage;
}

f32 RAMInfo::GetProcessRAMUsage() noexcept
{
	f32 percentage = MathUtils::Map(
		static_cast<f32>(this->GetProcessUsedRAM()), // value
		0.0f, static_cast<f32>(this->GetAvailableRAM()), // from 0 to whatever ram available
		0.0f, 100.0f // to 0 to 100 percentage range
	);
	return percentage;
}

NS_ENIGMA_END
