all: main lib
	g++ -g main.o string_list.o -o app

main:
	g++ -g main.cpp -c

lib:
	g++ -g string_list.cpp -c

clear:
	rm *.o
