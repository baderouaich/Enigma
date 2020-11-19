#include <pch.hpp>
#include "SelectFolderDialog.hpp"

NS_ENIGMA_BEGIN

SelectFolderDialog::SelectFolderDialog(const String& title, const String& initial_path, const bool force_path)
	:
	m_title(title),
	m_initial_path(initial_path),
	m_force_path(force_path)
{
}


String SelectFolderDialog::Show() const noexcept
{
	return pfd::select_folder(
		m_title,
		m_initial_path,
		m_force_path ? pfd::opt::force_path : pfd::opt::none)
		.result();
}

SelectFolderDialog::~SelectFolderDialog()
{
}

NS_ENIGMA_END

