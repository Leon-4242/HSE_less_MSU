all: main lib
	g++ main.o input.o output.o lu_linear_parallel.o -o app -lm -lpthread -g
main:
	g++ main.cpp -c -g

lib: input output lu_linear_parallel

input:
	g++ input.cpp -c -g

output:
	g++ output.cpp -c -g

lu_linear_parallel:
	g++ lu_linear_parallel.cpp -c -g -lpthread


clear:
	rm *.o
