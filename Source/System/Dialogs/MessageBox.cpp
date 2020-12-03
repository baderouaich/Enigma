#include <pch.hpp>
#include "MessageBox.hpp"

NS_ENIGMA_BEGIN

MessageBox::MessageBox(const String& title, const String& message, Icon icon, Choice choice)
	:
	m_title(title),
	m_message(message),
	m_icon(icon),
	m_choice(choice)
{
}

MessageBox::Action MessageBox::Show() const
{
	//Spawn pfd dialog & get performed user action
	auto result = pfd::message(
		m_title, 
		m_message,
		static_cast<pfd::choice>(m_choice),
		static_cast<pfd::icon>(m_icon))
		.result();
	
	return static_cast<MessageBox::Action>(result);
}

MessageBox::~MessageBox() noexcept
{
}

NS_ENIGMA_END

