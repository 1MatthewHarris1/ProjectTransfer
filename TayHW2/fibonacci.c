#include <stdio.h>

void clean()				//Accepts only the first character entered
{
	while(getchar() != '\n')
	{
	}
}

int sumFibonacciLoop(int n)
{
	int a[n];	
	int i;
	int sum = 0;

	//Stores the Fibonacci values in an array
	for(i = 0; i < n; i++)
	{
		if(i == 0 || i == 1)	//The first and second Fibonacci values are 1
		{
			a[i] = 1;
			sum += a[i];
		}
		else			//Each value is equal to the sum of the two preceding values
		{
			a[i] = a[i - 1] + a[i - 2];
			sum += a[i];
		}
	}
	
	//This prints the values and the sum
	printf("Loop :\t\t");	
	for(i = 0; i < n; i++)
	{
		if(i == n - 1)
		{
			printf("%d = ", a[i]);
		}
		else
		{
			printf("%d + ", a[i]);
		}
	}
	printf("%d\n", sum);
	
	return 0;
}

int sumFibonacciRecursive(int n, int* sum, int c)		//c allows for totaling only one of each Fibonacci value
{								//The int* allows us to add to the same 'sum' value located at 
								//the memory adress
	int value = 0;		//sets the value to be returned

	if(n == 1)
	{
		value = 1;
		if(c == 1)
		{
			*sum += value;		//The * before sum tells the program not to change the adress of sum, but the value
			printf("%d", value);
		}
	}
	else if(n == 2)	//The second Fibonacci value is equal to the first
	{
		value = sumFibonacciRecursive(n - 1, sum, c);
		if(c == 1)
		{
			*sum += value;
			printf(" + %d", value);
		}	
	}	
	else if(c == 1)	//Prints and sums the first instance of each value	
	{
		value = sumFibonacciRecursive(n - 1, sum, 1) + sumFibonacciRecursive(n - 2, sum, 0);
		*sum += value;
		printf(" + %d", value);
	}
	else		//It doesn't print or sum repeat values used for its calculation
	{
		value = sumFibonacciRecursive(n - 1, sum, 0) + sumFibonacciRecursive(n - 2, sum, 0);
	}

	return value;
}

int main(void)
{
	int n = 1;
	int sum;

	while(n >= 1)				//Allows the program to run multiple times without executing it again
	{
		sum = 0;	//Resets the sum for the recursive function
		printf("Enter a value:  ");
		scanf("%d", &n);
		clean();
		
		if(n <= 0)			//Error message
		{
			printf("Invalid input.\n");
		}
		else
		{
			sumFibonacciLoop(n);
			printf("Recursive :\t");		//Print statement here so it doesn't get reprinted by the recursion
			sumFibonacciRecursive(n, &sum, 1);
			printf(" = %d\n", sum);
		}
	}

	return 0;
}




















