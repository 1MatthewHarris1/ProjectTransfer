#define _GNU_SOURCE // for asprintf(), if needed
#include <stdio.h>  // for printf() (and asprintf(), if needed)
#include <stdlib.h> // for malloc() and free()
#include <string.h> // for strlen() and strcat(), if needed
#include <assert.h> // just in case

#define DBG printf("Here\n");

char *catStrings(char *strs[], int nStrs)
{
	// Modify the body of this function as requested. Do not modify
	// any other part of this file.
	int i = 0;
	int j = 0;
	int n = 0;
	int pos = 0;
	char* rAr = NULL;

	for(i = 0; i < nStrs; i++)
	{
		n += strlen(strs[i]) + 1;
		rAr = realloc(rAr, (sizeof(char) * n));
		
		for(j = 0; j < strlen(strs[i]) + 1; j++)
		{
			if(strs[i][j] == '\0' && i != nStrs - 1)
			{
				//printf("NULL adding: %c\n", strs[i][j]);
				rAr[pos] = ' ';
			}
			else
			{
				//printf("adding: %c\n", strs[i][j]);
				rAr[pos] = strs[i][j];
			}
			pos++;
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
