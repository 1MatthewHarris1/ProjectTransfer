#include <stdio.h>
#include <assert.h> // for assert()
#include <ctype.h> // for isspace()

typedef struct {
    int lineCount;
    int linesWithCodeCount;
    int foundCodeOnLine;
    int cplusplusCommentCount;
    int cCommentCount;
} CodeStats;


void codeStats_init(CodeStats *codeStats)
{
    codeStats->lineCount = 0;
    codeStats->linesWithCodeCount = 0;
    codeStats->foundCodeOnLine = 0;
    codeStats->cplusplusCommentCount = 0;
    codeStats->cCommentCount = 0;
}


void codeStats_print(CodeStats codeStats, char *fileName)
{
    printf("           file: %s\n", fileName);
    printf("     line count: %d\n", codeStats.lineCount);
    printf("     lines with code: %d\n", codeStats.linesWithCodeCount);
    printf("     c++ comments found: %d\n", codeStats.cplusplusCommentCount);
    printf("     c comments found: %d\n", codeStats.cCommentCount);
}


void codeStats_accumulate(CodeStats *codeStats, char *fileName)
{
    FILE *f = fopen(fileName, "r");
    int ch;
    enum {
        START,
        FOUNDSLASH, // He's playing guitar for Guns n' Roses
        FOUNDCPPCOMMENT,
        FOUNDOPENSTAR,
        FOUNDCLOSESTAR,
        FOUNDSTRING,
        FOUNDESCAPEINSTRING,
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
        else if(ch == '\"')
        {
            state = FOUNDSTRING;
            codeStats->foundCodeOnLine = 1;
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
        else if(ch == '*')
        {
            state = FOUNDOPENSTAR;
            codeStats->cCommentCount++;
        }
        else if(ch == '\"')
        {
            state = FOUNDSTRING;
            codeStats->foundCodeOnLine = 1;
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

        case FOUNDOPENSTAR:
        if(ch == '\n')
        {
            codeStats->lineCount++;
            codeStats->linesWithCodeCount += codeStats->foundCodeOnLine;
            codeStats->foundCodeOnLine = 0;
        }
        else if(ch == '*')
        {
            state = FOUNDCLOSESTAR;
        }
        else
        {
            codeStats->foundCodeOnLine = 1;
        }

        break;
    
        case FOUNDCLOSESTAR:
        if(ch == '\n')
        {
            codeStats->lineCount++;
            codeStats->linesWithCodeCount += codeStats->foundCodeOnLine;
            codeStats->foundCodeOnLine = 0;
        }
        else if(ch == '/')
        {
            state = START;
        }
        else
        {
            state = FOUNDOPENSTAR;
        }
        break;

        case FOUNDSTRING:
        if(ch == '\n')
        {
            codeStats->lineCount++;
            codeStats->linesWithCodeCount += codeStats->foundCodeOnLine;
            codeStats->foundCodeOnLine = 0;
        }    
        else if(ch == '\"')
        {
            state = START;
        }
        else if(ch == '\\')
        {
            state = FOUNDESCAPEINSTRING;
        }
        else
        {
            codeStats->foundCodeOnLine = 1;
        }
        break;
        
        case FOUNDESCAPEINSTRING:

        if(ch == '\n')
        {
            codeStats->lineCount++;
            codeStats->linesWithCodeCount += codeStats->foundCodeOnLine;
            codeStats->foundCodeOnLine = 0;
        }
        state = FOUNDSTRING;
        
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
