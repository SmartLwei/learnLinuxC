#include <linux/kernel.h>
#include "its_netlink.h"

int deal_fac_data(int8_t* buf, uint16_t len)
{
    //以下是一个处理安全层到来的数据的例子，仅做打印数据和转发数据到设备层处理
    printk("data from facility layer is \n");
    printh(buf,len,10,0);
    send_msg(buf,len,NET_SEC);
    return 0;
}

int deal_sec_data(int8_t* buf, uint16_t len)
{
    //以下是一个处理设备层到来的数据的例子，仅做打印数据和转发数据到安全层处理
    printk("data from security layer is \n");
    printh(buf,len,10,0);
    send_msg(buf,len,NET_FAC);
    return 0;
}


int printh(unsigned char* buf, int len, int charPerLine, int ifCopy)
{
    if(ifCopy == 0)
    {
        printk("0X");
        for(int i=0; i<len; i++)
        {
            printk("%02X",buf[i]);
            if((i+2)%charPerLine == 0)
                printk("\n");
        }
        if(((len+2)%charPerLine) != 0)
            printk("\n");
        return len;
    }
    else
    {
        for(int i=0; i<len; i++)
        {
            printk("0X%02X, ",buf[i]);      //便于拷贝
            if(i%charPerLine == 0)
                printk("\n");
        }
        if((len%charPerLine) != 0)
            printk("\n");
        return len;
    }
}
