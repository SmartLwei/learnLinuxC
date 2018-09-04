/*
剑指offer 第二版 面试题40 最小的k个数
题目：输入n个整数，找出其中最小的k个数。例如，输入4、5、1、6、2、7、3、8
这8个数字，则最小的4个数字是1，2，3，4。
*/

/*
思路：利用快排的思想
快排的每一轮可以将数组分成两部分，后面副本的所有数字都比前面部分的任何数字大
并且，分界线处的数字的位置已经确定。
如果分界线的位置大于k，说明最小的k个数在前半部分。
如果分界线的位置小于k，说明后半部分也有一些是最小的k个数，且前半部分全部是最小的k个数
如此，复杂度为O(n)，且会改变原始数组

方法二：
使用额外的数据结构（最大堆）保存最小的k个数
将输入的数据看作一个序列，前k个数全部存入最大堆，后续对每个数字进行判断，
如果它小于最大堆的最大值（堆顶元素）那么首先弹出堆顶元素，然后将新数据
添加到最大堆中。
该方法的优点是可以从无穷个数中选出最小的k个数。
因为每次对堆的添加和删除操作的复杂度都为O(logk)，因此总的复杂度为O(nlogk);
*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define DEBUG_PRINT_VEC(N) do{cout << "line " << __LINE__ << ": " #N << " = "; for(int i=0; i<N.size();i++) cout << N[i] <<" "; cout << endl;}while(0);
#define DEBUG_PRINT(N) cout << "line " << __LINE__ << ": " << #N << " = " << N << endl;

//快排核心函数，填坑法
//start: 需要快排的首部,end：需要快排的超尾
int QuickSortCore(vector<int>& que, int start, int end)
{
	if(end-start<1)
		return start;
	//选取最后一个元素作为参考数
	int ref = que[end-1];
	int i = start;
	int j = end-1;
	
	while(i<j)
	{
		//从前往后找到第一个比ref大的数字的下标
		while(i<j && que[i]<=ref)
			i++;
		//将找到的数字填充到后面的坑里
		que[j] = que[i];
		
		//从后往前找到第一个比ref小的数字的下标
		while(i<j && que[j]>=ref)
			j--;

		//将找到的数字填充到前面的坑里
		que[i] = que[j];	
	}
	
	//上面循环结束后，i和j指向同一个数，且该数所在位置是一个"坑"
	que[i] = ref;
	//返回参考数在数组中的下标（排序）
	return i;
}

//方法一
void GetMinKNumber_Way1(vector<int> que, int length, int k)
{
	int pos = -1;
	//首先将数组二分
	//经过该函数后que[pos]的前面所有数都小于等于que[pos]，其后的数都大于que[pos]
	int front = 0;
	int end = length;
	
	while(pos != k && pos != k-1)
	{
		DEBUG_PRINT(front)
		DEBUG_PRINT(end)
		pos = QuickSortCore(que, front, end);
		if(pos > k)
			end = pos;
		else
			front = pos+1;

		DEBUG_PRINT_VEC(que);
		DEBUG_PRINT(pos)
	}
	
	for(int i=0; i<k; i++)
		cout << que[i] << " ";
	cout << endl;
}

//方法二
void GetMinKNumber_Way2(vector<int> que, int length, int k)
{

	if(k<=0 || k>length)
		return;
	DEBUG_PRINT_VEC(que);
	//将que当作一个数据流处理
	//最大堆
	vector<int> maxHeap;
	//对前k个数字,直接插入到最大堆中
	for(int i=0; i<k; i++)
	{
		//先将数据插入到末尾
		maxHeap.push_back(que[i]);
		//将末尾的数据调整到合适的位置
		//push_heap()的第三个参数如果是greater<int>(),则是最小堆
		push_heap(maxHeap.begin(), maxHeap.end(),less<int>());
	}
	
	DEBUG_PRINT_VEC(maxHeap);
	
	//对之后的数据，如果小于堆顶的元素，则删除堆顶元素后，插入新的元素
	//否则不做处理
	for(int i=k; i<length; i++)
	{
		//判断与堆顶元素的大小
		if(que[i] >= maxHeap[0])
			continue;
		else
		{
			//首先弹出堆顶元素
			pop_heap(maxHeap.begin(),maxHeap.end(),less<int>());
			maxHeap.pop_back();
			//然后插入新元素
			maxHeap.push_back(que[i]);
			push_heap(maxHeap.begin(), maxHeap.end(),less<int>());
		}
		DEBUG_PRINT_VEC(maxHeap);
	}
	
	for(int i=0; i<k; i++)
		cout << maxHeap[i] << " ";
	cout << endl;
	
	//以上是用最大堆实现的，也可以使用红黑树(multiset)来实现
}

#include <ctime>

//用于打乱输入序列
int myRandom(int i) {return rand()%i;}

int main()
{
	vector<int> que = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 2, 4, 6, 8, 10, 12};
	//首先打印que
	sort(que.begin(),que.end());
	DEBUG_PRINT_VEC(que);
	//然后打乱que
	srand(unsigned(time(0)));
	random_shuffle(que.begin(),que.end(),myRandom);
	DEBUG_PRINT_VEC(que);
	
	cout << "please input parameters k" << endl;
	int k = 0;
	cin >> k;
	
	//使用方法1找到最小的k个数
	cout << "way1:" << endl;
	GetMinKNumber_Way1(que, que.size(),k);
	
	cout << endl << "way2" << endl;
	GetMinKNumber_Way2(que, que.size(),k);
	return 0;
}


