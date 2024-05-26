#ifndef STD_RANDOM_H
#define STD_RANDOM_H

#include <stdlib.h>

/**
 * Seeds the random number generator (required for use of all random functions)
 */
void random_seed();

/**
 * Returns a random integer between the start and end parameters inclusively.
 * If end is greater than start, the variables are flipped.
 * If end is equal to start, end is returned
 */
int random_int(size_t start_inclusive, size_t end_inclusive);

#endif
