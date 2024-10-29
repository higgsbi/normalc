#include "string.h"
#include "../memory/memory.h"
#include "../error/error.h"
#include "string_builder.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

String* string_empty() {
	String* dest = allocate(sizeof(String));
	char* buffer = (char*) allocate(sizeof(char) * 1);
	strncpy(buffer, "\0", 1);
	dest->buffer = buffer;
	dest->length = 0;	

	return dest;
}

String* string_from(char* src) {
	ASSERT_NONNULL(src);	

	size_t length = strlen(src);

	String* string = (String*) allocate(sizeof(String));
	string->buffer = (char*) allocate(sizeof(char) * (length + 1));
	string->length = length; 

	strncpy(string->buffer, src, length);
	strncpy(string->buffer + length, "\0", 1);

	return string;
}

String* string_from_substring(char* source, size_t start, size_t length) {
	ASSERT_NONNULL(source);

	String* string = (String*) allocate(sizeof(String));
	string->buffer = (char*) allocate(sizeof(char) * (length + 1));
	string->length = length; 

	strncpy(string->buffer, source + start, length);
	strncpy(string->buffer + length, "\0", 1);

	return string;
}

String* string_from_format(char* format, ...) {
	ASSERT_NONNULL(format); 

    va_list args;
    va_start(args, format);

    String* string = (String*) allocate(sizeof(String));
    vasprintf(&string->buffer, format, args);
	string->length = strlen(string->buffer);

    va_end(args);

	return string;
}

String* string_clone(String* src) {
	ASSERT_NONNULL(src);

	return string_from(src->buffer);
}

void string_free(String* string) {
	ASSERT_NONNULL(string);	

	free(string->buffer);
	free(string);
	string = NULL;
}


int string_nth_index_of(String* string, size_t n, char query) {
	ASSERT_NONNULL(string);
	ASSERT_NONNULL(string->buffer);

	size_t count = 0;

	for (size_t i = 0; i < string->length; i++) {
		if (string->buffer[i] == query) {
			count++;
			if (count >= n) {	
				return i;
			}
		}	
	}
	
	return -1;
}

int string_index_of(String* string, char query) {
	return string_nth_index_of(string, 1, query);
}

int string_nth_index_of_last(String* string, size_t n, char query) {
	size_t count = 0;

	ASSERT_NONNULL(string);
	ASSERT_NONNULL(string->buffer);

	for (size_t i = string->length - 1; i != SIZE_MAX; i--) {
		if (string->buffer[i] == query) {
			count++;

			if (count >= n) {
				return i;
			}
		}	
	}

	return -1;
}

int string_index_of_last(String* string, char query) {
	return string_nth_index_of_last(string, 1, query);
}

int string_index_of_string(String* string, char* query) {
	ASSERT_NONNULL(string);
	ASSERT_NONNULL(string->buffer);

	size_t length = strlen(query);

	if (((int) string->length) - length < 1) {
		return -1;
	}

	for (size_t i = 0; i < string->length - length; i++) {
		for (size_t j = 0; j < length; j++) {
			if (string->buffer[i + j] != query[j]) {
				break;
			}

			if (j == length - 1) {	
				return i;
			}
		}	
	}

	return -1;
}

int string_index_of_last_string(String* string, char* query) {
	ASSERT_NONNULL(string);
	ASSERT_NONNULL(string->buffer);	

	int length = strlen(query);

	for (int i = string->length - length - 1; i >= 0; i--) {
		for (int j = length - 1; j >= 0; j--) {
			if (string->buffer[i + j] != query[j]) {
				break;
			}

			if (j == length -1) {	
				return i;
			}
		}	
	}

	return -1;
}

String* string_sub(String* src, size_t start, size_t length) {
	ASSERT_NONNULL(src);
	ASSERT_VALID_RANGE((int) start, (int) (start + length));
	ASSERT_VALID_BOUNDS(src, (int) start, (int) src->length);
	ASSERT_VALID_BOUNDS(src, (int) (start + length), (int) src->length + 1);

	String* string = (String*) allocate(sizeof(String));
	string->buffer = (char*) allocate(sizeof(char) * length + 1);
	string->length = length;

	strncpy(string->buffer, src->buffer + start, length);
	strncpy(string->buffer + string->length, "\0", 1);

	return string;
}

