#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <pthread.h>

/**
 * 生产者——消费者模型
 * 生产者向任务队列中添加任务，每个任务包含：
 * 该任务需要调用那个函数处理+函数的参数
 * 为了构造队列，还需要包含一个next的指针
 * 子进程运行过程中调用回调函数处理job中的参数
 * */
struct job
{
	//子线程的回调函数，子线程调用该函数处理数据
	void* (*callback_function)(void* arg);
	//传递给子进程的参数
	void* arg;
	//使用链表构成一个任务队列
	struct job* next;
};

/**
 * 线程池"句柄"
 * 包含线程中加锁和同步相关的信息
 * */
struct threadpool
{
	int thread_num;					//线程池中的线程个数
	int queue_max_num;				//排队的最大任务数量
	struct job* head;				//任务队列中的第一个，当一个线程空闲后取该任务执行
	struct job* tail;				//任务队列中的最后一个，添加任务时添加到该任务之后
	pthread_t *pthreads;			//线程池中的所有线程
	pthread_mutex_t mutex;			//互斥锁，对队列的所有操作必须互斥
	pthread_cond_t queue_empty;		//任务队列为空的条件锁。当关闭线程池时，使用该条件保证队列中的所有任务都被处理
	pthread_cond_t queue_not_empty;	//任务队列不为空的条件锁。当子进程提取job来处理，但是job队列为空时，会等待该条件
									//此后当添加新的任务时，可以触发该条件
	pthread_cond_t queue_not_full;	//当向任务队列添加任务时，如果任务队列已满，则添加任务的线程应该等待队列非满
									//此后当有子进程提起一个job处理时，因为此时任务数量-1，可以触发该条件
	int queue_cur_num;				//当前排队的队列长度
	int queue_close;				//关闭队列
	int pool_close;					//关闭线程池
};

/**
 * 初始化线程池
 * @thread_num		子线程数量
 * @queue_max_num	最大任务队列
 * */
struct threadpool* threadpool_init(int thread_num, int queue_max_num);

/**
 * 向任务队列添加任务
 * 添加时如果任务队列有空转换为非空，需要触发条件变量queue_not_empty
 * 如果任务队列已满，则等待条件变量queue_not_full，然后再添加
 * 需要随时注意线程池是否已经发出了关闭线程池或者关闭任务队列的指令
 * @pool				线程池
 * @callback_function	函数指针，使用这个函数处理数据
 * @arg					数据，回调函数的参数
 * */
int threadpool_add_job(struct threadpool* pool, void* (*callback_function)(void *arg), void* arg); 

/**
 * 使用这个函数创建子线程
 * 该函数从线程池的任务队列中提取任务
 * 提起任务时，如果任务队列为空，则等待条件变量queue_not_empty
 * 如果提取任务之前，队列已满，则提起之后触发条件变量queue_not_full
 * 提取任务之后，如果任务队列为空，则触发条件变量queue_empty
 * 需要随时注意线程池是否已经发出了关闭线程池或者关闭任务队列的指令
 * @arg				线程池
 * */
void* threadpool_function(void* arg);

/**
 * 销毁线程池，程序退出时调用
 * 在该函数中将关闭任务队列和关闭线程池的标志置位
 * 该函数需要等待条件变量queue_empty，保证队列中的所有任务都被处理
 * @pool			线程池
 * */
int threadpool_destory(struct threadpool* pool);


#endif
