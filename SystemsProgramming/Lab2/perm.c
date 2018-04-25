#include <stdio.h>

/*
 * WARNING: This version of the program contains at least one bug!
 */

#define N_ELEM 3
int level;
int val[N_ELEM];
enum { NOT_DONE = -1 };

void perm(int k)
{
    int i;

    val[k] = level;
    level++;

    if (level == N_ELEM) 
    {
        for (i = 0; i < N_ELEM; i++)
        {
            printf("%d ", val[i]);
        }
        printf("\n");
    }

    for (i = 0; i < N_ELEM; i++)
    {
        if (val[i] == NOT_DONE)
        {
            perm(i);
        }
    }
    val[k] = NOT_DONE;
    level--;
}

int main(int argc, char *argv[])
{
    int i;

    level = 0;
    for (i = 0; i < N_ELEM; i++)
        val[i] = NOT_DONE;

    for (i = 0; i < N_ELEM; i++)
    {
            perm(i);
    }

    return 0;
}
