/*
* 信号量使用方法
* int semget(key_t key, int nsems, int semflg);
* 成功返回信号量，失败返回-1
* nsems表示申请的信号量个数，若信号量已经由其他进程创建，则设为0
* semflg通常设置为 IPC_CREAT | 0666 其他可选参数为 SEM_R(read)，SEM_A(alter)，IPC_EXCL 
*
* 改变信号量的值
* int semop(int semid, struct sembuf* sops, unsigned nops);
* 其中
* struct sembuf
* {
* 	short sem_num; 	//信号量的序号，从0开始计数。0是第一个信号量，1是第二个信号量...
*	short sem_op; 	//-1：P操作，对信号量的值-1。如果当前信号量的值为0，则阻塞等待
*			//+1：V操作，对信号量的值+1。如果有进程在等待该信号，则唤醒那个进程
*			//应该可以是其他数值吧？表示对信号量作 +num 或 -num 操作
*	short sem_flg;	//通常为SEM_UNDO,使操作系统跟踪信号
*			//并在进程没有释放该信号量而终止时，操作系统释放信号量
*			//其他值：IPC_NOWAIT(出错返回EAGAIN)
* };
* nops指示spos，即第二个参数中sembuf数组的个数，通常为1
*
* 控制信号量
* int semctl(int semid, int sem_num, int cmd [可选参数, semun un])
* union semnu
* {
* 	int val;
*	struct semid_ds *buf;
*	unsigned short *arry;
* };
* struct semid_ds 
* {
*	struct ipc_permsem_perm ;
*	structsem* sem_base ; 	//信号数组指针
*	ushort sem_nsem ; 	//此集中信号个数
*	time_t sem_otime ; 	//最后一次semop时间
*	time_t sem_ctime ; 	//最后一次创建时间
* };
* cmd常用的值为
* SETVAL	设置信号量集中的一个单独的信号量的值
* IPC_RMID 	从内存中删除该信号量
* IPC_STAT	读取一个信号量集的数据结构semid_ds，并将其存储在semun中的buf参数中
* IPC_SET	设置信号量集的数据结构semid_ds中的元素ipc_perm，其值取自semun中的buf参数
* GETALL	用于读取信号量集中的所有信号量的值
* GETNCNT	返回正在等待资源的进程数目
* GETPID	返回最后一个执行semop操作的进程的PID
* GETVAL	返回信号量集中的一个单个的信号量的值
* GETZCNT	返回这在等待完全空闲的资源的进程数目
* SETALL	设置信号量集中的所有的信号量的值
*/



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#define PRINT_LOCATION printf("line:%d\n",__LINE__);

// 联合体，用于semctl初始化
typedef union Semun
{
	int              val; /*for SETVAL*/
	struct semid_ds *buf;
	unsigned short  *array;
}Semun;

// 初始化信号量
int init_sem(int sem_id, int value)
{
	union Semun tmp;
	tmp.val = value;
	if (semctl(sem_id, 0, SETVAL, tmp) == -1)
	{
		perror("Init Semaphore Error");
		return -1;
	}
	return 0;
}

// P操作:
// 若信号量值为1，获取资源并将信号量值-1 
// 若信号量值为0，进程挂起等待
int sem_p(int sem_id)
{
	struct sembuf sbuf;
	sbuf.sem_num = 0; /*序号*/
	sbuf.sem_op = -1; /*P操作*/
	sbuf.sem_flg = SEM_UNDO;

	if (semop(sem_id, &sbuf, 1) == -1)
	{
		perror("P operation Error");
		return -1;
	}
	return 0;
}

// V操作：
// 释放资源并将信号量值+1
// 如果有进程正在挂起等待，则唤醒它们
int sem_v(int sem_id)
{
	struct sembuf sbuf;
	sbuf.sem_num = 0; /*序号*/
	sbuf.sem_op = 1;  /*V操作*/
	sbuf.sem_flg = SEM_UNDO;

	if (semop(sem_id, &sbuf, 1) == -1)
	{
		perror("V operation Error");
		return -1;
	}
	return 0;
}

// 删除信号量集
int del_sem(int sem_id)
{
	union Semun tmp;
	if (semctl(sem_id, 0, IPC_RMID, tmp) == -1)
	{
		perror("Delete Semaphore Error");
		return -1;
	}
	return 0;
}

int main()
{
	//信号量集ID
	int sem_id;
	key_t key;
	pid_t pid;

	//获取key值
	if ((key = ftok(".", 'z')) < 0)
	{
		perror("ftok error");
		PRINT_LOCATION;
		exit(1);
	}

	//创建信号量集，其中只有一个信号量
	if ((sem_id = semget(key, 1, IPC_CREAT | 0666)) == -1)
	{
		perror("semget error");
		PRINT_LOCATION
		exit(1);
	}

	//初始化：初值设为0资源被占用
	//init_sem(sem_id, 0);
	Semun semun;
	semun.val = 0;
	if(semctl(sem_id, 0, SETVAL, semun) == -1)
	{
		perror("semctl failed");
		PRINT_LOCATION
		return -1;
	}


	if ((pid = fork()) == -1)
	{
		perror("Fork Error");
		PRINT_LOCATION;
		return -1;
	}
	
	//子进程
	//V操作：
	//释放资源并将信号量值+1
	//如果有进程正在挂起等待，则唤醒它们
	else if (pid == 0)
	{
		sleep(2);
		printf("Process child: pid=%d\n", getpid());
		//sem_v(sem_id);
		struct sembuf sbuf;
		//信号量序号
		sbuf.sem_num = 0;
		//V操作，对信号量+1
		sbuf.sem_op = 1;
		sbuf.sem_flg = SEM_UNDO;
		printf("Process child are going to set Signal0\n");
		if(semop(sem_id, &sbuf, 1) == -1)
		{
			perror("V operation failed");
			PRINT_LOCATION
			return -1;
		}
	}


	//父进程
	else
	{
		//P操作，等待资源
		//sem_p(sem_id);
		struct sembuf sbuf;
		//操作第0号进程
		sbuf.sem_num = 0;
		//操作为P操作
		sbuf.sem_op = -1;
		//该值一般设为SEM_UNDO
		sbuf.sem_flg = SEM_UNDO;

		//如果sem_id的0号信号量值为0，则阻塞等待其他程序将该信号量置1
		printf("Process father are waiting for Signal0 to be set\n");
		if(semop(sem_id, &sbuf, 1) == -1)
		{
			perror("P operation failed");
			PRINT_LOCATION
			return -1;
		}
		
		printf("Process father: pid=%d, and Signal0 have been set\n", getpid());

		//V操作
		//sem_v(sem_id);
		sbuf.sem_op = 1;
		if(semop(sem_id, &sbuf, 1) == -1)
		{
			perror("V operation failed");
			PRINT_LOCATION
			return -1;
		}


		//删除信号量
		if(semctl(sem_id,0,IPC_RMID, semun) == -1)
		{
			perror("Delete Semaphore Error");
			PRINT_LOCATION
			return 0;
		}
		else
		{
			printf("Delete Semaphore Succeed\n");
		}

	}
	printf("end\n");
	return 0;
}
