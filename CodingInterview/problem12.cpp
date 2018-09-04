/*
剑指offer 第二版 面试题12 矩阵中的路径
题目：请设计一个函数，用雷判断在一个矩阵中是否存在一条包含某字符串所有字符的路径。
路径可以从矩阵中的任意一个开始，每一古可以在矩阵中向左，上，右，下移动一个。如果
一条路径经过了矩阵的某一格，那么该路径不能再次进入该格子。例如，在下面的3*4的矩阵
中包含一条字符串"bfce"的路径。但是矩阵中不包含字符串"abfb"的路径，因为字符串的第
一个字符b占据了矩阵中的第一行第二个格子之后，路径不能再次进入这个格子。
a b t g
c f c s
j d e h
*/

/*
思路：回溯法

*/

#include <iostream>
#include <vector>
using namespace std;

#define DEBUG_PRINT_VEC(N,rows,cols) \
//do{\
	cout << #N << " = " << endl;\
	for(int i=0; i<rows; i++)\
	{\
		for(int j=0; j<cols; j++)\
			cout << N[i*cols+j] << " ";\
		cout << endl;\
	}\
}while(0);

#define DEBUG_PRINT(N) //cout << #N << " = " << N << endl;
#define DEBUG_PRINT_POINT(x,y) //cout << "visit point (" << x << ", " << y << ")" << endl; 

int count = 0;

struct Point {int x; int y; Point(int _x, int _y):x(_x),y(_y){}};


bool FindPathCore(const char* matrix, int rows, int cols, int x, int y, bool* visit, const char* path, int pathPos, vector<Point>& points)
{
	DEBUG_PRINT(++count);
	DEBUG_PRINT_POINT(x,y);
	DEBUG_PRINT(matrix[x*cols+y]);
	DEBUG_PRINT(pathPos);
	DEBUG_PRINT(path[pathPos]);
	DEBUG_PRINT_VEC(visit, rows, cols);

	int curPos = x*cols+y;
	if(path[pathPos] == '\0')
	{
		return true;
	}
	
	if(matrix == NULL || x>=rows || x<0 || y>=cols || y<0 || visit[curPos] == true || matrix[curPos] != path[pathPos])
		return false;
	
	
	
	//则访问当前位置
	visit[curPos] = true;
	//尝试找到路径中的下一个位置
	bool isFound = false;
	//从当前位置的左上右下查找路径
	isFound = FindPathCore(matrix, rows, cols, x, y-1, visit, path, pathPos+1, points)
			||FindPathCore(matrix, rows, cols, x-1, y, visit, path, pathPos+1, points)
			||FindPathCore(matrix, rows, cols, x, y+1, visit, path, pathPos+1, points)
			||FindPathCore(matrix, rows, cols, x+1, y, visit, path, pathPos+1, points);
	//如果任意一个方向找到了路径，返回成功
	if(isFound == true)
	{
		DEBUG_PRINT_POINT(x,y);
		points.emplace_back(x,y);
		return true;
	}
	//否则回退
	else
	{
		visit[curPos] = false;
		return false;
	}
}


int main()
{
	//矩阵
	const char matrix[] = {"abtgcfcsjdeh"};
	
	//访问标志
	bool visit[sizeof(matrix)];
	for(int i=0; i<sizeof(matrix); i++) visit[i] = false;
	
	//矩阵的大小
	int rows = 3;
	int cols = 4;
	
	//需要查找的路线
	char path[] = "bfce";
	
	//保存路线
	vector<Point> points;
	
	//查找路线，主函数提供递归函数的入口
	for(int x=0; x<rows; x++)
	{
		for(int y=0; y<cols; y++)
		{
			if(FindPathCore(matrix, rows, cols, x, y, visit, path, 0, points) == true)
			{
				cout << "path " << path <<" is found" << endl;
				//打印出点的路径
				for(int k=points.size()-1; k>=0; k--)
					cout << "(" << points[k].x << ", " << points[k].y << ") ";
				cout << endl;
				return 0;
			}
		}
	}
	cout << "path " << path <<" is not found" << endl;
	return 0;
}





