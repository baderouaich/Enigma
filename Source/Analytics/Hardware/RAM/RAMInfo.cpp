#include <pch.hpp>
#include "RAMInfo.hpp"

NS_ENIGMA_BEGIN

RAMInfo::RAMInfo() noexcept
{
#if defined(ENIGMA_PLATFORM_WINDOWS)
	m_memory_status.dwLength = sizeof(m_memory_status);
#elif defined(ENIGMA_PLATFORM_LINUX)
#elif defined(ENIGMA_PLATFORM_MACOS)
#endif
}

void RAMInfo::Update()
{
	static bool success = false;

#if defined(ENIGMA_PLATFORM_WINDOWS)
	success = GlobalMemoryStatusEx(&m_memory_status);
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

size_t RAMInfo::GetFreeRAM() const noexcept
{
#if defined(ENIGMA_PLATFORM_WINDOWS)
	return m_memory_status.ullAvailPhys;
#elif defined(ENIGMA_PLATFORM_LINUX)
	return m_memory_status.freeram;
#elif defined(ENIGMA_PLATFORM_MACOS)
	return m_memory_status.free_count;
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
#endif
}

f32 RAMInfo::GetRAMUsage() noexcept
{
	f32 percentage = Map(
		static_cast<f32>(this->GetUsedRAM()), // value
		0.0f, static_cast<f32>(this->GetAvailableRAM()), // from 0 to whatever ram available
		0.0f, 100.0f // to 0 to 100 percentage range
	);
	return percentage;
}

NS_ENIGMA_END
