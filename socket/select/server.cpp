/*
FD_ZERO(fd_set* fds);			//将fds清零
FD_SET(int fd, fd_set* fds);	//设置fd在fds中的有效性
FD_ISSET(int fd, fd_set* fds);	//检查fd在fds中是否有效
FD_CLR(int fd, fd_set* fds);	//取消fd在fds中的有效性

int select(int nfds, fd_set* readFds, fd_set* writeFds, fd_set* exceptfds, struct timeval* timeout);
其中，nfds为最大的fd值+1
如果timeout之前三个Fds中的任何fd有响应，则返回非0值
如果timeout时间到了仍然没有fd有响应，则返回0
该函数会更改三个Fds及timeout的值，因此每次使用前都需要重新设置这些参数
*/

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <memory.h>

#define DEFAULT_PORT 6666
int main(int argc, char* argv[])
{
	int serverfd, acceptedfd;
	struct sockaddr_in serv_addr, cli_addr;
	unsigned int sin_size, serv_port = 6666, lisnum = 10;
	
	//获取服务器socket
	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("apply for socket failed");
		return -1;
	}
	
	//绑定ip地址和端口号
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(DEFAULT_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(serverfd, (struct sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
	{
		perror("bind failed");
		return -1;
	}
	
	//侦听服务器fd
	if(listen(serverfd, lisnum) == -1)
	{
		perror("listen failed");
		return -1;
	}
	
	//客户端fd集合
	fd_set client_fdset;
	int maxsock = serverfd;
	struct timeval tv;
	int client_sockfd[5] = {0};
	int conn_count = 0;
	char buf[1024] = {0};
	int ret = 0;
	
	while(1)
	{
		tv.tv_sec = 30;
		tv.tv_usec = 0;
		//清空client_fdset的监控列表
		FD_ZERO(&client_fdset);
		//将服务器fd添加至读取列表
		FD_SET(serverfd, &client_fdset);
		//把活跃的fd添加到fdset中
		for(int i=0; i<5; ++i)
		{
			if(client_sockfd[i] != 0)
				FD_SET(client_sockfd[i], &client_fdset);
		}
		
		//select轮询活跃的fd
		printf("we are selecting\n");
		ret = select(maxsock+1, &client_fdset, NULL, NULL, &tv);
		if(ret < 0)
		{
			perror("select failed");
			return -1;
		}
		if(ret == 0)
		{
			printf("timeout\n");
			continue;
		}
		
		//轮询活跃的客户端fd
		for(int i=0; i<conn_count; i++)
		{
			//判断对应的fd是否已经有数据到来
			if(FD_ISSET(client_sockfd[i], &client_fdset))
			{
				printf("start recv from client[%d]\n", i);
				ret = recv(client_sockfd[i], buf, 1024, 0);
				printf("recvLen = %d\n", ret);
				if(ret <= 0)
				{
					printf("client[%d] closed\n",i);
					close(client_sockfd[i]);
					FD_CLR(client_sockfd[i], &client_fdset);
					client_sockfd[i] = 0;
					conn_count--;
				}
				else
				{
					printf("Msg is: %s\n", buf);
					//客户端控制关闭服务器
					//只提供接口，方式可以更换为其他
					if(memcmp(buf, "closeServer", 11) == 0)
					goto end;
				}
			}
		}
		bzero(buf, 1024);
		
		//查看是否有新的客户端的连接请求
		if(FD_ISSET(serverfd, &client_fdset))
		{
			//接受连接
			acceptedfd = accept(serverfd,(struct sockaddr*)&cli_addr, &sin_size);
			if(acceptedfd < 0)
			{
				perror("accept failed");
				continue;
			}
			
			//将新加入的连接添加到集合中
			if(conn_count < 5)
			{
				for(int i=0; i<5; i++)
				{
					if(client_sockfd[i] == 0)
					{
						client_sockfd[i] = acceptedfd;
						break;
					}
				}
				printf("new connection fd = %d\nip = %s, port = %d\n", acceptedfd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
				send(acceptedfd, "this is server, welcome!\n", 25, 0);
				if(acceptedfd > maxsock)
					maxsock = acceptedfd;
				conn_count++;		
			}
			else
			{
				printf("client count larger than 5, we will close this connect\n");
				send(acceptedfd, "close", 5, 0);
				close(acceptedfd);
			}
		}
	}
	
//关闭客户端的连接
end:
	for(int i=0; i<5; i++)
	{
		if(client_sockfd[i] != 0)
		{
			close(client_sockfd[i]);
		}
	}
	close(serverfd);
	return 0;
}
