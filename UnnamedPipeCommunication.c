#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


void calculate_result(int values[3]);

int main(void)
{
	int child_pipe_fd[2]; //pipe_fd[0] -> r , pipe_fd[1] -> w
	int parent_pipe_fd[2];
	pid_t child_pid;
	int pipe_status = 0;
	int child_buff[3];
	int parent_buff[3];
	pipe_status = pipe(parent_pipe_fd);
	if(pipe_status == -1)
	{
		perror("pipe()");
	}
	else
	{
		pipe_status = pipe(child_pipe_fd);
		if(pipe_status == -1)
		{
			perror("pipe()");
			return EXIT_FAILURE;
		}
		child_pid = fork();
		if(child_pid == -1)
		{
			perror("fork()");
		}
		else if(child_pid == 0)
		{
			/*child*/
			child_buff[0] = 2;
			child_buff[1] = 3;
			child_buff[2] = -5;
			close(parent_pipe_fd[0]);
			close(child_pipe_fd[1]);
			write(parent_pipe_fd[1],child_buff,sizeof(child_buff));
			close(parent_pipe_fd[1]);
			read(child_pipe_fd[0],child_buff,sizeof(child_buff));
			printf("Parent said: %d,%d,%d\n",child_buff[0],child_buff[1],child_buff[2]);
			_Exit(EXIT_SUCCESS); // or _exit()
		}
		else
		{
			/*parent*/
			close(parent_pipe_fd[1]);
			close(child_pipe_fd[0]);
			read(parent_pipe_fd[0],parent_buff,sizeof(parent_buff));
			close(parent_pipe_fd[0]);
			printf("Child said: %d,%d,%d\n",parent_buff[0],parent_buff[1],parent_buff[2]);
			calculate_result(parent_buff);
			write(child_pipe_fd[1] , parent_buff , sizeof(parent_buff));
			wait(NULL);
		}
	}
	return 0;
}

void calculate_result(int values[3])
{
	int a = values[0];
	int b = values[1];
	int c = values[2];
	int d = (b*b) - (4*a*c);
	printf("a,b,c: %d , %d , %d\n" , a,b,c);
	printf("D: %d\n" , d);
	if(d < 0)
	{
		values[0] = (int)NAN;
		values[1] = (int)NAN;
	}
	else if(d == 0)
	{
		values[0] = values[1] = (-b) / (2*a);
	}
	else
	{
		values[0] = ((-b) + sqrt(d)) / (2*a);
		values[1] = ((-b) - sqrt(d)) / (2*a);
	}
	values[2] = (int)NAN;
}
	
