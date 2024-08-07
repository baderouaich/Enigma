#pragma once
#ifndef ENIGMA_DIALOG_UTILS_H
#define ENIGMA_DIALOG_UTILS_H

#include <Core/Core.hpp>
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>
#include <Utility/FileUtils.hpp>

NS_ENIGMA_BEGIN

class DialogUtils final {
    ENIGMA_STATIC_CLASS(DialogUtils);

  public:
    static Enigma::MessageBox::Action Error(const std::string& title, const std::string& message, const Enigma::MessageBox::Choice choice = Enigma::MessageBox::Choice::Ok) {
      const auto msgbox = std::make_unique<Enigma::MessageBox>(
        title,
        message,
        Enigma::MessageBox::Icon::Error,
        choice);
      return msgbox->Show();
    }

    static Enigma::MessageBox::Action Error(const std::string& message, const Enigma::MessageBox::Choice choice = Enigma::MessageBox::Choice::Ok) {
      const auto msgbox = std::make_unique<Enigma::MessageBox>(
        "Enigma",
        message,
        Enigma::MessageBox::Icon::Error,
        choice);
      return msgbox->Show();
    }

    static Enigma::MessageBox::Action Warn(const std::string& title, const std::string& message, const Enigma::MessageBox::Choice choice = Enigma::MessageBox::Choice::Ok) {
      const auto msgbox = std::make_unique<Enigma::MessageBox>(
        title,
        message,
        Enigma::MessageBox::Icon::Warning,
        choice);
      return msgbox->Show();
    }

    static Enigma::MessageBox::Action Warn(const std::string& message, const Enigma::MessageBox::Choice choice = Enigma::MessageBox::Choice::Ok) {
      const auto msgbox = std::make_unique<Enigma::MessageBox>(
        "Enigma",
        message,
        Enigma::MessageBox::Icon::Warning,
        choice);
      return msgbox->Show();
    }

    static Enigma::MessageBox::Action Question(const std::string& title, const std::string& message, const Enigma::MessageBox::Choice choice = Enigma::MessageBox::Choice::Yes_No) {
      const auto msgbox = std::make_unique<Enigma::MessageBox>(
        title,
        message,
        Enigma::MessageBox::Icon::Question,
        choice);
      return msgbox->Show();
    }

    static Enigma::MessageBox::Action Question(const std::string& message, const Enigma::MessageBox::Choice choice = Enigma::MessageBox::Choice::Yes_No) {
      const auto msgbox = std::make_unique<Enigma::MessageBox>(
        "Enigma",
        message,
        Enigma::MessageBox::Icon::Question,
        choice);
      return msgbox->Show();
    }

    static Enigma::MessageBox::Action Info(const std::string& title, const std::string& message, const Enigma::MessageBox::Choice choice = Enigma::MessageBox::Choice::Ok) {
      const auto msgbox = std::make_unique<Enigma::MessageBox>(
        title,
        message,
        Enigma::MessageBox::Icon::Info,
        choice);
      return msgbox->Show();
    }

    static Enigma::MessageBox::Action Info(const std::string& message, const Enigma::MessageBox::Choice choice = Enigma::MessageBox::Choice::Ok) {
      const auto msgbox = std::make_unique<Enigma::MessageBox>(
        "Enigma",
        message,
        Enigma::MessageBox::Icon::Info,
        choice);
      return msgbox->Show();
    }

    /*
	static fs::path SelectFolder(const std::string& title, const std::string& initial_path = FileUtils::GetEnigmaExecutableDir().string(), const bool force_path = true)
	{
		const auto sfd = std::make_unique<Enigma::SelectFolderDialog>(
			title,
			initial_path,
			force_path
			);
		return fs::path(sfd->Show());
	}
	static fs::path SelectFile(const std::string& title, const std::string& initial_path = FileUtils::GetEnigmaExecutableDir().string(), const bool allow_multiselect = true)
	{
		std::initializer_list<std::string> filters{};
		const auto sfd = std::make_unique<Enigma::OpenFileDialog>(
			title,
			initial_path,
			false,
			filters
			);
		return fs::path(sfd->Show()[0]);
	}*/
};

NS_ENIGMA_END


#endif // !ENIGMA_DIALOG_UTILS_H
