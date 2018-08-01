// 未完成，半成品

#include <iostream>

// 顺序容器
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <array>

// 关联容器
#include <set>
#include <map>

using namespace std;

class MyClass
{
	// 重载<<运算符,用于输出我们的类
	friend ostream& operator<<(ostream& os, const MyClass& a);
private:
	int m_value;
public:
	// 类构造相关
	MyClass() { cout << "MyClass():"<< m_value << endl; }
	MyClass(int value) { m_value = value; cout << "MyClass(int):" << m_value << endl; }
	MyClass(const MyClass& a) { m_value = a.m_value; cout << "MyClass(MyClass&):" << m_value << endl; }
	
	// 类析构相关
	~MyClass() { cout << "~MyClass:" << m_value << endl; }

	int GetValue() { return m_value; }

	// 类运算符重载
	MyClass& operator=(const MyClass &a) { m_value = a.m_value; cout << "operator=" << endl; return *this; }
	bool operator==(const MyClass &a) { cout << "operator== :" << m_value << "==" << a.m_value << endl;  return a.m_value == m_value; }
	bool operator!=(const MyClass &a) { cout << "operator!= :" << m_value << "!=" << a.m_value << endl;  return a.m_value != m_value; }
	bool operator>(const MyClass &a) { cout << "operator> :" << m_value << ">" << a.m_value << endl; return m_value > a.m_value; }
	bool operator<(const MyClass &a) { cout << "operator< :" << m_value << "<" << a.m_value << endl; return m_value < a.m_value; }
};

// 用于向os输出我们的类
ostream& operator<< (ostream& os, const MyClass &a)
{
	os << "os << MyClass:" << a.m_value << endl;
	return os;
}

// 测试我们的定义的类是否有错
int ClassTest()
{
	int a = 1;
	cout << endl << "flag1" <<  endl;
	MyClass my1;

	cout << endl << "flag2" <<  endl;
	MyClass my2(2);

	cout << endl << "flag3" << endl;
	MyClass my3(my2);

	cout << endl << "flag4" << endl;
	MyClass my4 = 4;

	cout << endl << "flag5" << endl;
	my1 = my2 = my3;

	cout << endl << "flag6" << endl;
	my1 = 6;

	cout << endl << "flag7" << endl;
	cout << (my1 > my2) << endl;

	cout << endl << "flag8" << endl;
	cout << (my1 < my2) << endl;

	cout << endl << "flag9" << endl;
	cout << (my1 == my2) << endl;

	cout << endl << "flag10" << endl;
	cout << (my1 != my2) << endl;

	cout << endl << "flag11" << endl;
	cout << my1 << endl;

	cout << endl << "flag12" << endl;
	cout << my1 << my2 << endl;
	
	return 0;

/*
flag1
MyClass():-858993460

flag2
MyClass(int):2

flag3
MyClass(MyClass&):2

flag4
MyClass(int):4

flag5
operator=
operator=

flag6
MyClass(int):6
operator=
~MyClass:6

flag7
operator> :6>2
1

flag8
operator< :6<2
0

flag9
operator== :6==2
0

flag10
operator!= :6!=2
1

flag11
os << MyClass:6


flag12
os << MyClass:6
os << MyClass:2

~MyClass:4
~MyClass:2
~MyClass:2
~MyClass:6
*/
}

int MyTest()
{
	return 0;
}

int StringTest()
{
	cout << "==========StringTest==========" << endl;
	string number = "0123456789";
	string s1 = "Hello(0), This is Hello(1) World!";

	string::size_type pos;
	
	pos = s1.find("Hello");
	cout << "第一个Hello的下标为：" << pos << endl;

	pos = s1.rfind("Hello");
	cout << "最后一个Hello的下标为：" << pos << endl;

	pos = s1.find_first_of(number);
	cout << "第一个数字出现的下标为：" << pos << endl;

	pos = s1.find_last_of(number);
	cout << "最后一个数字出现的下标为：" << pos << endl;

	pos = s1.find_first_not_of(number);
	cout << "第一个不是数字的字符的下标为：" << pos << endl;

	pos = s1.find_last_not_of(number);
	cout << "最后一个不是数字的字符的下标为：" << pos << endl;

	pos = s1.find('o');
	cout << "第一个o出现的下标为" << pos << endl;

	pos = s1.find('o', 10);
	cout << "从第10个位置开始，第一个o出现的下标为:" << pos << endl;

	// 循环查找字符l
	pos = 0;
	int i = 1;
	while ((pos = s1.find_first_of('l', pos)) != string::npos)
	{
		cout << "第" << i++ << "个l出现的下标为：" << pos << endl;
		pos++;
	}

	i = 15;
	s1 = to_string(i);
	cout << i << "转换为字符串的表示为:" << s1 << endl;
	cout << "s1.size() = " << s1.size() << endl;

	double d = stod(s1);
	cout << s1 << "转换为double值为" << d << endl;
	
	string s2 = "pi = .3.14";
	d = stod(s2.substr(s2.find_first_of("+-.0123456789")));
	cout << "d = " << d << endl;

	return 0;
}

