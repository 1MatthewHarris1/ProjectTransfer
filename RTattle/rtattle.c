/*
 * remsh -- remote shell (client)
 *
 * adapted from Stevens & Rago (2nd. ed.) Figure 16.18
 */
#define __USE_XOPEN
#define _GNU_SOURCE
#include <stdio.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<time.h>

#include "syscall_check.h"
#include "get_sockaddr.h"
#include "client_server.h"

#define MAX_ENTRIES 10000
#define DBG printf("Here %d\n", BUGERATOR); BUGERATOR++;

int BUGERATOR = 0;
int ERROR_NO_READ = 0;

struct Entry
{
	char rawString[200];
};

struct Entry EntryArray[MAX_ENTRIES];
int EntryIndex = 0;

void remoteShell(char *host, char *command, int port)
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * declare a char buffer of RESPONSE_SIZE `response`
     * let `socketFd` be a datagram socket for the Internet address
     *  family (hint: socket(2))
     * fill in a "struct sockaddr_in" `inetSocketAddress` associating
     *  socket with `host` and `port` (hint: getInetSocketAddress())
     * send `command` out on `socketFd` with a "return address" of
     *  `inetSocketAddress` (hint: sendto(2), which expects a pointer
     *  to a "struct sockaddr *", so you'll have to cast it)
     * set `responseLength` to the result of reading `response` from
     *  `socketFd` (hint: recvfrom(2))
     * write `response` on standard output (hint: fwrite(3))
     */

	char response[RESPONSE_SIZE];
	ssize_t responseLength;
	socklen_t sp;
	int socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	int done = 0;
	struct sockaddr_in inetSocketAddress;

	getInetSocketAddress(host, SOCK_DGRAM, port, &inetSocketAddress);
	bind(socketFd, (struct sockaddr*)&inetSocketAddress, sizeof(struct sockaddr_in));
	sendto(socketFd, command, (size_t)strlen(command), 0, (struct sockaddr*)&inetSocketAddress, sizeof(struct sockaddr));
	sp = sizeof(struct sockaddr_in);

	do
	{
		responseLength = recvfrom(socketFd, (void*)response, RESPONSE_SIZE, 0, (struct sockaddr*)&inetSocketAddress, &sp);
		if(strcmp(response, "END OF FILE") == 0)
		{
			done = 1;
		}
		else
		{
			//fwrite((void*)response, sizeof(char), responseLength, stdout);
			if(response[0] == '~')
			{
				ERROR_NO_READ = 1;
			}
			response[responseLength] = '\0';
			if(ERROR_NO_READ == 1)
			{
				strcpy(EntryArray[EntryIndex].rawString, &response[1]);
			}
			else
			{
				strcpy(EntryArray[EntryIndex].rawString, response);
			}
			EntryIndex++;
		}
	}
	while(done == 0);


}

void printEntries(void)
{
	int i;
	int j;
	int k;
	char pString[200];
	
	for(i = 0; i < EntryIndex; i++)
	{
		for(j = 0, k = 0; j <= strlen(EntryArray[i].rawString); j++)
		{
			if(EntryArray[i].rawString[j] != '+')
			{
				pString[k] = EntryArray[i].rawString[j];
				k++;
			}
		}
		printf("%s", pString);
	}

	return;
}


int cmpFunc(const void* alpha, const void* beta)
{
	int rVal = -1;;
	int i, j;
	time_t aTime;
	time_t bTime;
	struct tm aTM;
	struct tm bTM;
	double dif;
	char temp[50];

	for(i = 0; i < strlen(((struct Entry*)alpha)->rawString); i++)
	{
		if(((struct Entry*)alpha)->rawString[i] == '+')
		{
			i++;
			j = 0;
			strcpy(temp, "");
			while(((struct Entry*)alpha)->rawString[i] != '+')
			{
				temp[j] = ((struct Entry*)alpha)->rawString[i]; 
				temp[j + 1] = '\0';
				j++;
				i++;
			}
		}
	}
	
	strptime(temp, "%D", &aTM);

	
	for(i = 0; i < strlen(((struct Entry*)beta)->rawString); i++)
	{
		if(((struct Entry*)beta)->rawString[i] == '+')
		{
			i++;
			j = 0;
			strcpy(temp, "");
			while(((struct Entry*)beta)->rawString[i] != '+')
			{
				temp[j] = ((struct Entry*)beta)->rawString[i]; 
				j++;
				i++;
			}
		}
	}

	strptime(temp, "%D", &bTM);


	aTM.tm_hour = 0;
	aTM.tm_min = 0;
	aTM.tm_sec = 0;

	bTM.tm_hour = 0;
	bTM.tm_min = 0;
	bTM.tm_sec = 0;

	aTime = mktime(&aTM);
	bTime = mktime(&bTM);


	dif = difftime(aTime, bTime);


	if(dif < 0.0)
	{
		rVal = -1;
	}
	else if(dif > 0.0)
	{
		rVal = 1;
	}
	else
	{
		rVal = 0;
	}

	return rVal;
}

int main(int argc, char *argv[])
{
    int port = -1;
    int ch;
	int i, j;
	int hosted = 0;
	char command[500] = "";

    while ((ch = getopt(argc, argv, "u:d:t:p:")) != -1) {
        switch (ch) {

        case 'p':
            port = atoi(optarg);
            break;

	case 'u':
		break;
	case 'd':
		break;
	case 't':
		break;

        default:
            fprintf(stderr, "unknown option \"-%c\" -- exiting\n", ch);
            exit(EXIT_FAILURE);
        }
    }

	for(i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], "host") == 0)
		{
			for(j = (i + 1); j < argc; j++)
			{
				remoteShell(argv[j], command, port);
				hosted = 1;
			}
			i = argc + 1;
		}
		if(hosted == 0 && i != 0)
		{
			strcat(command, argv[i]);
			strcat(command, "+");
		}
	}

	qsort(EntryArray, EntryIndex, sizeof(struct Entry), cmpFunc);

/*
login: the userâs login
on host: the host the user logged in on
tty: the line (control tty) the user logged in on
log on: the time they logged on
log off: one of
from host: the host they logged in fromlogin: the userâs login
*/
	if(ERROR_NO_READ == 0)
	{
		printf("%-20s%-10s%-10s%20s%20s%20s\n", "login:", "line:", "log on:", "log off:", "from host:", "on host:");
		printf("%-20s%-10s%-10s%20s%20s%20s\n", "------", "-----", "-------", "--------", "----------", "--------");
	}
	printEntries();

	if(hosted == 0)
	{
		fprintf(stderr,
			"syntax: %s -p port [ -v ] host command\n",
			argv[0]);
		exit(EXIT_FAILURE);
		
	}

    if (port < 0) {
        fprintf(stderr,
                "syntax: %s -p port [ -v ] host command\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

	/*
    if (optind == argc - 2)
        remoteShell(argv[optind], argv[optind+1], port);
    else if (optind == argc || port < 0) {
        fprintf(stderr,
                "syntax: %s -p port [ -v ] host command\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
	*/

    return EXIT_SUCCESS;
}
