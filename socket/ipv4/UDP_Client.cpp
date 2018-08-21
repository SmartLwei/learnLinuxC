/**
socket UDP协议服务器客户端交互流程为
server: socket()-->bind()-->recvfrom()-->sendto()-->close()
                              ^              v
client: socket()-------->sendto()--------->recvfrom()-->close()

接收消息函数
ssize_t recvfrom(int sockfd, void *buf, size_t len, unsigned int flags, 
		struct sockaddr *from, socket_t *fromlen); 
flags常用参数（未完全列举）
MSG_DONTWAIT：操作不会被阻塞

发送消息函数
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
		const struct sockaddr *dest_addr, socklen_t addrlen);

其余函数请参考TCP_Server.cpp
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_PORT 8887
#define MAXLINE 1024
#define SERVER_IP "127.0.0.1"

int main(int argc, char* argv[])
{
    int client_fd;
    int readLen = 0, writeLen = 0;
    char buf[MAXLINE] = {0};
    struct sockaddr_in serv_addr;
    socklen_t addrLen = sizeof(serv_addr);
    
    //申请sockt
    client_fd = socket(AF_INET,SOCK_DGRAM, 0);
    if(client_fd<0)
    {
        perror("apply for socket failed");
        return -1;
    }
    
    //设置服务器的网络地址
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    if(argc == 1)
    {
        if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr)<=0)
        {
            printf("inet_pton error for %s\n",argv[1]);
            goto end;
        }
    }
    else
    {
        if(inet_pton(AF_INET,argv[1], &serv_addr.sin_addr)<=0)
        {
            printf("inet_pton error for %s\n",argv[1]);
            goto end;
        }
    }
    
    //发送消息到服务器
    struct sockaddr_in src;
    printf("please input a string to send to server\n");
    fgets(buf, MAXLINE, stdin);
    writeLen = sendto(client_fd,buf,strlen(buf),0,(struct sockaddr*)&serv_addr,addrLen);
    printf("writeLen = %d\n", writeLen);
    if(writeLen < 0)
    {
    	perror("sendto failed");
    	goto end;
    }
    
    //从服务器接收数据
    memset(buf,0,MAXLINE);
    printf("we are wating Msg from server\n");
    readLen = recvfrom(client_fd,buf,MAXLINE,0,(struct sockaddr*)&serv_addr,&addrLen);
    printf("readLen = %d\n", readLen);
    if(readLen < 0)
    {
    	perror("recvfrom failed");
    	goto end;
    }
    printf("recvMsg from server is: %s\n",buf);
    
end:
    //关闭连接
    close(client_fd);
    return 0;
}
