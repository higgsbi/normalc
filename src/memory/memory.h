#ifndef STD_MEMORY_H
#define STD_MEMORY_H

#include "stdlib.h"
#include "stdio.h"

static void* alloc(size_t size) {
	void* pointer = malloc(size);
	
	if (!pointer) {
		printf("Failed to allocate variable of size %zu", size);
		exit(EXIT_FAILURE);
	}

	return pointer;
}

#endif
