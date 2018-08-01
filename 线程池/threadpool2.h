#include <pthread.h>

struct job
{
	void* (*callback_function)(void *arg);	//线程回调函数
	void* arg;								//回调函数参数
	struct job* next;
};

struct threadpool
{
	int thread_num;					//线程池中开启线程的个数
	int queue_max_num;				//队列中最大job的个数
	struct job* head;				//指向job的头指针
	struct job* tail;				//指向job的尾指针
	pthread_t* pthreads;			//线程池中的所有线程
	pthread_mutex_t mutex;			//互斥信号量
	pthread_cond_t queue_empty;		//队列为空的条件变量
	pthread_cond_t queue_not_empty;	//队列不为空的条件变量
	pthread_cond_t queue_not_full;	//队列不为满的条件变量
	int queue_cur_num;				//队列当前的job个数
	int queue_close;				//队列是否已经关闭
	int pool_close;					//线程池是否已经关闭
};

//初始化线程池
//@pthread_num 线程池开启的线程个数
//@queue_max_num 队列的最大job个数
struct threadpool* threadpool_init(int thread_num, int queue_max_num);

//向线程池中添加任务
//@pool  线程池地址
//@callback_function 回调函数
//@arg 回调函数参数
int threadpool_add_job(struct threadpool* pool, void* (*callback_function)(void* arg), void* arg);

//销毁线程池
//@pool 线程池地址
int threadpool_destory(struct threadpool* pool);

//线程池中的线程函数
//@arg 线程池地址
void* threadpool_function(void* arg);
