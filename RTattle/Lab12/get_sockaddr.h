#include <sys/types.h>

extern void getInetSocketAddress(char *host, int socketType, int port,
                                 struct sockaddr_in *inetSocketAddress);
