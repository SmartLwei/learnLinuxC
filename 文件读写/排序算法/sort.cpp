/**
* copywrite: Lwei
* smartlwei@163.com
* 主要参考了这一篇博客 https://www.cnblogs.com/onepixel/articles/7674659.html
* 但是本文档的内容与该博客的内容差别很大，因此结合起来参考效果更好
* 博客中有很多动图，参考价值很大
* 本文档中包含了大量的说明和例子，旨在将排序过程讲得更加清晰

* 除非无法避免，本文档的所有排序算法都是按照从小到大，从前往后排序。
* 其他的方案如从后往前，从大到小排序方案过程相似，博客中有的算法就是采用的这样的过程。
	
*/


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

//检查序列是否是从小到大排列，正确返回0，错误程序阻塞以排错 //错误返回-1
static int checkArray(const vector<int>& arr)
{
	int len = arr.size();
	for(int i=0; i<len-1; i++)
	{
		if (arr[i] > arr[i+1])
		{
			cout << endl << endl 
				<< "******************" << endl 
				<< "****** Error ****** " 
				<< "******************" << endl << endl <<endl;
			cout << "序列不是按照从小到大排序" << endl;
			printa(arr);
			getchar();
			return -1;
		}
	}
	return 0;
}



/**
* 冒泡排序
* 过程： （考虑从小到大排序，且先排小数的情况；先排大数的方法类似）
* 0. 初始化时，首部拥有0个已排序数组
* 1. 在未排序数中比较相邻两个数的大小，若前数大于后数，则交换两个数，
		循环一轮后，首部已排序数字个数+1 （此时应该从数组末尾向前循环）
* 2. 如此重复，当已排序数包含整个数列时，排序结束
* 3. 如果一轮中没有交换任何两个数，则该数列已经排好序，可以提前退出循环 (可选)
* 效率 O(n^2) 稳定
*/
//我们不考虑其他开销，例如参数arr的复制，下同
static int bubbleSort(vector<int> arr)
{
	cout << endl << "****** 冒泡排序 ******" << endl; 
	cout << "比较相邻两个数，小数往数组首部冒泡" << endl;
	cout << "每一轮下来，已排序部分的长度+1" << endl;
	cout << "排序前的数组为" << endl;
	printa(arr);
	int len = arr.size();
	int i,j,temp;
	
	//i指向未排序部分的第一个值
	//i<len-1 而不是i<len 是因为当只有一个数是未排序数时，对于冒泡排序，其实它已经排好序了
	for(i=0; i<len-1; i++)
	{
		//j在未排序部分中循环
		//需要先排序数列的前面部分时，必须从后往前滑
		//该循环结束后，未排序部分的最小值已经滑到最前面
		for(j=len-2;j>=i;j--)
		{
			if(arr[j+1] < arr[j])
			{
				temp = arr[j+1];
				arr[j+1] = arr[j];
				arr[j] = temp;
			}
		}
		cout << "第" << i+1 <<"轮" << endl;
		printa(arr);
	} 
	checkArray(arr);
	cout << endl;
	return 0;
}

/**
* 选择排序
* 查找到未排序序列中的最小值的索引，与未排序的第一个交换位置
*/
static int selectionSort(vector<int> arr)
{
	cout << endl << "****** 选择排序 ******" << endl; 
	cout << "已排序的部分放在前面，每轮找到后续未排序部分中最小值的索引" << endl;
	cout << "每一轮下来，已排序部分的长度+1" << endl;
	cout << "排序前的数组为" << endl;
	printa(arr);
	int len = arr.size();
	int i,j,index,temp;
	//i指向未排序部分的第一个值
	for(i=0; i<len-1; i++)
	{
		index = i;
		//j在未排序部分中循环
		//循环结束后，找到了未排序部分中最小值的索引
		for(j=i+1; j<len; j++)
		{
			if(arr[j] < arr[index])
				index = j;
		}
		temp = arr[i];
		arr[i] = arr[index];
		arr[index] = temp;
		cout << "第" << i+1 <<"轮" << endl;
		printa(arr);
	}
	checkArray(arr);
	cout << endl;
	return 0;
}


