#ifndef NORMALC_STRING_BUILDER_H
#define NORMALC_STRING_BUILDER_H

#include <stdlib.h>
#include "../error/error.h"
#include "string.h"

/**
 * StringBuilder define a mutable non-null terminated string
 */
typedef struct {
	char* buffer;
	size_t length;
} StringBuilder;

StringBuilder* string_builder_new();
StringBuilder* string_builder_from(char* buffer);
StringBuilder* string_builder_clone(StringBuilder* src);
void string_builder_free(StringBuilder* builder);

void string_builder_append(StringBuilder* builder, char* appended);
void string_builder_append_char(StringBuilder* builder, char appended);
void string_builder_append_substring(StringBuilder* builder, char* src, size_t start, size_t length);
String* string_builder_build(StringBuilder* builder);

#endif
