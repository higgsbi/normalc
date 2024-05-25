#ifndef STD_VECTOR_H
#define STD_VECTOR_H

#include <stdlib.h>
#include "../error/error.h"
#include "../memory/memory.h"

/**
 * Vector defines a collection of heap allocated opaque pointers with a default duplicator 
 * and destructor method to allow for easy memory management.
 */
typedef struct {
	void** data;
	size_t count;
	size_t capacity;
	Destructor destructor;
	Duplicator duplicator;
} Vector;

/**
 * Vector splices are references to windows within a Vector.
 * This allows for reading of parts of an array without duplication.
 */
typedef struct {
	Vector* original;
	size_t start;
	size_t count;	
} VectorSplice;

Vector* vector_new(
			size_t capacity, 
			Duplicator duplicator, 
			Destructor destructor
		);

Vector* vector_clone(Vector* vector);
void vector_free(Vector* vector);

// Reference based
void vector_add(Vector* vector, void* element);
void vector_set(Vector* vector, size_t index, void* element);
void* vector_get(Vector* vector, size_t index);
void* vector_remove(Vector* vector, size_t index);

void vector_add_clone(Vector* vector, void* element);
void vector_set_clone(Vector* vector, size_t index, void* element);
void* vector_get_clone(Vector* vector, size_t index);
void vector_remove_discard(Vector* vector, size_t index);

VectorSplice* vector_splice_from(Vector* vector, size_t start, size_t count);
void* vector_splice_get(VectorSplice* splice, size_t index);
void vector_splice_free(VectorSplice* splice);

#endif
