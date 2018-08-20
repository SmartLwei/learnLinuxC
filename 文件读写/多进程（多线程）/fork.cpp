/**
fork()函数
功能：通过系统调用，创建一个与自身几乎完全相同的进程（不是线程），
    创建时复制调用进程的数据和代码。相当于从fork()处克隆了一个自己。
    如果是调用进程（父进程）本身执行到该函数，则fork()函数返回子进程ID；
    如果是新产生的进程（子进程），则返回0；
    返回值为负数则出错
*/

/**
* 创建进程函数fork()
* 失败返回-1，父进程返回子进程的id，子进程返回0
* 该函数测试父子进程之间的数据共享
* 从结果看：
* 全局变量，堆变量，栈变量，父子进程无关
*/


#include <iostream>
#include <stdlib.h>		//malloc()函数
#include <unistd.h>		//fork()函数
using namespace std;

#define DEBUG_PRINT(N) cout << __LINE__ << ": " << N << endl;
#define DEBUG_PRINT_ADDR(N) cout << #N << ": " << N << " at " << &N << endl;

int globalVar = 1;

int main()
{
    //保存进程id
    pid_t pid;
    int stackVar = 1;
    const char* constVar = "AString";
    int *heapVar = (int*)malloc(sizeof(int));
    *heapVar = 1;

    pid = fork();
    if(pid < 0)
    {
        DEBUG_PRINT("fork()失败");
        return 0;
    }
     else if(pid == 0)
     {
         DEBUG_PRINT("子进程：")
         globalVar++;
         stackVar++;
         (*heapVar)++;
         DEBUG_PRINT_ADDR(globalVar);
         DEBUG_PRINT_ADDR(stackVar);
         DEBUG_PRINT_ADDR(*heapVar);
         DEBUG_PRINT_ADDR(constVar);
     }
     else
     {
         usleep(1000);
         DEBUG_PRINT("父进程：")
         DEBUG_PRINT_ADDR(globalVar);
         DEBUG_PRINT_ADDR(stackVar);
         DEBUG_PRINT_ADDR(*heapVar);
         DEBUG_PRINT_ADDR(constVar);
         if(globalVar != 1)
            cout << "全局变量父子进程相关"  << endl;
         else
            cout << "全局变量父子进程无关" << endl;

         if(stackVar != 1)
            cout << "栈变量父子进程相关"  << endl;
         else
            cout << "栈变量父子进程无关" << endl;

        if(*heapVar != 1)
            cout << "堆变量父子进程相关"  << endl;
         else
            cout << "堆变量父子进程无关" << endl;
     }
     return 0;
}

