/*
**Name: 	Matthew Harris
**Institution: 	Washington State University
**Class:	Computer Science 360
**Assignment:	Homework #1: "finddups"
**Date:		February 14th, 2017
*/

//LIBRARIES
//=============================================================================
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include "./MYsyscall_check.h"	// I revised your syscall_check.h file for error outputs
//=============================================================================

//PREPROCESSOR DEFINITIONS
//=============================================================================
#define DBG printf("Here %d\n", BUGERATOR); BUGERATOR++;	// Used for debugging
#define LINESIZE 250	// Size of a line (I considered trying to make this dynamic, but alas this was easier) 

//FUNCTION PROTOTYPES
//=============================================================================

//readDirectory
//=============================================================================
/*
**Function Name: readDirectory(const char* path)
**
**Function Args: (const char* path) is a string containing the path to the
**				    directory to be read
**
**Description: This function reads the elements of a directory and calls itself
**recursively for each sub directory. The path to each non-directory file is
**stored in a global file using fPtrA (declared in global variables)
**
**Return Type: void
*/
void readDirectory(const char* path);
//=============================================================================

//fileCompare
//=============================================================================
/*
**Function Name: fileCompare(char* fileA, char* fileB)
**
**Function Args: (char* fileA) is a string containing the address of a file
**		 (char* fileB) is a string containing the address of a file
**
**Description: This function compares two files to see if they are the same
**
**Return Type: int	If the files are the same, the function returns 0
**			Otherwise a -1 is returned
*/
int fileCompare(char* fileA, char* fileB);
//=============================================================================

//fileCopy
//=============================================================================
/*
**Function Name: fileCopy(FILE* fileA, FILE* fileB)
**
**Function Args: (FILE* fileA) is a pointer to a file
**		 (FILE* fileB) is a pointer to a file
**
**Description: This function copies the data from fileA into fileB
**
**Return Type: int
*/
int fileCopy(FILE* fileA, FILE* fileB);
//=============================================================================

//linesInFile
//=============================================================================
/*
**Function Name: linesInFile(FILE* file)
**
**Function Args: (FILE* file) is a pointer to a file
**
**Description: reads file until EOF, counting each line, and returns the number
**	       of lines in the file
**
**Return Type: int	The number of lines in the file
*/
int linesInFile(FILE* file);
//=============================================================================

//superFree
//=============================================================================
/*
**Function Name: superFree(char** ar, int arLen)
**
**Function Args: (char** ar) is a pointer to the start of a 2-dimensional array
**		 (int arLen) is the length of the 2-dimensional array
**
**Description: frees the data stored in the ar array
**	       superFree, a type of freedom known only to bald eagles
**
**Return Type: void
*/
void superFree(char** ar, int arLen);
//=============================================================================

//printFile
//=============================================================================
/*
**Function Name: printFile(FILE* file)
**
**Function Args: (FILE* file) is a pointer to the file to be printed
**
**Description: prints the contents of a file
**
**Return Type: void
*/
void printFile(FILE* file);
//=============================================================================

//GLOBAL VARIABLES
//=============================================================================
int BUGERATOR = 0;		// Used for debugging
size_t lineSize = LINESIZE;	// Standard line size
FILE* fPtrA;			// Pointer to file storing all file paths

