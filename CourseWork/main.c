#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define MAIN_FILE "/tmp/ticketsMainFIFO"
#define DATAFILE_NAME "data.db"

typedef struct passenger
{
	char * name;
	char * ticket;
	int alive;
	struct passenger * next;
	struct passenger * previous;
} passenger_t;

int main()
{
	FILE * mainFile;
	char readbuff[50];
	umask(0);
	mknod(MAIN_FILE, S_IFIFO|0666, 0);
	passenger_t * firstPassenger = (passenger_t *)malloc(sizeof(passenger_t));
	FILE * inputFile = fopen(DATAFILE_NAME, "r");
	if(inputFile)
	{
		char lineread[100];
		fgets(lineread, 100, inputFile);
		char ticketBuff[12];
		char nameBuff[50];
		int alive = 0;
		sscanf(lineread, "%s %s %d", ticketBuff, nameBuff, &alive); 
		firstPassenger->name = (char *)malloc(strlen(nameBuff)+1);
		strcpy(firstPassenger->name, nameBuff);
		firstPassenger->ticket = (char *) malloc(strlen(ticketBuff)+1);
		strcpy(firstPassenger->ticket, ticketBuff);
		firstPassenger->alive = alive;
		passenger_t * current = firstPassenger;
		while(1)
		{
			if(fgets(lineread, 100, inputFile));
			else break;

			current->next = (passenger_t *) malloc(sizeof(passenger_t));
			current->next->previous = current;
			current = current->next;
			printf("%s\n", lineread);
			sscanf(lineread, "%s %s %d", ticketBuff, nameBuff, &alive);
			current->name = (char *) malloc(strlen(nameBuff) + 1);
			strcpy(current->name, nameBuff);
			current->ticket = (char *) malloc(strlen(ticketBuff)+1);
			strcpy(current->ticket, ticketBuff);
			current->alive = alive;
		}
	}
	else
	{
		return -1;
	}

	while(1)
	{
		mainFile = fopen(MAIN_FILE, "r");
		memset(readbuff, 0, sizeof(readbuff));
		fgets(readbuff, sizeof(readbuff), mainFile);
		fclose(mainFile);
		if(1 || strlen(readbuff) > 0)
		{
			char fifoFileName[200];
			char ticketNumber[12];
			char fifoName[6];
			sscanf(readbuff, "%s %s", fifoName, ticketNumber);
			printf("%s %s\n", fifoName, ticketNumber);
			passenger_t * current = firstPassenger;
			sprintf(fifoFileName, "/tmp/%s.tmp", fifoName);
			printf("%s\n", fifoFileName);

			pid_t forkResult = vfork();

			if(forkResult == 0)
			{
				printf("Forked");
				unlink(fifoFileName);
				mknod(fifoFileName, 0600, 0);
				printf("Fifo created");
				FILE * fifoFile = fopen(fifoFileName, "w");
				printf("1 %s %s %s", ticketNumber, fifoName, current->ticket);
				while(1)
				{
					printf("%s %s", current->ticket, ticketNumber);
					if(strcmp(current->ticket, ticketNumber) == 0)
					{
						printf("Found");
						if(current->alive == 1)
						{
							fprintf(fifoFile,"Alive\n");
							fclose(fifoFile);
							break;
						}
						else
						{
							fprintf(fifoFile, "):\n");
							fclose(fifoFile);
							break;
						}
						exit(0);
					}
					else
					{
						if(current->next == NULL)
						{
							fclose(fifoFile);
							break;
						}
						else current = current->next;
					}
				}
			}
			else if (forkResult > 0)
			{
				printf("Parent process");
			}
		}
	}
}
