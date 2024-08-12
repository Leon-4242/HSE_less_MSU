#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

void func(void);
int search(char*, char**);
int read(FILE*, char**);
int check(char*);

int main(void) {
	func();
	return 0;
}

void func(void) {
	int in; char *file = NULL, *w = NULL;

	printf("Choose type of input data:\n1)Standart filename     2)Custom filename\n");
	if (scanf("%d", &in) < 1) {
		return;
	} else if (in != 1 && in != 2) {
		return;
	}
	if (in == 2) {
		printf("\nEnter filename:\n");
		scanf("%s", file);
	} else {
		file = (char*)malloc(10*sizeof(char));
		strcpy(file, "input.txt");
	}

	if (search(file, &w) == -1) {
		printf ("\nERROR: opening file\n");
		free(file);
		return;
	}
	printf("\nword = %s \n", w);
	free(file);
	free(w);
	return;
}

int search(char *SInputFile, char **word) {
	FILE *f; int buff = 0, max = 0; char *tmp = NULL;
	f = fopen(SInputFile, "r");
        if (f == NULL) {
                return -1;
        }

        while(read(f, &tmp)) {
		buff = check(tmp);
		if (buff > max) {
			if (*word != NULL) {
				free(*word);
				*word = NULL;
			}
			max = buff;
			*word  = (char*)malloc((max+1)*sizeof(char));
			strcpy(*word, tmp);	
		}

		free(tmp);
		tmp = NULL;
	}	
	fclose(f);
	return max;

}

int read(FILE* f, char** w) {
	char c;
       	int length = 0;	
	if(feof(f)) {
		return 0;
	}
	c = fgetc(f);
	*w = (char*)malloc((length+1)*sizeof(char));
	do {
		if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
			(*w)[length] = '\0';
			return 1;
		} else {
			++length;
			*w = (char*)realloc(*w, (length+1)*sizeof(char));
			(*w)[length-1] = c;
		}
		c = fgetc(f);
	} while (!feof(f));

	if (feof(f)) {
		if (length != 0) {
			(*w)[length] = '\0';
		} else {
			return 0;
		}
	}

	return 1;
}

int check(char *word) {
	int l = 0, flag = 0;
	char c;
	c = word[l];
	while (c != '\0') {
		if (!isalpha(c)) {
			return -1;
		}
		if (!flag  && isupper(c)) {
			flag = 1;
		}
		++l;
		c = word[l];
	}

	return flag ? l : -1;
}
