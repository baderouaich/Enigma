#pragma once
#ifndef ENIGMA_RAM_INFO_H
#define ENIGMA_RAM_INFO_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#if defined(ENIGMA_PLATFORM_WINDOWS)
	//https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-memorystatusex
	#include <Windows.h>
typedef MEMORYSTATUSEX memory_status_t;
#elif defined(ENIGMA_PLATFORM_LINUX)
	//https://man7.org/linux/man-pages/man2/sysinfo.2.html
	#include <sys/sysinfo.h>
typedef struct sysinfo memory_status_t;
#elif defined(ENIGMA_PLATFORM_MACOS)
	//https://stackoverflow.com/questions/5012886/determining-the-available-amount-of-ram-on-an-ios-device
	#include <mach/vm_statistics.h>
	#include <mach/mach.h>
	#include <mach/mach_host.h>
typedef struct vm_statistics_data_t memory_status_t;
#endif

NS_ENIGMA_BEGIN
/*
*	RAMInfo class will gather informations about the RAM usage at runtime
*/
class ENIGMA_API RAMInfo
{
public: /* Constructors / Destructor */
	RAMInfo() noexcept;
	~RAMInfo() noexcept = default;

	ENIGMA_NON_COPYABLE(RAMInfo);
	ENIGMA_NON_MOVEABLE(RAMInfo);

public:
	/*
	*	Updates the RAM status at runtime
	*/
	void Update();

	/*
	*	Returns the total physical memory reserved (in bytes)
	*/
	size_t GetUsedRAM() const noexcept;

	/*
	*	Returns the total physical memory free (in bytes)
	*/
	size_t GetFreeRAM() const noexcept;

	/*
	*	Returns the max available memory on the system (in bytes)
	*/
	size_t GetAvailableRAM() const noexcept;

	/*
	*	Returns memory usage (in percentage [0% -> 100%])
	*/
	f32 GetRAMUsage() noexcept;

private:
	/*
	*	Maps value from a range to another
	*	@param value: the incoming value to be converted
	*	@param start1: lower bound of the value's current range
	*	@param stop1: upper bound of the value's current range
	*	@param start2: lower bound of the value's target range
	*	@param stop2: upper bound of the value's target range
	*/
	template<typename T>
	constexpr inline const T Map(const T& value, const T& start1, const T& stop1, const T& start2, const T& stop2)
	{
		return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	}

private:
	 memory_status_t m_memory_status;
};


NS_ENIGMA_END


#endif // !ENIGMA_RAM_INFO_H
