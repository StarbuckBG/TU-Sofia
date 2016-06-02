#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_TO_READ "commands.txt"
#define SHELL "/bin/sh"

int main(int argc, char ** argv)
{
	FILE * file = fopen(FILE_TO_READ, "r");
	if(argv[1] != NULL)
	{
		mknod(argv[1],S_IFIFO| 0600, 0);
	}
	int currentLineNumber = 0;
	char commandBuffer[10000];
	memset(commandBuffer, 0, sizeof(commandBuffer));
	while(1)
	{
		char currentCommand[200];
		memset(currentCommand, 0, sizeof(currentCommand));
		if(fgets(currentCommand, sizeof(currentCommand), file) != NULL)
		{
			currentCommand[strlen(currentCommand)-1] = 0;
			if(argv[1] == NULL) strcat(commandBuffer, currentCommand);
		}
		else
		{
			if(argv[1] == NULL) 
				{
					commandBuffer[strlen(commandBuffer)-1] = 0;
					commandBuffer[strlen(commandBuffer)-2] = 0;
					printf("%s\n", commandBuffer);
					system(commandBuffer);
				}
			break;
		}
		if(argv[1] != NULL)
		{
			if(currentLineNumber % 2 == 0)
			{
				execl(currentCommand, ">>","/tmp/", argv[1], (char *)NULL);
			}
			else
			{
				execl(currentCommand, "<<","/tmp/", argv[1], NULL);
			}
			currentLineNumber++;
		}
		else
		{
			strcat(commandBuffer, " | ");
		}
	}
	fclose(file);
	return 0;
}
