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
		printf("\nMy arsh = %.12lf\n\n", function(x));
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
		n = 0;
		an = x;
		sn = 0;
		while (fabs(an) > eps)
		{
			sn += an;
			an *= ((-1)*pow(2*n+1, 2)*pow(x, 2))/(2*(n+1)*(2*n+3));
			n++;
		}
		
		return sn;
	}
	else
	{
		y = sqrt((sqrt(pow(x, 2)+1)-1)/2);
		if (x < 0) y *= -1;
		return 2*function(y);
	}
}
