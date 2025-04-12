#include <pch.hpp>
#include "SelectFolderDialog.hpp"

NS_ENIGMA_BEGIN

SelectFolderDialog::SelectFolderDialog(const std::string& title, const std::string& initialPath)
  : m_title(title),
    m_initialPath(initialPath) {
}

std::string SelectFolderDialog::Show() const {
  /* returns NULL on cancel */
  const char* res = tinyfd_selectFolderDialog(m_title.c_str(), m_initialPath.c_str());
  return res ? std::string(res) : std::string();
}

NS_ENIGMA_END
