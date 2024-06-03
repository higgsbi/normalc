#include "io.h"
#include "../string/string.h"
#include <stdbool.h>

bool _io_read_line(FILE* stream, String** dest);

String* io_file_read(Path* path) {
	ASSERT_NONNULL(path);

	StringBuilder* builder = string_builder_new();
	FILE* file = fopen(path->url->buffer, "r");

	if (!file) {
		String* blank = string_builder_build(builder);
		string_builder_free(builder);
		return blank;
	}

	String* line;
	bool not_eof = false;

	while ((not_eof = _io_read_line(file, &line))) {
		string_builder_append_substring(builder, line->buffer, 0, line->length);

		if (not_eof) {
			string_builder_append_char(builder, '\n');
		}

		string_free(line);
	}

	string_free(line);
	fclose(file);
	String* built = string_builder_build(builder);
	string_builder_free(builder);

	return built;
}

Vector* io_file_read_lines(Path* path) {
	return io_file_read_n_lines(path, -1);
}

Vector* io_file_read_n_lines(Path* path, int n) {
	ASSERT_NONNULL(path);
	
	Vector* lines = vector_new(DEFAULT_LINE_PER_FILE, (Duplicator) string_clone, (Destructor) string_free);
	FILE* file = fopen(path->url->buffer, "r");

	if (!file) {
		return lines;
	}

	String* line;	
	int i = 0;

	for (int i = 0; (n < 0 || i < n) && (_io_read_line(file, &line)); i++) {
		vector_add(lines, line);		
	}

	if (n <= 0) {
		string_free(line);
	}

	fclose(file);

	return lines;
}

bool io_input_read_line(String** dest) {
	return _io_read_line(stdin, dest);
}

// INTERNAL

// return false if we are at the end of file
bool _io_read_line(FILE* stream, String** dest) {
	StringBuilder* builder = string_builder_new();
	char current;
	bool eof = false;

	while ((current = fgetc(stream))) {
		if (current == '\0') {
			break;
		}

		if (current == -1) {
			eof = true;
			break;
		}

		if (current == '\n') {
			break;
		}

		string_builder_append_char(builder, current);
	}	
		
	*dest = string_builder_build(builder);
	string_builder_free(builder);

	return !eof;
}