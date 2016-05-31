#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>


#define FIFO_FILE "/tmp/MYFIFO"

int main()
{
	umask(0);
	mknod(FIFO_FILE, S_IFIFO|0666, 0);
	pid_t forkResult;
	forkResult = fork();
	if(forkResult == 0)
	{
		FILE *fp;
		char readbuff[50];
		// create a FIFO buffer if it doesn't exist
		while(1)
		{
			if((fp = fopen(FIFO_FILE, "r"))!=NULL)
			{
				memset(readbuff, 0, sizeof(readbuff)/sizeof(readbuff[0]));
				fgets(readbuff, sizeof(readbuff)/sizeof(readbuff[0]), fp);
				if(strlen(readbuff) > 0)
				{
					printf("PID %d: Received string : %s\n",getpid() , readbuff);
					fclose(fp);
					fp = fopen(FIFO_FILE, "w");
				}
				fclose(fp);
			}
		}
		return 0;
	}
	else if(forkResult < 0)
	{
		return 69;
	}
	else
	{
		// parent process
		FILE * fp;
		for(;;)
		{
			if((fp = fopen(FIFO_FILE, "w")) != NULL)
			{
				fputs("Hello manyak\n", fp);
				printf("Message sent from PID:%d\n", getpid());
				fclose(fp);
			}
			usleep(5361 * 1000);
		}
	}
	return 0;
}