//MAIN
//=============================================================================
int main(int argc, char* argv[])
{
	int i;				// Used for loops
	int j;				// Used for loops
	int groupNum = 0;		// Integer storing the number of elements in a group
	int entryNum = 0;		// Integer storing the index of a file within a group
	int elNum = 0;			// 
	int foundAGroup = 0;		// Integer indicating whether a new group has been found
	int resPos = 0;			// Stores position in result array
	off_t argFileSize;		// Size (length of path) of argument
	struct stat argFileStats;	// Buffer for stat command
	int* inAr;			// Array of integers mapping to each file in list ->
					// -> prevents both (a match b) and (b match a) from showing up
	int* gNumAr;			// Array of integers storing the size of each group
	char* lineA;			// Array containing a line from fileA
	char* lineB;			// Array containing a line from fileB
	char** resAr;			// Array storing all of the file names to be printed
	FILE* fPtrB;			// Pointer to a second file which will eventually contain all of the data of fileA

	lineA = (char*)malloc(sizeof(char) * lineSize); // malloc space for lineA
	lineB = (char*)malloc(sizeof(char) * lineSize); // malloc space for lineB

	fPtrA = tmpfile(); // Create temporary file
	fPtrB = tmpfile(); // Create temporary file

	for(i = 1; i < argc; i++) // Read through program arguments (skip argv[0] as it will simply be the program name)
	{
		stat(argv[i], &argFileStats);				// Get stats on file at path argv[i]
		argFileSize = argFileStats.st_size;			// Store the size of this file
		if(S_ISDIR(argFileStats.st_mode) != 0)			// If this file is a directory
		{
			if(argv[i][strlen(argv[i]) - 1] == '/')		// If it already contains a slash
			{
				argv[i][strlen(argv[i]) - 1] = '\0';	// Remove the slash
			}
			readDirectory(argv[i]);				// call readDirectory
		}
		else if(S_ISREG(argFileStats.st_mode) != 0)
		{
			fprintf(fPtrA, "%s\n", argv[i]);		// If file is not a directory, store it in fileA
			
		}
	}

	rewind(fPtrA);		// Set fileA back to the beginning
	fileCopy(fPtrA, fPtrB); // Copy fileA to fileB
	rewind(fPtrB);		// Set fileA back to the beginning
	rewind(fPtrA);		// Set fileB back to the beginning

	entryNum = linesInFile(fPtrA);				// Store the number of lines in fileA in entryNum
	inAr = (int*)malloc(sizeof(int) * entryNum);		// Allocate space for integer array
	gNumAr = (int*)malloc(sizeof(int) * entryNum);		// Allocate space for group number array
	resAr = (char**)malloc(sizeof(char*) * entryNum);	// Allocate space for result array

	for(i = 0; i < entryNum; i++)	// Initialize array values
	{
		inAr[i] = 0;		// Set integers to 0
		gNumAr[i] = -1;		// Set group numbers to -1
		resAr[i] = (char*)malloc(sizeof(char) * lineSize);	// Malloc each string in the char** array
		strcpy(resAr[i], "void");				// Set each string in the char** array to "void"
	}

	i = 0;
	while(getline(&lineA, &lineSize, fPtrA) != EOF) // Read every line in fileA
	{
		lineA[strlen(lineA) - 1] = '\0';	// Replace \n with \0
		j = 0;
		elNum = 1;				// Element in group begins at 1
		while(getline(&lineB, &lineSize, fPtrB) != EOF)		// Read every line in fileB
		{
			lineB[strlen(lineB) - 1] = '\0';		// Replace \n with \0
			if(inAr[j] == 0)				// If this path has not been output already
			{
				if(fileCompare(lineA, lineB) == 0)	// Compare this file with the one from fileB
				{
					if(inAr[i] == 0)		// Determines that this is a new group
					{
						groupNum++;		// Increment the number of files in this group
						foundAGroup = 1;	// Indicate that a new group has been found
						strcpy(resAr[resPos], lineA);	// Place the file path in the result array
						resPos++;			// Increment the position in the result array
						inAr[i] = 1;			// Indicate that this combination has been exhausted
					}
					if(inAr[j] == 0)		// Does this check need to be done twice?
					{
						elNum++;		  // Increment element number
						gNumAr[groupNum] = elNum; // Keep track of the max element number in a group (used for output)
						strcpy(resAr[resPos], lineB); // Place file path in the result array
						resPos++;		  // Increment the position in the result array
						inAr[j] = 1;		  // Indicate that this combination has been exhausted
					}
				}
			}
			j++;	// Increment j

		}
		foundAGroup = 0;	// Reset foundAGroup to 0 (no groups found)
		rewind(fPtrB);		// Set fileB back to the beginning
		i++;		// Increment i
	}

	j = 0;
	resPos = 0;
	for(i = 0; i < entryNum; i++) // Execute the max number of loops possible
	{
		if(gNumAr[i] != -1)   // If there are still files to be output
		{
			for(j = 1; j <= gNumAr[i]; j++) // Execute a number of loops equal to the size of this group
			{
				if(strcmp(resAr[resPos], "void") != 0) // If we don't read a void value (shouldn't ever happen)
				{
					printf("%d %d %s\n", gNumAr[i], j, resAr[resPos]); // Print the group information
				}
				else
				{
					i = entryNum + 1; // set conditions to exit loop
				}
				resPos++; // Increment position in result array
			}
			if(gNumAr[i + 1] != -1)	// Do not execute on the last loop
			{
				printf("\n"); // Put a space between groups
			}
		}
	}

	// Free a bunch of memory
	free(lineA);
	free(lineB);
	free(inAr);
	free(gNumAr);
	superFree(resAr, entryNum);

	// Close files even though they're supposed to close on their own after program execution
	fclose(fPtrA);
	fclose(fPtrB);

	return 0;
}
//FUNCTION DEFINITIONS
//=============================================================================

