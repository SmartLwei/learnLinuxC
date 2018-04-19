
//处理设备层到来的数据
//不考虑数据中的任何数据结构和封装方式
int deal_fac_data(int8_t* buf, uint16_t len);

//处理安全层到来的数据
//不考虑数据中的任何数据结构和封装方式
int deal_sec_data(int8_t* buf, uint16_t len);

//工具类函数，以十六进制的形式打印字符串
//@charPerLine 每行打印多少个字符
//@ifCopy 如果为1，每个字符前都打印0X，便于从标准输出拷贝，然后粘贴到需要的地方，如初始化一个字符串
int printh(unsigned char* buf, int len, int charPerLine, int ifCopy );


