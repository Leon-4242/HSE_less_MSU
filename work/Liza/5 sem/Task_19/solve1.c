#include "solve1.h"

int solve(int n, double *a, double *b, int *m) {
	int i, j, ind = 0, k, pos, mi, *p;
	double max, v, sum, norm = -1, u;
	for (i = 0; i < n; ++i) {
		m[i] = i;
	} 
	for (i = 0; i < n; ++i) {
		for (j = 0; j < i; ++j) {
			b[n*i + j] = 0;
		}
		b[n*i + i] = 1;
		for (j = i + 1; j < n; ++j) {
			b[n*i + j] = 0;
		}
	} //construct B = E
	for (j = 0; j < n; ++j) {
		sum = 0;
		for (i = 0; i < n; ++i) {
			sum += fabs(a[i*n + j]);
		}
		if (sum > norm) {
			norm = sum;
		}
	} //norm calculated
	for (i = 0; i < n; ++i) {
		max = -1;
		for (j = i; j < n; ++j) {
			v = fabs(a[i*n + m[j]]);
			if (v > max) {
				max = v;
				ind = j;
			}
		}
		mi = m[ind];
		m[ind] = m[i];
		m[i] = mi;
		if (max < EPS * norm) {
			return -1;
		}
		v = a[i*n + mi];
		//a[i*n + m[i]] = 1;
		for (j = i+1; j < n; ++j) {
			a[i*n + m[j]] /= v; //A
		}
		for (j = 0; j < n; ++j) {
			b[i*n + j] /= v; //B
		}
		for (j = i + 1; j < n; ++j) {
			u = a[n*j + mi];
			p = m + i + 1;
			for (k = i + 1; k < n; ++k) {
				pos = *p; //1
				a[n*j + pos] -= u * a[n*i + pos]; //*
				++p;
			}
			for (k = 0; k < n; ++k) {
				b[n*j + k] -= u * b[n*i + k]; //*
			}
		}
	}// make zeros under diagonal
	for (i = n - 1; i > 0; --i) {
		for (j = 0; j < i; ++j) {
			u = a[n*j + m[i]];
			for (k = 0; k < n; ++k) {
				b[n*j + k] -= u * b[n*i + k]; //*
			}
		}
	}// back way Jordan
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			a[n*i + j] = b[n*i + j];
		}
	}
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			b[m[i]*n + j] = a[i*n + j];
		}
	}
	return 0;
}
