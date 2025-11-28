enum error_codes {
    ERROR_INIT = (-3),
    ERROR_READ,
    ERROR_OPEN,
    SUCCESS,
};

#define EPS 1e-16

#include <stdio.h>
double max(double n, double m);
double f1(int n, int m, int i, int j);
double f2(int n, int m, int i, int j);
double f3(int n, int m, int i, int j);
double f4(int n, int m, int i, int j);
int read_matrix(double *a, char *name, int n, int m);
int init_matrix(double *a, int k, int n, int m);
void print_matrix(double *a, int n, int m, int r);
void calc_r1_r2(double *a, double *b, int n, double *r);
