all: lib main
	gcc main.o fill_matrix.o matrix_solve.o -o app -lpthread

main:
	gcc main.c -c -lpthread

lib: fill solve

fill:
	gcc fill_matrix.c -c

solve:
	gcc matrix_solve.c -c -lpthread


clean:
	rm *.o
