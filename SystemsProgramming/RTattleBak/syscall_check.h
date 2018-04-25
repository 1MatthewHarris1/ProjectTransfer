#ifndef _SYSCALL_H
#include <stdio.h> /* for sprintf(), fprintf(), and perror() */
#include <stdlib.h> /* for exit */
#include <errno.h>  /* for errno */
#include <assert.h> /* for assert() */
#include <string.h> /* for strlen() */

#include "allocarray.h"

/*
 *  Use this macro to surround all POSIX-compliant system calls.  It
 *  will check the and, if there is an error, print information about
 *  it and exit.
 *
 *  Exception: For some reason, despite being POSIX-compliant,
 *  getaddrinfo() has its own set of error returns. In that case, use
 *  the GETADDRINFO_CHECK() macro below instead of SYSCALL_CHECK().
 */

#define SYSCALL_CHECK(call) \
    \
    do { \
        int __status = (call); \
    \
        if (__status < 0) { \
            char *buf; \
    \
            /* make sure buf is big enough */ \
            ALLOC_ARRAY(buf, char, strlen(#call) + strlen(__FILE__) + 1024); \
            sprintf(buf, \
                    "in file \"%s\", line %d,\n  failed system call: %s\n" \
                    "  errno: %d\n" \
                    "  message", \
                    __FILE__, __LINE__, #call, errno); \
            perror(buf); \
            FREE_ARRAY(buf); \
            fprintf(stderr, "  consequence: exiting\n"); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#define GETADDRINFO_CHECK(call) \
    \
    do { \
        int __status = (call); \
    \
        if (__status < 0) { \
            const char *gaiError = gai_strerror(__status); \
    \
            fprintf(stderr, \
                    "in file \"%s\", line %d,\n" \
                    "  failed call: %s\n" \
                    "       return: %d\n" \
                    "      message: %s\n" \
                    "  consequence: exiting\n", \
                        __FILE__, __LINE__, #call, __status, gaiError);  \
            exit(EXIT_FAILURE); \
        } \
     } while (0)
#endif /* _SYSCALL_H */