//superFree
//=============================================================================
void superFree(char** ar, int arLen)
{
	int i;
	for(i = 0; i < arLen; i++)	// Count through ever element in the array
	{
		free(ar[i]);		// Free every element in the array
	}
	free(ar);			// Free the array

	return;
}
//=============================================================================

//linesInFile
//=============================================================================
int linesInFile(FILE* file)
{
	int entryNum = 0;					// Number of lines in file
	char* buff = (char*)malloc(sizeof(char) * lineSize);	// Buffer to store each line

	rewind(file);						// Set file to beginning
	while(getline(&buff, &lineSize, file) != EOF)		// Read every line in the file
	{
		entryNum++;					// Increment number of entries (lines)
	}
	rewind(file);	// Set file to beginning
	free(buff);	// Free the buffer

	return entryNum; // Return the number of lines
}
//=============================================================================

/*
//printFile (unused in program execution)
//=============================================================================
void printFile(FILE* file)
{
	char* line = (char*)malloc(sizeof(char) * lineSize);	// Allocate array to store line

	rewind(file);						// Set file to beginning
	while(getline(&line, &lineSize, file) != EOF)		// Read every line in the file
	{
		printf("%s", line);				// Print every line in the file
	}
	rewind(file);						// Set file to beginning

	return;
}
//=============================================================================
*/

//isInFile (unused in program execution)
//=============================================================================
int isInFile(FILE* file, char* str)
{
	int rVal = -1;
	char* line = (char*)malloc(sizeof(char) * lineSize);

	while(getline(&line, &lineSize, file) != EOF)
	{
		if(line[strlen(line) - 1] == '\n')
		{
			line[strlen(line) - 1] = '\0';
		}

		if(strcmp(str, line) == 0)
		{
			rVal = 0;
		}
	}

	return rVal;
}
//=============================================================================

