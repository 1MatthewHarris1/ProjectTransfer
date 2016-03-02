#include<iostream>
#include<fstream>
#include<stdlib.h>

#define VERBOSE true

using namespace std; // Because I'm lazy

void getData(char* fileName); // A function used to get data from file (and export formated data to output file)
void cArToiAr(); // A function to convert our input character array to an integer array
int findMaxProduct(); // a function to find the maximum PLENGTH digit product

int FILESIZE = 0; // The number of Characters in the input file
int PLENGTH = 0; // The number of integers we would like to multiply to form out Product (13 for this assignment)

char* cAr; // Global array of characters to be input from file
int* iAr; // Global array of integers which will be converted from the character array above

int main(int argc, char* argv[])
{
	int maxProduct; // The maximum sum (largest PLENGTH digit product in our data set

	if(argc == 3) // Do this only if we have the correct number of command line arguments
	{
		PLENGTH = atoi(argv[2]); // Product length is input from the command line
		getData(argv[1]); // Input Data Array of characters from file
		iAr = (int*)malloc(sizeof(int) * FILESIZE); // Allocate space for integer array based on FILESIZE (which is global)
		cArToiAr(); // Convert character array to integer array
		maxProduct = findMaxProduct(); // Find the max PLENGTH digit  sum
		cout << "The maximum " << PLENGTH << " digit Product is: " << maxProduct << endl;
	}
	else
	{
		cout << "You've made a terrible error" << endl;
		cout << "The syntax is: ./\"fileName\".out \"source file\" \"number of characters to multiply\"; Try again" << endl;
	}

	return 0;
}
int findMaxProduct()
{
	int max = 0; // Our maximum sum
	int* maxAr = (int*)malloc(sizeof(int) * PLENGTH); // An array of PLENGTH integers used to store the digits of our max Product
	int temp = 1; // A temporary variable to help calculate the max

	for(int j = 0; j < FILESIZE - PLENGTH; j++) // FILESIZE - PLENGTH will make it so we won't go over the bounds of our array near the end
	{
		for(int i = 0; i < PLENGTH; i++) // Count up PLENGTH integers from out current position
		{
			temp *= iAr[j + i]; // Multiply each integer together and store the Product in temp
		}
		if(temp > max)
		{
			max = temp; // If our new Product is greater than max, then it becomes the new max
			for(int i = 0; i < PLENGTH; i++)
			{
				maxAr[i] = iAr[i + j]; // Store digits used to form our new max into the maxAr array
			}
		}
		temp = 1; // Reset temp to 1. Must be 1, not 0 because 0 will cause our Product to always be 0
	}
	cout << endl;

	//The following code just prints the maxAr
	cout << endl << "Digits: ";
	for(int i = 0; i < PLENGTH; i++)
	{
		cout << maxAr[i] << " ";
	}
	cout << "\t";

	return max; // Return the max PLENGTH digit Product value
}
void cArToiAr()
{
	for(int i = 0; i < FILESIZE; i++) // For the size of the array (which is equal to FILESIZE
	{
		iAr[i] = (int)cAr[i] - 48; // Convert character to integer and store it in iAr
		//cout << iAr[i]; // Output the formated array so I can see if it comes out correctly
	}
	cout << endl;

	return;
}
void getData(char* fileName)
{
	ifstream inFile(fileName); // Open the input file
	ofstream outFile("out.txt"); // This output file will be used to output formated data for the other part of the assignment
	string inString; // String used to input from file
	int counter = 0; // Used to keep track of position in array

	while(getline(inFile, inString)) // Read through every line in the source file
	{
		for(int i = 0; i < inString.size(); i++) // Read through every character in the string
		{
			if(VERBOSE)			// Prints the whole data set if VERBOSE is true
				cout << inString.c_str()[i];

			FILESIZE++; // Count the file size. This part was a BITCH because I forgot newline characters were a thing lol
		}
		if(VERBOSE) // More stuff for VERBOSE mode
			cout << endl;
	}
	inFile.close(); // Close and reopen the file. I tried to just use inFile.seekg(0, inFile.beg) buuut it didn't like that. So now this.
	inFile.open(fileName);

	if(VERBOSE) // If VERBOSE is true, print the FILESIZE
		cout << "FILESIZE: " << FILESIZE << endl;

	cAr = (char*)malloc((sizeof(char) * FILESIZE + 1)); // Allocate the space for my character array. It's based on the filesize
	cAr[FILESIZE] = '\0'; // Manually append the null terminating character
	
	while(getline(inFile, inString)) // Read through every line in the source file
	{
		for(int i = 0; i < inString.size(); i++) // Read through every character in the string
		{
			cAr[counter] = inString.c_str()[i]; // Store characters in the array
			outFile << cAr[counter] << ","; // Output formatted data to output file
			counter++; // Increment the counter
		}
		counter = 0; // Reset counter
	}

	return;
}
