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
	static const String Get(const std::string_view& key);
	static void Set(const std::string_view& key, const std::string_view& value);
	static void Delete(const std::string_view& key);

public:
	ENIGMA_NON_COPYABLE(ENV);
	ENIGMA_NON_MOVEABLE(ENV);
};

NS_ENIGMA_END
#endif // !ENIGMA_CLIPBOARD_H
