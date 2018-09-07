/*
cout 格式化输出
常用控制信息
boolalpha , noboolalpha(true打印为true还是1), showbase, noshowbase(十六进制或八进制打印时是否打印0x或0), 
showpoint, noshowpoint(没有小数时是否输出小数部分)，showpos, noshowpos(正数是否打印+号),
uppercase, nouppercase(十六进制输出时，输出0x还是0X)，
dec, otc, hex 不同的进制输出
fixted(定点十进制), scientific(科学表示法)，hexfloat(十六进制打印小数)，defaultfloat(默认方式)
cout.precision()获取当前的精度，cout.precision(int)设置精度
unitbuf, nounitbuf(是否需要缓存打印还是立即打印)
flush(立即刷新缓冲区)，ends(插入空格后立即刷新缓冲区)，endl(插入换行符后立即刷新缓冲区)
left, right, internal(插入填充字符时的对齐方式)

下面的控制函数必须include<iomanip>
setw(int) 指示下一个下一个下一个数字或字符串值的最小空间，只生效一次
setprecision(n) 设置浮点数的精度(重复)
setfill(char) 设置填充字符(默认为空格)
setbase(b)	设置进制为b进制，b只能为8，10，16之一

*/


#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main()
{
	//bool类型的打印
	//默认情况下将true打印为1，false打印为0
	cout << "true = " << true << endl;
	cout << "false = " << false << endl;
	
	//使用boolalpha将bool类型打印为字符串true和false
	//除非用noboolalpha取消按字符串打印bool，否则该设置一直生效
	cout << "after cout << boolalpha" << endl;
	cout << boolalpha;
	cout << "true = " << true << endl;
	cout << "false = " << false << endl;
	
	//使用noboolalpha将bool类型打印为数字1和0
	cout << "after cout << noboolalpha" << endl;
	cout << noboolalpha;
	cout << "true = " << true << endl;
	cout << "false = " << false << endl;
	
	//改变输出的进制
	//除非使用另外一种进制重新设置输出进制，否则进制一直生效
	cout << endl << "输出进制测试" << endl;
	cout << "default: " << 20 << " " << 1024 << endl;
	cout << oct;
	cout << "octal: " << 20 << " " << 1024 << endl;
	cout << hex;
	cout << "hex: " << 20 << " " << 1024 << endl;
	cout << dec;
	cout << "decimal: " << 20 << " " << 1024 << endl;
	
	//输出不同进制时，输出进制提示符(首字符为0表示八进制，首字符0x表示十六进制)
	//uppercase可以控制十六进制输出时字母的大小写
	//除非使用noshowbase，否则showbase持续生效；uppercase也是如此
	cout << endl << "输出进制描述符" << endl;
	cout << showbase << uppercase;
	cout << "default: " << 20 << " " << 1024 << endl;
	cout << oct;
	cout << "octal: " << 20 << " " << 1024 << endl;
	cout << hex;
	cout << "hex: " << 20 << " " << 1024 << endl;
	cout << dec;
	cout << "decimal: " << 20 << " " << 1024 << endl;
	cout << noshowbase << nouppercase;
	
	//浮点数的输出控制
	//默认的输出精度为6
	cout << endl << "输出精度控制" << endl;
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(-1);	//设置精度为负数，但是实际打印时仍然按照6位打印
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(0);	//用科学表示法，只保留一个有效数字
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(1);	//用科学表示法，只保留一个有效数字
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(12);
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(32);
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(52);	//ubuntu180432环境下，最多输出52个有效数字
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(99);	//实际环境下并不能输出99个有效数字,ubuntu180432环境下，最多输出52个有效数字
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(6);

	//浮点数的其他控制
	cout << endl << "浮点数的其他控制" << endl;
	//默认情况下，如果小数没有小数部分，则不输出小数点
	cout << sqrt(1.0) << endl;	//输出为1
	//即便小数不包含小数部分，也打印小数点.，并且按照精度在小数部分输出0
	cout << showpoint;	//输出为1.00000
	cout << sqrt(1.0) << endl;
	cout << noshowpoint;	//输出为1
	cout << sqrt(1.0) << endl;
	
	//对正数输出正号+
	cout << endl << "+号打印测试" << endl;
	cout << showpos;
	cout << 1 << endl;
	cout << sqrt(200.0) << endl;
	cout << noshowpos;
	cout << 1 << endl;
	cout << sqrt(200.0) << endl;

	//对小数按照定点时进制的方式显示
	cout << endl << "定点十进制显示小数" << endl;
	cout << fixed;
	//从结果来看，当设置定点时进制后，precision控制的是小数点后的位数，而不是有效数字的位数
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(-1); //使用默认值6，打印6位小数，结果为14.142136
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(0);	//不打印小数点，且没有小数部分，结果为14
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(1);	//打印一位小数，结果为14.1
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(12);	//打印12位小数，结果为14.142135623731
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(6);
	
	//对小数按照科学表示法的方式显示
	cout << endl << "科学表示法打印小数" << endl;
	cout << scientific;
	//从结果来看，当设置科学表示法后，小数部分的有效数字部分为precision
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(-1); //使用默认值6，打印6位小数，结果为1.414214e+01
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(0);	//不打印小数点，且没有小数部分，结果为1e+01
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(1);	//打印一位小数，结果为1.4e+01
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(12);	//打印12位小数，结果为1.414213562373e+01
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(6);

	//对小数按照十六进制打印
	cout << endl << "十六进制形式打印小数" << endl;
	cout << hexfloat;
	//以下cout都输出0x1.c48c6001f0acp+3，从结果看此时的输出与精度无关
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(-1); 
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(0);
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(1);
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(12);
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(52);
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(6);

	//以默认方式打印小数
	cout << defaultfloat;
	cout << endl << "默认方式打印小数" << endl;
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(-1);	//14.1421
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(0);	//1e+01
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(1);	//1e+01
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(12);	//14.1421356237
	cout << "precision: " << cout.precision() << ", value: " << sqrt(200.0) << endl;
	cout.precision(6);

	//设置打印宽度
	int i=-65432;
	double d = 3.141592;
	//显示正号
	cout << showpos;
	cout << endl << "设置打印宽度和填充字符" << endl;
	cout << "默认情况下右对齐" << endl;
	cout << "i=" << setw(12) << i << "flag" << endl;
	cout << "d=" << setw(12) << d << "flag" << endl;
	cout << "设置左对齐" << endl;
	cout << left;
	cout << "i=" << setw(12) << i << "flag" << endl;
	cout << "d=" << setw(12) << d << "flag" << endl;
	cout << "设置两端对齐" << endl;
	cout << "设置以#填充" << endl;
	cout << internal;
	cout << setfill('#');
	cout << "i=" << setw(12) << i << "flag" << endl;
	cout << "d=" << setw(12) << d << "flag" << endl;
	
	return 0;
}

