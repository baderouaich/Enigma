#pragma once
#ifndef ENIGMA_NOTIFICATION_H
#define ENIGMA_NOTIFICATION_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
/**
*	System Notification
*/
class Notification {
public: /* Enums */
  /**
    * Notification Icon type
    */
  enum class Icon : std::uint8_t {
    Info = 0,
    Warning,
    Error
  };
  static constexpr std::string_view Icon2Str[3] = {
    [static_cast<std::size_t>(Icon::Info)] = "info",
    [static_cast<std::size_t>(Icon::Warning)] = "warning",
    [static_cast<std::size_t>(Icon::Error)] = "error"
  };

public:
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
  std::string m_title;   /*< Notification title */
  std::string m_message; /*< Notification message */
  Icon m_icon;           /*< Notification icon (Info, Error or Warning) */
};

NS_ENIGMA_END

#endif // !ENIGMA_NOTIFICATION_H
