#include <pch.hpp>
#include "SaveFileDialog.hpp"


NS_ENIGMA_BEGIN

SaveFileDialog::SaveFileDialog(const std::string& title, const std::string& initial_path, const bool force_overwrite, const std::initializer_list<std::string>& filters)
    : m_title(title),
      m_initial_path(initial_path),
      m_force_overwrite(force_overwrite),
      m_filters(filters) {
}

std::string SaveFileDialog::Show() const {
  return pfd::save_file(
           m_title,
           m_initial_path,
           m_filters,
           m_force_overwrite ? pfd::opt::force_overwrite : pfd::opt::none)
    .result();
}

NS_ENIGMA_END
