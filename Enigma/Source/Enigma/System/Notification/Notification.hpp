#pragma once
#ifndef ENIGMA_NOTIFICATION_H
#define ENIGMA_NOTIFICATION_H

#include <Enigma/Core/Core.hpp>
#include <portable-file-dialogs.h>

NS_ENIGMA_BEGIN
/*
*	System Notification
*/
class ENIGMA_API Notification
{
public: /* Enums */
	/*
	* Notification Icon type
	*/
	enum class Icon : ui8
	{
		Info    = static_cast<ui8>(pfd::icon::info),
		Warning = static_cast<ui8>(pfd::icon::warning),
		Error   = static_cast<ui8>(pfd::icon::error)
	};

public: /* Constructors / Destructor */
	/*
	*	@param title: title of the notification
	*	@param message: description of the notification
	*	@param icon: Icon type of the notification
	*/
	Notification(const String& title, const String& message, Icon icon = Icon::Info);
	~Notification();

public: /* Accessors */
	/*
	*	Spawns System Notification
	*/
	void Show() const noexcept;

private:
	const String m_title;
	const String m_message;
	const Icon m_icon;
};

NS_ENIGMA_END

#endif // !ENIGMA_NOTIFICATION_H