/*
* 看完选择排序和冒泡排序两个算法后，冒出一个想法：
* 如果将选择排序和冒泡排序结合，即每一轮随机选择两个算法之一来对未排序的部分进行排序
* 那么会不会一定程度上降低最坏的结果，排序所花的时间？
* 以后花点时间验证一下
*/


/**
* 插入排序
* 数列前面部分为已排序部分
* 每一轮将未排序部分的第一个插入到已排序部分的正确位置
* 插入过程中，已排序序列的大于待插入值的部分需要整体后移
*/
static int insertionSort(vector<int> arr)
{
	cout << endl << "****** 插入排序 ******" << endl; 
	cout << "未排序部分的第一个元素插入到已排序部分的适当位置" << endl;
	cout << "每一轮下来，已排序部分的长度+1" << endl;
	cout << "排序前的数组为" << endl;
	printa(arr);
	int len = arr.size();
	int i,j,temp;
	//i指向未排序部分的第一个值
	for(i=1; i<len; i++)
	{
		//j指向已排序部分
		//循环结束后，未排序部分的第一个值已经插入到了合适的位置
		j = i-1;
		temp = arr[i];	//待插入的值
		while(j>=0 && arr[j]>temp)
		{
			arr[j+1] = arr[j];
			j--;
		}
		arr[j+1] = temp;
		cout << "第" << i <<"轮" << endl;
		printa(arr);
	}
	checkArray(arr);
	cout << endl;
	return 0;
}


/**
* 希尔排序
* 选择一个增量序列t1，t2，…，tk，其中ti>t(i+1)，tk=1；
* 按增量序列个数k，对序列进行k 趟排序；
* 每趟排序，根据对应的增量ti，将待排序列分割成若干长度为m 的子序列，分别对各子表进行直接插入排序。仅增量因子为1 时，整个序列作为一个表来处理，表长度即为整个序列的长度。

* 举个例子，假如有10个数需要排序
* 原始序列为(10 9 8 7 6 5 4 3 2 1)
* 我们先对第1个和第6个数排序，保证第1个数小于第6个数
* 同样，排序其他距离为5的数，即第2个数小于第7个数，第3个数小于第8个数，第4个小于第9个，第5个小于第10个
* 一轮下来的序列为 (5 4 3 2 1 10 9 8 7 6)  此时的序列，总的来说后面的比前面的大一些
(注意：如果原始序列为10 8 6 4 2 9 7 5 3 1的话，一轮下来的结果为9 7 5 3 1 10 8 6 4 2，因此总体情况是后面的数大于前面的数，但是后面的数不一定比前面的数都大)
* 接下来，将步长设置为2，即
* 第1个数<第3个数<第5个数<第7个数<第9个数    (1 * 3 * 5 * 7 * 9 * )
* 第2个数<第4个数<第6个数<第8个数<第10个数   (* 2 * 4 * 6 * 8 * 10)
* 排序下来的结果为(1 2 3 4 5 6 7 8 9 10)
* 最后一轮步长设置为1(虽然对于我们举得例子已经排好序，但是不是对所有的序列都可以如此)
* 排序结果为 1 2 3 4 5 6 7 8 9 10
* 
* 前面排序的过程中涉及到步长，(我们的例子中，第一次步长为5，第二次为2，第三次为1)
* 而公认的好的步长为1,4,13,40,(x*3+1)的逆序
* 首先计算出最大增量，然后依次计算 gap = (gap-1)/3计算下一次增量，即
* 数组长度为1-5     增量为1
* 数组长度为6-14    增量为4, 1
* 数组长度为15-41   增量为13, 4, 1
* 数组长度为42-122  增量为40, 13, 4, 1  
* ... 
* 数组长度为 ...-... 增量为x,(x-1)/3,((x-1)/3-1)/3,...
* 数组长度为x*3+3 - (x*3+1)*3+2 增量为(x*3)+1,x,(x-1)/3,((x-1)/3-1)/3,...
*/

