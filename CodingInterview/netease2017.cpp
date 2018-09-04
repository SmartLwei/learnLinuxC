/*
网易2017年内推笔试题目
题目：有 n 个学生站成一排，每个学生有一个能力值，牛牛想从这 n个学生中按照顺序
选取 k名学生，要求相邻两个学生的位置编号的差不超过 d，使得这 k 个学生的能力值
的乘积最大，你能返回最大的乘积吗？

输入描述：
每个输入包含1个测试用例。每个测试数据的第一行包含一个整数 n (1<=n<=50)，表示学
生的个数，接下来的一行，包含n个整数，按顺序表示每个学生的能力值 ai（-50<=ai<= 50）。
接下来的一行包含两个整数k 和 d (1 <= k <= 10, 1 <= d <= 50)。

输出描述：
输出一行表示最大的乘积。

输入例子：
3
7 4 7
2 50
输出例子：
49
*/

/*
分析：动态规划
从N个人中选出k个人，求他们能力值乘积的最大值。等效于
从N个人中选出k个人，其中最后一个人是这N个人中的第k个人，第k+1个人，第k+2个人...最后一个人
从这N-k个数中选择最大那个，就是所需要的结果。
例如
从a b c d e这5个人中选出两个人，使他们能力值的乘积最大，等效于
从这5个人中选则两个人，其中最后一个人是b，得到一个乘积值
最后一个人是c，得到一个乘积值，最后一个人是d，得到一个乘积值，最后一个人是e，得到一个乘积值
（其中最后一个人不可能是a，因为此时只能选择a这一个人）
从上面4个值中选择最大的值就是我们需要的值。
为什么要按照这样来分解这个问题，而不按照其他方案呢？。。。因为其他方案不好动态规划 【囧】
按照上面方案分拆问题后，接下来的问题就是使用动态规划求出这几个子问题的最优解了。
f(k, n) = max{ f(k-1, n-1), f(k-1,n-2),...,f(k-1, n-k)} * ai[n];
结合题目，上面式子的物理意义如下
等号左边：在N个人中选择k个人，其中这k个人中最后一个人是第n个人
等号右边：因为题目要求相邻两个人之间的间隔最大为k，因此从之前选出的k-1个人中，找到距离小于d
	的最大的乘积值，乘以第n个人的能力值，得到以第n个人结尾的k个人的能力值乘积的最大值。
由于该题目中个人能力值还有可能为负值，因此还需要保存最小的负数值
*/

/*
动态规划的特点
1. 求一个问题的最优解
2. 可以将大问题分解为若干个小问题，然后继续向下细分
3. 整体问题的最优解依赖于子问题的最优解
4. 从上向下分析问题，但是从下向上解决问题
*/
#include <iostream>
#include <vector>
using namespace std;

#define DEBUG_PRINT(N) cout << #N << " = " << N << endl;
#define DEBUG_PRINT_VEC(VEC, rows, cols)\
do\
{\
	cout << #VEC << " = " << endl;\
	for(int i=1; i<=rows; i++)\
	{\
		for(int j=1; j<=cols; j++)\
			cout << VEC[i][j] << "\t";\
		cout << endl;\
	}\
}while(0);

//找到[row][col]上一行，距离col小于d的最大值
inline long long getMax(long long maxRes[11][51], int row, int col, int distance)
{
	long long max = 0;
	int k = row-1;
	for(int n=col-1, d=0; n>=1, d<distance; n--, d++)
	{
		if(max < maxRes[k][n])
			max = maxRes[k][n];
	}
	return max;
	DEBUG_PRINT(row);
	DEBUG_PRINT(col);
	DEBUG_PRINT(max);
} 

//找到[row][col]上一行，距离col小于d的最小值
inline long long getMin(long long minRes[11][51], int row, int col, int distance)
{
	long long min = 0;
	int k = row-1;
	for(int n=col-1, d=0; n>=1, d<distance; n--, d++)
	{
		if(min > minRes[k][n])
			min = minRes[k][n];
	}
	return min;
	DEBUG_PRINT(row);
	DEBUG_PRINT(col);
	DEBUG_PRINT(min);
} 


int main()
{
	//获取输入
	int N, K, D;
	cin >> N;
	int ai[51] = { 0 };
	for (int i = 1; i <= N; i++) cin >> ai[i];
	cin >> K >> D;

	//保存最大值和最小值
	long long maxRes[11][51] = { 0 };
	long long minRes[11][51] = { 0 };
	
	//首先初始化第一行（下标从1开始计数）
	for(int n=1; n<=N; n++)
	{
		if(ai[n] > 0)
			maxRes[1][n] = ai[n];
		else
			minRes[1][n] = ai[n];
	}
	
	DEBUG_PRINT_VEC(maxRes, K, N);
	DEBUG_PRINT_VEC(minRes, K, N);
	
	//依次求出maxRes和minRes数组
	for(int k=2; k<=K; k++)
	{
		for(int n=k; n<=N; n++)
		{
			if(ai[n] > 0)
			{
				maxRes[k][n] = getMax(maxRes, k, n, D)*ai[n];
				minRes[k][n] = getMin(minRes, k, n, D)*ai[n];
			}
			else if(ai[n] < 0)
			{
				maxRes[k][n] = getMin(minRes, k, n, D)*ai[n];
				minRes[k][n] = getMax(maxRes, k, n, D)*ai[n];
			}
			else
				NULL;
		}
		DEBUG_PRINT_VEC(maxRes, K, N);
		DEBUG_PRINT_VEC(minRes, K, N);
	}
	
	//找到第k行的最大值
	long long max = 0x8000000000000000;
	for(int n=1; n<=N; n++)
		max = max>maxRes[K][n]?max:maxRes[K][n];
	
	cout << max << endl;
	
	/*
	测试用例：2 -1 2 2 1 失败，有bug
	*/
	return 0;
}
