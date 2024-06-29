#pragma once
#ifndef ENIGMA_MESSAGE_BOX_H
#define ENIGMA_MESSAGE_BOX_H

#include <Core/Core.hpp>
#include <portable-file-dialogs.h>

/** Avoid mismatch with windows's MessageBox macro */
#if defined(ENIGMA_PLATFORM_WINDOWS)
#undef MessageBox
#undef ERROR
#undef IGNORE
#endif

NS_ENIGMA_BEGIN

class MessageBox {
  public: /* Enums */
    /**
	* Message box Icon type
	*/
    enum class Icon : std::uint8_t {
      Info = static_cast<std::uint8_t>(pfd::icon::info),
      Warning = static_cast<std::uint8_t>(pfd::icon::warning),
      Error = static_cast<std::uint8_t>(pfd::icon::error),
      Question = static_cast<std::uint8_t>(pfd::icon::question)
    };

    /**
	*	Message box button layout choices
	*/
    enum class Choice : std::uint8_t {
      Ok = static_cast<std::uint8_t>(pfd::choice::ok),
      Ok_Cancel = static_cast<std::uint8_t>(pfd::choice::ok_cancel),
      Yes_No = static_cast<std::uint8_t>(pfd::choice::yes_no),
      Yes_No_Cancel = static_cast<std::uint8_t>(pfd::choice::yes_no_cancel),
      Retry_Cancel = static_cast<std::uint8_t>(pfd::choice::retry_cancel),
      Abort_Retry_Ignore = static_cast<std::uint8_t>(pfd::choice::abort_retry_ignore)
    };

    /**
	*	User Performed Action Result
	*/
    enum class Action : std::int8_t {
      Cancel = static_cast<std::int8_t>(pfd::button::cancel),
      Ok = static_cast<std::int8_t>(pfd::button::ok),
      Yes = static_cast<std::int8_t>(pfd::button::yes),
      No = static_cast<std::int8_t>(pfd::button::no),
      Abort = static_cast<std::int8_t>(pfd::button::abort),
      Retry = static_cast<std::int8_t>(pfd::button::retry),
      Ignore = static_cast<std::int8_t>(pfd::button::ignore)
    };

  public: /* Constructors / Destructor */
    /**
	*	Constructor
	*	@param title: title of the message box
	*	@param message: description of the message box
	*	@param icon: Icon type of the message box Info, Warning, Error or Question
	*	@param choice: buttons layout of the message box
	*/
    MessageBox(const std::string& title, const std::string& message,
               Icon icon = Icon::Info,
               Choice choice = Choice::Ok_Cancel);
    ~MessageBox() noexcept = default;

  public: /* Accessors */
    /**
	*	Spawns MessageBox and returns user Action
	*/
    MessageBox::Action Show() const;

  private:
    std::string m_title;   /*< MessageBox title */
    std::string m_message; /*< MessageBox message */
    Icon m_icon;           /*< MessageBox icon */
    Choice m_choice;       /*< MessageBox choice/button template */
};

NS_ENIGMA_END

#endif // !ENIGMA_MESSAGE_BOX_H
