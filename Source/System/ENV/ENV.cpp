#include <pch.hpp>
#include "ENV.hpp"

NS_ENIGMA_BEGIN

void ENV::Set(const std::string_view& key, const std::string_view& value)
{
	bool success{ false };
#if defined(ENIGMA_PLATFORM_WINDOWS)
    success = ::SetEnvironmentVariableA(key.data(), value.data());
	if(!success)
		// Since ENV is used in the initialization of Logger, we cant use ENIGMA_ERROR here, printf will do.
		std::printf("%s: SetEnvironmentVariable failed (%d)\n", ENIGMA_CURRENT_FUNCTION, ::GetLastError());
#else
	success = setenv(key.data(), value.data(), true) == 0; // https://pubs.opengroup.org/onlinepubs/009604499/functions/setenv.html
	if(!success)
		std::printf("%s: setenv failed (%d)\n", ENIGMA_CURRENT_FUNCTION, errno);
#endif
}

const String ENV::Get(const std::string_view& key)
{
	String value{};
#if defined(ENIGMA_PLATFORM_WINDOWS)
	constexpr size_t MAX_ENV_VALUE_SIZE = static_cast<size_t>(std::numeric_limits<i16>::max());
	value.resize(MAX_ENV_VALUE_SIZE, '\000');
	const DWORD value_size = GetEnvironmentVariableA(key.data(), value.data(), MAX_ENV_VALUE_SIZE);
	if (value_size)
		value.resize(static_cast<size_t>(value_size), '\000');
	else
		value.clear();
	if (value.empty())
		std::printf("%s: GetEnvironmentVariableA failed (%d)\n", ENIGMA_CURRENT_FUNCTION, ::GetLastError());
#else
	value = strdup(getenv(key.data())); // do not return pointer to env, make a copy instead
	if (value.empty())
		std::printf("%s: getenv failed (%d)\n", ENIGMA_CURRENT_FUNCTION, errno);
#endif

	return value;
}


void ENV::Delete(const std::string_view& key)
{
	bool success{ false };
#if defined(ENIGMA_PLATFORM_WINDOWS)
	success = ::SetEnvironmentVariableA(key.data(), nullptr);
	if (!success)
		std::printf("%s: SetEnvironmentVariable failed (%d)\n", ENIGMA_CURRENT_FUNCTION, ::GetLastError());
#else
	success = unsetenv(key.data()) == 0;
	if (!success)
		std::printf("%s: unsetenv failed (%d)\n", ENIGMA_CURRENT_FUNCTION, errno);
#endif
}
NS_ENIGMA_END

