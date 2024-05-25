#include "entry.h"

Entry* entry_new(void* key, void* value) {
	Entry* entry = allocate(sizeof(Entry));
	entry->value = value;
	entry->key = key;

	return entry;
}

void entry_free(Entry* entry, Destructor key_destructor, Destructor value_destructor) {
	key_destructor(entry->key);
	value_destructor(entry->value);
	free(entry);
}

Entry* entry_clone(Entry* entry, Duplicator key_duplicator, Duplicator value_duplicator) {
	return entry_new(
				key_duplicator(entry->key), 
				value_duplicator(entry->value)
			);	
}
