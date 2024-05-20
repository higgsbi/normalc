#ifndef STD_STRING_H
#define STD_STRING_H

#include <string.h>
#include "../error/error.h"

/**
 * String defines a immutable null-terminated string with a predefined length
 */
typedef struct {
	char* buffer;
	size_t length;	
} String;

String* string_empty();
String* string_from(char* src);
String* string_from_format(char* format, ...);
String* string_clone(String* src);
void string_free(String* string);

int string_index_of(String* string, char query);
int string_index_of_last(String* string, char query);
int string_index_of_string(String* string, char* query);
int string_index_of_last_string(String* string, char* query);

String* string_replace(String* src, char* replaced, char* replacer);
String* string_sub(String* src, size_t start, size_t end);
String* string_uppercase(char* src);
String* string_lowercase(char* src);

bool string_contains(String* src, char contains);
bool string_contains_string(String* src, char* contains);

bool string_equals(String* src, char* other);
bool string_equals_ignore_case(String* src, char* other);

void string_print(String* string);
void string_println(String* string);

#endif

