//#define MYTEST_THREAD1
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
    for(i=0; i<MAX; i++)
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
    for(i = 0; i<MAX; i++)
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
#endif //MYTEST_THREAD1




//多线程实例2
//#define MYTEST_THREAD2
#ifdef MYTEST_THREAD2
/*
  http://bbs.csdn.net/topics/391922234
 * Copyright 2016 Che Hongwei <htc.chehw@gmail.com>
 * The MIT License (MIT)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define N 6
/*
将cond和mutex绑定在一起，并且该mutex仅用于对该cond的锁定
如果将该结构体中的mutex用作其他的加锁解锁应用，可能造成意想不到的结果
*/
typedef struct cond_mutex
{
    pthread_cond_t cond;        //条件变量
    pthread_mutex_t mutex;  //互斥锁
} cond_mutex_t;

/*
初始化互斥锁和条件变量
*/
cond_mutex_t cm_g=
{
    PTHREAD_COND_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};

static void * wait_thread(void* param);         //等待进程
static void * worker_thread(void* param);    //工作者进程

pthread_mutex_t mutex_g=PTHREAD_MUTEX_INITIALIZER;
static int count_g = 0;
static int data_g[N];
volatile int quit_g = 0;

int main(int argc, char** argv)
{
    int i;
    int *symbols = new int[10] {0,1,2,3,4,5,6,7,8,9};       //用以标识不同的线程
#define NUM_WORKERS (4)
    pthread_t th[NUM_WORKERS+1];
    void * ret_code = NULL;
    //创建等待进程
    if(pthread_create(&th[0],NULL,wait_thread,NULL) != 0)
    {
        perror("pthread_create");
        exit(1);
    }
    //usleep(5000);   //5ms
    //创建工作者进程
    for(i=1; i<=NUM_WORKERS; ++i)
    {
        //将symbols+i传递到线程，以标识不同的线程
        if(0 != pthread_create(&th[i],NULL,worker_thread,symbols+i))
        {
            perror("pthread_create");
            exit(1);
        }
    }
    char c = 0;
    printf("press enter to quit_g. \n");
    while(1)
    {
        scanf("%c",&c);
        if(c == '\n')
            break;
    }
    quit_g = 1;
    //激活在所有进程中正在等待中的条件变量
    pthread_cond_broadcast(&cm_g.cond);
    //等待相应的进程结束
    pthread_join(th[0], &ret_code);
    for(i=1; i<NUM_WORKERS; ++i)
    {
        pthread_join(th[i],&ret_code);
    }
    pthread_mutex_destroy(&mutex_g);
    pthread_cond_destroy(&cm_g.cond);
    pthread_mutex_destroy(&cm_g.mutex);
    delete[] symbols;
    return (int) (long) ret_code;
}

static void * worker_thread(void * param)
{
    while(!quit_g)
    {
        //先加锁，再修改data和count的值
        pthread_mutex_lock(&mutex_g);
        //printf("线程%d\n",*(int *)param);
        if(quit_g)  //在等待的过程中，用户可能已经触发了该信号
        {
            pthread_mutex_unlock(&mutex_g);
            break;
        }
        if(count_g == N)    //某一个 工作者线程 已经触发了条件变量，但 等待线程还没来得及加锁
        {
            printf("线程%d正在与[等待进程]争抢资源...\n",*(int *)param);
            pthread_mutex_unlock(&mutex_g);
            usleep(100000);
            continue;
        }
        if(count_g > N) //正常逻辑情况下，该条件不会发生
        {
            fprintf(stderr,"同步的逻辑出现问题\n");
            printf("请按回车键退出...\n");
            quit_g = 1;
            pthread_mutex_unlock(&mutex_g);
            break;
        }
        unsigned int seed=time(NULL);        //获取当前时间（从1900年到现在的秒数）
        seed = rand_r(&seed)%10000;
        usleep(seed);     //模拟真实场景下可能需要的工作量
        data_g[count_g++] = *(int*) param;
        if(count_g == N)
        {
            pthread_mutex_lock(&cm_g.mutex);
            pthread_cond_signal(&cm_g.cond);
            pthread_mutex_unlock(&cm_g.mutex);
        }
        pthread_mutex_unlock(&mutex_g);

    }
    pthread_exit((void*) (long) 0);
}

static void * wait_thread(void *)
{
    int i,sum;
    //cm_g.mutex互斥锁仅用于cond_wait
    pthread_mutex_lock(&cm_g.mutex);
    while(!quit_g)
    {
        pthread_cond_wait(&cm_g.cond,&cm_g.mutex);
        if(quit_g) break;
        pthread_mutex_lock(&mutex_g);
        sum = 0;
        printf("average = (");
        for(i=0; i<N; ++i)
        {
            sum+=data_g[i];
            printf(" %3d ",data_g[i]);
            if(i<(N-1))
                printf("+");
        }
        printf(") / %d =  %.2f\n",N,(double)sum/ (double)N);
        count_g = 0;
        pthread_mutex_unlock(&mutex_g);
    }
    pthread_mutex_unlock(&cm_g.mutex);
    pthread_exit((void*)(long)0);
}
#endif // MYTEST_THREAD2


