#include <assert.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "threadpool.h"
using std::cout; using std::cin; using std::endl;
using std::stringstream;

#define DEBUG_PRINT_ERROR(MSG) {stringstream str; str << "Error in: "<< __FUNCTION__ << ",line:" << __LINE__ << endl << MSG << endl; cout << str.str();}
#define DEBUG_PRINT_PROCESS(MSG) {stringstream str; str << __FUNCTION__ << ", line:" << __LINE__ << endl << syscall(224) << ": " << MSG << endl << endl; cout << str.str();}
#define DEBUG_PRINT_FUNCTION_IN {stringstream str; str << syscall(224) << ":Enter " <<__FUNCTION__ << endl << endl; cout << str.str(); }
#define DEBUG_PRINT_FUNCTION_OUT {stringstream str; str << syscall(224) << ":Exit " <<__FUNCTION__ << endl << endl; cout <<str.str();}

//初始化线程池
struct threadpool* threadpool_init(int thread_num, int queue_max_num)
{
	DEBUG_PRINT_FUNCTION_IN
	struct threadpool* pool=NULL;
	do
	{
		pool = (threadpool*)malloc (sizeof(struct threadpool));
		if(NULL == pool)
		{
			DEBUG_PRINT_ERROR("failed to malloc threadpool!\n");
			break;
		}
		pool->thread_num = thread_num;
		pool->queue_max_num = queue_max_num;
		pool->queue_cur_num = 0;
		pool->head = NULL;
		pool->tail = NULL;
		if(pthread_mutex_init(&(pool->mutex),NULL))
		{
			DEBUG_PRINT_ERROR("failed to init mutex!\n");
			break;
		}
		if(pthread_cond_init(&(pool->queue_empty),NULL))
		{
			DEBUG_PRINT_ERROR("failed to init queue_empty");
			break;
		}
		if(pthread_cond_init(&(pool->queue_not_empty),NULL))
		{
			DEBUG_PRINT_ERROR("failed to init queue_not_empty");
			break;
		}
		if(pthread_cond_init(&(pool->queue_not_full),NULL))
		{
			DEBUG_PRINT_ERROR("failed to init queue_not_full");
			break;
		}
		pool-> pthreads = (pthread_t*)malloc(sizeof(pthread_t)* thread_num);
		if(NULL == pool->pthreads)
		{
			DEBUG_PRINT_ERROR("failed to malloc ptheads!\n");
			break;
		}
		pool->queue_close = 0;
		pool->pool_close = 0;
		int i;
		//创建线程池
		DEBUG_PRINT_PROCESS("开始创建子线程");
		for(i=0; i<pool->thread_num; ++i)
		{
			pthread_create(&(pool->pthreads[i]),NULL,threadpool_function,(void*)pool);
		}
		DEBUG_PRINT_FUNCTION_OUT;
		return pool;
	}while(0);
	return NULL;
}

//向线程池添加任务
int threadpool_add_job(struct threadpool* pool, void* (*callback_function)(void *arg), void *arg)
{
	DEBUG_PRINT_FUNCTION_IN
	assert(pool != NULL);
	assert(callback_function != NULL);
	assert(arg != NULL);

	pthread_mutex_lock(&(pool->mutex));
	DEBUG_PRINT_PROCESS("获得互斥锁");
	while((pool->queue_cur_num == pool->queue_max_num) && !(pool->queue_close || pool->pool_close))
	{
		// 队列满时等待
		DEBUG_PRINT_PROCESS("队列已满，等待线程处理数据后空出队列");
		pthread_cond_wait(&(pool->queue_not_full),&(pool->mutex));
	}

	if(pool->queue_close || pool->pool_close)
	{
		//队列关闭就退出
		DEBUG_PRINT_PROCESS("queue_close或者pool_close设置为1");
		pthread_mutex_unlock(&(pool->mutex));
		return -1;
	}
	
	//向队列添加任务
	struct job* pjob = (struct job*)malloc(sizeof(struct job));
	if(pjob == NULL)
	{
		DEBUG_PRINT_ERROR("job malloc error");
		pthread_mutex_unlock(&(pool->mutex));
		return -1;
	}

	pjob->callback_function = callback_function;
	pjob->arg = arg;
	pjob->next = NULL;

	if(pool->head == NULL)
	{
		pool->head = pool->tail = pjob;
		DEBUG_PRINT_PROCESS("队列为空，我们现在添加数据到队列中,并通知其他线程队列不再为空");
		//队列为空时，有任务到来则通知线程池中的线程，队列不再为空
		pthread_cond_broadcast(&(pool->queue_not_empty));
	}
	else
	{
		pool->tail->next = pjob;
		pool->tail = pjob;
	}

	pool->queue_cur_num++;
	pthread_mutex_unlock(&(pool->mutex));
	DEBUG_PRINT_FUNCTION_OUT;
	return 0;
}

