#ifndef NORMALC_ENTRY_H
#define NORMALC_ENTRY_H

#include "../../memory/memory.h"
#include "../../safety/option.h"

typedef struct {
	void* key;
	void* value;
} Entry;

#define DEFAULT_ENTRY { NULL, NULL }
OPTION_TYPE(Entry, Entry, entry, DEFAULT_ENTRY)

Entry* entry_new(void* key, void* value);
void entry_free(Entry* entry, Destructor key_destructor, Destructor value_destructor);
Entry* entry_clone(Entry* entry, Duplicator key_duplicator, Duplicator value_duplicator);

#endif
