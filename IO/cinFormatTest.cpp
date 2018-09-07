/*
cin的控制信息
#define EOF -1
skipws，noskipws(输入时是否跳过空白字符)

is.get(ch)		从is读取下一个字节并存入ch中，返回is
os.put(ch)		将字符ch输入到os中，返回os
is.get()		将is的下一字节作为int返回
				警告，返回值为int不是char，如果使用
				char c=cin.get()可能会出错，因为char的范围为-128~127，
				因此可能返回负数。而int可以存放0-255所有的可能输出
is.putback(ch)	将字符ch放回is，返回is
is.unget()		将is向后移动一个字节，返回is
is.peek()		将下一字节作为int返回，但是不从流中删除它

is.get(sink, size, delim)	从is中读取最多size个字节并保存到数组中
							直到读取到eof或size个字符或delim(delim保留在流中)
is.getline(sink, size, delim)	与上面的函数功能类似，只是delim从流中删除
is.read(sink, size)		读取最多size个字节到sink中，返回is
is.gcount() 			返回上一个未格式化读取操作从is读取的字节数
os.write(sorce,size)	将sorce中size个字节写入os，返回os
is.ignore(size=1, delim=eof)	读取并忽略最多size个字符，包括delim
is.clear()		清除流的状态位

以下操作仅适用于fstream和sstream
tellg()	tellp()	返回一个输入流(tellg)或输出流(tellp)标记的当前位置
seekg(pos) seekp(pos)	在一个输入流或输出流中将标记重定位到给定的绝对地址
						pos通常是tellg或tellp返回的值
seekg(off, from)
seekp(off, from)	在一个输入和输出流中将标记从定位到from之前或之后off个字符
					from可以为：beg, cur, end
					

 
*/


#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <memory.h>
using namespace std;

//因为我们会尝试进行文件处理，因此我们需要先新建一个文件
void ResetFile()
{
	//先为fstream做好准备
	string oriStr="12345 67890 123 45ab 789";
	fstream fileOut;
	//删除文件
	unlink("fileTest.txt");
	//新建文件
	fileOut.open("fileTest.txt",fstream::out);
	if(!fileOut.is_open())
		cerr << "open file failed" << endl;
	fileOut << oriStr << endl;
	fileOut.close();
}

//清楚io的状态并回到初始位置
void InitState(iostream& os)
{
	os.clear();
	os.seekg(0,iostream::beg);
	os >> skipws;
}

