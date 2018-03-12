#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>

int main()
{
    const char *socketfile = "./socketfile";
    int fd = 0;
    int len = 0;
    char buf[1000] = {0};
    int len_buf = 0;
    struct sockaddr_un un;
    if((fd = socket(AF_UNIX, SOCK_STREAM, 0))<0)
    {
        printf("socket failed! %s\n",strerror(errno));
        return -1;
    }
    len = offsetof(struct sockaddr_un, sun_path) + strlen(socketfile);
    memset(&un,0,sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path,socketfile);
    if(connect(fd,(struct sockaddr*)&un,(socklen_t)len)<0)
    {
        printf("connection failed! %s\n",strerror(errno));
        return -1;
    }
    while(1)
    {
        len = write(fd,"I'm client!\n",12);
        printf("发送的字节数为%d\n",len);
        len = read(fd,buf,100);
        printf("接收的字节数为%d\n",len);
        sleep(5);
        printf("%s",buf);
    }
    close(fd);
    
    return 0;
}
