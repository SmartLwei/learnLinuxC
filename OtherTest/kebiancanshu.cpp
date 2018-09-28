/**
* 可变参数编程
* va_list 类型用来接受"..."的全部参数，本质上是char* 
* 即typedef char *va_list;
*
* va_start(va_list, parm)
* va_arg(va_list, return_type)
* va_end(va_list)
*/

#include <stdarg.h>
#include <stdio.h>

#define DEBUG_PRINT_D(N) printf(#N" = %f\n", N);
#define DEBUG_PRINT_I(N) printf(#N" = %X\n", N);

/**
* 求n个int数的平均数
*/
double Average_int(int n, ...)
{
	va_list valist;
	double sum = 0;
	va_start(valist, n);
	for (int i = 0; i < n; i++)
	{
		sum = sum + va_arg(valist, int);
	}
	va_end(valist);
	return sum / n;
}

/**
* 求n个long long数的平均数
*/
double Average_longlong(int n, ...)
{
	va_list valist;
	double sum = 0;
	va_start(valist, n);
	for (int i = 0; i < n; i++)
	{
		sum = sum + va_arg(valist, long long);
	}
	va_end(valist);
	return sum / n;
}


/**
* 求n个double数的平均数
*/
double Average_double(int n, ...)
{
	va_list valist;
	double sum = 0;
	va_start(valist, n);
	for (int i = 0; i < n; i++)
	{
		sum = sum + va_arg(valist, double);
	}
	va_end(valist);
	return sum / n;
}

/**
* 按事务等级决定是否打印一个参数
*/
int printLevel = 3;
int MyPrintf(int level, const char* fmt, ...)
{
	va_list valist;
	va_start(valist, fmt);
	if (level <= printLevel)
	{
		return vprintf(fmt, valist);
	}
	va_end(valist);
	return 0;
}


int main()
{
	//求多个int类型值的平均数
	int int1 = 1, int2 = 2, int3 = 3, int4 = 4, int5 = 5;
	DEBUG_PRINT_D(Average_int(1, int1));
	DEBUG_PRINT_D(Average_int(2, int1, int2));
	DEBUG_PRINT_D(Average_int(3, int1, int2, int3));
	DEBUG_PRINT_D(Average_int(4, int1, int2, int3, int4));
	printf("\n");

	//尝试错误的数量参数
	DEBUG_PRINT_D(Average_int(3, int1, int2, int3, int4));
	DEBUG_PRINT_D(Average_int(5, int1, int2, int3, int4));
	DEBUG_PRINT_D(Average_int(6, int1, int2, int3, int4));
	printf("\n");
	printf("\n");

	//求多个double类型值的平均数
	double d1 = 1.0, d2 = 2.0, d3 = 3.0, d4 = 4.0, d5 = 5.0;
	DEBUG_PRINT_D(Average_double(1, d1));
	DEBUG_PRINT_D(Average_double(2, d1, d2));
	DEBUG_PRINT_D(Average_double(3, d1, d2, d3));
	DEBUG_PRINT_D(Average_double(4, d1, d2, d3, d4));
	printf("\n");

	//尝试错误的数量参数
	DEBUG_PRINT_D(Average_double(3, d1, d2, d3, d4));
	DEBUG_PRINT_D(Average_double(5, d1, d2, d3, d4));
	DEBUG_PRINT_D(Average_double(6, d1, d2, d3, d4));
	printf("\n");
	printf("\n");

	//求多个long long类型值的平均数
	long long ll1 = 1ll, ll2 = 2ll, ll3 = 3ll, ll4 = 4ll, ll5 = 5ll;
	DEBUG_PRINT_D(Average_longlong(1, ll1));
	DEBUG_PRINT_D(Average_longlong(2, ll1, ll2));
	DEBUG_PRINT_D(Average_longlong(3, ll1, ll2, ll3));
	DEBUG_PRINT_D(Average_longlong(4, ll1, ll2, ll3, ll4));
	printf("\n");

	//尝试错误的数量参数
	DEBUG_PRINT_D(Average_longlong(3, ll1, ll2, ll3, ll4));
	DEBUG_PRINT_D(Average_longlong(5, ll1, ll2, ll3, ll4));
	DEBUG_PRINT_D(Average_longlong(6, ll1, ll2, ll3, ll4));
	printf("\n");
	printf("\n");

	//尝试错误的数据类型参数
	DEBUG_PRINT_D(Average_int(4, ll1, ll2, ll3, ll4));
	DEBUG_PRINT_D(Average_double(4, ll1, ll2, ll3, ll4));
	printf("\n");
	printf("\n");

	//尝试探索一下va_arg的本质
	char arr[100] = { 0x05,0x00,0x00,0x00, 0x06,0x00,0x00,0x00,0x07,0x00,0x00,0x00 };
	va_list valist = arr;
	DEBUG_PRINT_I(va_arg(valist, int));
	DEBUG_PRINT_I(va_arg(valist, int));
	DEBUG_PRINT_I(va_arg(valist, int));
	//从结果看，可变个数参数其实是对字符串的不同"解释"
	//执行va_arg时，讲valist指向的内存区解释为相应的类型，并将valist向后移动几个字节
	printf("\n");
	printf("\n");

	//按照等级打印参数
	MyPrintf(2, "MyPrintf i = %d\n", int2);
	MyPrintf(3, "MyPrintf i = %d\n", int3);
	MyPrintf(4, "MyPrintf i = %d\n", int4);
	MyPrintf(5, "MyPrintf i = %d\n", int5);
	printf("\n");

	printLevel = 5;
	MyPrintf(2, "MyPrintf i = %d\n", int2);
	MyPrintf(3, "MyPrintf i = %d\n", int3);
	MyPrintf(4, "MyPrintf i = %d\n", int4);
	MyPrintf(5, "MyPrintf i = %d\n", int5);
	printf("\n");
	printf("\n");
	getchar();
	return 0;
}
