#include "qr_3diagonal.h"
#include "output.h"

int min(int a, int b);

int qr_3diagonal(int n, double* a, double* res, double eps, double* t1, double* t2, int* its, double* x) {
	struct timeval start, end;
    long long start_us, end_us;
	int i = 0, j = 0, k = 0, iter = 0, m = 0, t = 0;
	double tmp = 0, norm_A = 0, sum = 0, c = 0, s = 0,
		   s_k = 0, norm_a1 = 0, prod = 0, norm_x = 0, xk = 0, zk = 0, d_new_k = 0, d_new_k1 = 0, e_new_k  = 0;

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

	for (k = 0; k < n-1; ++k) {
		s_k = 0;
		for(i = k+2; i < n; ++i) {s_k += a[i*n+k]*a[i*n+k];}

		norm_a1 = sqrt(a[(k+1)*n+k]*a[(k+1)*n+k]+s_k);

		for(i = k+1; i < n; ++i) {
			x[i] = a[i*n+k];
		}
		x[k+1] -= norm_a1;
		
		norm_x = sqrt(x[k+1]*x[k+1] + s_k);
		if (fabs(norm_x) < eps) continue;

		for(i = k+1; i < n; ++i) {
			x[i] /= norm_x;
		}

		for(i = k+1; i < n; ++i) {
			a[i*n+k] = (i == k+1) ? norm_a1 : 0;
			a[k*n+i] = a[i*n+k];
		}

		for (i = k+1; i < n; ++i) {
			sum = 0;
			for (j = k+1; j < n; ++j) {
				sum += a[i*n+j]*x[j];
			}
			x[i+n] = sum;
		}

		prod = 0;
		for (i = k+1; i < n; ++i) {prod += x[i]*x[i+n];}
		prod *= 2;
		
		for (i = k+1; i < n; ++i) {
			x[i+n] = 2*x[i+n] - prod*x[i];
		}

		for (i = k+1; i < n; ++i) {
			for (j = i; j < n; ++j) {
				a[i*n+j] -= x[i+n]*x[j]+ x[j+n]*x[i];
				if (i != j) a[j*n+i] = a[i*n+j];
			}
		}
	}

	gettimeofday(&end, NULL);

	start_us = start.tv_sec * 1000000 + start.tv_usec;
	end_us = end.tv_sec * 1000000 + end.tv_usec;

	*t1 = (double)(end_us - start_us) / 1000000.;

//	printf("\n\nSym: \n");
//	output(n, n, n, a);
//	printf("\n");

	gettimeofday(&start, NULL);

	while (m > 2) {
		while (fabs(a[(m-1)*n+m-2]) > eps*norm_A && fabs(a[(m-2)*n+(m-3)]) > eps*norm_A) {
//			if (iter > 50) break;
			s_k = a[(m-1)*n+(m-1)];
			for (i = 0; i < m; ++i) {a[i*n+i] -= s_k;}
	

//			printf("\nbefore iter %d\n", iter+1);
//			output(n, n, n, a);
//			printf("\n");

			xk = a[0*n + 0];
			zk = a[1*n + 0];   
			for (k = 0; k < m-1; ++k) {
				norm_a1 = sqrt(xk*xk+zk*zk);
		
				if (norm_a1 < eps) {c = 1; s = 0;}
				else {	
					c = xk/norm_a1;
					s = -zk/norm_a1;
				}

				if (fabs(s) < eps) s = 0;
				if (fabs(c) < eps) c = 0;

//				if (iter == 20)
//				printf("\nnorm_a1 = %e, cos = %e, sin = %e\n", norm_a1, cos, sin);	


			    /* новые диагонали */
			    d_new_k   =  c*c*a[k*n+k]- c*s*a[(k+1)*n + k] - c*s*a[(k+1)*n + k] + s*s*a[(k+1)*n + (k+1)];
			    d_new_k1  =  s*s*a[k*n + k] + 2*c*s*a[(k+1)*n + k] + c*c*a[(k+1)*n + (k+1)];
			    e_new_k = c*s*(a[k*n + k] - a[(k+1)*n+(k+1)]) + (c*c-s*s)*a[(k+1)*n + k];
				
			    a[k*n + k] = d_new_k;
			    a[(k+1)*n + k] = e_new_k;
			    a[k*n + (k+1)] = a[(k+1)*n+k];
			    a[(k+1)*n + (k+1)] = d_new_k1;

				if (k < m-2) {
			        xk = e_new_k;
			        zk = s * a[(k+2)*n + (k+1)];
			        a[(k+2)*n + (k+1)] = c* a[(k+2)*n + (k+1)];
			        a[(k+1)*n + (k+2)] = a[(k+2)*n + (k+1)];
			    }


/*
				a[k*n+k] = norm_a1;
				a[(k+1)*n+k] = 0;		
					
				for (j = k+1; j < min(k+3, m); ++j) {
					if (j == k+2) {
						a[(k+1)*n+j] = cos*a[(k+1)*n+j];
					} else { 
						tmp = cos*a[k*n+j] - sin*a[(k+1)*n+j];
						a[(k+1)*n+j] = sin*a[k*n+j] + cos*a[(k+1)*n+j];
						a[k*n+j] = tmp;	
					}
				}
			

				for (i = k; i < min(k+2, m); ++i) {
					tmp = cos*a[i*n+k] - sin*a[i*n+(k+1)];
					a[i*n+(k+1)] = sin*a[i*n+k] + cos*a[i*n+(k+1)];
					a[i*n+k] = tmp;
				}
				a[k*n+(k+1)] = a[(k+1)*n+k];

				if (k == m-1) {
					a[(m-1)*n+(m-1)] = cos*a[(m-1)*n+(m-1)];
				}
//				if (iter == 20) {
//				printf("\nk = %d\n", k);
//				output(n, n, n, a);
//				printf("\n");
//				}
//		*/
			}
	
			for (i = 0; i < m; ++i) {a[i*n+i] += s_k;}
			++iter;

//			printf("\n\n iter = %d; m = %d\n", iter, m);
//			output(n, n, n, a);
//			printf("\n");
		}
		if (!(fabs(a[(m-1)*n+m-2]) > eps*norm_A)) {
			res[m-1] = a[(m-1)*n+(m-1)];
			--m;
		} else {
			tmp = sqrt((a[(m-2)*n+(m-2)]-a[(m-1)*n+(m-1)])*(a[(m-2)*n+(m-2)]-a[(m-1)*n+(m-1)])+4*a[(m-2)*n+(m-1)]*a[(m-2)*n+(m-1)]);
			res[m-2] = (a[(m-2)*n+(m-2)] + a[(m-1)*n+(m-1)] + tmp)/2.;
			res[m-1] = (a[(m-2)*n+(m-2)] + a[(m-1)*n+(m-1)] - tmp)/2.;
			m -= 2;
		}
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
