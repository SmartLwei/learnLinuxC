#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 8888
#define BUFF_LEN 512
#define SERVER_IP "127.0.0.1"

void UDP_msg_sender(int fd, struct sockaddr* dst)
{
    socklen_t len_server_sock;
    struct sockaddr_in src;
    while(1)
    {
        char buf[BUFF_LEN] = "TEST UDP MSG!\n";
        len_server_sock = sizeof(*dst);
        printf("client original: %s\n",buf);
        sendto(fd,buf,BUFF_LEN,0,dst,len_server_sock);
        memset(buf,0,BUFF_LEN);
        recvfrom(fd,buf,BUFF_LEN,0,(struct sockaddr*)&src,&len_server_sock);
        printf("from server: %s\n",buf);
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    int client_fd;
    struct sockaddr_in serv_addr;
    client_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(client_fd<0)
    {
        printf("create socket error!\n");
        return -1;
    }
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);
    UDP_msg_sender(client_fd,(struct sockaddr*)&serv_addr);
    close(client_fd);
    return 0;
}













