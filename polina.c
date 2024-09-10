#include <stdio.h>
#include <malloc.h>
int comp (int*, int, int, int);
void swap (int*, int, int, int);
void multiplay (int*, int*, int, int);
int main (void)
{
	FILE *input=NULL, *output=NULL;
	int *array, *vector;
	int n, m, i, j, tmp;
	input = fopen ("in.txt", "r");
	if (input == NULL)
	{
		printf ("blyat\n");
		return -1;
	}
	if (fscanf(input, "%d %d", &n, &m)<2)
	{
		printf ("pizdec\n");
		fclose (input);
		return -1;
		}
	if (n<0||m<0)
	{
		printf ("huy\n");
		fclose (input);
		return -1;
	}
	array = (int*)malloc(n*m*sizeof(int));
	for (i=0; 1<n; i++)
	{
		for (j=0; j<m; j++)
		{
			if (fscanf (input, "%d", &tmp) <1)
			 {
			 	fclose (input);
			 	free (array);
				return -1;
			}
			array [i*m+j] = tmp;
		}
		}	
	fclose (input);
	for (i=0; i<n; i++)
	{
		for (j=0; j<n-1; j++)
		{
			if (comp (array, m, j, j+1) == 1)
			{
			swap (array, m, j, j+1);
			}
		}
	}
	vector = (int*)malloc(n*sizeof(int));
	multiplay (array, vector, m, n);
	output  = fopen ("out.txt", "w");
	if (output == NULL)
	{
		printf ("govno\n");
		return -1;
	}
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			fprintf (output, "%d ", array[i*m+j]);
		}
		fprintf (output, "\n");
	}
	
	fprintf (output, "\n");
	
	for (i=0; i<n; i++)
	{
		fprintf (output, "%d ", vector [i]);
	}
	fclose (output);
	free (array);
	free (vector);
	return 0;
}
int comp (int *array, int m, int k, int t)
{
	int i;
	for (i=0; 1<m; i++)
	{
		if (array[k*m+i] < array [t*m +i])
		{
			return 1;
		}
		if (array[k*m+i] > array [t*m +i])
		{
			return -1;
		}
	}
	return 0;
}
void swap (int *array, int m, int k, int t)
{
	int i, tmp;
		for (i=0; i<m; i++)
		{
			tmp = array [k*m+i];
			array [k*m+i] = array [t*m + i];
			array [t*m + i] = tmp;
		}
}
void multiplay (int *array, int *vector, int m, int n)
{
	int i, j, tmp;
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			tmp += array [i*m+j] * array [j];
		}
		vector [i] = tmp;
	}
}
