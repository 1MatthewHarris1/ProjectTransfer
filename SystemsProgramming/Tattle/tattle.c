#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<utmp.h>
#include<time.h>
#include <fcntl.h>
#include<pwd.h>

#define DBG printf("Here %d\n", ++BUGERATOR);
#define ENTRY_INCR 100000 // This should be enough I think. Using this approach I can fill an array and use qsort

int BUGERATOR = 0;

/*
Steps:

First, populate the list. 
Assume that logins happen before logouts.
Only create entries on logins.
Match dead processes to logins to "fill" the login.
Upon finding a match, set the logout time.
Once all entries are read, sort the array using qsort
Determine filter settings
Print to screen each element of the array which passes the filter settings.
*/

struct Filter
{
	char user[10000][50]; // This should be plenty
	int userNum;
	int enableUserFiltering;
	int enableDayFiltering;
	int enableTimeFiltering;
	int hour;
	int min;
	struct tm time;
}sFilter;

struct Entry
{
	short ut_type;
	char tty[UT_LINESIZE];
	char user[UT_LINESIZE];
	char host[UT_HOSTSIZE];
	struct tm loginTime;
	struct tm logoutTime;
	//int full;
	int loginTimeSet;
	int logoutTimeSet;
};

//struct Entry EntryArray[MAX_ENTRIES];
struct Entry* EntryArray;

int WTMP_FILE_DESC;
int EntryIndex = 0;

int initFileDesc(void)
{
	WTMP_FILE_DESC = open("/var/log/wtmp.1", O_RDONLY);

	return 0;
}
struct Entry* searchByUser(char* name)
{
	struct Entry* rVal = NULL;
	int i;

	for(i = 0; i < EntryIndex; i++)
	{
		if(strcmp(EntryArray[i].user, name) == 0)
		{
			if(EntryArray[i].logoutTimeSet == 0) //?
			{
				rVal = &EntryArray[i];
				EntryArray[i].logoutTimeSet = 1; //?
			}
		}
	}

	return rVal;
}

