/*
剑指offer 第二版 面试题58 翻转字符串
题目一：翻转单词顺序
输入一个英文句子，翻转句子中单词的顺序，但单词内字符的顺序不变。
为简单期间，标点符号和普通字母一样处理。例如输入字符串为
"I am a student."，则输出"student. a am I"
要求不是用额外的空间
*/

/*
思路：
1. 首先翻转整个字符串
得到".tneduts a ma I"
2. 对每个单词再翻转一次
得到"student. a am I"
*/

#include <iostream>
#include <string>

using namespace std;

#define DEBUG_PRINT(N) //cout << "line " << __LINE__ << ": " << #N << " = " << N << endl;

void ReverseStr(string& str)
{
	DEBUG_PRINT(str);
	string::size_type pTemp = str.length()/2;
	//首先翻转整个字符串
	for(int i=0; i<pTemp; i++)
	{
		char temp = str[i];
		str[i] = str[str.length()-1-i];
		str[str.length()-1-i] = temp;
	}
	DEBUG_PRINT(str);
	
	//接下来翻转每个单词
	string::size_type pBeg = 0;
	string::size_type pEnd = 0;
	while(pEnd < str.length())
	{
		//找到当前单词的“尾后”位置
		while(pEnd < str.length() && str[pEnd] != ' ')
			pEnd++;
		//经过上述操作后，pEnd指向了空格或者尾后
		
		//翻转当前单词
		pTemp = pBeg + (pEnd-pBeg)/2;
		for(string::size_type i=pBeg; i<pTemp; i++)
		{
			char temp = str[i];
			str[i] = str[pEnd-1-(i-pBeg)];
			str[pEnd-1-(i-pBeg)] = temp;
		}
		DEBUG_PRINT(str);
		
		//结束后，改变两个指针的值，以操作下一个单词
		pEnd++;
		pBeg = pEnd;
	}
	DEBUG_PRINT(str);	
}

int main()
{
	string str;
	cout << "please input a sentance" << endl;
	while(getline(cin, str))
	{
		cout << "before Reverse, str = ";
		cout << str << endl;
		ReverseStr(str);
		cout << "after Reverse, str = ";
		cout << str << endl;
		cout << "please input a sentance:(Ctrl+D to quit)" << endl;
	}	
	return 0;
}

