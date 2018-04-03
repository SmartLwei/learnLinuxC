
//网络层与安全层的协议约定
#define NET_SEC 25          
//网络层与设备层的协议约定
#define NET_FAC 26
//安全层的port约定
//(随便写的，只要与用户态使用是相同且不冲突就行)
#define SEC_PORT    2000
//设备层的port约定
#define FAC_PORT    2001

//消息发送函数,发送到安全层还是设备层由port指定
//该函数只发送消息，消息内部的数据结构不考虑
//port为SEC_PORT或FAC_PORT
int send_msg(int8_t *sendBuf, uint16_t len, int port);



