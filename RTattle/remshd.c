/*
 * unconnected_server -- an unconnected server
 *
 * adapted from Stevens & Rago (2nd. ed.) Figure 16.17
 */
#include <stdio.h>

#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#include "daemonize_me.h"
#include "syscall_check.h"
#include "client_server.h"

#define MAXADDRLEN	256

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

#define DBG printf("Here %d\n", BUGERATOR); BUGERATOR++;

static int runAsDaemon = 0;
int BUGERATOR = 1;

/*
 * Ports less than this are not user-accessible.
 */
#define MIN_USER_PORT 1024

/*
static void pushMessage(char *command, int socketFd,
                    struct sockaddr *socketAddress, socklen_t socketAddressLen)
*/
static void executeCommand(char *command, int socketFd,
                    struct sockaddr *socketAddress, socklen_t socketAddressLen)
/* runs `command` and sends the results back to the client via `socketFd` */
{
	char response[RESPONSE_SIZE];
	/*
	char* rPtr = response;
	char** rpPtr = &rPtr;
	*/
	size_t responseLength = 200;
	FILE* fPtr = popen(command, "r");

	if(fPtr != NULL)
	{
		/*
		responseLength = fread((void*)response, sizeof(char), RESPONSE_SIZE, fPtr);
		pclose(fPtr);
		*/
	
		//while(getline(rpPtr, &responseLength, fPtr) != -1)
		while(fgets(response, RESPONSE_SIZE, fPtr) != NULL)
		{
			printf("The line I got: %s\n", response);
			//strcpy(response, "Hello?\n");
			responseLength = strlen(response);
			sendto(socketFd, (void*)response, responseLength, 0, socketAddress, socketAddressLen);
		}
		sendto(socketFd, (void*)"END OF FILE", responseLength, 0, socketAddress, socketAddressLen);
		
	}
	else
	{
		snprintf(response, RESPONSE_SIZE, "Error! failed to open pipe");
		responseLength = strlen(response);
		sendto(socketFd, (void*)response, responseLength, 0, socketAddress, socketAddressLen);
	}
}


static int initServer(int port)
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * declare a "struct sockaddr_in" `inetSocketAddress`
     * declare a "struct sockaddr" pointer and cast the address of 
     *  `inetSocketAd dress` to it
     * set `inetSockAddress`'s "sin_family" attribute to `AF_INET`
     * set `inetSockAddress`'s "sin_addr.s_addr" attribute to
     *  `INADDR_ANY`
     * set `inetSockAddress`'s port attribute (you'll have to find the
     *  exact name) to `port`
     * let `socketFd` be a datagram socket with family
     *  `socketAddress->sa_family` (hint: socket(2))
     * bind the socket to `socketAddress` (hint: bind(2), and be sure
     *  to use `sizeof(struct sockaddr_in)` as the length of
     *  `socketAddress`
     * if the bind fails,
     *     close `socketFd` (hint: close(2))
     *     return a -1
     * otherwise
     *     return `socketFd`
     */

	struct sockaddr_in inetSockAddress;
	struct sockaddr* addr = (struct sockaddr*)&inetSockAddress;

	inetSockAddress.sin_family = AF_INET;
	inetSockAddress.sin_addr.s_addr = INADDR_ANY;
	inetSockAddress.sin_port = port;

	int socketFd = socket(addr->sa_family, SOCK_DGRAM, 0);
	if(bind(socketFd, addr, sizeof(struct sockaddr_in)) == 0)
	{
		return socketFd;
	}
	else
	{
		close(socketFd);
		return -1;
	}
	
	/*
		int bind(int sockfd, const struct sockaddr *addr,
               		 socklen_t addrlen);

	*/
}


static void logError(char *funcName)
/* note error in `funcName` on the system log (daemon) or stderr (non-daemon) */
{
    if (runAsDaemon)
        syslog(LOG_ERR, "%s error: %s", funcName, strerror(errno));
    else
        fprintf(stderr, "%s error: %s\n", funcName, strerror(errno));
}


void serve(char *hostName, int port)
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * start a server on `port` (hint: initServer())
     * if there's an error,
     *     log the error
     *     return
     * loop:
     *     let `commandLength` be the result of reading a
     *      `COMMAND_SIZE` char buffer `command` from `socketFd`,
     *      noting the requestor's `socketAddress` (and its length
     *      `socketAddressLen`, which must be initialised to the size
     *      of the socketAddress) (hint: recvfrom())
     *     make `command` nul-terminated
     *     execute `command` on `socketFd` with a reply to
     *      `socketAddress` (hint: executeCommand())
     */
	char command[COMMAND_SIZE]; // Maybe
	int commandLength;
	struct sockaddr_in* socketAddress = malloc(sizeof(struct sockaddr_in));
	socklen_t sp = sizeof(struct sockaddr_in);
	int socketFd = initServer(port);

	if(socketFd == -1)
	{
		//log the error (it's a function above)
		logError("serve()");
		return;
	}
	while(1==1)
	{
		commandLength = recvfrom(socketFd, (void*)command, COMMAND_SIZE, 0, (struct sockaddr*)socketAddress, &sp);
		command[commandLength] = '\0';
		executeCommand(command, socketFd, (struct sockaddr*)socketAddress, sp);
	}

	free(socketAddress);
}


static char *getDefaultHostName(void)
{
    char *hostName = NULL;
    int hostNameLen = HOST_NAME_MAX;

    /*
     * Allocate a buffer to hold the host name. If sysconf() provides
     * a maximum host name length, get it. Otherwise, use
     * HOST_NAME_MAX.
     */
#ifdef _SC_HOST_NAME_MAX
    hostNameLen = sysconf(_SC_HOST_NAME_MAX);
    if (hostNameLen < 0)                  /* best guess */
        hostNameLen = HOST_NAME_MAX;
#endif
    /* Allocate a buffer to hold the host name. */
    hostName = malloc((hostNameLen+1) * sizeof(char)); // allow for nul-term
    SYSCALL_CHECK(gethostname(hostName, hostNameLen));
    return hostName;
}


int main(int argc, char *argv[])
{
    int port = -1;
    int ch;
    char *hostName = NULL;

    while ((ch = getopt(argc, argv, "dh:p:")) != -1) {
        switch (ch) {

        case 'd':
            runAsDaemon = 1;
            break;

        case 'h':
            hostName = strdup(optarg); // so we can free() it later
            break;

        case 'p':
            port = atoi(optarg);
            break;

        default:
            fprintf(stderr, "unknown option \"-%c\" -- exiting\n", ch);
            exit(EXIT_FAILURE);
        }
    }
    if (optind != argc || port < 0) {
        fprintf(stderr, "usage: %s [-h hostName] -p port [-d]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (port < MIN_USER_PORT) {
        fprintf(stderr, "port must be at least %d -- exiting\n", MIN_USER_PORT);
        exit(EXIT_FAILURE);
    }
    if (hostName == NULL) // hostName defaulted
        hostName = getDefaultHostName();
    if (runAsDaemon)
        daemonizeMe(argv[0]);
    serve(hostName, port);
    free(hostName);
    exit(EXIT_FAILURE); // shouldn't be reached
}
