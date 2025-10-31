all: main lib
	gcc -g main.o input.o output.o qr_3diagonal.o -o app -lm
main:
	gcc main.c -c

lib: input.o output.o qr_3diagonal.o

input.o:
	gcc input.c -c

output.o:
	gcc output.c -c

qr_3diagonal.o:
	gcc -g qr_3diagonal.c -c


clear:
	rm *.o
