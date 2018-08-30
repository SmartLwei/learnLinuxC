/*
避免僵尸进程的方法2
fork()两次
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	
	pid = fork();
	if(pid<0)
	{
		perror("fork failed");
		return -1;
	}
	
	//子进程
	else if(pid == 0)
	{
		printf("I am level one child\npid = %d\tppid = %d\n", getpid(), getppid());
		//子进程再创建子进程
		pid = fork();
		if(pid<0)
		{
			perror("child fork failed");
			return -1;
		}
		
		//第一层级的子进程退出
		else if(pid > 0)
		{
			printf("level one child exited\n");
			return 0;
		}
		
		
		//第二层级的子进程执行任务
		else
		{
			sleep(3);
			//从ppid的结果看以看出，二级子进程过继给了 1188号进程 / init进程
			printf("I am level two child\npid = %d\tppid = %d\n", getpid(), getppid());
			return 0;
		}
	}
	
	//父进程等待子进程的退出
	else
	{
		if(waitpid(pid, NULL,0) != pid)
		{
			perror("waitpid failed");
			return -1;
		}
		return 0;
	}
	return 0;
}

