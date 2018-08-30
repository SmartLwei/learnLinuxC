/*
孤儿进程
父进程创建子进程后，没有wait()或者waitpid()子进程就结束了，而子进程还在继续执行
此时子进程成为孤儿进程，并过继到init进程下
[但是通过执行这个例子，孤儿进程的ppid=1188并不是init进程号1。原因是？不重要，有人接管就好]
孤儿进程并不会有什么危害
技巧：可以利用孤儿进程的方式创建守护进程
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	//创建子进程
	pid = fork();
	if(pid < 0)
	{
		perror("fork failed");
		return -1;
	}
	
	//子进程
	if(pid == 0)
	{
		//输出子进程的信息
		printf("I'm child process.\n"
		"pid = %d\t ppid = %d\n", getpid(), getppid());
		
		//休眠，保证父进程退出
		sleep(2);
		printf("I'm child process.\n"
		"pid = %d\t ppid = %d\n", getpid(), getppid());
		printf("child process exited\n");
	}
	//父进程
	else
	{
		usleep(10000);
		//从结果可以看出，孤儿也并不是过继给爷爷
		printf("I'm father process.\n"
		"pid = %d\t ppid = %d\n", getpid(), getppid());
		printf("father process exited\n");
	}
	return 0;
}


