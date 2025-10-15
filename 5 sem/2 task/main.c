#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "output.h"
#include "qr_3diagonal.h"

int main(int argc, char* argv[]) {
	int n = 0, r = 0, s = 0, flag = 0, res = 0, its = 0;
	char* filename = NULL; double* array = NULL, *result = NULL, *d = NULL;
	double res1 = 0, res2 = 0, t1 = 0, t2 = 0, eps = 0;
	
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

	flag = qr_3diagonal(n, array, result, eps, &t1, &t2);
	
	output(r, n, n, array);
	printf("\n");
	output(r, 1, n, d);
	printf("\n");
	res = input(s, filename, n, array);	
	if (!flag) {
		output(r, n, n, result);
		r1_r2(n, array, result, &res1, &res2);
	} else {
		r1 = -1; r2 = -1; t2 = 0;
	}
	printf ("%s : Residual1 = %e Residual2 = %e Iterations = %d \ Iterations1 = %d Elapsed1 = %.2f Elapsed2 = %.2f\n", argv[0], res1, res2, its, its / n, t1, t2);	
	free(array);
	free(result);
	free(d);
	return 0;
}
