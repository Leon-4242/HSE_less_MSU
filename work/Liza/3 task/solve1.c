#include "solve1.h"

void* solve(void* arg) {
	ThreadArgs *args; int k = 0, start = 0, local_n_max = 0, buff = 0, j = 0, i = 0;
	double local_max = 0;
	args = (ThreadArgs*)arg;

	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->ind[i] = i;
	}

	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		for (j = 0; j < args->n; ++j) {
			args->b[i* args->n + j] = (i == j ? 1 : 0);
		}
	}

	pthread_barrier_wait(args->barrier);
	
	for (k = 0; k < args->n; ++k) {
		if (args->thread_id == 0) *args->s = 0;
		pthread_barrier_wait(args->barrier);

		local_n_max = k; local_max = fabs(args->a[k*args->n + args->ind[k]]);
		
		start = args->thread_id;
		while (start < k+1) {start += args->numThreads;}
		for (j = start; j < args->n; j += args->numThreads) {
			if (local_max < fabs(args->a[k*args->n + args->ind[j]])) {
				local_n_max = j; local_max = fabs(args->a[k*args->n + args->ind[j]]);
			}
		}

		pthread_mutex_lock(args->mutex);
		if (*args->s < local_max) {
			*args->s = local_max;
			*args->k = local_n_max;	
		}
		pthread_mutex_unlock(args->mutex);

		pthread_barrier_wait(args->barrier);

		if (args->thread_id == 0) {
			if (k != *args->k) {
				buff = args->ind[k];
				args->ind[k] = args->ind[*args->k];
				args->ind[*args->k] = buff;
			}
		}
/*		pthread_barrier_wait(args->barrier);
	
		if (args->thread_id == 0) {
			for (i = 0; i < args->n; ++i) {
				for (j = 0; j < args->n; ++j) {
					printf("%e ", args->a[i*args->n + args->ind[j]]);
				}
				printf("\n");
			}
		}
*/		pthread_barrier_wait(args->barrier);

		if (*args->s < 1e-15) return (void*)args->a;
		
		start = args->thread_id;
		while (start < k+1) {start += args->numThreads;}
		for (j = start; j < args->n; j += args->numThreads) {
			args->a[k*args->n + args->ind[j]] /= args->a[k*args->n+args->ind[k]];
		}

		for (j = args->thread_id; j < args->n; j += args->numThreads) {
			args->b[k*args->n + args->ind[j]] /= args->a[k*args->n+args->ind[k]];
		}

		pthread_barrier_wait(args->barrier);
		if (args->thread_id == 0) {
			args->a[args->ind[k]*args->n + k] = 1.;
		}

		pthread_barrier_wait(args->barrier);
	
		for (i = args->thread_id; i < args->n; i += args->numThreads) {
			if (i != k) {
				for (j = k+1; j < args->n; ++j) {
					args->a[i*args->n+args->ind[j]] -= args->a[i*args->n+args->ind[k]]*args->a[k*args->n+args->ind[j]]; 
				}
			}
		}

		for (i = args->thread_id; i < args->n; i += args->numThreads) {
			if (i != k) {
				for (j = 0; j < args->n; ++j) {
					args->b[i*args->n+args->ind[j]] -= args->a[i*args->n+args->ind[k]]*args->b[k*args->n + args->ind[j]]; 
				}
			}
		}
		pthread_barrier_wait(args->barrier);
	}
	
	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->buff[args->ind[i]] = i;
	}

	pthread_barrier_wait(args->barrier);

	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->ind[i] = args->buff[i];
	}

	pthread_barrier_wait(args->barrier);
    
	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		for (j = 0; j < args->n; ++j) {
			args->a[i*args->n + j] = args->b[args->ind[i]*args->n + j];
		}
	}

	pthread_barrier_wait(args->barrier);

	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		for (j = 0; j < args->n; ++j) {
			args->b[i*args->n + j] = args->a[i*args->n + j];
		}
	}
	
	return NULL;

}
/*
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
*/
