/**
* 策略模式
* 该模式使得算法可独立于使用它的客户而变化
* 对外接口一样，但是内部实现细节可能不同
* 本例程中，排序算法对外只有一个接口，但是可以选择三种排序方案
* 注: 例程中只实现了策略，并没有实现算法
*/



#include <iostream>

using std::cout;
using std::endl;

//抽象基类，排序算法
class sortAlgorithm
{
public:
    virtual void sort_() = 0;
};

//快速排序
class fastSortAlgorithm: public sortAlgorithm
{
public:
    void sort_() {cout << "fastSortAlgorithm" << endl;}
};

//冒泡排序
class bubbleSortAlgorithm: public sortAlgorithm
{
public:
    void sort_() {cout << "bubbleSortAlgorithm" << endl;}
};

//插入排序
class insertSortAlgorithm: public sortAlgorithm
{
public:
    void sort_() {cout << "insertSortAlgorithm" << endl;}
};

#if defined WAY1
//方法1,新建类时给出排序算法
//待排序数据
class randomData
{
private:
    sortAlgorithm* m_sort;
public:
    randomData(sortAlgorithm* so) {m_sort = so;}
    ~randomData(){delete m_sort;}
    void sort_() {m_sort->sort_(); }
};
#elif defined WAY2
//方法2,新建类时给出算法类型
//待排序数据
typedef enum
{
fast,
bubble,
insert
}sortStrategy;

class randomData
{
private:
    sortAlgorithm* m_sort;
public:
    randomData(sortStrategy ways)
    {
        if(ways == fast)
            m_sort = new fastSortAlgorithm;
        if(ways == bubble)
            m_sort = new bubbleSortAlgorithm;
        if(ways == insert)
            m_sort = new insertSortAlgorithm;    
    }
    ~randomData(){delete m_sort;}
    void sort_() {m_sort->sort_(); }
};
#else
//方法3,使用模板选择排序算法
//待排序数据
template <class SA>
class randomData
{
private:
    SA m_sort;
public:
    randomData(){}
    ~randomData(){}
    void sort_() {m_sort.sort_(); }
};
#endif
