int main()
{
	ResetFile();
	string str;
	int value;
	char buf[1000];
	fstream fileIn("fileTest.txt", fstream::in|fstream::out);
	if(!fileIn.is_open())
		cerr << "open file failed" << endl;
	
	//打印文件初始位置作为参考。可以发现，文件中下标从0开始
	cout << fileIn.tellg() << endl;
	
	//从结果来看，因为文件中有空格，因此读取的结果为五个字符串
	//与预计结果相同，读取到5个字符串
	cout << "获取文件的内容" << endl;
	while(fileIn >> str)
	{
		cout << "str = " << str << endl;
		cout << fileIn.tellg() << endl;
	}
	InitState(fileIn);	
		
	
	//尝试将文件读取为数字
	//读取到4个数字，并且在遇到ab字符时出错
	cout << endl << "尝试将文件内容读取为数字" << endl;
	while(fileIn >> value)
	{
		cout << "value = " << value << "\t" << hex << showbase << value << dec << endl;
		cout << fileIn.tellg() << endl;
	}
	InitState(fileIn);
	
	
	//尝试不要跳过空白字符读取字符串
	cout << endl << "不跳过空白符获取字符串" << endl;
	//从结果来看，设置noskipws后,只能获取到一个字符串，fileIn卡在了空格处
	fileIn >> noskipws;
	while(fileIn >> str)
	{
		cout << "str = " << str << endl;
		cout << fileIn.tellg() << endl;
	}
	InitState(fileIn);
	
	
	//尝试不跳过空白字符读取数字
	cout << endl << "不跳过空白符获取数字" << endl;
	//从结果看，设置noskipws后，只能获取到一个数字，fileIn卡在了空格处
	fileIn >> noskipws;
	while(fileIn >> value)
	{
		cout << "value = " << value << "\t" << hex << showbase << value << dec << endl;
		cout << fileIn.tellg() << endl;
		//使用fileIn.get();跳过空格后可以输出4个数字
	}
	InitState(fileIn);
	
	
	//使用in.get(ch)获取字符
	//从结果看，in.get(ch)可以获取所有字符，包括空格和换行符
	cout << endl << "使用get(ch)获取字符" << endl;
	char ch = 0;
	while(fileIn.get(ch))
	{
		cout << ch;
	}
	cout << endl;
	InitState(fileIn);
	
	
	//使用in.get()获取数字
	//从结果看，in.get()可以获取所有字符，每个字符以int值的返回ASCII码，失败时返回-1(EOF)
	cout << endl << "使用get()获取字符的ASCII码" << endl;
	while((value = fileIn.get()) != EOF)
	{
		cout << value << " ";
	}
	cout << endl;
	InitState(fileIn);
	

	//使用putback(ch)尝试往流中写回数据
	cout << endl << "使用putback(ch)将ch写回到文件流中" << endl;
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	cout << endl;
	//接下来尝试写回字符
	//实测发现,最多只能使用一次putback(ch),否则fineIn会出错
	//并且，putback(ch)并不会影响原始文件。
	fileIn.putback('A');
	//fileIn.putback('A');//不允许第二次调用，否则下面的getline无法获取数据
	//获取剩余的字符
	str = "";
	getline(fileIn,str); cout << str << endl;
	InitState(fileIn);
	//从结果来看，原始文件并没有被改变 ubuntu180432
	str = "";
	getline(fileIn,str); cout << str << endl;
	InitState(fileIn);
	
	
	//使用sstream尝试一下putback()是否会影响sstream中的内容
	cout << endl << "使用putback(ch)将ch写回到内存流中" << endl;
	str = "abcdefg";
	stringstream sstr(str);
	sstr.get(ch); cout << ch << " ";
	sstr.get(ch); cout << ch << " ";
	sstr.get(ch); cout << ch << " ";
	cout << endl;
	//对sstream可以调用多次putback(ch)
	sstr.putback('A');
	//sstr.putback('A');
	//sstr.putback('A');
	//sstr.putback('A'); //当到达sstream的首部时，继续putback(ch)会使sstream失效
	str = "";
	getline(sstr,str); cout << str << endl;
	InitState(sstr);
	//从结果来看，sstr中的内容已经被改变了
	str = "";
	getline(sstr,str); cout << str << endl;
	//从上面连个例子可以看出，putback()不会影响文件流，但会影响内存流
	
	
	//尝试在cin中使用putback()
	cout << endl << "使用putback(ch)将ch写回到cin中" << endl;
	//从结果看，cin可以重复多次putback(ch)
	//当到达cin的首部时，还可以继续putback(ch),且会使得输入缓存变大
	cin.putback('\n');
	cin.putback('F');
	cin.putback('E');
	cin.putback('D');
	cin.putback('C');
	cin.putback('B');
	cin.putback('A');
	str = "";
	//cin.putback(ch)居然可以这么用！！！
	getline(cin, str); cout << str << endl;
	
	
	//使用unget()将pos向后移动一个字节
	cout << endl << "使用unget()将pos向后移动两个字节" << endl;
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	cout << endl;
	//接下来尝试回退两个字节
	fileIn.unget();
	fileIn.unget();
	//再重新获取字符
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	cout << endl;
	InitState(fileIn);
	
	
	//使用unpeek()读取流中的下一字节，但是不从流中删除该字节
	cout << endl << "peek()测试" << endl;
	value = fileIn.peek(); cout << "value = " << value << endl;
	value = fileIn.peek(); cout << "value = " << value << endl;
	value = fileIn.peek(); cout << "value = " << value << endl;
	value = fileIn.get();  cout << "value = " << value << endl;
	value = fileIn.peek(); cout << "value = " << value << endl;
	value = fileIn.peek(); cout << "value = " << value << endl;
	InitState(fileIn);
	
	
	//流的随即访问
	cout << endl << "流的随机访问" << endl;
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	//保存当前的位置
	auto pos = fileIn.tellg();
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.seekg(pos);
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	cout << endl;
	//跳转到倒数第三个字符
	fileIn.seekg(-3,fstream::end);
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	cout << endl;
	InitState(fileIn);
	
	
	//ignore的测试
	cout << endl << "ignore测试" << endl;
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	//从测试结果来看，ignore遇到delim后就停止
	//如果一直没有遇到delim,则ignore size个字符
	fileIn.ignore(2,' ');
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	fileIn.get(ch); cout << ch << " ";
	cout << endl;
	
	getchar();
	unlink("fileTest.txt");
	return 0;
}
