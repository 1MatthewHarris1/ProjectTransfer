#include <stdio.h>
#include<stdlib.h>
#include "./gen_perms.h"

int level;
enum { NOT_DONE = -1 };

void dewIt(int k, int nElems,int val[], void (*handlePerm) (int elems[], int nElems, void *userArg), void *userArg);

void genPerms(int nElems, void (*handlePerm) (int elems[], int nElems, void *userArg), void *userArg)
{	
    int i;
    level = 0;
    int val[nElems];

    for(i = 0; i < nElems; i++)
    {
        val[i] = NOT_DONE;
    }

    for (i = 0; i < nElems; i++)
    {
        dewIt(i, nElems, val,handlePerm, userArg);
    }
    
}
void dewIt(int k, int nElems, int val[], void (*handlePerm) (int elems[], int nElems, void *userArg), void *userArg)
{
    int i;

    val[k] = level;
    level++;

    if(level == nElems)
    {
        handlePerm(val, nElems, userArg);
    }

    for (i = 0; i < nElems; i++)
    {
        if (val[i] == NOT_DONE)
        {
	    dewIt(i, nElems, val,handlePerm, userArg);
        }
    }

    val[k] = NOT_DONE;
    level--;
}
