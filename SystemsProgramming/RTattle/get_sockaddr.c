/*
 * Package to convert a hostname, socketType, and port into a socket
 * address.
 */

#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "get_sockaddr.h"
#include "syscall_check.h"

static void getSocketAddress(char *hostName, int socketFamily,
                             int socketType,
                             struct sockaddr *socketAddress)
/* find a `socketAddress` on `hostName` of `socketFamily` and `socketType` */
{
    /*
     * looks on `hostName` (either a host name or -- for AF_INET -- a
     * string representation of a dotted decimal IP number) for a
     * socket `socketFamily` (AF_*) and `socketType` (SOCK_*) and sets
     * `*socketAddress` to its sockaddr.
     */
    struct addrinfo *addrInfoResponses;
    struct addrinfo addrInfoQuery;

    // provide the canonical name of the hostName
    addrInfoQuery.ai_flags = AI_CANONNAME;

    addrInfoQuery.ai_family = socketFamily;
    addrInfoQuery.ai_socktype = socketType;
    if (!hostName) // i.e., we're a server (this is new)
        addrInfoQuery.ai_flags = AI_PASSIVE;
    addrInfoQuery.ai_protocol = 0;
    addrInfoQuery.ai_addrlen = 0;
    addrInfoQuery.ai_canonname = NULL;
    addrInfoQuery.ai_addr = NULL;
    addrInfoQuery.ai_next = NULL;

    // note that `getaddrinfo()` gets its own CHECK macro (see
    // syscall_check.h)
    GETADDRINFO_CHECK(getaddrinfo(hostName, NULL, &addrInfoQuery,
                              &addrInfoResponses));

    // a single entry should be returned
    assert(addrInfoResponses != NULL);

    // If there's more than one entry, issue a warning.
    if (addrInfoResponses->ai_next != NULL) {
        fprintf(stderr,
                "warning: in getSocketAddress(), multiple addrinfo"
                " records found -- returning the first one\n");
    }
    *socketAddress = *addrInfoResponses->ai_addr;
    freeaddrinfo(addrInfoResponses);
}

void getInetSocketAddress(char *hostName, int socketType, int port,
                          struct sockaddr_in *inetSocketAddress)
/* returns a socket address for an Internet node */
{
/*
static void getSocketAddress(char *hostName, int socketFamily,
                             int socketType,
                             struct sockaddr *socketAddress)
*/
	getSocketAddress(hostName, inetSocketAddress->sin_family, socketType, (struct sockaddr*)inetSocketAddress);
	inetSocketAddress->sin_port = port;
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * get a socket address (hint: getSocketAddress()) (You'll need to
     *  pass the right value for `socketFamily` and cast
     *  `inetSocketAddress` to a "struct sockaddr" pointer.)
     * set `inetSocketAddress`'s port attribute (you'll have to find
     *  the exact name) to `port`
     */
}

/*
 * If ever needed (not on this assignment): Provide
 * get*SocketAddress() functions for all address families (AF_UNIX,
 * AF_INET6, etc.).
 */
