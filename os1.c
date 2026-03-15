#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#define B_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main()
{
	char write_msg[B_SIZE]="Greetings";
	char read_msg[B_SIZE]="";
	int fd[2];
	pid_t pid;

	if (pipe(fd) == -1)
	{
		printf("Pipe failed!");
		return 1;
	}

	pid=fork();
	if(pid>0)
	{
		close(fd[READ_END]);
		write(fd[WRITE_END],write_msg,strlen(write_msg)+1);
		close(fd[WRITE_END]);
		wait(NULL);
	}
	else if(pid==0)
	{
		close(fd[WRITE_END]);
		read(fd[READ_END],read_msg,B_SIZE);
		printf("child process read- %s\n", read_msg);
		close(fd[READ_END]);
	}
	else
	{
		printf("Fork failed.");
		return 0;
	}
}
