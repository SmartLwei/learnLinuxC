/*******************************
file:           its_netlink.c
description:    实现网络层与安全层的通信
                实现网路成与传送层的通信
kernel:         linux-
author:         Lwei
email:          418877608@qq.com
*******************************/
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <linux/netlink.h>

#define ITS_NET 25          

//安全层的port约定
//(随便写的，只要与用户态使用是相同且不冲突就行)
#define SEC_PORT    2000
//设备层的port约定
#define FAC_PORT    2001

MODULE_LICENSE("GPL");  
MODULE_AUTHOR("Lwei");  
MODULE_DESCRIPTION("its_netlink_demo");


//类似于一个服务器fd
//与用户态的int fd=socket(协议族，传输类型，子协议)做类比
//监听自定义协议
static struct sock *netlinkfd = NULL;


static int printh(unsigned char* buf, int len, int charPerLine, int ifCopy)
{
    int i = 0;
    int j = 0;
    unsigned char temp = 0;
    char *bufH = kmalloc(2*len +3,GFP_KERNEL);
    if(len > 2000)
    {
        printk("打印长度%d大于2000，我们只打印前2000字节",len);
        len = 2000;
    }
    //2*len 每个字符十六进制打印时站两个字节
    //3 = '0'+'X'+最后的'\0';

    if(bufH == NULL)
        printk("error in kmallc");
    bufH[0] = '0';
    bufH[1] = 'X';
    for(i=0,j=2;i<len;i++,j+=2)
    {
        temp = buf[i]/16;
        if(temp < 0)
        {
            printk("error in printk: temp<0");
            return -1;
        }
        else if(temp < 10)
            bufH[j] = temp + '0';
        else if(temp < 16)
            bufH[j] = temp - 10 + 'A';
        else
        {
            printk("error in printk: temp>=16");
            return -1;
        }
        
        temp = buf[i]%16;
        if(temp < 0)
        {
            printk("error in printk: temp<0");
            return -1;
        }
        else if(temp < 10)
            bufH[j+1] = temp + '0';
        else if(temp < 16)
            bufH[j+1] = temp - 10 + 'A';
        else
        {
            printk("error in printk: temp>=16");
            return -1;
        }
    }
    bufH[2*len +3] = '\0';
    printk("%s",buf);
    printk("%s",bufH);
    kfree(bufH);
    return 2*len +3;
}


//消息发送函数,发送到安全层还是设备层由port指定
//该函数只发送消息，消息内部的数据结构不考虑
int send_msg(unsigned char *sendBuf, int len, int port)
{
    struct sk_buff *nl_skb;
    struct nlmsghdr *nlh;

    int ret;
    
    //GFP_ATOMIC为内核中的一种内存分配方式
    nl_skb = nlmsg_new(len, GFP_ATOMIC);
    if(!nl_skb)
    {
        printk("netlink_alloc_skb error\n");
        return -1;
    }

    nlh = nlmsg_put(nl_skb, 0, 0, port, len, 0);
    if(nlh == NULL)
    {
        printk("nlmsg_put() error\n");
        nlmsg_free(nl_skb);
        return -1;
    }
    memcpy(nlmsg_data(nlh), sendBuf, len);
    
    //USER_PORT好像可以从接收缓存中的nlh中取出
    //不过在我们事先约定好的情况下,可以直接使用约定的值
    ret = netlink_unicast(netlinkfd, nl_skb, port, MSG_DONTWAIT);

    return ret;
}

//当系统收到安全层的消息时的回调函数
static void recv_cb(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = NULL;
    void *data = NULL;
    uint32_t fromWhere = 0;		//facility or security
    printk("skb->len:%d\n", skb->len);
    if(skb->len >= nlmsg_total_size(0))
    {
        nlh = nlmsg_hdr(skb);
        fromWhere = nlh->nlmsg_pid;
        data = NLMSG_DATA(nlh);
        if(data)
        {
            //调用自定义的函数处理数据
            // test
            printh(data, nlmsg_len(nlh),10,0);
            if(fromWhere == SEC_PORT)
                send_msg(data, nlmsg_len(nlh), FAC_PORT);
            else if(fromWhere == FAC_PORT)
                send_msg(data, nlmsg_len(nlh), SEC_PORT);
            else
                printk("don't know where the data come from\n");
        }
    }
} 


//设置子协议收到消息时的参数，其中包括回调函数
struct netlink_kernel_cfg cfg = 
{
    //设置回调函数，系统将使用struct sk_buff *作为回调函数的参数
    .input = recv_cb,
};



static int __init test_netlink_init(void)
{
    printk("init netlink_demo!\n");

    //向系统注册我们自定义的NET_SEC子协议和NET_FAC子协议
    //如果我们没有向系统注册该子协议，则在用户态编程时
    //skfd = socket(AF_NETLINK, SOCK_RAW, NET_SEC);函数会返回-1
    netlinkfd = netlink_kernel_create(&init_net, ITS_NET, &cfg);
    
    if(!netlinkfd)
    {
        printk(KERN_ERR "can not create a netlink socket!\n");
        return -1;
    }
    
    printk("netlink demo init ok!");
    return 0;
    //虽然程序在这里返回了，但是通过上面的netlink_kernel_create系统调用
    //已经告诉了操作系统：如果来了ITS_NET协议的socket请求以及消息
    //就调用cfg中的回调函数
}

static void __exit test_netlink_exit(void)
{
    //注销新建的子协议
    sock_release(netlinkfd->sk_socket); 
    printk(KERN_DEBUG "netlink exit\n!");
}

//模块insmod时调用的函数
module_init(test_netlink_init);

//模块rmmod时调用的函数
module_exit(test_netlink_exit);






