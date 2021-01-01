#pragma once
#ifndef ENIGMA_DIALOG_UTILS_H
#define ENIGMA_DIALOG_UTILS_H

#include <Core/Core.hpp>
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>

#include <functional>

NS_ENIGMA_BEGIN
class ENIGMA_API DialogUtils 
{
public:
	static const Enigma::MessageBox::Action Error(const String& message, const Enigma::MessageBox::Choice& choice = Enigma::MessageBox::Choice::Ok)
	{
		const auto msgbox = std::make_unique<Enigma::MessageBox>(
			"Enigma",
			message,
			Enigma::MessageBox::Icon::Error,
			choice
			);
		return (msgbox->Show());
	}
    static const Enigma::MessageBox::Action Warn(const String& message, const Enigma::MessageBox::Choice& choice = Enigma::MessageBox::Choice::Ok)
	{
		const auto msgbox = std::make_unique<Enigma::MessageBox>(
			"Enigma",
			message,
			Enigma::MessageBox::Icon::Warning,
			choice
			);
		return (msgbox->Show());
	}

	static const Enigma::MessageBox::Action Question(const String& message, const Enigma::MessageBox::Choice& choice = Enigma::MessageBox::Choice::Yes_No)
	{
		const auto msgbox = std::make_unique<Enigma::MessageBox>(
			"Enigma",
			message,
			Enigma::MessageBox::Icon::Question,
			choice
			);
		return (msgbox->Show());
	}

	static const Enigma::MessageBox::Action Info(const String& message, const Enigma::MessageBox::Choice& choice = Enigma::MessageBox::Choice::Ok)
	{
		const auto msgbox = std::make_unique<Enigma::MessageBox>(
			"Enigma",
			message,
			Enigma::MessageBox::Icon::Info,
			choice
			);
		return (msgbox->Show());
	}
};

NS_ENIGMA_END


#endif	// !ENIGMA_DIALOG_UTILS_H
