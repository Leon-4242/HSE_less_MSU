#include "qr_3diagonal.h"
#include "output.h"

int min(int a, int b);

int qr_3diagonal(int n, double* a, double* res, double eps, double* t1, double* t2, int* its) {
//page 67 - to 3diagonal by round
//page 126 - qr for 3diagonal by reflection
	struct timeval start, end;
    long long start_us, end_us;
	int i = 0, j = 0, k = 0, iter = 0, m = 0, t = 0, iterk = 0;
	double cos = 0, sin = 0, tmp = 0, norm_A = 0, sum = 0, 
		   s_k = 0, norm_a1 = 0, prod = 0, norm_x = 0, s = 0;
	double x_k[2] = {0, 0}, x_old[2] = {0, 0};

	m = n;
	gettimeofday(&start, NULL);

	for (i = 0; i < n; ++i) {
		sum = 0;
		for (j = 0; j < n; ++j) {
			sum += fabs(a[i*n+j]);
		}
		if (i == 0) {norm_A = sum;}
		else if (norm_A < sum) {norm_A = sum;}
	}

//	printf("\n\nnorm_A = %e\n",norm_A);

	for (k = 0; k < n-2; ++k) {
		/* 
		 *	k+1, k     k+1, n
		 *
		 *	n, k       n, n
		 */
		for (i = k+2; i < n; ++i) {
			if (fabs(a[i*n+k]) < eps) continue;
			tmp = sqrt(a[(k+1)*n+k]*a[(k+1)*n+k] + a[i*n+k]*a[i*n+k]);
			if (fabs(tmp) < 1e-15) continue;
			cos = a[(k+1)*n+k]/tmp; sin = - a[i*n+k]/tmp;

			a[i*n+k] = 0;
			a[(k+1)*n + k] = tmp;
			for (j = k+1; j < n; ++j) {
				tmp = a[(k+1)*n+j]*cos -a[i*n+j]*sin;
				a[i*n+j] = a[(k+1)*n+j]*sin +a[i*n+j]*cos;
				a[(k+1)*n+j] = tmp;
			}

			for (j = k+1; j < n; ++j) {
				if (j == k+1) {
					tmp = a[(k+1)*n+(k+1)]*cos-a[(k+1)*n+i]*sin;
					a[(k+1)*n+i] = a[(k+1)*n+(k+1)]*sin+a[(k+1)*n+i]*cos;
					a[(k+1)*n+(k+1)] = tmp;
				}
				else if (j == i) {
					tmp = a[i*n+(k+1)]*cos-a[i*n+i]*sin;
					a[i*n+i] = a[i*n+(k+1)]*sin+a[i*n+i]*cos;
					a[i*n+(k+1)] = tmp;
				} else {
					a[j*n+i] = a[i*n+j];
					a[j*n+(k+1)] = a[(k+1)*n+j];
				}
			}
		}
		a[k*n+(k+1)] = a[(k+1)*n+k];
		for (j = k+2; j < n; ++j) a[k*n+j] = 0;

	}

	gettimeofday(&end, NULL);

	start_us = start.tv_sec * 1000000 + start.tv_usec;
	end_us = end.tv_sec * 1000000 + end.tv_usec;

	*t1 = (double)(end_us - start_us) / 1000000.;

	printf("\n\nSym: \n");
	output(n, n, n, a);
	printf("\n");

	gettimeofday(&start, NULL);

	while (m > 2) {
		iterk = 0;
		while (fabs(a[(m-1)*n+m-2]) > eps*norm_A) {
//			if(iter > 7) break;
			s_k = a[(m-1)*n+(m-1)];
			for (i = 0; i < m; ++i) {a[i*n+i] -= s_k;}
		
//			printf("\nbefore iter %d\n", iter+1);
//			output(n, n, n, a);
//			printf("\n");

			for (k = 0; k < m; ++k) {
				s = 0;
				for (i = k+1; i < min(k+2, m); ++i) {s += a[i*n+k]*a[i*n+k];}

				norm_a1 = sqrt(a[k*n+k]*a[k*n+k] + s);
	
				x_k[0] = a[k*n+k]-norm_a1;
				if (k != m-1) x_k[1] = a[(k+1)*n+k];
				else x_k[1] = 0;

//				printf("\nx_k[0] = %e; x_k[1] = %e\n", x_k[0], x_k[1]);				
				
				norm_x = sqrt(x_k[0]*x_k[0] + s);
				if (!(fabs(norm_x) < 1e-15)) {

					x_k[0] /= norm_x;
					if (k != m-1) {x_k[1] /= norm_x;}

//					printf("\nnorm_x_old = %e; x_k[0] = %e; x_k[1] = %e\n", norm_x, x_k[0], x_k[1]);		
				
					a[k*n+k] = norm_a1;
					if (k != m-1) a[(k+1)*n+k] = 0;

					for (j = k+1; j < min(k+3, m); ++j) {
						if(j == k+2) prod = 0;
						else prod = x_k[0]*a[k*n+j];
					
						for (t = k+1; t < min(k+2, m); ++t) {prod += x_k[t-k]*a[t*n+j];}
						prod *= 2;
					
						for (t = k; t < min(k+2, m); ++t) {
							if (j == k+2 && t == k) continue;
							a[t*n+j] -= prod*x_k[t-k];
						}
					}
				}
				
				if (k != 0) {
					for (i = k-1; i < min(k+1, m); ++i) {
						prod = x_old[0]*a[i*n+(k-1)];
						for (t = k; t < min(k+1, m); ++t) {prod += x_old[t-(k-1)]*a[i*n+t];}
						prod *= 2;
						for (t = k-1; t < min(k+1, m); ++t) {a[i*n+t] -= prod*x_old[t-(k-1)];}
					}
					a[(k-1)*n+k] = a[k*n+(k-1)];
				}
	
				x_old[0] = x_k[0]; x_old[1] = x_k[1];

				if (k == m-1) {
					prod = 2*x_k[0]*a[(m-1)*n+(m-1)];
					a[(m-1)*n+(m-1)] -= prod*x_k[0];
				}

//				printf("\n\n k = %d\n", k);
//				output(n, n, n, a);
//				printf("\n");
				
			}

			for (i = 0; i < m; ++i) {a[i*n+i] += s_k;}
			++iterk;
			
			if (iterk > 50) {printf("\nm = %d\n", m); break;}
			printf("\n\n iter = %d; m = %d\n", iterk, m);
//			printf("\nelem = %e\n",fabs(a[(m-1)*n+m-2]));
			output(n, n, n, a);
			printf("\n");
		}
		res[m-1] = a[(m-1)*n+(m-1)];
		iter += iterk;
		--m;
	}

	if (m == 2) {
		tmp = sqrt((a[0]-a[n+1])*(a[0]-a[n+1])+4*a[n]*a[1]);
		res[0] = (a[0]+a[n+1]+tmp)/2.;
		res[1] = (a[0]+a[n+1]-tmp)/2.;
	} else if (m == 1) {
		res[0] = a[0];
	}

	gettimeofday(&end, NULL);

	start_us = start.tv_sec * 1000000 + start.tv_usec;
	end_us = end.tv_sec * 1000000 + end.tv_usec;

	*t2 = (double)(end_us - start_us) / 1000000.;

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
