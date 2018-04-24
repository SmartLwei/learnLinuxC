/**
* https://www.cnblogs.com/skyfsm/p/6287787.html?utm_source=itdadao&utm_medium=referral
* socket UDP协议服务器端
*
* server: socket()-->bind()-->recvfrom()-->sendto()-->close()
*                               ^              v
* client: socket()-------->sendto()--------->recvfrom()-->close()
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 8888
#define BUFF_LEN 1024

int handle_UDP_msg(int fd)
{
    char buf[BUFF_LEN];
    socklen_t len_client_sockaddr;
    int count;
    struct sockaddr_in client_addr;
    while(1)
    {
        memset(buf,0,BUFF_LEN);
        len_client_sockaddr = sizeof(client_addr);
        //当没有消息时，recvfrom会阻塞
        count = recvfrom(fd, buf, BUFF_LEN, 0, 
            (struct sockaddr*)&client_addr, &len_client_sockaddr);
        if(count == -1)
        {
            printf("receive data faild!\n");
            return -1;
        }
        printf("client: %s\n",buf);
        memset(buf,0,BUFF_LEN);
        sprintf(buf,"I have received %d bytes data!\n",count);
        printf("server: %s\n",buf);
        sendto(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&client_addr,len_client_sockaddr);
    }
}

int main(int argc, char* argv[])
{
    int server_fd, value_ret;
    struct sockaddr_in serv_addr;
    server_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(server_fd < 0)
    {
        printf("create socket error!\n");
        return -1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);
    value_ret = bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(value_ret < 0)
    {
        printf("socket bind error!\n");
        return -1;
    }
    handle_UDP_msg(server_fd);
    close(server_fd);
    return 0;
}











