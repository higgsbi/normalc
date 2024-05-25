#include "array.h"
#include <string.h>

void _array_try_expand(Array* array);
void _array_move_down(Array* array, size_t removed);

Array* array_new(size_t capacity, size_t element_size) {
	Array* array = allocate(sizeof(Array));
	array->capacity = capacity;
	array->count = 0;
	array->element_size = element_size;
	array->data = allocate(sizeof(void*) * capacity);

	return array;
}

Array* array_clone(Array* array) {
	ASSERT_NONNULL(array);

	Array* clone = allocate(sizeof(Array));
	clone->capacity = array->capacity;
	clone->count = array->count;
	clone->element_size = array->element_size;
	clone->data = allocate(sizeof(void*) * array->capacity);

	memcpy(clone->data, array->data, sizeof(void*) * array->count);

	return clone;
}

void array_free(Array* array) {
	free(array->data);
	free(array);
}

void array_add(Array* array, void* element) {
	ASSERT_NONNULL(array);
	ASSERT_NONNULL(element);

	_array_try_expand(array);
	memcpy(array->data + (array->count * array->element_size), element, array->element_size);
	array->count++;
}

void array_set(Array* array, size_t index, void* element) {
	ASSERT_NONNULL(array);
	ASSERT_NONNULL(element);

	if (index >= array->count) {	
		ASSERT_VALID_BOUNDS(array, (int) index, (int) array->count + 1);
		_array_try_expand(array);
		memcpy(array->data + (index * array->element_size), element, array->element_size);
		array->count++;
	} else {	
		memcpy(array->data + (index * array->element_size), element, array->element_size);
	}
}

void array_set_int(Array* array, size_t index, int element) {
	array_set(array, index, &element);
}

void array_set_long(Array* array, size_t index, long element) {	
	array_set(array, index, &element);
}

void array_set_short(Array* array, size_t index, short element) {
	array_set(array, index, &element);
}

void array_set_float(Array* array, size_t index, float element) {
	array_set(array, index, &element);
}

void array_set_double(Array* array, size_t index, double element) {
	array_set(array, index, &element);
}

void array_set_char(Array* array, size_t index, char element)  {
	array_set(array, index, &element);
}

void array_set_cstring(Array* array, size_t index, char* element) {
	array_set(array, index, &element);
}

void array_add_int(Array* array, int element) {
	array_add(array, &element);
}

void array_add_long(Array* array, long element) {
	array_add(array, &element);
}

void array_add_short(Array* array, short element) {
	array_add(array, &element);
}

void array_add_float(Array* array, float element) {
	array_add(array, &element);
}

void array_add_double(Array* array, double element) {
	array_add(array, &element);
}

void array_add_char(Array* array, char element) {
	array_add(array, &element);
}

void array_add_cstring(Array* array, char* element) {
	array_add(array, &element);
}

void* array_get(Array* array, size_t index) {
	ASSERT_NONNULL(array);
	ASSERT_VALID_BOUNDS(array, (int) index, (int) array->count);

	return array->data + (index * array->element_size);
}

int array_get_int(Array* array, size_t index) {
	return *(int*) array_get(array, index);
}

short array_get_short(Array* array, size_t index) {
	return *(short*) array_get(array, index);
}

long array_get_long(Array* array, size_t index) {
	return *(long*) array_get(array, index);
}

float array_get_float(Array* array, size_t index) {
	return *(float*) array_get(array, index);
}

double array_get_double(Array* array, size_t index) {
	return *(double*) array_get(array, index);
}

char array_get_char(Array* array, size_t index) {
	return *(char*) array_get(array, index);
}

char* array_get_cstring(Array* array, size_t index) {
	return *(char**) array_get(array, index);
}

void array_remove(Array* array, size_t index) {
	ASSERT_NONNULL(array);
	ASSERT_VALID_BOUNDS(array, (int) index, (int) array->count);

	_array_move_down(array, index);
	array->count--;
}

ArraySplice* array_splice_from(Array* array, size_t start, size_t count) {
	ASSERT_NONNULL(array);
	ASSERT_VALID_BOUNDS(array, (int) start, (int) array->count);
	ASSERT_VALID_BOUNDS(array, (int) (start + count - 1), (int) array->count);

	ArraySplice* splice = allocate(sizeof(Array));
	splice->start = start;
	splice->count = count;
	splice->original = array;

	return splice;
}
void* array_splice_get(ArraySplice* splice, size_t index) {
	ASSERT_NONNULL(splice);
	ASSERT_NONNULL(splice->original);
	ASSERT_VALID_BOUNDS(splice, (int) (index), (int) splice->count);

	size_t offset = ((splice->start + index) * splice->original->element_size);
	return splice->original->data + offset;
}

int array_splice_get_int(ArraySplice* splice, size_t index) {
	return *(int*) array_splice_get(splice, index);
}

short array_splice_get_short(ArraySplice* splice, size_t index) {
	return *(short*) array_splice_get(splice, index);
}

long array_splice_get_long(ArraySplice* splice, size_t index) {
	return *(long*) array_splice_get(splice, index);
}

float array_splice_get_float(ArraySplice* splice, size_t index) {
	return *(float*) array_splice_get(splice, index);
}

double array_splice_get_double(ArraySplice* splice, size_t index) {
	return *(double*) array_splice_get(splice, index);
}

char array_splice_get_char(ArraySplice* splice, size_t index) {
	return *(char*) array_splice_get(splice, index);
}

char* array_splice_get_cstring(ArraySplice* splice, size_t index) {
	return *(char**) array_splice_get(splice, index);
}

void array_splice_free(ArraySplice* splice) {
	ASSERT_NONNULL(splice);
	free(splice);
}

void _array_try_expand(Array* array) {
	if (array->count + 1 < array->capacity) {
		return;
	}

	if (array->capacity == 0) {	
		array->capacity = 2;
	} else {	
		array->capacity = array->capacity * 2;
	}

	array->data = reallocate(array->data, sizeof(void*) * array->capacity);
}

void _array_move_down(Array* array, size_t removed) {
    size_t elements_count = array->count - removed - 1;
    size_t start_offset = (removed + 1) * array->element_size;
    size_t dest_offset = removed * array->element_size;
    memcpy(array->data + dest_offset, array->data + start_offset, elements_count * array->element_size);
}


