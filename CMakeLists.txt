cmake_minimum_required(VERSION 3.0)
project(normalc)

set(SOURCES 
	src/string/string.c
	src/string/string_builder.c
	src/error/error.c
	src/path/path.c
	src/path/io.c
	src/random/random.c
	src/collections/vector.c
	src/collections/array.c
	src/collections/linked_list.c
	src/collections/map.c
	src/collections/map/entry.c
)

set(HEADERS 
	src/string/string.h
	src/string/string_builder.h
	src/error/error.c
	src/memory/memory.h
	src/path/path.h
	src/path/io.h
	src/random/random.h
	src/collections/vector.h
	src/collections/array.h
	src/collections/linked_list.h
	src/collections/map.h
	src/collections/map/entry.h
)

add_library(normalc STATIC ${SOURCES} ${HEADERS})
target_compile_options(normalc PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-function)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/lib)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/" 
	DESTINATION "include/${PROJECT_NAME}" 
        FILES_MATCHING
        PATTERN "*.h"
)

