#ifndef NORMALC_IO_H
#define NORMALC_IO_H

#include "path.h"
#include "../string/string.h"

/*
 * Redefining DEFAULT_LINE_PER_FILE larger will result in less reallocations for larger files,
 * but will cause excess memory allocations for smaller average files
 */
#define DEFAULT_LINE_PER_FILE 10 

String* io_file_read(Path* path);
Vector* io_file_read_lines(Path* path);
Vector* io_file_read_n_lines(Path* path, int n);
bool io_input_read_line(String** dest);

#endif
