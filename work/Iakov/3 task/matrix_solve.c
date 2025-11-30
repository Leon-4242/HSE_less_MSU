#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "matrix_solve.h"

void* solve (void* arg) {
	ThreadArgs *args; 
	int k = 0, start = 0, local_i_max = 0, local_j_max = 0, buff = 0, j = 0, i = 0;
	double local_max = 0;
	args = (ThreadArgs*)arg;

	
	for (k = 0; k < args->n; ++k) {
		if (args->thread_id == 0) *args->s = 0;
		pthread_barrier_wait(args->barrier);

		local_i_max = k; 
		local_j_max = k;
		local_max = fabs(args->a[args->indi[k]*args->n + args->indj[k]]);
		
		start = args->thread_id;
		while (start < k+1) {start += args->numThreads;}
		for (i = start; i < args->n; i += args->numThreads) {
			for (j = k+1; j < args->n; ++j) {
				if (local_max < fabs(args->a[args->indi[i]*args->n + args->indj[j]])) {
					local_i_max = i; 
					local_j_max = j;
					local_max = fabs(args->a[args->indi[i]*args->n + args->indj[j]]);
				}
			}
		}

		pthread_mutex_lock(args->mutex);
		if (*args->s < local_max) {
			*args->s = local_max;
			*args->i = local_i_max;	
			*args->j = local_j_max;
		}
		pthread_mutex_unlock(args->mutex);

		pthread_barrier_wait(args->barrier);

		if (args->thread_id == 0) {
			if (k != *args->i) {
				buff = args->indi[k];
				args->indi[k] = args->indi[*args->i];
				args->indi[*args->i] = buff;
			}
			if (k != *args->j) {
				buff = args->indj[k];
				args->indj[k] = args->indj[*args->j];
				args->indj[*args->j] = buff;
			}
		}

		pthread_barrier_wait(args->barrier);

		if (*args->s < 1e-15) return (void*)args->a;
		
		start = args->thread_id;
		while (start < k+1) {start += args->numThreads;}
		for (j = start; j < args->n; j += args->numThreads) {
			args->a[args->indi[k]*args->n + args->indj[j]] /= args->a[args->indi[k]*args->n+args->indj[k]];
		}

		pthread_barrier_wait(args->barrier);
		if (args->thread_id == 0) {
			args->b[args->indi[k]] /= args->a[args->indi[k]*args->n + args->indj[k]];
			args->a[args->indi[k]*args->n + args->indj[k]] = 1.;
		}

		pthread_barrier_wait(args->barrier);
		
		for (i = args->thread_id; i < args->n; i += args->numThreads) {
			if (i != k) {
				for (j = k+1; j < args->n; ++j) {
					args->a[args->indi[i]*args->n+args->indj[j]] -= 
						args->a[args->indi[i]*args->n+args->indj[k]]*
						args->a[args->indi[k]*args->n+args->indj[j]]; 
				}
			}
		}

		for (i = args->thread_id; i < args->n; i += args->numThreads) {
			if (i != k) {
				args->b[args->indi[i]] -= 
					args->a[args->indi[i]*args->n+args->indj[k]]*
					args->b[args->indi[k]]; 
			}
		}
		pthread_barrier_wait(args->barrier);
	}
	
    for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->res[i] = args->b[args->indi[i]];
	}

	pthread_barrier_wait(args->barrier);

    for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->b[i] = args->res[i];
	}

	pthread_barrier_wait(args->barrier);

    for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->indi[args->indj[i]] = i;
	}

	pthread_barrier_wait(args->barrier);

    for (i = args->thread_id; i < args->n; i += args->numThreads) {
		args->res[i] = args->b[args->indi[i]];
	}

	
	return NULL;

}


double comp_1(double *A, double *b, double *x, int n)
{
    double tmp;
    double tmp1 = 0;
    double tmp2 = 0;
    int i,j;
    for(i = 0; i < n; i++)
    {
        tmp2 += fabs(b[i]);
        tmp = 0;
        for(j = 0; j < n; j++)
        {
            tmp += A[i*n + j]*x[j];
        }
        tmp1 += fabs(tmp - b[i]);
    }
    return tmp1/tmp2;
}

double comp_2(double *x, int n)
{
    int i;
    double tmp1,tmp2;
    int y;
    tmp1 = 0;
    tmp2 = 0;
    y = 0;
    for(i = 0; i < n - 1; i++)
    {
        y = i%2;
        tmp1 += fabs(x[i] - y);
        tmp2 += fabs(y);
    }
    return tmp1/tmp2;
}

double m_min(double a, double b)
{
    if(a < b) return a;
    else return b;
}
 
void m_print(double *A, int l, int n, int r)
{
    int i,j,m1,m2;
    m1 = m_min(l,r);
    m2 = m_min(n,r);
    for(i = 0; i < m1; i++)
    {
        for(j = 0; j < m2; j++)
        {
            printf("%e ",A[i*n + j]);
        }
        printf("\n");
    }
}
/*
void m_swap(double *a, double *b)
{
    double tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void solve(double *A, double *b, int *S, double *x, int n)
{
    int i, j, next;
    for(i = 0; i < n; i++)
    {
        x[i] = b[i]/A[i*n + i];
    } 
    for(i = 0; i < n; i++)
    {
        if (S[i] >= 0) 
        {
            j = i;
            while (S[j] >= 0) 
            {
                next = S[j];
                m_swap(&x[i], &x[next]);
                S[j] = -1; 
                j = next;
                if (j == i) break;
            }
        }
    }
}

int diagonal(double *A, double *b, int *S, int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        find_max(A,b,S,n,i);
        if(zeroing(A,b,n,i) != 0)
        {
               return -4;
        }
    }
    return 0;
}

void swap_col(double *A, int *S, int n, int i, int j)
{
    double tmp;
    int k;
    for(k = 0; k < n; k++)
    {
        tmp = A[k*n + i];
        A[k*n + i] = A[k*n + j];
        A[k*n + j] = tmp;
    }
    k = S[i];
    S[i] = S[j];
    S[j] = k;
}
void swap_rows(double *A, double *b, int n, int i, int j)
{
    double tmp;
    int k;
    for(k = 0; k < n; k++)
    {
        tmp = A[i*n + k];
        A[i*n + k] = A[j*n + k];
        A[j*n + k] = tmp;
    }
    tmp = b[i];
    b[i] = b[j];
    b[j] = tmp;
}

int zeroing(double *A, double *b, int n, int k)
{
    int i,j;
    double multiplier;
    double nrm;
    nrm = 0;
    for(i = 0; i < n*n; i++)
    {
        if(fabs(A[i]) > nrm) nrm = A[i];
    }
    if(fabs(A[k*n + k]) < nrm*DBL_EPSILON)
    {
        return -4;
    }
    for(i = 0; i < n; i++)
    {
        if(i != k)
        {
            multiplier = A[i*n + k] / A[k*n + k];
            b[i] = b[i] - b[k]*multiplier;
        }
        for(j = k; j < n; j++)
        {
            if(i!= k)
            {
                A[i*n + j] -= A[k*n + j]*multiplier;
            }
        }
    }
    return 0;
}

void find_max(double *A, double *b, int *S, int n, int k)
{
    int i,j;
    int max_i,max_j;
    max_i = k;
    max_j = k;
    for(i = k; i < n; i++)
    {
        for(j = k; j < n; j++)
        {
            if(fabs(A[i*n + j]) > fabs(A[max_i*n + max_j]))
            {
                max_i = i;
                max_j = j;
            }
        }
    }
    swap_rows(A,b,n,k,max_i);
    swap_col(A,S,n,k,max_j);
}
*/
