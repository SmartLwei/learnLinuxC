#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

int main()
{
	vector<int> ivec;
	for(vector<int>::size_type i=0; i!= 10; ++i)
	{
		ivec.push_back(i);
		//赋值两个重复的值
		ivec.push_back(i);
	}

	set<int> iset(ivec.cbegin(),ivec.cend());
	multiset<int> miset(ivec.cbegin(),ivec.cend());

	cout << ivec.size() << endl; //打印20
	cout << iset.size() << endl; //打印10
	cout << miset.size() << endl; //打印20
	// set的关键字是唯一的，而multiset的关键字不唯一

}

