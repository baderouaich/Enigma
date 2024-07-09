#include <pch.hpp>
#include "MessageBox.hpp"

NS_ENIGMA_BEGIN

MessageBox::MessageBox(const std::string& title, const std::string& message, Icon icon, Choice choice)
    : m_title(title),
      m_message(message),
      m_icon(icon),
      m_choice(choice) {
}

MessageBox::Action MessageBox::Show() const {
  //Spawn pfd dialog & get performed user action
  auto result = pfd::message(
                  m_title,
                  m_message,
                  static_cast<pfd::choice>(m_choice),
                  static_cast<pfd::icon>(m_icon))
                  .result();
  return static_cast<MessageBox::Action>(result);

#if 0 
	// asynchrounous
	auto msg = pfd::message(
		m_title,
		m_message,
		static_cast<pfd::choice>(m_choice),
		static_cast<pfd::icon>(m_icon));
	while (!msg.ready(1000))
	{
		ENIGMA_TRACE("MessageBox: Waited 1 second for user input");
	}
	auto result = msg.result();
	return static_cast<MessageBox::Action>(result);
#endif
}

NS_ENIGMA_END
