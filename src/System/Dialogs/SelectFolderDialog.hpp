#pragma once
#ifndef ENIGMA_SELECT_FOLDER_DIALOG_H
#define ENIGMA_SELECT_FOLDER_DIALOG_H

#include <Core/Core.hpp>
#include <portable-file-dialogs.h>

NS_ENIGMA_BEGIN

/**
*	System Dialog to ask the user to select a folder from disk
*/
class SelectFolderDialog {
  public: /* Constructors / Destructor */
    /**
    *	Constructor
    *	@param title: title of the dialog
    *	@param initial_path: the default path of the dialog.
    *	@param force_path: to force the operating system to use the provided path. Some systems default to the most recently used path, if applicable.
    */
    SelectFolderDialog(const std::string& title,
                       const std::string& initial_path = ".",
                       const bool force_path = true);
    ~SelectFolderDialog() noexcept = default;

  public: /* Accessors */
    /**
    *	Spawns Select Folder Dialog and returns selected folder path
    */
    std::string Show() const;

  private:
    std::string m_title;        /*< Dialog title */
    std::string m_initial_path; /*< Dialog initial path */
    bool m_force_path;          /*< Shall force the operating system to use the provided path? Some systems default to the most recently used path, if applicable. */
};

NS_ENIGMA_END


#endif // !ENIGMA_SELECT_FOLDER_DIALOG_H
