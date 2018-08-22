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
	int connectedfd;
	int ret = 0;
	char buf[1024] = {0};
	struct sockaddr_in serv_addr;
	
	//设置服务器的地址
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(DEFAULT_PORT);
	
	//申请socket
	if((connectedfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("apply for socket failed");
		return -1;
	}
	
	//连接服务器
	if(connect(connectedfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("connect failed");
		return -1;
	}
	
	//从服务器接收消息
	ret = recv(connectedfd, buf, 1024, 0);
	printf("recvLen = %d\n", ret);
	printf("recvMsg is: %s\n", buf);
	send(connectedfd, "this is client\n", 15, 0);
	
	//数据交互
	while(1)
	{
		bzero(buf, 1024);
		printf("please input Msg: \n");
		scanf("%s",buf);
		buf[strlen(buf)] = '\0';
		if(memcmp(buf, "quit", 4) == 0)
			break;
		send(connectedfd, buf, strlen(buf), 0);
	}
	
	//关闭连接
	close(connectedfd);
	return 0;	
}
