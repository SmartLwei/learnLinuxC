#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	pid_t result;
	result = fork();
	int newret;
	if(result <0)
	{
		perror("创建子进程失败\n");
		exit(1);
	}
	// 子进程
	else if(result == 0)
	{
		raise(SIGSTOP);
		printf("子进程被阻塞\n");
		exit(0);
	}
	// 父进程
	else
	{
		sleep(1);
		printf("子进程的id为:%d\n",result);
		if((waitpid(result, NULL, WNOHANG)) == 0)
		{
			if(kill(result, SIGKILL) == 0)
			  printf("kill 成功\n");
			else
			  perror("kill 失败\n");
		}
	}



}


