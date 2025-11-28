#include "solve1.h"

typedef struct {
	double* a;
	double* b;
	int n;
	int* ind;

	int i;
	int k;
	double coef;
} ThreadArgs;

void* parallel_conversion(void* arg) {
	ThreadArgs* args; int j = 0;
	args = (ThreadArgs*)arg;

	for (j = (args->k)+1; j < args->n; ++j) {
		(args->a)[ (args->i)*(args->n) + (args->ind)[j] ] -= (args->coef) * (args->a)[ (args->k)*(args->n) + (args->ind)[j] ]; 
	}

	for (j = 0; j < args->n; ++j) {
		(args->b)[ (args->i)*(args->n) + (args->ind)[j]] -= (args->coef) * (args->b)[ (args->k)*(args->n) + (args->ind)[j] ]; 
	}

	return NULL;
}

int solve(int n, double *a, double *b, int *m, int* tmp) {
	ThreadArgs* args; pthread_t* threads;

	int i = 0, j = 0, k = 0, n_max = 0, buff = 0;
	double max = 0, norm = 0, sum = 0;

	args = (ThreadArgs*)malloc(n*sizeof(ThreadArgs));
	for (i = 0; i < n; ++i) {
		(args+i)->a = a;
		(args+i)->b = b;
		(args+i)->n = n;
		(args+i)->ind = m;
	}
	threads = (pthread_t*)malloc(n*sizeof(pthread_t));

	for (j = 0; j < n; ++j) m[j] = j;

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			b[i*n + j] = (i == j) ? 1 : 0;
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

	for (k = 0;  k < n; ++k) {
		n_max = k; max = fabs(a[k*n+m[k]]);
		for (j = k+1; j < n; ++j) {
			if (max < fabs(a[k*n+m[j]])) {n_max = j; max = fabs(a[k*n+m[j]]);}
		}
		
		if (k != n_max) {
			buff = m[k];
			m[k] = m[n_max];
			m[n_max] = buff;
		}

		if (max < EPS * norm) {return -1;}

		for (j = k+1; j < n; ++j) {
			a[k*n + m[j]] /= a[k*n+m[k]];
		}
		for (j = 0; j < n; ++j) {
			b[k*n + j] /= a[k*n+m[k]];
		}	
		a[k*n+m[k]] = 1.;

		for (i = 0; i < n; ++i) {
			if (i != k) {
				(args+i)->i = i;
				(args+i)->k = k;
				(args+i)->coef = a[i*n+m[k]];
				if (pthread_create(threads+i, NULL, parallel_conversion, args+i) != 0) {return 1;}
			}
		}

		for (i = 0; i < n; ++i) {
			if (i != k) {
				if (pthread_join(threads[i], NULL) != 0) {
				    return 1;
				}
			}
		}
	}

	for (i = 0; i < n; ++i) tmp[m[i]] = i;
	for (i = 0; i < n; ++i) m[i] = tmp[i];

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			a[i*n + j] = b[m[i]*n + j];
		}
	}

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			b[i*n + j] = a[i*n + j];
		}
	}

	free(args);
	free(threads);

	return 0;
}
