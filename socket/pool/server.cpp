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
#define PORT 7777
#define MAXLINE 1024
#define LISTENQ 5
#define OPEN_MAX 1000
#define INFTIM -1

int main()
{
	char buf[MAXLINE] = {0};
	int serverfd, acceptedfd;
	struct sockaddr_in serv_addr, cli_addr;
	unsigned int sin_size = sizeof(struct sockaddr_in);
	
	//申请socket
	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Apply for socket failed");
		return -1;
	}
	
	//bind socket
	memset(&serv_addr, 0, sin_size);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);
	if(bind(serverfd, (struct sockaddr*)&serv_addr, sin_size) == -1)
	{
		perror("bind failed");
		return -1;
	}
	
	//listen socket
	if(listen(serverfd, LISTENQ) == -1)
	{
		perror("listen failed");
		return -1;
	}
	
	//新建pollfd并初始化
	struct pollfd clientfds[OPEN_MAX];
	clientfds[0].fd = serverfd;
	clientfds[0].events = POLLIN;
	for(int i=1; i<OPEN_MAX; i++)
		clientfds[i].fd = -1;
		
	
	int nready;
	int count = 1;
	int readLen = 0;
	int writeLen = 0;
	
	while(1)
	{
		//获取可用fd的个数
		nready = poll(clientfds, count, INFTIM);
		if(nready == -1)
		{
			perror("poll failed");
			return -1;
		}
		
		//检查是否有新的连接请求
		if(clientfds[0].revents & POLLIN)
		{
			//获取新建立的连接
			if((acceptedfd = accept(serverfd, (struct sockaddr*)&cli_addr, &sin_size)) == -1)
			{
				if(errno == EINTR)
				{
					perror("accept EINTR");
					continue;
				}
				else
				{
					perror("accept failed");
					return -1;
				}
			}
			fprintf(stdout, "accept a new client: fd = %d\nip = %s, port = %d\n", acceptedfd, inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
			//将新的连接添加到数组中
			if(count < OPEN_MAX)
			{
				for(int i=1; i<OPEN_MAX; i++)
				{
					if(clientfds[i].fd < 0)
					{
						clientfds[i].fd = acceptedfd;
						writeLen = send(acceptedfd, "Hello, this server, Welcome!", 28, 0);
						printf("writeLen = %d\n", writeLen);
						bzero(buf, MAXLINE);
						readLen = recv(acceptedfd, buf, MAXLINE, 0);
						buf[readLen] = '\0';
						printf("readLen = %d\n", readLen);
						printf("recvMsg is: %s\n", buf);
						//将新的描述符添加到读描述符集合中
						clientfds[i].events = POLLIN;
						count++;
						break;
					}
				}
			}
			//如果连接数量过多，则握手后关闭会话
			else
			{
				printf("There are too many clients\n");
				writeLen = send(acceptedfd, "closeClient", 11, 0);
				printf("writeLen = %d\n", writeLen);
				bzero(buf, MAXLINE);
				readLen = recv(acceptedfd, buf, MAXLINE, 0);
				buf[readLen] = '\0';
				printf("readLen = %d\n", readLen);
				printf("recvMsg is: %s\n", buf);
				close(acceptedfd);
			}
			
			//判断是否有其他fd
			if(--nready <= 0)
				continue;
		}
		
		//处理多个客户端发送的数据包
		for(int i=0; i<count; i++)
		{
			if(clientfds[i].fd < 0)
				continue;
			//客户端是否有数据
			if(clientfds[i].revents & POLLIN)
			{
				//读取客户端的数据
				bzero(buf, MAXLINE);
				readLen = read(clientfds[i].fd, buf, MAXLINE);
				printf("readLen = %d\n", readLen);
				if(readLen == 0)
				{
					//readLen == 0表示客户端已经close了这个socket
					printf("clientfds[%d] has closed\n", i);
					close(clientfds[i].fd);
					clientfds[i].fd = clientfds[count-1].fd;
					clientfds[i].events = clientfds[count-1].events;
					clientfds[count-1].fd = -1;
					count--;
					continue;
				}
				buf[readLen] = '\0';
				printf("readMsg is: %s\n", buf);
				if(memcmp(buf,"closeServer", 11) == 0)
					goto end;
			}
		}
		
	}
	
	
//关闭服务器标志
end:
	//关闭与客户端的连接
	for(int i=1; i < count; i++)
	{
		if(clientfds[i].fd != -1)
			close(clientfds[i].fd);
	}
	close(serverfd);
	
	return 0;
}
