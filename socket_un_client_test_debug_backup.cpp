#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#define UNIX_DOMAIN "/tmp/UNIX.domain" 
int main()
{
    int clifd = 0;
    int unlen = 0;
    char buf[1000] = {0};
    char buf1[1000] = {0};
    int len_buf = 0;
    int writelen = 0, readlen = 0;
    int value_ret = 0;
    struct sockaddr_un un;
    if((clifd = socket(AF_UNIX, SOCK_STREAM, 0))<0)
    {
        printf("socket failed! %s\n",strerror(errno));
        return -1;
    }
    printf("客户端申请socket成功, clifd = %d\n",clifd);
    unlen = offsetof(struct sockaddr_un, sun_path) + strlen(UNIX_DOMAIN);
    memset(&un,0,sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path,UNIX_DOMAIN);
    if(value_ret = connect(clifd,(struct sockaddr*)&un,(socklen_t)unlen)<0)
    {
        printf("connection failed! %s\n",strerror(errno));
        return -1;
    }
    printf("客户端socket connect成功, 返回值为%d\n",value_ret);
    while(1)
    {
        memset(buf,0,500);
        printf("请输入需要传向服务器的字符串\n");
        scanf("%s",buf);
        if(buf[0] == 'q')
            break;
        printf("buf中的字符串为%s\n",buf);
        writelen = write(clifd,buf,4);
        printf("write的字节数为%d\n",writelen);
        readlen = read(clifd,buf,4);
        printf("read的字节数为%d\n",readlen);
        printf("read的字符串为%s",buf);
    }
    close(clifd);
    printf("end\n");
    return 0;
}
