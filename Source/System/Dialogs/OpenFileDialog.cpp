#include <pch.hpp>
#include "OpenFileDialog.hpp"


NS_ENIGMA_BEGIN
OpenFileDialog::OpenFileDialog(const String& title, const String& initial_path, const bool allow_multiselect, const std::initializer_list<String>& filters)
	:
	m_title(title),
	m_initial_path(initial_path),
	m_allow_multiselect(allow_multiselect),
	m_filters(filters)
{
}

std::vector<String> OpenFileDialog::Show() const
{
	return pfd::open_file(
		m_title,
		m_initial_path,
		m_filters,
		m_allow_multiselect ? pfd::opt::multiselect : pfd::opt::none)
		.result();
}

NS_ENIGMA_END

