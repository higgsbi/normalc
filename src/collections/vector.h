#ifndef NORMALC_VECTOR_H
#define NORMALC_VECTOR_H

#include <stdlib.h>
#include "../safety/option.h"
#include "../memory/memory.h"

/**
 * Vector defines a collection of heap allocated void pointers with a default duplicator 
 * and destructor method to allow for easy memory management.
 *
 * As opposed to the Array type, vectors should be used for complex objects that require
 * the use of the heap.
 */
typedef struct {
	void** data;
	size_t count;
	size_t capacity;
	Destructor destructor;
	Duplicator duplicator;
} Vector;

/**
 * Defines type safe functions for the Vector and VectorSplice type
 */
#define VECTOR_SAFE(type, type_name) \
    static inline void vector_##type_name##_add(Vector* vector, type* element) { \
        vector_add(vector, (void*) element); \
    } \
    static inline type* vector_##type_name##_get(Vector* vector, size_t index) { \
        return (type*) vector_get(vector, index); \
    } \
    static inline type* vector_##type_name##_get_clone(Vector* vector, size_t index) { \
        return (type*) vector_get_clone(vector, index); \
    } \
    static inline type* vector_##type_name##_remove(Vector* vector, size_t index) { \
        return (type*) vector_remove(vector, index); \
    } \
    static inline void vector_##type_name##_set(Vector* vector, size_t index, type* element) { \
        vector_set(vector, index, (void*) element); \
    } \
    static inline void vector_##type_name##_add_clone(Vector* vector, type* element) { \
        vector_add_clone(vector, (void*) element); \
    } \
    static inline void vector_##type_name##_set_clone(Vector* vector, size_t index, type* element) { \
        vector_set_clone(vector, index, (void*) element); \
    } \
    static inline type* vector_splice_##type_name##_get(VectorSplice* splice, size_t index) { \
        return (type*) vector_splice_get(splice, index); \
    } \

OPTION_TYPE(Vector*, Vector, vector, NULL)

/**
 * Vector splices are references to windows within a Vector.
 * This allows for reading of parts of an array without duplication.
 */
typedef struct {
	Vector* original;
	size_t start;
	size_t count;	
} VectorSplice;

OPTION_TYPE(VectorSplice*, VectorSplice, vector_splice, NULL)

/**
 * Returns a new array with the given initial capacity.
 * The duplicator and destructor cannot be null
 */
Vector* vector_new(
			size_t capacity,
			Duplicator duplicator,
			Destructor destructor
		);

/**
 * Returns a deep clone of the given vector without freeing the original
 */
Vector* vector_clone(Vector* vector);

/**
 * Frees the given vector and all of its elements
 */
void vector_free(Vector* vector);

/**
 * Adds the given element to the array and takes ownership of it
 */
void vector_add(Vector* vector, void* element);

/**
 * Sets the given element to the given index and takes ownership of it
 */
void vector_set(Vector* vector, size_t index, void* element);

/**
 * Returns a reference to the element at the given index.
 * Note: this reference should not be freed
 */
void* vector_get(Vector* vector, size_t index);

/**
 * Removes and returns an element at the given index.
 * If index is outside the bounds of the vector, the system will exist with an error.
 * Return type is guaranteed to not be null.
 * The user is expected to free the removed element
 */
void* vector_remove(Vector* vector, size_t index);

/**
 * Adds the given element to the end of the vector by cloning it.
 * This does not take ownership of the original variable, thus the user
 * is responsible for freeing it
 */
void vector_add_clone(Vector* vector, void* element);

/**
 * Sets the element at the given index in the vector by cloning it.
 * As opposed to vector_set(), this function does not take ownership of the
 * original variable, thus the ser is responsible for freeing it
 */
void vector_set_clone(Vector* vector, size_t index, void* element);

/**
 * Returns a deep copy of the element at the given index. Does not return
 * a reference like vectior_get(), thus the user is responsible for 
 * freeing the original variable.
 */
void* vector_get_clone(Vector* vector, size_t index);

/**
 * Sorts the vector via stdlib's qsort with the supplied comparator
 */
void vector_sort(Vector* vector, Comparator comparator);

/**
 * Removes the element from the vector at the given index and frees it.
 */
void vector_delete(Vector* vector, size_t index);

/**
 * Frees all the elements in this vector leaving the vector with a size of 0
 * but keeping the same capacity
 */
void vector_clear(Vector* vector);

/**
 * Returns a splice (shallow copy) of the original vector
 */
VectorSplice* vector_splice_from(Vector* vector, size_t start, size_t count);

/**
 * Returns the element from the splice at the given index. 
 */
void* vector_splice_get(VectorSplice* splice, size_t index);

/**
 * Frees the given splice
 */
void vector_splice_free(VectorSplice* splice);

#endif
