/*
文件io操作
<fstream>
fstream s; fstream s(name); fstream s(name, mode);
s.open(name,mode); s.close(); s.is_open();
mode有如下参数(使用时需要使用域说明符 steam::)
in, out, app, ate, trunc, binary
注意：如果mode为空，则默认不创建新文件
out模式能够新建文件
out模式隐含trunc模式
app模式隐含out模式（因此也能创建文件）

*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

//以十六进制的方式打印一个字符串
void printh(const string& str)
{
	for(int i=0; i<str.length(); i++)
	{
		cout << hex << setw(2) << setfill('0') << (int)(unsigned char)str[i] << " ";
	}
	cout << endl;
}

void process(ifstream& is)
{
	string str;
	while(getline(is, str))
		printh(str);
}


int main(int argc, char* argv[])
{
	for(auto p=argv; p!=argv+argc; ++p)
	{
		ifstream input(*p);
		if(input)
			process(input);
		else
			cerr << "could not open " << *p << endl;
	}
	return 0;
}
