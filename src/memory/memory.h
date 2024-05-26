#ifndef STD_MEMORY_H
#define STD_MEMORY_H

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

/**
 * Destructor defines a function that takes in an opaque pointer and returns nothing
 */
typedef void (*Destructor) (void*);

/**
 * Duplicator defines a function that takes in an opaque pointer and returns a deep clone 
 * of that pointer
 */
typedef void* (*Duplicator) (void*);

/**
 * Hasher defines a function that takes in an opaque pointer and returns a size_t hash
 */
typedef size_t (*Hasher) (void*);

/**
 * Comparator defines a function that takes in two opaque pointers of the same time and
 * returns true if same, and false if different
 */
typedef bool (*Comparator) (void*, void*);

/**
 * Wrapper around malloc which exists system and prints debug message on failure
 */
static void* allocate(size_t size) {
	void* pointer = malloc(size);
	
	if (!pointer) {
		printf("Failed to allocate variable of size %zu", size);
		exit(EXIT_FAILURE);
	}

	return pointer;
}

/**
 * Wrapper around realloc which exists system and prints debug message on failure
 */
static void* reallocate(void* original, size_t size) {
	void* pointer = realloc(original, size);

	if (!pointer) {
		printf("Failed to realloc variable of size %zu", size);
		exit(EXIT_FAILURE);
	}

	return pointer;
}

#endif
