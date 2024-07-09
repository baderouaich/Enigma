#include <pch.hpp>
#include "CPUInfo.hpp"


NS_ENIGMA_BEGIN

CPUInfo::CPUInfo() noexcept {
}

float CPUInfo::GetCPUUsage() noexcept {
  float percentage{0.0f};

#if defined(ENIGMA_PLATFORM_WINDOWS)

  if (::GetSystemTimes(&m_idle_time, &m_kernel_time, &m_user_time)) {
    const std::uint64_t idle_ticks = this->FileTimeToUInt64(m_idle_time);
    const std::uint64_t total_ticks = this->FileTimeToUInt64(m_kernel_time) + this->FileTimeToUInt64(m_user_time);
    percentage = this->CalculateCPULoad(idle_ticks, total_ticks);
  } else
    percentage = -100.0f;

#elif defined(ENIGMA_PLATFORM_LINUX)

  std::uint64_t total_user{}, total_user_low{}, total_sys{}, total_idle{}, total{};

  std::FILE *file = std::fopen("/proc/stat", "r");
  std::fscanf(file, "cpu %zu %zu %zu %zu",
              &total_user, &total_user_low,
              &total_sys, &total_idle);
  std::fclose(file);

  if (total_user < m_last_total_user || total_user_low < m_last_total_user_low ||
      total_sys < m_last_total_sys || total_idle < m_last_total_idle) {
    //Overflow detection. Just skip this value.
    percentage = -100.0f;
  } else {
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
  if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t) &m_cpu_info, &m_count) == KERN_SUCCESS) {
    std::uint64_t total_ticks{0};
    for (auto i = 0; i < CPU_STATE_MAX; ++i)
      total_ticks += m_cpu_info.cpu_ticks[i];
    const std::uint64_t idle_ticks = m_cpu_info.cpu_ticks[CPU_STATE_IDLE];
    percentage = this->CalculateCPULoad(idle_ticks, total_ticks);
  } else
    percentage = -100.0f;
#endif

  return percentage;
}


float CPUInfo::GetProcessCPUUsage() noexcept {
  float percentage = 0.0f;

#if defined(ENIGMA_PLATFORM_WINDOWS)

  static ::SYSTEM_INFO sys_info{};
  ::GetSystemInfo(&sys_info);
  static std::int32_t num_processors = sys_info.dwNumberOfProcessors;
  static ::ULARGE_INTEGER last_sys_cpu{}, last_cpu{}, last_user_cpu{};

  ::ULARGE_INTEGER now{}, sys{}, user{};

  ::GetSystemTimeAsFileTime(&m_idle_time);
  std::memcpy(&now, &m_idle_time, sizeof(::FILETIME));

  ::GetProcessTimes(::GetCurrentProcess(), &m_idle_time, &m_idle_time, &m_kernel_time, &m_user_time);
  std::memcpy(&sys, &m_kernel_time, sizeof(::FILETIME));
  std::memcpy(&user, &m_user_time, sizeof(::FILETIME));

  percentage = static_cast<float>(sys.QuadPart - last_sys_cpu.QuadPart) + static_cast<float>(user.QuadPart - last_user_cpu.QuadPart);
  percentage /= static_cast<float>(now.QuadPart - last_cpu.QuadPart);
  percentage /= static_cast<float>(num_processors);

  last_cpu = now;
  last_user_cpu = user;
  last_sys_cpu = sys;

  percentage *= 100.0f;

#elif defined(ENIGMA_PLATFORM_LINUX)
  static clock_t lastCPU, lastSysCPU, lastUserCPU;
  static int numProcessors;
  static bool initialized = false;

  static auto init = [&]() {
    std::FILE *file{};
    struct tms timeSample {};
    char line[128]{};

    lastCPU = times(&timeSample);
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    file = std::fopen("/proc/cpuinfo", "r");
    numProcessors = 0;
    while (std::fgets(line, 128, file) != nullptr) {
      if (std::strncmp(line, "processor", 9) == 0)
        numProcessors++;
    }
    std::fclose(file);
    initialized = true;
  };

  if (!initialized)
    init();

  struct tms timeSample {};
  clock_t now{};

  now = times(&timeSample);
  if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
      timeSample.tms_utime < lastUserCPU) {
    //Overflow detection. Just skip this value.
    percentage = -100.0f;
  } else {
    percentage = (timeSample.tms_stime - lastSysCPU) + (timeSample.tms_utime - lastUserCPU);
    percentage /= (now - lastCPU);
    percentage /= numProcessors;
    percentage *= 100.0f;
  }
  lastCPU = now;
  lastSysCPU = timeSample.tms_stime;
  lastUserCPU = timeSample.tms_utime;

#elif defined(ENIGMA_PLATFORM_MACOS)
#error TODO N/I yet
#endif

  return percentage;
}

NS_ENIGMA_END
