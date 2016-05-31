#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAIN_FILE "/tmp/ticketsMainFIFO"


int main(int argc, char ** argv)
{
	FILE * mainFile;
	if(mainFile = fopen(MAIN_FILE, "w"))
	{
		char dataToSend[30];
		char fileName[6];
		sprintf(fileName, "%d", getpid());
		sprintf(dataToSend, "%s %s", fileName, argv[1]);
		fputs(dataToSend, mainFile);
		fclose(mainFile);
		while(1)
		{
		usleep(200000);
		char fileName1[20];
		memset(fileName1, 0, sizeof(fileName1));
		sprintf(fileName1, "/tmp/%s.tmp", fileName);
		FILE * inputFile = fopen(fileName1, "r");
		if(inputFile)
		{
			usleep(100000);
			printf("File opened\n");
			while(1)
			{
				char outputString[20];
				memset(outputString, 0, sizeof(outputString));
				fgets(outputString, sizeof(outputString), inputFile);
				puts(outputString);
				fclose(inputFile);
				break;
			}
			break;
		}
		}
	}

	return 0;
}
	
