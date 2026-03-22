#include <math.h>
#include <malloc.h>
#include <pthread.h>
#define EPS 1e-16

typedef struct {
	double* a;
	double* b;
	int n;
	int* ind;
	int* buff;

	double* s;
	int* k;

	int numThreads;
	int thread_id;
	pthread_barrier_t* barrier;
	pthread_mutex_t* mutex;
} ThreadArgs;

void* solve(void*);
//int solve(int n, double *a, double *b, int* m, int* tmp, ThreadArgs* args, pthread_t* threads, int p);
double norm_calc(double *a, int n);
