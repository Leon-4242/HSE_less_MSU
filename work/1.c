#include <stdio.h>

int main(void) {
	FILE *fin, *fout;
	double s = 0, res = 0, curr = 0;
	fin = fopen("input.txt", "r");
	if (fin == NULL) {
		return -1;
	}
	fout = fopen("output.txt", "w");
	if (fout == NULL) {
		fclose(fin);
		return -1;
	}

	if (feof(fin)) {
		fclose(fin);
		fclose(fout);
		return 0;
	}

	while (fscanf(fin, "%lf", &curr) == 1) {
		res += s*curr;
		s += curr;
	}

	fprintf(fout, "%lf", res);
	fclose(fin);
	fclose(fout);
	return 0;
}
