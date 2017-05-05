/*
 * This function will "daemonize" its calling program.  It is adapted
 * from Stevens & Rago (2nd. ed.), Figure 13.1.
 */

#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#include "syscall_check.h"

enum {
    MAX_FD_DEFAULT = 1024, // default maximum number of file descriptors
};

void daemonizeMe(const char *cmd)
{
    int fd, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rlMaxFd;
    struct sigaction sa;

    /*
     * Clear file creation mask.
     */
    umask(0);

    /*
     * Get maximum number of file descriptors.
     */
    SYSCALL_CHECK(getrlimit(RLIMIT_NOFILE, &rlMaxFd));

    /*
     * Become a session leader to lose controlling TTY.
     */
    SYSCALL_CHECK(pid = fork());
    if (pid != 0)          /* parent */
        exit(0);
    setsid();

    /*
     * Ensure future opens won't allocate controlling TTYs.
     */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    SYSCALL_CHECK(sigaction(SIGHUP, &sa, NULL));
    SYSCALL_CHECK(pid = fork());
    if (pid != 0)          /* parent */
        exit(0);

    /*
     * Change the current working directory to the root so
     * we won't prevent file systems from being unmounted.
     */
    SYSCALL_CHECK(chdir("/"));

    /*
     * Close all open file descriptors.  If there's no maximum file
     * descriptor, use MAX_FD_DEFAULT.
     */
    if (rlMaxFd.rlim_max == RLIM_INFINITY)
        rlMaxFd.rlim_max = MAX_FD_DEFAULT;
    for (fd = 0; fd < rlMaxFd.rlim_max; fd++)
        close(fd); // ignore error (fd not open)

    /*
     * Attach file descriptors 0, 1, and 2 to /dev/null.
     */
    SYSCALL_CHECK(fd0 = open("/dev/null", O_RDWR));
    fd1 = dup(0);
    fd2 = dup(0);

    /*
     * Initialize the log file. Remember that you can read the system
     * log with dmesg(1).
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
            fd0, fd1, fd2);
        exit(1);
    }
}
