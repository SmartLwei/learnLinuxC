/**
* 单例模式
* 程序中仅仅存在一个类的实例
* 当需要管理文件句柄或者日志句柄时，就可以用到该模式
* 通过将构造函数设为私有，然后使用GetInstance函数获取实例
*/

#include <iostream>
using std::cout;
using std::endl;

class Singleton
{
private:
    static Singleton* singleton;
protected:
    //将构造函数设置为保护属性，子类能够访问它
    Singleton() {}
public:
    static Singleton* GetInstance(const char* name);
    virtual void show() {cout << "Singleton" << endl;}
};

class SingletonA: public Singleton
{
	//父类是子类的友元类，可以访问子类的（构造）函数
    friend class Singleton;
private:
    SingletonA() {}
public:
    void show() {cout << "SingletonA" << endl;}
};

class SingletonB: public Singleton
{
    //父类是子类的友元类，可以访问子类的（构造）函数
    friend class Singleton;
private:
    SingletonB() {}
public:
    void show() {cout << "SingletonB" << endl;}
};





















