#ifndef JORDAN_LINEAR
#define JORDAN_LINEAR

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>


void* parallel_conversion(void*);

int jordan_linear(int, double*, double*, double*, double*, int*);

void r1_r2(int, double*, double*, double*, double*, double*, double*);

#endif
