/*
epoll使用方法
#include <sys/epoll.h>
三个接口函数
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);
int epoll_wait(int epfd, struct epoll_event*events, int maxevents, int timeout);

创建epoll句柄
int epoll_create(int size);

epoll事件注册函数
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);
op常用参数为：
1) EPOLL_CTL_ADD 注册新的事件到epfd中
2) EPOLL_CTL_MOD 修改已经注册的fd的监听事件
3) EPOLL_CTL_DEL 从epfd中删除一个fd

等待事件的发生
int epoll_wait(int epfd, struct epoll_event*events, int maxevents, int timeout);
maxevents告诉内核这个events有多大，该值必须小于传入epoll_create的参数
timeout单位为毫秒，0立即返回，-1将阻塞
返回0表示已超时

struct poll_event
{
	__uint32_t events;
	epoll_data_t data;
};
常用的events
1) EPOLLIN  对应的fd可读（包括对端socket正常关闭）
2) EPOLLOUT 对应的fd可写
3) EPOLLPRI 对应的fd有紧急事件可读 (表示有带外信号到来)
4) EPOLLERR 对应的fd发生错误
5) EPOLLHUP 对应的fd被挂断
6) EPOLLET  将EPOLL设置为边缘触发模式(Edge Triggered)(相对于水平触发(Level Triggered))
7) EPOLLNESHOT 只监听一次该fd的该事件，如需继续监听事件，需要重新添加到EPOLL队列中
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>

#define IPADDRESS "127.0.0.1"
#define PORT 6666
#define MAXLINE 2048
#define LISTENQ 5
#define FDSIZE 1000
#define EPOLLEVENTS 20

//通常我们需要对socket(),bind(),listen(),accept(),send(),write()等函数的返回值进行判断
//该宏封装了判断过程，是的代码更加简洁
#define TESTNUM(N) \
do{\
	printf(#N" = %d\n", N); \
	if(N <= 0){\
		perror(#N" failed"); \
		printf("line: %d\n", __LINE__);\
		goto end;\
	}\
} while(0)
int main()
{
	int connectedfd, tempfd;
	int ret, readLen, writeLen;
	int epollfd;
	socklen_t len = sizeof(struct sockaddr_in);
	char buf[MAXLINE] = {0};
	struct sockaddr_in servaddr;
	struct epoll_event events[EPOLLEVENTS];
	struct epoll_event ev;
	
	//申请socket
	if((connectedfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("apply for socket failed");
		return -1;
	}
	
	//设置服务器网络地址
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr(IPADDRESS);
	
	//连接服务器
	if(connect(connectedfd, (struct sockaddr*)&servaddr, len) == -1)
	{
		perror("connect failed");
		goto end;
	}
	
	//建立连接后先确认连接是否正常，与服务器交互一次
	writeLen = write(connectedfd, "Hello, this is client", 21);
	TESTNUM(writeLen);
	//从服务器接收消息
	bzero(buf, MAXLINE);
	readLen = read(connectedfd, buf, MAXLINE);
	TESTNUM(readLen);
	printf("recvMsg is: %s\n", buf);
	
	//使用epoll管理fd
	epollfd = epoll_create(FDSIZE);
	TESTNUM(epollfd);
	
	//侦听终端(键盘)的输入
	ev.events = EPOLLIN;
	ev.data.fd = STDIN_FILENO;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);
	
	//侦听与服务器连接的文件描述符
	ev.events = EPOLLIN;
	ev.data.fd = connectedfd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, connectedfd, &ev);
	
	//与服务器交互
	while(1)
	{
		printf("we are epoll_wating...\n");
		printf("please input Msg to send to server\n");
		ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
		TESTNUM(ret);
		
		for(int i=0; i<ret; i++)
		{
			tempfd = events[i].data.fd;
			printf("tempfd = %d\n", tempfd);
			//有数据可读
			if(events[i].events & EPOLLIN)
			{
				bzero(buf, MAXLINE);
				readLen = read(tempfd, buf, MAXLINE);
				printf("readLen = %d\n", readLen);
				if(readLen == -1)
				{
					perror("read failed");
					goto end;
				}
				else if(readLen == 0)
				{
					printf("peer entity may closed, and we will exit\n");
					goto end;
				}
				else
				{
					//从键盘读取的数据
					if(tempfd == STDIN_FILENO)
					{
						printf("we got Msg form keyBoard, and we will send them to server\n");
						writeLen = write(connectedfd, buf, strlen(buf));
						printf("writeLen = %d\n", writeLen);
					}
					else if(tempfd == connectedfd)
					{
						buf[readLen] = '\0'; 
						printf("Msg from server is: %s\n", buf);
					}
					
				}
			}
		}
		
	}
	
	
end:
	close(epollfd);
	close(connectedfd);
	return 0;
}

