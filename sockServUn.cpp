#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/un.h>  
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
using namespace std;
#define UNIX_DOMAIN "/tmp/UNIX.domain" 

int main()
{
    int servFd,cliFd;
    char writebuf[1000] = {0};
    char readbuf[1000] = {0};
    struct sockaddr_un servUn,cliUn;
    int servUnLen = 0, cliUnLen = 0;
    int readLen =0, writeLen = 0;
    int value_ret = 0;
    servFd = socket(AF_UNIX,SOCK_STREAM,0);
    unlink(UNIX_DOMAIN);
    memset(&servUn,0,sizeof(servUn));
    servUn.sun_family = AF_UNIX;
    strcpy(servUn.sun_path,UNIX_DOMAIN);
    servUnLen = offsetof(struct sockaddr_un,sun_path) + strlen(UNIX_DOMAIN); 
    //offsetof(struct sockaddr_un, sun_path)就是取sockaddr_un结构体
    //的sun_path成员在结构体中的偏移，也就是从结构体的第几个字节开始是sun_path成员。
    //想一想，这个宏是如何实现这一功能的？
    //(先将TYPE类型的指针首地址设为0，然后取MEMBER成员的地址就是该成员在TYPE中的偏移数。)
    value_ret = bind(servFd,(sockaddr*)&servUn,servUnLen);
    if(value_ret != 0)
    {
        perror("bind error \n");
        close(servFd);
        return -1;
    }
    value_ret = listen(servFd,2);
    if(value_ret != 0)
    {
        perror("listen error \n");
        close(servFd);
        return -1;
    }
    cliFd = accept(servFd, (sockaddr*)&cliUn, (socklen_t*)&cliUnLen);
    if(cliFd < 0)
    {
        perror("accept error \n");
        close(servFd);
        return -1;
    }
    while(1)
    {
        memset(readbuf,0,1000);
        memset(writebuf,0,1000);
        readLen = read(cliFd,readbuf,99);
        cout << "readbuf = " << readbuf << endl;
        cout << "readLen = " << readLen << endl;
        cout << "please input string to writebuf:";
        cin >> writebuf;
        writeLen = write(cliFd,writebuf,strlen(writebuf));
        cout << "writeLen = " << writeLen << endl; 
        if(writebuf[0] == 'q')
            break;
    }
    
    close(servFd);
    cout << "end" <<endl;
    return 0;
}






