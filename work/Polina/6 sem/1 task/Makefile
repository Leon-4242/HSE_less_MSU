CC = mpicc

all: main lib
	$(CC) main.o input.o output.o jordan_linear_MPI.o -o app -lm
main:
	$(CC) -c main.c

lib: input output jordan_linear_MPI

input:
	$(CC) -c input.c

output:
	$(CC) -c output.c

jordan_linear_MPI:
	$(CC) -c jordan_linear_MPI.c


clear:
	rm *.o

start:
	echo "mpirun -np p ./app n r s"
