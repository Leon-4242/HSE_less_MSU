#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "matrix_io.h"
#include "solve1.h"

int main(int argc, char* argv[]) {
    int n, r, s, p, i = 0, l = 0;
    int ret, res = 0;
    double *a, *b, bb = 0, *buff;
	int *m, *tmp;
    double t1, t2 = 0, r1, r2;
    char *name = 0;
	pthread_mutex_t mutex;
	pthread_barrier_t barrier;
	ThreadArgs* args; 
	pthread_t* threads;
	struct timeval start, end;
    long long start_us, end_us;

    if (!((argc == 6 || argc == 5) && sscanf(argv[1], "%d", &n) == 1 &&
    sscanf(argv[2], "%d", &p) == 1 && sscanf(argv[3], "%d", &r) == 1 && sscanf(argv[4], "%d", &s) && ((argc == 5 && s > 0) || (argc == 6 && s == 0)))) {
        printf("Usage %s : n p r s [name]\n", argv[0]);
        return 1;
    }
    if (s == 0) {
        name = argv[5];
    }
    if (!(a = (double *)malloc(n*n*sizeof(double)))) {
        printf("Not enough memory!\n");
        return 2;
    }
    if (name) {
        ret = read_matrix(a, name, n, n);
    }
    else {
        ret = init_matrix(a, s, n, n);
    }
    if (ret != SUCCESS) {
        switch (ret) {
            case ERROR_OPEN:
                printf("Cannot open %s\n", name);
                break;
            case ERROR_READ:
                printf("Cannot read %s\n", name);
                break;
            case ERROR_INIT:
                printf("Cannot initialize a matrix with formula %d\n", s);
                break;
            default:
                printf("Unknown error %d\n", ret);
        }
        free(a);
        return 3;
    }
    if (!(b = (double *)malloc(n*n*sizeof(double)))) {
        printf("Not enough memory!\n");
        free(a);
        return 2;
    }
    if (!(m = (int *)malloc(n*sizeof(int)))) {
        printf("Not enough memory!\n");
        free(a);
        free(b);
        return 2;
    }
    if (!(tmp = (int *)malloc(n*sizeof(int)))) {
        printf("Not enough memory!\n");
        free(a);
        free(b);
		free(m);
        return 2;
    }
    if (!(args = (ThreadArgs *)malloc(p*sizeof(ThreadArgs)))) {
        printf("Not enough memory!\n");
        free(a);
        free(b);
		free(m);
		free(tmp);
        return 2;
    }
    if (!(threads = (pthread_t *)malloc(p*sizeof(pthread_t)))) {
        printf("Not enough memory!\n");
        free(a);
        free(b);
		free(m);
		free(tmp);
		free(args);
        return 2;
    }

	
    printf("Initial matrix:\n");
    print_matrix(a, n, n, r);
    
	pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&barrier, NULL, p);
	gettimeofday(&start, NULL);

    for (i = 0; i < p; ++i) {
		(args+i)->a = a;
		(args+i)->b = b; 
		(args+i)->n = n;
		(args+i)->ind = m;
		(args+i)->buff = tmp;

		(args+i)->numThreads = p;
		(args+i)->thread_id = i;
		(args+i)->barrier = &barrier;

		(args+i)->s = &bb;
		(args+i)->k = &l;
		(args+i)->mutex = &mutex;
		if (pthread_create(threads + i, NULL, solve, args+i) != 0)
			return 1;
	}
	
    for (i = 0; i < p; ++i) {
        if (pthread_join(threads[i], (void**)&buff) != 0) {
            fprintf(stderr, "Failed to join thread");
            return 1;
        }

		if (buff != NULL) 
			res = 1;
	}
	
	gettimeofday(&end, NULL);
 
    start_us = start.tv_sec * 1000000 + start.tv_usec;
    end_us = end.tv_sec * 1000000 + end.tv_usec;
 
    t1 = (double)(end_us - start_us)/1000000.;
	
    if (name) {
        ret = read_matrix(a, name, n, n);
    }
    else {
        ret = init_matrix(a, s, n, n);
    }
    if (ret != SUCCESS) {
        switch (ret) {
            case ERROR_OPEN:
                printf("Cannot open %s\n", name);
                break;
            case ERROR_READ:
                printf("Cannot read %s\n", name);
                break;
            case ERROR_INIT:
                printf("Cannot initialize a matrix with formula %d\n", s);
                break;
            default:
                printf("Unknown error %d\n", ret);
        }
        free(a);
        free(b);
        free(m);
		free(tmp);
		free(args);
		free(threads);

        return 3;
    }
    if (n > 11000) {
    	r1 = (r2 = 0);
    }
    else {
        t2 = clock();
    	calc_r1_r2(a, b, n, &r1);
    	calc_r1_r2(b, a, n, &r2);
        t2 = (clock() - t2) / CLOCKS_PER_SEC;
    }
    if (res == 0) {
    	printf("Result matrix:\n");
    	print_matrix(b, n, n, r);
    	printf("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = %d\n", argv[0], 19, r1, r2, t1, t2, s, n);
    }
    else {
    	printf("%s : Task = %d Res1 = %d Res2 = %d T1 = %.2f T2 = %.2f S = %d N = %d\n", argv[0], 19, -1, -1, t1, t2, s, n);
    }
    
	free(a);
    free(b);
    free(m);
	free(tmp);
	free(args);
	free(threads);

    return 0;
}
