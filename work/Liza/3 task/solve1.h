#include <math.h>
#include <malloc.h>
#include <pthread.h>
#define EPS 1e-16

void* parallel_conversion(void*);
int solve(int n, double *a, double *b, int* m, int* tmp);
double norm_calc(double *a, int n);
