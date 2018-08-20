#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define DEFAULT_PORT 8000
#define MAXLINE 4096
int main(int argc, char** argv)
{
    int    socket_fd, connect_fd;
    struct sockaddr_in     servaddr;
    /*
    struct  sockaddr_in
    {
        short  int  sin_family;              // Address family
        unsigned  short  int  sin_port;      // Port number
        struct  in_addr  sin_addr;           // Internet address,有多种方式设置网络地址
        unsigned  char  sin_zero[8];         // Same size as struct sockaddr
    };
    typedef struct in_addr
    {
        union
        {
            struct
            {
                unsigned char s_b1,
                s_b2,
                s_b3,
                s_b4;
            } S_un_b;                       //以字节的方式设置IP地址
            struct
            {
                unsigned short s_w1, s_w2;
            } S_un_w;                       //以双字的方式设置IP地址
            unsigned long S_addr;           //以long的方式设置IP地址
          } S_un;
    } IN_ADDR;
    */
    char    buff[4096];
    int     n;
    //初始化Socket
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //初始化
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //#define	INADDR_ANY		((in_addr_t) 0x00000000)
    // Address to accept any incoming messages.
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //以long的方式设置IP地址
    //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT

    //将本地地址绑定到所创建的套接字上
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    /*
    原型
    struct sockaddr
    {
        unsigned  short  sa_family;    // address family, AF_xxx
        char  sa_data[14];             // 14 bytes of protocol address
    };
    */
    //开始监听是否有客户端连接
    if( listen(socket_fd, 10) == -1)    //10表示监听队列的最大排队长度
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("======waiting for client's request======\n");
    while(1)
    {
        //阻塞直到有客户端连接，不然多浪费CPU资源。
        if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1)
        {
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        //接受客户端传过来的数据
        n = recv(connect_fd, buff, MAXLINE, 0);
        //向客户端发送回应数据
        if(!fork())  /*子进程执行下列语句，也就是说父进程只执行接收任务，子进程还要执行发送任务*/
        {
            if(send(connect_fd, "Hello,you are connected!\n", 26,0) == -1)
                perror("send error");
            close(connect_fd);
            exit(0);
        }
        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);
        close(connect_fd);
    }
    close(socket_fd);
}

