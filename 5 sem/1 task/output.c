#include "output.h"

void output(int r, int l, int n, double* a) {
	int i = 0, j = 0;

	for (i = (l-r < 0 ? 0 : l-r); i < l; ++i) {
		for (j = (n-r < 0 ? 0 : n-r) ; j < n; ++j) {
			printf(" %10.3e", a[i*n+j]);
		}
		printf("\n");
	}
}
