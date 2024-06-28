#pragma once
#ifndef ENIGMA_NOTIFICATION_H
#define ENIGMA_NOTIFICATION_H

#include <Core/Core.hpp>
#include <portable-file-dialogs.h>

NS_ENIGMA_BEGIN
/**
*	System Notification
*/
class ENIGMA_API Notification
{
public: /* Enums */
	/**
	* Notification Icon type
	*/
	enum class Icon : std::uint8_t
	{
		Info    = static_cast<std::uint8_t>(pfd::icon::info),
		Warning = static_cast<std::uint8_t>(pfd::icon::warning),
		Error   = static_cast<std::uint8_t>(pfd::icon::error)
	};

public: /* Constructors / Destructor */
	/** Constructor
	*	@param title: title of the notification
	*	@param message: description of the notification
	*	@param icon: Icon type of the notification
	*/
	Notification(const std::string& title, const std::string& message, Icon icon = Icon::Info);
	~Notification() noexcept = default;

public: /* Accessors */
	/**
	*	Spawns System Notification
	*/
	void Show() const;

private:
	const std::string m_title;	/*< Notification title */
	const std::string m_message; /*< Notification message */
	const Icon m_icon;		/*< Notification icon (Info, Error or Warning) */
};

NS_ENIGMA_END

#endif // !ENIGMA_NOTIFICATION_H
