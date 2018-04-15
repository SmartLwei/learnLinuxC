/**
* 原型模式：用原型实例指定创建对象的种类，并且通过拷贝这些原型创建新的对象
* 其他实例通过引用的方式索引到该原型，而不是将该原型拷贝到每一个地方
* 这样可以通过修改原型的方法修改所有引用它的对象
* 而不用在每一个对象上都重写该部分代码
* https://blog.csdn.net/wuzhekai1985/article/details/6667020
*/

#include <iostream>
using std::cout;
using std::endl;

//基类
class Resume
{
protected:
    char* name;
public:
    Resume() {}
    virtual ~Resume(){}
    virtual Resume* Clone() {return NULL;}
    virtual void Set(char* n){}
    virtual void Show() {}
};

class ResumeA: public Resume
{
public:
    ResumeA(const char* srt);	//构造函数
    ResumeA(const ResumeA &r);	//拷贝构造函数
    ~ResumeA();					//析构函数
    ResumeA* Clone();			//克隆函数
    void Show();
};

class ResumeB: public Resume
{
public:
    ResumeB(const char* srt);	//构造函数
    ResumeB(const ResumeB &r);	//拷贝构造函数
    ~ResumeB();					//析构函数
    ResumeB* Clone();			//克隆函数
    void Show();
};


















