#pragma once
#ifndef ENIGMA_SAVE_FILE_DIALOG_H
#define ENIGMA_SAVE_FILE_DIALOG_H

#include <Core/Core.hpp>
#include <portable-file-dialogs.h>

NS_ENIGMA_BEGIN

/**
*	System Dialog to ask the user to select a path to save a file to
*/
class SaveFileDialog {
  public: /* Constructors / Destructor */
    /**
    *	Constructor
    *	@param title: title of the dialog
    *	@param initial_path: the default path of the dialog.
    *	@param force_overwrite: force overwrite and disable the confirmation dialog
    *	@param filters: The filter list enumerates filter names and corresponded space-separated wildcard lists.
              It defaults to { "All Files", "*" }
    */
    SaveFileDialog(const std::string& title,
                   const std::string& initial_path = ".",
                   const bool force_overwrite = true,
                   const std::initializer_list<std::string>& filters = {"All Files", "*"});
    ~SaveFileDialog() noexcept = default;

  public: /* Accessors */
    /**
    *	Spawn Save file dialog and get selected path
    */
    std::string Show() const;

  private:
    std::string m_title;                /*< Dialog title */
    std::string m_initial_path;         /*< Dialog initial path */
    bool m_force_overwrite;             /*< Shall force file overwriting? */
    std::vector<std::string> m_filters; /*< Which files to allow to save e.g .txt, .jpg... { "Text Files (.txt .text)", "*.txt *.text", "All Files", "*" }, */
};

NS_ENIGMA_END


#endif // !ENIGMA_SAVE_FILE_DIALOG_H
