#include "path.h"
#include "../memory/memory.h"
#include "../string/string_builder.h"
#include <normalc/string/string.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

bool _cstring_is_dir(char* path);

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

	path->url = string_from_format("/home/%s/", user);

	return path;
}

Path* path_from_cstring(char* url) {
	ASSERT_NONNULL(url);

	Path* path = allocate(sizeof(Path));	
	size_t length = strlen(url);
	
	// account for input of directory without trailing slash
	if (_cstring_is_dir(url) && url[length - 1] != '/') {
		path->url = string_from_format("%s/", url);
	} else {
		path->url = string_from(url);
	}

	return path;
}

Path* path_from_string(String* url, bool discard_url) {
	ASSERT_NONNULL(url);
	ASSERT_NONNULL(url->buffer);
	Path* path = allocate(sizeof(Path));

	// account for input of directory without trailing slash
	if (_cstring_is_dir(url->buffer) && url->buffer[url->length - 1] != '/') {
		path->url = string_from_format("%s/", url);
		if (discard_url) {
			string_free(url);
		}
	} else {
		if (discard_url) {	
			path->url = url;
		} else {
			path->url = string_clone(url);
		}
	}

	return path;
}


Path* path_clone(Path* src) {
	ASSERT_NONNULL(src);

	Path* path = allocate(sizeof(Path));
	path->url = string_clone(src->url);	

	return path;
}

Path* path_parent(Path* path) {
	// root directory or file in root directory
	if (string_index_of_last(path->url, '/') == 0) {
		return path_root();
	}

	size_t last_slash;

	// non-root directory 
	if (path_is_dir(path)) {
		last_slash = string_nth_index_of_last(path->url, 2, '/');	
	} else {
		last_slash = string_index_of_last(path->url, '/');	
	}

	String* substring = string_sub(path->url, 0, last_slash + 1);

	return path_from_string(substring, true);
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
	return _cstring_is_dir(path->url->buffer);
}

bool path_exists(Path* path) {	
	struct stat path_stat;
    return (lstat(path->url->buffer, &path_stat) != 0);
}

void path_free(Path* path) {
	string_free(path->url);
	free(path);
}

Vector* path_get_files(Path* path, bool use_absolute) {
	struct dirent* file_entry;

	DIR* directory = opendir(path->url->buffer);	
	if (!directory) {
		closedir(directory);
		return vector_new(0, (Duplicator) path_clone, (Destructor) path_free);
	}
	
	Vector* files = vector_new(3, (Duplicator) path_clone, (Destructor) path_free);

	while ((file_entry = readdir(directory))) {
		if (file_entry->d_type == DT_DIR) {
			if (use_absolute) {
				String* appended = string_from_format("%s%s/", path->url->buffer, file_entry->d_name);
				vector_add(files, path_from_string(appended, true));
			} else {
				String* appended = string_from_format("%s/", file_entry->d_name);;
				vector_add(files, path_from_string(appended, true));
			}	
		} else {
			if (use_absolute) {
				String* appended = string_from_format("%s%s", path->url->buffer, file_entry->d_name);
				vector_add(files, path_from_string(appended, true));
			} else {	
				vector_add(files, path_from_cstring(file_entry->d_name));
			}
		}
	}

	closedir(directory);

	return files;
}

// Internal

bool _cstring_is_dir(char* path) {
	struct stat path_stat;

    if (lstat(path, &path_stat) != 0) {
		return false;
	}

	return (path_stat.st_mode & S_IFDIR);
}
