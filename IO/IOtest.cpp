/*
C++中的IO类
该文件中为最常用的IO方法

<iostream> <istream> <ostream>
<fstream> <ifstream> <ofstream>
<sstream> <istringstream> <ostringstream>

常用的条件状态
strm::iostate, strm::badbit, strm::failbit, strm::eofbit, strm::goodbit
(本质：typedef int strm::iostate)

常用的判断
s.eof(), s.fail(), s.good()

调整和获取io的状态
s.clear(), s.clear(flags), s.setstate(flags), strm::iostate flags=s.rdstate()
例如复位failbit和badbit：cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit);

刷新缓冲区：unitbuf操纵符(每次cout直接输出，而不是缓存)，
flush操纵符(立即将cout缓存刷新到终端)，ends操纵符(刷新前会插入一个空白字符)

*/

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

//缓冲区刷新测试
void test1()
{
#define TIMEWAIT 2
	//下面的输出语句不会立即输出
	//测试平台为windows10和linux(ubuntu 1804 gcc)
	//但是在windows中实测结果为：会立即输出
	cout << "典型cout测试" << endl;
	cout << "缓冲区内容，请判断他们是否立即输出到屏幕";
	sleep(TIMEWAIT);
	cout << endl;
	
	//在linux和windows中会立即输出
	cout << "flush测试" << endl;
	cout << "缓冲区内容，请判断他们是否立即输出到屏幕" << flush;
	sleep(TIMEWAIT);
	cout << endl;
	
	//在linux和windows中会立即输出
	//在windows中会在末尾输出一个空格，且立即刷新
	//但是在linux中没有输出空格，没有立即刷新缓冲区
	cout << "ends测试" << endl;
	cout << "缓冲区内容，请判断他们是否立即输出到屏幕" << ends;
	sleep(TIMEWAIT);
	cout << endl;
	
	//endl会输出一个换行符并刷新缓冲区
	cout << "endl测试" << endl;
	cout << "缓冲区内容，请判断他们是否立即输出到屏幕" << endl;
	sleep(TIMEWAIT);
	cout << endl;
	
	//unitbuf / nounitbuf控制刷新方式
	//设置unitbuf后，cout不再等待
	cout << "unitbuf测试" << endl;
	cout << unitbuf;
	cout << "缓冲区内容，请判断他们是否立即输出到屏幕";
	cout << "缓冲区内容，请判断他们是否立即输出到屏幕";
	cout << "缓冲区内容，请判断他们是否立即输出到屏幕";
	sleep(TIMEWAIT);
	cout << endl;
	
	//设置nounitbuf后，cout恢复原始的等待结束符刷新缓冲区的状态
	cout << "nounitbuf测试" << endl;
	cout << nounitbuf;
	cout << "缓冲区内容，请判断他们是否立即输出到屏幕";
	sleep(TIMEWAIT);
	cout << endl;
	
	//尝试在cerr中设置nounitbuf
	//从结果看，设置nounitbuf后，没有改变cerr的输出缓存方式，仍然是立即输出
	cout << "对cerr设置nounitbuf测试" << endl;
	cerr << nounitbuf;
	cerr << "缓冲区内容，请判断他们是否立即输出到屏幕";
	sleep(TIMEWAIT);
	cout << endl;
}


int main()
{
	test1();
	cout << "缓存策略测试完毕，按任意键盘继续" << endl;
	cin.get();
	return 0;
}







