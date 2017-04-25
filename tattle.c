#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<utmp.h>
#include<time.h>
#include <fcntl.h>

#define DBG printf("Here %d\n", ++BUGERATOR);
#define ENTRY_INCR 500 // This should be enough I think. Using this approach I can fill an array and use qsort

int BUGERATOR = 0;

struct Filter
{
	char user[500][50]; // This should be plenty
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
	int full;
};

//struct Entry EntryArray[MAX_ENTRIES];
struct Entry* EntryArray;

int WTMP_FILE_DESC;
int EntryIndex = 0;

int initFileDesc()
{
	WTMP_FILE_DESC = open("/var/log/wtmp", O_RDONLY);

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
			if(EntryArray[i].full == 0)
			{
				rVal = &EntryArray[i];
				EntryArray[i].full = 1;
			}
		}
	}

	return rVal;
}

int readEntry()
{
	int bytesRead;
	int rVal = 0;
	int i;
	struct utmp buffer;
	struct Entry* ePtr = &EntryArray[EntryIndex];
	time_t tm;

	bytesRead = read(WTMP_FILE_DESC, &buffer, sizeof(struct utmp));

	if(bytesRead == sizeof(struct utmp))
	{
		ePtr->ut_type = buffer.ut_type;
		strcpy(ePtr->tty, buffer.ut_line);
		strcpy(ePtr->user, buffer.ut_user);
		strcpy(ePtr->host, buffer.ut_host);
		tm = buffer.ut_tv.tv_sec;

		if(buffer.ut_type == USER_PROCESS)
		{
			//ePtr->loginTime = *gmtime((time_t*)&buffer.ut_tv.tv_sec);
			//printf("buffer.ut_user: %s\nePtr->user: %s\n", buffer.ut_user, ePtr->user);
			ePtr->loginTime = *localtime(&tm);
			ePtr->full = 0;
			EntryIndex++;
		}
		else if(buffer.ut_type == DEAD_PROCESS)
		{
			//printf("Dead Process\n");
			ePtr = searchByUser(ePtr->user);
			if(ePtr != NULL)
			{
				tm = buffer.ut_tv.tv_sec;
				ePtr->logoutTime = *localtime(&tm);
			}
		}
		else if(buffer.ut_type == BOOT_TIME)
		{
			//printf("Boot Time\n");
			//Log everybody out at this point
			for(i = 0; i < EntryIndex; i++)
			{
				if(EntryArray[i].full == 0)
				{
					EntryArray[i].full = 1;
					EntryArray[i].logoutTime = *localtime(&tm);
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
	strftime(timeBuff, sizeof(timeBuff), "%D %H:%M", &ePtr->loginTime); 
	strcat(output, timeBuff);
	strcat(output, "     ");
	//printf("%s\n", timeBuff);
	if(ePtr->full != 0)
	{
		strftime(timeBuff, sizeof(timeBuff), "%D %H:%M", &ePtr->logoutTime);
		//printf("%s\n", timeBuff);
		strcat(output, timeBuff);
	}
	else
	{
		//printf("(still logged in)\n");
		strcat(output, "          (still logged in)");
	}
	strcat(output, "     ");
	strcat(output, ePtr->host);
	
	printf("%s\n", output);

	return;
}

int cmpFunc(const void* alpha, const void* beta)
{
	int rVal;

	time_t aTime = mktime(&((struct Entry*)alpha)->loginTime);
	time_t bTime = mktime(&((struct Entry*)beta)->loginTime);

	if(aTime < bTime)
	{
		rVal = -1;
	}
	else if(aTime > bTime)
	{
		rVal = 1;
	}
	else
	{
		rVal = 0;
	}

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

	sFilter.time.tm_mon = (atoi(month) - 1);
	sFilter.time.tm_mday = atoi(day);
	if(atoi(year) < 17)
	{
		sFilter.time.tm_year = (atoi(year) + 100);
	}
	else
	{
		sFilter.time.tm_year = atoi(year);
	}
}

void parseTime(char* time)
{
	char hour[3];
	char min[3];
	char timeBuff[50];

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

void initFilter()
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

		tempA->tm_hour = 0;
		tempA->tm_min = 0;
		tempB->tm_hour = 0;
		tempB->tm_min = 0;

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

int main(int argc, char* argv[])
{
	int i;
	struct Entry* tempArray;
	int optionLetter;
	sFilter.userNum = 0;
	char timeBuff[20];
	struct tm* curTime;
	time_t curTime_t;

	while((optionLetter = getopt(argc, argv, "u:d:t:")) != -1)
        {
                switch(optionLetter)
                {
		case 'u':
			parseUsers(optarg);
			sFilter.enableUserFiltering = 1;
                        break;
                case 'd':
			parseDate(optarg);
			sFilter.enableDayFiltering = 1;
                        break;
                case 't':
			parseTime(optarg);
			sFilter.enableTimeFiltering = 1;
                        break;
                default:
			break;
                }
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
		//printEntry(&EntryArray[EntryIndex - 1]);
		if(EntryIndex + 1 % ENTRY_INCR == 0)
		{
			tempArray = (struct Entry*)malloc(sizeof(struct Entry) * ((EntryIndex + 1) + (ENTRY_INCR)));
			for(i = 0; i < EntryIndex; i++)
			{
				tempArray[i] = EntryArray[i];
			}
			free(EntryArray);
			EntryArray = tempArray;
		}
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