int readEntry(void)
{
	int bytesRead;
	int rVal = 0;
	int i;
	struct utmp buffer;
	struct Entry* ePtr = &EntryArray[EntryIndex];
	time_t tm;
	//char timeBuff[50];

	bytesRead = read(WTMP_FILE_DESC, &buffer, sizeof(struct utmp));

	if(bytesRead == sizeof(struct utmp))
	{
		ePtr->ut_type = buffer.ut_type;
		strcpy(ePtr->tty, buffer.ut_line);
		strcpy(ePtr->user, buffer.ut_user);
		strcpy(ePtr->host, buffer.ut_host);
		tm = buffer.ut_tv.tv_sec;

		/*
		strftime(timeBuff, sizeof(timeBuff), "%D %H:%M", localtime(&tm));
		printf("I just read: %s\n", timeBuff);
		//getchar();
		*/

		//printf("Buffer: %s\n", buffer.ut_user);

		if(buffer.ut_type == USER_PROCESS)
		{
			//printf("User Process: %s\t%s\n", buffer.ut_user, buffer.ut_line);
			//getchar();
			//ePtr->loginTime = *gmtime((time_t*)&buffer.ut_tv.tv_sec);
			//printf("buffer.ut_user: %s\nePtr->user: %s\n", buffer.ut_user, ePtr->user);
	
			/* SAVE THIS
			ePtr->loginTime = *localtime(&tm);
			ePtr->full = 0;
			EntryIndex++;
			*/

			//foundMatch = 0;
			ePtr->loginTime = *localtime(&tm);
			ePtr->loginTimeSet = 1;
			EntryArray[EntryIndex] = *ePtr;
			EntryIndex++;


			/*
			for(i = 0; i < EntryIndex; i++)
			{
				if(strcmp(EntryArray[i].tty, buffer.ut_line) == 0)
				{
					if(EntryArray[i].full == 0)
					{
						EntryArray[i].full = 1;
						EntryArray[i].loginTime = *localtime(&tm);
						i = EntryIndex + 1;
						foundMatch = 1;
					}
				}
			}

			if(foundMatch == 0)
			{
				ePtr->loginTime = *localtime(&tm);
				ePtr->full = 0;
				ePtr->loginTimeSet = 1;
				ePtr->logoutTimeSet = 0;
				EntryIndex++;
			}
			*/

			/*
			ePtr = searchByUser(ePtr->user);
			if(ePtr != NULL)
			{
				tm = buffer.ut_tv.tv_sec;
				ePtr->loginTime = *localtime(&tm);
			}
			*/

		}
		else if(buffer.ut_type == DEAD_PROCESS)
		{
			//printf("user: %s\n", ePtr->user);
			//getchar();

			//printf("Dead process: %s\n", buffer.ut_line);
			//getchar();

			//foundMatch = 0;
			for(i = 0; i < EntryIndex; i++)
			{
				if(strcmp(EntryArray[i].tty, buffer.ut_line) == 0)
				{
					if(EntryArray[i].logoutTimeSet == 0) // ?
					{
						EntryArray[i].logoutTimeSet = 1; // ?
						EntryArray[i].logoutTime = *localtime(&tm);
						i = EntryIndex + 1; // break out of loop
						//foundMatch = 1;
					}
				}
			}
			
			/*
			if(foundMatch == 0)
			{
				ePtr->logoutTime = *localtime(&tm);
				ePtr->logoutTimeSet = 1;
				ePtr->loginTimeSet = 0;
				ePtr->full = 0;
				EntryIndex++;
			}
			*/

			/*SAVE THIS
			ePtr = searchByUser(ePtr->user);
			if(ePtr != NULL)
			{
				tm = buffer.ut_tv.tv_sec;
				ePtr->logoutTime = *localtime(&tm);
			}
			*/
		}
		else if(buffer.ut_type == BOOT_TIME)
		{
			//printf("Boot Time\n");
			//Log everybody out at this point
			for(i = 0; i < EntryIndex; i++)
			{
				if(EntryArray[i].logoutTimeSet == 0)
				{
					EntryArray[i].logoutTime = *localtime(&tm);
					EntryArray[i].logoutTimeSet = 1;
				}
			}
		}
	}
	else
	{
		rVal = -1;
	}
	
	return rVal;
}
void printEntry(struct Entry* ePtr)
{
	char timeBuff[50];
	char output[200];
	//printf("%s\t%s\t", ePtr->user, ePtr->tty);
	snprintf(output, 200, "%-20s%-10s", ePtr->user, ePtr->tty);
	if(ePtr->loginTimeSet == 1)
	{
		strftime(timeBuff, sizeof(timeBuff), "%D %H:%M", &ePtr->loginTime); 
		strcat(output, timeBuff);
	}
	strcat(output, "     ");
	//printf("%s\n", timeBuff);
	if(ePtr->logoutTimeSet == 1)
	{
		strftime(timeBuff, sizeof(timeBuff), "%D %H:%M", &ePtr->logoutTime);
		//printf("%s\n", timeBuff);
		strcat(output, timeBuff);
	}
	else
	{
		//printf("(still logged in)\n");
		strcat(output, "(still logged in)");
	}
	strcat(output, "     ");
	strcat(output, ePtr->host);
	
	printf("%s\n", output);

	return;
}

