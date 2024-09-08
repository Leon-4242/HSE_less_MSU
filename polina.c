#include <stdio.h>
#include <malloc.h>
int weight (int*, int, int);
int comp (int*, int, int, int);
void swap (int*, int, int, int);
void multiplay (int*, int*, int*, int, int);
int main (void)
{
	int *array, *str, *vector;
	int n, m, tmp, i=0, j=0;
	FILE *input = NULL, *output = NULL;
	input = fopen ("in.txt", "r");
	if (input = NULL)
	{
		printf ("pizdec\n");
		return -1;
	}
	if (fscanf (input, "%d %d", &n, &m)<2)
	{
		printf ("pizdec blya\n");
		fclose (input);
		return -1;
	}
	array = (int*)malloc(n*m*sizeof(int));
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			if (fscanf (input, "%d", &tmp) < 1) 
			{
				printf("blyat");
				free(array);
				fclose(input);
				return -1;
			}
			array[i*n+j] = tmp;
		}
	}
	fclose (input);
	
	for (j = 0; j < n; j++) {
		for (i = 0; i < n-1; i++) {
			if (comp(array, m, i, i+1)) {
				swap(array, m, i, i+1);
			}
		}
	}
	vector = (int*)malloc(n*sizeof(int));
	str = (int*)malloc(m*sizeof(int));
	for (i=0; i<m; i++)
	{
		str [i] = array [i];
	}
	multiplay (array, str, vector, m, n);
	output = fopen ("out.txt", "w");
	if (output == NULL)
	{
		printf ("ebat\n");
		free (array);
		free (str);
		free (vector);
		return -1;
	}
	for (i=0; i<n; i++)
	{
		for (j=0; j<m; j++)
		{
			fprintf (output, "%d ", array [i*m + j]) ;
		}
		fprintf (output, "\n");
	}
	fprintf (output, "\n");
	for (i=0; i<n; i++)
	{
		fprintf (output, "%d ", vector[i]);
	}
	free (array);
	free (str);
	free (vector);
	return 0;
}
int weight (int *array, int m, int k)
{
	int mod, p, wei = 0;
	for (p=0; p<m-1; p++)
	{
	if (array[k*m + p+1]>=array[k*m + p])
	{
		mod = array [k*m + p+1] - array [k*m + p];
	}
	else
	{
		mod = array[k*m + p] - array [k*m + p+1];
	
	}
	wei = wei + mod;
	}
	return wei;
}
int comp (int *array, int m, int k, int t)
{
	int v, w;
	v = weight(array, m, k);
	w = weight(array, m, t);
	if (w<v)
	{
		return 1;
	} 
	else if (w>v)
	{
		return -1;
	 } 
	return 0;
}
void swap (int *array, int m, int k, int t)
{
	int tmp, p=0;
	for (p=0; p<m-1; p++)
	{
		tmp = array [k*m + p];
		array [k*m + p] = array [t*m + p];
		array [t*m + p] = tmp; 
	}
}
void multiplay (int *array, int *str, int *vector, int m, int n)
{
	int p, q, tmp, element=0;
	for (p=0; p<n; p++)
	{
		for (q=0 ;q<m; q++)
		{
			tmp = array [p*m + q] * str[q];
			element = element + tmp;
		}
		vector[p] = element;
		element = 0;
	}
}
