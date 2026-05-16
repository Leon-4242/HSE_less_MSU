#ifndef BARRIER
#define BARRIER

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct {
	pthread_mutex_t sem;
	sem_t sem_f;
	sem_t sem_l;
	int k;
	int curr;
} barrier_t;

void barrier_init(barrier_t* b, int k);
void barrier_wait(barrier_t* b);
void barrier_destroy(barrier_t* b);

#endif
