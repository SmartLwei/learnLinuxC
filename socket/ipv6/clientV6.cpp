#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/socket.h>  
#include <resolv.h>  
#include <stdlib.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  

#define MAXBUF 1024  
int main(int argc, char **argv)  
{  
    int sockfd, len;  
    struct sockaddr_in6 dest;  
    char buffer[MAXBUF + 1];  
    if (argc != 3) 
    {  
        printf ("参数格式错误！正确用法如下：\n");
        printf("./%s IP地址 端口\n",argv[0]);
        printf("比如:%s ::1 5555\n",argv[0]); 
        exit(0);  
    }
    
    /* 申请socket套接字 */
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)  
    {
        perror("Socket");  
        exit(errno);  
    }  
    printf("socket created\n");  
  
    /* 初始化服务器端（对方）的地址和端口信息 */  
    bzero(&dest, sizeof(dest));  
    dest.sin6_family = AF_INET6;   
    dest.sin6_port = htons(atoi(argv[2]));  
    if ( inet_pton(AF_INET6, argv[1], &dest.sin6_addr) <= 0 ) 
    {  
        perror(argv[1]); 
        printf("inet_pton err\n"); 
        return -1;
    }  
    printf("address created\n");  
  
    /* 连接服务器 */  
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) 
    {  
        perror("Connect ");  
        exit(errno);  
    }  
    printf("server connected\n");  
         
    /* 发送消息到服务器 */
    bzero(buffer, MAXBUF + 1);  
    strcpy(buffer, "本句话是客户端产生的，如果你是服务器，且收到了该消息，则通信正常\n");  
    len = send(sockfd, buffer, strlen(buffer), 0);  
    if (len < 0)  
        printf("消息发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));  
    else  
        printf("消息发送成功，共发送了%d个字节！\n",len);  
  
    /* 接收对方发过来的消息 */  
    bzero(buffer, MAXBUF + 1);  
    len = recv(sockfd, buffer, MAXBUF, 0);  
    if (len > 0)  
        printf("接收消息成功:消息为：\n%s\n共%d个字节的数据\n",buffer, len);  
    else  
        printf("消息接收失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno)); 
        
    /* 关闭连接 */  
    close(sockfd);  
    return 0;  
}
