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
#include<errno.h>   
#include<stddef.h>  
#include<sys/un.h>  
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#define UNIX_DOMAIN "/tmp/UNIX.domain" 
int main()
{
    int servfd=0, clifd = 0, unlen=0, value_ret = 0;
    char buf[1000] = {0};
    char buf1[1000] = {0};
    int buflen = 0;
    int readlen = 0, writelen = 0;
    struct sockaddr_un un; 
    if((servfd = socket(AF_UNIX, SOCK_STREAM, 0))<0)
    {
        printf("socket failed! %s\n",strerror(errno));
        return -1;
    }
    printf("服务器 申请socket成功, servfd = %d\n",servfd);
    unlink(UNIX_DOMAIN);
    
    unlen = strlen(UNIX_DOMAIN);
    if(unlen>107)
    {
        printf("socket file len is too long!");
        return -1;
    }
    
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, UNIX_DOMAIN);
    
    unlen += offsetof(struct sockaddr_un, sun_path);
    if(value_ret = bind(servfd,(struct sockaddr*)&un, unlen)<0)
    {
        printf("bind error: %s", strerror(errno));
        return -1;
    }
    printf("服务器socket绑定成功, 返回值为%d\n",value_ret);
    if(value_ret = listen(servfd,10)<0)
    {
        printf("listen error: %s", strerror(errno));
        return -1;
    }
    printf("服务器socket listen成功, 返回值为%d\n",value_ret);
    if(clifd = accept(servfd,(struct sockaddr*)&un,(socklen_t*)&unlen)<0)
    {
        printf("connect failed!\n");
        close(servfd);
        return -1;
    }
    printf("服务器socket accept成功\n，clifd = %d\n", clifd);
    while(1)
    {
        memset(buf,0,500);
        printf("服务器开始read\n");
        readlen = read(clifd,buf1,5);
        printf("read的字符串为%s,长度为%d\n",buf1,readlen);
        printf("请输入需要发送给客户端的字符串\n");
        scanf("%s",buf);
        if(buf[0] == 'q')
            break;
        printf("buf中的字符串为%s\n",buf);
        writelen = write(clifd,buf,5);
        printf("发送的字符串长度为%d\n",writelen);
    }
    close(clifd);
    printf("end\n");
    return 0;
}









