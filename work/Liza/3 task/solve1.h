#include <math.h>
#include <malloc.h>
#include <pthread.h>
#define EPS 1e-16

typedef struct {
	double* a;
	double* b;
	int n;
	int* ind;

	int start;
	int end;
	int k;
} ThreadArgs;

void* parallel_conversion(void*);
int solve(int n, double *a, double *b, int* m, int* tmp, ThreadArgs* args, pthread_t* threads, int p);
double norm_calc(double *a, int n);
