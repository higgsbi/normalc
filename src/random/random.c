#include <time.h>
#include <stdlib.h>
#include "random.h"

void random_seed() {
	srand((unsigned) time(NULL));
}

int random_int(size_t start_inclusive, size_t end_inclusive) {
	if (start_inclusive > end_inclusive) {
		size_t temp = start_inclusive;
		start_inclusive = end_inclusive;
		end_inclusive = temp;
	} else if (start_inclusive == end_inclusive) {
		return start_inclusive;
	}

	size_t difference = end_inclusive + 1 - start_inclusive;
	return (rand() % difference) + start_inclusive;
}
