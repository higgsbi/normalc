#include <normalc/string/string.h>
#include <normalc/string/string_builder.h>
#include <stdio.h>
#include <time.h>

void test_builder();
void test_string();
void test_split();
void test_substrings();

int main() {
	// test_string();
	test_builder();
	// test_split();
	// test_substrings();
	return 0;
}

void test_split() {
	printf("\n STRING SPLIT TESTS\n\n");

	String* string = string_from("---Message--Separated-By--Dashes--With-Final-Word");
	Vector* split = string_split(string, '-');

	for (size_t i = 0; i < split->count; i++) {
		string_println(vector_get(split, i));		
	}

	vector_free(split);
	string_free(string);
}

void test_substrings() {
	printf("\n STRING SUBSTRING TESTS\n\n");
	String* data = string_from("List of elements: Hydrogen, Helium, Sulfur\nList of places: China, Korea, Japan\n");
	
	int start_elements = string_index_of(data, ':') + 2;
	int start_places = string_index_of_last(data, ':') + 2;

	int end_elements = string_index_of(data, '\n');
	int end_places = string_index_of_last(data, '\n');

	String* elements = string_substring(data, start_places, end_places - start_places);
	String* places = string_substring(data, start_elements, end_elements - start_elements);

	string_println(data);
	string_println(elements);
	string_println(places);

	string_free(data);
	string_free(elements);
	string_free(places);
}

void test_builder() {
	printf("\nSTRING BUILDER TESTS\n\n");
	String* string = string_from("Hello");
	string_println(string);

	StringBuilder* builder = string_builder_from(string->buffer);
	string_builder_append(builder, ".World");
	string_builder_append(builder, ".Appended");
	string_builder_append(builder, ".Message");

	String* built = string_builder_build(builder);
	string_println(built);

	double time_elapsed = 0.0;

    for (size_t rounds = 0; rounds < 50000; rounds++) {
	    clock_t start = clock();
    	StringBuilder* large = string_builder_new();
    	for (size_t i = 0; i < 100; i++) {
			string_builder_append(large, "test");
		}
	    clock_t end = clock();
	    time_elapsed += ((double) (end - start)) / CLOCKS_PER_SEC;
	    string_builder_free(large);
    }

    printf("Large builder append timing: %fs\n", time_elapsed);
	
	string_free(string);
	string_free(built);
	string_builder_free(builder);
}

void test_string() {
	printf("\nSTRING TESTS\n");
	// Memory check
	String* string = string_empty();
	string_println(string);
	string_free(string);

	// From check
	String* hello = string_from("Hello World! This is a message with two Hello's.");
	string_println(hello);

	// Clone check
	String* clone = string_clone(hello);
	string_println(clone);

	// Compare check
	printf("\nOriginal equals clone: %i\n\n", string_equals(hello, clone->buffer));

	// Format check
	String* formatted = string_from_format("This is a message: %s. This is an int %i:", "Hello", 20);
	string_println(formatted);

	// Index check
	String* separated = string_from("A.B.C.D");	
	printf("Index First: %i\n", string_index_of(separated, '.'));
	printf("Index Last: %i\n", string_index_of_last(separated, '.'));

	// Index string check
	String* separated_string = string_from("A...B...C...D");
	printf("Index First String: %i\n", string_index_of_string(separated_string, "..."));
	printf("Index Last String: %i\n", string_index_of_last_string(separated_string, "..."));

	// Replace check
	String* replaced = string_replace(hello, "Hello", "Goodbye");
	string_println(replaced);

	// free
	string_free(formatted);
	string_free(replaced);
	string_free(separated);
	string_free(separated_string);
	string_free(hello);
	string_free(clone);
}
