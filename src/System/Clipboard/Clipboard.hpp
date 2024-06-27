#pragma once
#ifndef ENIGMA_CLIPBOARD_H
#define ENIGMA_CLIPBOARD_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN

/** Clipboard Manager static class */
class ENIGMA_API Clipboard final
{
	ENIGMA_STATIC_CLASS(Clipboard);
public:
	/** Get Clipboard text */
	static String Get();

	/** Set Clipboard text */
	static void Set(const String& buffer);
};

NS_ENIGMA_END
#endif // !ENIGMA_CLIPBOARD_H
