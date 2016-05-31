#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_TO_READ "commands.txt"

int main()
{
	FILE * file = fopen(FILE_TO_READ, "r");
	while(1)
	{
		char currentCommand[200];
		memset(currentCommand, 0, sizeof(currentCommand));
		if(fgets(currentCommand, sizeof(currentCommand), file) != NULL) system(currentCommand);
		else
		break;
	}
	fclose(file);
	return 0;
}
