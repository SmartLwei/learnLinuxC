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
    int connectedFd;
    char readbuf[1000] = {0};
    char writebuf[1000] = {0};
    int readLen = 0, writeLen = 0;
    struct sockaddr_un servUn;
    int servUnLen = 0;
    int value_ret = 0;
    connectedFd = socket(AF_UNIX,SOCK_STREAM,0);
    cout << "connectedFd = " << connectedFd << endl; 
    memset(&servUn,0,sizeof(servUn));
    servUn.sun_family = AF_UNIX;
    strcpy(servUn.sun_path, UNIX_DOMAIN);
    servUnLen = offsetof(struct sockaddr_un, sun_path) + strlen(UNIX_DOMAIN);
    value_ret = connect(connectedFd,(struct sockaddr*)&servUn,servUnLen);
    if(value_ret != 0)
    {
        perror("connect error ");
        close(connectedFd);
        return -1;
    }
    while(1)
    {
        memset(readbuf,0,1000);
        memset(writebuf,0,1000);
        cout << "please input string to tell the server!" << endl;
        cin >> writebuf;
        writeLen = write(connectedFd,writebuf,strlen(writebuf));
        cout << "writeLen = " << writeLen << endl;
        if(writebuf[0] == 'q')
            break;
        readLen = read(connectedFd, readbuf, 88);
        cout << "readbuf = " << readbuf << endl;
        cout << "readLen = " << readLen << endl;
    }
    close(connectedFd);
    cout << "end" <<endl;
    return 0;
}
