#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	int num = atoi(argv[1]);
	int bound = atoi(argv[2]);
	int seed = atoi(argv[3]);

	rng(num, bound, seed);

	return 0;
}
