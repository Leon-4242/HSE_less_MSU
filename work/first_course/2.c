#include <stdio.h>
#include <math.h>

typedef struct Point {
	double x;
	double y;
} point;

double d(point p, point q);

int main(void) {
        FILE *fin, *fout;
        double res = 0; point curr = {0, 0}, last = {0, 0};
        fin = fopen("input.txt", "r");
        if (fin == NULL) {
                return -1;
        }
        fout = fopen("output.txt", "w");
        if (fout == NULL) {
                fclose(fin);
                return -1;
        }

	if (fscanf(fin, "%lf %lf", &(last.x), &(last.y)) < 2) {
		fclose(fin);
		fclose(fout);
		return 0;
	}

        while (fscanf(fin, "%lf %lf", &(curr.x), &(curr.y)) == 2) {
              	res += d(curr, last);
		last = curr;
        }

        fprintf(fout, "%lf", res);

	fclose(fin);
	fclose(fout);
	return 0;
}

double d(point p, point q) {
	return sqrt((p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y));
}
