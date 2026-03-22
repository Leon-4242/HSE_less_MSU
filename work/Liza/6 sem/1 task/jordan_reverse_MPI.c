#include <stdio.h>
#include "output.h"
#include "jordan_reverse_MPI.h"

int jordan_reverse_mpi(int n, double *local_a, double *local_rev, int rank, int size, double* buffer, int* ind) {
	int local_n = 0, k = 0, start = 0, local_n_max = 0, j = 0, i = 0;
	double local_max = 0, tmp = 0, buff = 0;
	MAX global;
	
	local_n = n/size;
	if (rank < n%size)
		++local_n;
	
	for (k = 0; k < n; ++k) {
		if (rank == k%size) {
			local_n_max = k;
			local_max = fabs(local_a[(k-rank)/size * n + ind[k]]);
			for(j = k+1; j < n; ++j) {
				if (local_max < fabs(local_a[(k-rank)/size * n + ind[j]])) {
					local_n_max = j;
					local_max = fabs(local_a[(k-rank)/size * n + ind[j]]);
				}
			}

			global.value = local_max;
			global.index = local_n_max;
		}

		MPI_Bcast(&global, 1, MPI_DOUBLE_INT, k%size, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);
		if (global.value < 1e-15)
			return -1;

		if (k != global.index) {
			buff = ind[k];
			ind[k] = ind[global.index];
			ind[global.index] = buff;
		}

		MPI_Barrier(MPI_COMM_WORLD);

		if (rank == k%size) {
			for (j = k+1; j < n; ++j) {
				local_a[(k-rank)/size * n + ind[j]] /= local_a[(k-rank)/size * n + ind[k]];
				buffer[j] = local_a[(k-rank)/size * n + ind[j]];
			}
			for (j = 0; j < n; ++j) {
				local_rev[(k-rank)/size * n + j] /= local_a[(k-rank)/size * n + ind[k]];
				buffer[j+n] = local_rev[(k-rank)/size * n + j];
			}
			local_a[(k-rank)/size * n + ind[k]] = 1;
			buffer[k] = 1.;
		}

		MPI_Bcast(buffer, 2*n, MPI_DOUBLE, k%size, MPI_COMM_WORLD);

/*		if (rank == 0) {
			printf("\nbuffer\n");
			for (i = 0; i < n; ++i){
				printf("%lf ", buffer[i]);
			}
			printf("\n");
		}
*/
		for (i = 0; i < local_n; ++i) {
			if (!(rank == k%size && (k-rank)/size == i)) {
				for (j = k+1; j < n; ++j) {
					local_a[i*n+ind[j]] -= local_a[i*n+ind[k]]*buffer[j];
				}

				for (j = 0; j < n; ++j) {
					local_rev[i*n+j] -= local_a[i*n+ind[k]]*buffer[n+j];
				}
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	for (i = 0; i < n; ++i) {
		ind[ind[i]+n] = i;
	}
	for (i = 0; i < n; ++i) {
		ind[i] = ind[i+n];
	}

	for (i = 0; i < n; ++i) {
		if (i%size == ind[i]%size) {
			if (i%size == rank) {
				for (j = 0; j < n; ++j) {
					local_a[(ind[i]-rank)/size * n + j] = local_rev[(i-rank)/size * n + j];
				}
			}
		} else {
			if (rank == i%size) {
				MPI_Send(local_rev + (i-rank)/size*n, n, MPI_DOUBLE, ind[i]%size, 0, MPI_COMM_WORLD);
			} else if (rank == ind[i]%size) {
				MPI_Recv(local_a + (ind[i]-rank)/size * n, n, MPI_DOUBLE, i%size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}

	for (i = 0; i < local_n; ++i) {
		for (j = 0; j < n; ++j) {
			local_rev[i*n+j] = local_a[i*n+j];
		}
	}

	return 0;
}

void r_mpi(int n, double* local_a, double* local_b, double* r, int rank, int size, double* buffer) {
	double norm_ab_e = 0, sum = 0, max = 0;
	int i = 0, k = 0, new_rank = 0, tmp_n = 0, local_n = 0, j = 0, t = 0;
	MPI_Datatype local_col_t;

	MPI_Type_vector(n/size+1, 1, n, MPI_DOUBLE, &local_col_t);
	MPI_Type_commit(&local_col_t);

	norm_ab_e = -1;

	local_n = n/size;
	if (rank < n%size) 
		++local_n;

	for (t = 0; t < n; ++t) {
		sum = 0;
		for (i = 0; i < local_n; ++i) {
			if (i*size+rank == t)
				buffer[i] = -1;
			else
				buffer[i] = 0;
		}

		for (k = 0; k < size; ++k) {
			new_rank = (rank+k)%size;
			tmp_n = n/size;
			if (new_rank < n%size) 
				++tmp_n;

			for (i = 0; i < local_n; ++i) {
				for (j = 0; j < tmp_n; ++j) {
					buffer[i] += local_a[i*n+(j*size+new_rank)]*local_b[j*n+t];
				}
			}	
		
			MPI_Sendrecv_replace(local_b+t, 1 , local_col_t, (rank - 1 + size) % size, 0, (rank+1)%size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		for (i = 0; i < local_n; ++i) {
			sum += fabs(buffer[i]);
		}
		MPI_Reduce(&sum, &max, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0 && norm_ab_e < max) {
			norm_ab_e = max;
		}
	}
	MPI_Type_free(&local_col_t);
/*
	for (t = 0; t < n; ++t) {
		sum = 0;
		for (i = 0; i < local_n; ++i) {
			if (i*size+rank == t)
				buffer[i] = -1;
			else
				buffer[i] = 0;
		}

		for (k = 0; k < size; ++k) {
			new_rank = (rank+k)%size;
			tmp_n = n/size;
			if (new_rank < n%size) 
				++tmp_n;

			for (i = 0; i < local_n; ++i) {
				for (j = 0; j < tmp_n; ++j) {
					buffer[i] += local_a[i*n+(j*size+new_rank)]*local_b[j*n+t];
				}
			}	
		
			MPI_Sendrecv_replace(local_b, ((n/size)+1)*n, MPI_DOUBLE, (rank - 1 + size) % size, 0, (rank+1)%size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		for (i = 0; i < local_n; ++i) {
			sum += fabs(buffer[i]);
		}
		MPI_Reduce(&sum, &max, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0 && norm_ab_e < max) {
			norm_ab_e = max;
		}
	}
*/
	if (rank == 0)
		*r = norm_ab_e;
} 

void r1_r2_mpi(int n, double* local_a, double* local_rev, double* r1, double* r2, int rank, int size, double* buffer) {
	if (n > 11000) {
		*r1 = 0;
		*r2 = 0;
		return;
	}

	r_mpi(n, local_a, local_rev, r1, rank, size, buffer);
	r_mpi(n, local_rev, local_a, r2, rank, size, buffer);
}
