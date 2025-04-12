#include <pch.hpp>
#include "SaveFileDialog.hpp"


NS_ENIGMA_BEGIN

SaveFileDialog::SaveFileDialog(const std::string& title, const std::string& initialPath, const bool forceOverwrite, const std::initializer_list<std::string>& filters, const std::string& singleFilterDescription)
  : m_title(title),
    m_initialPath(initialPath),
    m_forceOverwrite(forceOverwrite),
    m_filters(filters),
    m_singleFilterDescription(singleFilterDescription) {
}

std::string SaveFileDialog::Show() const {

  std::vector<const char*> filters;
  if (!m_filters.empty()) {
    filters.reserve(m_filters.size());
    for (const std::string& filter: m_filters) {
      filters.push_back(filter.c_str());
    }
  }
  /* returns NULL on cancel */
  const char* res = tinyfd_saveFileDialog(m_title.c_str(),
                                          m_initialPath.c_str(),
                                          m_filters.size(),
                                          filters.empty() ? nullptr : filters.data(),
                                          m_singleFilterDescription.empty() ? nullptr : m_singleFilterDescription.c_str());
  return res ? std::string(res) : std::string();
}

NS_ENIGMA_END
