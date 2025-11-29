#ifndef REFLECTION_INVERSE
#define REFLECTION_INVERSE

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <mutex.h>

typedef struct {
	double* a;
	double* b;
	double* d;
	int n;
	double* s;

	int numThreads;
	int thread_id;
	pthread_barier_t* barier;
	pthread_mutex_t* mutex;
} ThreadArgs;

void* reflection_inverse_parallel(void*);

void r1_r2(int, double*, double*, double*, double*, double*);

double norm_ab_e(int, double*, double*);
#endif
