#pragma once
#ifndef ENIGMA_SAVE_FILE_DIALOG_H
#define ENIGMA_SAVE_FILE_DIALOG_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN

/**
*	System Dialog to ask the user to select a path to save a file to
*/
class SaveFileDialog {
public: /* Constructors / Destructor */
  /**
    *	Constructor
    *	@param title: title of the dialog
    *	@param initialPath: the default path of the dialog.
    *	@param forceOverwrite: force overwrite and disable the confirmation dialog
    *	@param filters: The filter list enumerates filter names and corresponded space-separated wildcard lists. It defaults to { "*" }
    *	@param singleFilterDescription: The single filter description. It defaults to "All Files"
    */
  explicit SaveFileDialog(const std::string& title,
                          const std::string& initialPath = ".",
                          const bool forceOverwrite = true,
                          const std::initializer_list<std::string>& filters = {"*"},
                          const std::string& singleFilterDescription = "All Files");
  ~SaveFileDialog() noexcept = default;

public: /* Accessors */
  /**
    *	Spawn Save file dialog and get selected path
    */
  [[nodiscard]] std::string Show() const;

private:
  std::string m_title;                   /*< Dialog title */
  std::string m_initialPath;             /*< Dialog initial path */
  bool m_forceOverwrite;                 /*< Shall force file overwriting? */
  std::vector<std::string> m_filters;    /*< Which files to allow e.g {"*.txt", "*.jpg"}  */
  std::string m_singleFilterDescription; /*< All Files, Text Files...  */
};

NS_ENIGMA_END


#endif // !ENIGMA_SAVE_FILE_DIALOG_H
