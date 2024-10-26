#include "map.h"
#include "linked_list.h"
#include "vector.h"
#include "map/entry_set.h"
#include "../error/error.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

void _map_insert(Map* map, EntrySet* entry_set, void* key, void* value, bool should_rehash);
void _map_rehash(Map* map);
size_t _map_index_from_hash(Map* map, size_t hash);

Map* map_new(
		size_t initial_capacity,
		Hasher key_hasher,
		Comparator key_comparator,
		Destructor key_destructor,
		Destructor value_destructor,
		Duplicator key_duplicator,
		Duplicator value_duplicator
	) {

	ASSERT_NONNULL(key_comparator);		
	ASSERT_NONNULL(key_hasher);		
	ASSERT_NONNULL(key_destructor);		
	ASSERT_NONNULL(value_destructor);		
	ASSERT_NONNULL(key_duplicator);
	ASSERT_NONNULL(value_duplicator);

	Map* map = allocate(sizeof(Map));
	map->entry_count = 0;
	map->key_hasher = key_hasher;
	map->key_comparator = key_comparator;
	map->key_destructor = key_destructor;
	map->value_destructor = value_destructor;
	map->key_duplicator = key_duplicator;
	map->value_duplicator = value_duplicator;

	// 0 element map requires too much annoying bound checking and resizing -> 2 is an acceptable minimum
	initial_capacity = (initial_capacity <= 0 ? 2 : initial_capacity);
	map->entries = entry_set_new(initial_capacity);

	for (size_t i = 0; i < map->entries->capacity; i++) {
		map->entries->data[i] = NULL; 
	}	

	return map;
}

Map* map_clone(Map* map) {
	ASSERT_NONNULL(map);

	Map* clone= allocate(sizeof(Map));
	clone->entry_count = map->entry_count;
	clone->key_destructor = map->key_destructor;
	clone->value_destructor = map->value_destructor;
	clone->key_duplicator = map->key_duplicator;
	clone->value_duplicator = map->value_duplicator;
	clone->key_hasher = map->key_hasher;
	clone->key_comparator = map->key_comparator;

	clone->entries = entry_set_clone(map->entries, map->key_duplicator, map->value_duplicator);
	return clone;
}

void map_free(Map* map) {
	entry_set_free(map->entries, map->key_destructor, map->value_destructor, true);
	free(map);
}

void map_insert(Map* map, void* key, void* value) {
	_map_insert(map, map->entries, key, value, true);
}

void* map_get_value(Map* map, void* key, bool discard_key) {	
	return map_get_entry(map, key, discard_key)->value;
}

Entry* map_get_entry(Map* map, void* key, bool discard_key) {
	size_t index = _map_index_from_hash(map, map->key_hasher(key));

	if (index >= map->entries->capacity || map->entries->data[index] == NULL) {
		if (discard_key) {
			map->key_destructor(key);
		}
		return NULL;
	}

	LinkedList* entries = entry_set_get(map->entries, index);

	if (entries == NULL) {
		if (discard_key) {
			map->key_destructor(key);
		}
		return NULL;
	}

	Node* iterator = entries->head;

	// find entry matching key exactly when hash collides
	while (iterator != NULL) {	
		Entry* entry = ((Entry*) iterator->element);
		if (map->key_comparator(key, entry->key)) {
			if (discard_key) {
				map->key_destructor(key);
			}
			return entry;
		}
		iterator = iterator->next;
	}

	if (discard_key) {
		map->key_destructor(key);
	}

	// return NULL if no match occurs
	return NULL;
}

// Does not delete entry
Entry* _override_vector_remove(Map* map, LinkedList* list, size_t list_index, size_t vector_index) { 
	// Delete element later in list
	if (list_index > 0) {
		Node* removed = linked_list_pop(list, list_index);
		Entry* entry = removed->element;
		free(removed);
		map->entry_count--;
		return entry;
	} 

	// delete first element, but move sequential elements back one
	if (list->head->next != NULL) {
		Node* removed = linked_list_pop_head(list);
		Entry* entry = removed->element;
		free(removed);
		map->entry_count--;
		return entry;
	}

	// delete first element, then the entire list
	Node* head = linked_list_pop_head(list);
	Entry* entry = head->element;
	free(head);
	free(list);
	map->entries->data[vector_index] = NULL;
	map->entry_count--;
	
	return entry;
}

