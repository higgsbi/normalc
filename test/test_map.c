#include <normalc/collections/map.h>
#include <normalc/string/string.h>
#include <normalc/collections/vector.h>
#include <normalc/error/error.h>
#include <normalc/memory/memory.h>
#include <stdio.h>

void test_memory();
void test_complex_values();
void test_removal();
void test_safety();

int main() {
	test_memory();
	test_removal();
	test_complex_values();
	test_safety();
	return 0;
}

MAP_SAFE(String, String, string)

void test_safety() {
	Map* map = map_new(
			2,
			(Hasher) string_hash,
			(Comparator) string_equals_string_ignore_case,
			(Destructor) string_free, 
			(Destructor) string_free, 
			(Duplicator) string_clone, 
			(Duplicator) string_clone
		);

	map_string_insert(map, string_from("Safe Key 1"), string_from("Safe Value 1"));
	map_string_insert(map, string_from("Safe Key 2"), string_from("Safe Value 2"));

	MapSplice* splice = map_splice_new(map);

	for (size_t i = 0; i < splice->count; i++) {
		string_println(map_splice_string_get_value(splice, i));
	}
	
	map_splice_free(splice);
	map_free(map);
}

void test_removal() {
	Map* map = map_new(
				15,
				(Hasher) string_hash,
				(Comparator) string_equals_string_ignore_case,
				(Destructor) string_free, 
				(Destructor) string_free, 
				(Duplicator) string_clone, 
				(Duplicator) string_clone
			);


	for (size_t i = 0; i < 10; i++) {
		map_insert(map, string_from_format("Key %zu", i), string_from_format("Value %zu", i));	
	}

	for (size_t i = 0; i < 5; i++) {
		map_delete(map, string_from_format("Key %zu", i), true);
	}

	MapSplice* splice = map_splice_new(map);

	for (size_t i = 0; i < splice->count; i++) {	
		Entry* entry = map_splice_get_entry(splice, i);
		String* key = entry->key;
		String* value = entry->value;	

		printf("Pair { %s -> %s } \n", key->buffer, value->buffer);
	}

	map_splice_free(splice);
	map_free(map);
}

void test_complex_values() {
	Map* map = map_new(
				10,
				(Hasher) string_hash,
				(Comparator) string_equals_string_ignore_case,
				(Destructor) string_free, 
				(Destructor) vector_free, 
				(Duplicator) string_free, 
				(Duplicator) vector_clone
			);

	for (size_t i = 0; i < 5; i++) {
		Vector* vector = vector_new(5, (Duplicator) string_clone, (Destructor) string_free);
		vector_add(vector, string_from_format("Vector index %zu, value: %zu", i, i));
		vector_add(vector, string_from_format("Vector index %zu, value: %zu", i, i * 2));
		vector_add(vector, string_from_format("Vector index %zu, value: %zu", i, i * 4));

		map_insert(map, string_from_format("Map key %zu", i), vector);	
	}

	MapSplice* splice = map_splice_new(map);
	for (size_t i = 0; i < splice->count; i++) {
		Entry* entry = map_splice_get_entry(splice, i);
		String* key = entry->key;
		Vector* value = entry->value;
		string_println(key);

		for (size_t j = 0; j < value->count; j++) {
			string_println(vector_get(value, j));
		}
	}

	map_splice_free(splice);
	map_free(map);
}

void test_memory() {
	Map* map = map_new(
				0,
				(Hasher) string_hash,
				(Comparator) string_equals_ignore_case,
				(Destructor) string_free, 
				(Destructor) string_free, 
				(Duplicator)string_clone, 
				(Duplicator) string_clone
			);

	Map* clone = map_clone(map);
	map_free(clone);

	map_free(map);
}
