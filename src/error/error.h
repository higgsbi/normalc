#ifndef STD_ERROR_H 
#define STD_ERROR_H 

#include "stdbool.h"
#include "stdio.h"

/**
 * Error defines a type that is either Error#OK or not
 */
typedef enum {
	OK = 0,
	BAD_RANGE = 1,
	BAD_FORMAT = 2,
} Error;

/**
 * Returns true if error is Error#OK
 */
bool ok(Error error);

/**
 * Retruns true if error is not Error#OK
 */
bool err(Error error);


/**
 * Exits system if field if null
 */
#define ASSERT_NONNULL(field)\
	if (!field) {\
		printf("\nNULL POINTER ERROR: %s is null\nSee: %s (%d)\n", #field, __FILE__, __LINE__);\
		exit(EXIT_FAILURE);\
	}\
	

/**
 * Exits system if start >= end
 */
#define ASSERT_VALID_RANGE(start, end)\
	if (start > end || end - start == 0) {\
		printf("\nRANGE ERROR: range of start %s to end %s (length %d) is invalid.\nSee: %s (%d)\n", #start, #end, end - start, __FILE__, __LINE__);\
		exit(EXIT_FAILURE);\
	}\

/**
 * Exits system if start >= end
 */
#define ASSERT_VALID_BOUNDS(collection, index, size)\
	if (index < 0 || index >= size) {\
		printf("\nBOUND ERROR: collection %s of size %d cannot be indexed at %d\nSee: %s (%d)\n", #collection, size, index, __FILE__, __LINE__);\
		exit(EXIT_FAILURE);\
	}\

#endif
