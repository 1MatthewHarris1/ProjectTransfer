#include <stdio.h>
#include <assert.h> // for assert()
#include <ctype.h> // for isspace()

typedef struct {
    int lineCount;
    int linesWithCodeCount;
    int foundCodeOnLine;
    int cplusplusCommentCount;
} CodeStats;


void codeStats_init(CodeStats *codeStats)
{
    codeStats->lineCount = 0;
    codeStats->linesWithCodeCount = 0;
    codeStats->foundCodeOnLine = 0;
    codeStats->cplusplusCommentCount = 0;
}


void codeStats_print(CodeStats codeStats, char *fileName)
{
    printf("           file: %s\n", fileName);
    printf("     line count: %d\n", codeStats.lineCount);
    printf("     lines with code: %d\n", codeStats.linesWithCodeCount);
    printf("     c++ comments found: %d\n", codeStats.cplusplusCommentCount);
}


void codeStats_accumulate(CodeStats *codeStats, char *fileName)
{
    FILE *f = fopen(fileName, "r");
    int ch;
    enum {
        START,
        FOUNDSLASH, // He's playing guitar for Guns n' Roses
        FOUNDCPPCOMMENT,
    } state = START;

    assert(f);
    while ((ch = getc(f)) != EOF) {
        switch (state) {

        case START:
            if (ch == '\n') {
            codeStats->lineCount++;
            codeStats->linesWithCodeCount += codeStats->foundCodeOnLine;
            codeStats->foundCodeOnLine = 0;
            }
        else if(ch == '/')
        {
            state = FOUNDSLASH;
        }
        else if(isspace(ch) != 0)
        {
            codeStats->foundCodeOnLine = 1;
        }

            break;

        case FOUNDSLASH:
        if(ch == '/')
        {
            state = FOUNDCPPCOMMENT;
            codeStats->cplusplusCommentCount++;
        }
        else
        {
            codeStats->foundCodeOnLine = 1;
            state = START;
        }

        if(ch == '\n')
        {
            codeStats->lineCount++;
            codeStats->linesWithCodeCount += codeStats->foundCodeOnLine;
            codeStats->foundCodeOnLine = 0;
        }
        break;

        case FOUNDCPPCOMMENT:
        if(ch == '\n')
        {
            codeStats->lineCount++;
            state = START;
        }
        break;
        
        default:
            assert(0);
            break;
        }
    }
    fclose(f);
    assert(state == START);
}


int main(int argc, char *argv[])
{
    CodeStats codeStats;
    int i;

    for (i = 1; i < argc; i++) {
        codeStats_init(&codeStats);
        codeStats_accumulate(&codeStats, argv[i]);
        codeStats_print(codeStats, argv[i]); // no "&" -- see why?
        if (i != argc-1)   // if this wasn't the last file ...
            printf("\n");  // ... print out a separating newline
    }
    return 0;
}
