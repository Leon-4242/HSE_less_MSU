#include <stdio.h>
#include "reflection_inverse_parallel.h"

void* reflection_inverse_parallel(void* arg) {
	ThreadArgs* args; int i = 0, j = 0, k = 0, t = 0, start = 0;
	double norm_a1 = 0, prod = 0, s = 0;

	args = (ThreadArgs*)arg;

	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->d[i] = 0.;
	}
	
	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		for (j = 0; j < args->n; ++j) {
			args->b[i*args->n+j] = (i == j ? 1 : 0);
		}
	}

//	pthread_barrier_wait(args->barrier);
	barrier_wait(args->barrier);	
	for (k = 0; k < args->n; ++k) {
		if (args->thread_id == 0) {
			pthread_mutex_lock(args->mutex);
			*args->s = 0;
			pthread_mutex_unlock(args->mutex);
		}
		barrier_wait(args->barrier);	
//		pthread_barrier_wait(args->barrier);

		start = args->thread_id;
		while (start < k+1) {start += args->numThreads;}
		for(i = start; i < args->n; i += args->numThreads) {
			pthread_mutex_lock(args->mutex);
			*args->s += args->a[i * args->n + k] * args->a[i * args->n + k];
			pthread_mutex_unlock(args->mutex);
		}

		barrier_wait(args->barrier);	
//		pthread_barrier_wait(args->barrier);

		if (args->thread_id == 0) {
			norm_a1 = sqrt(args->a[k*args->n + k] * args->a[k* args->n + k] + *args->s);
			args->a[k*args->n + k] -= norm_a1;
			args->d[k] = norm_a1;
			*args->s = sqrt(args->a[k*args->n+k]*args->a[k*args->n+k] + *args->s);
		}

		barrier_wait(args->barrier);	
//		pthread_barrier_wait(args->barrier);

		if (fabs(args->d[k]) < 1e-15)
			return (void*)args->a;
				
		if (fabs(*args->s) < 1e-15) continue;

		start = args->thread_id;
		while (start < k) {start += args->numThreads;}
		for (i = start; i < args->n; i += args->numThreads) {
			args->a[i * args->n + k] /= *args->s;
		}

		barrier_wait(args->barrier);	
//		pthread_barrier_wait(args->barrier);

		start = args->thread_id;
		while (start < k+1) {start += args->numThreads;}
		for (i = start; i < args->n; i += args->numThreads) {
			prod = args->a[k*args->n+k]*args->a[k*args->n+i];
			for (t = k+1; t < args->n; ++t) {prod += args->a[t*args->n+k]*args->a[t*args->n+i];}
			prod *= 2;
			for (t = k; t < args->n; ++t) {args->a[t*args->n+i] -= prod* args->a[t*args->n+k];}
		}
		
		for (i = args->thread_id; i < args->n; i += args->numThreads) {
			prod = args->a[k*args->n+k]*args->b[k*args->n+i];
			for (t = k+1; t < args->n; ++t) {prod += args->a[t*args->n+k]*args->b[t*args->n+i];}
			prod *= 2; 
			for (t = k; t < args->n; ++t) {args->b[t*args->n+i] -= prod*args->a[t*args->n+k];}
		}

		barrier_wait(args->barrier);	
//		pthread_barrier_wait(args->barrier);
	}

	for (i = args->thread_id; i < args->n; i += args->numThreads) {
		for (k = args->n-1; k >= 0; --k) {
			s = 0;
			for (j = k+1; j < args->n; ++j) {
				s += args->a[k*args->n+j]*args->b[j*args->n+i];
			}
			args->b[k*args->n+i] = (args->b[k*args->n+i] - s)/(args->d[k]);
		}
	}

	return NULL;
}

/*
int reflection_inverse(int n, double* a, double* res, double* time, double* d, ThreadArgs* args, pthread_t* threads, pthread_barier_t barier) {
	struct timeval start, end;
    long long start_us, end_us;
	int i = 0, j = 0, k = 0, t = 0;
	double s = 0, norm_a1 = 0, norm_x = 0, prod = 0;

	gettimeofday(&start, NULL);

	for (k = 0;  k < n; ++k) {
		s = 0;
		for (j = k+1; j < n; ++j) {
			s+= a[j*n+k]*a[j*n+k];
		}
		norm_a1 = sqrt(a[k*n+k]*a[k*n+k]+s);
		a[k*n+k] -= norm_a1;
		d[k] = norm_a1;
		if (fabs(d[k]) < 1e-15) {
			gettimeofday(&end, NULL);
 
		    start_us = start.tv_sec * 1000000 + start.tv_usec;
		    end_us = end.tv_sec * 1000000 + end.tv_usec;
 
		    *time = (double)(end_us - start_us) / 1000000.;
			return -1;
		}

		norm_x = sqrt(a[k*n+k]*a[k*n+k] + s);
		if (fabs(norm_x) < 1e-15) continue;	
		for (j = k; j < n; ++j) {
			a[j*n+k] /= norm_x;
		}

		for (j = k+1; j < n; ++j) {
			prod = a[k*n+k]*a[k*n+j];
			for (t = k+1; t < n; ++t) {prod += a[t*n+k]*a[t*n+j];}
			prod *= 2;
			for (t = k; t < n; ++t) {a[t*n+j] -= prod*a[t*n+k];}
		}
	
		for (j = 0; j < n; ++j) {
			prod = a[k*n+k]*res[k*n+j];
			for (t = k+1; t < n; ++t) {prod += a[t*n+k]*res[t*n+j];}
			prod *= 2; 
			for (t = k; t < n; ++t) {res[t*n+j] -= prod*a[t*n+k];}
		}
	}

	for (i = 0; i < n; ++i) {
		for (k = n-1; k >= 0; --k) {
			s = 0;
			for (j = k+1; j < n; ++j) {
				s += a[k*n+j]*res[j*n+i];
			}
			res[k*n+i] = (res[k*n+i] - s)/d[k];
		}
	}

	
    gettimeofday(&end, NULL);
 
    start_us = start.tv_sec * 1000000 + start.tv_usec;
    end_us = end.tv_sec * 1000000 + end.tv_usec;
 
    *time = (double)(end_us - start_us)/1000000.;

	return 0;
}
*/
void r1_r2(int n, double* a, double* inv_a, double* r1, double* r2, double* time) {
	struct timeval start, end;
    long long start_us, end_us;
    
	gettimeofday(&start, NULL);
 	
	*r1 = norm_ab_e(n, a, inv_a);
	*r2 = norm_ab_e(n, inv_a, a);

    gettimeofday(&end, NULL);
 
    start_us = start.tv_sec * 1000000 + start.tv_usec;
    end_us = end.tv_sec * 1000000 + end.tv_usec;
 
    *time = (double)(end_us - start_us)/1000000.;
}

double norm_ab_e(int n, double* a, double* b) {
	int i = 0, j = 0, k = 0; double buff = 0, sum = 0, max = 0;
	
	if (n > 11000) return 0;

	for (j = 0; j < n; ++j) {
		sum = 0;
		for (i = 0; i < n; ++i) {
			buff = (i == j ? -1 : 0);
			for (k = 0; k < n; ++k) {buff += a[i*n+k]*b[k*n+j];}
			/*tmp[i*n+j] = buff */
			sum += (buff < 0 ? -buff : buff);
		}
		if (j == 0) max = sum;
		else max = (max < sum ? sum : max);
	}

	return max;
}
