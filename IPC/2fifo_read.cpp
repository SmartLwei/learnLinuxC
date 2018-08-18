/**
用法
#include <sys.types.h>
#include <sys/stat.h>
返回值：成功返回0，出错返回-1
int mkfifo(const char *pathname, mode_t mode);
mode常用参数    0777

接收方:
makefifo(const char *pathname, mode_t mode);
int fd = open(const char* pathName, mode_t mode);
read(fd, buf, BUFLEN);
close(fd);


说明
FIFO，也称为命名管道，它是一种文件类型。
特点
FIFO可以在无关的进程之间交换数据，与无名管道不同。
FIFO有路径名与之相关联，它以一种特殊设备文件形式存在于文件系统中。
FIFO先进先出
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
#define FIFONAME "myFiFo"

int main()
{
	int fd;
	int len;
	char buf[1024];

	// 如果该文件已经存在，则删除
	if(access(FIFONAME,F_OK)==0)
	{
		execlp("rm", "-f", FIFONAME, NULL);
		printf("file already exist! and we delete it");
	}

	// 创建FIFO管道
	if (mkfifo(FIFONAME, 0666) < 0 && errno != EEXIST)
		perror("Create FIFO Failed");

	// 以读打开FIFO
	if ((fd = open(FIFONAME, O_RDONLY)) < 0)
	{
		perror("Open FIFO Failed");
		exit(1);
	}

	// 读取FIFO管道
	while ((len = read(fd, buf, 1024)) > 0)
		printf("Read message: %s", buf);

	//printf("len = %d\n", len); //read返回0表示对端已经关闭fd
	// 关闭FIFO文件
	close(fd);
	// 删除fifo
	//unlink(FIFONAME);
	execlp("rm", "-f", FIFONAME, NULL);
	return 0;
}

