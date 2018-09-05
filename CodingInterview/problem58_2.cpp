/*
剑指offer 第二版 面试题58 左旋转字符串
题目二：左旋转字符串
字符串的左旋转操作是把字符串前面的若干个字符转移到字符串的尾部。
请定义一个函数实现字符串左旋转操作的功能。比如，输入字符串
"abcdefg"和数字2， 该函数将返回左旋转两位得到的结果"cdefgab"。
要求不使用额外的空间
*/

/*
思路：
根据58_1的思路
1. 先将整个字符串翻转 得到"gfedcba"
2. 再翻转前后两部分，即分别翻转"gfedc" 和 "ba"
	得到"cdefgab"
*/

#include <iostream>
#include <string>

using namespace std;

#define DEBUG_PRINT(N) cout << "line " << __LINE__ << ": " << #N << " = " << N << endl;

void LeftRotateStr(string& str, ssize_t k)
{
	if(k>str.length())
		return;
	DEBUG_PRINT(str);
	//首先翻转整个字符串
	string::size_type pTemp = str.length()/2;
	for(string::size_type i=0; i<pTemp; i++)
	{
		char temp = str[i];
		str[i] = str[str.length()-1-i];
		str[str.length()-1-i] = temp;
	}
	DEBUG_PRINT(str);
	
	//然后分别翻转前len-k个字符和后k个字符
	pTemp = (str.length()-k)/2;
	DEBUG_PRINT(pTemp);	//打印出来方便思路整理
	DEBUG_PRINT(str[pTemp]);
	for(string::size_type i = 0; i<pTemp; i++)
	{
		char temp = str[i];
		str[i] = str[str.length()-k-1-i];
		str[str.length()-k-1-i] = temp;
	}
	DEBUG_PRINT(str);
	
	//翻转后半部分
	pTemp = (str.length()-k) + k/2;
	DEBUG_PRINT(str.length()-k);
	DEBUG_PRINT(str[str.length()-k]);
	for(string::size_type i = str.length()-k; i<pTemp; i++)
	{
		char temp = str[i];
		str[i] = str[str.length()-1-(i-str.length()+k)];
		str[str.length()-1-(i-str.length()+k)] = temp;
	}
	DEBUG_PRINT(str);
}


int main()
{
	string str;
	int k;
	cout << "please input a sentance and parameter K" << endl;
	getline(cin, str);
	cin >> k;
	cout << "before Reverse, str = ";
	cout << str << endl;
	string strtemp(str);
	LeftRotateStr(strtemp,k);
	cout << "after Reverse, str = ";
	cout << strtemp << endl;
	
	//使用C++中的方法再尝试一遍
	strtemp = str;
	/*
	笔记
	substr()用于求一个字符串的子串
	string.substr(字串第一个字符的下标，子字符串的长度);
	如果不指定尾字符的下标，那么默认设置为string.length()
	*/
	string str1 = strtemp.substr(0,k);
	string str2 = strtemp.substr(k);
	DEBUG_PRINT(str1);
	DEBUG_PRINT(str2);
	cout << str2+str1 << endl;
	
	return 0;
}

