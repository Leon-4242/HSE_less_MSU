#include <iostream>
#include "output.h"
#include "lu_linear_parallel.h"

void* lu_linear_parallel(void* arg) {
// n - размерность матрицы
// A - матрица n * n
// b - правая часть системы (размера n)
// x - выходной вектор решения (размера n)
// Возвращает 0 при успехе, 1 если матрица вырождена или возникло деление на ноль.
    ThreadArgs* args = (ThreadArgs*)arg;

	double* a = args->a;
	double* b = args->b;
	double* res = args->res;
	int n = args->n;
	int* pivot = args->ind;

	double* s = args->s;
	int* k = args->k;

	for (int k = 0; k < n; k++) {
		if (args->thread_id == 0) *args->s = 0;
		pthread_barrier_wait(args->barrier);

		/* ===== 1. Вычисляем столбец L(:,k) ===== */
		int start = args->thread_id;
		while (start < k) start += args->numThreads;
		for (int i = start; i < n; i += args->numThreads) {
		    double sum = 0.0;
		    for (int t = 0; t < k; t++)
		        sum += a[pivot[i]*n + t] * a[pivot[t]*n + k];

		    a[pivot[i]*n + k] -= sum;
		}

		pthread_barrier_wait(args->barrier);
	    /* ===== 2. Pivoting по столбцу k ===== */
	    int local_n_max = k;
		double local_max = fabs(a[pivot[k]*n+k]);
		start = args->thread_id;
		while (start < k+1) start += args->numThreads;
	    for (int i = start; i < n; i += args->numThreads) {
	        if (fabs(a[pivot[i]*n + k]) > local_max) {
	            local_n_max = i;
				local_max = fabs(a[pivot[i]*n+k]);
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
				int buff = pivot[k];
				pivot[k] = pivot[*args->k];
				pivot[*args->k] = buff;
			}
		}

		pthread_barrier_wait(args->barrier);

	    if (*args->s < 1e-15)
	        return (void*)args->a;  // матрица вырождена


	    /* ===== 3. Вычисляем строку U(k,:) ===== */
	    start = args->thread_id;
		while (start < k+1) start += args->numThreads;
		for (int j = start; j < n; j += args->numThreads) {
	        double sum = 0.0;
	        for (int t = 0; t < k; t++)
	            sum += a[pivot[k]*n + t] * a[pivot[t]*n + j];

	        a[pivot[k]*n + j] -= sum;
	        a[pivot[k]*n + j] /= a[pivot[k]*n + k];
	    }

		pthread_barrier_wait(args->barrier);
	}

    // Ly = b
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = args->thread_id; j < i; j += args->numThreads)
            sum += a[pivot[i]*n+j]*b[pivot[j]];

		pthread_mutex_lock(args->mutex);
        
		b[pivot[i]] -= sum;
	
		pthread_mutex_unlock(args->mutex);
	
		pthread_barrier_wait(args->barrier);

		if (args->thread_id == 0) 
			b[pivot[i]] /= a[pivot[i]*n+i];
		
		pthread_barrier_wait(args->barrier);
	}
    
    // Ux = y
    for (int i = n-1; i >= 0; i--) {
        double sum = 0;

		int start = args->thread_id;
		while (start < i+1) start += args->numThreads;
        for (int j = start; j < n; j += args->numThreads)
            sum += a[pivot[i]*n+j]*b[pivot[j]];
        
		pthread_mutex_lock(args->mutex);
        
		b[pivot[i]] -= sum;

		pthread_mutex_unlock(args->mutex);

		pthread_barrier_wait(args->barrier);
    }
	
	for (int i = args->thread_id; i < n; i += args->numThreads) 
		res[i] = b[pivot[i]];
        
    return NULL;
}

void r1_r2(int n, double* a, double* x, double* b, double& r1, double& r2) {
	double norm_ax_b = 0;
	for (int i = 0; i < n; ++i) {
		double sum = 0;
		for (int k = 0; k < n; ++k) {
			sum += a[i*n+k]*x[k];
		}
		norm_ax_b += fabs(sum - b[i]);
	}
	
	double norm_b = 0;
	for (int i = 0; i < n; ++i) {
		norm_b += fabs(b[i]);
	}

	r1 = norm_ax_b/norm_b;

	double norm_x_xx = 0;
	for (int i = 0; i < n; ++i) {
		norm_x_xx += fabs(x[i]-((i+1)%2));
	}

	double norm_xx = 0;
	for (int i = 0; i < n; ++i) {
		norm_xx += (i+1)%2;
	}

	r2 = norm_x_xx/norm_xx;
}
