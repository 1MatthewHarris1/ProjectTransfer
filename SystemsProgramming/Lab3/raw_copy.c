#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "./syscall_check.h"
#include "./allocarray.h"

#define VERBOSE FALSE
#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[])
{
	int rVal = 0;
	int bufferSize;
	int fileADesc;
	int fileBDesc;
	int i;
	size_t bytesRead;
	char* inputFileName;
	char* outputFileName;
	char* buffer;

	if(argc == 4)
	{
		bufferSize = atoi(argv[1]);
		inputFileName = argv[2];
		outputFileName = argv[3];

		if(VERBOSE == TRUE)
		{
			printf("Buffer: %d, Input: %s, Output: %s\n", bufferSize, inputFileName, outputFileName); // Delete this later
		}
		buffer = malloc(sizeof(char) * bufferSize);
		
		SYSCALL_CHECK(fileADesc = open(inputFileName, O_RDONLY));
		SYSCALL_CHECK(fileBDesc = open(outputFileName, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU));

		do
		{
			SYSCALL_CHECK(bytesRead = read(fileADesc, buffer, bufferSize));

			if(bytesRead != 0)
			{
				SYSCALL_CHECK(write(fileBDesc, buffer, bytesRead));
			}
		}
		while(bytesRead != 0);

		free(buffer);
		SYSCALL_CHECK(close(fileADesc));
		SYSCALL_CHECK(close(fileBDesc));
	}
	else
	{
		printf("Erorr! Invalid arguments\n");
		rVal = -1;
	}
	
	return rVal;
}

