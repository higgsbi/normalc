#include <normalc/random/random.h>
#include <stdio.h>

[[nodiscard]]
int test() {
	return 5 + 1;
}

int main() {
	random_seed();
	test();
	for (int i = 0; i < 100; i++) {
		int random = random_int(5, 10);	
		printf("%i\n", random);
	}

	return 0;
}
