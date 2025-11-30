#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "output.h"
#include "jordan_linear_parallel.h"

int main(int argc, char* argv[]) {
	int i = 0, n = 0, p = 0, r = 0, s = 0, task = 0, flag = 0, res = 0, k = 0, l = 0;
	char* filename = NULL; double* array = NULL, *result = NULL, *b = NULL, *buff = NULL; int *indi = NULL;
	double r1 = 0, r2 = 0, t1 = 0, t2 = 0, sum = 0, tmp = 0;
	ThreadArgs* args; pthread_t* threads; pthread_barrier_t barrier;
	pthread_mutex_t mutex;
	struct timeval start, end;
    long long start_us, end_us;

	task = 15;
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

	b = (double*)malloc(n*sizeof(double));
	for (i = 0; i < n; ++i) {
		sum = 0;
		for (k = 0; k < n; k += 2) {
			sum += array[i*n+k];
		}
		b[i] = sum;
	}

	indi = (int*)malloc(n*sizeof(int));
	for (i = 0; i < n; ++i) {indi[i] = i;}

	output(r, n, n, array);
	printf("\n\n");

	result = (double*)malloc(n*sizeof(double));
    args = (ThreadArgs *)malloc(p*sizeof(ThreadArgs));
    threads = (pthread_t *)malloc(p*sizeof(pthread_t));
	
	pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&barrier, NULL, p);
	gettimeofday(&start, NULL);
	
	for (i = 0; i < p; ++i) {
		(args+i)->a = array;
		(args+i)->b = b; 
		(args+i)->res = result;
		(args+i)->n = n;
		(args+i)->ind = indi;

		(args+i)->numThreads = p;
		(args+i)->thread_id = i;
		(args+i)->barrier = &barrier;
		(args+i)->s = &tmp;
		(args+i)->k = &l;
		(args+i)->mutex = &mutex;
		if (pthread_create(threads + i, NULL, jordan_linear_parallel, args+i) != 0)
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

	res = input(s, filename, n, array);	
	
	for (i = 0; i < n; ++i) {
		sum = 0;
		for (k = 0; k < n; k += 2) {
			sum += array[i*n+k];
		}
		b[i] = sum;
	}
	
	if (!flag) {
		output(r, 1, n, b);
		printf("\n\n");
		output(r, 1, n, result);
		r1_r2(n, array, result, b, &r1, &r2, &t2);
	} else {
		r1 = -1; r2 = -1; t2 = 0;
	}
	printf ("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = %d\n", argv[0], task, r1, r2, t1, t2, s, n);
	
	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);
	
	free(array);
	free(result);
	free(b);
	free(indi);
	free(args);
	free(threads);

	return 0;
}
