#include <normalc/collections/vector.h>
#include <normalc/path/path.h>
#include <normalc/string/string.h>
#include <stdio.h>

void test_relative();
void test_appending();
void test_metadata();
void test_custom();
void test_parent();
void test_subdirectories();
void test_reading();

int main() {
	test_relative();
	test_custom();
	test_parent();
	test_appending();
	test_metadata();
	test_subdirectories();
	test_reading();
	return 0;
}

void test_reading() {	
	printf("\n--Reading File--\n\n");	

	Path* cwd = path_current();
	Path* test = path_append(cwd, "file.txt");
	Vector* lines = path_get_lines(test);

	for (size_t i = 0; i < lines->count; i++) {
		string_println(vector_get(lines, i));
	}

	vector_free(lines);
	path_free(cwd);
	path_free(test);
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

void test_parent() {	
	printf("\n--Parent--\n\n");	
	Path* custom_file = path_from_cstring("/lib/ld-linux.so.2");
	string_println(custom_file->url);

	Path* dir_lib = path_parent(custom_file);
	string_println(dir_lib->url);

	Path* root_file = path_from_cstring("/test.txt");
	string_println(root_file->url);

	Path* root = path_parent(root_file);
	string_println(root->url);

	Path* custom_dir = path_from_cstring("/boot/grub2/");
	string_println(custom_dir->url);

	Path* parent = path_parent(custom_dir);
	string_println(parent->url);

	path_free(custom_dir);
	path_free(parent);
	path_free(root);
	path_free(root_file);
	path_free(custom_file);
	path_free(dir_lib);
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