int cmpFunc(const void* alpha, const void* beta)
{
	int rVal = -1;;
	time_t aTime;
	time_t bTime;
	double dif;

	if(((struct Entry*)alpha)->loginTimeSet == 1)
	{
		aTime = mktime(&((struct Entry*)alpha)->loginTime);
	}
	else
	{
		rVal = 0;
	}

	if(((struct Entry*)beta)->loginTimeSet == 1)
	{
		bTime = mktime(&((struct Entry*)beta)->loginTime);
	}
	else
	{
		rVal = 0;
	}

	if(rVal == -1)
	{
		dif = difftime(aTime, bTime);
	}

	if(dif < 0)
	{
		rVal = -1;
	}
	else if(dif > 0)
	{
		rVal = 1;
	}
	else
	{
		rVal = 0;
	}

/*
	aTime = mktime(&((struct Entry*)alpha)->loginTime);
	bTime = mktime(&((struct Entry*)beta)->loginTime);

	if(aTime - bTime > 0)
	{
		rVal = 1;
	}
	else if(aTime - bTime < 0)
	{
		rVal = -1;
	}
	else
	{
		rVal = 0;
	}
*/

	return rVal;
}

void parseUsers(char* uList)
{
	int i;
	char tUser[50];
	int tUserIndex = 0;

	for(i = 0; i < strlen(uList); i++)
	{
		tUser[tUserIndex] = uList[i];
		tUserIndex++;
		if(uList[i] == ',' || i == strlen(uList) - 1)
		{
			if(uList[i] == ',')
			{
				tUser[tUserIndex - 1] = '\0';
			}
			else
			{
				tUser[tUserIndex] = '\0';
			}
			if(getpwnam(tUser) == NULL)
			{
				fprintf(stderr, "Error! Invalid user \"%s\"\n", tUser);
				exit(-1);
			}
			strcpy(sFilter.user[sFilter.userNum], tUser);
			sFilter.userNum++;
			tUserIndex = 0;
		}
	}

	return;
}

void parseDate(char* date)
{
	//char timeBuff[50];

	strptime(date, "%D", &sFilter.time);
	//strftime(timeBuff, sizeof(timeBuff), "%D", &sFilter.time);
	//printf("timeBuff: %s\n", timeBuff);

	/*
	char month[3];
	char day[3];
	char year[3];

	//I'm sure there's a much better way to do this
	month[0] = date[0];
	month[1] = date[1];
	month[2] = '\0';

	day[0] = date[3];
	day[1] = date[4];
	day[2] = '\0';

	year[0] = date[6];
	year[1] = date[7];
	year[2] = '\0';
	*/

	//sFilter.time.tm_mon = (atoi(month) - 1);
	//sFilter.time.tm_mday = atoi(day);
	/*
	if(atoi(year) < 17)
	{
		//sFilter.time.tm_year = (atoi(year) + 100);
	}
	else
	{
		//sFilter.time.tm_year = atoi(year);
	}
	*/
}

void parseTime(char* time)
{
	char hour[3];
	char min[3];
	//char timeBuff[50];

	hour[0] = time[0];
	hour[1] = time[1];
	hour[2] = '\0';

	min[0] = time[3];
	min[1] = time[4];
	min[2] = '\0';

	sFilter.hour = atoi(hour);
	sFilter.min = atoi(min);
	
	return;
}

void initFilter(void)
{
	sFilter.userNum = 0;
	sFilter.enableUserFiltering = 0;
	sFilter.enableDayFiltering = 0;
	sFilter.enableTimeFiltering = 0;
	sFilter.time.tm_hour = 0;
	sFilter.time.tm_min = 0;
}

int isFilterUser(char* name)
{
	int rVal = -1;
	int i;

	for(i = 0; i < sFilter.userNum; i++)
	{
		if(strcmp(sFilter.user[i], name) == 0)
		{
			rVal = 0;
		}
	}

	return rVal;
}

