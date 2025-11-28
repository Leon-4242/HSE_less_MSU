#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix_io.h"
#include "solve1.h"

int main(int argc, char* argv[]) {
    int n, r, s;
    int ret, res;
    double *a, *b;
    int *m;
    double t1, t2 = 0, r1, r2;
    char *name = 0;
    if (!((argc == 5 || argc == 4) && sscanf(argv[1], "%d", &n) == 1 &&
    sscanf(argv[2], "%d", &r) == 1 && sscanf(argv[3], "%d", &s) == 1 && ((argc == 4 && s > 0) || (argc == 5 && s == 0)))) {
        printf("Usage %s : n r s [name]\n", argv[0]);
        return 1;
    }
    if (s == 0) {
        name = argv[4];
    }
    if (!(a = (double *)malloc(n*n*sizeof(double)))) {
        printf("Not enough memory!\n");
        return 2;
    }
    if (name) {
        ret = read_matrix(a, name, n, n);
    }
    else {
        ret = init_matrix(a, s, n, n);
    }
    if (ret != SUCCESS) {
        switch (ret) {
            case ERROR_OPEN:
                printf("Cannot open %s\n", name);
                break;
            case ERROR_READ:
                printf("Cannot read %s\n", name);
                break;
            case ERROR_INIT:
                printf("Cannot initialize a matrix with formula %d\n", s);
                break;
            default:
                printf("Unknown error %d\n", ret);
        }
        free(a);
        return 3;
    }
    if (!(b = (double *)malloc(n*n*sizeof(double)))) {
        printf("Not enough memory!\n");
        free(a);
        return 2;
    }
    if (!(m = (int *)malloc(n*sizeof(int)))) {
        printf("Not enough memory!\n");
        free(a);
        free(b);
        return 2;
    }
    printf("Initial matrix:\n");
    print_matrix(a, n, n, r);
    t1 = clock();
    res = solve(n, a, b, m);
    t1 = (clock() - t1) / CLOCKS_PER_SEC;
    if (name) {
        ret = read_matrix(a, name, n, n);
    }
    else {
        ret = init_matrix(a, s, n, n);
    }
    if (ret != SUCCESS) {
        switch (ret) {
            case ERROR_OPEN:
                printf("Cannot open %s\n", name);
                break;
            case ERROR_READ:
                printf("Cannot read %s\n", name);
                break;
            case ERROR_INIT:
                printf("Cannot initialize a matrix with formula %d\n", s);
                break;
            default:
                printf("Unknown error %d\n", ret);
        }
        free(a);
        free(b);
        free(m);
        return 3;
    }
    if (n > 11000) {
    	r1 = (r2 = 0);
    }
    else {
        t2 = clock();
    	calc_r1_r2(a, b, n, &r1);
    	calc_r1_r2(b, a, n, &r2);
        t2 = (clock() - t2) / CLOCKS_PER_SEC;
    }
    if (res == 0) {
    	printf("Result matrix:\n");
    	print_matrix(b, n, n, r);
    	printf("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = %d\n", argv[0], 19, r1, r2, t1, t2, s, n);
    }
    else {
    	printf("%s : Task = %d Res1 = %d Res2 = %d T1 = %.2f T2 = %.2f S = %d N = %d\n", argv[0], 19, -1, -1, t1, t2, s, n);
    }
    free(a);
    free(b);
    free(m);
    return 0;
}
