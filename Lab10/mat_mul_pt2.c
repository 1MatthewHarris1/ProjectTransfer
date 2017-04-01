#include <stdlib.h> // for malloc()
#include <time.h>
#include <assert.h>
#include <pthread.h>

#include "a2d.h"
#include "tspec_diff.h"
#include "mat_mul_pt2.h"

/*
 *  Good practice: This structure contains all of the "globals" each
 *  thread can access.
 *
 *  A pointer to a shared instance of this one struct is shared by all
 *  threads used for a single matrix multiply, so any values that
 *  might be modified by more than one thread should not be changed by
 *  any thread without being protected by some kind of mutex, a la
 *  "rowDone".
 */
typedef struct {
    /*
     * `rowDone` is modified by every thread, so it needs a mutex.
     */
    int rowDone; /* # of rows already done *and* the next row to be assigned */
    pthread_mutex_t rowDoneMutex; /* restricts access to `rowDone` */

    /*
     * As far as a thread is concerned, these are constants, so no
     * mutex is necessary.
     */
    int nThreads;
    int n, m, p; /* matrix dimensions */
    const double *_a, *_b; /* input (1D) matrices */

    /*
     * Each row of this matrix is modified by only one thread, so
     * again no mutex is necessary. (Note how this decision, which is
     * critical to good threaded performance, requires knowledge of
     * the algorithm.)
     */
    double *_c; /* output (1D) matrix */
} ThreadGlobals;


static void multiplyRow(double *_c, const int i, const int n, const int m,
                        const double *_a, const int p, const double *_b)
{
#define c(i,j) _c[I2D(i, n, j, m)]
#define a(i,j) _a[I2D(i, n, j, p)]
#define b(i,j) _b[I2D(i, p, j, m)]
    int j, k;
    double sum;

    sum = 0.0;
    for (j = 0; j < m; j++) {
        sum = 0.0;
        for (k = 0; k < p; k++)
            sum += a(i,k) * b(k,j);
        c(i,j) = sum;
    }
#undef a
#undef b
#undef c
}


// inThread -- function for each thread
static void *inThread(void *threadGlobals_)
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * allocate a MatMulThreadStats instance `matMulThreadStats`
     *  (hint: malloc())
     * set `matMulThreadStats->nRowsDone` to 0
     * get this thread's cpu clock id (hint: pthread_self() and
     *  pthread_getcpuclockid())
     * get the thread clock id's start cpu time (hint: clock_gettime())
     * loop the following indefinitely,
     *     if there are multiple threads,
     *         lock the "row done" mutex (hint: pthread_mutex_lock())
     *     set `i` to the current value of `threadGlobals->rowDone`
     *     increment `threadGlobals->rowDone`
     *     if there are multiple threads,
     *         unlock the "row done" thread mutex (hint: pthread_mutex_unlock())
     *     if `i` >= `n` (the number of rows),
     *         break out of the loop
     *     multiply row `i` (hint: multiplyRow())
     *     increment `matMulThreadStats->nRowsDone`
     * get the thread clock id's end cpu time (hint: clock_gettime())
     * set `matMulThreadStats->cpuTime` to the difference of thread start and
     *  stop cpu times (hint: tspecDiff())
     * return `matMulThreadStats`
     */
	clockid_t cid;
	struct timespec start;
	struct timespec end;
	double diff;
	int i;
	ThreadGlobals* tg = (ThreadGlobals*)threadGlobals_;

	MatMulThreadStats* matMulThreadStats = (MatMulThreadStats*)malloc(sizeof(MatMulThreadStats));
	matMulThreadStats->nRowsDone = 0;
	
	pthread_getcpuclockid(pthread_self(), &cid);
	clock_gettime(cid, &start);

	while(1==1)
	{
		if(tg->nThreads > 1)
		{
			pthread_mutex_lock(&tg->rowDoneMutex);
			i = tg->rowDone;
			tg->rowDone++;
			if(tg->nThreads > 1)
			{
				pthread_mutex_unlock(&tg->rowDoneMutex);
				if(i >= tg->n)
				{
					break;
				}
				multiplyRow(tg->_c, i, tg->n, tg->m, tg->_a, tg->p, tg->_b);
				matMulThreadStats->nRowsDone++;
			}
		}
	}
	clock_gettime(cid, &end);
	diff = tspecDiff(start, end);
	matMulThreadStats->cpuTime = diff;

	return matMulThreadStats;
}


void mat_mul(double *_c, const int n, const int m,
             const double *_a, const int p, const double *_b,
             const int nThreads, MatMulThreadStats matMulThreadStats[])
{
    /*
     * ASSIGNMENT
     *
     * Implement the following pseudocode:
     *
     * declare a ThreadGlobals struct and set all fields to the
     *  corresponding parameters
     * if `nThreads` > 0,
     *     malloc() an array of `nThreads` pthread_t's
     *     initialize the `rowDoneMutex` thread mutex (hint:
     *      pthread_mutex_init())
     *     for each of `nThreads` threads,
     *         create a thread calling inThread() and pass it a
     *          pointer to `threadGlobals` (hint: pthread_create())
     *     for each thread `i` of `nThreads` threads,
     *         wait for the thread to complete (hint: pthread_join())
     *         copy its MatMulThreadStats return to `matMulThreadStats[i]`
     *         free the returned struct
     *     destroy the thread mutex (hint: pthread_mutex_destroy())
     *     free the pthread_t array
     * otherwise
     *     call the thread function directly (hint: inThread())
     *     copy its MatMulThreadStats return to `matMulThreadStats[0]`
     *     free the returned struct
     */

	/*
		QUESTION
		Am I supposed to initialize the members like this?
	*/
	ThreadGlobals tg;
	tg.rowDone;
	tg.nThreads = nThreads;
	tg.n = n;
	tg.m = m;
	tg.p = p;
	tg._a = _a;
	tg._b = _b;
	tg._c = _c;

	int i = 0;

/*
typedef struct {
    int rowDone;  # of rows already done *and* the next row to be assigned
    pthread_mutex_t rowDoneMutex;  restricts access to `rowDone
    int nThreads;
    int n, m, p;  matrix dimensions 
    const double *_a, *_b;  input (1D) matrices 
    double *_c;  output (1D) matrix 
} ThreadGlobals;
*/
	
	pthread_t* ptAr;
	MatMulThreadStats** mmts;
	
	if(nThreads > 0)
	{
		ptAr = (pthread_t*)malloc(sizeof(pthread_t) * nThreads);
		pthread_mutex_init(&tg.rowDoneMutex, NULL);
		/*
			QUESTION
			Am I handling the function pointer correctly?
			How do I get the return from the function?
		*/
		for(i = 0; i < nThreads; i++)
		{
			pthread_create(&ptAr[i], NULL, inThread, (void*)&tg);
		}
		for(i = 0; i < nThreads; i++)
		{
			/*
				QUESTION
			*/
			pthread_join(&ptAr[i], mmts);
			matMulThreadStats[i].cpuTime = (*mmts)->cpuTime;
			matMulThreadStats[i].nRowsDone = (*mmts)->nRowsDone;
			free(*mmts);
		}
	}
}
