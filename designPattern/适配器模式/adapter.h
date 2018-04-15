/**
* 适配器模式
* 将一个类的接口转换成客户希望的另一个接口，使得兼容更多的类
* 对象适配器
* https://blog.csdn.net/wuzhekai1985/article/details/6665542
*/

#include <iostream>

using std::cout;
using std::endl;

//双端队列
class Deque
{
public:
    void push_back(int x) {cout << "Deque push_back" << endl;}
    void push_front(int x) {cout << "Deque push_front" << endl;}
    void pop_back(){cout << "Deque pop_back" << endl;}
    void pop_front() {cout << "Deque pop_front" << endl;}
};

//顺序容器
class Sequence
{
public:
    virtual void push(int x) = 0;
    virtual void pop() = 0;
};

//栈
class Stack: public Sequence
{
private:
    Deque m_deque;
public:
    void push(int x) {m_deque.push_back(x);}
    void pop() {m_deque.pop_back();}
};

//队列
class Queue: public Sequence
{
private:
    Deque m_deque;
public:
    void push(int x) {m_deque.push_back(x);}
    void pop() {m_deque.pop_front();}
};














