#ifndef NORMALC_PATH_H
#define NORMALC_PATH_H

#include "../string/string_builder.h"

/**
 * Path defines a wrapper around an immutable heap allocated string url
 */
typedef struct {
	String* url;	
} Path;

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
 * Returns a deep clone of a given path
 */
Path* path_clone(Path* src);

/**
 * Frees the given path
 */
void path_free(Path* path);

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
 * Returns true if this path is a directory
 * Note: uses !S_ISREG(path#st_mode) and thus may behave unexpectedly with virtual files
 */
bool path_is_dir(Path* path);

#endif
