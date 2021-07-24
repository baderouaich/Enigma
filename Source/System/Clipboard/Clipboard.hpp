#pragma once
#ifndef ENIGMA_CLIPBOARD_H
#define ENIGMA_CLIPBOARD_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN

class ENIGMA_API Clipboard final
{
	ENIGMA_STATIC_CLASS(Clipboard);
public:
	static String Get();
	static void Set(const String& buffer);
};

NS_ENIGMA_END
#endif // !ENIGMA_CLIPBOARD_H
