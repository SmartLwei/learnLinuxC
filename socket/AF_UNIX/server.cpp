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

//用于UNIX DOMAIN 通信的文件
#define UNIX_DOMAIN "/tmp/UNIX.domain" 
#define MAXLINE 1024

int main()
{
    int servFd,cliFd;
    char buf[MAXLINE] = {0};
    struct sockaddr_un servUn, cliUn;
    socklen_t servUnLen = 0, cliUnLen = 0;
    int readLen =0, writeLen = 0;
    int value_ret = 0;
    
    //申请socket
    servFd = socket(AF_UNIX,SOCK_STREAM,0);
    
    //删除用于通信的旧文件
    //如果上次通信的文件没有删除，可能会影响本次通信
    unlink(UNIX_DOMAIN);
    
    //设置本次通信的地址
    memset(&servUn,0,sizeof(servUn));
    servUn.sun_family = AF_UNIX;
    strcpy(servUn.sun_path,UNIX_DOMAIN);
    servUnLen = offsetof(struct sockaddr_un,sun_path) + strlen(UNIX_DOMAIN); 
    
    //printf("offsetof(strcut socketaddr_un, sun_path) = %ld\n", offsetof(struct sockaddr_un,sun_path));
    //offsetof(struct sockaddr_un, sun_path)就是取sockaddr_un结构体
    //的sun_path成员在结构体中的偏移，也就是从结构体的第几个字节开始是sun_path成员。
    //想一想，这个宏是如何实现这一功能的？
    //(先将TYPE类型的指针首地址设为0，然后取MEMBER成员的地址就是该成员在TYPE中的偏移数。)
    
    //绑定地址
    value_ret = bind(servFd,(sockaddr*)&servUn,servUnLen);
    if(value_ret != 0)
    {
        perror("bind error \n");
        close(servFd);
        return -1;
    }
    
    //侦听套接字
    value_ret = listen(servFd,2);
    if(value_ret != 0)
    {
        perror("listen error \n");
        close(servFd);
        return -1;
    }
    
    
    //接收客户端的连接请求
    printf("we are waiting client to connect\n");
    cliFd = accept(servFd, (sockaddr*)&cliUn, (socklen_t*)&cliUnLen);
    printf("cliFd = %d\n", cliFd);
    if(cliFd < 0)
    {
        perror("accept error \n");
        close(servFd);
        return -1;
    }
    
    //接收客户端的消息
    memset(buf,0,MAXLINE);
    readLen = read(cliFd, buf, MAXLINE);
    printf("readLen = %d\n", readLen);
    printf("Msg from client is: %s\n", buf);
    
    //发送消息到客户端
    sprintf(buf, "Hello, this is server, welcome!");
    writeLen = write(cliFd, buf, strlen(buf));
    printf("writeLen = %d\n", writeLen);
    
    //关闭服务器
    close(servFd);
    //删除用于通信的文件
    unlink(UNIX_DOMAIN);
    return 0;
}
