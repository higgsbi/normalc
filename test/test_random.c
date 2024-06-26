#include <normalc/random/random.h>
#include <stdio.h>

int main() {
	random_seed();
	for (int i = 0; i < 100; i++) {
		int random = random_int(5, 10);	
		printf("%i\n", random);
	}

	return 0;
}
