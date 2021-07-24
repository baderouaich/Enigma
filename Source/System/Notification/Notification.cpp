#include <pch.hpp>
#include "Notification.hpp"

NS_ENIGMA_BEGIN

Notification::Notification(const String& title, const String& message, Icon icon)
	:
	m_title(title),
	m_message(message),
	m_icon(icon)
{
}


void Notification::Show() const
{
	[[maybe_unused]] auto r = pfd::notify(
		m_title,
		m_message,
		static_cast<pfd::icon>(m_icon)
	);
}


NS_ENIGMA_END
