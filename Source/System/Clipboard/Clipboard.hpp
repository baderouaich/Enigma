#ifndef ENIGMA_CLIPBOARD_H
#define ENIGMA_CLIPBOARD_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
class Application;
class ENIGMA_API Clipboard
{
public:
	static const String Get();
	static void Set(const String& buffer);

public:
	ENIGMA_NON_COPYABLE(Clipboard);
	ENIGMA_NON_MOVEABLE(Clipboard);
};
NS_ENIGMA_END
#endif // !ENIGMA_CLIPBOARD_H
