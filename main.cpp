




#define MYTEST_THREAD1
#ifdef MYTEST_THREAD1
//该文档主要实现了linux中的多线程操作
//来源：https://www.cnblogs.com/armlinux/archive/2010/05/28/2396997.html
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#define MAX 10

//pthread_t原型
//typedef unsigned long int pthread_t;
//它是一个线程的标识符
pthread_t thread[2];
pthread_mutex_t mut;
int number = 0, i;

void *thread1(void *)
{
    printf("thread1: I'm thread 1\n");
    for(i=0;i<MAX;i++)
    {
        printf("thread1: number = %d\n",number );
        /*
         pthread_mutex_lock声明开始用互斥锁上锁，此后的代码直至调用pthread_mutex_unlock为止，
         均被上锁，即同一时间只能被一个线程调用执行。当一个线程执行到pthread_mutex_lock处时，
         如果该锁此时被另一个线程使用，那此线程被阻塞，即程序将等待到另一个线程释放此互斥锁。
        */
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        //pthread_exit(NULL);   //如果执行该语句，该函数会直接返回。如果该函数不是通过pthread_create()调用的，该语句会直接终止程序
        //sleep()单位为秒
        //usleep()单位为微秒
        sleep(2);
    }
    printf("thread1: 主函数在等我完成任务吗？\n");
    /*
    功能：结束一个线程
    原型：extern void pthread_exit (void *__retval) __attribute__ ((__noreturn__));
    */
    //pthread_exit(NULL);
    return NULL;
}

void *thread2(void *)
{
    printf("thread2: I'm thread 2 \n");
    for(i = 0;i<MAX; i++)
    {
        printf("thread2: number = %d\n", number);
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        sleep(3);
    }
    printf("thread2: 主函数在等我完成任务吗？\n");
    //pthread_exit(NULL);
    return NULL;
}

/**
自定义的创建线程的函数
*/
void thread_create(void)
{
    int temp;
    memset(&thread,0,sizeof(thread));
    /*
    功能：创建线程函数原型
    原型：extern int pthread_create (pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg) __THROWNL __nonnull ((1, 3));
    pthread_create(指向线程标识符的指针，线程属性(NULL表示默认属性)，线程运行函数的起始地址，函数运行的参数)

    https://www.cnblogs.com/armlinux/archive/2010/05/28/2396997.html
    [第一个参数为指向线程标识符的指针，第二个参数用来设置线程属性，第三个参数是线程运行函数的起始地址，最后一个
    参数是运行函数的参数。这里，我们的函数thread不需要参数，所以最后一个参数设为空指针。第二个参数我们也设为空
    指针，这样将生成默认属性的线程。对线程属性的设定和修改我们将在下一节阐述。当创建线程成功时，函数返回0，若不
    为0则说明创建线程失败，常见的错误返回代码为EAGAIN和EINVAL。前者表示系统限制创建新的线程，例如线程数目
    过多了；后者表示第二个参数代表的线程属性值非法。创建线程成功后，新创建的线程则运行参数三和参数四确定的函数，
    原来的线程则继续运行下一行代码。]
    */
    if((temp = pthread_create(&thread[0],NULL,thread1,NULL))!=0)
        printf("创建线程1失败！\n");   //也就是说，pthread_create函数成功返回0
    else
        printf("线程1被创建\n");
    if((temp = pthread_create(&thread[1],NULL,thread2,NULL))!=0)
        printf("线程2创建失败\n");
    else
        printf("线程2被创建\n");
}

void thread_wait(void)
{
    /*等待线程结束*/
    if(thread[0]!=0)
    {
        /*
        功能：等待一个线程的结束
        原型：extern int pthread_join (pthread_t __th, void **__thread_return);
        pthread_join(进程标识符，程序返回值保存的位置)
        该函数会阻塞程序进行，也就是说在thread[0]结束之前，printf()函数都无法执行
        除了使用pthread_join()函数等待线程结束外，还可以使用pthread_exit()

        最后要说明的是，一个线程不能被多个线程等待，否则第一个接收到信号的线程
        成功返回，其余调用pthread_join的线程则返回错误代码ESRCH
        */
        pthread_join(thread[0],NULL);
        printf("线程1已经结束\n");
    }
    if(thread[1] !=0)
    {
        pthread_join(thread[1], NULL);
        printf("线程2已经结束\n");
    }
}

int main()
{
    /*
    功能：用默认属性初始化互斥锁
    原型：extern int pthread_mutex_init (pthread_mutex_t *__mutex,
			       const pthread_mutexattr_t *__mutexattr)
     __THROW __nonnull ((1));
     如果需要声明特定属性的互斥锁，须调用函数 pthread_mutexattr_init
     函数pthread_mutexattr_setpshared和函数 pthread_mutexattr_settype
     用来设置互斥锁属性
    */
    pthread_mutex_init(&mut,NULL);
    //thread1(NULL);
    printf("我是主函数，我正在创建线程\n");
    thread_create();
    printf("我是主函数，我正在等待线程完成任务\n");

    thread_wait();
    printf("程序执行完毕\n");
    return 0;
}
#endif













