#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

typedef struct Product 
{
	char *Name;
	int Count;
	double Rating;	
} prod;

typedef struct ProductM
{
	char *Name;
	int Count;
	double Rating;
	char *Manufactory;
} prodm;

typedef struct FLAGS 
{
int m;
int s;	
} flag;

prod* get (void);
prodm* getm (void);

int input (prod ***array, prodm ***arraym, int*n, prod **curr, prodm **currm, flag *flag );

int input1 (FILE *f, prod**w);
int input1m (FILE *f, prodm **w);

int input2 (FILE *f, prod *w);
int input2m (FILE *f, prodm *w);
int input2mm (FILE *f, prodm *w);

int inputs(FILE *f, prod ***W);
int inputm(FILE *f, prodm ***w);

int output(prod **array, prodm **arraym, int n, prod *curr, prodm *curr_m, flag flag);

void output1 (FILE *f, prod *w);
void output1m (FILE *f, prodm *w);
void outputs (FILE *f, prod **s, int n);
void outputsm (FILE *f, prodm **s, int n);


int main (void)
{
	int i = 0, n = 0, e_flag = 0; flag flag = { 0, 0};

	prod **array = NULL;
	prodm **arraym = NULL;
	prod *curr = NULL;
	prodm *currm  = NULL;

	srand(time(NULL));
	if (input(&array, &arraym, &n, &curr, &currm, &flag)) 
	{
		return -1;
	}	
	if (output(array, arraym, n, curr, currm, flag)) 
	{
		e_flag = 1;
	}

	if (flag.m) 
	{
    	if (flag.s) 
		{
            for (i = 0; i < n; i++) 
			{
	            free(arraym[i]->Name);
				free(arraym[i]->Manufactory);
        		free(arraym[i]);
        	}
        	free(arraym);
        }
		else 
		{
			for (i = 0; i < n; i++) 
			{
				free(array[i]->Name);
        		free(array[i]);
       		}
        	free(array);
        }
    } 
	else 
	{
        if (flag.s)  
		{
            free(currm->Name);
			free(currm->Manufactory);
            free(currm);
        } 
		else 
		{
            free(curr->Name);
            free(curr);
        }
    }
	if (e_flag) 
	{
		return -1;
	}

	return 0;
}
int inputs (FILE *f, prod ***w)
{
	int i=0, res = 0, n = 0;
	prod *curr = NULL;
	res = input1(f, &curr);
	while (res == 0)
	{
		n++;
		*w = (prod**)realloc(*w, n*sizeof(prod*));
		(*w)[n-1] = (prod*)malloc(sizeof(prod));
		(*w)[n-1]->Name = (char*)malloc(strlen(curr->Name)+1);
		strcpy((*w)[n-1]->Name, curr->Name);
		(*w)[n-1]->Count = curr->Count;
		(*w)[n-1]->Rating = curr->Rating;
		free (curr->Name);
		free (curr);
		res = input1(f, &curr);
	}
	if (res == -2)
	{
		for (i=0; i<n; i++)
		{
			free ((*w)[i]->Name);
			free((*w)[i]);
		}
		free ((*w));
		return -1;
	}
	free (curr->Name);
	free (curr);
	return n;
}

int inputm (FILE *f, prodm ***w)
{
	int i=0, res = 0, n = 0;
	prodm *curr = NULL;
	res = input1m(f, &curr);
	while (res == 0)
	{
		n++;
		*w = (prodm**)realloc(*w, n*sizeof(prodm*));
		(*w)[n-1] = (prodm*)malloc(sizeof(prodm));
		(*w)[n-1]->Name = (char*)malloc(strlen(curr->Name)+1);
		strcpy((*w)[n-1]->Name, curr->Name);
		(*w)[n-1]->Manufactory = (char*)malloc(strlen(curr->Manufactory)+1);
		strcpy((*w)[n-1]->Manufactory, curr->Manufactory);
		(*w)[n-1]->Count = curr->Count;
		(*w)[n-1]->Rating = curr->Rating;
		free (curr->Name);
		free (curr->Manufactory);
		free (curr);
		res = input1m(f, &curr);
	}
	if (res == -2)
	{
		for (i=0; i<n; i++)
		{
			free ((*w)[i]->Name);
			free ((*w)[i]->Manufactory);
			free((*w)[i]);
		}
		free ((*w));
		return -1;
	}
	free (curr->Name);
	free (curr->Manufactory);
	free (curr);
	return n;
}