int entryFilter(struct Entry* ePtr)
{
	int rVal = -1; // return 0 if entry meets filter criteria, -1 otherwise
	int byName = -1;
	int byDate = -1;
	int byTime = -1;
	struct tm* tempA;
	struct tm* tempB;
	time_t alpha;
	time_t beta;
	double dif;
	//char timeBuff[50];

	//printf("[\n");

	if(sFilter.enableUserFiltering == 1)
	{
		byName = isFilterUser(ePtr->user);
	}
	else
	{
		//printf("Works by name\n");
		byName = 0;
	}

	if(sFilter.enableDayFiltering == 1)
	{
		tempA = (struct tm*)malloc(sizeof(struct tm));
		tempB = (struct tm*)malloc(sizeof(struct tm));

		tempA->tm_mon = ePtr->loginTime.tm_mon;
		tempA->tm_mday = ePtr->loginTime.tm_mday;
		tempA->tm_year = ePtr->loginTime.tm_year;

		tempB->tm_mon = sFilter.time.tm_mon;
		tempB->tm_mday = sFilter.time.tm_mday;
		tempB->tm_year = sFilter.time.tm_year;

		/*
		tempA->tm_hour = 0;
		tempA->tm_min = 0;
		tempB->tm_hour = 0;
		tempB->tm_min = 0;
		*/

		/*
		if(tempA->tm_year > 100)
		{
			tempA->tm_year -= 100;
		}
		*/

		alpha = mktime(tempA);
		beta = mktime(tempB);
		dif = difftime(alpha, beta);

		printf("alpha: %ld\nbeta: %ld\n", alpha, beta);
		if(dif == 0.0)
		{
			byDate = 0;
		}
		else
		{
			printf("dif: %lf\n\n", dif);
		}

		/*
		strftime(timeBuff, sizeof(timeBuff), "%D", &ePtr->loginTime);
		printf("ePtr->loginTime: %s\n", timeBuff);
		printf("A: %d/%d/%d vs ", tempA->tm_mon, tempA->tm_mday, tempA->tm_year);
		printf("B: %d/%d/%d\n", tempB->tm_mon, tempB->tm_mday, tempB->tm_year);
		//getchar();
		*/

		/*
		if(tempA->tm_mon == tempB->tm_mon)
		{
			if(tempA->tm_mday == tempB->tm_mday)
			{
				if(tempA->tm_year == tempB->tm_year)
				{
					//printf("Works by date\n");
					byDate = 0;
				}
			}
		}
		*/
		/*
		time_t alpha = mktime(tempA);
		time_t beta = mktime(tempB);
		double dif = difftime(alpha, beta);

		if(dif == 0.0)
		{
			byDate = 0;
		}
		else
		{
			printf("dif: %lf\n", dif);
		}
		*/

		free(tempA);
		free(tempB);
	}
	else
	{
		//printf("Works by date\n");
		byDate = 0;
	}

	if(sFilter.enableTimeFiltering == 1)
	{
		tempA = (struct tm*)malloc(sizeof(struct tm));
		tempB = (struct tm*)malloc(sizeof(struct tm));
		int A;
		int B;

		//struct tm* current = localtime(time(NULL));

		tempA->tm_min = ePtr->loginTime.tm_min;
		tempA->tm_hour = ePtr->loginTime.tm_hour;

		tempB->tm_min = sFilter.min;
		tempB->tm_hour = sFilter.hour;
		

		A = (100 * tempA->tm_hour) + tempA->tm_min;
		B = (100 * tempB->tm_hour) + tempB->tm_min;
		//printf("%d vs %d\n", A, B);

		if(A < B)
		{
			//printf("%d < %d\n", A, B);
			tempA->tm_min = ePtr->logoutTime.tm_min;
			tempA->tm_hour = ePtr->logoutTime.tm_hour;
			A = (100 * tempA->tm_hour) + tempA->tm_min;

			if(ePtr->logoutTime.tm_mday == ePtr->loginTime.tm_mday)
			{
				if(A > B)
				{
					//printf("Works by time\n");
					byTime = 0;
				}
				else
				{
					//printf("%d < %d\n", A, B);
				}
			}
			else
			{
				//printf("Works by time\n");
				byTime = 0;
			}
		}
		free(tempA);
		free(tempB);
	}
	else
	{
		sFilter.time.tm_min = 0;
		sFilter.time.tm_hour = 0;

		byTime = 0;
	}

	if((byDate + byTime + byName) == 0)
	{
		rVal = 0;
	}

	//printf("]\n");

	return rVal;
}

