/**
* 字节对其规则
* 对象大小为k字节，则其地址必须是k的倍数
*/



#include <iostream>
using namespace std;

//从结果来看，32位和64位下 sizeof(A) = 8
struct A
{
	A() {}
	~A() {}
	int m1;
	int m2;
};

//从结果来看，32位和64位下 sizeof(A) = 16
struct B:A
{
	B() {}
	~B() {}
	int m1;
	short m2;		//会对齐到4字节
	static char m3;	//静态成员不占用该类的sizeof
};

//从结果来看，32位下sizeof(C) = 12， 64位下 sizeof(C) = 16;
struct C
{
	C() {}
	virtual ~C() {}	//虚成员32位下为4字节,64位linux下为8字节
	int m1;
	short m2;		//对齐，4字节
};

//从结果来看，32位和64位下 sizeof(D) = 1
struct D
{};

//从结果来看，32位和64位下 sizeof(E) = 8
struct E
{
	int m1;
	char m2;
	char m3;
};

//从结果来看，32位和64位下 sizeof(F) = 16
struct F
{
	short m1;
	char m2;
	short m3;
	char m4;
	short m5;
	char m6;
	short m7;
	char m8;
};

//从结果来看，32位和64位下 sizeof(F) = 12
struct G
{
	short m1;
	short m3;
	short m5;
	short m7;
	char m2;
	char m4;
	char m6;
	char m8;
};



int main(int argc, char* argv[])
{
	cout << "sizeof(A) = " << sizeof(A) << endl;
	cout << "sizeof(B) = " << sizeof(B) << endl;
	cout << "sizeof(C) = " << sizeof(C) << endl;
	cout << "sizeof(D) = " << sizeof(D) << endl;
	cout << "sizeof(E) = " << sizeof(E) << endl;
	cout << "sizeof(F) = " << sizeof(F) << endl;
	cout << "sizeof(G) = " << sizeof(G) << endl;
	return 0;
}

















