#ifndef STD_ARRAY_H
#define STD_ARRAY_H

#include <stdlib.h>
#include "../error/error.h"
#include "../memory/memory.h"

/**
 * Array defines a collection of opaque pointers to some generic data type. 
 * Unlike Vector, array's do not support heap allocated objects and assumes
 * every value is the same size
 */
typedef struct {
	void* data;
	size_t count;
	size_t capacity;
	size_t element_size;
} Array;

/**
 * Array splices are references to windows within a Array.
 * This allows for reading of parts of an array without duplication.
 */
typedef struct {
	Array* original;
	size_t start;
	size_t count;	
} ArraySplice;

Array* array_new(size_t capacity, size_t element_size);
Array* array_clone(Array* array);
void array_free(Array* array);

void array_add(Array* array, void* element);
void array_add_int(Array* array, int element);
void array_add_long(Array* array, long element);
void array_add_short(Array* array, short element);
void array_add_float(Array* array, float element);
void array_add_double(Array* array, double element);
void array_add_char(Array* array, char element);
void array_add_cstring(Array* array, char* element);

void array_set(Array* array, size_t index, void* element);
void array_set_int(Array* array, size_t index, int element);
void array_set_long(Array* array, size_t index, long element);
void array_set_short(Array* array, size_t index, short element);
void array_set_float(Array* array, size_t index, float element);
void array_set_double(Array* array, size_t index, double element);
void array_set_char(Array* array, size_t index, char element);
void array_set_cstring(Array* array, size_t index, char* element);

void* array_get(Array* array, size_t index);
int array_get_int(Array* array, size_t index);
short array_get_short(Array* array, size_t index);
long array_get_long(Array* array, size_t index);
float array_get_float(Array* array, size_t index);
double array_get_double(Array* array, size_t index);
char array_get_char(Array* array, size_t index);
char* array_get_cstring(Array* array, size_t index);

void array_remove(Array* array, size_t index);

ArraySplice* array_splice_from(Array* array, size_t start, size_t count);

void* array_splice_get(ArraySplice* splice, size_t index);
int array_splice_get_int(ArraySplice* splice, size_t index);
short array_splice_get_short(ArraySplice* splice, size_t index);
long array_splice_get_long(ArraySplice* splice, size_t index);
float array_splice_get_float(ArraySplice* splice, size_t index);
double array_splice_get_double(ArraySplice* splice, size_t index);
char array_splice_get_char(ArraySplice* splice, size_t index);
char* array_splice_get_cstring(ArraySplice* splice, size_t index);

void array_splice_free(ArraySplice* splice);

#endif
