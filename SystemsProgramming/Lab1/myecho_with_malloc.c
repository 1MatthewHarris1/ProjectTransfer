#define _GNU_SOURCE // for asprintf(), if needed
#include <stdio.h>  // for printf() (and asprintf(), if needed)
#include <stdlib.h> // for malloc() and free()
#include <string.h> // for strlen() and strcat(), if needed
#include <assert.h> // just in case

char *catStrings(char *strs[], int nStrs)
{
	// Modify the body of this function as requested. Do not modify
	// any other part of this file.
	int i = 0;
	int j = 0;
	int n = 0;
	int elements = 0;
	char* rAr;
	char temp = '0';

	for(i = 0; i < nStrs; i++)
	{
		for(j = 0, temp = '0'; j < strlen(strs[i]); j++)
		{
			temp = strs[i][j];
			if(temp != '\0')
			{
				n++;
			}
		}
	}

	elements = (n + nStrs);
	rAr = (char*)malloc(sizeof(char) * elements);

	rAr[elements - 1] = '\0';

	for(i = 0, n = 0; i < nStrs; i++)
	{
		for(j = 0, temp = '0'; j < strlen(strs[i]); j++)
		{
			temp = strs[i][j];
			if(temp != '\0')
			{
				rAr[n] = temp;
				n++;
			}
		}
		if(i != nStrs - 1)
		{
			rAr[n] = ' ';
			n++;
		}
	}
	
	return rAr;
}

int main(int argc, char *argv[])
{
	char *result = catStrings(argv+1, argc-1);
	printf("%s\n", result);
	free(result);
	return 0;
}

