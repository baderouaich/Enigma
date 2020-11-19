#pragma once
#ifndef ENIGMA_SAVE_FILE_DIALOG_H
#define ENIGMA_SAVE_FILE_DIALOG_H

#include <Core/Core.hpp>
#include <portable-file-dialogs.h>

NS_ENIGMA_BEGIN

/*
*	System Dialog to ask the user to select a path to save a file to
*/
class ENIGMA_API SaveFileDialog
{
public: /* Constructors / Destructor */
	/*
	*	@param title: title of the dialog
	*	@param initial_path: the default path of the dialog.
	*	@param force_overwrite: force overwrite and disable the confirmation dialog
	*	@param filters: The filter list enumerates filter names and corresponded space-separated wildcard lists.
						It defaults to { "All Files", "*" }
	*/
	explicit SaveFileDialog(const String& title,
		const String& initial_path = ".",
		const bool force_overwrite = true,
		const std::vector<String>& filters = { "All Files", "*" });
	~SaveFileDialog();

public: /* Accessors */
	/*
	*	Spawn Save file dialog and get selected path
	*/
	String Show() const noexcept;

private:
	const String m_title;
	const String m_initial_path;
	const bool m_force_overwrite;
	const std::vector<String> m_filters;
};

NS_ENIGMA_END


#endif // !ENIGMA_SAVE_FILE_DIALOG_H
