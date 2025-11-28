#ifndef JORDAN_LINEAR
#define JORDAN_LINEAR

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

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

int jordan_linear(int, double*, double*, double*, double*, int*, ThreadArgs*, pthread_t*, int);


void r1_r2(int, double*, double*, double*, double*, double*, double*);

#endif
