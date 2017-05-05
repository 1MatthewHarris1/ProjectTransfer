/*
 * remsh -- remote shell (client)
 *
 * adapted from Stevens & Rago (2nd. ed.) Figure 16.18
 */
#include <stdio.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "syscall_check.h"
#include "get_sockaddr.h"
#include "client_server.h"


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
			fwrite((void*)response, sizeof(char), responseLength, stdout);
		}
	}
	while(done == 0);


	/*
		size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream);

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
	/*
		int bind(int sockfd, const struct sockaddr *addr,
               		 socklen_t addrlen);

	*/

	/*
		ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);

	*/
}

int main(int argc, char *argv[])
{
    int port = -1;
    int ch;

    while ((ch = getopt(argc, argv, "p:")) != -1) {
        switch (ch) {

        case 'p':
            port = atoi(optarg);
            break;

        default:
            fprintf(stderr, "unknown option \"-%c\" -- exiting\n", ch);
            exit(EXIT_FAILURE);
        }
    }
    if (port < 0) {
        fprintf(stderr,
                "syntax: %s -p port [ -v ] host command\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
    if (optind == argc - 2)
        remoteShell(argv[optind], argv[optind+1], port);
    else if (optind == argc || port < 0) {
        fprintf(stderr,
                "syntax: %s -p port [ -v ] host command\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
