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
class ENV final {
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

    /**
    *	Unset process runtime environment variable
    */
    static void Delete(const std::string& key);
};

NS_ENIGMA_END
#endif // !ENIGMA_CLIPBOARD_H
