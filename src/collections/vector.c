#include "vector.h"
#include "../memory/memory.h"

void _vector_try_expand(Vector* vector);
void _vector_move_down(Vector* vector, size_t removed);

Vector* vector_new(size_t capacity, Duplicator duplicator, Destructor destructor) {	
	ASSERT_NONNULL(duplicator);
	ASSERT_NONNULL(destructor);

	Vector* vector = allocate(sizeof(Vector));

	vector->capacity = capacity;
	vector->count = 0;
	vector->data = allocate(sizeof(void*) * capacity);
	vector->destructor = destructor;
	vector->duplicator = duplicator;

	return vector;
}

Vector* vector_clone(Vector* vector) {
	ASSERT_NONNULL(vector);

	Vector* cloned = allocate(sizeof(Vector));

	cloned->capacity = vector->capacity;
	cloned->count = vector->count;
	cloned->destructor = vector->destructor;
	cloned->duplicator = vector->duplicator;

	cloned->data = allocate(sizeof(void*) * vector->capacity);
	for (size_t i = 0; i < vector->count; i++) {	
		cloned->data[i] = vector->duplicator(vector->data[i]);
	}

	return cloned;
}

void vector_free(Vector* vector) {
	ASSERT_NONNULL(vector);

	for (size_t i = 0; i < vector->count; i++) {
		vector->destructor(vector->data[i]);
	}
	free(vector->data);
	free(vector);
}

void vector_add(Vector* vector, void* element) {
	ASSERT_NONNULL(vector);
	ASSERT_NONNULL(element);

	_vector_try_expand(vector);
	vector->data[vector->count] = element;
	vector->count++;
}

void vector_set(Vector* vector, size_t index, void* element) {
	ASSERT_NONNULL(vector);
	ASSERT_NONNULL(element);

	if (index >= vector->count) {	
		ASSERT_VALID_BOUNDS(vector, (int) index, (int) vector->count + 1);
		_vector_try_expand(vector);
		vector->data[vector->count] = element;
		vector->count++;
	} else {	
		vector->data[index] = element;
	}	
}

void vector_add_clone(Vector* vector, void* element) {
	vector_add(vector, vector->duplicator(element));	
}
	
void vector_set_clone(Vector* vector, size_t index, void* element) {
	vector_set(vector, index, vector->duplicator(element));	
}

void* vector_get_clone(Vector* vector, size_t index) {
	return vector->duplicator(vector_get(vector, index));
}

void* vector_get(Vector* vector, size_t index) {
	ASSERT_NONNULL(vector);
	ASSERT_VALID_BOUNDS(vector, (int) index, (int) vector->count);

	return vector->data[index];
}

void* vector_remove(Vector* vector, size_t index) {
	ASSERT_NONNULL(vector);
	ASSERT_VALID_BOUNDS(vector, (int) index, (int) vector->count);

	void* value = vector->data[index];
	_vector_move_down(vector, index);
	vector->count--;

	return value;
}


void vector_delete(Vector* vector, size_t index) {
	void* value = vector_remove(vector, index); 
	vector->destructor(value);
}

VectorSplice* vector_splice_from(Vector* vector, size_t start, size_t count) {
	ASSERT_NONNULL(vector);
	ASSERT_VALID_BOUNDS(collection, (int) start, (int) vector->count);
	ASSERT_VALID_BOUNDS(collection, (int) (start + count - 1), (int) vector->count);

	VectorSplice* splice = allocate(sizeof(VectorSplice));
	splice->start = start;
	splice->count = count;
	splice->original = vector;

	return splice;
}

void* vector_splice_get(VectorSplice* splice, size_t index) {
	ASSERT_NONNULL(splice);
	ASSERT_NONNULL(splice->original);
	ASSERT_VALID_BOUNDS(splice, (int) (index), (int) splice->count);

	return splice->original->data[splice->start + index];
}

void vector_splice_free(VectorSplice* splice) {
	ASSERT_NONNULL(splice);
	free(splice);
}

void _vector_move_down(Vector* vector, size_t removed) {
	for (size_t i = removed; i < vector->count - 1; i++) {
		// Faster for large data types to move the pointer pointers back one, instead of copying data
		vector->data[i] = vector->data[i + 1];
	}
}

void _vector_try_expand(Vector* vector) {
	if (vector->count + 1 < vector->capacity) {
		return;	
	}
	
	if (vector->capacity == 0) {
		vector->capacity = 2;
	} else {	
		vector->capacity = vector->capacity * 2;
	}

	vector->data = reallocate(vector->data, sizeof(void*) * vector->capacity);
}


