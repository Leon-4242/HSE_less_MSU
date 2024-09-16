#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>


//Standart struct:
typedef struct Product {
        char *Name;
        int Count;
        double Rating;

} prod;

//Modified struct:
typedef struct Product_m {
	char *Name;
	int Count;
	double Rating;
	int Cal;	
} prod_m;

//Flag struct:
typedef struct FLAGS {
	int m;
	int s;
} Flag;


//Enter data block
int input(prod ***array, prod_m ***array_m, int *n, prod **curr, prod_m **curr_m, Flag *flag);

int input_s(FILE *f, prod ***s);
int input_m(FILE *f, prod_m ***s);

int input1(FILE *f, prod **s);
int input1_m(FILE *f, prod_m **s);

int input2(FILE *f, prod *w);
int input2_m(FILE *f, prod_m *w);
int input2_m1(FILE *f, prod_m *w);


//Print data block:
int output(prod **array, prod_m **array_m, int n, prod *curr, prod_m *curr_m, Flag flag);

void output_m(FILE *f, prod_m **s, int n);
void output_s(FILE *f, prod **s, int n);

void output1(FILE *f, prod *s);
void output1_m(FILE *f, prod_m *s);

void output_t(prod **s, int n);
void output_t_m(prod_m **s, int n);

void output1_t(prod *s);
void output1_t_m(prod_m *s);

//Generate data block:
prod* get(void);
prod_m* get_m(void);


//Special functions block:
void func(prod_m ***s, int *n);


//Auxiliary functions block:
int compare(const void * s1, const void * s2);
int pow_i(int base, int exp);


int main(void) {
	int res  = 0, i = 0, n = 0, e_flag = 0; Flag flag = { 0, 0};

	prod **array = NULL;
	prod_m **array_m = NULL;
	prod *curr = NULL;
	prod_m *curr_m  = NULL;

	srand(time(NULL));
	if (input(&array, &array_m, &n, &curr, &curr_m, &flag)) {
		return -1;
	}
	if (flag.m) {
		return 0;
	}
	if(flag.m && flag.s) {
		func(&array_m, &n);
	}
	res = output(array, array_m, n, curr, curr_m, flag);
	if (res == -1) {
		e_flag = 1;
	}

	if (flag.m) {
                if (flag.s) {
                        for (i = 0; i < n; i++) {
	                	free(array_m[i]->Name);
        		        free(array_m[i]);
        		}
        		free(array_m);
                } else {
			for (i = 0; i < n; i++) {
				free(array[i]->Name);
        		        free(array[i]);
       			}
        		free(array);
                }
        } else {
                if (flag.s)  {
                        free(curr_m->Name);
                        free(curr_m);
                } else {
                        free(curr->Name);
                        free(curr);
                }
        }
	if (e_flag) {
		return -1;
	}

	return 0;
}



