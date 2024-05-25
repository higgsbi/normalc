#include "map.h"
#include "linked_list.h"
#include "vector.h"
#include <standard/error/error.h>
#include <standard/string/string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

bool _override_vector_set(Map* map, Vector* vector, size_t index, LinkedList* list, bool should_adjust_capacity);
LinkedList* _override_vector_get(Vector* vector, size_t index);
void _override_vector_free(Map* map, Vector* vector, bool should_delete_entry);
Vector* _override_vector_clone(Map* map);
Entry* _override_vector_remove(Map* map, LinkedList* list, size_t list_index, size_t vector_index);

void _map_insert(Map* map, Vector* vector, void* key, void* value, bool should_rehash);
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
	map->entries = vector_new(initial_capacity, (Duplicator) entry_clone, (Destructor) entry_free);
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

	clone->entries = _override_vector_clone(map);
	return clone;
}

void map_free(Map* map) {
	_override_vector_free(map, map->entries, true);
	free(map);
}

void map_insert(Map* map, void* key, void* value) {
	_map_insert(map, map->entries, key, value, true);
}

void* map_get_value(Map* map, void* key, bool discard_key) {	
	return map_get_entry(map, key, discard_key)->value;
}

Entry* map_get_entry(Map* map, void* key, bool discard_key) {
	int index = _map_index_from_hash(map, map->key_hasher(key));

	if (index >= map->entries->capacity || map->entries->data[index] == NULL) {
		if (discard_key) {
			map->key_destructor(key);
		}
		return NULL;
	}

	LinkedList* entries = _override_vector_get(map->entries, index);

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
	int index = _map_index_from_hash(map, map->key_hasher(key));

	if (index >= map->entries->capacity || map->entries->data[index] == NULL) {
		return NULL;
	}

	LinkedList* values = _override_vector_get(map->entries, index);

	if (values == NULL) {
		return NULL;
	}

	Node* iterator = values->head;

	// find entry matching key exactly when hash collides
	size_t list_index = 0;
	while (iterator != NULL) {	
		Entry* next = ((Entry*) iterator->element);

		if (map->key_comparator(key, next->key)) {
			Entry* removed = _override_vector_remove(map, values, list_index, index);
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

void map_remove_discard(Map* map, void* key, bool discard_key) {
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

void _map_insert(Map* map, Vector* entries, void* key, void* value, bool should_rehash) {
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

	bool did_change_capacity = _override_vector_set(map ,entries, index, list_new, should_rehash);

	if (should_rehash && did_change_capacity) {	
		_map_rehash(map);
	} 
}

// SPLICE

MapSplice* map_splice_new(Map* map) {
	Vector* entries = vector_new(
				map->entries->count, 
				(Duplicator) entry_clone,
				(Destructor) entry_free
			);

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

Entry* splice_get_entry(MapSplice* splice, size_t index) {
	ASSERT_NONNULL(splice);	
	ASSERT_VALID_BOUNDS(splice, (int) index, (int) splice->count);

	return vector_get(splice->entries, index);
}

Entry* splice_get_value(MapSplice* splice, size_t index) {
	return splice_get_entry(splice, index)->value;
}

// INTERNAL FUNCTIONS

void _map_rehash(Map* map_existing) {
	// Create new vector, but keep old map (only free old vector and replace with this)
	size_t original_entry_count = map_existing->entry_count;
	Vector* entries_rehashed = vector_new(map_existing->entries->capacity, 
				(Duplicator) entry_clone, 
				(Destructor) entry_free
			);

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
	_override_vector_free(map_existing, map_existing->entries, false);
	map_existing->entries = entries_rehashed;
	map_existing->entry_count = original_entry_count;
}

// OVERRIDES 

LinkedList* _override_vector_get(Vector* vector, size_t index) {
	return ((LinkedList*) vector->data[index]);
}

bool _override_vector_set(Map* map, Vector* vector, size_t index, LinkedList* list, bool should_adjust_capacity) {	
	bool changed_capacity = false;

	if (should_adjust_capacity && (((float) map->entry_count / MAP_LOAD_SIZE) + 1) > vector->capacity) {
		changed_capacity = true;	
		size_t old_capacity = vector->capacity;

		if (vector->capacity == 0) {
			vector->capacity = 2;
		} else {	
			vector->capacity = vector->capacity * 2;
		}

		vector->data = reallocate(vector->data, sizeof(void*) * vector->capacity);

		// Set unitialized new memory to null pointers
		for (size_t i = old_capacity; i < vector->capacity; i++) {
			vector->data[i] = NULL; 
		}
	}
	
	vector->data[index] = list;
	vector->count++;
	map->entry_count++;

	return changed_capacity;
}

LinkedList* _override_linked_list_clone(Map* map, LinkedList* list) {
	LinkedList* list_clone = allocate(sizeof(LinkedList));
	list_clone->destructor = list->destructor;
	list_clone->duplicator = list->duplicator;
	list_clone->count= list->count;

	if (linked_list_headless(list)) {
		return list_clone;	
	}	

	list_clone->head = allocate(sizeof(Node));
	list_clone->head->element = entry_clone(
				(Entry*) list->head->element,
				(Duplicator) map->key_duplicator, 
				(Duplicator) map->value_duplicator
			);

	Node* temp_node = list->head;
	Node* temp_clone = list_clone->head;

	while (temp_node->next != NULL) {
		temp_node = temp_node->next;	

		temp_clone->next = linked_node_new(entry_clone(
					(Entry*) list->head->element,
					(Duplicator) map->key_duplicator, 
					(Duplicator) map->value_duplicator
				), NULL);

		temp_clone = temp_clone->next;
	}

	return list_clone;
}

Vector* _override_vector_clone(Map* map) {
	Vector* clone_entries = allocate(sizeof(Vector));

	clone_entries->capacity = map->entries->capacity;
	clone_entries->count = map->entries->count;
	clone_entries->destructor = map->entries->destructor;
	clone_entries->duplicator = map->entries->duplicator;

	clone_entries->data = allocate(sizeof(void*) * map->entries->capacity);
	for (size_t i = 0; i < map->entries->capacity; i++) {
		if (map->entries->data[i] != NULL) {
			clone_entries->data[i] = _override_linked_list_clone(map, map->entries->data[i]);
		} else {
			clone_entries->data[i] = NULL;
		}
	}

	return clone_entries;
}

// vector holds many LinkedList pointers. We cannot rely on basic list free
void _override_vector_free(Map* map, Vector* vector, bool should_delete_entry) {
	for (size_t i = 0; i < vector->capacity; i++) {
		LinkedList* bucket = vector->data[i];

		if (bucket == NULL) {
			continue;
		} 

		Node* temp = bucket->head;
		Node* free_temp = temp;

		while (temp != NULL) {
			temp = temp->next;	

			if (should_delete_entry) {
				entry_free(
						(Entry*) free_temp->element, 
						(Destructor) map->key_destructor, 
						(Destructor) map->value_destructor
					);
			} else {
				free(free_temp->element);
			}

			free(free_temp);
			free_temp = temp;
		}
		
		free(bucket);
	}

	free(vector->data);
	free(vector);
}
