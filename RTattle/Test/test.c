#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void tFunc(char ar[10][20], int count)
{
	int i;

	for(i = 0; i < count; i++)
	{
		printf("%s\n", ar[i]);
	}

	return;
}

int main(int argc, char* argv[])
{
	int i;
	char ar[10][20];

	for(i = 0; i < argc; i++)
	{
		strcpy(ar[i], argv[i]);
	}

	tFunc(ar, argc);

	return 0;
}
