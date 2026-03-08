all: main lib
	gcc main.o input.o output.o lu_linear_parallel.o -o app -lm -lpthread -g
main:
	gcc main.c -c -g

lib: input output lu_linear_parallel

input:
	gcc input.c -c -g

output:
	gcc output.c -c -g

lu_linear_parallel:
	gcc lu_linear_parallel.c -c -g -lpthread


clear:
	rm *.o
