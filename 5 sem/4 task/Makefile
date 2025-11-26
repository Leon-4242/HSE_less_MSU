all: main lib
	gcc main.o input.o output.o reflection_inverse_parallel.o -o app -lm -lpthread
main:
	gcc main.c -c

lib: input output reflection_inverse_parallel

input:
	gcc input.c -c

output:
	gcc output.c -c

reflection_inverse_parallel:
	gcc reflection_inverse_parallel.c -c -lpthread

clear:
	rm *.o
