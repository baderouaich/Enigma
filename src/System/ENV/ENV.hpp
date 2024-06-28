#pragma once
#ifndef ENIGMA_ENV_H
#define ENIGMA_ENV_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
/**
*	System's Environment Variable Manager
*	The environment variables are set within the context of your program.
*	When your program exits, you're back in the context from where your program was started.
*/
class ENIGMA_API ENV final
{
	ENIGMA_STATIC_CLASS(ENV);
public:
	/**
	*	Get process runtime environment variable value
	*/
	static std::string Get(const std::string& key);

	/**
	*	Set process runtime environment variable
	*/
	static void Set(const std::string& key, const std::string& value);

#if 0
	/**
	*	Set machine permanent environment variable
	*/
	static void SetPermanent(const std::string_view& key, const std::string_view& value)
	{
		using std::operator""s;
		std::string cmd{};
#if defined(ENIGMA_PLATFORM_WINDOWS)
		cmd = "setx "s + std::string(key) + " \"" + std::string(value) + "\" /m";
#elif  defined(ENIGMA_PLATFORM_LINUX) || defined(ENIGMA_PLATFORM_MACOS)
		cmd = "export "s + std::string(key) + "=\""s + std::string(value) + '\"';
#endif
		const std::int32_t status = std::system(cmd.c_str());
		(void)std::printf("CMD: %s\nStatus: %d\n\n", cmd.c_str(), status);

		std::printf(Get(key).c_str());
	}
#endif

	/**
	*	Unset process runtime environment variable
	*/
	static void Delete(const std::string& key);

};

NS_ENIGMA_END
#endif // !ENIGMA_CLIPBOARD_H
