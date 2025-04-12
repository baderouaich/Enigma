#pragma once
#ifndef ENIGMA_MESSAGE_BOX_H
#define ENIGMA_MESSAGE_BOX_H

#include <Core/Core.hpp>

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
    Info = 0,
    Warning,
    Error,
    Question,
  };
  static constexpr std::string_view Icon2Str[4] = {
    [static_cast<std::size_t>(Icon::Info)] = "info",
    [static_cast<std::size_t>(Icon::Warning)] = "warning",
    [static_cast<std::size_t>(Icon::Error)] = "error",
    [static_cast<std::size_t>(Icon::Question)] = "question"};
  /**
	  *	Message box button layout choices
	  */
  enum class Choice : std::uint8_t {
    Ok = 0,
    Ok_Cancel,
    Yes_No,
    Yes_No_Cancel
  };
  static constexpr std::string_view Choice2Str[4] = {
    [static_cast<std::size_t>(Choice::Ok)] = "ok",
    [static_cast<std::size_t>(Choice::Ok_Cancel)] = "okcancel",
    [static_cast<std::size_t>(Choice::Yes_No)] = "yesno",
    [static_cast<std::size_t>(Choice::Yes_No_Cancel)] = "yesnocancel"};

  /**
	*	User Performed Action Result
	*/
  enum class Action : std::int8_t {
    Cancel = 0,
    No = Cancel,
    Ok = 1,
    Yes = Ok
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
  [[nodiscard]] MessageBox::Action Show() const;

private:
  std::string m_title;   /*< MessageBox title */
  std::string m_message; /*< MessageBox message */
  Icon m_icon;           /*< MessageBox icon */
  Choice m_choice;       /*< MessageBox choice/button template */
};

NS_ENIGMA_END

#endif // !ENIGMA_MESSAGE_BOX_H
