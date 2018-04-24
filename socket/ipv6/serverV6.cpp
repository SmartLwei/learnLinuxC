#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <unistd.h>  
#include <arpa/inet.h>  

#define MAXBUF 1024  
int main(int argc, char *argv[])  
{
    int sockfd, connectedFd;  
    socklen_t len;
    struct sockaddr_in6 serv_addr, connected_addr;  
    char buf[MAXBUF + 1];  
    
    //申请套接字
    if ((sockfd = socket(PF_INET6, SOCK_STREAM, 0)) == -1)  
    { 
        perror("subscribe socket err");  
        return -1; 
    } 
    else  
        printf("socket created\n");  

    //设置网络地址
    bzero(&serv_addr, sizeof(serv_addr));  
    serv_addr.sin6_family = PF_INET6; 
    serv_addr.sin6_port = htons(5555);
    //inet_pton(AF_INET6, "fe80::20c:29ff:fe8a:5dd7/64", &serv_addr.sin6_addr); 
    serv_addr.sin6_addr = in6addr_any; 
 
 	//绑定套接字和地址
    if ((bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in6)))== -1) 
    {  
        perror("bind err");  
        exit(1);  
    } 
    else  
        printf("binded\n");  
 
 	//监听套接字
    if (listen(sockfd, 10) == -1) 
    {  
        perror("listen");  
        exit(1);  
    } 
    else  
        printf("begin listen\n");  
  
    while (1) 
    {
    	//等待客户端的连接
        if ((connectedFd = accept(sockfd, (struct sockaddr *) &connected_addr, &len)) == -1) 
        {  
            perror("accept");  
            exit(errno);  
        } 
        else  
            printf("server: got connection from %s, port %d, socket %d\n",  
                   inet_ntop(AF_INET6, &connected_addr.sin6_addr, buf, sizeof(buf)), 
                   connected_addr.sin6_port, connectedFd); 
                   
        /* 接收客户端的消息 */ 
        bzero(buf, MAXBUF + 1);  
        len = recv(connectedFd, buf, MAXBUF, 0);  
        if (len > 0)  
            printf("服务器接收消息成功: 消息为：\n%s\n共%d个字节的数据\n", buf, len);  
        else  
            printf("消息接收失败！错误代码是%d，错误信息是\n%s\n", errno, strerror(errno)); 
            
        
        /* 发送消息到客户端 */
        bzero(buf, MAXBUF + 1);  
        strcpy(buf,"这条消息是在服务器产生的，如果你是客户端且收到了该消息，说明通信正常");  
        len = send(connectedFd, buf, strlen(buf), 0);  
        if (len < 0) 
        {  
            printf("消息发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));  
        } 
        else  
            printf("消息发送成功，共发送了%d个字节！\n", len);  
    }  
  
    close(sockfd);  
    return 0;  
}
