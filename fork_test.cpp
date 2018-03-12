/**
fork()函数
功能：通过系统调用，创建一个与自身几乎完全相同的进程（不是线程），
    创建时复制调用进程的数据和代码。相当于从fork()处克隆了一个自己。
    如果是调用进程（父进程）本身执行到该函数，则fork()函数返回子进程ID；
    如果是新产生的进程（子进程），则返回0；
    返回值为负数则出错
*/

#include <stdio.h>
#include <unistd.h>
int main()
{
    //pid_t在Linux 32位中本质上是int类型
    pid_t pid;
    //子进程并不会执行父进程调用fork()之前的函数调用
    printf("执行fork()之前的函数调用\n");
    int count = 0;
    pid = fork();
    //子进程的返回值为0
    if(pid == 0)
    {
        printf("this is child process, pid is %d \n",getpid());
        count += sizeof(pid);
        //usleep(100);
        printf("child fork()return value is %d, count = %d\n",pid, count);
    }
    //父进程的返回值为子进程的PID
    else if(pid >0)
    {
        printf("this is father process, pid is %d \n", getpid());
        count++;
        //如果加上一定的延迟等待，则父进程和子进程抢夺资源的行为就可以显现出来
        //usleep(100);
        printf("father fork()return value is %d, count = %d\n",pid, count);
    }
    //如果返回值为负数，则出现了错误
    else
    {
        fprintf(stderr,"ERROR: fork() failed\n");
    }
    //sleep(1);
    return 0;
}
