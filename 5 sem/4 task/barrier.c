#include "barrier.h"

void barrier_init(barrier_t* b, int k) {
	pthread_mutex_init(&b->sem, NULL);
	sem_init(&b->sem_f, 0, 0);
	sem_init(&b->sem_l, 0, 1);
	
	b->curr = 0;
	b->k = k;
}

void barrier_wait(barrier_t* b) {
	int i = 0;

	pthread_mutex_lock(&b->sem);
	++b->curr;
	if (b->curr == b->k) {
		sem_wait(&b->sem_l);
		for (i = 0; i < b->k; ++i) sem_post(&b->sem_f);
	}

	pthread_mutex_unlock(&b->sem);

	sem_wait(&b->sem_f);

	pthread_mutex_lock(&b->sem);

	--b->curr;

	if (b->curr == 0) {
		for (i = 0; i < b->k + 1; ++i) sem_post(&b->sem_l);
	}

	pthread_mutex_unlock(&b->sem);

	sem_wait(&b->sem_l);
}

void barrier_destroy(barrier_t* b) {
    sem_destroy(&b->sem_f);
	sem_destroy(&b->sem_l);
    pthread_mutex_destroy(&b->sem);
}

