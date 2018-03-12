/*
测试从char* 转换为int* 会不会出错
有的系统好像是（忘了在哪里听说过了）必须4字节对齐了才能够正确转换，
即所有的char类型的存储模式都为4字节，或者说每一个int类型的地址必须
是4的倍数
但是我不确定该说法，所以写了这个程序验证一下
从结果看，以上说法纯属谣言
*/


#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	char buf[10] = { 0x11,0x22,0X33,0X44,0X55,0X66,0X77,0X88,0x99,0X00 };
	int value = 1;
	char *charptr = (char *)&value;	//小端系统
	if (*charptr == 1)
		printf("小端系统\n");
	else
		printf("大端系统\n");
	for (int i = 0; i < 7; i++)
	{
		cout << i << ": ";
		int *intptr = (int*)&buf[i];
		value = *intptr;
		cout << value;
		printf("\t%X\n", value);
	}
	getchar();
    return 0;
}
