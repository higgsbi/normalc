#include <normalc/collections/vector.h>
#include <normalc/string/string.h>
#include <normalc/memory/memory.h>
#include <stdio.h>

void test_memory();
void test_splice();

int main() {
	test_memory();
	test_splice();
	return 0;
}

void test_splice() {
	printf("\n--TESTING VEC SPLICE--\n\n");
	Vector* vector = vector_new(
				5, 
				(Duplicator) string_clone, 
				(Destructor) string_free
			);

	for (int i = 0; i < 5; i++) {
		vector_add(vector, string_from_format("Index: %d", i));	
	}	

	printf("Output original: \n");
	for (int i = 0; i < vector->count; i++) {
		string_println(vector_get(vector, i));
	}

	printf("\nOutput splice from index 2: \n");

	VectorSplice* splice = vector_splice_from(vector, 2, vector->count - 2);
	for (int i = 0; i < splice->count; i++) {
		string_println(vector_splice_get(splice, i));
	}

	vector_splice_free(splice);
	vector_free(vector);
}

void test_memory() {
	printf("\n--TESTING VEC MEMORY--\n\n");

	Vector* vector = vector_new(
				0, 
				(Duplicator) string_clone, 
				(Destructor)string_free
			);

	vector_add(vector, string_from("Hello, this is a test"));
	string_println((String*) vector_get(vector, 0));

	Vector* clone = vector_clone(vector);
	string_println((String*) vector_get(vector, 0));

	for (int i = 1; i < 11; i++) {
		vector_add(vector, string_from_format("Iteration: %d", i));
		string_println((String*) vector_get(vector, i));
	}	

	String* stolen = (String*) vector_remove(vector, 2);
	printf("\bStolen value:\n");
	string_println(stolen);
	vector->destructor(stolen);

	printf("\nPrinting out vector\n");
	for (int i = 0; i < vector->count; i++) {
		string_println(vector_get(vector, i));
	}

	vector_free(clone);	
	vector_free(vector);	
}
