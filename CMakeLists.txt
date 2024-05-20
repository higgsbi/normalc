cmake_minimum_required(VERSION 3.0)
project(standard)

set(SOURCES 
	src/string/string.c
	src/string/string_builder.c
	src/error/error.c
)

set(HEADERS 
	src/string/string.h
	src/string/string_builder.h
	src/error/error.c
	src/memory/memory.h
)

add_library(standard STATIC ${SOURCES} ${HEADERS})

install(TARGETS standard DESTINATION /usr/local/lib)
# install(FILES ${HEADERS} DESTINATION /usr/local/include/)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/" 
        DESTINATION "include/standard" 
        FILES_MATCHING
        PATTERN "*.h"
)