int shellSort(vector<int> arr)
{
	cout << endl << "****** 希尔排序 ******" << endl; 
	cout << "对增量序列进行排序，增量为1,4,13,40,x*3+1的逆序" << endl;
	cout << "排序前的数组为" << endl;
	printa(arr);
	int len = arr.size();
	int i,j,k,temp,round=1;
	int gap = 1;
	//计算最大增量
	while(gap < len/3)
		gap = gap*3 + 1;
	while(gap > 0)
	{
		cout << "第" << round <<"轮开始" << endl;
		cout << "gap = " << gap << endl; 
		//进行插入排序
		
		//方法1 分小组进行排序
		/*
		//gap将数组分成了gap个组，k指向每一个组
		for(k=0; k<gap; k++)
		{
			//在每一个小组内进行插入排序
			//i指向未排序部分的第一个值
			for(i=k+gap;i<len;i+=gap)
			{
				//j在已排序部分循环
				j = i - gap;
				temp = arr[i];
				while(j>=0 && arr[j]>temp)
				{
					arr[j+gap] = arr[j];
					j -= gap;
				}
				arr[j+gap] = temp;
			}
			cout << "第" << k+1 <<"组排序后" << endl;
			printa(arr);
		}
		*/
		
		//方法2 分间隔进行排序
		//虽然方法2看似比方法1少了一个循环，本质上来说其实是将两个循环组合到了一个循环中
		//i指向未排序部分的第一个值，将每个小组的未排序部分统一管理
		//因为只要默认的前gap个字段已经排序，因此后面的所有元素都需要插入排序
		for(i=gap;i<len;i++)	
		{
			temp = arr[i];
			//j指向与当前i在同一个小组的字段的已排序部分
			j=i-gap;
			while(j>=0 && arr[j]>temp)
			{
				arr[j+gap] = arr[j];
				j -= gap;
			}
			arr[j+gap] = temp;
		}
		cout << "第" << round++ <<"轮结束" << endl;
		printa(arr);
		gap = (gap-1)/3;
	}
	
	checkArray(arr);
	cout << endl;
	return 0;
}


/**
* 归并排序
* 归并排序是稳定的排序算法(排序前相等的两个元素排序后相对位置不变)
* 对数组分成左右两部分
* 对该两部分再次分成左右两部分 
* ...
* 直到每一部分只有1个元素
* 对只有1个元素的两个部分合并，合并时需要排序
* 对合并过后的两部分继续合并与其他的分支合并，合并时需要排序

* 两个包含一个元素的数组合成一个包含两个元素的数组
* 两个包含两个元素的数组合成一个包含四个元素的数组
* 两个包含四个元素的数组合成一个包含八个元素的数组
* ...

* 例如：
* 排序 (10 9 8 7 6 5 4 3 2 1)
* 首先从中间分成两部分，得到
* 	片段1 				(10 9 8 7 6) 	和片段2 			(5 4 3 2 1) 
* 对片段1再分成两部分，得到
* 	片段1.1 				(10 9) 			和片段1.2 		(8 7 6)
* 对片段1.1 再分成两部分，得到
* 	片段 1.1.1 			(10) 			和片段1.1.2 		(9)
* 合并片段1.1.1 (10) 和片段1.1.2 (9)，得到		(合并时需要排序)
*	片段1.1.DONE 		(9 10)
* 对片段1.2 再分成两部分
*	片段1.2.1 			(8)				和片段1.2.2		(7 6)
* 对片段1.2.2再分成两部分，得到
*	片段1.2.2.1 			(7) 			和片段1.2.2.2 	(6)
* 合并片段1.2.2.1 (7)和片段1.2.2.2 (6)，得到 
*	片段1.2.2.DONE 		(6 7)
* 合并片段1.2.1 (8) 和片段1.2.2.DONE (6 7)，得到	
* 	片段1.2.DONE			(6 7 8)
* 合并片段1.1.DONE (9 10)和片段1.2.DONE (6 7 8)，得到
*	片段1.DONE			（6 7 8 9 10）
* 同样的方法获得片段2.DONE
* 再合并1.DONE和2.DONE就成功排好序了



*/

