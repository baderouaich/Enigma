#pragma once

#include <portable-file-dialogs.h>

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN

/**
*	System Dialog to ask the user to select a file from disk
*/
class OpenFileDialog {
  public:
    /**
    *	Constructor
    *	@param title: title of the dialog
    *	@param initial_path: the default path of the dialog.
    *	@param allow_multiselect:  to allow selecting multiple files
    *	@param filters: The filter list enumerates filter names and corresponded space-separated wildcard lists. It defaults to { "All Files", "*" }
    */
    OpenFileDialog(const std::string& title,
                   const std::string& initial_path = ".",
                   const bool allow_multiselect = true,
                   const std::initializer_list<std::string>& filters = {"All Files", "*"});
    ~OpenFileDialog() noexcept = default;

  public: /* Accessors */
    /**
    *	@brief Spawns Open File Dialog and returns selected file(s) paths
    */
    std::vector<std::string> Show() const;

  private:
    std::string m_title;                /*< Dialog title */
    std::string m_initial_path;         /*< Dialog initial path */
    bool m_allow_multiselect;           /*< Shall allow multi file selection ? */
    std::vector<std::string> m_filters; /*< Which files to allow e.g .txt, .jpg... */
};

NS_ENIGMA_END
