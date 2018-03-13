#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
int main(void)
{
    printf("I'm server\n");
    int fd, size, clifd,readlen,writelen;
    char buf[1000] = {0};
    int len_buf = 0;
    struct sockaddr_un un;
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, "foo.socket");
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket error\n");
        exit(1);
    }
    size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
    //offsetof(struct sockaddr_un, sun_path)就是取sockaddr_un结构体
    //的sun_path成员在结构体中的偏移，也就是从结构体的第几个字节开始是sun_path成员。
    //想一想，这个宏是如何实现这一功能的？
    //(先将TYPE类型的指针首地址设为0，然后取MEMBER成员的地址就是该成员在TYPE中的偏移数。)
    unlink("foo.socket");
    if (bind(fd, (struct sockaddr *)&un, size) < 0) 
    {
       perror("bind error\n");
       exit(1);
    }
    printf("UNIX domain socket bound/n");
    if(listen(fd, 10) < 0)
    {
       perror("listen error");
       exit(1);
    }
    printf("flag1\n");
    if((clifd = accept(fd, (struct sockaddr*)&un,(socklen_t*)&size)) <0 )
    {
       perror("accept error");
       exit(1);
    }
    printf("fd = %d\t clifd = %d\n",fd,clifd);
    printf("flag2\n");
    readlen = read(clifd, buf, 100);
    printf("server read string %s\nreadlen is %d\n",buf, readlen);
    strcpy(buf,"this is the message form server through socket\n");
    writelen = write(clifd, buf,100);
    printf("writelen is %d\n",writelen);
    close(clifd);
    
    return 0;
}





//accept模块
/*
#include <stddef.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
int serv_accept(int listenfd, uid_t *uidptr)
{
    int                 clifd, len, err, rval;
    time_t              staletime;
    struct sockaddr_un  un;
    struct stat         statbuf;
    len = sizeof(un);
    if ((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0)
    return(-1);     //often errno=EINTR, if signal caught
    //obtain the client's uid from its calling address 
    len -= offsetof(struct sockaddr_un, sun_path); //len of pathname 
    un.sun_path[len] = 0;           // null terminate 
    if (stat(un.sun_path, &statbuf) < 0) 
    {
        rval = -2;
        goto errout;
    }
    if (S_ISSOCK(statbuf.st_mode) == 0) 
    {
        rval = -3;      // not a socket 
        goto errout;
    }
    if (uidptr != NULL)
    *uidptr = statbuf.st_uid;   // return uid of caller 
    unlink(un.sun_path);        // we're done with pathname now 
    return(clifd);
errout:
    err = errno;
    close(clifd);
    errno = err;
    return(rval);
}
*/



//listen 模块
/*
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#define QLEN 10
*/
/*
* Create a server endpoint of a connection.
* Returns fd if all OK, <0 on error.
*/
/*
int serv_listen(const char *name)
{
    int fd, len, err, rval;
    struct sockaddr_un  un;
    // create a UNIX domain stream socket 
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return(-1);
    unlink(name);   // in case it already exists 
    //fill in socket address structure 
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    //bind the name to the descriptor
    if (bind(fd, (struct sockaddr *)&un, len) < 0) 
    {
        rval = -2;
        goto errout;
    }
    if (listen(fd, QLEN) < 0) 
    {      // tell kernel we're a server
        rval = -3;
        goto errout;
    }
    return(fd);
errout:
    err = errno;
    close(fd);
    errno = err;
    return(rval);
}
*/

//bind模块
/*
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
int main(void)
{
    int fd, size;
    struct sockaddr_un un;
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, "foo.socket");
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket error");
        exit(1);
    }
    size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
    //offsetof(struct sockaddr_un, sun_path)就是取sockaddr_un结构体
    //的sun_path成员在结构体中的偏移，也就是从结构体的第几个字节开始是sun_path成员。
    //想一想，这个宏是如何实现这一功能的？
    //(先将TYPE类型的指针首地址设为0，然后取MEMBER成员的地址就是该成员在TYPE中的偏移数。)
    if (bind(fd, (struct sockaddr *)&un, size) < 0) 
    {
       perror("bind error");
       exit(1);
    }
    printf("UNIX domain socket bound/n");
    exit(0);
}

*/
