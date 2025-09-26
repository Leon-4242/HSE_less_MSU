#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int i = 0; long int n = 0; long int r = 0;
	long int s = 0; char* filename = NULL;
	
	//Initialization
	if (argc < 4 || argc > 5) {
		printf("Invalid number of arguments.");
		return -1;
	}
	n = strtol(argv[1], NULL, 10);
	r = strtol(argv[2], NULL, 10);
	s = strtol(argv[3], NULL, 10);
	if (s != 0) {filename  = argv[4];}
	return 0;
}
