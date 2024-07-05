#include "OpenFileDialog.hpp"
#include <pch.hpp>


NS_ENIGMA_BEGIN
OpenFileDialog::OpenFileDialog(const std::string& title, const std::string& initial_path, const bool allow_multiselect, const std::initializer_list<std::string>& filters)
    : m_title(title),
      m_initial_path(initial_path),
      m_allow_multiselect(allow_multiselect),
      m_filters(filters) {
}

std::vector<std::string> OpenFileDialog::Show() const {
  return pfd::open_file(
           m_title,
           m_initial_path,
           m_filters,
           m_allow_multiselect ? pfd::opt::multiselect : pfd::opt::none)
    .result();
}

NS_ENIGMA_END
