/**
* 工厂方法模式
* 对于每一类产品，都有相应的工厂对其进行生产（简单工厂模式只有一个工厂）
* 定义一个用于创建对象的接口，让子类决定实例化哪一个类
* 本质上来说，该设计模式使用的是类的虚函数方法
*/


#ifndef FACTORT_H_2_
#define FACTORT_H_2_

#include <iostream>
using std::cout;
using std::endl;

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

//抽象基类：工厂
class factory
{
public:
    virtual product* createProduct() = 0;
};

//工厂A生产A产品
class factoryA: public factory
{
public:
    product* createProduct() {return new productA;}
};

//工厂B生产B产品
class factoryB: public factory
{
public:
    product* createProduct() {return new productB;}
};

//缺点：
//每增加一种产品，就需要增加一个对象的工厂

#endif



















