#include <stdio.h>
#include <stdlib.h>
#include "SFMT.h"

int main(int argc, char* argv[]) {
    int i, cnt, seed;
    double x, y, pi;
    const int NUM = 1002;
    sfmt_t sfmt;

    if (argc >= 2) {
	seed = strtol(argv[1], NULL, 10);
    } else {
	seed = 12345;
    }
    cnt = 0;
    sfmt_init_gen_rand(&sfmt, seed);
    for (i = 0; i < NUM; i++) {
	x = sfmt_genrand_res53(&sfmt);
        printf("%lf\n", x);
	}
    return 0;
}
