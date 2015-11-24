#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	int num = atoi(argv[1]);
	int bound = atoi(argv[2]);
	int sVal = atoi(argv[3]);
	int result = 0;
	int i = 0;

	seed(sVal);

	for(i = 0; i < num; i++)
	{
		result = rng(bound);
		printf("%d: %d\n", i + 1, result);
	}

	return 0;
}
