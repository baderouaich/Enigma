#pragma once
#ifndef ENIGMA_RAM_INFO_H
#define ENIGMA_RAM_INFO_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#if defined(ENIGMA_PLATFORM_WINDOWS)
//https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-memorystatusex
#include <Windows.h>
#include <psapi.h>
typedef MEMORYSTATUSEX memory_status_t;
#elif defined(ENIGMA_PLATFORM_LINUX)
//https://man7.org/linux/man-pages/man2/sysinfo.2.html
#include <stdio.h> // fopen
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <unistd.h> // syscall
typedef struct sysinfo memory_status_t;
#elif defined(ENIGMA_PLATFORM_MACOS)
//https://stackoverflow.com/questions/5012886/determining-the-available-amount-of-ram-on-an-ios-device
#include <mach/mach.h> // mach_task_basic_info
#include <mach/mach_host.h>
#include <mach/vm_statistics.h>
typedef struct vm_statistics_data_t memory_status_t;
#else
#error "Unsupported Platform"
#endif

NS_ENIGMA_BEGIN
/**
*	RAMInfo class will gather information about the RAM at runtime
*/
class RAMInfo {
  public:
    RAMInfo() noexcept;
    ~RAMInfo() noexcept = default;

    ENIGMA_NON_COPYABLE(RAMInfo);
    ENIGMA_NON_MOVEABLE(RAMInfo);

  public:
    /**
    *	Updates the RAM status at runtime
    */
    void Update();

    /**
    *	Returns the total physical memory reserved by all processes (in bytes)
    */
    std::size_t GetUsedRAM() const noexcept;

    /**
    *	Returns the total physical memory used by this process (in bytes)
    */
    std::size_t GetProcessUsedRAM() const noexcept;

    /**
    *	Returns the total physical memory free (in bytes)
    */
    std::size_t GetFreeRAM() const noexcept;

    /**
    *	Returns the max available memory on the system (in bytes)
    */
    std::size_t GetAvailableRAM() const noexcept;

    /**
    *	Returns memory usage by all processes (in percentage [0% -> 100%])
    */
    float GetRAMUsage() noexcept;

    /**
    *	Returns memory usage by current process (in percentage [0% -> 100%])
    */
    float GetProcessRAMUsage() noexcept;


  private:
    memory_status_t m_memory_status{}; /**< memory_status_t will be MEMORYSTATUSEX in windows, sysinfo in linux, vm_statistics_data_t in macos */
};


NS_ENIGMA_END


#endif // !ENIGMA_RAM_INFO_H
