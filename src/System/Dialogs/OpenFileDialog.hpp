#pragma once
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
    *	@param initialPath: the default path of the dialog.
    *	@param allowMultiselect:  to allow selecting multiple files
    *	@param filters: The filter list enumerates filter names and corresponded space-separated wildcard lists. It defaults to { "*" }
    *	@param singleFilterDescription: The single filter description. It defaults to "All Files"
    */
  explicit OpenFileDialog(const std::string& title,
                          const std::string& initialPath = ".",
                          const bool allowMultiselect = true,
                          const std::initializer_list<std::string>& filters = {"*"},
                          const std::string& singleFilterDescription = "All Files");
  ~OpenFileDialog() noexcept = default;

public: /* Accessors */
  /**
    *	@brief Spawns Open File Dialog and returns selected file(s) paths
    */
  [[nodiscard]] std::vector<std::string> Show() const;

private:
  std::string m_title;                   /*< Dialog title */
  std::string m_initialPath;             /*< Dialog initial path */
  bool m_allowMultiselect;               /*< Shall allow multi file selection ? */
  std::vector<std::string> m_filters;    /*< Which files to allow e.g {"*.txt", "*.jpg"}  */
  std::string m_singleFilterDescription; /*< All Files, Text Files...  */
};

NS_ENIGMA_END
