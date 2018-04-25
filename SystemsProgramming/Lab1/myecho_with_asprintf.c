#define _GNU_SOURCE // for asprintf(), if needed
#include <stdio.h>  // for printf() (and asprintf(), if needed)
#include <stdlib.h> // for malloc() and free()
#include <string.h> // for strlen() and strcat(), if needed
#include <assert.h> // just in case

/*==================Comment Responses========================
*Practical: 	The effeciency here really depends on the effeciency of the funcions used.
*		Assuming that they are well build (a safe assumption I think) it seems that
*		using asprintf to solve the problem is more effecient as it relies less on
*		my code and more on the well built function.
*
*Design:	I prefer to use the solution that gives me the most control. I like knowing
*		exactly what is going on in my program, meaning that the solution with
*		malloc is preferable.
*///=========================================================

char *catStrings(char *strs[], int nStrs)
{
	// Modify the body of this function as requested. Do not modify
	// any other part of this file.
	int i = 0;
	int j = 0;
	int pos = 0;
	char* rAr = NULL;
	char* garbage;

	for(i = 0; i < nStrs; i++)
	{
		garbage = rAr;
		asprintf(&rAr, "%s%s", rAr, strs[i]);
		free(garbage);
		
		for(j = 0; j < strlen(strs[i]) + 1; j++)
		{
			if(strs[i][j] == '\0' && i != nStrs - 1)
			{
				rAr[pos] = ' ';
			}
			else
			{
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
