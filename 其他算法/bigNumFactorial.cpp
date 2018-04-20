/**
* 大数阶乘运算
* 主要参考了这边博客https://blog.csdn.net/wuzhekai1985/article/details/6845868
* 加上了自己的理解和注释
* 注释的目的旨在几年后的的自己花上很短的时间就能回顾该算法

* 问题描述：求一个整数 n 的阶乘，0 <= n <=5000。
     比如n = 50，结果为30414093201713378043612608166064768844377641568960512000000000000
*/


/*
 考虑小学的时候学的乘法运算的手算方法
                   9 9 9 9
                 * 9 9 9 9
--------------------------------
                 8 9 9 9 1
               8 9 9 9 1
             8 9 9 9 1
           8 9 9 9 1
---------------------------------
           1 2 3 3 2 1       进位
---------------------------------
           9 9 9 8 0 0 0 1
我们只需要依次计算出 1 0 0 0 8 9 9 9 并保存到数组中即可


*/


int way1(int n)
{
	int a[20000] = {1,0};		//保存结果
	int m = 0;					//m保存a中有效的位数
	int c = 0; 					//进位值
	for(; n>0; n--)				//依次乘n,n-1,...,1
	{
		c = 0;
		for(int i=0; i<=m; i++)	//中间结果乘n
		{
			c += a[i]*n;
			a[i] = c%10;
			c /= 10;
		}
		for(; c; c/=10)
			a[++m] = c%10;
	}
	
}
























