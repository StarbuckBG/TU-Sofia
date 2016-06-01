#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAIN_FILE "/tmp/inputFile1.txt"


int main(int argc, const char * argv[]) {

while(1)
{
    FILE * mainFile;
	printf("Starting\n");
        char dataToSend[30];
        char fileName[6];
        sprintf(fileName, "%d", getpid());
        float sum = 0;
        while(1)
        {
		printf("Begin\n");
            char productCode[20];
            memset(productCode, 0, sizeof(productCode));
	    scanf("%s", productCode);
            if(strcmp(productCode, "!!!") == 0)
            {
                printf("Sum %0.2f\n", sum);
            }
            else
            {
		while(mainFile == NULL) mainFile = fopen(MAIN_FILE, "w");
                sprintf(dataToSend, "%d %s\n", getpid(), productCode);
		fputs(dataToSend, mainFile);
		fclose(mainFile);
		printf("Close mainFile\n");
                usleep(200000);
                char fileName1[30];
                memset(fileName1, 0, sizeof(fileName1));
                sprintf(fileName1, "/tmp/%s", fileName);
		printf("%s\n", fileName1);
               	FILE * inputFile = fopen(fileName1, "r");
		while(inputFile == NULL) inputFile = fopen(fileName1, "r");
		if(inputFile)
                {
                    printf("File opened\n");
                    while(1)
                    {
                        char outputString[200];
                        memset(outputString, 0, sizeof(outputString));
                        if(!fgets(outputString, sizeof(outputString), inputFile))
			{
				if(inputFile)fclose(inputFile);
				 break;
			}

			float tempSum = 0;
			printf("String from main %s\n", outputString);
                        sscanf(outputString, "%f", &tempSum);
                        sum+=tempSum;
                    }
                }
            }
        }
    }
    return 0;
}

