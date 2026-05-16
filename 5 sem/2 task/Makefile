all: main lib
	gcc -g main.o input.o output.o qr_3diagonal.o -o app -lm
main:
	gcc main.c -c

lib: input output qr_3diagonal

input:
	gcc input.c -c

output:
	gcc output.c -c

qr_3diagonal:
	gcc -g qr_3diagonal.c -c


clear:
	rm *.o
