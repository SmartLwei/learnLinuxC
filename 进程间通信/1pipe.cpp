/**
进程间通信方式1：管道

管道，通常指无名管道，是 UNIX 系统IPC最古老的形式。
特点：
它是半双工的（即数据只能在一个方向上流动），具有固定的读端和写端。
它只能用于具有亲缘关系的进程之间的通信（也是父子进程或者兄弟进程之间）。
它可以看成是一种特殊的文件，对于它的读写也可以使用普通的read、write 等函数。但是它不是普通的文件，并不属于其他任何文件系统，并且只存在于内存中。

管道的使用方式
#include <unistd.h>
int pipe(int fd[2]);    // 返回值：若成功返回0，失败返回-1
fd[0]用于读，fd[1]用于写
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
	// 两个文件描述符,fd[0]用于读，fd[1]用于写
	int fd[2]; 
	pid_t pid;
	char buff[20];

	// 创建管道
	if (pipe(fd) < 0) 
		printf("Create Pipe Error!\n");

	if ((pid = fork()) < 0)  // 创建子进程
		printf("Fork Error!\n");

	 // 父进程
	else if (pid > 0) 	
	{
		close(fd[0]); // 关闭读端
		write(fd[1], "hello world\n", 12);
		close(fd[1]);
	}

	// 子进程
	else
	{
		close(fd[1]); // 关闭写端
		int readLen = read(fd[0], buff, 20);
		buff[readLen] = '\0';
		printf("子进程收到的数据为: %s", buff);
		close(fd[0]);
	}

	return 0;
}
