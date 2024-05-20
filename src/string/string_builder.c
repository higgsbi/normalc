#include "string_builder.h"
#include "../memory/memory.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void _string_builder_expand(StringBuilder* builder, size_t added);

StringBuilder* string_builder_new() {
	StringBuilder* builder = (StringBuilder*) alloc(sizeof(StringBuilder));
	(*builder).buffer = alloc(sizeof(char) * 1);
	(*builder).length = 0;

	return builder;
}

StringBuilder* string_builder_from(char* buffer) {
	ASSERT_NONNULL(buffer);

	size_t length = strlen(buffer);

	StringBuilder* builder = (StringBuilder*) alloc(sizeof(StringBuilder));
	builder->buffer = alloc(sizeof(char) * length);
	builder->length = length;

	strncpy(builder->buffer, buffer, length);	

	return builder;
}

StringBuilder* string_builder_clone(StringBuilder* src) {
	ASSERT_NONNULL(src);
	
	StringBuilder* copy = (StringBuilder*) alloc(sizeof(StringBuilder));
	copy->buffer = alloc(sizeof(char) * src->length);
	copy->length = src->length;

	strncpy(copy->buffer, src->buffer, src->length);

	return copy;	
}

void string_builder_free(StringBuilder* builder) {
	ASSERT_NONNULL(builder);	

	free(builder->buffer);
	free(builder);
}

void string_builder_append(StringBuilder* builder, char* appended) {
	ASSERT_NONNULL(builder);
	ASSERT_NONNULL(appended);
	
	size_t length = strlen(appended);
	_string_builder_expand(builder, length); 

	strncpy(builder->buffer + builder->length, appended, length);
	builder->length += length;
}


void string_builder_append_substring(StringBuilder* builder, char* src, size_t start, size_t length) {
	ASSERT_NONNULL(builder);
	ASSERT_NONNULL(src);

	_string_builder_expand(builder, length); 

	strncpy(builder->buffer + builder->length, src + start, length);
	builder->length += length;
}

String* string_builder_build(StringBuilder* builder) {
	ASSERT_NONNULL(builder);
	
	String* built = alloc(sizeof(String));
	built->buffer = alloc(sizeof(char) * (builder->length + 1));
	built->length = builder->length + 1;

	strncpy(built->buffer, builder->buffer, builder->length);	
	strncpy(built->buffer + builder->length, "\0", 1);

	return built;
}

void _string_builder_expand(StringBuilder* builder, size_t added) {
	ASSERT_NONNULL(builder);	

	if (added == 0) {
		return;
	}
	
	builder->buffer = realloc(builder->buffer, builder->length + added);
	if (builder->buffer == NULL) {
		printf("Could not reallocate with size %zu", added);
		exit(EXIT_FAILURE);
	}
}


