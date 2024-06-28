#include <pch.hpp>
#include "ENV.hpp"

NS_ENIGMA_BEGIN

void ENV::Set(const std::string& key, const std::string& value)
{
	bool success{ false };
#if defined(ENIGMA_PLATFORM_WINDOWS)
    success = ::SetEnvironmentVariableA(key.data(), value.data());
	if(!success)
		// Since ENV is used in the initialization of Logger, we cant use ENIGMA_ERROR here, printf will do.
		std::printf("%s: SetEnvironmentVariable failed (%lu)\n", ENIGMA_CURRENT_FUNCTION, ::GetLastError());
#else
	success = setenv(key.c_str(), value.c_str(), true) == 0; // https://pubs.opengroup.org/onlinepubs/009604499/functions/setenv.html
	if(!success)
    ENIGMA_ERROR("Failed to set env {}:{}", errno, std::strerror(errno));
#endif
}

std::string ENV::Get(const std::string& key)
{
	std::string value{};
#if defined(ENIGMA_PLATFORM_WINDOWS)
	constexpr std::size_t MAX_ENV_VALUE_SIZE = static_cast<std::size_t>(std::numeric_limits<std::int16_t>::max());
	value.resize(MAX_ENV_VALUE_SIZE, '\000');
	const DWORD value_size = GetEnvironmentVariableA(key.data(), value.data(), MAX_ENV_VALUE_SIZE);
	if (value_size)
		value.resize(static_cast<std::size_t>(value_size), '\000');
	else
		value.clear();
	if (value.empty())
		std::printf("%s: GetEnvironmentVariableA failed (%lu)\n", ENIGMA_CURRENT_FUNCTION, ::GetLastError());
#else
	value = strdup(getenv(key.c_str())); // do not return pointer to env, make a copy instead
	if (value.empty())
    ENIGMA_ERROR("Failed to get env {}:{}", errno, std::strerror(errno));
#endif

	return value;
}


void ENV::Delete(const std::string& key)
{
	bool success{ false };
#if defined(ENIGMA_PLATFORM_WINDOWS)
	success = ::SetEnvironmentVariableA(key.data(), nullptr);
	if (!success)
		std::printf("%s: SetEnvironmentVariable failed (%lu)\n", ENIGMA_CURRENT_FUNCTION, ::GetLastError());
#else
	success = unsetenv(key.c_str()) == 0;
	if (!success)
    ENIGMA_ERROR("Failed to unset env {}:{}", errno, std::strerror(errno));
#endif
}
NS_ENIGMA_END

