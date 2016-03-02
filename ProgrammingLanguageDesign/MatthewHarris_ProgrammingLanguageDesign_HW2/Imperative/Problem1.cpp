#include<iostream>
#include<stdlib.h>

#define MAX 1000

using namespace std;

int main(int argc, char* argv[])
{
	system("clear");
	int total = 0;
	
	for(int i = 0; i <= MAX; i++)
	{
		if(i % 5 == 0 || i % 3 == 0)
		{
			total += i;
		}
	}

	cout << "Total: " << total << endl;

	return 0;
}
