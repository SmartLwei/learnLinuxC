/**
* socket不仅应用于网络中的进程通信，
* 也应用于单机的进程间通信；
* 本测试示例在网络socket的基础上，
* 走一边本地socket通信流程，熟悉熟悉
* 来源：http://blog.csdn.net/deggfg/article/details/21787419
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>   //定义了errno
#include<stddef.h>  //定义了sockaddr_un结构体
#include<sys/un.h>  //定义了sockaddr_un结构体
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
int main()
{
    const char *socketfile = "./socketfile";
    int fd=0, len=0;
    char buf[1000] = {0};
    int len_buf = 0;
    struct sockaddr_un un; 
    if((fd = socket(AF_UNIX, SOCK_STREAM, 0))<0)
    {
        printf("socket failed! %s\n",strerror(errno));
        return -1;
    }
    
    unlink(socketfile);
    
    len = strlen(socketfile);
    if(len>107)
    {
        printf("socket file len is too long!");
        return -1;
    }
    
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, socketfile);
    
    len += offsetof(struct sockaddr_un, sun_path);
    if(bind(fd,(struct sockaddr*)&un, len)<0)
    {
        printf("bind error: %s", strerror(errno));
        return -1;
    }
    if(listen(fd,10)<0)
    {
        printf("listen error: %s", strerror(errno));
        return -1;
    }
    while(1)
    {
        if(accept(fd,(struct sockaddr*)&un,(socklen_t*)&len)<0)
        {
            printf("connect failed!\n");
            close(fd);
            return -1;
        }
        len = read(fd,buf,1);
        printf("读取的字符串为%s,长度为%d\n",buf,len);
        write(fd,"Hello client!\n",14);
        
    }
    close(fd);
    return 0;
}









