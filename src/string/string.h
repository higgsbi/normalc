#ifndef NORMALC_STRING_H
#define NORMALC_STRING_H

#include "../collections/vector.h"

/**
 * @Member char* buffer: null terminated heap allocated buffer
 * @Member size_t length: length of string not including null terminator 
 * @Note String defines an immutable null-terminated string with a predefined length
 */
typedef struct {
	char* buffer;
	size_t length;	
} String;

OPTION_TYPE(String*, String, string, NULL)

/**
 * @Type String
 * @Returns String: a heap allocated string with only a null terminator
 */
String* string_empty();

/**
 * @Type String
 * @Param char* src: parameter from which to build a heap allocated string
 * @Returns String*: a heap-allocated null-terminated immutable string 
 * @Note Parameter src is not mutated nor freed
 */

String* string_from(char* src);

/**
 * @Type String
 * @Param char* format: format string with which to format the varargs
 * @Param varargs: arguments formatted via the format parameter and stdarg.h's vaspring' function
 * @Returns String*: a heap-allocated null-terminated immutable string using stdarg.h's vasprintf` function
 * @Note This function is helpful for concatenation of strings
 * @Note This function does not mutate nor free the given parameters
 */
String* string_from_format(char* format, ...);

/**
 * @Type String
 * @Param char* source: source to substringed
 * @Param size_t start: inclusive substring start
 * @Param size_t count: number of characters after start
 * @Returns String*: a string from a sub length of a cstring.
 * @Note If the given start and count parameters are outside the source's capacity,
 * the system will exit with an error
 */
String* string_sub_cstring(char* source, size_t start, size_t count);

/**
 * @Type String
 * @Param String* string: cloneable string
 * @Returns String*: a deep clone of the given string
 */
String* string_clone(String* string);

/**
 * @Type String
 * @Param String* string: string to free
 * @Note Frees the given string
 */
void string_free(String* string);

/**
 * @Type String
 * @Param String* string: string in which to search
 * @Param char query: character to find 
 * @Returns int: the first index of a query
 * @Note Returns -1 if none can be found
 */
int string_index_of(String* string, char query);

/**
 * @Type String
 * @Param String* string: string in which to search
 * @Param char query: character to find 
 * @Returns int: the first index of a query from the back of the string
 * @Note Returns -1 if none can be found
 */
int string_index_of_last(String* string, char query);

/**
 * @Type String
 * @Param String* string: string in which to search
 * @Param size_t n: how many times query should be seen before returning
 * @Param char query: character to find 
 * @Returns int: the first index of a query given n iterations
 * @Note the nth parameter should be non-zero with an input of 1 representing the first match
 * @Note Returns -1 if none can be found
 */
int string_nth_index_of(String* string, size_t n, char query);

/**
 * @Type String
 * @Param String* string: string in which to search
 * @Param char* query: string to match
 * @Returns int: the first index of the substring matching the given query 
 * @Note E.g., 'bar' in 'foobar' returns 3
 * @Note Returns -1 if none can be found
 */
int string_index_of_string(String* string, char* query);

/*
 * @Type String
 * Returns the index of the last instance of a query for the given string.
 * The index represents the 0th index of the given query
 * Returns -1 if none can be found
 */

/**
 * @Type String
 * @Param String* string: string in which to search
 * @Param char* query: string to match
 * @Returns int: the first index of the substring matching the given query searching backwards
 * @Note E.g., 'bar' in 'foobarbar' returns 7
 * @Note Returns -1 if none can be found
 */
int string_index_of_last_string(String* string, char* query);

/**
 * @Type String
 * @Param String* string: string in which to search
 * @Param size_t n: how many times query should be seen before returning
 * @Param char query: character to find 
 * @Returns int: the first index of a query given n iterations
 * @Note the nth parameter should be non-zero with an input of 1 representing the first match
 * from the back of the string
 * @Note Returns -1 if none can be found
 */
int string_nth_index_of_last(String* string, size_t n, char query);

/**
 * @Type String
 * @Param String string: string to modify
 * @Param char* replaced: string to be replaced
 * @Param char* replacer: replacer to replace replaced
 * @Returns a new heap allocated string
 * @Note replacer can be a blank string, but both replaced and replacer must be non-null
 */
String* string_replace(String* string, char* replaced, char* replacer);

/**
 * @Type String
 * @Param String* source: source to substringed
 * @Param size_t start: inclusive substring start
 * @Param size_t count: number of characters after start
 * @Returns String*: a string from a sub length of a string
 * @Note If the given start and count parameters are outside the source's capacity,
 * the system will exit with an error
 */
String* string_substring(String* string, size_t start, size_t length);

/**
 * @Type String
 * Returns the string with all letters turned into uppercase.
 * Uses cctype's `toupper()` function
 */
String* string_uppercase(String* string);

/**
 * @Type String
 * Returns the string with all letters turned into lowercase.
 * Uses cctype's `tolower()` function
 */
String* string_lowercase(String* string);

/**
 * @Type String
 * Returns the string split into parts delimited by the given delimiter.
 * If the deliminter occurs twice or more in a row, this substring is ignored.
 * If there is no instance of the delimiter, the entire string is returned at 
 * the vector's first index.
 */
Vector* string_split(String* string, char delimiter);

/**
 * @Type String
 * Returns a hash of a given string using Dan Bernstein's hash function
 */
size_t string_hash(String* string);

/**
 * @Type String
 * Returns true if the string contains one or more instances of the given character
 */
bool string_contains(String* string, char contains);

/**
 * @Type String
 * Returns true if the string contains one or more instances of the given cstring
 */
bool string_contains_string(String* string, char* contains);

/**
 * @Type String
 * Returns true if the string's buffer equals the given cstring exactly.
 * Uses C's strcmp
 */
bool string_equals(String* string, char* other);

/**
 * @Type String
 * Returns true if the string's buffer equals the given cstring ignoring case.
 * Uses C's strcasecmp
 */
bool string_equals_ignore_case(String* string, char* other);

/**
 * @Type String
 * A wrapper function for `string_equals()`
 */
bool string_equals_string(String* string, String* other);

/**
 * @Type String
 * A wrapper function for `string_equals_ignore_case()`
 */
bool string_equals_string_ignore_case(String* string, String* other);

/**
 * @Type String
 * Returns 0 if string == other, negative if string is < other, positive if other > string
 *
 * Note: this uses unsafe void pointers to match the signature of stdlib's `qsort()` function
 */
int string_compare(const void* string, const void* other);

/**
 * @Type String
 * Prints the given string's buffer terimnated with a new line
 */
void string_print(String* string);

/**
 * @Type String
 * Prints the given string's buffer 
 */
void string_println(String* string);

#endif

