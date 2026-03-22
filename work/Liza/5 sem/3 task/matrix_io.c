#include <math.h>
#include "matrix_io.h"

double max(double n, double m) {
    return (n > m) ? n : m;
}

double f1(int n, int m, int i, int j) {
    i++;
    j++;
    return max(n, m) - max(i,j) + 1;
}

double f2(int n, int m, int i, int j) {
    (void)n;
    (void)m;
    i++;
    j++;
    return max(i, j);
}

double f3(int n, int m, int i, int j) {
    (void)n;
    (void)m;
    i++;
    j++;
    return (i > j) ? (i - j) : (j - i);
}

double f4(int n, int m, int i, int j) {
    (void)n;
    (void)m;
    i++;
    j++;
    return 1./(i + j - 1);
}

int read_matrix(double *a, char *name, int n, int m) {
    FILE *fp;
    int i, j;
    if (!(fp = fopen(name, "r"))) {
        return ERROR_OPEN;
    }
    for (i = 0; i < n; ++i) {
        for (j = 0; j < m; ++j) {
            if (fscanf(fp, "%lf", a + i*m + j) != 1) {
                fclose(fp);
                return ERROR_READ;
            }
        }
    }
    fclose(fp);
    return SUCCESS;
}

int init_matrix(double *a, int s, int n, int m) {
    double *p;
    int i, j;
    double (*f[])(int, int, int, int) = {f1, f2, f3, f4};
    int len = sizeof(f)/sizeof(f[0]);
    double (*func)(int, int, int, int);
    if (s < 1 || s > len) {
        return ERROR_INIT;
    }
    func = f[s - 1];
    for (i = 0; i < n; ++i) {
        p = a + i*m;
        for (j = 0; j < m; ++j) {
            p[j] = func(n, m, i, j);
        }
    }
    return SUCCESS;
}

void print_matrix(double *a, int n, int m, int r) {
    double *q;
    int nr = n > r ? r : n;
    int mr = m > r ? r : m;
    int i, j;
    for (i = 0; i < nr; ++i) {
        q = a + i*m;
        for (j = 0; j < mr; ++j) {
            printf(" %10.3e", q[j]);
        }
        printf("\n");
    }
}

void calc_r1_r2(double *a, double *b, int n, double *r) {
	int i = 0, j = 0, k = 0; double buff = 0, sum = 0, max = 0;
	
	for (j = 0; j < n; ++j) {
		sum = 0;
		for (i = 0; i < n; ++i) {
			buff = (i == j ? -1 : 0);
			for (k = 0; k < n; ++k) {buff += a[i*n+k]*b[k*n+j];}
			/*tmp[i*n+j] = buff */
			sum += (buff < 0 ? -buff : buff);
		}
		if (j == 0) max = sum;
		else max = (max < sum ? sum : max);
	}

	*r =  max;
}


