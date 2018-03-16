/*
* http://blog.csdn.net/ziggyplayguitar/article/details/79426367
* 对这道题我自己的见解
* 线性规划问题
*/
/*
 有个穷困的艺术家。他画了一幅超现实主义的作品《方块手拉手》。现在他已经把图画中手拉手
 的一排大小不一的方块都画出来了。现在要考虑上颜色了。可惜他手中的钱并不多了。但是他是
 个有追求的人，他希望这幅画中每两个相邻的方块的颜色是不一样的。你能帮他计算一下把这幅
 画上色后，最少需要花多少钱么。
输入 N个方块，K个颜色
接下来每列是各个方块染成不同颜色的价钱

输出 最少花的钱
 
 例：
 4 3
 2 3 2
 9 1 4
 7 8 1
 2 8 3
 6
 */
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits.h>
using namespace std;

/**
* 思路：
* 先假设只有两个格子，对第二个格子的颜色进行遍历，查找到第二个格子着每个颜色时
* 两个盒子着色的总的价格的最低值。如此一来，两行的矩阵就简化为一行
* 例如：
* 5 2 3 8 4 6 1
* 1 5 6 3 2 1 4
* 可以简化为
* 2 6 7 4 3 2 6
* 即第2个盒子着第1个颜色时，第1个盒子着第7个颜色最便宜，因此第2个盒子着
* 第1个颜色的最低成本为1+1=2
* 第2个盒子着第2个颜色时，第1个盒子着第7个颜色最便宜，因此第2个盒子着
* 第2个颜色的最低成本为5+1=6
* 。。。
* 第2个盒子着第7个颜色时，第1个盒子着第2个颜色最便宜，因此第2个盒子着
* 第7个颜色的最低成本为4+2=6
* ---------------------------------------------------------------------------
* 然后考虑第三个盒子
* 根据上面的讨论，第2个盒子的着色最低成本已经得到，此时只需要再一次使用刚才的
* 思路将新的到的第2个盒子的价格和第3个盒子的价格整合为一行，找到第3个盒子着
* 每一个颜色时的最低成本
* 例如：原始矩阵为
* 5 2 3 8 4 6 1
* 1 5 6 3 2 1 4
* 2 6 4 1 5 9 3
* 根据上面一步后，矩阵变为
* 2 6 7 4 3 2 6
* 2 6 4 1 5 9 3
* 将给二阶矩阵降阶为一阶，变为
* 4 8 6 3 7 11 5
* 因此这三个盒子的最低着色价格为3
* ----------------------------------------------------------------------------
* 以此类推，最终找到最后一个盒子着每一个颜色对应的最低成本，其中的最小值就是我们
* 需要的值
*/
int minCost(vector<vector<int> > costs)
{
	int N = costs.size();				//盒子数
	int K = costs[0].size();			//每个盒子的颜色数
	vector<int> minCostForThisBox(K);	//矩阵压缩保存，将两行降阶为一行时，那一行的值
	int cost_thisRound = 0;
	int mincost_thisBoxthisColor = 0;
	int mincost_forAll = 0;
	for (int i = 1; i < N; i++)			//从第二行开始遍历，将上面一行的价格加到当前行
	{
		for (int j = 0; j < K; j++)		//对当前盒子（第i个盒子）的颜色进行遍历
		{
			for (int t = 0; t < K; t++)	//对当前盒子的上一个盒子的颜色进行遍历
			{
				if (j == t)
					continue;			//规定的是相邻盒子颜色不能相同
				cost_thisRound = costs[i][j] + costs[i - 1][t];
				if (((j == 0) && (t == 1)) || (t == 0))	//当着色为最前边的颜色时，初始化最低价格
					mincost_thisBoxthisColor = cost_thisRound;
				if(cost_thisRound < mincost_thisBoxthisColor)
					mincost_thisBoxthisColor = cost_thisRound;
					//还可以紧跟着一个标记记录下t的值
			}							//遍历完成后，理论上应该找到了第i个盒子着第j种颜色的最低成本
			costs[i][j] = mincost_thisBoxthisColor;
		}								//遍历完成后，costs的最后一行应该就是着每一个颜色的最低成本了
	}
	mincost_forAll = costs[N - 1][0];
	for (int i = 0; i < K; i++)
	{
		if (costs[N - 1][i] < mincost_forAll)
			mincost_forAll = costs[N - 1][i];
	}
	//将矩阵打印出来看看效果
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < K; j++)
		{
			cout << costs[i][j] << "\t";
		}
		cout << endl;
	}
	cout << "最低成本为\n";
	return mincost_forAll;
}


int main()
{
	int N,K;
	int cost;
	vector<vector<int> > costs;
	string s;
	istringstream is;

	getline(cin, s);
	is.str(s);
	is>>N>>K;
	for(int i =0 ; i < N; i++)
	{
		vector<int> t;
		getline(cin, s);
		is.clear();
		is.str(s);
		for(int j = 0; j < K; j++) 
		{
			is >> cost;
			t.push_back(cost);
		}
		costs.push_back(t);
	}
	//输出原始矩阵
	cout << "原始矩阵为" << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < K; j++)
		{
			cout << costs[i][j] << "\t";
		}
		cout << endl;
	}

	cout << "处理后的矩阵为" << endl;
	cout<<minCost(costs)<<endl;
	cout << "end" << endl;
	cin.get();
	return 0;
}
