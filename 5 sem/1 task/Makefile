all: main lib
	gcc main.o input.o output.o reflection_inverse.o -o app -lm
main:
	gcc main.c -c

lib: input.o output.o reflection_inverse.o

input.o:
	gcc input.c -c

output.o:
	gcc output.c -c

reflection_inverse.o:
	gcc reflection_inverse.c -c


clear:
	rm *.o app
