#include "linked_list.h"
#include "../error/error.h"

Node* linked_node_new(void* element, Node* next) {
	ASSERT_NONNULL(element);

	Node* node = allocate(sizeof(Node));	
	node->element = element;
	node->next = next;

	return node;
}

void linked_node_free(LinkedList* list, Node* node) {
	ASSERT_NONNULL(list);
	ASSERT_NONNULL(node);

	list->destructor(node->element);
	free(node);	
}

LinkedList* linked_list_new(Node* head, Destructor destructor, Duplicator duplicator) {
	ASSERT_NONNULL(head);
	ASSERT_NONNULL(destructor);
	ASSERT_NONNULL(duplicator);

	LinkedList* list = allocate(sizeof(LinkedList));
	list->destructor = destructor;
	list->duplicator = duplicator;
	list->count= 1;
	list->head = head;

	return list;
}

void linked_list_push(LinkedList* list, Node* next) {
	ASSERT_NONNULL(list);
	ASSERT_NONNULL(next);

	list->count++;

	if (linked_list_headless(list)) {
		list->head = next;
		return;		
	}

	Node* temp = list->head;

	while (temp->next != NULL) {
		temp = temp->next;	
	}
	
	temp->next = next;	
}


LinkedList* linked_list_clone(LinkedList* list) {
	ASSERT_NONNULL(list);

	LinkedList* clone = allocate(sizeof(LinkedList));
	clone->destructor = list->destructor;
	clone->duplicator = list->duplicator;
	clone->count= list->count;

	if (linked_list_headless(list)) {
		return clone;	
	}	

	clone->head = allocate(sizeof(Node));
	clone->head->element = list->duplicator(list->head->element);

	Node* temp = list->head;
	Node* clone_temp = clone->head;

	while (temp->next != NULL) {
		temp = temp->next;	
		clone_temp->next = linked_node_new(list->duplicator(temp->element), NULL);
		clone_temp = clone_temp->next;
	}

	return clone;
}

void linked_list_free(LinkedList* list) {
	ASSERT_NONNULL(list);

	if (!linked_list_headless(list)) {
		Node* temp = list->head;
		Node* free_temp = list->head;

		while (temp != NULL) {
			temp = temp->next;	
			list->destructor(free_temp->element);
			free(free_temp);
			free_temp = temp;
		}
	} 
	
	free(list);
}

bool linked_list_headless(LinkedList* list) {
	ASSERT_NONNULL(list);
	return !(list->head);
}

Node* linked_list_pop_head(LinkedList* list) {
	ASSERT_NONNULL(list);
	ASSERT_NONNULL(list->head);

	Node* popped = list->head;   
	list->count--;

	if (popped->next != NULL) {
		list->head = list->head->next;		
	} else {
		list->head = NULL;
	}

	return popped;
}

Node* linked_list_pop_tail(LinkedList* list) {
	ASSERT_NONNULL(list);
	ASSERT_NONNULL(list->head);
	
	list->count--;

	if (list->head->next == NULL) {
		Node* popped = list->head;
		list->head = NULL;
		return popped;
	}  

	Node* previous = NULL;
	Node* iterator = list->head;

	while (iterator->next != NULL) {
		previous = iterator;
		iterator = iterator->next;
	}
	
	if (previous != NULL) {
		previous->next = NULL;
	}

	return iterator;
}

Node* linked_list_pop(LinkedList* list, size_t index) {
	ASSERT_NONNULL(list);
	ASSERT_NONNULL(list->head);

	list->count--;

	// Only head
	if (list->head->next == NULL) {
		if (index > 0) {
			ASSERT_VALID_BOUNDS(list, (int) index, 1);
		}
		Node* popped = list->head;
		list->head = NULL;
		return popped;
	} 

	// Remove head
	if (index == 0) {
		Node* popped = list->head;
		list->head = list->head->next;
		return popped;
	}	

	// Valid removal of anything past head
	Node* previous = NULL;
	Node* iterator = list->head;
	Node* next = NULL;
	size_t iteration = 0;

	while (iterator->next != NULL) {
		previous = iterator;
		iterator = iterator->next;
		next = iterator->next;

		if (iteration == index) {
			break;	
		}

		iteration++;
	}

	if (iteration < index) {	
		ASSERT_VALID_BOUNDS(list, (int) index, (int) (iteration + 1));
	}
	
	if (previous != NULL) {
		if (next != NULL) {
			previous->next = next;
		} else {
			previous->next = NULL;
		}
	} 
	
	return iterator;
}

void linked_list_delete_head(LinkedList* list) {
	linked_node_free(list, linked_list_pop_head(list));	
}

void linked_list_delete_tail(LinkedList* list) {
	linked_node_free(list, linked_list_pop_tail(list));	
}

void linked_list_delete(LinkedList* list, size_t index) {	
	linked_node_free(list, linked_list_pop(list, index));	
}

