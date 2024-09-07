#include<stdio.h>
#include<malloc.h>
int main (void)
{
	char *word, c;
	int i, len=0;
	while (scanf("%c", &c)==1)
	{
		printf("%d\n", len);
		len++;
		word = (char*)realloc(word, (len+1)*sizeof(char));
		word [len-1]=c;
	}
	word[len]='\0';
	printf ("%s\n", word);
	return 0;
}
