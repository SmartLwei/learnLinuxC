/**
* 抽象工厂模式
* 为了解决经常增加产品种类而造成经常添加工厂或者修改工厂方法而设计的一种模式
* 经产品分为多个系列，每一个系列中有多种产品
* 
* 本质上来说，该设计模式使用的是类的虚函数方法
*/


#ifndef FACTORT_H_3_
#define FACTORT_H_3_

#include <iostream>
using std::cout;
using std::endl;

//抽象基类：产品系列1
class productSeries1
{
public:
    virtual void show()=0;
};

//产品系列1中的A产品
class productA_Series1: public productSeries1
{
public:    
    void show() { cout << "productA_Series1" << endl;}
};

//产品系列1中的B产品
class productB_Series1: public productSeries1
{
public:
    void show() { cout << "productB_Series1" << endl;}
};

//抽象基类：产品系列2
class productSeries2
{
public:
    virtual void show()=0;
};

//产品系列2中的A产品
class productA_Series2: public productSeries2
{
public:    
    void show() { cout << "productA_Series2" << endl;}
};

//产品系列2中的B产品
class productB_Series2: public productSeries2
{
public:
    void show() { cout << "productB_Series2" << endl;}
};

//抽象基类：工厂
class factory
{
public:
    //生产系列1产品
    virtual productSeries1* createSeries1() = 0;
    //生产系列2产品
    virtual productSeries2* createSeries2() = 0;
};

//工厂A生产A产品
class factoryA: public factory
{
public:
    //生产系列1的A产品
    productSeries1* createSeries1() {return new productA_Series1;}
    //生产系列2的A产品
    productSeries2* createSeries2() {return new productA_Series2;}
};

//工厂B生产B产品
class factoryB: public factory
{
public:
    //生产系列1的A产品
    productSeries1* createSeries1() {return new productB_Series1;}
    //生产系列2的A产品
    productSeries2* createSeries2() {return new productB_Series2;}
};


#endif



















