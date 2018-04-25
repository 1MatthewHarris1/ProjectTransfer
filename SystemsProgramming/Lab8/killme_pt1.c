#include <stdlib.h>    // for exit()
#include <stdio.h>     // for the usual printf(), etc.
#include <getopt.h>    // for getopt()
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

#define NAMELEN 20
/*
 * "#include" any other necessary headers (as indicated by "man" pages)
 */


// To get `getopt_long()` to work, you need to provide a static
// (usually) array of `struct option` structures.  There are four
// members to be filled in:

// 1. `name` is a (char *) string containing the "long" option name
// (e.g. "--help" or "--format=pdf").

// 2. `has_arg` has one of these values that describe the
// corresponding option:
enum {
    NO_ARG  = 0,  // the option takes no argument
    REQ_ARG = 1, // the option must have an argument
    OPT_ARG = 2  // the option takes an optional argument
};

// 3. The "flag" is an int pointer that determines how the function
// will return its value.  If it is NULL, getopt_long() will return
// "val" (the fourth member) as its function return.  If it is not
// NULL, getopt_long() will return 0 and set "*flag" to "val".

// 4. "val" is an int which is either a character to denote a "short"
// (e.g. "-h" or "-f pdf") option or 0, to denote an option which does
// not have a "short" form.

// The array is terminated by an entry with a NULL name (first element)

static struct option options[] = {
    // elements are:
    // name       has_arg   flag   val
    { "children", OPT_ARG,  NULL,  'c'},
    { "help",     NO_ARG,   NULL,  'h'},
    { "nosync",   NO_ARG,   NULL,  'n'},
    { "pgid",     NO_ARG,   NULL,  'g'},
    { "ppid",     NO_ARG,   NULL,  'p'},
    { NULL }  // end of options table
};

/*
 * These globals are set by command line options. Here, they are set
 * to their default values.
 */
int showPpids = 0;   /* show parent process IDs */
int showPgids = 0;   /* show process group IDs */
int synchronize = 1; /* synchronize outputs (don't use until Part 3) */


enum { IN_PARENT = -1 }; /* must be negative */
/*
 * In the parent, this value is IN_PARENT. In the children, it's set
 * to the order in which they were spawned, starting at 0.
 */
int siblingIndex = IN_PARENT;


void writeLog(char message[], const char *fromWithin)
/* print identifying information about the current process to stdout */
{
	int colonIndent;
	size_t size = 20;

	char processName[NAMELEN];
	if(siblingIndex == IN_PARENT)
	{
		strcpy(processName, "parent");
		colonIndent = 20;
	}
	else
	{
		snprintf(processName, size, "child %d", siblingIndex);
		colonIndent = 30;
	}

	printf("%*s: %d\n", colonIndent, "process ID", getpid());
	if(showPpids == 0)
	{
		printf("%*s: %d\n", colonIndent, "process parent ID", getppid());
	}
	if(showPgids == 0)
	{
		printf("%*s: %d\n", colonIndent, "process group ID", getpgrp());
	}
	printf("%*s: %s\n", colonIndent, "message", message);
	printf("%*s: %s\n", colonIndent, "from within", fromWithin);
	printf("\n");
    /*
     * if `siblingIndex` is IN_PARENT,
     *     set a string buffer `processName` to "parent" (hint: strcpy())
     *     set `colonIndent` to 20
     * otherwise,
     *     set a string buffer `processName` to a string of the form
     *      "child #" where "#" is `siblingIndex` (hint: snprintf())
     *     set `colonIndent` to 30
     * use `colonIndent` to set the indent up to the ":"
     *  using this example, which prints `processName`:
     *
     *    printf("%*s: %s\n", colonIndent, "process name", processName);
     *
     * print the process ID (hint: getpid(2)) indented as above
     * if `showPpids` is true,
     *     print the parent process ID (hint: getppid(2)) indented as above
     * if `showPgids` is true,
     *     print the process group ID (hint: getpgrp(2)) indented as above
     * print `message` indented as above
     * print `fromWithin` indented as above
     * print a blank line to separate this from other log entries
     *
     * (Note: The second argument to this function, `fromWithin`, should
     *  always be `__func__`.)
     */
}


void handler(int sigNum)
/* handle signal `sigNum` */
{
	char message[50];
	size_t size = 50;
	snprintf(message, size, "Signal ID %d \"%s\"", sigNum, strsignal(sigNum));
	writeLog(message, __func__);
    /*
     * create a message that includes `sigNum` and its string
     *  equivalent (hint: snprintf(3) and strsignal(3))
     * add an entry to the log that includes that message (hint:
     *  writeLog())
     */
}


void initSignals(void)
/* initialize all signals */
{
	int i;
	for(i = 1; i < 31; i++)
	{
		if(i != 5 && i != 3)
		{
			if(signal(i, handler) == SIG_ERR)
			{
				writeLog(strsignal(i), __func__);
			}	
		}
	}
    /*
     * for every signal from 1 to _NSIG (NSIG on MacOS)
     *  except SIGTRAP and SIGQUIT (to make debugging easier),
     *     try to set its handler to handler() (hint: signal(2))
     *     if this fails,
     *         add an entry to the log with the signal number
     *          and its string equivalent (hint: strsignal(3) and writeLog())
     */
}


static void usage(char *progname)
/* issue a usage error message */
{
	fprintf(stderr,
			"usage: %s [{args}]*\n", progname);
	fprintf(stderr, "%s\n",
			" {args} are:");
	fprintf(stderr, "%s",
        "  -g or --pgid                     list process group IDs\n"
        "  -n or --nosync                   turn off synchronization\n"
        "  -p or --ppid                     list parent PIDs (default: no)\n"
        "  -h or --help                     help (this message) and exit\n"
        );
	return;
}

int main(int argc, char **argv)
{
    int ch;
    static char *progname = "**UNSET**";

    /*
     * Parse the command line arguments.
     */
    progname = argv[0];
    for (;;) {
        ch = getopt_long(argc, argv, "c::ghnp", options, NULL);
        if (ch == -1)
            break;

        switch (ch) {

        case 'c':
            break;

        case 'g':
			showPgids = 1;
			break;

        case 'h':
			usage(progname);
			exit(0);

        case 'n':
			synchronize = 0;
			break;

        case 'p':
			showPpids = 1;
			break;

        default:
            printf("?? getopt returned character code 0x%02x ??\n", ch);
			exit(1);
        }
    }
	initSignals();
	writeLog("parent is paused for a signal", __func__);
	pause();
    /*
     * initialize all signals (hint: initSignals())
     * add an entry to the log that the parent is pause()'d for a signal
     *  (hint: writeLog())
     * await a signal (hint: pause())
     */
    return 0;
}
