#include <pch.hpp>
#include "OpenFileDialog.hpp"
#include <Utility/StringUtils.hpp>

NS_ENIGMA_BEGIN
OpenFileDialog::OpenFileDialog(const std::string& title, const std::string& initialPath, const bool allowMultiselect, const std::initializer_list<std::string>& filters, const std::string& singleFilterDescription)
  : m_title(title),
    m_initialPath(initialPath),
    m_allowMultiselect(allowMultiselect),
    m_filters(filters),
    m_singleFilterDescription(singleFilterDescription) {
}

std::vector<std::string> OpenFileDialog::Show() const {
  std::vector<std::string> files;
  std::vector<const char*> filters;
  if (!m_filters.empty()) {
    filters.reserve(m_filters.size());
    for (const std::string& filter: m_filters) {
      filters.push_back(filter.c_str());
    }
  }
  /* in case of multiple files, the separator is | */
  /* returns NULL on cancel */
  const char* res = tinyfd_openFileDialog(m_title.c_str(),
                                          m_initialPath.c_str(),
                                          m_filters.size(),
                                          filters.empty() ? nullptr : filters.data(),
                                          m_singleFilterDescription.empty() ? nullptr : m_singleFilterDescription.c_str(),
                                          m_allowMultiselect);
  if (res == nullptr) {
    return files;
  }
  if (m_allowMultiselect) {
    return StringUtils::Split(std::string(res), '|');
  }
  files.emplace_back(res);
  return files;
}

NS_ENIGMA_END
