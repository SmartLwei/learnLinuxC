/*
避免僵尸进程的方法1
子进程退出时发出一个信号，父进程捕获该信号并waipid()子进程释放资源
*/


#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

//进程捕获到信号量后跳转执行该函数，结束子进程
static void dealSignal(int signo)
{
	pid_t pid;
	int stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child process %d terminated\n", pid);
}

int main()
{
	pid_t pid;
	//注册信号捕获函数
	signal(SIGCHLD, dealSignal);
	
	pid = fork();
	if(pid < 0)
	{
		perror("fork failed");
		return -1;
	}
	//子进程
	else if(pid == 0)
	{
		//输出子进程的信息
		printf("I'm child process.\n"
		"pid = %d\t ppid = %d\n", getpid(), getppid());
		printf("child process exited\n");
	}
	//父进程
	else
	{
		//虽然父进程会在这里休眠2秒，但是他也可能被signal唤醒
		sleep(2);
		printf("I'm father process.\npid = %d\n", getpid());
		system("ps -o pid,ppid,state,tty,command");
		printf("father process exited\n");
	}
	return 0;
}
