//UNIX Domain Socket不同于网络socket
//需要显示调用bind()函数
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
/*
* Create a client endpoint and connect to a server.
* Returns fd if all OK, <0 on error.
*/
int main()
{
    printf("I'm client\n");
    const char *name = "foo.socket";
    int clifd, unlen;
    char buf[1000] = {0};
    strcpy(buf,"this is the message form client through socket\n");
    int writelen,readlen,buflen;
    struct sockaddr_un un;
    /* create a UNIX domain stream socket */
    if ((clifd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        printf("socket error\n");
        return(-1);
    }
    /* fill socket address structure with our address */
    /*
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path,"foo.socket");
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
    unlink(un.sun_path);        // in case it already exists 
    if (bind(fd, (struct sockaddr *)&un, len) < 0) 
    {
        rval = -2;
        goto errout;
    }
    // fill socket address structure with server's address 
    */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    unlen = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    //if (bind(clifd, (struct sockaddr *)&un, unlen) < 0) 
    //{
    //    printf("bind error\n");
    //    goto errout;
    //}
    if (connect(clifd, (struct sockaddr *)&un, unlen) < 0) 
    {
        printf("connect error\n");
        goto errout;
    }
    writelen = write(clifd,buf,100 );
    printf("write len is %d\t",writelen);
    readlen = read(clifd, buf, 100);
    printf("read len is %d\t",readlen);
    printf("%s\n",buf);
    
    
errout:
    close(clifd);
    return 0;
}
