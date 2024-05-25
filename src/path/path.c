#include "path.h"
#include "../memory/memory.h"
#include <standard/string/string_builder.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

Path* path_current() {
	char* raw = getcwd(NULL, 0);
	Path* path = allocate(sizeof(Path)); 
	String* wrapper = allocate(sizeof(String));
	wrapper->length = strlen(raw) + 2;
	wrapper->buffer = raw;
	path->url = wrapper;

	// Append / to cwd
	wrapper->buffer = reallocate(wrapper->buffer, sizeof(char) * wrapper->length);
	wrapper->buffer[wrapper->length - 2] = '/';
	wrapper->buffer[wrapper->length - 1] = '\0';

	return path;
}

Path* path_root() {
	Path* path = allocate(sizeof(Path));
	path->url = string_from("/");

	return path;
}

Path* path_user() {	
	Path* path = allocate(sizeof(Path));
	char* user = getenv("USER");

	ASSERT_NONNULL(user);

	path->url = string_from_format("/home/%s/", getenv("USER"));

	return path;
}

Path* path_clone(Path* src) {
	ASSERT_NONNULL(src);

	Path* path = allocate(sizeof(Path));
	path->url = string_clone(src->url);	

	return path;
}

Path* path_append(Path* path, char* appended) {
	ASSERT_NONNULL(path);
	ASSERT_NONNULL(appended);

	Path* resolved = allocate(sizeof(Path));
	StringBuilder* builder = string_builder_from(path->url->buffer);
	string_builder_append(builder, appended);
	resolved->url = string_builder_build(builder);

	string_builder_free(builder);

	return resolved;	
}

String* path_extension(Path* path) {
	int index = string_index_of_last(path->url, '.');
	
	if (index < 0) {
		return string_empty();
	}

	return string_sub(path->url, index + 1, path->url->length - index - 2);
}

bool path_is_dir(Path* path) {
	struct stat path_stat;
    lstat(path->url->buffer, &path_stat);
    return !S_ISREG(path_stat.st_mode);	
}

void path_free(Path* path) {
	string_free(path->url);
	free(path);
}