prod* get (void)
{
	FILE *inA, *inN;
	int n, l;
	char *w = NULL;
	prod* res;
	res = (prod*)malloc(sizeof(prod));
	res->Count = rand()%10000;
	res->Rating = rand()/1000.0;
	inA = fopen("inA.txt", "r");
	if (inA == NULL)
	{
		printf("Error G\n");
		return res;
	 } 
	 inN = fopen("inN.txt", "r");
	 if (inN == NULL)
	 {
	 	printf("Error G\n");
	 	fclose (inA);
	 	return res;
	 }
	 n = rand()%10;
	 input2(inA, res);
	 while (n>0)
	 {
	 	free (res->Name);
	 	res->Name = NULL;
	 	input2(inA, res);
	 	n--;
	 }
	 w = (char*)malloc((strlen(res->Name)+1)*sizeof(char));
	 strcpy(w, res->Name);
	 
	 free(res->Name);
	 res->Name = NULL;
	 n = rand()%10;
	 input2(inN, res);
	 while (n>0)
	 {
	 	free (res->Name);
	 	res->Name = NULL;
	 	input2(inN, res);
	 	n--;
	 }
	 l = strlen(w);
	 w = (char*)realloc(w, (strlen(w) + strlen(res->Name)+2)*sizeof(char));
	 w[l-1] = ' ';
	 strcat(w, res->Name);
	 free (res->Name);
	 res->Name = (char*)malloc((strlen(w)+1)*sizeof(char));
	 strcpy(res->Name, w);
	 l = strlen (w);
	 res->Name[l-1] = 0;
	 free(w);
	 fclose (inA);
	 fclose (inN);
	 return res;
}

prodm* getm (void)
{
		FILE *inA, *inN, *inM;
	int n, l;
	char *w = NULL;
	prodm* res;
	res = (prodm*)malloc(sizeof(prodm));	
	res->Count = rand()%10000;
	res->Rating = rand()/1000.0;
	
	inA = fopen("inA.txt", "r");
	if (inA == NULL)
	{
		printf("Error G\n");
		return res;
	 } 
	 inN = fopen("inN.txt", "r");
	 if (inN == NULL)
	 {
	 	printf("Error G\n");
	 	fclose (inA);
	 	return res;
	 }
	 inM = fopen ("inM.txt", "r");
	 if (inM == NULL)
	 {
	 	printf("Error G\n");
	 	fclose (inA);
	 	fclose (inN);
	 	return res;
	 }
	 
	 n = rand()%10;
	 input2m(inA, res);
	 while (n>0)
	 {
	 	free (res->Name);
	 	res->Name = NULL;
	 	input2m(inA, res);
	 	n--;
	 }
	 w = (char*)malloc((strlen(res->Name)+1)*sizeof(char));
	 strcpy(w, res->Name);
	 
	 free(res->Name);
	 res->Name = NULL;
	 
	 n = rand()%10;
	 input2mm(inN, res);
	 while (n>0)
	 {
	 	free (res->Name);
	 	res->Name = NULL;
	 	input2m(inN, res);
	 	n--;
	 }
	 l = strlen(w);
	 w = (char*)realloc(w, (strlen(w) + strlen(res->Name)+2)*sizeof(char));
	 w[l-1] = ' ';
	 strcat(w, res->Name);
	 free (res->Name);
	 res->Name = (char*)malloc((strlen(w)+1)*sizeof(char));
	 strcpy(res->Name, w);
	 l = strlen (w);
	 res->Name[l-1] = 0;
	 free(w);
	 fclose (inA);
	 fclose (inN);
	 
	 n = rand()%10;
	 input2m(inM, res);
	 while (n>0)
	 {
	 	free(res->Manufactory);
	 	res->Manufactory = NULL;
	 	input2mm (inM, res);
	 	n--;
	 }
	 fclose (inM);
	 return res;
}

