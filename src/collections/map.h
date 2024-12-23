#ifndef NORMALC_MAP_H
#define NORMALC_MAP_H

#include "vector.h"
#include "map/entry.h"
#include "map/entry_set.h"
#include <stdbool.h>

#ifndef MAP_LOAD_SIZE

/**
 * MAP_LOAD_SIZE is used to determine if the map needs to be rehashed.
 *
 * The map will opt to rehash if ((map->entry_count / MAP_LOAD_SIZE) + 1) > entries->capacity
 *
 * If this is set to a smaller value, the map will rehash more often which may provide higher 
 * insert speeds at the cost of memory usage. If this is set to a higher value, the map will rehash
 * left often resulting in lower insert speeds, but less memory usage.
 */
#define MAP_LOAD_SIZE 0.75
#endif

/**
 * Map defines a hash map with a dynamically allocated void pointer
 * for both key and value types. Destructors, duplicators, comparators,
 * and hashers must be used to allow for the map to allocate, deallocate
 * and compare data at will. 
 *
 * Member field "entries" contains fields of type "LinkedList*" which itself
 * contains fields of type "Entry*" which holds the passed key and value pairs.
 *
 * Important: "entry_count" is the number of total entries, even with matching 
 * hash values. "entries->count" should be used if any traversing of the
 * entries entry set is done
 */
typedef struct {
	EntrySet* entries;
	size_t entry_count;
	Hasher key_hasher;
	EqualityChecker key_comparator;
	Destructor key_destructor;
	Duplicator key_duplicator;
	Destructor value_destructor;
	Duplicator value_duplicator;
} Map;

#define DEFAULT_MAP { NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL }
OPTION_TYPE(Map, Map, map, DEFAULT_MAP)


/**
 * Defines type safe functions for the Map, EntrySet, and Entry type
 */
#define MAP_SAFE(key_type, value_type, type_name) \
    static inline value_type* map_##type_name##g_et_value(Map* map, key_type* key, bool discard_key) { \
        return (value_type*) map_get_value(map, (void*) key, discard_key); \
    } \
    static inline Entry* map_##type_name##_get_entry(Map* map, key_type* key, bool discard_key) { \
        return map_get_entry(map, (void*) key, discard_key); \
    } \
    static inline Entry* map_##type_name##_remove(Map* map, key_type* key, bool discard_key) { \
        return map_remove(map, (void*) key, discard_key); \
    } \
    static inline void map_##type_name##_delete(Map* map, key_type* key, bool discard_key) { \
        map_delete(map, (void*) key, discard_key); \
    } \
    static inline void map_##type_name##_insert(Map* map, key_type* key, value_type* value) { \
        map_insert(map, (void*) key, (void*) value); \
    } \
    static inline void entry_set_##type_name##_add(EntrySet* entries, key_type* element) { \
        entry_set_add(entries, (void*) element); \
    } \
    static inline Entry* entry_##type_name##_new(key_type* key, value_type* value) { \
        return entry_new((void*) key, (void*) value); \
    } \
    static inline value_type* map_splice_##type_name##_get_value(MapSplice* splice, size_t index) { \
        return (value_type*) map_splice_get_value(splice, index); \
    } \

/**
 * Map splice defines a vector of non-null Entry's with 
 * an immutable key and a mutable value
 */
typedef struct {
	Map* original;
	Vector* entries;
	size_t count;
} MapSplice;

#define DEFAULT_MAP_SPLICE { NULL, NULL, 0 }
OPTION_TYPE(MapSplice, MapSplice, map_splice, DEFAULT_MAP_SPLICE)

/**
 * Creates a new map with a given initial capacity.
 * A high initial capacity will use more memory, but will cause
 * less reallocations if the capacity is exceeded.
 *
 * Each function pointer is required to be non-null to allow the map
 * to effectively handle data management and comparison. 
 */
Map* map_new(
		size_t initial_capacity,
		Hasher hasher,
		EqualityChecker key_comparator,
		Destructor key_destructor,
		Destructor value_destructor,
		Duplicator key_duplicator,
		Duplicator value_duplicator
	);

/**
 * Returns a deep clone of the given map
 */
Map* map_clone(Map* map);

/**
 * Frees the given map and all entries
 */
void map_free(Map* map);

/**
 * Returns the value corresponding to the given key.
 * If no value is found by the given key, null is returned.
 * The `discard_key` parameter can be used to automatically discard the given key.
 */
void* map_get_value(Map* map, void* key, bool discard_key);

/**
 * Returns the entry (key value pair) corresponding to the given key.
 * If no entry is found by the given key, null is returned
 * The `discard_key` parameter can be used to automatically discard the given key.
 */
Entry* map_get_entry(Map* map, void* key, bool discard_key);

/**
 * Removes and returns the entry by the given key.
 * If no entry is found by the given key, null is returned.
 * The `discard_key` parameter can be used to automatically discard the given key.
 */
Entry* map_remove(Map* map, void* key, bool discard_key);

/**
 * Helper function for `map_remove()`, but automatically discards the return value
 */
void map_delete(Map* map, void* key, bool discard_key);

/**
 * Inserts a key value pair into the map. 
 * If a value already exists for the given key, the stored value is replaced.
 *
 * The map takes ownership of the passed key and value parameters.
 */
void map_insert(Map* map, void* key, void* value);

/**
 * Creates a splice from the given map. Each element is guaranteed to be non-null.
 */
MapSplice map_splice_from(Map* map);

/**
 * Returns the entry at the given index for this splice. This is guaranteed to be non-null.
 */
Entry* map_splice_get_entry(MapSplice* splice, size_t index);

/**
 * Returns the value at the given index for this splice. This is guaranteed to be non-null.
 */
void* map_splice_get_value(MapSplice* splice, size_t index);

/**
 * Frees the given splice leaving the map untouched
 */
void map_splice_free(MapSplice* splice);

#endif
