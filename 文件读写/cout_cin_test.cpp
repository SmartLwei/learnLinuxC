/**
* cout 和 cerr 刷新缓冲区的测试
* cout 并不是实时地刷新输出到屏幕，而是遇到 \n ,endl, 或者cin 等时，才会刷新数据到屏幕
* cerr 实时地刷新内容到屏幕
* clog 有的资料上说它是非实时的（w3c School），也就是先缓冲，遇到刷新符号再输出到屏幕，
*      但是从这个测试结果来看，clog是实时地刷新到缓冲区的 （测试环境 Ubuntu 1604 32bit）
*/

#include <iostream>
#include <unistd.h>
using namespace std;
#define SLEEP_TIME 5

int main()
{
	cout << "BEGIN" << endl;
	// cout 测试
	cout << endl << "cout Test" << endl;
	cout << "如果这是sleep前看到的最后一句话，说明cout实时刷新缓冲区";
	sleep(SLEEP_TIME);
	cout << endl << "如果上一句话与这句话同时出现，说明cout不是实时刷新缓冲区(预计结果应该是同时出现)" << endl;
	
	cout << "按任意键继续" << endl;
	cin.get();
	
	// cerr 测试
	cout << endl << "cerr Test" << endl;
	cerr << "如果这是sleep前看到的最后一句话，说明cerr实时刷新缓冲区";
	sleep(SLEEP_TIME);
	cerr << endl << "如果上一句话与这句话同时出现，说明cerr不是实时刷新缓冲区(预计结果应该不是同时出现)" << endl;
	
	cout << "按任意键继续" << endl;
	cin.get();
	
	// clog 测试
	cout << endl << "clog Test" << endl;
	clog << "如果这是sleep前看到的最后一句话，说明clog实时刷新缓冲区";
	sleep(SLEEP_TIME);
	clog << endl << "如果上一句话与这句话同时出现，说明clog不是实时刷新缓冲区(预计结果应该是同时出现，但是实际上。。。好像不是)" << endl;
	
	cout << "END" << endl;
	cin.get();
}
