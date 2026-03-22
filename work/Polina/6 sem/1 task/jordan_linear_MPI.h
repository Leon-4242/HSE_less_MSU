#ifndef JORDAN_LINEAR_MPI
#define JORDAN_LINEAR_MPI

#include <stdlib.h>
#include <math.h>
#include <mpi.h>

typedef struct {
	double value;
	int index;
} MAX;

int jordan_linear_mpi(int, double*, double*, double*, int, int, double*);
/*
int jordan_linear(int, double*, double*, double*, double*, int*, ThreadArgs*, pthread_t*, int);
*/

void r1_r2_mpi(int, double*, double*, double*, double*, double*, int, int);

#endif
