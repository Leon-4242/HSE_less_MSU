#include "input.h"

int input(int s, char* filename, int n, double* a) {
	if (s == 0) return finput(filename, n, a);
	else return sinput(s, n, a);
}

int finput(char* filename, int n, double* a) {
	int i = 0, j = 0; FILE* in = NULL; double curr = 0;

	in = fopen(filename, "r");
	if (in == NULL) return -1;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			if (fscanf(in, "%lf", &curr) < 1) {fclose(in); return -2;}
			else a[i*n+j] = curr;
		}
	}
	fclose(in);
	return 0;
}

int sinput(int s, int n, double* a) {
	int i = 0, j = 0;
	
	if (s < 1 || s > 4) {return -3;}
	
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			a[i*n+j] = formula(s, n, i, j);
		}
	}

	return 0;
}

//formuls changed because (i, j) starts from (0, 0)
double formula(int s, int n, int i, int j) {
	switch (s) {
		case 1:
			return n- (i > j ? i : j);
		case 2:
			if (i == j) return 2;
			else if (i == j+1 || j = i+1) return -1;
			return 0;
		case 3:
			if (i == j && j < n-1) return 1;
			else if (j == n-1) return i;
			else if (i == n-1) return j;	
			return 0;
		case 4:
			return 1./(i+j+1);
	}
	return 0;
}
