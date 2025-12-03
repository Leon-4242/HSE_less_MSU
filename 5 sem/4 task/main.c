#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "output.h"
#include "reflection_inverse_parallel.h"

int main(int argc, char* argv[]) {
	int i = 0, n = 0, p = 0, r = 0, s = 0, task = 0, flag = 0, res = 0;
	char* filename = NULL; double* array = NULL, *result = NULL, *d = NULL, *buff = NULL;
	double r1 = 0, r2 = 0, t1 = 0, t2 = 0, tmp = 0;
	ThreadArgs* args; pthread_t* threads; 
	//pthread_barrier_t barrier;
	barrier_t barrier; pthread_mutex_t sup; sem_t sup1, sup2;
	pthread_mutex_t mutex;

	struct timeval start, end;
    long long start_us, end_us;

	barrier.sem = sup;
	barrier.sem_f = sup1;
	barrier.sem_l = sup2;
	barrier.k = 0;
	barrier.curr = 0;

	task = 24;
	//Initialization
	if (argc < 5 || argc > 6) {
		printf("Invalid number of arguments.\n");
		return -1;
	}
	n = strtol(argv[1], NULL, 10);
	p = strtol(argv[2], NULL, 10);
	r = strtol(argv[3], NULL, 10);
	s = strtol(argv[4], NULL, 10);

	if (s == 0 && argc == 5) {
		printf("Invalid number of arguments.\n");
		return -1;
	}

	if (s == 0) {filename  = argv[5];}
	
	array = (double*)malloc(n*n*sizeof(double));

	res = input(s, filename, n, array);
	switch (res) {
		case -1:
			printf("Error during opening file %s\n", filename);
			free(array);
			return -1;
		case -2:
			printf("Wrong data in file %s\n", filename);
			free(array);
			return -1;
		case -3:
			printf("Wrong type of formule: %d\n", s);
			free(array);
			return -1;
	}

	result = (double*)malloc(n*n*sizeof(double));
	d = (double*)malloc(n*sizeof(double));
	threads = (pthread_t*)malloc(p*sizeof(pthread_t));
	args = (ThreadArgs*)malloc(p*sizeof(ThreadArgs));

	pthread_mutex_init(&mutex, NULL);
//	pthread_barrier_init(&barrier, NULL, p);
	barrier_init(&barrier, p);
	gettimeofday(&start, NULL);

	for (i = 0; i < p; ++i) {
		(args+i)->a = array;
		(args+i)->b = result; 
		(args+i)->d = d;
		(args+i)->n = n;

		(args+i)->numThreads = p;
		(args+i)->thread_id = i;
		(args+i)->barrier = &barrier;
		(args+i)->s = &tmp;
		(args+i)->mutex = &mutex;
		if (pthread_create(threads + i, NULL, reflection_inverse_parallel, args+i) != 0)
			return 1;
	}

    for (i = 0; i < p; ++i) {
        if (pthread_join(threads[i], (void**)&buff) != 0) {
            fprintf(stderr, "Failed to join thread");
            return 1;
        }

		if (buff != NULL) 
			flag = 1;
    }

    gettimeofday(&end, NULL);
 
    start_us = start.tv_sec * 1000000 + start.tv_usec;
    end_us = end.tv_sec * 1000000 + end.tv_usec;
 
    t1 = (double)(end_us - start_us)/1000000.;


	output(r, n, n, array);
	printf("\n");
	output(r, 1, n, d);
	printf("\n");
	res = input(s, filename, n, array);	
	if (!flag) {
		output(r, n, n, result);
		r1_r2(n, array, result, &r1, &r2, &t2);
	} else {
		r1 = -1; r2 = -1; t2 = 0;
	}
	printf ("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = %d\n", argv[0], task, r1, r2, t1, t2, s, n);
	
	pthread_mutex_destroy(&mutex);
//	pthread_barrier_destroy(&barrier);
	barrier_destroy(&barrier);

	free(array);
	free(result);
	free(d);
	free(threads);
	free(args);
	return 0;
}
