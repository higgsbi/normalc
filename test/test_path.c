#include <normalc/collections/vector.h>
#include <normalc/path/path.h>
#include <normalc/string/string.h>
#include <stdio.h>

void test_relative();
void test_appending();
void test_metadata();
void test_custom();
void test_subdirectories();
void test_reading();
void test_name();
void test_normalize();
void test_removal();

int main() {
	test_relative();
	test_custom();
	test_appending();
	test_metadata();
	test_subdirectories();
	test_name();
	test_normalize();
	test_removal();
	return 0;
}

void test_removal() {
	printf("\n--Path Removal--\n\n");	
	Path* file = path_from_cstring("/home/user/Documents/folder/test.txt");
	Path* directory = path_from_cstring("/home/user/Documents/folder/subfolder/");

	Path* file_removed = path_remove(file, 1);
	Path* dir_removed = path_remove(directory, 3);

	string_println(file_removed->url);
	string_println(dir_removed->url);

	path_free(file_removed);
	path_free(dir_removed);
	path_free(file);
	path_free(directory);
}

void test_normalize() {
	printf("\n--Path Normalization--\n\n");	
	Path* home = path_user();
	Path* moved = path_append(home, "../other/");
	Path* moved_file = path_append(home, "../other/file.txt");
	Path* normalized = path_normalize(moved);
	Path* normalized_file = path_normalize(moved_file);

	string_println(moved->url);
	string_println(normalized->url);
	string_println(moved_file->url);
	string_println(normalized_file->url);

	path_free(home);
	path_free(moved);
	path_free(moved_file);
	path_free(normalized_file);
	path_free(normalized);
}

void test_name() {
	printf("\n--File Name--\n\n");	
	Path* home = path_user();
	Path* current = path_current();
	Path* root = path_root();
	Path* file = path_append(current, "text.txt");
	String* home_name = path_name(home);
	String* file_name = path_name(file);
	String* root_name = path_name(root);

	string_println(home->url);
	string_println(home_name);
	
	string_println(file->url);
	string_println(file_name);

	string_println(root->url);
	string_println(root_name);

	path_free(home);
	path_free(current);
	path_free(file);
	path_free(root);
	string_free(file_name);
	string_free(home_name);
	string_free(root_name);
}

void test_subdirectories() {
	printf("\n--Subdirectories and Files--\n\n");	
	Path* home = path_user(); 

	Vector* files = path_get_files(home, true);

	for (size_t i = 0; i < files->count; i++) {
		Path* file = vector_get(files, i);
		string_println(file->url);
	}
	
	vector_free(files);
	path_free(home);
}

void test_custom() {	
	printf("\n--Custom--\n\n");	
	Path* custom_dir = path_from_cstring("/lib/");
	Path* custom_file = path_from_cstring("/lib/ld-linux.so.2");

	string_println(custom_dir->url);
	string_println(custom_file->url);

	path_free(custom_dir);
	path_free(custom_file);
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
