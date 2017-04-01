#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <time.h> // for `clock_gettime()`

#include "a2d.h"
#include "tspec_diff.h"
#include "mat_mul_pt1.h"

char *progname = "*** error: 'progname' not set ***";

/*couldn't get the makefile to work correctly, this fixed it*/
double tspecDiff(struct timespec tspecStart, struct timespec tspecEnd)
/* returns the difference between two timespec structs in seconds */
{
    return (tspecEnd.tv_sec - tspecStart.tv_sec)
        + 1e-9 * (tspecEnd.tv_nsec - tspecStart.tv_nsec);
}

double *da2d_new(int n, int m)
/* instances an n x m array of doubles */
{
    return malloc(n * m * sizeof(double));
}


void da2d_delete(double *d)
/* frees an array */
{
    free(d);
}


void da2d_printf(double *_d, int n, int m, char fmt[])
/* prints a 2D array using a given format `fmt` for each element */
{
#define d(i,j) _d[I2D(i, n, j, m)]
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf(fmt, d(i,j));
        }
        printf("\n");
    }
#undef d
}


void da2d_fillRandom(double *_a, int n, int m)
/* fills a 2D array with random numbers */
{
#define a(i,j) _a[I2D(i, n, j, m)]
    int i, j;

    // set a to a random matrix
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            a(i,j) = ((double) random()) / RAND_MAX;
        }
    }
#undef a
}

/* usage: issue a usage error message */
static void usage(void)
{
    fprintf(stderr,
            "usage: %s [{args}]\n", progname);
    fprintf(stderr, "%s",
            "computes the matrix product c = a x b for two random matrices\n"
            "a[] and b[] of user-specified size, optionally with threads,\n"
            "and prints the times involved\n"
            " {args} are:\n");
    fprintf(stderr, "%s\n",
            "  -h      this help message");
    fprintf(stderr, "%s\n",
            "  -n {i}  number of rows of c[] and a[] (default: 4)");
    fprintf(stderr, "%s\n",
            "  -m {i}  number of columns of c[] and b[] (default: 4)");
    fprintf(stderr, "%s\n",
            "  -p {i}  number of columns of a[] and rows of b[] (default: 4)");
    fprintf(stderr, "%s\n",
            "  -s {i}  seed value for srandom (default: no seeding)");
    fprintf(stderr, "%s",
            "  -v      verbose: print out a and b as well as their product\n");
    return;
}


int main(int argc, char *argv[])
{
    int n = 4;
    int p = 4;
    int m = 4;
    int verbose = 0;
    int ch, ok;
    double *a, *b, *c;
    int colonIndent;
	
	double realDiff;
	double pCPUDiff;
	struct timespec realTimeStart;
	struct timespec	pCPUTimeStart;
	struct timespec realTimeEnd;
	struct timespec pCPUTimeEnd;

    ok = 1;
    progname = argv[0];
    while ((ch = getopt(argc, argv, "hm:n:p:qs:v")) != -1) {
        switch (ch) {

        case 'h':
            usage();
            exit(EXIT_SUCCESS);

        case 'n':
            n = atoi(optarg);
            break;

        case 'm':
            m = atoi(optarg);
            break;

        case 'p':
            p = atoi(optarg);
            break;

        case 's':
            srandom(atoi(optarg));
            break;

        case 'v':
            verbose = 1;
            break;

        default:
            ok = 0;
            break;
        }
    }
    if (!ok || n <= 0 || p <= 0  || m <= 0) {
        usage();
        exit(1);
    }

    /*
     * This value is set empirically so that all the ':'s line up for
     * readability.
     */
    colonIndent = 30;

    a = da2d_new(n, p);
    da2d_fillRandom(a, n, p);
    if (verbose) {
        printf("%*s:\n", colonIndent, "a");
        da2d_printf(a, n, p, "%8.3f ");
        printf("\n");
    }

    b = da2d_new(p, m);
    da2d_fillRandom(b, p, m);
    if (verbose) {
        printf("%*s:\n", colonIndent, "b");
        da2d_printf(b, p, m, "%8.3f ");
        printf("\n");
    }

    c = da2d_new(n, m);

    /*
     * ASSIGNMENT
     *
     * Insert clock_gettime(3) calls to get the start times for both
     * the elapsed ("REALTIME") and CPU ("PROCESS_CPUTIME") clocks.
     */
	
	clock_gettime(CLOCK_REALTIME, &realTimeStart);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &pCPUTimeStart);


    /*
     * ASSIGNMENT
     *
     * Insert call to `mat_mul()` here.
     */
	mat_mul(c, n, m, a, p, b);

    /*
     * ASSIGNMENT
     *
     * Insert clock_gettime(3) calls to get the stop times for both
     * the elapsed ("REALTIME") and CPU ("PROCESS_CPUTIME") clocks.
     */

	clock_gettime(CLOCK_REALTIME, &realTimeEnd);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &pCPUTimeEnd);

    if (verbose) {
        printf("%*s:\n", colonIndent, "c (= a x b)");
        da2d_printf(c, n, m, "%8.3f ");
        printf("\n");
    }

    /*
     * ASSIGNMENT
     *
     * print out the cpu time (hint: `tspecDiff()`)
     * print out the wall clock time (hint: `tspecDiff()`)
     */

	realDiff = tspecDiff(realTimeStart, realTimeEnd);
	pCPUDiff = tspecDiff(pCPUTimeStart, pCPUTimeEnd);

	printf("realDiff: %lf\npCPUDiff: %lf\n", realDiff, pCPUDiff);

    da2d_delete(a);
    da2d_delete(b);
    da2d_delete(c);

    exit(EXIT_SUCCESS);
}