//合并[begein,middle)， [middle,end)两个区间
//其中middle = (begin + end) / 2
//end指向超尾，即var的区间为 [begin,end)
int merge(vector<int>& arr, int begin, int end)
{
	int middle = (begin+end)/2;
	int leftPtr,rightPtr,i;
	vector<int> temp;
	//只有left或者right有一方的值排序完毕，结束循环，此时另一方并没有将值全部插入到临时数组中
	for(leftPtr=begin,rightPtr=middle;leftPtr<middle && rightPtr<end; )
	{
		if(arr[leftPtr] < arr[rightPtr])
			temp.push_back(arr[leftPtr++]);
		else
			temp.push_back(arr[rightPtr++]);
	}
	//将left或者right的未排序部分插入到临时数组中
	while(leftPtr < middle)
		temp.push_back(arr[leftPtr++]);
	while(rightPtr < end)
		temp.push_back(arr[rightPtr++]);
	//将临时数组中的元素放入到我们的数组中
	for(i=begin;i<end;i++)
		arr[i] = temp[i-begin];
	return 0;
}

//该函数中仅允许操作arr的begin到end区间的数据
//end指向超尾，即var的区间为 [begin,end)
int mergeSort(vector<int>& arr, int begin, int end)
{
	int len=end-begin;
	if(len < 2)
	{
		return 0;
	}
	int middle = (begin+end) / 2;
	//排序左半部分
	mergeSort(arr,begin,middle);
	//排序右半部分
	mergeSort(arr,middle,end);
	//合并两部分
	merge(arr,begin,end);
	cout << "归并排序后为" << endl;
	printa(arr);
	return 0;
}




