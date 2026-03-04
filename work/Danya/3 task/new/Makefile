all: main lib
	gcc main.o input.o output.o jordan_linear_parallel.o -o app -lm -lpthread -g
main:
	gcc main.c -c -g

lib: input output jordan_linear_parallel

input:
	gcc input.c -c -g

output:
	gcc output.c -c -g

jordan_linear_parallel:
	gcc jordan_linear_parallel.c -c -g -lpthread


clear:
	rm *.o
