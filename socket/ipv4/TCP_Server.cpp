/*
TCP Server通常的流程为

1. 申请socket
int serverfd = socket(int __domain, int __type, int __protocol);
其中__domain 通常为
AF_INET(ipv4), AF_UNIX(进程间通信), AF_INET6(ipv6), AF_NETLINK(与内核通信) 等
__type 通常为 SOCK_STREAM, SOCK_DGRAM 等
__protocol通常设置为0，表示根据__type设置默认值

2. 绑定地址
bind(int __fd, const struct sockaddr* __addr, socklen_t __len);
地址包括协议，ip地址，端口号
#define	INADDR_ANY ((in_addr_t) 0x00000000)
Address to accept any incoming messages.
因为服务器IP地址为本机IP地址，因此通常设置为INADDR_ANY，表示接收所有到来的消息
该参数可以用于多网卡的场景。如果多个网卡的ip地址不同，则可以通过设置这个ip地址
来非别接收不同网卡到来的数据
常用函数 htonl, htons, inet_pton, inet_addr, inet_ntoa,strerror等
例如
inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
inet_addr("127.0.0.1");
inet_ntoa(servaddr.sin_addr);
strerror(errno);

3. 侦听socket
listen(int __fd, int __n);
__n表示请求连接的队列的大小

4. 使用accept响应客户端的连接请求
int acceptedfd = accept(int serverfd, struct sockaddr* addr, socklen_t* len);

ipv4网络地址为
struct sockaddr_in
{
	short int			sin_family;		// Address family
	unsigned short int	sin_port;		// Port number
	struct in_addr		sin_addr;		// Internet address,有多种方式设置网络地址
	unsigned char		sin_zero[8];	// 填充，使得Same size as struct sockaddr
};
struct in_addr
{
	union
	{
		struct
		{
			unsigned char s_b1,
			s_b2,
			s_b3,
			s_b4;
		}s_un_b;						//以字节的方式设置IP地址
		struct
		{
			unsigned short s_w1, s_w2;
		}s_un_w;						//以双字的方式设置IP地址
		unsigned long s_addr;			//以long的方式设置IP地址
	}s_un;
};

通用地址结构
struct sockaddr
{
	unsigned short sa_family;			// address family, AF_xxx
	char sa_data[14];					// 14 bytes of protocol address
};
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define PORT 7788
#define MAXLINE 4096

int main(int argc, char** argv)
{
    int serverfd, connect_fd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE] = {0};
    int readLen = 0, writeLen = 0;
    
    //申请Socket
    if( (serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    
    //设置服务器地址，包括协议，地址，端口号
    //#define	INADDR_ANY		((in_addr_t) 0x00000000)
    // Address to accept any incoming messages.
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

	//如果短时间内连续多次打开/关闭服务器，可能因为time_wait导致bind失败
	//设置socket参数，来避免time_wait
	int on=1;  
	if((setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
	{
		perror("setsockopt failed");
		goto end;
	}
	
    //将地址绑定到套接字上
    if(bind(serverfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        printf("if errno is 98, try two minutes later\n");
        goto end;
    }

    //侦听该socket
    if( listen(serverfd, 10) == -1)    
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        goto end;
    }
    
    //信息交互
    printf("======waiting for client's request======\n");
    //阻塞直到有客户端连接
    if((connect_fd = accept(serverfd, (struct sockaddr*)NULL, NULL)) == -1)
    {
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        goto end;
    }
    
    //接受客户端传过来的数据
    bzero(buff, MAXLINE);
    readLen = recv(connect_fd, buff, MAXLINE, 0);
    printf("readLen = %d\n", readLen);
    buff[readLen] = '\0';
    printf("recv msg from client: %s\n", buff);
        
    //向客户端发送回应数据
    bzero(buff, MAXLINE);
    sprintf(buff, "Hello, this is TCP server, Welcome!");
    writeLen = send(connect_fd, buff, strlen(buff),0);
    printf("writeLen = %d\n", writeLen);
    if(writeLen == -1)
    {    
        perror("send error");
        //关闭连接
        close(connect_fd);
    }
    
end:
    //关闭服务器socket
    printf("close socket\n");
    close(serverfd);
}


