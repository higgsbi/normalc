#ifndef NORMALC_LINKED_LIST_H
#define NORMALC_LINKED_LIST_H

#include "../memory/memory.h"
#include "../safety/option.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * Node defines a heap allocated void pointer element and a link
 * to another node used in a LinkedList
 */
typedef struct Node {
	void* element;	
	struct Node* next;
} Node;

/**
 * Node defines a heap allocated opaque pointer element and a link
 * to another node used in a LinkedList
 */
typedef struct Node Node;

OPTION_TYPE(Node*, Node, node, NULL)

/**
 * LinkedList defines a singly linked list of nodes
 */
typedef struct {
	Node* head;
	Destructor destructor;
	Duplicator duplicator;
	size_t count;
} LinkedList;

OPTION_TYPE(LinkedList*, LinkedList, linked_list, NULL)

/**
 * Defines type safe functions for the LinkedList type
 */
#define LINKED_LIST_SAFE(type, type_name) \
    static inline Node* linked_node_new_##type_name(type* element, Node* next) { \
        linked_node_new((void*) element, next); \
    } \
    
/**
 * Crates a new node which takes ownership of the given element
 */
Node* linked_node_new(void* element, Node* next);

/**
 * Frees the given node and its element from the linked list. Does not
 * modify any part of the list
 */
void linked_node_free(LinkedList* list, Node* node);

/**
 * Creates a linked list with a non-null head. A destructor and duplicator for the 
 * list element must be given.
 */
LinkedList* linked_list_new(Node* head, Destructor destructor, Duplicator duplicator);

/**
 * Pushes a non-null node onto the linked list
 */
void linked_list_push(LinkedList* list, Node* next);

/**
 * Returns a deep clone of the given linked list
 */
LinkedList* linked_list_clone(LinkedList* list);

/**
 * Frees the given list and all nodes
 */
void linked_list_free(LinkedList* list);

/**
 * Returns true if the list's head node is null
 */
bool linked_list_headless(LinkedList* list);

/**
 * Removes the first node from the linked list and pushes all subsequent
 * nodes down one
 */
Node* linked_list_pop_head(LinkedList* list);

/**
 * Removes the last node from the linked list 
 */
Node* linked_list_pop_tail(LinkedList* list);

/**
 * Removes a node from the linked list at the given index.
 * If the index is outside of the list's capacity, the system
 * will exit with an error.
 */
Node* linked_list_pop(LinkedList* list, size_t index);

/**
 * Helper function for linked_list_pop_head(), but frees the return value
 */
void linked_list_delete_head(LinkedList* list);

/**
 * Helper function for linked_list_pop_tail(), but frees the return value
 */
void linked_list_delete_tail(LinkedList* list);

/**
 * Helper function for linked_list_pop(), but frees the return value
 */
void linked_list_delete(LinkedList* list, size_t index);

#endif
