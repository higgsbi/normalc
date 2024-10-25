#ifndef NORMALC_ARRAY_H
#define NORMALC_ARRAY_H

#include <stdlib.h>
#include "../error/error.h"
#include "../safety/option.h"
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

#define DEFAULT_ARRAY { NULL, 0, 0, 0 }
OPTION_TYPE(Array, Array, array, DEFAULT_ARRAY)

/**
 * Array splices are references to windows within a Array.
 * This allows for reading of parts of an array without duplication.
 */
typedef struct {
	Array* original;
	size_t start;
	size_t count;	
} ArraySplice;

#define DEFAULT_ARRAY_SPLICE { NULL, 0, 0 }
OPTION_TYPE(ArraySplice, ArraySplice, array_splice, DEFAULT_ARRAY_SPLICE)

/**
 * Returns an array with each element being of size `element_size`.
 * An initial capacity can be used to avoid unnecessary reallocations
 */
Array* array_new(size_t capacity, size_t element_size);

/**
 * Returns a deep clone of the passed `array`
 */
Array* array_clone(Array* array);

/**
 * Frees the array as well as all of its elements
 */
void array_free(Array* array);

/**
 * Adds an element to the array. Does not free any
 * passed parameters
 */
void array_add(Array* array, void* element);

/** Helper function for array_add() for integer types */
void array_add_int(Array* array, int element);
/** Helper function for array_add() for long types */
void array_add_long(Array* array, long element);
/** Helper function for array_add() for shorttypes */
void array_add_short(Array* array, short element);
/** Helper function for array_add() for float types */
void array_add_float(Array* array, float element);
/** Helper function for array_add() for double types */
void array_add_double(Array* array, double element);
/** Helper function for array_add() for char types */
void array_add_char(Array* array, char element);
/** Helper function for array_add() for cstring types */
void array_add_cstring(Array* array, char* element);

/**
 * Sets the given element at the given index in the array.
 * Does not free any passed parameters.
 * If the index passed is larger than the array's current
 * count + 1, system will exit with error
 */
void array_set(Array* array, size_t index, void* element);

/** Helper function for array_set() for int types */
void array_set_int(Array* array, size_t index, int element);
/** Helper function for array_set() for long types */
void array_set_long(Array* array, size_t index, long element);
/** Helper function for array_set() for short types */
void array_set_short(Array* array, size_t index, short element);
/** Helper function for array_set() for float types */
void array_set_float(Array* array, size_t index, float element);
/** Helper function for array_set() for double types */
void array_set_double(Array* array, size_t index, double element);
/** Helper function for array_set() for char types */
void array_set_char(Array* array, size_t index, char element);
/** Helper function for array_set() for cstring types */
void array_set_cstring(Array* array, size_t index, char* element);

/** 
 * Returns a pointer to the element at the given index.
 * If index is greater than (array count - 1), system 
 * will exist with an error
 */
void* array_get(Array* array, size_t index);

/** Helper function for array_get() for integer types */
int array_get_int(Array* array, size_t index);
/** Helper function for array_get() for short types */
short array_get_short(Array* array, size_t index);
/** Helper function for array_get() for long types */
long array_get_long(Array* array, size_t index);
/** Helper function for array_get() for float types */
float array_get_float(Array* array, size_t index);
/** Helper function for array_get() for double types */
double array_get_double(Array* array, size_t index);
/** Helper function for array_get() for char types */
char array_get_char(Array* array, size_t index);
/** Helper function for array_get() for cstring types */
char* array_get_cstring(Array* array, size_t index);

/**
 * Removes and frees the element in an array at the given index. 
 */
void array_remove(Array* array, size_t index);

/**
 * Returns a splice of a given array with guaranteed null safety.
 * This allocates only `3 * size_t` bytes of data
 */
ArraySplice* array_splice_from(Array* array, size_t start, size_t count);

/**
 * Returns an element from a given splice at the given index
 */
void* array_splice_get(ArraySplice* splice, size_t index);

/** Helper function for array_get() for integer types */
int array_splice_get_int(ArraySplice* splice, size_t index);
/** Helper function for array_get() for short types */
short array_splice_get_short(ArraySplice* splice, size_t index);
/** Helper function for array_get() for long types */
long array_splice_get_long(ArraySplice* splice, size_t index);
/** Helper function for array_get() for float types */
float array_splice_get_float(ArraySplice* splice, size_t index);
/** Helper function for array_get() for double types */
double array_splice_get_double(ArraySplice* splice, size_t index);
/** Helper function for array_get() for char types */
char array_splice_get_char(ArraySplice* splice, size_t index);
/** Helper function for array_get() for cstring types */
char* array_splice_get_cstring(ArraySplice* splice, size_t index);

/**
 * Frees the splice. Does not free the referenced array
 */
void array_splice_free(ArraySplice* splice);

#endif
