#include "standard/collections/linked_list.h"
#include "standard/string/string.h"
#include <standard/memory/memory.h>
#include <stdio.h>

void test_memory();
void test_pop();

int main() {
	test_memory();
	test_pop();
	return 0;
}

void test_pop() {
	printf("\n--TEST LINKED LIST POP--\n\n");
	LinkedList* list = linked_list_new(
				linked_node_new(string_from("Element 0"), NULL), 
				(Destructor) string_free, 
				(Duplicator) string_clone	
			);

	linked_list_push(list, linked_node_new(string_from("Element 1"), NULL));
	linked_list_push(list, linked_node_new(string_from("Element 2"), NULL));
	linked_list_push(list, linked_node_new(string_from("Element 3"), NULL));

	Node* iterator = list->head;
	while (iterator != NULL) {
		string_println((String*) iterator->element);
		iterator = iterator->next;
	}

	printf("Post popping tail - 1, head and tail\n");
	linked_list_pop_discard(list, 2);
	linked_list_pop_head_discard(list);
	linked_list_pop_tail_discard(list);

	iterator = list->head;
	while (iterator != NULL) {
		string_println((String*) iterator->element);
		iterator = iterator->next;
	}
	
	linked_list_free(list);
}

void test_memory() {
	printf("\n--TEST LINKED LIST MEMORY--\n\n");
	LinkedList* list = linked_list_new(
				linked_node_new(string_from("Element 0"), NULL), 
				(Destructor) string_free, 
				(Duplicator) string_clone	
			);

	linked_list_push(list, linked_node_new(string_from("Element 1"), NULL));

	Node* iterator = list->head;
	while (iterator != NULL) {
		string_println((String*) iterator->element);
		iterator = iterator->next;
	}

	LinkedList* clone = linked_list_clone(list);
	linked_list_push(clone, linked_node_new(string_from("Element 1 Appended"), NULL));

	iterator = clone->head;

	while (iterator != NULL) {
		string_println((String*) iterator->element);
		iterator = iterator->next;
	}

	linked_list_free(clone);
	linked_list_free(list);
}
