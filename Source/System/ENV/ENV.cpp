#include <pch.hpp>
#include "ENV.hpp"
#include <stdlib.h>
NS_ENIGMA_BEGIN

void ENV::Set(const std::string_view& key, const std::string_view& value)
{
	bool success{ false };
#if defined(ENIGMA_PLATFORM_WINDOWS)
    success = ::SetEnvironmentVariableA(key.data(), value.data());
#else
	success = std::setenv(key.data(), value.data(), true) == 0; // https://pubs.opengroup.org/onlinepubs/009604499/functions/setenv.html
#endif
	if(!success)
		// Since ENV is used in the initialization of Logger, we cant use ENIGMA_ERROR here, printf will do.
		std::printf("Couldn't Set Environment Variable %s to %s\n", key.data(), value.data());
}

const String ENV::Get(const std::string_view& key)
{
	String value{};
#if defined(ENIGMA_PLATFORM_WINDOWS)
	constexpr size_t MAX_ENV_VALUE_SIZE = 32767;
	value.resize(MAX_ENV_VALUE_SIZE, '\000');
	const DWORD value_size = GetEnvironmentVariableA(key.data(), value.data(), MAX_ENV_VALUE_SIZE);
	if (value_size)
		value.resize(value_size);
	else
		value.clear();
#else
	value = std::strdup(std::getenv(key.data())); // shall not return pointer to env, make a copy instead
#endif
	if (value.empty())
		std::printf("Couldn't Get Environment Variable %s\n", key.data());

	return value;
}


void ENV::Delete(const std::string_view& key)
{
	bool success{ false };
#if defined(ENIGMA_PLATFORM_WINDOWS)
	success = ::SetEnvironmentVariableA(key.data(), nullptr);
#else
	success = std::unsetenv(key.data()) == 0;
#endif
	if (!success)
		std::printf("Couldn't Delete Environment Variable %s\n", key.data());
}
NS_ENIGMA_END

