/*
#include <poll.h>
int poll(struct pollfd* fds, unsigned int nfds, int timeout);
nfds表示fds的数量
timeout单位为毫秒，如果为负数，表示阻塞
失败返回-1，并置位errno
超时前没有任何事件发生，则返回0
有事件发生则返回正数

struct pollfd
{
	int fd;
	short events;	//等待的事件
	short revents;	//实际发生的事件
}

poll事件如下
合法事件
POLLIN	有数据可读
POLLRDNORM	有普通数据可读
POLLRDBAND	有优先数据可读
POLLPRI		有紧急事件可读
POLLOUT		写数据不会导致阻塞
POLLWRNORM	写普通数据不会导致阻塞
POLLWRBAND	写优先数据不会导致阻塞
POLLMSGSIGPOLL	消息可用
非法事件
POLLER		指定的文件描述符发生错误
POLLHUP		指定的文件描述符挂起事件
POLLNVAL	指定的文件描述符非法
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

#define IPADDRESS "127.0.0.1"
#define PORT 6666
#define MAXLINE 1024
#define LISTENQ 5
#define OPEN_MAX 1000
#define INFTIM -1

int main()
{
	int readLen = 0;
	int writeLen = 0;
	int ret = 0;
	int connectedfd;
	char buf[MAXLINE] = {0};
	struct sockaddr_in serv_addr;

	//申请socket
	if((connectedfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Apply for socket failed");
		return -1;
	}
	
	//设置服务器网络地址
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORT);
	
	//连接服务器
	if(connect(connectedfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("connect failed");
		goto end;
	}
	
	//跟服务器打招呼
	writeLen = send(connectedfd, "Hello, I'm client!", 18, 0);
	printf("writeLen = %d\n", writeLen);
	bzero(buf, MAXLINE);
	readLen = recv(connectedfd, buf, MAXLINE, 0);
	printf("readLen = %d\n", readLen);
	if(readLen == 0)
	{
		printf("Server is closed\n");
		goto end;
	}
	else
	{
		buf[readLen] = '\0';
		printf("recvMsg is: %s\n", buf);
	}
	
	//建立poll池
	struct pollfd fds[2];
	fds[0].fd = connectedfd;
	fds[0].events = POLLIN;
	fds[1].fd = STDIN_FILENO;
	fds[1].events = POLLIN;

	//与服务器会话
	while(1)
	{
		printf("Client are waiting poll\n");
		ret = poll(fds, 2, -1);
		//判断服务器是否发送了数据
		if(fds[0].revents & POLLIN)
		{
			printf("There are msg from server, we will try to recv it\n");
			bzero(buf, MAXLINE);
			readLen = recv(connectedfd, buf, MAXLINE, 0);
			printf("readLen = %d\n", readLen);
			//读取失败，则关闭fd
			if(readLen == 0)
			{
				printf("Server is closed\n");
				goto end;
			}
			else
			{
				buf[readLen] = '\0';
				printf("recvMsg is: %s\n", buf);
			}
		}
		
		//测试标准输入是否有数据，如果有，发送到服务器
		if(fds[1].revents & POLLIN)
		{
			printf("There are msg from stdin\n");
			bzero(buf, MAXLINE);
			//从键盘读取输入
			readLen = read(STDIN_FILENO, buf, MAXLINE);
			printf("readLen = %d\n", readLen);
			if(readLen == 0)
			{
				goto end;
			}
			
			//将消息发送给服务器
			writeLen = send(connectedfd, buf, readLen, 0);
			printf("writeLen = %d\n", writeLen);
			if(writeLen == 0)
			{
				goto end;
			}
		}
		
	}
	
end:
	//关闭连接
	close(connectedfd);
	return 0;
}
