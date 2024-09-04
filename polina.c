#include <stdio.h>
int main (void)
{
	int x;
	FILE* input = NULL;
	input = fopen("in.txt", "r");
	if (input==NULL)
	{
		printf ("pizdec\n");
		return -1;
	}
	while (fscanf (input, "%d", &x)==1)
	{
		printf ("%d\n", x);
	}
	if (!feof(input))
	{
		printf("vse pizdec\n");
		fclose (input);
		return -1;
	}
	
fclose (input);
return	0;
} 