/**
* 快速排序
* 思想：先将数组分为两组，使得左边的所有数字都比右边的数字大
* 然后递归调用，最终就可以实现排序

* 例如： 排序 (5 6 4 1 3 8 6 2 9)
* 我们以第一个数5为参考数，将数列分成了 (4 1 3 2) 5 (6 6 8 9) 
* 左边的数都比右边的数小，因为5已经在中间，因此可以理解已经在正确的位置上了，
* 递归左半部分和有半部分时可以不在考虑
* 在递归的思想中，(4 1 3 2)的顺序并不重要，不同的方案产生的顺序可能不一样

* 接下来我们考虑如何将上述序列分成左右两部分
* 设置两个指针(或者说是下标索引)，
* 一个指向数组最右边并逐步向做移动
* 一个指向数组最左边并逐步向右移动
* 我们取第一个元素为参考数 referenceNumber
* 因为第一个元素已经取出保存，所以第一个位置可以保存其他数据
* 因此我们从右边向左边扫描，找到从右边起第一个比参考数字小的数，将其保存在第一个位置
* 此时，因为数字已经被保存，右边被拷贝出去的数的原始位置可以用于保存其他数字了
* 我们就考虑从左边找到比参考数字大的数，将其保存到右边，然后左边位置又可以保存其他数字了
* ...
* 最终当左边的索引值 == 右边的索引值时，左边所有的数都比参考数字小，右边所有数字都比参考数字大

* 以刚才的数组为例
* (5 6 4 1 3 8 6 2 9)
*  0 1 2 3 4 5 6 7 8 为对应的索引值
* 定义一个 leftPtr=1 索引和 rightPtr=endOfVector 索引
* 首先取出第0个数字5，保存为 referenceNumber
* 
* rightPtr左移，找比5小的数，找到了7号位置的数字2，因此将7号位置的数字拷贝到0号位置，此时数组为
* 		(2 6 4 1 3 8 6 2 9)		参考数字为5
*  		 0 1 2 3 4 5 6 7 8		对应的索引值,7号索引可以用于保存其他值
* 7号位置上的数字此时已经没有用处了，可以用于保存其他的值
* 好的策略是7号位置存放大于参考数字的值
* 因此从左边搜索大于参考数字的数，leftPtr右移，找到了1号位置的4
* 将4保存到7号位置，同时1号位置的数字已经没有用了，可以保存其他的值。此时的数组为
* 		(2 6 4 1 3 8 6 6 9)		参考数字为5
*  		 0 1 2 3 4 5 6 7 8		对应的索引值，1号索引可以用于保存其他值
* 此时从右边接着从刚才右边的位置向左边找比5小的数，放到1号索引位置。找到了4号位置的3
* 将3保存到1号位置，此时数组为
* 		(2 3 4 1 3 8 6 6 9)		参考数字为5
*  		 0 1 2 3 4 5 6 7 8		对应的索引值，4号索引可以用于保存其他值
* 再从左边找大于参考数字的数，直到左边的索引和右边的索引相同，此时它指向了4号索引
* 将 referenceNumber 保存在4号索引处，此时数组为
* 		(2 3 4 1 5 8 6 6 9)	
*  		 0 1 2 3 4 5 6 7 8		对应的索引值
* 可以看到，5左边的数都比5小，5右边的数都比5大
* 然后对5的左右两个子序列使用相同的算法，最终可以得到排列好的数
* 值得注意的是，5已经在正确的位置上了，因此不用将5放到子序列中排序


*/
//end指向超尾 即 [begin,end)
int quickSort(vector<int>& arr, int begin, int end)
{
	if(end <= begin)
		return 0;
	int len = end-begin;
	int refNum = arr[begin];
	int leftPtr=begin;
	int rightPtr = end-1;
	while(leftPtr < rightPtr)
	{
		//当循环结束时，找到了从右边起第一个小于参考数的值的索引
		while(leftPtr < rightPtr && arr[rightPtr] >= refNum)
			rightPtr--;
		//leftPtr指向的值已经保存到refNum中或者上一轮中的rightPtr中
		arr[leftPtr] = arr[rightPtr];	
		//当循环结束时，找到了从左边起第一个大于参考数的值的索引
		while(leftPtr < rightPtr && arr[leftPtr] <= refNum)
			leftPtr++;
		//rightPtr指向的值已经保存到前面的leftPtr中了
		arr[rightPtr] = arr[leftPtr];
	}
	//上面的循环结束时，leftPtr==rightPtr
	arr[leftPtr] = refNum;
	quickSort(arr,begin,leftPtr); 
	//其实这个时候leftPtr(或者说rightPtr)位置的值已经在正确的位置了
	quickSort(arr,leftPtr+1,end);
	cout << "快速排序后的数组为" << endl;
	printa(arr);
	return 0;
}



/** 
* 堆排序
*/

/**
* 计数排序
*/

/**
* 桶排序
*/

/**
* 基数排序
*/


int main()
{
	vector<int> arr = {12,81,18,96,55,29,23,44,30,88,67,73};
	bubbleSort(arr);
	selectionSort(arr);
	insertionSort(arr);
	shellSort(arr);
	vector<int> arr2(arr); //用于归并排序
	cout << endl << "****** 归并排序 ******" << endl; 
	cout << "排序前的数组为" << endl;
	printa(arr2);
	mergeSort(arr2,0,arr.size());
	vector<int> arr3(arr); //用于快速排序
	cout << endl << "****** 快速排序 ******" << endl; 
	cout << "排序前的数组为" << endl;
	printa(arr3);
	quickSort(arr3,0,arr.size());
	
	return 0;
}









