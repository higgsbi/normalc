#ifndef STD_LINKED_LIST_H
#define STD_LINKED_LIST_H

#include "../memory/memory.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * Node defines a heap allocated opaque pointer element and a link
 * to another node used in a LinkedList
 */
struct Node {
	void* element;	
	struct Node* next;
};

typedef struct Node Node;

/**
 * LinkedList defines a singly linked list of nodes
 */
typedef struct {
	Node* head;
	Destructor destructor;
	Duplicator duplicator;
	size_t count;
} LinkedList;

Node* linked_node_new(void* element, Node* next);
void linked_node_free(LinkedList* list, Node* node);

LinkedList* linked_list_new(Node* head, Destructor destructor, Duplicator duplicator);
void linked_list_push(LinkedList* list, Node* next);
LinkedList* linked_list_clone(LinkedList* list);
void linked_list_free(LinkedList* list);

bool linked_list_headless(LinkedList* list);

Node* linked_list_pop_head(LinkedList* list);
Node* linked_list_pop_tail(LinkedList* list);
Node* linked_list_pop(LinkedList* list, size_t index);
void linked_list_pop_head_discard(LinkedList* list);
void linked_list_pop_tail_discard(LinkedList* list);
void linked_list_pop_discard(LinkedList* list, size_t index);

#endif