#define MYTEST_THREAD3
#ifdef MYTEST_THREAD3
/*
根据MYTEST_THREAD2的灵感，想出如下的问题。
任务：
创建5个线程，第1到第4个线程以竞争的方式向一个buf中提供随机数或者固定的数，如第1个
线程提供数字1或者随机数到buf,第2个进程提供数字2或者随机数到buf。当buf中数的个数为N
时，暂停1-4号线程，并在第5个线程中计算5个数的平均数，计算完成后清空buf，1-4号线程
再次以竞争的方式向buf提供数字。。。
分析：
1-4号线程必须利用互斥锁，以避免向buf中填充数字时发生冲突
buf中的数字个数小于N时，5号线程需要暂停等待
当1-4号线程发现buf中的数字个数为N是，唤醒5号线程
5号线程运行过程中，需要暂停1-4号线程，以避免buf的读写冲突
(也就是说1-5号进程两两互斥)
1-4号线程可以使用同一个函数
设置一个退出的flag全局变量，当flag没有被设置为ture时，1-5号线程进行while循环
当flag被设置为true时，线程尝试结束
*/
#include <pthread.h>         //pthread相关
#include <stdio.h>          //标准输入输出
#include <unistd.h>         //包含sleep()函数
#include <stdlib.h>         //包含rand_r()函数，用以生成一个随机数

#define N  6                    //buf中需要的数字个数
#define RANDOM false    //使用随机数还是固定的数。使用随机数更显该程序的真实，使用固定的数便于程序理解

//线程1-4调用该函数，用于向buf中填充数据
static void * thread1_4(void *);
//线程5调用，用于计算buf中N个数的平均数
static void * thread5(void *);

//互斥锁，用于线程1-5争夺对buf的操作
pthread_mutex_t mutex_g = PTHREAD_MUTEX_INITIALIZER;
//条件锁，用于线程1-4在检测到top==N时唤醒线程5
pthread_cond_t cond_g = PTHREAD_COND_INITIALIZER;
//互斥锁，用于唤醒线程在唤醒线程5后，自己加锁等待
pthread_mutex_t mutex2_g = PTHREAD_MUTEX_INITIALIZER;
//条件锁，用于线程5在计算完平均数后，唤醒 唤醒线程5的线程
pthread_cond_t cond2_g = PTHREAD_COND_INITIALIZER;
//存放N个数的buf
int buf[N] = {0};
//指向buf中的top位置，如果top=N，线程5被唤醒
int top = 0;
//程序退出信号，当被设置为ture时，程序退出
bool exit_flag = false;
//随机数种子
unsigned int seed;

int main()
{
    top = 0;
    int *symbols = new int[5];    //标识5个线程
    for(int i=0; i<5; i++)
        symbols[i] = i+1;
    pthread_t thread[5];
    if(pthread_create(&thread[4], NULL, thread5, &symbols[4])!=0)
    {
        printf("线程5创建失败!\n");
        return -1;
    }
    for(int i=0; i<4; i++)
    {
        if(pthread_create(&thread[i],NULL,thread1_4,&symbols[i])!=0)
        {
            printf("线程%d创建失败!\n",i+1);
            return -1;
        }
    }
    char c;
    while(1)
    {
        //在键盘输入'\n'之前，main函数将会在此等待，以避免结束所有线程
        c=getchar();
        if (c == '\n')
            break;
    }
    //将结束程序标志设为ture，各个线程准备结束
    exit_flag = true;
    for(int i=0; i<5; i++)
        pthread_join(thread[i],NULL);
    delete[] symbols;
    return 0;
}

static void * thread1_4(void * id)
{
    int threadNumber = *(int *)id;
    printf("线程%d创建成功\n",threadNumber);
    while(!exit_flag)
    {
        //因为线程1-4需要竞争对buf的写入，所以需要加上互斥锁
        pthread_mutex_lock(&mutex_g);
        if(exit_flag)   //等待后被唤醒，首先检查退出标志是否已经被置位
        {
            pthread_mutex_unlock(&mutex_g);
            break;
        }
        if(top == N)
        {
            usleep(1);
            pthread_mutex_unlock(&mutex_g);
            continue;
        }
        //填充buf
        if(RANDOM == true)
        {
            //获得一个1-100的随机数
            buf[top++] = rand();
        }
        else
            buf[top++] = threadNumber;

        if(top == N)
        {
            // 注：可以在这里也加一个cond条件，避免线程1-4与线程5抢夺资源，而是利用线程5唤醒该线程
            pthread_cond_signal(&cond_g);
            //在线程5计算平均数的过程中，不允许线程1-4对buf进行修改，一方面可以通过线程1-4判断top的值，如果
            //top==N，则continue等待
            //更好的办法是，在这里再加一个锁进行等待，等待线程5计算完平均数后，再继续执行，代码如下
        }
        usleep(1);
        pthread_mutex_unlock(&mutex_g);
    }
    printf("线程%d运行结束\n",threadNumber);
    return NULL;
}

static void * thread5(void * id)
{
    int threadNumber = *(int *)id;
    printf("线程%d创建成功\n",threadNumber);
    while(!exit_flag)
    {
        pthread_mutex_lock(&mutex_g);
        //等待其他线程根据条件top=N唤醒
        //如果在等待的过程中，exit_flag被设置为true，应怎么办？
        pthread_cond_wait(&cond_g,&mutex_g);
        if(exit_flag)   //等待后被唤醒，首先检查退出标志是否已经被置位
        {
            pthread_mutex_unlock(&mutex_g);
            break;
        }
        //打印求平均值的过程
        int sum = 0;
        printf("\t\t\t(");

        top = 0;
        usleep(1);
        //printf("\t\t\t线程%d释放锁资源\n",threadNumber);
        //pthread_mutex_unlock(&mutex_g);
    }
    printf("\t\t\t线程%d运行结束\n",threadNumber);
    return NULL;
}
#endif // MYTEST_THREAD3


















