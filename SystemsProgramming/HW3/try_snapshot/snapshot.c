#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>
#include<fcntl.h>
#include<libgen.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/resource.h>
#include"./syscall_check.h"
#include"./allocarray.h"

void my_unlink(char* path, char* file)
{
	char p[PATH_MAX];

	strcpy(p, path);
	strcat(p, file);

	unlink(p);

	return;
}
int copy_file(char* source, char* destination, int option)
{
	struct stat st;
	struct stat cur;
	int errorStatus = 0;
	int rVal = 0;
	size_t bufSize = 1;
	blksize_t blkSize = 0;
	char* buf;
	int sDesc;
	int dDesc;
	int bytesRead;

	errorStatus = stat(source, &st);
	if(stat("./", &cur) == 0)
	{
		blkSize = cur.st_blksize;
	}

	if(errorStatus == 0)
	{
		bufSize = st.st_size;

		if(bufSize > blkSize)
		{
			bufSize = blkSize;
		}
		buf = (char*)malloc(sizeof(char) * bufSize);

		SYSCALL_CHECK(sDesc = open(source, O_RDONLY));
		if(option == 0)
		{
			SYSCALL_CHECK(dDesc = open(destination, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU));
		}
		else
		{
			SYSCALL_CHECK(dDesc = open(destination, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP));
		}

		do
		{
			SYSCALL_CHECK(bytesRead = read(sDesc, buf, bufSize));
			if(bytesRead != 0)
			{
				SYSCALL_CHECK(bytesRead = write(dDesc, buf, bytesRead));
			}
		}
		while(bytesRead != 0);
		free(buf);
		
		SYSCALL_CHECK(close(sDesc));
		SYSCALL_CHECK(close(dDesc));
	}
	else
	{
		fprintf(stderr, "Error! Failed to stat file");
		rVal = -1;
	}

	return rVal;
}
int createFileFromBuf(char* buffer, char* destinationDir, char* fileName)
{
	int rVal = 0;
	int dDesc;
	char destination[PATH_MAX];

	strcpy(destination, destinationDir);
	strcat(destination, "/");
	strcat(destination, fileName);

	SYSCALL_CHECK(dDesc = open(destination, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	SYSCALL_CHECK(write(dDesc, buffer, strlen(buffer)));
	SYSCALL_CHECK(close(dDesc));

	return rVal;
}

extern int snapshot(char* ssname, char* progpath, char* readme)
{
	struct rlimit lim;
	struct stat st;
	char destination[PATH_MAX];
	char tempDest[PATH_MAX];
	char tarCom[PATH_MAX] = "tar -zcf ";
	int status;
	int rVal = 0;
	pid_t child;

	lim.rlim_cur = RLIM_INFINITY;
	lim.rlim_max = RLIM_INFINITY;
	setrlimit(RLIMIT_CORE, &lim);

	if(stat(ssname, &st) != 0)
	{
		mkdir(ssname, S_IRWXU);
	}
	else
	{
		rVal = -1;
	}

	if(rVal == 0)
	{
		strcat(tempDest, ssname);
		createFileFromBuf(readme, tempDest, "README");
		strcat(tempDest, "/");
		strcpy(destination, tempDest);
		strcat(destination, basename(progpath));
	
		copy_file(progpath, destination, 0);
	
		child = fork();
	
		if(child == 0)
		{
			abort();
		}
		else
		{
			wait(&status);
			strcpy(destination, tempDest);
			strcat(destination, "core");
			copy_file("./core", destination, 1);
			remove("./core");
			
		}
		strcat(tarCom, ssname);
		strcat(tarCom, ".tar.tgz ./");
		strcat(tarCom, ssname);
		system(tarCom);

		my_unlink(tempDest, "core");
		my_unlink(tempDest, "README");
		my_unlink(tempDest, progpath);

		rmdir(tempDest);
	}

	return rVal;
}
