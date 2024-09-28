#include <stdio.h>
#include <math.h>
#define eps 1e-12

double function (double);

int main (void)
{
	double x;
	while(1) 
	{
		printf ("Enter number:\n");
		if (scanf ("%lf", &x)<1)
		{
			break;
		}
		printf("\nMy arsh = %lf\n\n", function(x));
	}
	return 0;
}

double function (double x)
{
	int n=0;
	double an = 0, sn = 0, y = 0;
	if (fabs(x) < eps) 
	{
		return 0;
	}
	if (fabs(x) < 1)
	{
		n = 1;
		an = x;
		sn = x;
		while (fabs(an) > eps)
		{
			an *=(-1)*(2*n)*pow(x, 2)*(2n-1)/(4*pow(n, 2)*(2n+1));
			sn += an;
			n++;
		}
		
		return sn;
	}
	else
	{
		y = sqrt((sqrt(pow(x, 2)+1)-1)/2);
		return 2*function(y);
	}
}
