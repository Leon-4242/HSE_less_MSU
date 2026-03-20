#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "input.h"
#include "output.h"
#include "jordan_linear_MPI.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

	int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	int local_n = 0, n = 0, r = 0, s = 0, task = 0, flag = 0, i = 0, k = 0;
	char *filename = NULL; 
	double *local_array = NULL, *local_result = NULL, *local_b = NULL, *buffer = NULL; 
	double r1 = 0, r2 = 0, t1 = 0, t2 = 0;
	
	task = 15;
	//Initialization
	if (argc < 4 || argc > 5) {
		printf("Invalid number of arguments.\n");
		MPI_Finalize();
		return -1;
	}
	n = strtol(argv[1], NULL, 10);
	r = strtol(argv[2], NULL, 10);
	s = strtol(argv[3], NULL, 10);

	if (s == 0 && argc == 4) {
		printf("Invalid number of arguments.\n");
		MPI_Finalize();
		return -1;
	}

	local_n = n/size;
	if (rank < n%size) ++local_n;
	
	local_array = (double*)malloc(local_n*n*sizeof(double));

	if (rank == 0 && s == 0) {
		filename  = argv[4]; 
	}
	buffer = (double*)malloc((n+1)*sizeof(double));


	flag = input_mpi(s, filename, n, local_array, buffer, rank, size);
	switch (flag) {
		case -1:
			if(rank == 0) printf("Error during opening file %s\n", filename);
			free(local_array);
			free(buffer);
			MPI_Finalize();
			return -1;
		case -2:
			if (rank == 0) printf("Wrong data in file %s\n", filename);
			free(local_array);
			free(buffer);
			MPI_Finalize();
			return -1;
		case -3:
			if (rank == 0) printf("Wrong type of formule: %d\n", s);
			free(local_array);
			free(buffer);
			MPI_Finalize();
			return -1;
	}
	
	local_b = (double*)malloc((n/size + 1)*sizeof(double));
	
	for (i = 0; i < local_n; ++i) {
		local_b[i] = 0;
		for (k = 0; k < n; k += 2) {
			local_b[i] += local_array[i*n+k];
		}
	}

	local_result = (double*)malloc(local_n*sizeof(double));

	MPI_Barrier(MPI_COMM_WORLD);
	t1 = MPI_Wtime();

	//TODO: function
	flag = jordan_linear_mpi(n, local_array, local_result, local_b, rank, size, buffer);
	
	MPI_Barrier(MPI_COMM_WORLD);
	t1 = MPI_Wtime()-t1;

	if (flag) {	
		r1 = -1; r2 = -1; t2 = 0;
	} else {
		flag = input_mpi(s, filename, n, local_array, buffer, rank, size);
		for (i = 0; i < local_n; ++i) {
			local_b[i] = 0;
			for (k = 0; k < n; k += 2) {
				local_b[i] += local_array[i*n+k];
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);
		t2 = MPI_Wtime();

		r1_r2_mpi(n, local_array, local_result, local_b, &r1, &r2, rank, size);
	
		MPI_Barrier(MPI_COMM_WORLD);
		t2 = MPI_Wtime()-t2;
	}
	
	if (rank == 0) {
		printf ("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = %d\n", argv[0], task, r1, r2, t1, t2, s, n);
	}
	
	free(local_array);
	free(local_result);
	free(local_b);
	free(buffer);

	MPI_Finalize();
	return 0;
}