int VectorTest()
{
	cout << "==========VectorTest==========" << endl;

	// 初始化相关

	// vMyClass1中没有任何元素
	cout << endl << "flag1" << endl;
	vector<MyClass> vMyClass1;
	cout << "size = " << vMyClass1.size() << endl;

	// vMyClass2中有两个元素，每个元素的值随机
	cout << endl << "flag2" << endl;
	vector<MyClass> vMyClass2(2);
	cout << "size = " << vMyClass2.size() << endl;

	// vMyClass3中有3个元素，每个元素为1
	cout << endl << "flag3" << endl;
	vector<MyClass> vMyClass3(3, 1);
	cout << "size = " << vMyClass3.size() << endl;

	// vMyClass4中有1个元素，值为4
	cout << endl << "flag4" << endl;
	vector<MyClass> vMyClass4 = { 4 };
	cout << "size = " << vMyClass4.size() << endl;

	// vMyClass5中有5个元素，分别为 1 2 3 4 5 
	cout << endl << "flag5" << endl;
	vector<MyClass> vMyClass5 = { 1,2,3,4,5 };
	cout << "size = " << vMyClass5.size() << endl;

	// vMyClass6中有一个元素，使用vMyClass4中的元素拷贝构造的
	cout << endl << "flag6" << endl;
	vector<MyClass> vMyClass6(vMyClass4);
	cout << "size = " << vMyClass6.size() << endl;

	// 首先析构vMyClass2的两个元素，然后使用vMyClass5中的5个元素拷贝构造到vMyClass2中
	cout << endl << "flag7" << endl;
	vMyClass2 = vMyClass5;
	cout << "size = " << vMyClass2.size() << endl;
	
	// 首先使用1 2 3 构造三个临时变量
	// 然后使用这三个临时变量赋值(operator=)到vMyClass2中的前三个变量
	// 然后析构vMyClass2的第4，5给元素
	// 最后析构上面生成的三个临时变量
	cout << endl << "flag8" << endl;
	vMyClass2 = { 1,2,3 };
	cout << "size = " << vMyClass2.size() << endl;

	// 首先使用1，2，3，4构造4个临时变量
	// 因为vMyClass2之前有三个值，因此前三个调用operator=赋值
	// 第四个元素使用的是拷贝构造
	// 最后析构上面的四个临时变量
	// 从这两个例子可以看出，如果容器中原先具有一些元素，当需要赋值新元素时
	// 首先对原有的值进行赋值操作（operator=）
	// 如果新的容器具有更多的元素，则剩下的元素使用拷贝构造的方式构造
	// 如果新的容器中的元素更少，则析构原来容器中靠后的值
	cout << endl << "flag9" << endl;
	vMyClass2 = { 1,2,3,4 };
	cout << "size = " << vMyClass2.size() << endl;

	// 首先使用1来构造一个临时对象
	// 然后使用临时对象赋值（operator=）原来的4个元素
	// 第五个元素使用拷贝构造函数构造
	// 最后析构临时变量
	cout << endl << "flag10" << endl;
	vMyClass2.assign(5, 1);
	cout << "size = " << vMyClass2.size() << endl;

	cout << endl << "flag11" << endl;
	vMyClass1.clear();
	vMyClass1.push_back(1);
	vMyClass1.push_back(2);
	vMyClass1.push_back(3);
	vMyClass1.emplace(vMyClass1.begin(), 4);
	for (auto value : vMyClass1)
		cout << value << endl;

	cout << endl << "flag12" << endl;
	vMyClass1.clear();
	cout << vMyClass1.capacity() << endl;
	vMyClass1.push_back(1);
	vMyClass1.push_back(2);
	vMyClass1.push_back(3);
	vMyClass1.emplace(vMyClass1.begin(), 4);
	vMyClass1.emplace_back(5);
	for (vector<MyClass>::const_iterator value= vMyClass1.begin(); value!= vMyClass1.end(); ++value)
		cout << *value << endl;

	cout << endl << "flag13" << endl;
	vMyClass2.assign(vMyClass1.begin(), vMyClass1.end());
	cout << "size = " << vMyClass2.size() << endl;

	cout << endl << "flag14" << endl;
	vMyClass2.assign(vMyClass1.begin(), vMyClass1.end()-1);
	cout << "size = " << vMyClass2.size() << endl;

	cout << endl << "flag15" << endl;
	vMyClass2.assign(vMyClass1.begin(), vMyClass1.begin()+1);
	cout << "size = " << vMyClass2.size() << endl;

	cout << endl << "flag16" << endl;
	vector<int> vint = { 0,1,2,3,4,5,6,7,8,9 };
	auto it = vint.begin();
	while (it != vint.end())
	{
		if (*it % 2)
			it = vint.erase(it);
		else
			it++;
	}
	for (vector<int>::size_type i = 0; i < vint.size(); i++)
	{
		cout << vint[i] << "\t";
	}
	cout << endl;



	cout << endl << "flag17" << endl;
	vint = { 0,1,2,3,4,5,6,7,8,9 };
	it = vint.begin();
	while (it != vint.end())
	{
		if (*it % 2)
		{
			it = vint.insert(it, *it);
			it += 2;
		}
		else
			it = vint.erase(it);
	}
	for (vector<int>::size_type i = 0; i < vint.size(); i++)
	{
		cout << vint[i] << "\t";
	}
	cout << endl;


	cout << endl << "flag18" << endl;
	cout << endl << "flag19" << endl;




	return 0;
}

int DequeTest()
{
	cout << "==========DequeTest==========" << endl;


	return 0;
}

int ListTest()
{
	cout << "==========ListTest==========" << endl;
	
	return 0;
}

/*
不支持size()操作
*/
int Forward_list_Test()
{
	cout << "==========Forward_list_Test==========" << endl;

	return 0;
}

/*
array的存储空间是固定的，不能更改
*/
int ArrayTest()
{
	cout << "==========ArrayTest==========" << endl;

	return 0;
}

int main()
{
	//ClassTest();
	MyTest();
	//StringTest();
	//VectorTest();
	//DequeTest();
	//ListTest();
	//Forward_list_Test();
	//ArrayTest();


}

