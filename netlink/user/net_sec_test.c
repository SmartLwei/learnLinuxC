/*******************************
file:           u_netlink.c
description:    netlink demo
author:         arvik
email:          1216601195@qq.com
blog:           http://blog.csdn.net/u012819339
*******************************/
//中文测试
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

#define ITS_NET 25          
//安全层的port约定
//(随便写的，只要与用户态使用是相同且不冲突就行)
#define SEC_PORT    2000
//设备层的port约定
#define FAC_PORT    2001

#define MAX_PLOAD 5000

struct _my_msg
{
    struct nlmsghdr hdr;
    int8_t  data[MAX_PLOAD];
};



int main(int argc, char **argv)
{
    const char *data = "hello kernel";
    struct sockaddr_nl  local_addr, dest_addr;

    int skfd;
    struct nlmsghdr *nlherder = NULL;
    struct _my_msg info;
    int ret;
    int len_dest_addr = 0;

    skfd = socket(AF_NETLINK, SOCK_RAW, ITS_NET);
    if(skfd == -1)
    {
        printf("create socket error...%s\n", strerror(errno));
        return -1;
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.nl_family = AF_NETLINK;
    local_addr.nl_pid = SEC_PORT;
    local_addr.nl_groups = 0;
    if(bind(skfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) != 0)
    {
        printf("bind() error\n");
        close(skfd);
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; // to kernel
    dest_addr.nl_groups = 0;

    nlherder = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PLOAD));
    memset(nlherder, 0, sizeof(struct nlmsghdr));
    nlherder->nlmsg_len = NLMSG_SPACE(MAX_PLOAD);
    nlherder->nlmsg_flags = 0;
    nlherder->nlmsg_type = 0;
    nlherder->nlmsg_seq = 0;
    nlherder->nlmsg_pid = local_addr.nl_pid; //self port

    memcpy(NLMSG_DATA(nlherder), data, strlen(data));
    //ret = sendto(skfd, nlherder, nlherder->nlmsg_len, 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_nl));

    //if(!ret)
    //{
    //    perror("sendto error1\n");
    //    close(skfd);
    //    exit(-1);
    //}
    printf("wait kernel msg!\n");
    memset(&info, 0, sizeof(info));
    ret = recvfrom(skfd, &info, sizeof(struct _my_msg), 0, (struct sockaddr *)&dest_addr, (socklen_t*)&len_dest_addr);
    if(!ret)
    {
        perror("recv form kernel error\n");
        close(skfd);
        exit(-1);
    }

    printf("msg receive from kernel:%s\n", info.data);

	memcpy(NLMSG_DATA(nlherder), data, strlen(data));
    ret = sendto(skfd, nlherder, nlherder->nlmsg_len, 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_nl));

    if(!ret)
    {
        perror("sendto error1\n");
        close(skfd);
        exit(-1);
    }

    close(skfd);

    free((void *)nlherder);
    return 0;

}

