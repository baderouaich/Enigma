#pragma once
#ifndef ENIGMA_SELECT_FOLDER_DIALOG_H
#define ENIGMA_SELECT_FOLDER_DIALOG_H

#include <Core/Core.hpp>
#include <portable-file-dialogs.h>

NS_ENIGMA_BEGIN

/*
*	System Dialog to ask the user to select a folder from disk
*/
class ENIGMA_API SelectFolderDialog
{
public: /* Constructors / Destructor */
	/*
	*	@param title: title of the dialog
	*	@param initial_path: the default path of the dialog.
	*	@param force_path: to force the operating system to use the provided path. Some systems default to the most recently used path, if applicable.
	*/
	explicit SelectFolderDialog(const String& title,
		const String& initial_path = ".",
		const bool force_path = true);
	~SelectFolderDialog();

public: /* Accessors */
	/*
	*	Spawns Select Folder Dialog and returns selected folder path
	*/
	String Show() const noexcept;

private:
	const String m_title;
	const String m_initial_path;
	const bool m_force_path;
};

NS_ENIGMA_END


#endif // !ENIGMA_SELECT_FOLDER_DIALOG_H
