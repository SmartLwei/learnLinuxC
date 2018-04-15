/**
* 简单工厂模式
* 一个工厂根据需要的类型（函数输入）生产相应的产品
* 本质上来说，该设计模式使用的是类的虚函数方法
*/


#ifndef FACTORT_H_1_
#define FACTORT_H_1_

#include <iostream>
using std::cout;
using std::endl;

typedef enum
{
    A,
    B
}productType;

//抽象基类：产品
class product
{
public:
    virtual void show()=0;
};

//产品A
class productA: public product
{
public:    
    void show() { cout << "productA" << endl;}
};

//产品B
class productB: public product
{
public:
    void show() { cout << "productB" << endl;}
};

//工厂
class factory
{
public:
    //一个工厂根据类型创建多类产品
    product* createProduct(productType ctype)
    {
        if(ctype == A)
            return new productA();
        else if(ctype == B)
            return new productB();
        else
        {
            cout << __FILE__ << __LINE__ << "Unknown type" << endl;
            return NULL;
        }
        //每添加一种产品，该函数都需要做相应的修改
        //因此违反了开放封闭原则：
        //软件实体（类，模块，函数）可以扩展，但是不可修改
    }
};





#endif


