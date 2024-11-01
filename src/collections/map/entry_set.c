#include "entry_set.h"
#include "../../error/error.h"

void _entry_set_try_expand(EntrySet* entries);
LinkedList* _override_linked_list_clone(LinkedList* list, Duplicator key_duplicator, Duplicator value_duplicator);

EntrySet* entry_set_new(size_t capacity) {	
	EntrySet* entries = allocate(sizeof(EntrySet));

	entries->capacity = capacity;
	entries->count = 0;
	entries->data = callocate(capacity, sizeof(void*));

	return entries;
}


EntrySet* entry_set_clone(EntrySet* entries, Duplicator key_duplicator, Duplicator value_duplicator) {
	EntrySet* clone_entries = allocate(sizeof(EntrySet));

	clone_entries->capacity = entries->capacity;
	clone_entries->count = entries->count;

	clone_entries->data = allocate(sizeof(void*) * entries->capacity);
	for (size_t i = 0; i < entries->capacity; i++) {
		if (entries->data[i] != NULL) {
			clone_entries->data[i] = _override_linked_list_clone(entries->data[i], key_duplicator, value_duplicator);
		} else {
			clone_entries->data[i] = NULL;
		}
	}

	return clone_entries;
}


void entry_set_free(EntrySet* entries, Destructor key_destructor, Destructor value_destructor, bool should_delete_entry) {
	for (size_t i = 0; i < entries->capacity; i++) {
		LinkedList* bucket = entries->data[i];

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
						key_destructor, 
						value_destructor
					);
			} else {
				free(free_temp->element);
			}

			free(free_temp);
			free_temp = temp;
		}
		
		free(bucket);
	}

	free(entries->data);
	free(entries);
}

LinkedList* entry_set_get(EntrySet* entry_set, size_t index) {
	return ((LinkedList*) entry_set->data[index]);
}

bool entry_set_set(EntrySet* entry_set, size_t map_entry_count, size_t index, LinkedList* list, bool should_adjust_capacity) {	
	bool changed_capacity = false;

	if (should_adjust_capacity && (((float) map_entry_count / MAP_LOAD_SIZE) + 1) > entry_set->capacity) {
		changed_capacity = true;	
		size_t old_capacity = entry_set->capacity;

		if (entry_set->capacity == 0) {
			entry_set->capacity = 2;
		} else {	
			entry_set->capacity = entry_set->capacity * 2;
		}

		entry_set->data = reallocate(entry_set->data, sizeof(void*) * entry_set->capacity);

		// Set unitialized new memory to null pointers
		for (size_t i = old_capacity; i < entry_set->capacity; i++) {
			entry_set->data[i] = NULL; 
		}
	}
	
	entry_set->data[index] = list;
	entry_set->count++;

	return changed_capacity;
}

void entry_set_add(EntrySet* entries, void* element) {
	ASSERT_NONNULL(entries);
	ASSERT_NONNULL(element);

	_entry_set_try_expand(entries);
	entries->data[entries->count] = element;
	entries->count++;
}

// OVERRIDES

LinkedList* _override_linked_list_clone(LinkedList* list, Duplicator key_duplicator, Duplicator value_duplicator) {
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
				key_duplicator, 
				value_duplicator
			);

	Node* temp_node = list->head;
	Node* temp_clone = list_clone->head;

	while (temp_node->next != NULL) {
		temp_node = temp_node->next;	

		temp_clone->next = linked_node_new(entry_clone(
					(Entry*) list->head->element,
					key_duplicator, 
					value_duplicator
				), NULL);

		temp_clone = temp_clone->next;
	}

	return list_clone;
}

// INTERNAL

void _entry_set_try_expand(EntrySet* entries) {
	if (entries->count + 1 < entries->capacity) {
		return;	
	}
	
	if (entries->capacity == 0) {
		entries->capacity = 2;
	} else {	
		entries->capacity = entries->capacity * 2;
	}

	entries->data = reallocate(entries->data, sizeof(void*) * entries->capacity);
}
