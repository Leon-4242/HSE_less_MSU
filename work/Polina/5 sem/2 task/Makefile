all: main lib
	gcc -g main.o input.o output.o jacobi_max.o -o app -lm
main:
	gcc main.c -c

lib: input.o output.o jacobi_max.o

input.o:
	gcc input.c -c

output.o:
	gcc output.c -c

jacobi_max.o:
	gcc -g jacobi_max.c -c


clear:
	rm *.o
