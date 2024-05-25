#include "standard/collections/array.h"
#include <stdio.h>

void test_basic();	
void test_char_array();
void test_removal();
void test_splice();

int main() {
	test_basic();	
	test_char_array();
	test_removal();
	test_splice();
	return 0;
}

void test_splice() {
	printf("\nTEST ARRAY SPLICE\n\n");
	Array* array = array_new(3, sizeof(char*));

	array_add_cstring(array, "Splice Element 1");	
	array_add_cstring(array, "Splice Element 2");	
	array_add_cstring(array, "Splice Element 2");	

	ArraySplice* splice = array_splice_from(array, 1, 2);

	for (size_t i = 0; i < splice->count; i++) {	
		printf("%s\n", array_splice_get_cstring(splice, i));
	}

	array_free(array);
	array_splice_free(splice);
}

void test_removal() {	
	printf("\nTEST ARRAY REMOVAL\n\n");
	Array* array = array_new(0, sizeof(char));

	array_add_char(array, 'W');	
	array_add_char(array, 'o');
	array_add_char(array, 'w');
	array_add_char(array, '!');

	printf("Before removal:\n");
	for (size_t i = 0; i < array->count; i++) {	
		printf("%c ", array_get_char(array, i));
	}

	array_remove(array, 3);

	printf("\nAfter removal:\n");
	for (size_t i = 0; i < array->count; i++) {	
		printf("%c ", array_get_char(array, i));
	}

	array_add_char(array, '?');

	printf("\nAfter adding back:\n");
	for (size_t i = 0; i < array->count; i++) {	
		printf("%c ", array_get_char(array, i));
	}

	printf("\n");
	
	array_free(array);
}

void test_char_array() {	
	printf("\nTEST ARRAY CHAR\n\n");
	Array* array = array_new(0, sizeof(char*));

	array_add_cstring(array, "Element 0 is a very long cstring");
	array_add_cstring(array, "Element 1");
	array_add_cstring(array, "Element 2");
	
	printf("%s\n", array_get_cstring(array, 0));
	printf("%s\n", array_get_cstring(array, 1));
	printf("%s\n", array_get_cstring(array, 2));

	array_free(array);
}

void test_basic() {
	printf("\nTEST ARRAY BASIC\n\n");
	Array* array = array_new(0, sizeof(long));

	array_set_long(array, 0, 5L);
	array_set_long(array, 1, 10L);
	array_add_long(array, 15L);
	
	printf("Index 0: %ld\n", array_get_long(array, 0));
	printf("Index 1: %ld\n", array_get_long(array, 1));
	printf("Index 2: %ld\n", array_get_long(array, 2));

	printf("Clone after freeing original:\n");

	Array* clone = array_clone(array);
	array_free(array);

	printf("Index 0: %ld\n", array_get_long(clone, 0));
	printf("Index 1: %ld\n", array_get_long(clone, 1));
	printf("Index 2: %ld\n", array_get_long(clone, 2));

	array_free(clone);
}
