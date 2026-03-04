#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "input.h"
#include "output.h"
#include "eigen_algorithms.h"

int main(int argc, char* argv[]) {
	int n = 0, r = 0, s = 0, res = 0, its = 0, k_eigen = 0;
	char* filename = NULL; 
	double* array = NULL, *diag = NULL, *subdiag = NULL;
	double t1 = 0, t2 = 0, eps = 0, lambda_k = 0;
	
	clock_t start, end;
	
	//Initialization
	if (argc < 5 || argc > 6) {
		printf("Invalid number of arguments.\n");
		return -1;
	}
	n = strtol(argv[1], NULL, 10);
	r = strtol(argv[2], NULL, 10);
	eps = strtod(argv[3], NULL);
	s = strtol(argv[4], NULL, 10);
	
	if (s == 0 && argc == 5) {
		printf("Invalid number of arguments.\n");
		return -1;
	}

	if (s == 0) {filename  = argv[5];}
	
	array = (double*)malloc(n*n*sizeof(double));
	diag = (double*)malloc(n*sizeof(double));
    if (n > 1) {
        subdiag = (double*)malloc((n-1) * sizeof(double));
    } else {
        subdiag = NULL;
    }

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

	printf("\n Data: \n");
	output(r, n, n, array);

    start = clock();
    
    tridiagonalize_symmetric(n, array, diag, subdiag);
    
    end = clock();
    t1 = (double)(end - start) / CLOCKS_PER_SEC;

    k_eigen = (n / 2) + 1;
    if (k_eigen > n) k_eigen = n;
    if (k_eigen < 1) k_eigen = 1;
    
	start = clock();

    its = 0;
    lambda_k = bisection_kth_eigenvalue(n, diag, subdiag, k_eigen, eps, &its);
	
	end = clock();
    
	t2 = (double)(end - start) / CLOCKS_PER_SEC;
		
	printf("\n");
	
    printf("\nResult: %d-th eigenvalue = %.10e\n", k_eigen, lambda_k);
	
	printf ("%s : Iterations = %d \n Iterations1 = %d Elapsed1 = %.2f Elapsed2 = %.2f\n", argv[0], its, its / n, t1, t2);	
	free(array);
	free(diag);
	if (subdiag) free(subdiag);
	return 0;
}
