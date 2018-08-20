/**
* vector 中的元素是保存在栈中还是堆中的？
* 从结果来看，vector中的数据是保存在堆中的
*/

#include <vector>
#include <iostream>
using namespace std;

int main()
{
	vector<int> testVector;
	int a=5;
	int *b = new int(6);
	testVector.push_back(a);
	a = 6;
	cout << "a= " << a << endl;
	cout << "*b = " << *b << endl;
	cout << "vector[0] = " << testVector[0] << endl;
	cout << "&a = " << &a << endl;
	cout << "b = " << b << endl;
	cout << "&(vector[0] ) = " << &(testVector[0]) << endl;
	return 0;
}
