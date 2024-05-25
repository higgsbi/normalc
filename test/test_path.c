#include "standard/path/path.h"
#include "standard/string/string.h"
#include <stdio.h>

void test_relative();
void test_appending();
void test_metadata();

int main() {
	test_relative();
	test_appending();
	test_metadata();
	return 0;
}

void test_metadata() {
	printf("\n--Metadata--\n\n");	
	Path* current = path_current();
	Path* appended = path_append(current, "test_path.c");

	String* ext_dir = path_extension(current);
	String* ext_c = path_extension(appended);

	if (path_is_dir(current)) {	
		printf("Extension dir: '%s'\n", ext_dir->buffer);
	}

	if (!path_is_dir(appended)) {
		printf("Extension c: '%s'\n", ext_c->buffer);
	}

	string_free(ext_dir);
	string_free(ext_c);
	path_free(appended);
	path_free(current);
}

void test_appending() {
	printf("\n--Appending Paths--\n\n");
	Path* current = path_current();
	string_println(current->url);

	Path* appended = path_append(current, "test_path.c");
	string_println(appended->url);

	path_free(appended);
	path_free(current);
}

void test_relative() {
	printf("\n--Relative Paths--\n\n");
	Path* user = path_user();
	Path* root = path_root();

	string_println(user->url);
	string_println(root->url);

	path_free(user);
	path_free(root);
}
