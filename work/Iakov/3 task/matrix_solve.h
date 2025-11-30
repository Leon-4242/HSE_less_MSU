#ifndef MATRIX_SOLVE_H
#define MATRIX_SOLVE_H

#include <pthread.h>
#include <semaphore.h>

typedef struct {
	double* a;
	double* b;
	double* res;
	int* indi;
	int* indj;
	int n;

	double* s;
	int* i;
	int* j;

	int numThreads;
	int thread_id;
	pthread_barrier_t* barrier;
	pthread_mutex_t* mutex;
} ThreadArgs;

void* solve (void*);
/*
void m_swap(double *a, double *b);
double m_min(double a, double b);
*/
double comp_1(double *A, double *b, double *x, int n);
double comp_2(double *x, int n);
/*
void swap_col(double *A, int *S, int n, int i, int j);
void swap_rows(double *A, double *b, int n, int i, int j);
int zeroing(double *A, double *b, int n, int k);
void find_max(double *A, double *b, int *S, int n, int k);
int diagonal(double *A, double *b, int *S, int n);
void solve(double *A, double *b, int *S, double *x, int n);
*/
void m_print(double *A, int l, int n, int r);

#endif
