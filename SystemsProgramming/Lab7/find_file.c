#define _GNU_SOURCE // to get asprintf() prototype
#include <stdio.h>  // this needs to be the first #include in that case

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>

#include "except.h"


#define DBG printf("Here\n");
/*
 * Here's how you declare an exception with the "except" package:
 */
Except_T StatFailed = { "Failed to open file." };
Except_T MiscellaneousError = {"Miscellaneous error"};
Except_T TargetFound = {"Target found"};

/*
 * ASSIGNMENT
 *
 * Add additional declarations for exceptions TargetFound and
 * MiscellaneousError.
 */

char *progname = "*** progname not set ***"; /* should be argv[0] */

int verbose = 0; /* set on command line */

static void explore(char *path, char *target);

static void traverseDirectory(char path[], char *target)
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * open the directory associated with `path` (hint: opendir(3))
     * if the open fails,
     *     raise the MiscellaneousError exception
     * for each entry in the directory (hint: readdir(3)),
     *     if the entry's name is "."  or ".."
     *         skip that entry
     *     allocate a string `subpath` concatenatiing `path`, "/", and
     *      the entry's name (hint: asprintf())
     *     if in the following ... (hint: TRY ... END_TRY)
     *         call explore on `subpath` and pass `target` as well
     *     ... the TargetFound exception is raised (hint: EXCEPT())
     *         free `subpath`
     *         close the open directory (hint: closedir(3)
     *         re-raise the exception (hint: RERAISE)
     *     ... any other exception is raised (hint: ELSE)
     *         print a message to stderr that explore() failed
     *     free `subpath`
     * close the directory associated with `path` (hint: closedir(3))
     */
	
	DIR* dir;
	struct dirent* entry;
	char* subPath;
	char* garbage;

	TRY
		dir = opendir(path);
		if(dir == NULL)
		{
			RAISE(MiscellaneousError);
		}

	END_TRY;

	do
	{
			entry = readdir(dir);
			if(entry != NULL)
			{
				if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					garbage = subPath;
					asprintf(&subPath, "%s/", entry->d_name);
					free(garbage);
					TRY
						explore(subPath, target);
					EXCEPT(TargetFound)
						free(subPath);
						closedir(dir);
					RERAISE;
					ELSE
						fprintf(stderr, "%s", "explore() has failed");
					END_TRY;
				}
				free(subPath);
			}
	}
	while(entry != NULL);

}
static void explore(char *path, char *target)
/* look at, in, and below `path` for a file named `target` */
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * get the status of `path` (hint: stat(2))
     * if it fails,
     *     raise the StatFailed exception (hint: RAISE)
     * find the last '/'-delimited component of `path` (or use `path`
     *  itself if it contains no '/'s, hint: strrchr())
     * if that component is equal to `target`, (hint: strcmp())
     *     if `verbose` is set,
     *         print `path` to standard output, followed by a newline
     *          (hint: printf())
     *     raise the TargetFound exception (hint: RAISE())
     * if `path` is a directory (hint: S_ISDIR())
     *     traverse it (hint: traverseDirectory())
     */
	struct stat sBuff;
	char* sp;

	TRY
		if(stat(path, &sBuff) != 0)
		{
			RAISE(StatFailed);
		}
	EXCEPT(StatFailed);
	ELSE
		DBG
		sp = strrchr(path, (int)'/');
		if(sp != NULL)
		{
			sp += 1;
		}

		if(strcmp(sp, target) == 0)
		{
			printf("%s\n", path);
			RAISE(TargetFound);
		}
		if(S_ISDIR(sBuff.st_mode) != 0)
		{
			traverseDirectory(path, target);
		}
	END_TRY;
}

void findFile(char *top, char *target)
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * if in the following ... (hint: TRY ... END_TRY)
     *     call explore on `top` and pass `target` as well
     * ... the StatFailed exception is raised
     *     do nothing (put a ";" here)
     * ... the TargetFound exception is raised
     *     exit successfully (hint: exit(3))
     */
	TRY
		explore(top, target);
	EXCEPT(StatFailed);
	EXCEPT(TargetFound)
            exit(EXIT_SUCCESS);
	END_TRY;
}


void usage(void)
{
    printf("usage: %s [-t] target [directory]*\n", progname);
}


int main(int argc, char *argv[])
{
    int i, ch;
    char *target;
    extern int optind;

    progname = argv[0];
    while ((ch = getopt(argc, argv, "hv")) != -1) {
        switch (ch) {

        case 'v':
            verbose = 1;
            break;

        case 'h':
            usage();
            exit(EXIT_SUCCESS);

        case '?':
            usage();
            exit(EXIT_FAILURE);
        }
    }
    if (optind >= argc) {
        usage();
        exit(EXIT_FAILURE);
    }
    target = argv[optind++];
    if (optind == argc) {
        /* directory name(s) not provided */
        findFile(".", target);
    } else {
        /* directory name(s) provided */
        for (i = optind; i < argc; i++)
            findFile(argv[i], target);
    }
    /*
     * If we find the target, we'll exit immediately (and
     * successfully), so if we get to this point...
     */
    exit(EXIT_FAILURE);
}
