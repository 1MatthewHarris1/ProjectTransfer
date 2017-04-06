#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_SIZE 15
#define FILM_SIZE 50
#define WEAKNESS_SIZE 25
#define DEFEATEDBY_SIZE 30
#define MAX_MONSTERS 1024 /* should be big enough */
#define TRUE 1
#define FALSE 0
#define DBG printf("Here %d\n", BUGERATOR++);

int BUGERATOR = 0;

typedef struct Monster 
{
	char name[NAME_SIZE];   
	char film[FILM_SIZE];	
	int year;
	char weakness[WEAKNESS_SIZE];
	char defeatedBy[DEFEATEDBY_SIZE];
	double rating;

} Monster;


struct Monster monsters[MAX_MONSTERS];




void printCsvHeader(void)
/* print a CSV-compatible "monster" header to standard output */
{
	printf("Name,");
	printf("Film,");
	printf("Year,");
	printf("Weakness,");
	printf("Defeated By,");
	printf("Rating\n");
    
}

void printCsvRow(struct Monster monster)
/* print a CSV-compatible "monster" row to standard output */
{
	printf("%s, ", monster.name);
	printf("%s, ", monster.film);
	printf("%i, ", monster.year);
	printf("%s, ", monster.weakness);
	printf("%s, ", monster.defeatedBy);
	printf("%.1lf\n", monster.rating);
}

void saveDatabase(struct Monster monsters[])
{


	struct Monster *monster_p;
	printCsvHeader();

	for(monster_p = &monsters[1]; monster_p->name[0] != '\0'; monster_p++)  //PROBLEM RIGHT HERE?!?!?!?!?!?!??!?!?!??!?!?!?!?!?!??!?!?!?!?!??!
	{
		printCsvRow(*monster_p);
	}

}

int foundColumnName(char columnName[], char followedBy)
/* helper function used to read an expected column name */
{
    char buffer[2048]; /* longer than the longest possible column name */
    /*
     * The "%[^,\n]s" format string tells scanf() to accept anything
     * that's not a ',' or a '\n' as part of a string. Those two
     * characters *separate* data in a CSV file.
     *
     * After reading and verifying the column name, we call getchar()
     * to verify that it's followed by the char `followedBy`,
     * returning 0 if any of these errors are detected.
     */
    if (scanf("%[^,\n]s",  buffer) != 1     /* didn't read one item */
        || strcmp(buffer, columnName) != 0  /* doesn't match `columnName` */
        || getchar() != followedBy) {       /* not followed by expected char */
        return 0;
    }
    return 1;
}


int scanCsvHeader(void)
/* scans a CSV-compatible "monster" header for compatibility */
{
   	if (!foundColumnName("Name", ','))
        	return 0; 
	if (!foundColumnName("Film", ','))
        	return 0;
        if (!foundColumnName("Year", ','))
        	return 0;
	if (!foundColumnName("Weakness", ','))
		return 0;
	if (!foundColumnName("Defeated by", ','))
		return 0;
	if(!foundColumnName("Rating", '\n'))
		return 0;
	return 1;
}


int scanCsvRow(struct Monster *monster)
/* reads a Monster row from a CSV file */
{

  	if (scanf("%[^,\n]s",  monster->name) != 1)
        	return 0;
   	getchar(); /* skip the following comma or newline (we could check this) */

	if (scanf("%[^,\n]s", monster->film) != 1)
		return 0;
	getchar();

	if (scanf("%i", &monster->year) != 1)
		return 0;
	getchar();

	if (scanf("%[^,\n]s", monster->weakness) != 1)
		return 0;
	getchar();
	
	if (scanf("%[^,\n]s", monster->defeatedBy) != 1)
		return 0;
	getchar();
	
	if (scanf("%lf", &monster->rating) != 1)
		return 0;
	getchar();

	return 1;

}


int loadDatabase(void)
/* reads a CSV monster database on standard input, returns the number of members */
{
	int count;
	int endOfLine = FALSE;
	
	if(scanCsvHeader() == 0)
	{
		return 0;
	}
	count = 0;
	while(!endOfLine)
	{
		if(scanCsvRow(&monsters[count]) == 0)
		{
			endOfLine = TRUE;
		}
		
		count++;
	}

	monsters[count].name[0] = '\0';
	return count;
}


int compareMonstersByYear(const void *monster0_vp, const void *monster1_vp)
{
    /*
     * Don't worry about this function for now. All you need to know
     * is that it's used by `qsort()` for sorting.
     */
    const struct Monster *monster0_p = monster0_vp;
    const struct Monster *monster1_p = monster1_vp;

    /* sort monsters by year of film release */
    return monster0_p->year - monster1_p->year;
}


int main(void)
{
    int monsterCount;

    monsterCount = loadDatabase();

    if (monsterCount > 0) 
    {
        qsort(monsters, monsterCount, sizeof(struct Monster),
              compareMonstersByYear);
        saveDatabase(monsters);
    }
    return 0;
}

