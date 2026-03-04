all: main lib
	gcc -g main.o input.o output.o eigen_algorithms.o -o app -lm
main:
	gcc main.c -c

lib: input.o output.o eigen_algorithms.o

input.o:
	gcc input.c -c

output.o:
	gcc output.c -c

eigen_algorithms.o:
	gcc -g eigen_algorithms.c -c


clear:
	rm *.o
