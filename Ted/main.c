#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define MAIN_FILE "/tmp/inputFile1.txt"
#define DATAFILE_NAME "data.db"



typedef struct item
{
    char * name;
    float price;
    struct item * next;
    struct item * previous;
} item_t;


int main(int argc, const char * argv[]) {
    FILE * mainFile;
    char readbuff[500];
    item_t * current;
    umask(0);
    mknod(MAIN_FILE, S_IFIFO|0666, 0);
    item_t * firstItem = (item_t *)malloc(sizeof(item_t));
    FILE * inputFile = fopen(DATAFILE_NAME, "r");
    if(inputFile)
    {
        char lineread[100];
        fgets(lineread, 100, inputFile);
        char nameBuff[50];
        float priceBuff = 0;
        sscanf(lineread, "%s %f", nameBuff, &priceBuff);
        firstItem->name = (char *)malloc(strlen(nameBuff)+1);
        strcpy(firstItem->name, nameBuff);
        firstItem->price = priceBuff;
        current = firstItem;
        while(1)
        {
            if(fgets(lineread, 100, inputFile));
            else
		{
			fclose(inputFile);
			break;
		}
            current->next = (item_t *) malloc(sizeof(item_t));
            current->next->previous = current;
            current = current->next;
            printf("%s", lineread);
            sscanf(lineread, "%s %f", nameBuff, &priceBuff);
            current->name = (char *) malloc(strlen(nameBuff) + 1);
            strcpy(current->name, nameBuff);
	    current->price = priceBuff;
		printf("%s %0.2f\n", current->name, current->price);
        }
	printf("Data read\n");
    }

    while(1)
    {
	mainFile = fopen(MAIN_FILE, "r");
	while(mainFile == NULL)
	{
		mainFile = fopen(MAIN_FILE, "r");
		//wait for opening the file
	}
        memset(readbuff, 0, sizeof(readbuff));
        fgets(readbuff, sizeof(readbuff), mainFile);
	fclose(mainFile);
	while(1)
        {
		printf("Waiting to read data from file ");
            char currentItem[50];
            char otherProcessId[20];
            memset(otherProcessId, 0, sizeof(otherProcessId));
            memset(currentItem, 0, sizeof(currentItem));
	char buffer[40];
		current = firstItem;
            if(sscanf(readbuff, "%s %s",otherProcessId, currentItem))
            {
		printf("Item to check price: %s\n", currentItem);
                char fifoFileName[30];
                sprintf(fifoFileName, "/tmp/%s", otherProcessId);
		printf("File name : %s\n", fifoFileName);
                pid_t forkResult = vfork();
                if(forkResult == 0)
                {
		while(1)
		{
                    //unlink(fifoFileName);
                    mknod(fifoFileName, 0600, 0);
                    printf("Fifo created");
                    FILE * fifoFile = fopen(fifoFileName, "w");
			printf("%s %s\n", current->name, currentItem);
                    if(strcmp(current->name, currentItem) == 0)
                    {
                        printf("Found");
                        fprintf(fifoFile,"%f", current->price);
                        fclose(fifoFile);
                        exit(0);
                    }
                    else
                    {
                        if(current->next == NULL)
                        {
                            fclose(fifoFile);
 			exit(0);
                        }
                        else current = current->next;
                    }
}
                }
		else break;
            }
        }
//	fclose(mainFile);
    }
    return 0;
}