String* string_replace(String *src, char *replaced, char *replacer) {
	ASSERT_NONNULL(src);
	ASSERT_NONNULL(replaced);
	ASSERT_NONNULL(replaced);

	StringBuilder* builder = string_builder_new();	

	size_t length_replaced = strlen(replaced);
	size_t start_unmatched = 0;
	bool matched = false; 

	for (size_t i = 0; i < src->length; i++) {
		matched = false;

		// 1.) Check if current index is replaced
		for (size_t j = 0; j < length_replaced; j++) {
			if (src->buffer[i + j] != replaced[j]) {
				// Not a match and not last index
				break;
			}
			
			matched = (j == length_replaced - 1);	
		}

		// 2.) Replace matched with replacer
		if (matched) {
			string_builder_append_substring(builder, src->buffer, start_unmatched, i - start_unmatched);	

			string_builder_append(builder, replacer);	
			i += length_replaced - 1;
			start_unmatched = i + 1;
		} else if (i == src->length - 1) {
			// 3.) Add unmatched tail from source
			string_builder_append_substring(builder, src->buffer, start_unmatched, i - start_unmatched + 1);			
		}
	}

	String* built = string_builder_build(builder);
	string_builder_free(builder);	

	return built;
}

bool string_contains(String* src, char query) {
	return string_index_of(src, query) != -1;	
}

bool string_contains_string(String* src, char* query) {
	return string_index_of_string(src, query) != -1;
}

String* string_uppercase(String* src) {
	ASSERT_NONNULL(src);

	String* upper = string_clone(src);
	for (size_t i = 0; i < upper->length; i++) {
		upper->buffer[i] = toupper(upper->buffer[i]);	
	}

	return upper;
}

String* string_lowercase(String* src) {
	ASSERT_NONNULL(src);

	String* upper = string_clone(src);
	for (size_t i = 0; i < upper->length; i++) {		
		upper->buffer[i] = tolower(upper->buffer[i]);
	}

	return upper;
}

Vector* string_split(String* src, char delimiter) {
	Vector* vector = vector_new(1, (Duplicator) string_clone, (Destructor) string_free);
	size_t last_start = 0;

	for (size_t i = 0; i < src->length; i++) {
		if (src->buffer[i] == delimiter) {
			if (i > 0 && src->buffer[i - 1] != delimiter) {		
				vector_add(vector, string_from_substring(src->buffer, last_start, i - last_start));
			}
			last_start = i + 1;
		} else if (i == src->length - 1 && i > 0 && src->buffer[i - 1] != delimiter) {		
			vector_add(vector, string_from_substring(src->buffer, last_start, i + 1 - last_start));
		}
	}

	return vector;
}

// Simple hash function provided by Dan Bernstein 
size_t string_hash(String* src) {
	ASSERT_NONNULL(src);

	size_t hash = 5381;

	for (size_t i = 0; i < src->length; i++) {
		hash = (hash << 5) + src->buffer[i];
	}

	return hash;
}

bool string_equals(String* src, char* other) {
	ASSERT_NONNULL(src);
	ASSERT_NONNULL(other);

	return strcmp(src->buffer, other) == 0;
}

bool string_equals_ignore_case(String* src, char* other) {
	ASSERT_NONNULL(src);
	ASSERT_NONNULL(other);

	return strcasecmp(src->buffer, other) == 0;
}

bool string_equals_string(String* src, String* other) {
	ASSERT_NONNULL(other);
	return string_equals(src, other->buffer);
}

bool string_equals_string_ignore_case(String* src, String* other) {
	ASSERT_NONNULL(other);
	return string_equals_ignore_case(src, other->buffer);
}

int string_compare(const void* string, const void* other) {
	ASSERT_NONNULL(string);
	ASSERT_NONNULL(other);

	String *string_a = *(String**) string;
	String *string_b = *(String**) other;
	
	return strcmp(string_a->buffer, string_b->buffer);
}

void string_print(String* string) {
	printf("%s", string->buffer);
}

void string_println(String* string) {
	printf("%s\n", string->buffer);
}


