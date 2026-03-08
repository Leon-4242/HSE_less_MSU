#include <stdio.h>
#include "lu_linear_parallel.h"

void* lu_linear_parallel(void* arg) {
    ThreadArgs *args;
    int k = 0, start = 0, local_n_max = 0, buff = 0;
    int i = 0, j = 0, t = 0;
    double local_max = 0, sum = 0;

    args = (ThreadArgs*)arg;

    for (k = 0; k < args->n; ++k) {
        if (args->thread_id == 0) *args->s = 0;
        pthread_barrier_wait(args->barrier);

		/* ===== 1. Вычисляем столбец L(:,k) ===== */
		
		start = args->thread_id;
		while(start < k) start += args->numThreads;
		for (i = start; i < args->n; i += args->numThreads) {
			sum = 0;
			for (t = 0; t < k; ++t) {
				sum += args->a[args->ind[i]*args->n+t]*args->a[args->ind[t]*args->n+k];
			}
			args->a[args->ind[i]*args->n+k] -= sum;
		}

		pthread_barrier_wait(args->barrier);

	    /* ===== 2. Перестановка по столбцу k ===== */
        local_n_max = k;
        local_max = fabs(args->a[args->ind[k]*args->n + k]);

        start = args->thread_id;
        while (start < k+1) start += args->numThreads;
        for (i = start; i < args->n; i += args->numThreads) {
            if (local_max < fabs(args->a[args->ind[i]*args->n + k])) {
                local_n_max = i;
                local_max = fabs(args->a[args->ind[i]*args->n + k]);
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

	    /* ===== 3. Вычисляем строку U(k,:) ===== */
    
		start = args->thread_id;
        while (start < k+1) start += args->numThreads;
        for (j = start; j < args->n; j += args->numThreads) {
			sum = 0;
			for (t = 0; t < k; ++t) {
				sum += args->a[args->ind[k]*args->n+t]*args->a[args->ind[t]*args->n+j];
			}

			args->a[args->ind[k]*args->n+j] -= sum;
			args->a[args->ind[k]*args->n+j] /= args->a[args->ind[k]*args->n+k];
		}

        pthread_barrier_wait(args->barrier);
    }

    /* =========================================
       Forward substitution (Ly = Pb)
       ========================================= */

    for (i = 0; i < args->n; i++) {
		sum = 0;
		for (j = args->thread_id; j < i; j += args->numThreads) {
			sum += args->a[args->ind[i]*args->n + j]*args->b[args->ind[j]];
		}

		pthread_mutex_lock(args->mutex);

		args->b[args->ind[i]] -= sum;

		pthread_mutex_unlock(args->mutex);

		pthread_barrier_wait(args->barrier);

		if (args->thread_id == 0) {
			args->b[args->ind[i]] /= args->a[args->ind[i]*args->n + i];
		}

		pthread_barrier_wait(args->barrier);
    }

    /* =========================================
       Back substitution (Ux = y)
       ========================================= */

    for (i = args->n - 1; i >= 0; i--) {
		sum = 0;
		start = args->thread_id;
		while(start < i+1) start += args->numThreads;
		for (j = start; j < args->n; j += args->numThreads) {
			sum += args->a[args->ind[i]*args->n+j]*args->b[args->ind[j]];
		}

		pthread_mutex_lock(args->mutex);

		args->b[args->ind[i]] -= sum;

		pthread_mutex_unlock(args->mutex);

		pthread_barrier_wait(args->barrier);

	}
	
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
