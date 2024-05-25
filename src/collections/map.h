#ifndef STD_MAP_H
#define STD_MAP_H

#include "linked_list.h"
#include "vector.h"
#include <stdbool.h>
#include "map/entry.h"

#define MAP_LOAD_SIZE 0.75

/**
 * Map defines a hash map with a dynamically allocated opaque pointer
 * for both key and value types. Destructors, duplicators, comparators,
 * and hashers must be used to allow for the map to allocate, deallocate
 * and compare data at will. 
 *
 * Member field "entries" contains fields of type "LinkedList*" which itself
 * contains fields of type "Entry*" which holds the passed key and value pairs.
 *
 * Important: "entry_count" is the number of total entries, even with matching 
 * hash values. "entries->count" should be used if any traversing of the
 * entries vector is done
 */
typedef struct {
	Vector* entries;
	size_t entry_count;
	Hasher key_hasher;
	Comparator key_comparator;
	Destructor key_destructor;
	Duplicator key_duplicator;
	Destructor value_destructor;
	Duplicator value_duplicator;
} Map;

/**
 * Map splice defines a vector of non-null Entry's with 
 * an immutable key and a mutable value
 */
typedef struct {
	Map* original;
	Vector* entries;
	size_t count;
} MapSplice;

Map* map_new(
		size_t initial_capacity,
		Hasher hasher,
		Comparator key_comparator,
		Destructor key_destructor,
		Destructor value_destructor,
		Duplicator key_duplicator,
		Duplicator value_duplicator
	);

Map* map_clone(Map* map);
void map_free(Map* map);

void* map_get_value(Map* map, void* key, bool discard_key);
Entry* map_get_entry(Map* map, void* key, bool discard_key);
Entry* map_remove(Map* map, void* key, bool discard_key);
void map_remove_discard(Map* map, void* key, bool discard_key);

void map_insert(Map* map, void* key, void* value);

MapSplice* map_splice_new(Map* map);
Entry* splice_get_entry(MapSplice* splice, size_t index);
Entry* splice_get_value(MapSplice* splice, size_t index);
void map_splice_free(MapSplice* splice);

#endif
