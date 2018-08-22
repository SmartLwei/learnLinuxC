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
#define EPOLLEVENTS 100

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
	int listenedfd = 0, acceptedfd = 0, tempfd = 0;
	int epollfd = 0;
	int ret = 0;
	int readLen = 0, writeLen = 0;
	socklen_t len = sizeof(struct sockaddr_in);
	char buf[MAXLINE] = {0};
	
	struct sockaddr_in servaddr, cliaddr;
	struct epoll_event events[EPOLLEVENTS];
	struct epoll_event ev;
	
	//申请socket
	if((listenedfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("apply for socket failed");
		return -1;
	}
	printf("listenedfd = %d\n", listenedfd);
	
	//设置服务器地址：协议+ip地址+端口号
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr(IPADDRESS);
	
	//如果短时间内连续多次打开/关闭服务器，可能因为time_wait导致bind失败
	//设置socket参数，来避免time_wait
	int on=1;  
	if((setsockopt(listenedfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
	{
		perror("setsockopt failed");
		goto end;
	}
	
	//绑定socket
	if(bind(listenedfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind failed");
		goto end;
	}
	
	//listen socket
	if(listen(listenedfd, LISTENQ) == -1)
	{
		perror("listen failed");
		goto end;
	}
	
	//开始使用epoll管理文件描述符fd
	//创建一个epoll描述符
	epollfd = epoll_create(FDSIZE);
	TESTNUM(epollfd);
	
	//添加监听描述符事件,保证新加入的客户端正常连接
	ev.events = EPOLLIN;
	ev.data.fd = listenedfd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenedfd, &ev);
	
	//信息交互
	while(1)
	{
		//获取已经准备好的fd
		printf("we are waiting epoll events\n");
		ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
		TESTNUM(ret);
		for(int i=0; i<ret; i++)
		{
			//获取发生事件的fd
			tempfd = events[i].data.fd;
			printf("fd with number %d has events happend\n", tempfd);
			
			//有新的客户端请求加入
			if((tempfd == listenedfd) && (events[i].events & EPOLLIN))
			{
				//首先接收客户端的连接
				acceptedfd = accept(listenedfd, (struct sockaddr*)&cliaddr, &len);
				TESTNUM(acceptedfd);
				printf("accepte a new client:\nip = %s, port = %d\n", 
						inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
				
				//建立连接后先交互一次，确定连接没有问题
				writeLen = write(acceptedfd, "Hello, this is server, welcome!", 31);
				TESTNUM(writeLen);
				
				bzero(buf, MAXLINE);
				readLen = read(acceptedfd, buf, MAXLINE);
				TESTNUM(readLen);
				printf("recvMsg is: %s\n", buf);
				
				//将新的连接添加到事件列表
				ev.events = EPOLLIN;
				ev.data.fd = acceptedfd;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, acceptedfd, &ev);
			}
			
			//有客户端发送消息到服务器
			else if(events[i].events & EPOLLIN)
			{
				bzero(buf, MAXLINE);
				readLen = read(tempfd, buf, MAXLINE);
				printf("readLen = %d\n", readLen);
				if(readLen < 0)
				{
					//连接出错，关闭连接
					perror("read failed");
					close(tempfd);
					//关闭epoll中当前fd的事件
					ev.events = EPOLLIN;
					ev.data.fd = tempfd;
					epoll_ctl(epollfd, EPOLL_CTL_DEL, tempfd, &ev);
				}
				else if(readLen == 0)
				{
					//readLen == 0表示客户端可能关闭了连接
					printf("client may closed connection, and we will close it\n");
					close(tempfd);
					//关闭epoll中当前fd的事件
					ev.events = EPOLLIN;
					ev.data.fd = tempfd;
					epoll_ctl(epollfd, EPOLL_CTL_DEL, tempfd, &ev);
				}
				else
				{
					buf[readLen] = '\0';
					printf("recvMsg is: %s\n", buf);
					//客户端关闭服务器的方法之一
					if(memcmp(buf, "closeServer", 11) == 0)
					{
						goto end;
					}
				}
			}
			
			//本机需要发送数据到客户端
			else if(events[i].events & EPOLLOUT)
			{
				printf("there are Msg to be sent, but we dont support it now\n");
			}
			else
			{
				printf("unsuppoted events\n");
				return -1;
			}	
		}
	}
	
end:
	//close epollfd并不会关闭其管理的fd
	close(epollfd);
	close(listenedfd);
	return 0;
}


