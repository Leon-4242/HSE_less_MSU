#include <stdio.h>
#include "jordan_linear.h"

int jordan_linear(int n, double* a, double* res, double* b, double* time, int* indi) {
	struct timeval start, end;
    long long start_us, end_us;
	int i = 0, j = 0, k = 0, t = 0, n_max = 0, buff = 0;
	double max = 0, s = 0, norm_a1 = 0, norm_x = 0, prod = 0;

	gettimeofday(&start, NULL);

	for (k = 0;  k < n; ++k) {
		n_max = k; max = a[indi[k]*n+k];
		for (i = k+1; i < n; ++i) {
			if (max < a[indi[i]*n+k]) {n_max = i; max = a[indi[i]*n+k];}
		}
		
		if (k != n_max) {
			buff = indi[k];
			indi[k] = indi[n_max];
			indi[n_max] = buff;
		}

		if (fabs(a[indi[k]*n+k]) < 1e-15) {
			gettimeofday(&end, NULL);
 
		    start_us = start.tv_sec * 1000000 + start.tv_usec;
		    end_us = end.tv_sec * 1000000 + end.tv_usec;
 
		    *time = (double)(end_us - start_us) / 1000000.;
			return -1;
		}

		for (j = k+1; j < n; ++j) {
			a[indi[k]*n+j] /= a[indi[k]*n+k];
		}
		b[indi[k]] /= a[indi[k]*n+k];
		a[indi[k]*n+k] = 1.;

		for (i = 0; i < n; ++i) {
			if (i != k) {
				for (j = k+1; j < n; ++j) {
					a[indi[i]*n+j] -= a[indi[i]*n+k]*a[indi[k]*n+j]; 
				}
			}
		}

		for (i = 0; i < n; ++i) {
			if (i != k) {
				b[indi[i]] -= a[indi[i]*n+k]*b[indi[k]]; 
			}
		}
	}

    for (i = 0; i < n; ++i) {
		res[i] = b[indi[i]];
	}
	
	gettimeofday(&end, NULL);
 
    start_us = start.tv_sec * 1000000 + start.tv_usec;
    end_us = end.tv_sec * 1000000 + end.tv_usec;
 
    *time = (double)(end_us - start_us)/1000000.;

	return 0;
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
