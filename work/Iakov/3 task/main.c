#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "fill_matrix.h"
#include "matrix_solve.h"

int main(int argc, const char **argv)
{
    int n = 0, p = 0, s = 0, r = 0 , i = 0, j = 0, k = 0, l = 0, flag = 0, res = 0;
    double r1 = 0, r2 = 0, t1 = 0, t2 = 0, bb = 0;
    const char *name_file = NULL;
    double *A = NULL, *x = NULL, *b = NULL, *buff = NULL;
    int *indi = NULL, *indj = NULL;
	struct timeval start, end;
    long long start_us, end_us;

	ThreadArgs* args;
	pthread_t* threads;
	pthread_barrier_t barrier;
	pthread_mutex_t mutex;

    if(argc != 6 && argc != 5)
    {
        printf("Error 1: Invalid command line request\n");
        return -1;
    }
    name_file = argv[5];
    sscanf(argv[1],"%d",&n);
    sscanf(argv[2],"%d",&p);
    sscanf(argv[3],"%d",&r);
	sscanf(argv[4],"%d",&s);
    A = (double*)malloc(n*n*sizeof(double));

	res  = fill(A, name_file, n, s);
    if(res == -2)
	{
		free(A);
		printf("Error 2: The file could not be opened.\n");
		return -2;
	}
	else if (res == -3) 
	{
        free(A);
        printf("Error 3: The file could not be read.\n");
        return -3;
    } 
	else if (res == -4) 
	{
		free(A);
		printf("Error 4 The wrong number of formule.\n");
	}

    x = (double*)malloc(n*sizeof(double));
    b = (double*)malloc(n*sizeof(double));

    indi = (int*)malloc(n*sizeof(int));
	indj = (int*)malloc(n*sizeof(int));

	args = (ThreadArgs*)malloc(p*sizeof(ThreadArgs));
	threads = (pthread_t*)malloc(p*sizeof(pthread_t));


    for(i = 0; i < n; i++)
    {
        b[i] = 0;
        for(j = 0; j < (n - 1)/2; j++)
        {
            b[i] += A[i*n + 2*j + 1];
        }
        //printf("%e\n",b[i]);
    }

	
    for(i = 0; i < n; i++)
    {
        x[i] = 0;
    }
    
	for(i = 0; i < n; i++)
    {
        indi[i] = i;
		indj[i] = i;
	}

	pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&barrier, NULL, p);
	gettimeofday(&start, NULL);
    // for(i = 0; i < n; i++)
    // {
    //printf("%lf ",b[i]);                                                              //print for b[]
    // }
    // printf("\n");
	
    for (i = 0; i < p; ++i) {
		(args+i)->a = A;
		(args+i)->b = b;
		(args+i)->res = x;
		(args+i)->n = n;
		(args+i)->indi = indi;
		(args+i)->indj = indj;

		(args+i)->numThreads = p;
		(args+i)->thread_id = i;
		(args+i)->barrier = &barrier;
		(args+i)->mutex = &mutex;

		(args+i)->s = &bb;
		(args+i)->i = &k;
		(args+i)->j = &l;
		if (pthread_create(threads + i, NULL, solve, args+i) != 0)
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

	res  = fill(A, name_file, n, s);
    if(res == -2)
	{
		free(A);
		printf("Error 2: The file could not be opened.\n");
		return -2;
	}
	else if (res == -3) 
	{
        free(A);
        printf("Errёor 3: The file could not be read.\n");
        return -3;
    } 
	else if (res == -4) 
	{
		free(A);
		printf("Error 4 The wrong number of formule.\n");
	}
	
 	for(i = 0; i < n; i++)
    {
        b[i] = 0;
        for(j = 0; j < (n - 1)/2; j++)
        {
            b[i] += A[i*n + 2*j + 1];
        }
        //printf("%e\n",b[i]);
    }

	
    // double tmp = 0;
    // printf("\n");
    // for(i = 0; i < n; i ++)
    // {
    //     tmp = 0;
    //     for(j = 0; j < n; j++)
    //     {
    //         tmp += A[i*n + j]*x[j];                                                          //print for Ax-b
    //     }
    //     b[i] = tmp - b[i];
    //     printf("%lf ",b[i]);
    //     printf("\n");
    // }
    // for(i = 0; i < n; i++)
    // {
    //     printf("%lf ",x[i]);
    // }
	m_print(A, n, n, r);
	printf("\n\n");
    m_print(x,1,n,r);
	if (flag) {
		r1 = -1;
		r2 = -1;
		t1 = 0;
		t2 = 0;
	} else {
		t2 = clock();
		r1 = comp_1(A,b,x,n);
		r2 = comp_2(x,n);
		t2 = (clock() - t2) / CLOCKS_PER_SEC;
	}
	printf("%s : Task = 17, Res1 = %e, Res2 = %e, T1 = %.2lf, T2 = %.2lf, S = %d, N = %d\n",argv[0], r1, r2, t1, t2, s,n);
    
    
	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);
    //printf("%.2lf\n", t);
    free(A);
    free(x);
    free(b);
    free(indi);
	free(indj);
	free(threads);
	free(args);
    return 0;
}

