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

static int runAsDaemon = 0;

/*
 * Ports less than this are not user-accessible.
 */
#define MIN_USER_PORT 1024

static void executeCommand(char *command, int socketFd,
                    struct sockaddr *socketAddress, socklen_t socketAddressLen)
/* runs `command` and sends the results back to the client via `socketFd` */
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * declare a char buffer of RESPONSE_SIZE `response`
     * create a pipe to execute command (hint: popen(3))
     * if that succeeds,
     *     fill `response` with the output of the command (hint: fread(3))
     *     close the pipe (hint: pclose(3))
     * otherwise,
     *     fill `response` with an error message (hint: snprintf(3))
     *     let `responseLength` be its length
     * send `response` to `socketFd` with a "return address" of
     *  `socketAddress` (hint: sendto(2))
     */

	char response[RESPONSE_SIZE];
	size_t responseLength;
	
	FILE* fPtr = popen(command, "r");
	if(fPtr != NULL)
	{
		responseLength = fread((void*)response, sizeof(char), RESPONSE_SIZE, fPtr);
		pclose(fPtr);
	}
	else
	{
		snprintf(response, RESPONSE_SIZE, "Error! failed to open pipe");
		responseLength = strlen(response);
	}
	sendto(socketFd, (void*)response, responseLength, 0, socketAddress, socketAddressLen);

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

	/*
static void executeCommand(char *command, int socketFd,
                    struct sockaddr *socketAddress, socklen_t socketAddressLen)
	*/

	/*
	char response[RESPONSE_SIZE];
	ssize_t responseLength;
	socklen_t sp;
	int socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in inetSocketAddress;

	getInetSocketAddress(host, SOCK_DGRAM, port, &inetSocketAddress);
	bind(socketFd, (struct sockaddr*)&inetSocketAddress, sizeof(struct sockaddr_in));
	sendto(socketFd, command, (size_t)strlen(command), 0, (struct sockaddr*)&inetSocketAddress, sizeof(struct sockaddr));
	sp = sizeof(struct sockaddr_in);
	responseLength = recvfrom(socketFd, (void*)response, RESPONSE_SIZE, 0, (struct sockaddr*)&inetSocketAddress, &sp);
	fwrite((void*)response, sizeof(char), responseLength, stdout);
	*/
	/*
	responseLength = recvfrom(socketFd, (void*)response, RESPONSE_SIZE, 0, (struct sockaddr*)&inetSocketAddress, &sp);

ssize_t recvfrom(int socket, void *restrict buffer, size_t length,
       int flags, struct sockaddr *restrict address,
       socklen_t *restrict address_len);

socket
Specifies the socket file descriptor.
buffer
Points to the buffer where the message should be stored.
length
Specifies the length in bytes of the buffer pointed to by the buffer argument.
flags
Specifies the type of message reception. Values of this argument are formed by logically OR'ing zero or more of the following values:
address
A null pointer, or points to a sockaddr structure in which the sending address is to be stored. The length and format of the address depend on the address family of the socket.
address_len
Specifies the length of the sockaddr structure pointed to by the address argument.

	*/
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
