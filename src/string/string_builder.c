#include "string_builder.h"
#include "../memory/memory.h"
#include "string.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void _string_builder_expand(StringBuilder* builder, size_t added);

StringBuilder* string_builder_new() {
	StringBuilder* builder = (StringBuilder*) allocate(sizeof(StringBuilder));
	(*builder).buffer = allocate(sizeof(char) * 1);
	(*builder).length = 0;
	(*builder).capacity = 1;

	return builder;
}

StringBuilder* string_builder_from(char* buffer) {
	ASSERT_NONNULL(buffer);

	size_t length = strlen(buffer);

	StringBuilder* builder = (StringBuilder*) allocate(sizeof(StringBuilder));
	builder->buffer = allocate(sizeof(char) * length);
	builder->length = length;
	builder->capacity = length;

	strncpy(builder->buffer, buffer, length);	

	return builder;
}

StringBuilder* string_builder_clone(StringBuilder* src) {
	ASSERT_NONNULL(src);
	
	StringBuilder* copy = (StringBuilder*) allocate(sizeof(StringBuilder));
	copy->buffer = allocate(sizeof(char) * src->length);
	copy->length = src->length;
	copy->capacity = src->capacity;

	strncpy(copy->buffer, src->buffer, src->length);

	return copy;	
}

void string_builder_free(StringBuilder* builder) {
	ASSERT_NONNULL(builder);	

	free(builder->buffer);
	free(builder);
}

void string_builder_append_char(StringBuilder* builder, char appended) {
	ASSERT_NONNULL(builder);

	_string_builder_expand(builder, 1);
	builder->buffer[builder->length] = appended;
	builder->length++;
}

void string_builder_append(StringBuilder* builder, char* appended) {
	ASSERT_NONNULL(builder);
	ASSERT_NONNULL(appended);
	
	size_t length = strlen(appended);
	_string_builder_expand(builder, length); 

	strncpy(builder->buffer + builder->length, appended, length);
	builder->length += length;
}

void string_builder_append_format(StringBuilder* builder, char* format, ...) {	
	ASSERT_NONNULL(builder); 
	ASSERT_NONNULL(format); 

    va_list args;
    va_start(args, format);
    String* formatted = string_from_format(format, args);
    va_end(args);

	string_builder_append(builder, formatted->buffer);
	string_free(formatted);
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
	
	String* built = allocate(sizeof(String));
	built->buffer = allocate(sizeof(char) * (builder->length + 1));
	built->length = builder->length;

	strncpy(built->buffer, builder->buffer, builder->length);	
	strncpy(built->buffer + builder->length , "\0", 1);

	return built;
}

void _string_builder_expand(StringBuilder* builder, size_t added) {
	ASSERT_NONNULL(builder);	

	if (added == 0) {
		return;
	}

	if (builder->capacity < (builder->length + added)) {
		if (added > builder->length) {
			builder->capacity = builder->length + added;
		} else {
			builder->capacity = builder->length * 2;
		}
		builder->buffer = reallocate(builder->buffer, builder->capacity);	
	}
}




