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
  const int res = tinyfd_messageBox(
    m_title.c_str(),
    m_message.c_str(),
    Choice2Str[static_cast<std::size_t>(m_choice)].data(),
    Icon2Str[static_cast<std::size_t>(m_icon)].data(),
    0);

  if (Choice::Yes_No_Cancel == m_choice && res == 2) {
    // 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel
    // "yesnocancel" -> 2 (No from YesNoCancel), treat it as no here.
    return Action::No;
  }
  return static_cast<Action>(res);
}

NS_ENIGMA_END