int input (prod ***array, prodm ***arraym, int*n, prod **curr, prodm **currm, flag *flag )
{
	FILE *in;
	char in_filename[20];
	int flag_in, i;
	
	printf ("Choose structure (enter numper):\n 1) Standart {Name, Count, Rating}\n 2) Modified {Name, Count, Rating, Manufactory}\n");
	scanf ("%d", &(flag->s));
	flag->s --;
	
	if (flag->s !=0 && flag->s != 1)
	{
		flag->m = 2;
		return 0;
		}
			
	printf ("\nChoose type of input data (enter number);\n1)From file\n2)Generate\n");
	scanf ("%d", &flag_in);
	flag_in --;
	
	if (flag_in !=0 && flag_in != 1)
	{
		flag->m = 2;
		return 0;
		}
	if (flag_in)
	{
		printf("\nEnter number of structures:\n");
        scanf("%d", n);
        if (*n == 1) 
		{
			flag->m = 0;
            if (flag->s) 
				{
                	*currm = getm();
            	} 
				else 
				{
                	*curr = get();
                }
        } 
		else 
		{
        	flag->m = 1;
        	if (flag->s) 
			{
                *arraym = (prodm**)malloc((*n)*sizeof(prodm*));
                for (i = 0; i < *n; i++)
				{
                    (*arraym)[i] = getm();
                }
            } 
			else 
			{
                *array = (prod**)malloc((*n)*sizeof(prod*));
                for (i = 0; i < *n; i++)
				{
                    (*array)[i] = get();
                }

            }
        }
	}
	else 
	{
        printf("\nEnter name of file:\n");
        scanf("%s", in_filename);
        in = fopen(in_filename, "r");
        if (in == NULL) 
		{
            printf("\nFailed to open %s.", in_filename);
            return -1;
        }
        printf ("\nChoose type of data (enter number):\n1)One structer\n2)Array of structures\n");
        scanf("%d", &(flag->m)); flag->m--;
        if (flag->m)
		{
            if(flag->s)
			{
                *n = inputm(in, arraym);
            } 
			else 
			{
                *n = inputs(in, array);
            }
        } 
		else 
		{
            if (flag->s) 
			{
                if (input1m(in, currm) != 0) 
				{
                    printf ("Failed to read data from file.");
                    fclose (in);
                    return -1;
                }
            } 
			else 
			{
                if (input1(in, curr) != 0) 
				{
                    printf ("Failed to read data from file.");
                    fclose (in);
                    return -1;
            	}
            }
        }
        fclose(in);
        }
	return 0;
}	
		

int input1 (FILE *f, prod **w)
{
	*w = (prod*)malloc(sizeof(prod));
	if (input2(f, *w))
	{
		return -1;
	}
	if (fscanf(f, "%d %lf\n", &((*w)->Count), &((*w)->Rating))<2)
	{
		return -2;
	}
	return 0;
}
int input1m (FILE *f, prodm **w)
{
	*w = (prodm*)malloc(sizeof(prodm));
	if (input2m(f, *w))
	{
		return -1;
	}
	if (fscanf(f, "%d %lf\n", &((*w)->Count), &((*w)->Rating))<2)
	{
		return -2;
	}
	if (input2mm(f, *w))
	{
		return -3;
	}
	
	return 0;
}

