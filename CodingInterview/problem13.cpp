/*
剑指offer 第二版 面试题13 机器人的运动范围
题目：地上有一个m行n列的方格。一个机器人从坐标(0,0)的格子开始移动，
它每次可以左、右、上、下移动一格，但不能进入行坐标和列坐标的位数之
和大于k的格子。例如，当k为18时，机器人能够进入格子(35,37),因为
3+5+3+7=18。但它不能进入方格(35,38)，因为3+5+3+8=19。请问该机器人
能够到达多少个格子？
*/

/*
思路
回溯法，类似于深度优先遍历。
每次访问左边的格子，上边的格子，右边的格子，下边的格子。访问过后做一个
标记，当不能再访问时，访问结束。
*/

#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

#define DEBUG_PRINT_VEC(N,rows,cols) \
do{\
	cout << #N << " = " << endl;\
	for(int i=0; i<rows; i++)\
	{\
		for(int j=0; j<cols; j++)\
			cout << setw(4) << N[i*cols+j];\
		cout << endl;\
	}\
}while(0);

class Point{public:int x; int y;Point(int _x, int _y):x(_x),y(_y){}};

//计算一个数字各个位的和
int GetDigitSum(int number)
{
	int res = 0;
	while(number)
	{
		res+=number%10;
		number/=10;
	}
	return res;
}

int count = 0;

int VisitBoxCore(int* visit, int rows, int cols, int x, int y, int k)
{
	//判断输入参数
	if(visit == NULL || x<0 || x>=rows || y<0 || y>=cols)
		return 0;
	//如果当前位置先前已经访问过
	if(visit[x*cols+y] != 0)
		return 0;
	//如果当前位置不符合条件
	if(GetDigitSum(x) + GetDigitSum(y) > k)
		return 0;
	
	//访问该位置
	visit[x*cols+y] = ++count;
	//访问相邻的位置
	return 1+VisitBoxCore(visit, rows, cols, x, y-1, k)
			+VisitBoxCore(visit, rows, cols, x-1, y, k)
			+VisitBoxCore(visit, rows, cols, x, y+1, k)
			+VisitBoxCore(visit, rows, cols, x+1, y, k);
}

int main()
{
	int visit[1000] = {0};
	int count = VisitBoxCore(visit, 20, 20, 0, 0, 15);
	cout << "count = " << count << endl;
	DEBUG_PRINT_VEC(visit, 20, 20);
	return 0;
}



