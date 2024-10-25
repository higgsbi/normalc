#ifndef NORMALC_STRING_H
#define NORMALC_STRING_H

#include <string.h>
#include "../error/error.h"
#include "../collections/vector.h"

/**
 * String defines a immutable null-terminated string with a predefined length
 */
typedef struct {
	char* buffer;
	size_t length;	
} String;

#define DEFAULT_STRING NULL
OPTION_TYPE(String*, String, string, DEFAULT_STRING)

/**
 * Returns a new empty string
 */
String* string_empty();

/**
 * Returns a string from the given cstring
 */
String* string_from(char* src);

/**
 * Returns a string using C's `vasprintf` function
 */
String* string_from_format(char* format, ...);

/**
 * Returns a string from a sub length of a cstring.
 * If the given start and count parameters are outside the source's capacity,
 * the system will exit with an error
 */
String* string_from_substring(char* source, size_t start, size_t count);

/**
 * Returns a deep clone of the given string
 */
String* string_clone(String* string);

/**
 * Frees the given string
 */
void string_free(String* string);

/**
 * Returns the first index of a query for the given string.
 * Returns -1 if none can be found
 */
int string_index_of(String* string, char query);

/**
 * Returns the last index of a query for the given string.
 * Returns -1 if none can be found
 */
int string_index_of_last(String* string, char query);

/**
 * Returns the nth index of a query for the given string.
 * The "nth index" starts at 1
 * Returns -1 if none can be found
 */
int string_nth_index_of_last(String* string, size_t n, char query);

/**
 * Returns the nth last index of a query for the given string.
 * The "nth index" starts at 1
 * Returns -1 if none can be found
 */
int string_nth_index_of(String* string, size_t n, char query);

/**
 * Returns the first index of the first letter of a query for the given string.
 * Returns -1 if none can be found
 */
int string_index_of_string(String* string, char* query);

/*
 * Returns the index of the last instance of a query for the given string.
 * The index represents the 0th index of the given query
 * Returns -1 if none can be found
 */
int string_index_of_last_string(String* string, char* query);

/**
 * Returns a string with any instance of `replaced` with `replacer`
 */
String* string_replace(String* string, char* replaced, char* replacer);

/**
 * Returns a substring of a string given a starting index and a length.
 * If the start and length reach outside the string's capacity, the 
 * system will exist with an error
 */
String* string_sub(String* string, size_t start, size_t length);

/**
 * Returns the string with all letters turned into uppercase.
 * Uses cctype's `toupper()` function
 */
String* string_uppercase(String* string);

/**
 * Returns the string with all letters turned into lowercase.
 * Uses cctype's `tolower()` function
 */
String* string_lowercase(String* string);

/**
 * Returns the string split into parts delimited by the given delimiter.
 * If the deliminter occurs twice or more in a row, this substring is ignored.
 * If there is no instance of the delimiter, the entire string is returned at 
 * the vector's first index.
 */
Vector* string_split(String* string, char delimiter);

/**
 * Returns a hash of a given string using Dan Bernstein's hash function
 */
size_t string_hash(String* string);

/**
 * Returns true if the string contains one or more instances of the given character
 */
bool string_contains(String* string, char contains);

/**
 * Returns true if the string contains one or more instances of the given cstring
 */
bool string_contains_string(String* string, char* contains);

/**
 * Returns true if the string's buffer equals the given cstring exactly.
 * Uses C's strcmp
 */
bool string_equals(String* string, char* other);

/**
 * Returns true if the string's buffer equals the given cstring ignoring case.
 * Uses C's strcasecmp
 */
bool string_equals_ignore_case(String* string, char* other);

/**
 * A wrapper function for `string_equals()`
 */
bool string_equals_string(String* string, String* other);

/**
 * A wrapper function for `string_equals_ignore_case()`
 */
bool string_equals_string_ignore_case(String* string, String* other);

/**
 * Prints the given string's buffer terimnated with a new line
 */
void string_print(String* string);

/**
 * Prints the given string's buffer 
 */
void string_println(String* string);

#endif