//使用该函数创建线程，该函数同时调用回调函数处理数据
//arg为线程池pool
void* threadpool_function(void* arg)
{
	DEBUG_PRINT_FUNCTION_IN
	struct threadpool* pool = (struct threadpool*)arg;
	struct job* pjob = NULL;
	//线程死循环，从队列中取消息并调用回调函数处理数据
	while(1)
	{
		pthread_mutex_lock(&(pool->mutex));
		DEBUG_PRINT_PROCESS("获得互斥锁");
		while((pool->queue_cur_num == 0) && !pool->pool_close)
		{
			//任务队列为空，则等待任务
			DEBUG_PRINT_PROCESS("线程等待任务队列非空");
			pthread_cond_wait(&(pool->queue_not_empty),&(pool->mutex));
		}
		if(pool->pool_close)
		{
			pthread_mutex_unlock(&(pool->mutex));
			pthread_exit(NULL);
		}

		pool->queue_cur_num--;
		pjob = pool->head;
		if(pool->queue_cur_num == 0)
		{
			pool->head = pool->tail = NULL;
		}
		else
		{
			pool->head = pjob->next;
		}

		if(pool->queue_cur_num == 0)
		{
			//通告队列为空
			pthread_cond_signal(&(pool->queue_empty));
		}
		if(pool->queue_cur_num == pool->queue_max_num -1)
		{
			//通告队列非满
			pthread_cond_broadcast(&(pool->queue_not_full));
		}
		pthread_mutex_unlock(&(pool->mutex));

		//线程调用回调函数处理数据
		DEBUG_PRINT_PROCESS("正在处理消息");
		(*(pjob->callback_function))(pjob->arg);
		free(pjob);
		pjob = NULL;
	}
	DEBUG_PRINT_FUNCTION_OUT;
}

// 销毁线程池
int threadpool_destory(struct threadpool* pool)
{
	DEBUG_PRINT_FUNCTION_IN
	assert(pool != NULL);
	pthread_mutex_lock(&(pool->mutex));
	DEBUG_PRINT_PROCESS("获得互斥锁");
	DEBUG_PRINT_PROCESS("准备销毁线程池");
	//若线程池已经销毁
	if(pool->queue_close || pool->pool_close)
	{
		pthread_mutex_unlock(&(pool->mutex));
		return -1;
	}

	//关闭队列
	pool->queue_close = 1;
	while(pool->queue_cur_num != 0)
	{
		//等待队列为空
		DEBUG_PRINT_PROCESS("等待队列为空");
		pthread_cond_wait(&(pool->queue_empty),&(pool->mutex));
	}

	//关闭线程
	pool->pool_close = 1;
	pthread_mutex_unlock(&(pool->mutex));
	//唤醒线程池中正在阻塞的线程
	pthread_cond_broadcast(&(pool->queue_not_empty));
	//唤醒添加任务的thread_add_job函数
	pthread_cond_broadcast(&(pool->queue_not_full));
	int i;
	for(i=0; i<pool->thread_num;++i)
	{
		//等待线程池中的线程执行完毕
		pthread_join(pool->pthreads[i],NULL);
	}

	//清理资源
	pthread_mutex_destroy(&(pool->mutex));
	pthread_cond_destroy(&(pool->queue_empty));
	pthread_cond_destroy(&(pool->queue_not_empty));
	pthread_cond_destroy(&(pool->queue_not_full));
	free(pool->pthreads);
	struct job *p;
	while(pool->head != NULL)
	{
		p = pool->head;
		pool->head = p->next;
		free(p);
	}
	free(pool);
	DEBUG_PRINT_FUNCTION_OUT;
	return 0;
}


void* work(void* arg)
{
	char* p = (char*) arg;
	printf("threadpool callback function:%s.\n",p);
	usleep(rand()%1000000+100000);
}

int main()
{
	DEBUG_PRINT_FUNCTION_IN;
	struct threadpool* pool = threadpool_init(4,8);
	char* arg[40] ;
	for(int i=0; i<40; i++)
	{
		arg[i] = (char*)malloc(3);
		arg[i][0] = i/10 + '0';
		arg[i][1] = i%10 + '0';
		arg[i][2] = '\0';
		threadpool_add_job(pool,work,arg[i]);
	}

	sleep(2);
	threadpool_destory(pool);

	for(int i=0; i<40;i++)
		free(arg[i]);

	DEBUG_PRINT_FUNCTION_OUT;
	sleep(20);
	return 0;
}