int input2 (FILE *f, prod *w)
{
	char c;
	int lenght = 0;
	if (!feof(f))
	{
		c = getc(f);
	}
	w->Name = (char*)malloc((lenght+1)*sizeof(char));
	while(!feof(f))
	{
		if (c == '\n' || isdigit(c))
		{
			if (lenght==0)
			{
				c = getc(f);
				continue;
			}
			w->Name[lenght] = '\0';
			if (isdigit(c))
			{
				if (fseek(f, -1, SEEK_CUR))
				{
					printf("ERROR\n");
					return -1;
				}
			 } 
			 return 0;
		}
		else 
		{
			lenght++;
			w->Name = (char*)realloc(w->Name, (lenght+1)*sizeof(char));
			w->Name[lenght - 1] = c;
		}
	c = getc(f);
	}
	if (feof(f))
	{
		if (lenght !=0)
		{
			lenght++;
			w->Name = (char*)realloc(w->Name, (lenght+1)*sizeof(char));
			w->Name[lenght-1] = c;
			w->Name[lenght] = '\0';
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

int input2m (FILE *f, prodm *w)
{
	char c;
	int lenght = 0;
	if (!feof(f))
	{
		c = fgetc(f);
	}
	w->Name = (char*)malloc((lenght+1)*sizeof(char));
	while(!feof(f))
	{
		if (c == '\n' || isdigit(c))
		{
			if (lenght==0)
			{
				c = fgetc(f);
				continue;
			}
			w->Name[lenght] = '\0';
			if (isdigit(c))
			{
				if (fseek(f, -1, SEEK_CUR))
				{
					printf("ERROR\n");
					return -1;
				}
			 } 
			 return 0;
		}
		else 
		{
			lenght++;
			w->Name = (char*)realloc(w->Name, (lenght+1)*sizeof(char));
			w->Name[lenght - 1] = c;
		}
	c = fgetc(f);
	}
	if (feof(f))
	{
		if (lenght !=0)
		{
			lenght++;
			w->Name = (char*)realloc(w->Name, (lenght+1)*sizeof(char));
			w->Name[lenght-1] = c;
			w->Name[lenght] = '\0';
		} else {
			return -1;
		}
	}
	return 0;
}

int input2mm (FILE *f, prodm* w)
{
	
	char c;
	int lenght = 0;
	if (!feof(f))
	{
		c = fgetc(f);
	}
	w->Manufactory = (char*)malloc((lenght+1)*sizeof(char));
	while(!feof(f))
	{
		if (c == '\n' || isdigit(c))
		{
			if (lenght==0)
			{
				c = fgetc(f);
				continue;
			}
			w->Manufactory[lenght] = '\0';
			if (isdigit(c))
			{
				if (fseek(f, -1, SEEK_CUR))
				{
					printf("ERROR\n");
					return -1;
				}
			 } 
			 return 0;
		}
		else 
		{
			lenght++;
			w->Manufactory = (char*)realloc(w->Manufactory, (lenght+1)*sizeof(char));
			w->Manufactory[lenght - 1] = c;
		}
	c = fgetc(f);
	}
	if (feof(f))
	{
		if (lenght !=0)
		{
			lenght++;
			w->Manufactory = (char*)realloc(w->Manufactory, (lenght+1)*sizeof(char));
			w->Manufactory[lenght-1] = c;
			w->Manufactory[lenght] = '\0';
		}
		else
		{
			return -1;
		}
	}
	return 0;	
}

int output(prod **array, prodm **arraym, int n, prod *curr, prodm *currm, flag flag)
{
	FILE *out;
	char out_filename[20]; int flag_out = 0;

	printf ("\nEnter place for answer:\n1)Terminal    2)File\n");
    scanf("%d", &flag_out); 
	flag_out--;

    if (flag_out) 
	{
        printf("\nEnter file name of file:\n");
        scanf("%s", out_filename);
        out = fopen(out_filename, "w");
        if (out == NULL) 
		{
            printf("\nFailed to open %s.", out_filename);
            return -1;
        }
    }
	if (flag_out) 
	{
		if (flag.m) 
		{
			if (flag.s) 
			{
				outputsm(out, arraym, n);
			} 
			else 
			{
				outputs(out, array, n);
			}
		} 
		else 
		{
			if (flag.s) 
			{
				output1m(out, currm);
			} 
			else 
			{
				output1(out, curr);
			}
		}
		fclose(out);
	} 
	else 
	{
		if (flag.m) 
		{
			if (flag.s) 
			{
				outputsm(stdout, arraym, n);
			} 
			else 
			{
				outputs(stdout, array, n);
			}	
		} 
		else 
		{
			if (flag.s) 
			{
				output1m(stdout, currm);
			} 
			else 
			{
				output1(stdout, curr);
			}       	
		}
	}

	return 0;
}
void output1 (FILE *f, prod *w)
{
	fprintf (f, "%s %d %lf\n", w->Name, w->Count, w->Rating);
}
void output1m (FILE *f, prodm *w)
{
	fprintf (f, "%s %d %lf %s\n", w->Name, w->Count, w->Rating, w->Manufactory);
}
void outputs (FILE *f, prod **s, int n)
{
	int i = 0;
	for (i=0; i<n; i++)
	{
		output1(f, s[i]);
	}
}
void outputsm (FILE *f, prodm **s, int n)
{
	int i = 0;
	for (i=0; i<n; i++)
	{
		output1m(f, s[i]);
	}
}
