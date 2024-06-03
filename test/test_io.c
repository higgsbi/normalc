#include <normalc/path/io.h>
#include <normalc/string/string.h>
#include <stdio.h>

void test_file_lines();
void test_file_all();
void test_input();

int main() {
	test_file_lines();
	test_file_all();
	test_input();
}

void test_input() {
	printf("\n--Reading Input--\n\n");	
	printf("Give input, system will return input. Use 'end' to stop\n");

	String* line;

	while ((io_input_read_line(&line)) && !string_equals(line, "end")) {
		string_println(line);
		string_free(line);
	}

	string_free(line);
}

void test_file_lines() {
	printf("\n--Reading File Lines--\n\n");	

	Path* cwd = path_current();
	Path* test = path_append(cwd, "file.txt");
	Vector* lines = io_file_read_lines(test);

	for (size_t i = 0; i < lines->count; i++) {
		string_println(vector_get(lines, i));
	}

	vector_free(lines);
	path_free(cwd);
	path_free(test);
}

void test_file_all() {
	printf("\n--Reading File All--\n\n");	

	Path* cwd = path_current();
	Path* test = path_append(cwd, "file.txt");
	String* all = io_file_read(test);

	string_println(all);

	string_free(all);
	path_free(cwd);
	path_free(test);
}
