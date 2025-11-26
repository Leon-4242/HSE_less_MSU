#include <stdio.h>
#include "reflection_inverse_parallel.h"

int reflection_inverse(int n, double* a, double* res, double* time, double* d) {
	struct timeval start, end;
    long long start_us, end_us;
	int i = 0, j = 0, k = 0, t = 0;
	double s = 0, norm_a1 = 0, norm_x = 0, prod = 0;

	for (i = 0; i < n; ++i) {
		d[i] = 0.;
	}

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			res[i*n+j] = (i == j ? 1 : 0);
		}
	}

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
