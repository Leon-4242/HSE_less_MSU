#ifndef JORDAN_LINEAR
#define JORDAN_LINEAR

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

typedef struct {
	double* a;
	double* b;
	double* res;
	int n;
	int* ind;

	double* s;
	int* k;

	int numThreads;
	int thread_id;
	pthread_barrier_t* barrier;
	pthread_mutex_t* mutex;
} ThreadArgs;

void* jordan_linear_parallel(void*);
/*
int jordan_linear(int, double*, double*, double*, double*, int*, ThreadArgs*, pthread_t*, int);
*/

void r1_r2(int, double*, double*, double*, double*, double*, double*);

#endif
