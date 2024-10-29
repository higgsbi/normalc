#ifndef NORMALC_PATH_H
#define NORMALC_PATH_H

#include "../string/string_builder.h"
#include "../collections/vector.h"

/**
 * Path defines a wrapper around an immutable heap allocated string url
 */
typedef struct {
	String* url;	
} Path;

OPTION_TYPE(Path*, Path, path, NULL)

/**
 * Returns the current working directory with a trailing slash
 */
Path* path_current();

/**
 * Returns the root directory with a trailing slash
 */
Path* path_root();

/**
 * Returns the path to the user folder of the active user with a trailing slash
 */
Path* path_user();

/**
 * Returns the current path given the passed url
 */
Path* path_from_cstring(char* url);

/**
 * Returns the current path given the passed url
 * If discard_url is false, the given url will be cloned, leaving the original untouched
 */
Path* path_from_string(String* url, bool discard_url);

/**
 * Returns a deep clone of a given path
 */
Path* path_clone(Path* src);

/**
 * Frees the given path
 */
void path_free(Path* path);

/**
 * Returns the parent of this path.
 * If the current path is a file, this function returns the file's directory
 * If the current path is a directory, this function returns the preceding directory.
 * If the current path is the root directory, path_root() is returned 
 */
Path* path_parent(Path* path);

/**
 * Returns a path with the given cstring appended to the end. Does not free
 * the original path
 */
Path* path_append(Path* path, char* appended);

/**
 * Returns the extension of the given path. 
 * Returns a blank string if this file is a directory
 */
String* path_extension(Path* path); 


/**
 * Returns the name of the file or directory of the given path
 * Returns / if a root path is given
 */
String* path_name(Path* path);

/**
 * Returns the path normalized against the '..' pattern
 * If '..' is the first string, `path_current()` is used to retrieve parent directory
 */
Path* path_normalize(Path* path);

/**
 * Returns the path with one of the path parts removed
 * If no path could be removed, a cloned version of the original is returned
 */
Path* path_remove(Path* path, size_t index);

/**
 * Returns true if the given path exists
 */
bool path_exists(Path* path);

/**
 * Returns a vector of Paths representing subdirectories 
 * and files in the given path.
 *
 * If the absolute_path parameter is true, the full path name
 * will be used for each file instead of the relative name.
 *
 * If the given path is either empty or not a directory,
 * an empty vector is returned.
 * 
 */
Vector* path_get_files(Path* path, bool absolute_path);

/**
 * Returns true if this path is a directory
 */
bool path_is_dir(Path* path);

#endif
