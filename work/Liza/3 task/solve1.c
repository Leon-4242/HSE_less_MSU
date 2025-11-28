#include "solve1.h"

void* parallel_conversion(void* arg) {
	ThreadArgs* args; int i = 0, j = 0;
	args = (ThreadArgs*)arg;

	for (i = args->start; i < args->end; ++i) {
		if (i == args->k) continue;
		for (j = (args->k)+1; j < args->n; ++j) {
			(args->a)[ i*(args->n) + (args->ind)[j] ] -= (args->a)[i*(args->n)+(args->ind)[args->k]] * (args->a)[ (args->k)*(args->n) + (args->ind)[j] ]; 
		}

		for (j = 0; j < args->n; ++j) {
			(args->b)[ i*(args->n) + (args->ind)[j]] -= (args->a)[i*(args->n)+(args->ind)[args->k]] * (args->b)[ (args->k)*(args->n) + (args->ind)[j] ]; 
		}
	}

	return NULL;
}

int solve(int n, double *a, double *b, int *m, int* tmp, ThreadArgs* args, pthread_t* threads, int p) {
	int i = 0, j = 0, k = 0, n_max = 0, buff = 0, rows_per_thread = 0, reminder = 0, start_row = 0;
	double max = 0, norm = 0, sum = 0;

	rows_per_thread = n/p;
	reminder = n%p;

	for (i = 0; i < p; ++i) {
		(args+i)->a = a;
		(args+i)->b = b;
		(args+i)->n = n;
		(args+i)->ind = m;
	}

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

		start_row = 0;
		for (i = 0; i < p; ++i) {
			(args+i)->k = k;

			(args+i)->start = start_row;			
			(args+i)->end = start_row + rows_per_thread + (i < reminder ? 1 : 0);
			start_row = (args+i)->end;
			
			if (pthread_create(threads+i, NULL, parallel_conversion, args+i) != 0) {return 1;}
		}

		for (i = 0; i < p; ++i) {
			if (pthread_join(threads[i], NULL) != 0) {
			    return 1;
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

	return 0;
}
