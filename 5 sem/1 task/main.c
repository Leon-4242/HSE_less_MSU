#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "output.h"
#include "reflection_inverse.h"

int main(int argc, char* argv[]) {
	int n = 0, r = 0, s = 0, task = 0, flag = 0, res = 0;
	char* filename = NULL; double* array = NULL, *result = NULL, *d = NULL;
	double r1 = 0, r2 = 0, t1 = 0, t2 = 0;
	
	task = 24;
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

	result = (double*)malloc(n*n*sizeof(double));
	d = (double*)malloc(n*sizeof(double));
	flag = reflection_inverse(n, array, result, &t1, d);
	
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
	
	free(array);
	free(result);
	free(d);
	return 0;
}
