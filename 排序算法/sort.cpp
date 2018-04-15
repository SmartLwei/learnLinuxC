
#include <iostream>
#include <vector>
using namespace std;

//打印数组，用于查看中间过程
static void printa(const vector<int>& arr)
{
	int len = arr.size();
	for(int i=0; i<len; i++)
	{
		cout << arr[i] << "  ";
	}
	cout << endl;
}

/**
* 冒泡排序
* 过程：
* 1. 比较相邻两个数的大小，若前数大于后数，则交换两个数，循环到未排序的数组的最后
* 		一个后，数组的最后一个数为最大的数，且已排序
* 2. 然后将前面的数作为未排序的数，重复过程1，一轮可以产生一个已排序数
* 3. 如此重复，当已排序数包含整个数列时，排序结束
* 4. 如果一轮中没有交换任何两个数，则该数列已经排好序，可以提前退出循环
*/
//我们不考虑其他开销，例如参数arr的复制
static int bubbleSort(vector<int> arr)
{
	int len = arr.size();
	int i,j,temp;
	for(i=0; i<len; i++)
	{
		for(j=0;j<len-1-i;j++)
		{
			if(arr[j] > arr[j+1])
			{
				temp = arr[j+1];
				arr[j+1] = arr[j];
				arr[j] = temp;
			}
		}
		cout << "第" << i+1 <<"轮" << endl;
		printa(arr);
	} 
	return 0;
}

static int slectionSort()
{
	return 0;
}



/*
* 看完选择排序和冒泡排序两个算法后，冒出一个想法：
* 如果将选择排序和冒泡排序结合，即每一轮随机选择两个算法之一来对未排序的部分进行排序
* 那么会不会一定程度上降低最坏的结果，排序所花的时间？
* 以后花点时间验证一下
*/


int main()
{
	vector<int> arr = {12,81,18,96,55,29,23,44,30,88,67,73};
	bubbleSort(arr);
}