int input(prod ***array, prod_m ***array_m, int *n, prod **curr, prod_m **curr_m, Flag *flag) {
	FILE *in;
	char in_filename[20]; int flag_in = 0, i = 0;

	printf ("Choose structure (enter number):\n1)Standart {Name, Count, Rating}\n2)Modified {Name, Count, Rating, Cal}\n");
        scanf("%d", &(flag->s)); flag->s--;
	if (flag->s != 0 && flag->s!= 1) {
		flag->m = 2;
		return 0;
	}

        printf ("\nChoose type of input data (enter number):\n1)From file\n2)Generate\n");
        scanf("%d", &flag_in); flag_in--;
	if (flag_in != 0 && flag_in != 1) {
		flag->m = 2;
		return 0;
	}
        if (flag_in) {
                printf("\nEnter number of structures:\n");
                scanf("%d", n);
		if (*n <= 0 )  {
			flag->m = 2;
			return 0;
		}
                if (*n == 1) {
			flag->m = 0;
                        if (flag->s) {
                                *curr_m = get_m();
                        } else {
                                *curr = get();
                        }
                } else {
                        flag->m = 1;
                        if (flag->s) {
                                *array_m = (prod_m**)malloc((*n)*sizeof(prod_m*));
                                for (i = 0; i < *n; i++) {
                                        (*array_m)[i] = get_m();
                                }
                        } else {
                                *array = (prod**)malloc((*n)*sizeof(prod*));
                                for (i = 0; i < *n; i++) {
                                        (*array)[i] = get();
                                }

                        }
                }
        } else {
                printf("\nEnter name of file:\n");
                scanf("%s", in_filename);
                in = fopen(in_filename, "r");
                if (in ==NULL) {
                        printf("\nFailed to open %s.", in_filename);
                        return -1;
                }
                printf ("\nChoose type of data (enter number):\n1)One structer\n2)Array of structures\n");
                scanf("%d", &(flag->m)); flag->m--;
		if (flag->m != 0 && flag->m != 1) {
			fclose(in);
			flag->m =2;
			return 0;
		}
                if (flag->m) {
                        if(flag->s) {
                                *n = input_m(in, array_m);
				if (*n == -1) {
					fclose(in);
					return -1;
				}
                        } else {
                                *n = input_s(in, array);
				if (*n == -1) {
					fclose(in);
					return -1;
				}
                        }
                } else {
                        if (flag->s) {
                                if (input1_m(in, curr_m) != 0) {
                                        printf ("Failed to read data from file.");
                                        fclose (in);
                                        return -1;
                                }
                        } else {
                                if (input1(in, curr) != 0) {
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



int input_s(FILE *f, prod*** s) {
        int i = 0, res = 0, n = 0; prod *curr = NULL;
	res = input1(f, &curr);
	while (res == 0) {	
  		n++;
		*s = (prod**)realloc(*s, n*sizeof(prod*));
		(*s)[n-1] = (prod*)malloc(sizeof(prod));
		(*s)[n-1]->Name = (char*)malloc(strlen(curr->Name)+1);
                strcpy((*s)[n-1]->Name, curr->Name);
                (*s)[n-1]->Count = curr->Count;
                (*s)[n-1]->Rating = curr->Rating;
                free(curr->Name);
		free(curr);
		res = input1(f, &curr);
	}
	if (res == -2) {
		for (i = 0; i < n; i++) {
                        free((*s)[i]->Name);
                        free((*s)[i]);
                }
                free((*s));
                return -1;
        }

        free(curr->Name);
	free(curr);
        return n;
}

int input_m(FILE *f, prod_m*** s) {
	int i = 0, n = 0, res = 0; prod_m* curr = NULL;
	res = input1_m(f, &curr);
	while (res == 0) {
		n++;
		*s = (prod_m**)realloc(*s, n*sizeof(prod_m*));
		(*s)[n-1] = (prod_m*)malloc(sizeof(prod_m));
		(*s)[n-1]->Name = (char*)malloc(strlen(curr->Name)+1);
		strcpy((*s)[n-1]->Name, curr->Name);
		(*s)[n-1]->Count = curr->Count;
		(*s)[n-1]->Rating = curr->Rating;
		(*s)[n-1]->Cal = curr->Cal;
		free(curr->Name);
		free(curr);
		res = input1_m(f, &curr);
	}
	if (res == -2) {
		for (i = 0; i < n; i++) {
			free((*s)[i]->Name);
			free((*s)[i]);
		}
		free((*s));
		return -1;
	}
	free(curr->Name);
	free(curr);
	return n;
}


int input1(FILE *f, prod** s) {
        
	*s =(prod*)malloc(sizeof(prod));

	if (input2(f, *s)) {
                return -1;
        }
        if(fscanf(f, "%d %lf\n", &((*s)->Count), &((*s)->Rating)) < 2) {
		return -2;
        }

        return 0;
}

int input1_m(FILE *f, prod_m **s) {
	
	*s = (prod_m*)malloc(sizeof(prod_m));
	if (input2_m(f, *s)) {
		return -1;
	}
	if(fscanf(f, "%d %lf %d\n", &((*s)->Count), &((*s)->Rating), &((*s)->Cal)) < 3) {
		return -2;
	}

	return 0;
}


int input2(FILE *f, prod *w) {
        char c;
        int length = 0;
        if (!feof(f)) {
                c = fgetc(f);
        }
        w->Name = (char *)malloc((length+1)*sizeof(char));
        while (!feof(f)) {
    		if (c == '\n' || isdigit(c)) {
			if (length == 0) {
				c = fgetc(f);
				continue;
			}
    			w->Name[length] = '\0';
			if (isdigit(c)) {
				if(fseek(f, -1, SEEK_CUR)) {
					printf("Error");
					return -1;
				}
			}
			return 0;
    		} else {
                length++;
                        w->Name = (char *)realloc(w->Name, (length + 1)*sizeof(char));
                        w->Name[length - 1] = c;
                }
                c = fgetc(f);
        }
        if (feof(f))
        {
                if (length !=0)
                {
                        length++;
                        w->Name = (char *)realloc(w->Name, (length + 1)*sizeof(char));
                        w->Name[length-1] = '\0';
                } else {
			return -1;
		}
        }
        return 0;
}

int input2_m(FILE *f, prod_m *w) {
        char c;
        int length = 0;
        if (!feof(f)) {
                c = fgetc(f);
        }
        w->Name = (char *)malloc((length+1)*sizeof(char));
        while (!feof(f)) {
            	if (c == '\n' || isdigit(c)) {
                	if (length == 0) {
				c = fgetc(f);
				continue;
			}
			w->Name[length] = '\0';
                    	if (isdigit(c)) {
                        	if(fseek(f, -1, SEEK_CUR)) {
                                	printf("Error");
                                        return -1;
                                }
                    	}
                    	return 0;
            	} else {
                length++;
                        w->Name = (char *)realloc(w->Name, (length + 1)*sizeof(char));
                        w->Name[length - 1] = c;
                }
                c = fgetc(f);
        }
        if (feof(f))
        {
                if (length !=0)
                {
                        length++;
                        w->Name = (char *)realloc(w->Name, (length + 1)*sizeof(char));
                        w->Name[length-1] = '\0';
                } else {
			return -1;
		}
        }
        return 0;
}


int output(prod **array, prod_m **array_m, int n, prod *curr, prod_m *curr_m, Flag flag) {
	FILE *out;
	char out_filename[20]; int flag_out = 0;

	printf ("\nEnter place for answer:\n1)Terminal    2)File\n");
        scanf("%d", &flag_out); flag_out--;

	if (flag_out != 0 && flag_out != 1) {
		return -2;
	}


        if (flag_out) {
                printf("\nEnter file name of file:\n");
                scanf("%s", out_filename);
                out = fopen(out_filename, "w");
                if (out == NULL) {
                        printf("\nFailed to open %s.", out_filename);
                        return -1;
                }
        }
	if (flag_out) {
		if (flag.m) {
			if (flag.s) {
				output_m(out, array_m, n);
			} else {
				output_s(out, array, n);
			}
		} else {
			if (flag.s) {
				output1_m(out, curr_m);
			} else {
				output1(out, curr);
			}
		}
		fclose(out);
	} else {
		if (flag.m) {
			if (flag.s) {
				output_t_m(array_m, n);
			} else {
				output_t(array, n);
			}	
		} else {
			if (flag.s) {
				output1_t_m(curr_m);
			} else {
				output1_t(curr);
			}       	
		}
	}

	return 0;
}

void output_s(FILE *f, prod **s, int n) {
        int i = 0;
        for (i = 0; i < n; i++) {
        	output1(f, s[i]);
	}
}

void output_m(FILE *f, prod_m **s, int n) {
	int i = 0;
	for (i = 0; i < n; i++) {
		output1_m(f, s[i]);
	}
}

void output1(FILE *f, prod *s) {
        fprintf(f, "%s %d %lf\n", s->Name, s->Count, s->Rating);
}

void output1_m(FILE *f, prod_m *s) {
	fprintf(f, "%-24.24s %7d    %-10.10lf %7d\n", s->Name, s->Count, s->Rating, s->Cal);
}


void output_t(prod **s, int n) {
	int i = 0;
	for (i = 0; i < n; i++) {
		output1_t(s[i]);
	}
}	

void output_t_m(prod_m **s, int n) {
        int i = 0;
        for (i = 0; i < n; i++) {
                output1_t_m(s[i]);
        }
}


void output1_t(prod *s) {
        printf("%s %d %lf\n", s->Name, s->Count, s->Rating);

}	

void output1_t_m(prod_m *s) {
	printf("%s %d %lf %d\n", s->Name, s->Count, s->Rating, s->Cal);
}



prod* get(void) {
        FILE *inA, *inN;
        int n = 0; char *w = NULL; prod* s = NULL;
        int l=0;
	s = (prod*)malloc(sizeof(prod));
        s->Count = rand()%10000;
        s->Rating = rand()/1000.0;

        ///////////////////////////
        inA = fopen("inA.txt", "r");
        if(inA == NULL)
        {
                printf("Error 5\n");
                return s;
        }

        inN = fopen("inN.txt", "r");
        if(inN == NULL)
        {
                printf("Error 5\n");
                fclose(inA);
                return s;
        }
	///////////////////////////


        n = rand()%10;
        input2(inA, s);
        while (n > 0) {
                free(s->Name);
                s->Name = NULL;
                input2(inA, s);
                n--;
        }
        w = (char *)malloc((strlen(s->Name) + 1)*sizeof(char));
        strcpy(w, s->Name);

        free(s->Name);
        s->Name = NULL;
        n = rand()%10;
        input2(inN, s);
        while (n > 0) {
                free(s->Name);
                s->Name = NULL;
                input2(inN, s);
                n--;
        }
        l=strlen(w);
        w = (char *)realloc(w, (strlen(w)+strlen(s->Name)+2)*sizeof(char));
        w[l-1]=' ';
        strcat(w, s->Name);
	free(s->Name);
        s->Name = (char *)malloc((strlen(w)+1)*sizeof(char));
        strcpy(s->Name, w);
        l=strlen(w);
        s->Name[l-1]=0;
        free(w);
        fclose(inA);
        fclose(inN);
        return s;
}


prod_m* get_m(void) {
	FILE *inA, *inN;
	int n ; char *w = NULL; prod_m* s = NULL;
	int l=0;
	s = (prod_m*)malloc(sizeof(prod_m));
	s->Count = rand()%10000;
	s->Rating = rand()/1000.0;
	s->Cal = rand()&1000;

	///////////////////////////
	inA = fopen("inA.txt", "r");
	if(inA == NULL) 
	{
		printf("Error 5\n");
		return s;
	}
	
	inN = fopen("inN.txt", "r");
	if(inN == NULL) 
	{
		printf("Error 5\n");
		fclose(inA);
		return s;
	}
	///////////////////////////

	n = rand()%10;
	input2_m(inA, s);
	while (n > 0) {
		free(s->Name);
		s->Name = NULL;
		input2_m(inA, s);
		n--;
	}
	w = (char *)malloc((strlen(s->Name) + 1)*sizeof(char));
	strcpy(w, s->Name);
	
	free(s->Name);
	s->Name = NULL;
	n = rand()%10;
	input2_m(inN, s);
	while (n > 0) {
		free(s->Name);
		s->Name = NULL;
		input2_m(inN, s);
		n--;
	}
	l=strlen(w);
	w = (char *)realloc(w, (strlen(w)+strlen(s->Name)+2)*sizeof(char));
	w[l-1]=' ';
	strcat(w, s->Name);
	s->Name = (char *)realloc(s->Name, (strlen(w)+1)*sizeof(char));
	strcpy(s->Name, w);
	l=strlen(w);
	s->Name[l-1]=0;		
	free(w);
	fclose(inA);
	fclose(inN);
	return s;
}

void func(prod_m ***s, int *n) {
	int k = 0, i = 0, j = 0, max = 0, max_n = 0, need = 0, flag = 0; int *buff = NULL; prod_m *buf = NULL;
	
	printf("\nWant check callories:\n1)Yes      2)No\n");
	scanf("%d", &k); 
	if (k != 1) {
		return;
	}
	k = 0;
	printf("\nEnter callories:\n");
	scanf("%d",&need); 
	if (need <= 0) {
		return;
	}
	printf("\nWhich list of products you need:\n1)Products with smaller number of callory\n2)Products with most similar sum of callories\n");
	scanf("%d", &flag); flag--;
	if (flag != 0 && flag != 1) {
		return;
	}
	if (flag) {
		buff = (int*)malloc(pow_i(2, *n)*sizeof(int));
		for (i = 0; i < pow_i(2, *n); i++) {
			buff[i] = 0;
			for (j = 0; j < *n; j++) {
				if(((1<<j)&i)>>j) {
					buff[i] += (*s)[j]->Cal;
				}
			}
		}

		for (i = 0; i < pow_i(2, *n); i++) {
			if(max < buff[i] && buff[i] <= need) {
				max = buff[i];
				max_n = i;
			}
		}
		free(buff);

		for(i = 0; i < *n; i++) {
			if(((1<<i)&max_n)>>i) { 
				buf = (*s)[k];
				(*s)[k] = (*s)[i];
				(*s)[i] = buf;
				buf = NULL;
				k++;		
			}
		}

		for (i = k; i < *n; i++) {
			free((*s)[i]->Name);
			free((*s)[i]);
		}
		if (k != 0) {
			*s = (prod_m**)realloc(*s, k*sizeof(prod_m*));
		}
		*n = k;
	} else {
		for (i = 0; i < *n; i++) {
			if((*s)[i]->Cal <= need) {
				k++;
				buff = (int*)realloc(buff, k*sizeof(int));
				buff[k-1] = i;
			}
		}

		for (i = 0; i < k; i++) {
			buf = (*s)[j];
			(*s)[j] = (*s)[buff[i]];
			(*s)[buff[i]] = buf;
			buf = NULL;
			j++;
		}
		free(buff);
		
		for (i = j; i < *n; i++) {
			free((*s)[i]->Name);
			free((*s)[i]);
		}
		if (j != 0) {
			*s = (prod_m**)realloc(*s, j*sizeof(prod_m*));
		}
		*n = j;
	}
}

int compare(const void* s1, const void* s2) {
	return strcmp(((prod_m*)s1)->Name, ((prod_m*)s2)->Name);
}

int pow_i(int base, int exp) {
	int  res = 1;

	while (exp) {
		if(exp%2) {
			res *= base;
			exp--;
		} else {
			base *= base;
			exp = exp/2;
		}
	}

	return res;
}
