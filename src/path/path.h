#ifndef STD_PATH_H
#define STD_PATH_H

#include "../string/string_builder.h"

typedef struct {
	String* url;	
} Path;

Path* path_current();
Path* path_root();
Path* path_user();
Path* path_clone(Path* src);
void path_free(Path* path);

Path* path_append(Path* path, char* appended);
String* path_extension(Path* path); 

bool path_is_dir(Path* path);

#endif
