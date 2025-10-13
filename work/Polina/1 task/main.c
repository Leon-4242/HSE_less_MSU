#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "output.h"
#include "jordan_linear.h"

int main(int argc, char* argv[]) {
	int n = 0, r = 0, s = 0, task = 0, flag = 0, res = 0, i = 0, k = 0;
	char* filename = NULL; double* array = NULL, *result = NULL, *b = NULL; int *indi = NULL;
	double r1 = 0, r2 = 0, t1 = 0, t2 = 0, sum = 0;
	
	task = 15;
	//Initialization
	if (argc < 4 || argc > 5) {
		printf("Invalid number of arguments.\n");
		return -1;
	}
	n = strtol(argv[1], NULL, 10);
	r = strtol(argv[2], NULL, 10);
	s = strtol(argv[3], NULL, 10);

	if (s == 0 && argc == 4) {
		printf("Invalid number of arguments.\n");
		return -1;
	}

	if (s == 0) {filename  = argv[4];}
	
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
	
	flag = jordan_linear(n, array, result, b, &t1, indi);
	/*
	...
	...
	...
	*/
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
	
	free(array);
	free(result);
	free(b);
	free(indi);
	return 0;
}
