#include <pch.hpp>
#include "Notification.hpp"

NS_ENIGMA_BEGIN

Notification::Notification(const std::string& title, const std::string& message, Icon icon)
  : m_title(title),
    m_message(message),
    m_icon(icon) {
}

void Notification::Show() const {
  // @todo: FIX undefined reference to `tinyfd_notifyPopup'
#if ENIGMA_ENABLE_NOTIFICATIONS
  std::ignore = tinyfd_notifyPopup(m_title.c_str(),
                                   m_message.c_str(),
                                   Icon2Str[static_cast<std::size_t>(m_icon)].data());
#endif
}

NS_ENIGMA_END
