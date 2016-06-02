#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_TO_READ "commands.txt"

int main(int argc, char ** argv)
{
	FILE * file = fopen(FILE_TO_READ, "r");
	if(argv[1] != NULL)
	{
		execl("mkfifo", argv[1]);
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
			currentCommand[strlen(currentCommand)-1] = 'a';
			if(argv[1] == NULL) strcat(commandBuffer, currentCommand);
		}
		else
		{
			if(argv[1] == NULL) 
				{
					printf("%s\n", commandBuffer);
					system(commandBuffer);
				}
			break;
		}
		if(argv[1] != NULL)
		{
			if(currentLineNumber % 2 == 0)
			{
				execl(currentCommand, ">>", argv[1]);
			}
			else
			{
				execl(currentCommand, "<<", argv[1]);
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
