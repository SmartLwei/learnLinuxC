/*
NUNIX DOMAIN SOCKET常用于进程间通信

typedef unsigned short int sa_family_t;
struct sockaddr_un
{
	sa_family_t sun_family;			// AF_UNIX
	char sun_path[108];	// 路径名
};

UNIX DOMAIN SOCKET的通信方式与网络间的进程间通信方式相似
只是不再绑定"IP+协议+端口号"这样的地址
而是使用Linux文件系统中的文件(或者说路径)作为通信的地址
*/

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/un.h>  
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLINE 1024
#define UNIX_DOMAIN "/tmp/UNIX.domain" 

int main()
{
    int connectedFd;
    char buf[MAXLINE] = {0};
    int readLen = 0, writeLen = 0;
    struct sockaddr_un servUn;
    socklen_t servUnLen = 0;
    int value_ret = 0;
    
    //申请socket
    connectedFd = socket(AF_UNIX,SOCK_STREAM,0);
    if(connectedFd == -1)
    {
    	perror("apply for socket failed\n");
    	return -1;
    }
    
    //设置服务器地址
    memset(&servUn,0,sizeof(servUn));
    servUn.sun_family = AF_UNIX;
    strcpy(servUn.sun_path, UNIX_DOMAIN);
    servUnLen = offsetof(struct sockaddr_un, sun_path) + strlen(UNIX_DOMAIN);
    
    //连接服务器
    value_ret = connect(connectedFd,(struct sockaddr*)&servUn,servUnLen);
    if(value_ret != 0)
    {
        perror("connect error ");
        close(connectedFd);
        return -1;
    }
    
    //向服务器发送消息
    sprintf(buf, "Hello, this is client!");
    writeLen = write(connectedFd, buf, strlen(buf));
    printf("writeLen = %d\n", writeLen);
    
    //从服务器接收消息
    bzero(buf, MAXLINE);
    readLen = read(connectedFd, buf, MAXLINE);
    printf("readLen = %d\n", readLen);
    printf("recvMsg is: %s\n", buf);
    
    //关闭连接
    close(connectedFd);
    return 0;
}