Entry* map_remove(Map* map, void* key, bool discard_key) {
	size_t index = _map_index_from_hash(map, map->key_hasher(key));

	if (index >= map->entries->capacity || map->entries->data[index] == NULL) {
		return NULL;
	}

	LinkedList* entries = entry_set_get(map->entries, index);

	if (entries == NULL) {
		return NULL;
	}

	Node* iterator = entries->head;

	// find entry matching key exactly when hash collides
	size_t list_index = 0;
	while (iterator != NULL) {	
		Entry* next = ((Entry*) iterator->element);

		if (map->key_comparator(key, next->key)) {
			Entry* removed = _override_vector_remove(map, entries, list_index, index);
			if (discard_key) {
				map->key_destructor(key);
			}
			return removed;
		}

		iterator = iterator->next;
		list_index++;
	}

	if (discard_key) {
		map->key_destructor(key);
	}

	// return null if nothing found 
	return NULL;
}

void map_delete(Map* map, void* key, bool discard_key) {
	Entry* retrieved = map_remove(map, key, discard_key);
	if (retrieved) {
		entry_free(retrieved, map->key_destructor, map->value_destructor);
	}	
}

size_t _map_index_from_hash(Map* map, size_t hash) {
	size_t divisor = map->entries->capacity == 0 ? 1 : map->entries->capacity;
	size_t index = hash % divisor;
	
	return index;	
}


// SPLICE


MapSplice* map_splice_new(Map* map) {
	Vector* entries = vector_new(map->entries->count, duplicator_empty, destructor_empty);

	for (size_t i = 0; i < map->entries->capacity; i++) {
		LinkedList* entry_list = map->entries->data[i];

		if (entry_list == NULL) {
			continue;
		}

		Node* iterator = entry_list->head;

		while (iterator != NULL) {
			vector_add(entries, iterator->element);
			iterator = iterator->next;
		}
	}
	
	MapSplice* splice = allocate(sizeof(MapSplice));
	splice->count = entries->count;
	splice->original = map;
	splice->entries = entries;

	return splice;
}

void map_splice_free(MapSplice* splice) {
	free(splice->entries->data);
	free(splice->entries);
	free(splice);
}

Entry* map_splice_get_entry(MapSplice* splice, size_t index) {
	ASSERT_NONNULL(splice);	
	ASSERT_VALID_BOUNDS(splice, (int) index, (int) splice->count);

	return vector_get(splice->entries, index);
}

void* map_splice_get_value(MapSplice* splice, size_t index) {
	return map_splice_get_entry(splice, index)->value;
}


// INTERNAL FUNCTIONS


void _map_insert(Map* map, EntrySet* entries, void* key, void* value, bool should_rehash) {
	ASSERT_NONNULL(map);
	ASSERT_NONNULL(key);
	ASSERT_NONNULL(value);

	size_t index = _map_index_from_hash(map, map->key_hasher(key));

	// Existing hash found
	if (entries->data[index] != NULL) {
		LinkedList* list = entries->data[index];
		Entry* existing = list->head->element;

		// Duplicates foun;
		if (map->key_comparator(existing->key, key)) {
			map->key_destructor(existing->key);
			map->value_destructor(existing->value);
			existing->key = key;
			existing->value = value;
		} else {
			Node* node = linked_node_new(entry_new(key, value), NULL);
			linked_list_push(list, node);
			map->entry_count++;
		}

		return;
	}

	// Nothing existing for hash, create new entry	
	LinkedList* list_new = linked_list_new(
				linked_node_new(
						entry_new(key, value),
						NULL
					), 
				map->value_destructor, 
				map->value_duplicator
			);

	bool did_change_capacity = entry_set_set(entries, map->entry_count, index, list_new, should_rehash);
	map->entry_count++;

	if (should_rehash && did_change_capacity) {	
		_map_rehash(map);
	} 
}

void _map_rehash(Map* map_existing) {
	// Create new vector, but keep old map (only free old vector and replace with this)
	size_t original_entry_count = map_existing->entry_count;
	EntrySet* entries_rehashed = entry_set_new(map_existing->entries->capacity);

	for (size_t i = 0; i < entries_rehashed->capacity; i++) {
		entries_rehashed->data[i] = NULL; 
	}

	// Place old entires into new entries with rehash
	for (size_t i = 0; i < map_existing->entries->capacity; i++) {		
		LinkedList* list = map_existing->entries->data[i];
		if (list == NULL) {
			continue;	
		}

		Node* iterator = list->head;

		while (iterator != NULL) {	
			Entry* entry = iterator->element;
			_map_insert(map_existing, entries_rehashed,entry->key, entry->value, false);				
			iterator = iterator->next;	
		}	
	}

	// replace old entries with rehashed entries
	entry_set_free(map_existing->entries, map_existing->key_destructor, map_existing->value_destructor, false);
	map_existing->entries = entries_rehashed;
	map_existing->entry_count = original_entry_count;
}





