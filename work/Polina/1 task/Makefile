all: main lib
	gcc main.o input.o output.o jordan_linear.o -o app -lm -g
main:
	gcc main.c -c -g

lib: input.o output.o jordan_linear.o

input.o:
	gcc input.c -c -g

output.o:
	gcc output.c -c -g

jordan_linear.o:
	gcc jordan_linear.c -c -g


clear:
	rm *.o
