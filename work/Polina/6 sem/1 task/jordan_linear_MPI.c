#include <stdio.h>
#include "jordan_linear_MPI.h"

int glob_loc(int loc, int rank, int size) {
	return
}

int loc_glob(int glob, int rank, int size) {

}

int jordan_linear_mpi(int n, double *local_a, double *local_res, double *local_b, int rank, int size, double* buffer) {
	int local_n = 0, k = 0, start = 0, local_n_max = 0, j = 0, i = 0;
	double local_max = 0, tmp = 0;
	MAX local, global;
	
	local_n = n/size;
	if (rank < n%size)
		++local_n;
	for (k = 0; k < n; ++k) {
		start = rank;
		while(start < k) 
			start += size;

		local_n_max = start; local_max = fabs(local_a[(start-rank)/size *n + k]);
		
		for (i = (start-rank)/size; i < local_n; ++i) {
			if (local_max < fabs(local_a[i*n + k])) {
				local_n_max = i*size+rank; local_max = fabs(local_a[i*n + k]);
			}
		}
		local.value = local_max;
		local.index = local_n_max;

		MPI_Allreduce(&local, &global, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);

		if (global.value < 1e-15)
			return -1;

		if (k != global.index) {
			if (rank == k%size || rank == global.index%size) {
				if (k%size == global.index%size) { 
					for (j = 0; j < n; ++j) {
						tmp = local_a[(k-rank)/size * n + j];
						local_a[(k-rank)/size * n + j] = local_a[(global.index-rank)/size * n + j];
						local_a[(local.index-rank)/size * n + j] = tmp;
					}
					tmp = local_b[(k-rank)/size];
					local_b[(k-rank)/size] = local_b[(local.index-rank)/size];
					local_b[(local.index-rank)/size] = tmp;
				} else {
					if (rank == k%size) {
						MPI_Sendrecv_replace(
									local_a + (k-rank)/size * n, n, MPI_DOUBLE, 
									local.index%size, 0, 
									local.index%size, 0, 
									MPI_COMM_WORLD, MPI_STATUS_IGNORE);

						MPI_Sendrecv_replace(
									local_b + (k-rank)/size, 1, MPI_DOUBLE, 
									local.index%size, 0, 
									local.index%size, 0, 
									MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					} else {
						MPI_Sendrecv_replace(
									local_a + (local.index-rank)/size * n, n, MPI_DOUBLE, 
									k%size, 0, 
									k%size, 0, 
									MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						MPI_Sendrecv_replace(
									local_b + (local.index-rank)/size, 1, MPI_DOUBLE, 
									k%size, 0, 
									k%size, 0, 
									MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					}
				}
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);

		if (rank == k%size) {
			for (j = k+1; j < n; ++j) {
				local_a[(k-rank)/size * n + j] /= local_a[(k-rank)/size * n + k];
				buffer[j] = local_a[(k-rank)/size * n + j];
			}
			local_b[(k-rank)/size] /= local_a[(k-rank)/size * n + k];
			buffer[n] = local_b[(k-rank)/size];
			local_a[(k-rank)/size * n + k] = 1;
			buffer[k] = 1.;
		}

		MPI_Bcast(buffer, n+1, MPI_DOUBLE, k%size, MPI_COMM_WORLD);

		for (i = 0; i < local_n; ++i) {
			if (rank != k%size || (k-rank)/size != i) {
				for (j = k+1; j < n; ++j) {
					local_a[i*n+j] -= local_a[i*n+k]*buffer[j];
				}

				local_b[i] -= local_a[i*n+k]*buffer[n];
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}
	
    for (i = 0; i < local_n; ++i) {
		local_res[i] = local_b[i];
	}
	
	return 0;
}

void r1_r2_mpi(int n, double* local_a, double* local_x, double* local_b, double* r1, double* r2, int rank, int size) {
	double norm_ax_b = 0, norm_b = 0, norm_x_xx = 0, norm_xx = 0, sum = 0;
	int i = 0, k = 0, new_rank = 0, tmp_n = 0, local_n = 0, j = 0;

	local_n = n/size;
	if (rank < n%size) 
		++local_n;

	sum = 0;
	for (i = 0; i < local_n; ++i) {
		sum += fabs(local_b[i]);
	}

	MPI_Reduce(&sum, &norm_b, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


	for (k = 0; k < size; ++k) {
		new_rank = (rank+k)%size;
		tmp_n = n/size;
		if (new_rank < n%size) 
			++tmp_n;

		for (i = 0; i < local_n; ++i) {
			for (j = 0; j < tmp_n; ++j) {
				local_b[i] -= local_a[i*n+(j*size+new_rank)]*local_x[j];
			}
		}	
		
		MPI_Sendrecv_replace(local_x, (n/size)+1, MPI_DOUBLE, (rank - 1 + size) % size, 0, (rank+1)%size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	sum = 0;
	for (i = 0; i < local_n; ++i) {
		sum += fabs(local_b[i]);
	}

	MPI_Reduce(&sum, &norm_ax_b, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		*r1 = norm_ax_b/norm_b;

	sum = 0;
	for (i = 0; i < local_n; ++i) {
		sum += fabs(local_x[i]-((rank+size*i+1)%2));
	}

	MPI_Reduce(&sum, &norm_x_xx, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	norm_xx = 0;
	for (i = 0; i < n; ++i) {
		norm_xx += (i+1)%2;
	}

	if (rank == 0)
		*r2 = norm_x_xx/norm_xx;
}
