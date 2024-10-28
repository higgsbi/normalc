#ifndef NORMALC_ENTRY_SET_H
#define NORMALC_ENTRY_SET_H

#include "../linked_list.h"
#include "entry.h"

#ifndef MAP_LOAD_SIZE
#define MAP_LOAD_SIZE 0.75
#endif

/**
 * EntrySet represents the underlying data structure for a HashMap's key value pairs.
 * Each linked list pointer contains one or more `Entry`. 
 *
 * The count supplied by the entry set does not necessarily represent the amount of values in the 
 * set as there may be keys with duplicate hash codes 
 */
typedef struct {
	LinkedList** data;
	size_t count;
	size_t capacity;
} EntrySet;

OPTION_TYPE(EntrySet*, EntrySet, entry_set, NULL)

/**
 * Returns a new entry set with the given initial capacity.
 */
EntrySet* entry_set_new(size_t capacity);

/**
 * Returns a clone of an entry set given duplicators for both the key and value of each entry
 */
EntrySet* entry_set_clone(EntrySet* entries, Duplicator key_duplicator, Duplicator value_duplicator);

/**
 * Frees all entries from the given entry set.
 * If should_delete_entry is false, only the pointer is freed while the underlying data is left alone
 */
void entry_set_free(EntrySet* entries, Destructor key_destructor, Destructor value_destructor, bool should_delete_entry);

/**
 * Adds the given element to the entry set and takes ownership of it
 */
void entry_set_add(EntrySet* entries, void* element);

/**
 * Sets a given entry list into an entry set at the given index.
 * If should_adjust_capacity is true, an attempt to resize the entry_set will be made.
 */
bool entry_set_set(EntrySet* entry_set, size_t map_entry_count, size_t index, LinkedList* list, bool should_adjust_capacity);

/**
 * Returns a list of entries from the given entry set.
 * If there is not more than 1 entry by the entry's key, this list will be of size 1.
 */
LinkedList* entry_set_get(EntrySet* entry_set, size_t index);

#endif
