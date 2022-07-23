#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

extern unsigned long long FOO(void* from, void* to, size_t stride_len);


static char* get_array(int elems) {
	char* s = calloc(elems, 1);
        char x = (char)rand();
//	memset(s, x, elems);
	return s;
}


// my CPU:
// 12 cores
// 32KB L1 per core
// 512KB L2 per core
// 64MB L3 in total

// L1: 512 cachelines per core, 6144 total

int main(int argc, char** argv) {

	int stride = atoi(argv[1]);
	printf("Stride: %d\n", stride);

	const int len = 1024 * 1024 * 1024;
	char* c = malloc(len);
	memset(c, 41, len);

	unsigned long long l = 0;
	int target_access_num = INT_MAX - 1;
	int access_per_run = 6000;
	int repeats = target_access_num / access_per_run;
	printf("%d repeats, %d stride\n", repeats, stride);
	
	for (int i = 0; i < repeats; i ++) {
		l = FOO(c, c + (stride * access_per_run), stride);
	}

	printf("%llu\n", l);

	return 0;
}
