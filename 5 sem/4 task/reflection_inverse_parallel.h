#ifndef REFLECTION_INVERSE
#define REFLECTION_INVERSE

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

int reflection_inverse(int, double*, double*, double*, double*);

void r1_r2(int, double*, double*, double*, double*, double*);

double norm_ab_e(int, double*, double*);
#endif
