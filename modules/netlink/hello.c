//必要的头文件
//（注：编译Linux内核所用的头文件与寻常的c不一样）
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

//模块加载函数（不可少，必须以"module_init(函数名)"的形式来指定）
static int __init hello_init(void) //必须有返回值，若成功，返回0，若返回值不为0模块将会出错
{
    //注：printk可以实现与printf相似的功能，printf不能应用于系统内核编译
    //注：printk输出的信息需要使用dmesg命令来查看
    printk( "Hello World enter/n");
    return 0;
}

//模块卸载函数（不可少，必须以"module_exit(函数名)"的形式来指定）
static void __exit hello_exit(void) //不能有返回值
{
    printk( "Hello World exit/n");
}

//模块的注册
module_init(hello_init);	//驱动程序初始化的入口点，相当于c语言程序中的main()函数
module_exit(hello_exit); 	//模块退出

//模块许可证声明（不可少，如果缺失，模块被加载时，将收到内核被污染(kernel tainted)的警告）
MODULE_LICENSE("GPL");

//声明模块的作者（可选）
MODULE_AUTHOR("Lwei");
//声明模块的描述（可选）
MODULE_DESCRIPTION("This is a simple example!/n");
//声明模块的别名（可选）
MODULE_ALIAS("A simplest example");
