#include "jacobi_max.h"
#include "output.h"

int min(int a, int b);

int jacobi_max(int n, double* a, double* res, double eps, double* t, int* its) {
	struct timeval start, end;
    long long start_us, end_us;
	int i = 0, j = 0, i_max = 0, j_max = 0, sign = 0, iter = 0; double sigma = 0, max_a_ij = 0, cos = 0, sin = 0, x = 0, y = 0, tmp = 0;
	gettimeofday(&start, NULL);

	if (n == 1) {
		res[0] = a[0];
		gettimeofday(&end, NULL);

		start_us = start.tv_sec * 1000000 + start.tv_usec;
		end_us = end.tv_sec * 1000000 + end.tv_usec;

		*t = (double)(end_us - start_us) / 1000000.;

		*its = 0;
		return 0;
	}

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			if (i != j) sigma += a[i*n+j]*a[i*n+j];
		}
	}

	while (sigma > eps) {
		
//		printf("\n\n");
//		output(n, n, n, a);
//		printf("\n\n");
		max_a_ij = a[1];
		i_max = 0;
		j_max = 1;
		for (i = 0; i < n; ++i) {
			for (j = i+1; j < n; ++j) {
				if (fabs(a[i*n+j]) > fabs(max_a_ij)) {
					max_a_ij = a[i*n+j];
					i_max = i;
					j_max = j;
				}
			}
		}

		if (fabs(max_a_ij) < eps) {
			break;
		}

		x = -2*max_a_ij;
		y = a[i_max*n+i_max]-a[j_max*n+j_max];

		if (fabs(y) < eps) {
			cos = 1./sqrt(2);
			sin = cos;
		} else {
			tmp = sqrt(x*x+y*y);
			cos = sqrt(0.5 * (1 + fabs(y)/tmp) );
			sign = (x < 0 ? -1 : 1)*(y < 0 ? -1 : 1);
			sin = (sign*fabs(x))/(2 * cos * tmp);
		}

		for (j = 0; j < n; ++j) {
			if (j != i_max && j != j_max) {
				tmp = a[i_max*n+j]*cos - a[j_max*n+j]*sin;
				a[j_max*n+j] = a[i_max*n+j]*sin + a[j_max*n+j]*cos;
				a[i_max*n+j] = tmp;
			}
		}

		for (i = 0; i < n; ++i) {
			if (i != i_max && i != j_max) {
				a[i*n+i_max] = a[i_max*n+i];
				a[i*n+j_max] = a[j_max*n+i];
			}
		}

		tmp = cos*cos*a[i_max*n+i_max]+sin*sin*a[j_max*n+j_max]-2*cos*sin*max_a_ij;
		
		a[j_max*n+j_max] = cos*cos*a[j_max*n+j_max]+sin*sin*a[i_max*n+i_max]+2*cos*sin*max_a_ij;
		a[i_max*n+i_max] = tmp;
		
		a[i_max*n+j_max] = 0.;
		a[j_max*n+i_max] = 0.;
		
		sigma -= 2*max_a_ij*max_a_ij;

		++iter;
	}
	
	for (i = 0; i < n; ++i) {
		res[i] = a[i*n+i];
	}
	gettimeofday(&end, NULL);

	start_us = start.tv_sec * 1000000 + start.tv_usec;
	end_us = end.tv_sec * 1000000 + end.tv_usec;

	*t = (double)(end_us - start_us) / 1000000.;

	*its = iter;
	return 0;
}

void r1_r2(int n, double* a, double* lambda, double* r1, double* r2) {
// A_inf = max_i sum_j fabs(a[i*n+j])
	int i = 0, j = 0; double sum = 0, max = 0, norm_A = 0, sigma = 0;

	for (i = 0; i < n; ++i) {
		sum = 0;
		for (j = 0; j < n; ++j) {
			sum += fabs(a[i*n+j]);
		}
		if (i == 0) {norm_A = sum;}
		else if (norm_A < sum) {norm_A = sum;}
	}

	sum = 0;
	for (i = 0; i < n; ++i) {
		sum += a[i*n+i]-lambda[i];
	}
	*r1 = fabs(sum)/norm_A;

	sigma = 0;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			sigma += a[i*n+j]*a[j*n+i];
		}
	}
	sigma = sqrt(sigma);

	sum = 0;
	for (i = 0; i < n; ++i) {
		sum += lambda[i]*lambda[i];
	}
	sum = sqrt(sum);

	*r2 = fabs(sigma-sum)/norm_A;
}

int min(int a, int b) {
	return (a < b ? a : b);
}
