#pragma once
#ifndef ENIGMA_OPEN_FILE_DIALOG_H
#define ENIGMA_OPEN_FILE_DIALOG_H

#include <Core/Core.hpp>
#include <portable-file-dialogs.h>

NS_ENIGMA_BEGIN

/**
*	System Dialog to ask the user to select a file from disk
*/
class ENIGMA_API OpenFileDialog 
{
public: /* Constructors / Destructor */
	/**
	*	Constructor
	*	@param title: title of the dialog
	*	@param initial_path: the default path of the dialog.
	*	@param allow_multiselect:  to allow selecting multiple files
	*	@param filters: The filter list enumerates filter names and corresponded space-separated wildcard lists. It defaults to { "All Files", "*" }
	*/
	OpenFileDialog(const String& title,
		const String& initial_path = ".",
		const bool allow_multiselect = true,
		const std::initializer_list<String>& filters = { "All Files", "*" });
	~OpenFileDialog() noexcept = default;

public: /* Accessors */
	/**
	*	Spawns Open File Dialog and returns selected file(s) paths
	*/
	std::vector<String> Show() const;

private: 
	String m_title; /*< Dialog title */
	String m_initial_path; /*< Dialog initial path */
	bool m_allow_multiselect; /*< Shall allow multi file selection ? */
	std::vector<String> m_filters; /*< Which files to allow e.g .txt, .jpg... */
};

NS_ENIGMA_END


#endif // !ENIGMA_OPEN_FILE_DIALOG_H
