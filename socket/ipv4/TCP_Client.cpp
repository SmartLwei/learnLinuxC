/*
TCP Client通常流程为

1. 申请socket
fd = socket(int __domain, int __type, int __protocol);

2. 连接服务器
connect(int __fd, const struct sockaddr* __addr, socklen_t __len);
其中__addr为服务器的网络地址:协议+ip地址+端口号

3. 然后发送和接收消息
send recv read write等

4. 关闭连接
close(fd)

细节请参考TCP_Server.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_pton函数

#define PORT 7788
#define MAXLINE 4096

int main(int argc, char**argv)
{
    int sockfd;
    char sendline[4096];
    char buf[MAXLINE];
    int readLen = 0, writeLen = 0;
    struct sockaddr_in servaddr;

	//申请socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        exit(0);
    }

	//设置服务器端网络地址：协议+ip地址+端口号
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(argc == 1)
    {
        if(inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr)<=0)
        {
            printf("inet_pton error for %s\n",argv[1]);
            goto end;
        }
    }
    else
    {
        if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)
        {
            printf("inet_pton error for %s\n",argv[1]);
            goto end;
        }
    }
    
    //连接服务器
    if(connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr))<0)
    {
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        goto end;
    }
    
    //发送消息到服务器
    printf("send msg to server: \n");
    //fgets可以获取空格和每行最后的'\n'
    fgets(sendline,4096,stdin);
    writeLen = send(sockfd,sendline,strlen(sendline),0);
    printf("writeLen = %d\n", writeLen);
    if(writeLen < 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        goto end;
    }
    
    //从服务器接收消息
    if((readLen = recv(sockfd,buf,MAXLINE,0)) == -1)
    {
        perror("recv error");
        goto end;
    }
    printf("readLen = %d\n", readLen);
    buf[readLen] = '\0';
    printf("Received: %s\n",buf);
    
end:
    //关闭与服务器的连接
    printf("close socket\n");
    close(sockfd);
}

