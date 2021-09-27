#pragma once
#ifndef ENIGMA_MESSAGE_BOX_H
#define ENIGMA_MESSAGE_BOX_H

#include <Core/Core.hpp>
#include <portable-file-dialogs.h>

// Avoid mismatch with windows's MessageBox macro
#if defined(ENIGMA_PLATFORM_WINDOWS)
	#undef MessageBox
	#undef ERROR
	#undef IGNORE
#endif

NS_ENIGMA_BEGIN

class ENIGMA_API MessageBox
{
public: /* Enums */
	/*
	* Message box Icon type
	*/
	enum class Icon : ui8
	{
		Info		= static_cast<ui8>(pfd::icon::info),
		Warning		= static_cast<ui8>(pfd::icon::warning),
		Error		= static_cast<ui8>(pfd::icon::error),
		Question	= static_cast<ui8>(pfd::icon::question)
	};

	/*
	*	Message box button layout choices
	*/
	enum class Choice : ui8
	{
		Ok					= static_cast<ui8>(pfd::choice::ok),
		Ok_Cancel			= static_cast<ui8>(pfd::choice::ok_cancel),
		Yes_No				= static_cast<ui8>(pfd::choice::yes_no),
		Yes_No_Cancel		= static_cast<ui8>(pfd::choice::yes_no_cancel),
		Retry_Cancel		= static_cast<ui8>(pfd::choice::retry_cancel),
		Abort_Retry_Ignore	= static_cast<ui8>(pfd::choice::abort_retry_ignore)
	};

	/*
	*	User Performed Action Result
	*/
	enum class Action : i8
	{
		Cancel	= static_cast<i8>(pfd::button::cancel),
		Ok		= static_cast<i8>(pfd::button::ok),
		Yes		= static_cast<i8>(pfd::button::yes),
		No		= static_cast<i8>(pfd::button::no),
		Abort	= static_cast<i8>(pfd::button::abort),
		Retry	= static_cast<i8>(pfd::button::retry),
		Ignore	= static_cast<i8>(pfd::button::ignore)
	};

public: /* Constructors / Destructor */
	/*
	*	@param title: title of the message box
	*	@param message: description of the message box
	*	@param icon: Icon type of the message box Info, Warning, Error or Question
	*	@param choice: buttons layout of the message box
	*/
	MessageBox(const String& title, const String& message,
		Icon icon = Icon::Info,
		Choice choice = Choice::Ok_Cancel);
	~MessageBox() noexcept = default;

public: /* Accessors */
	/*
	*	Spawns MessageBox and returns user Action
	*/
	MessageBox::Action Show() const;
	
private:
	String m_title;
	String m_message;
	Icon m_icon;
	Choice m_choice;
};

NS_ENIGMA_END

#endif // !ENIGMA_MESSAGE_BOX_H
