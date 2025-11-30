#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fill_matrix.h"

int fill(double *A, const char* filename, int n, int s)
{
    int i,j; FILE* fin;
    if(s == 0)
    {
		fin = fopen(filename, "r");
        if (fin == NULL) return -2;
		for(i = 0; i < n*n; i++)
        {
            if(fscanf(fin,"%lf",&A[i]) != 1)
            {
				fclose(fin);
                return -3;
            }
        }

		fclose(fin);
    }
    else 
    {
		if (s < 0 || s > 4) return -4;
        for(i = 0; i < n; i++)
        {
            for(j = 0; j < n; j++)
            {
                A[i*n + j] = f(s,n,i,j);
            }
        }
    }

    return 0;
}

double max(double a, double b)
{
    if(a > b) return a;
    else return b;
}

double f(int s, int n, int i, int j)
{
    double res;
    res = 0;
    if(s == 1)
    {
        res = n - max(i,j);
    }
    if(s == 2)
    {
        res = max(i,j)-1;
    }
    if (s == 3)
    {
       res = (i-j < 0 ? j-i : i-j);
    }
    if(s == 4)
    {
        res = 1.0/(i + j + 1);
    }
    return res;
}
