//请根据宏开关选择创建的方式
//#define WAY1
//#define WAY2
#include "strategy.h"

int main(int argc, char* argv[])
{
#if defined WAY1
	//以新建排序算法为参数创建数据
    randomData myRandomData(new insertSortAlgorithm);
#elif defined WAY2
    //告知使用哪一个算法为参数创建数据
    randomData myRandomData(bubble);
#else
    randomData<fastSortAlgorithm> myRandomData;
#endif

    //查看使用的排序算法
    myRandomData.sort_();
    return 0;
}
