#pragma once
#ifndef ENIGMA_ENV_H
#define ENIGMA_ENV_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
/*
*	System's Environment Variable Manager
*	The environment variables are set within the context of your program.
*	When your program exits, you're back in the context from where your program was started.
*/
class ENIGMA_API ENV
{
public:
	/*
	*	Get process runtime environment variable value
	*/
	static const String Get(const std::string_view& key);

	/*
	*	Set process runtime environment variable
	*/
	static void Set(const std::string_view& key, const std::string_view& value);

#if 0
	/*
	*	Set machine permanent environment variable
	*/
	static void SetPermanent(const std::string_view& key, const std::string_view& value)
	{
		using std::operator""s;
		String cmd{};
#if defined(ENIGMA_PLATFORM_WINDOWS)
		cmd = "setx "s + String(key) + " \"" + String(value) + "\" /m";
#elif  defined(ENIGMA_PLATFORM_LINUX) || defined(ENIGMA_PLATFORM_MACOS)
		cmd = "export "s + String(key) + "=\""s + String(value) + '\"';
#endif
		const i32 status = std::system(cmd.c_str());
		(void)std::printf("CMD: %s\nStatus: %d\n\n", cmd.c_str(), status);

		std::printf(Get(key).c_str());
	}
#endif

	/*
	*	Unset process runtime environment variable
	*/
	static void Delete(const std::string_view& key);

public:
	ENIGMA_NON_COPYABLE(ENV);
	ENIGMA_NON_MOVEABLE(ENV);
};

NS_ENIGMA_END
#endif // !ENIGMA_CLIPBOARD_H
