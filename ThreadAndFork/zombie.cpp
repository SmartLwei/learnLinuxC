/*
僵尸进程
子进程结束了，但是父进程没有(/没来得及)调用wait()或者waitpid()回收子进程遗留下来的资源，此时这个“已死”的子进程成为僵尸进程
使用ps命令可以看到子进程的状态为“Z”
如果僵尸进程占用的资源(如进程号)过多，可能导致系统不能再产生新的进程
父进程退出前如果没有处理它的僵尸子进程，则父进程退出后，僵尸子进程由init进程处理

可以使用 “ps -aux | grep 子进程号” 命令查看子进程的状态
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
		printf("child process exited\n");
	}
	//父进程
	else
	{
		sleep(2);
		printf("I'm father process.\n"
		"pid = %d\t ppid = %d\n", getpid(), getppid());
		system("ps -o pid,ppid,state,tty,command");
		printf("father process exited\n");
	}
	return 0;
}
