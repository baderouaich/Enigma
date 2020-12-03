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
	pfd::notify r = pfd::notify(
		m_title,
		m_message,
		static_cast<pfd::icon>(m_icon)
	);
	UNUSED(r);
}

Notification::~Notification() noexcept
{
}

NS_ENIGMA_END
