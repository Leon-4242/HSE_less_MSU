#include "output.h"

void output(int r, int l, int n, double* a) {
	int i = 0, j = 0;

	for (i = 0; i < (r < l ? r : l); ++i) {
		for (j = 0 ; j < (r < n ? r : n); ++j) {
			printf(" %10.3e", a[i*n+j]);
		}
		printf("\n");
	}
}
