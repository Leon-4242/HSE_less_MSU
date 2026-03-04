#include <stdio.h>
#include "jordan_linear_parallel.h"

void* jordan_linear_parallel(void* arg) {
	ThreadArgs *args; int k = 0, start = 0, local_n_max = 0, buff = 0, j = 0, i = 0;
	double local_max = 0;
	args = (ThreadArgs*)arg;

	
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

		pthread_barrier_wait(args->barrier);

		if (*args->s < 1e-15) return (void*)args->a;
		
		start = args->thread_id;
		while (start < k+1) {start += args->numThreads;}
		for (j = start; j < args->n; j += args->numThreads) {
			args->a[k*args->n + args->ind[j]] /= args->a[k*args->n+args->ind[k]];
		}

		pthread_barrier_wait(args->barrier);
		if (args->thread_id == 0) {
			args->b[k] /= args->a[k*args->n + args->ind[k]];
			args->a[k*args->n + args->ind[k]] = 1.;
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
				args->b[i] -= args->a[i*args->n+args->ind[k]]*args->b[k]; 
			}
		}
		pthread_barrier_wait(args->barrier);
	}

	for (i = args->thread_id; i < args->n; i+= args->numThreads) {
		args->buff[args->ind[i]] = i;
	}	

	pthread_barrier_wait(args->barrier);

	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->ind[i] = args->buff[i];
	}

	pthread_barrier_wait(args->barrier);

    for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->res[i] = args->b[args->ind[i]];
	}
	
	return NULL;
}

void r1_r2(int n, double* a, double* x, double* b, double* r1, double* r2, double* time) {
	double norm_ax_b = 0, norm_b = 0, norm_x_xx = 0, norm_xx = 0, sum = 0;
	int i = 0, k = 0;

	struct timeval start, end;
    long long start_us, end_us;
    
	gettimeofday(&start, NULL);

	norm_ax_b = 0;
	for (i = 0; i < n; ++i) {
		sum = 0;
		for (k = 0; k < n; ++k) {
			sum += a[i*n+k]*x[k];
		}
		norm_ax_b += fabs(sum - b[i]);
	}
	
	norm_b = 0;
	for (i = 0; i < n; ++i) {
		norm_b += fabs(b[i]);
	}

	*r1 = norm_ax_b/norm_b;

	norm_x_xx = 0;
	for (i = 0; i < n; ++i) {
		norm_x_xx += fabs(x[i]-((i+1)%2));
	}

	norm_xx = 0;
	for (i = 0; i < n; ++i) {
		norm_xx += (i+1)%2;
	}

	*r2 = norm_x_xx/norm_xx;

    gettimeofday(&end, NULL);
 
    start_us = start.tv_sec * 1000000 + start.tv_usec;
    end_us = end.tv_sec * 1000000 + end.tv_usec;
 
    *time = (double)(end_us - start_us)/1000000.;
}
