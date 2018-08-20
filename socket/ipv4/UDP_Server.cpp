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
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 8887
#define MAXLINE 1024

int main(int argc, char* argv[])
{
    int server_fd;
    int readLen = 0, writeLen = 0;
    socklen_t addrLen;
    char buf[MAXLINE];
    struct sockaddr_in serv_addr, client_addr;
    
    //申请socket
    server_fd = socket(AF_INET,SOCK_DGRAM, 0);
    if(server_fd < 0)
    {
        perror("apply for socket failed");
        return -1;
    }
    
    //绑定网络地址
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);
    if(bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind failed");
        return -1;
    }
    
    //从客户端接收消息
    memset(buf,0,MAXLINE);
    printf("we are waiting Msg from client\n");
    //recvfrom会阻塞等待消息
    memset(&client_addr, 0, sizeof(client_addr));
    readLen = recvfrom(server_fd, buf, MAXLINE, 0, (struct sockaddr*)&client_addr, &addrLen);
    printf("readLen = %d\n", readLen);
    if(readLen == -1)
    {
        perror("recvfrom failed");
        return -1;
    }
    buf[readLen] = '\0';
    printf("recvMsg from client is: %s\n",buf);
        
    //发送消息到客户端
    memset(buf,0,MAXLINE);
    sprintf(buf,"Hello, this is UDP server, Welcome!");
    writeLen = sendto(server_fd, buf, strlen(buf), 35, (struct sockaddr*)&client_addr, sizeof(client_addr));
    printf("writeLen = %d\n", writeLen);
    if(writeLen < 0)
    {
    	perror("sendto failed");
    }
    
end:
    //关闭服务器
    close(server_fd);
    return 0;
}
