/**
* 模板方法模式，根据框架完成实现
* 个人认为，本质上讲，类继承也算是一种模板方法模式
* https://blog.csdn.net/wuzhekai1985/article/details/6667020
*/

#include <iostream>
using std::cout;
using std::endl;


class Resume
{
protected:
    virtual void SetPersonalInfo(){}
    virtual void SetEducation(){}
    virtual void SetWorkExp(){}
public:
    void FillResume()
    {
        SetPersonalInfo();
        SetEducation();
        SetWorkExp();
    }
};

class ResumeA :public Resume
{
    protected:
    void SetPersonalInfo() {cout << "A's PersonalInfo" << endl;}
    void SetEducation() {cout << "A's Education" << endl;}
    void SetWorkExp() {cout << "A's Work Experience" << endl;}
};

class ResumeB :public Resume
{
    protected:
    void SetPersonalInfo() {cout << "B's PersonalInfo" << endl;}
    void SetEducation() {cout << "B's Education" << endl;}
    void SetWorkExp() {cout << "B's Work Experience" << endl;}
};