//readDirectory
//=============================================================================
void readDirectory(const char* path)
{
	DIR* dir = opendir(path);// Open the directory with the path passed to function (should always be a path to a directory)
	struct dirent* curDir;	 // Stores element in directory
	struct stat buff;	 // Buffer for stat function
	char* thisPath;		 // New path created with the name of a directory element is appended to the path passed to this function
	int i;			 // Used for loops

	do	// Always execute at least once
	{
		curDir = readdir(dir);	// Read directory element
		if(curDir != NULL)	// If an element was read
		{
	 		thisPath = (char*)malloc(sizeof(char) * (strlen(curDir->d_name) + strlen(path) + 2)); // Allocate space for thisPath
			strcpy(thisPath, path);			// Copy path onto thisPath
			strcat(thisPath, "/");			// Appent /
			strcat(thisPath, curDir->d_name);	// Concat directory element name
			stat(thisPath, &buff);			// Get stats on directory element

			if(S_ISDIR(buff.st_mode) != 0)		// If this element is a direcotry
			{
				// If this directory is not the current directory or the parent directory ("." or "..")
				if(strcmp(curDir->d_name, ".") != 0 && strcmp(curDir->d_name, "..") != 0)
				{
					readDirectory(thisPath); // Recursive function call to readDirectory
				}
			}
			else if(S_ISREG(buff.st_mode) != 0)
			{
				fprintf(fPtrA, "%s\n", thisPath);// Store the path in fileA
			}

			free(thisPath);				 // Free allocated memory
		}
	}
	while(curDir != NULL);  // While there is an element to read
	closedir(dir);		// close this directory

	return;
}

//fileCompare
//=============================================================================
int fileCompare(char* fileA, char* fileB)
{
	int rVal = 0;	// Value to be returned (0 if the files are the same, -1 otherwise)
	char a;		// character for comparisson between files
	char b;		// character for comparisson between files
	int fileADesc;	// descriptor of fileA
	int fileBDesc;	// descriptor of fileB
	size_t bytesReadA;	// Number of bytes read from fileA
	size_t bytesReadB;	// Number of bytes read from fileB
	struct stat fileAStats;	// Buffer for fileA stats
	struct stat fileBStats; // Buffer for fileB stats

	stat(fileA, &fileAStats);	// Get fileA stats
	stat(fileB, &fileBStats);	// Get fileB stats

	if(fileAStats.st_size == fileBStats.st_size)	// Only perform comparisson if both files are the same size
	{
	
		SYSCALL_CHECK(fileADesc = open(fileA, O_RDONLY));	// Open fileA
		SYSCALL_CHECK(fileBDesc = open(fileB, O_RDONLY));	// Open fileB
	
		do	// Execute at least once
		{
			SYSCALL_CHECK(bytesReadA = read(fileADesc, &a, 1));	// Read byte from fileA
			SYSCALL_CHECK(bytesReadB = read(fileBDesc, &b, 1));	// Read byte from fileB
	
			if(bytesReadA != 0 && bytesReadB != 0)	// If bytes were read from both files
			{
				if(a != b)		// If bytes were not the same
				{
					rVal = -1;	// Return value is -1 (files are not the same)
				}
			}
		}
		while(bytesReadA != 0 && bytesReadB != 0 && rVal == 0); 
		// Continue loop until either no bytes are read or file comparisson fails
	}
	else	// If file sizes are different
	{
		rVal = -1; // Return value defaults to -1
	}

	return rVal;
}
//=============================================================================

//fileCopy
//=============================================================================
int fileCopy(FILE* fileA, FILE* fileB)
{
	int rVal = 0;	// Value to be returned
	int bufferSize;	// Size of buffer for copying
	int fileADesc = fileno(fileA);	// Get fileA descriptor
	int fileBDesc = fileno(fileB);	// Get fileB descriptor
	int i;				// Used for loops
	size_t bytesRead;		// Number of bytes read from file
	struct stat fileAStats;		// Buffer for stat
	char* buffer;			// Buffer for copying

	fstat(fileADesc, &fileAStats);		// Get fileA stats
	bufferSize = fileAStats.st_size;	// Set bufferSize to the size of fileA

	buffer = malloc(sizeof(char) * bufferSize);	// Allocate space for buffer

	do	// Execute at least once
	{
		SYSCALL_CHECK(bytesRead = read(fileADesc, buffer, bufferSize));	// Read bytes from fileA into buffer

		if(bytesRead != 0)						// If some bytes were read
		{
			SYSCALL_CHECK(write(fileBDesc, buffer, bytesRead));	// Write those bytes to fileB
		}
	}
	while(bytesRead != 0);		// While there are bytes to read

	free(buffer);			// Free buffer

	return rVal;
}
//=============================================================================