int checkDate(char* date)
{
	int rVal = 0;

	if(strlen(date) != 8)
	{
		rVal = -1;
	}
	else
	{
		if(date[2] != '/' || date[5] != '/')
		{
			rVal = -1;
		}
	}

	return rVal;
}

int checkTime(char* time)
{
	int rVal = 0;

	if(strlen(time) != 5)
	{
		rVal = -1;
	}
	else
	{
		if(time[2] != ':')
		{
			rVal = -1;	
		}
	}

	return rVal;
}

int main(int argc, char* argv[])
{
	int i;
	//struct Entry* tempArray;
	int optionLetter;
	sFilter.userNum = 0;
	char timeBuff[20];
	struct tm* curTime;
	time_t curTime_t;
	int expArg = 1;

	while((optionLetter = getopt(argc, argv, "u:d:t:")) != -1)
        {
                switch(optionLetter)
                {
		case 'u':
			parseUsers(optarg);
			sFilter.enableUserFiltering = 1;
			expArg += 2;
                        break;
                case 'd':
			if(checkDate(optarg) == 0)
			{
				parseDate(optarg);
				sFilter.enableDayFiltering = 1;
			}
			else
			{
				fprintf(stderr, "Error! Invalid date\n");
				exit(-1);
			}
			expArg += 2;
                        break;
                case 't':
			if(checkTime(optarg) == 0)
			{
				parseTime(optarg);
				sFilter.enableTimeFiltering = 1;
			}
			else
			{
				fprintf(stderr, "Error! Invalid time\n");
				exit(-1);
			}
			expArg += 2;
                        break;
                default:
			break;
                }
        }

	if(argc != expArg)
	{
		fprintf(stderr, "Error! Invalid number of program arguments\n");
		exit(-1);
	}
	
	curTime_t = time(NULL);
	curTime = localtime(&curTime_t);

	if(sFilter.enableDayFiltering == 0)
	{
		if(sFilter.enableTimeFiltering == 1)
		{
			strftime(timeBuff, sizeof(timeBuff), "%D", curTime); 
			sFilter.enableDayFiltering = 1;
			parseDate(timeBuff);
		}
	}
	else
	{
		if(sFilter.enableTimeFiltering == 0)
		{  
			strftime(timeBuff, sizeof(timeBuff), "%H:%M", curTime); 
			sFilter.enableTimeFiltering = 1;
			parseTime(timeBuff);
		}
	}

	EntryArray = (struct Entry*)malloc(sizeof(struct Entry) * ENTRY_INCR);
	initFileDesc();

	while(readEntry() != -1)
	{
		/*
		//printEntry(&EntryArray[EntryIndex - 1]);
		if(EntryIndex + 1 % ENTRY_INCR == 0)
		{
			printf("Expanding array size...\n");

			tempArray = (struct Entry*)malloc(sizeof(struct Entry) * ((EntryIndex + 1) + (ENTRY_INCR)));
			for(i = 0; i < EntryIndex; i++)
			{
				tempArray[i] = EntryArray[i];
			}
			free(EntryArray);
			EntryArray = tempArray;
		}
		*/
	}

	qsort(EntryArray, EntryIndex, sizeof(struct Entry), cmpFunc);

	printf("%-20s%-10s%-19s%-19s%-10s\n", "login", "tty", "log on", "log off", "from host");
	for(i = 0; i < EntryIndex; i++)
	{
		if(entryFilter(&EntryArray[i]) == 0)
		{
			printEntry(&EntryArray[i]);
		}
	}

	free(EntryArray);

	return 0;
}
