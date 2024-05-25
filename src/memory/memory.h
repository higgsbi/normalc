#ifndef STD_MEMORY_H
#define STD_MEMORY_H

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

typedef void (*Destructor) (void*);
typedef void* (*Duplicator) (void*);
typedef size_t (*Hasher) (void*);
typedef bool (*Comparator) (void*, void*);

static void* allocate(size_t size) {
	void* pointer = malloc(size);
	
	if (!pointer) {
		printf("Failed to allocate variable of size %zu", size);
		exit(EXIT_FAILURE);
	}

	return pointer;
}

static void* reallocate(void* original, size_t size) {
	void* pointer = realloc(original, size);

	if (!pointer) {
		printf("Failed to realloc variable of size %zu", size);
		exit(EXIT_FAILURE);
	}

	return pointer;
}

#endif
