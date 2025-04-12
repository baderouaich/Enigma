#pragma once
#ifndef ENIGMA_SELECT_FOLDER_DIALOG_H
#define ENIGMA_SELECT_FOLDER_DIALOG_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN

/**
*	System Dialog to ask the user to select a folder from disk
*/
class SelectFolderDialog {
public:
  /**
    *	Constructor
    *	@param title: title of the dialog
    *	@param initialPath: the default path of the dialog.
    */
  explicit SelectFolderDialog(const std::string& title,
                              const std::string& initialPath = ResourceManager::getAppDataDir().string());
  ~SelectFolderDialog() noexcept = default;

public: /* Accessors */
  /**
    *	Spawns Select Folder Dialog and returns selected folder path
    */
  [[nodiscard]] std::string Show() const;

private:
  std::string m_title;       /*< Dialog title */
  std::string m_initialPath; /*< Dialog initial path */
};

NS_ENIGMA_END


#endif // !ENIGMA_SELECT_FOLDER_DIALOG_H
