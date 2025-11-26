all: backery philosophers_deadlock philosophers gluttons

backery: Backery_lib
	gcc -g backery.o -o backery -lpthread

Backery_lib:
	gcc backery.c -c -lpthread

philosophers_deadlock: Philosophers_deadlock_lib
	gcc -g philosophers_deadlock.o -o philosophers_deadlock -lpthread

Philosophers_deadlock_lib:
	gcc philosophers_deadlock.c -c -lpthread

philosophers: Philosophers_lib
	gcc -g philosophers.o -o philosophers -lpthread

Philosophers_lib:
	gcc philosophers.c -c -lpthread

gluttons: Gluttons_lib
	gcc -g gluttons.o -o gluttons -lpthread

Gluttons_lib:
	gcc gluttons.c -c -lpthread

clear:
	rm *.o
