#ifndef _INI_TEST_H_
#define _INI_TEST_H_

/**
 * \file fs.h
 * \brief File System Helpers.
 *
 * fs.h provides helper functions related to the file system.
 */

/**
 * Makes a path absolute.
 * \param	path	The path to make absolute.
 */
void path_make_absolute(STRING *path)
{
	static STRING *tmp = "#1024";
	str_cpy(tmp, work_dir);
	str_cat(tmp, "\\");
	str_cat(tmp, path);
	str_cpy(path, tmp);
}


#endif