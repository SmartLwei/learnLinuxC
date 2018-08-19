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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>

#define SEM_KEY 1234
#define SHM_KEY 5678

typedef union Semun
{
	int val;
}Semun;

int main()
{
	//获取共享内存
	int shmid = shmget(SHM_KEY, sizeof(int), 0777);
	if(shmid < 0)
	{
		perror("shmget failed");
		return -1;	
	}

	//建立本程序到共享内存的链接
	void* sharedMem = shmat(shmid, NULL, 0);
	if(sharedMem == (void*)-1)
	{
		perror("shmat failed");
		return -1;
	}
	int* data = (int *)sharedMem;

	//新建信号量
	int semid = semget(SEM_KEY, 2, 0777);
	struct sembuf semBuf;
	Semun semun;
	while(1)
	{
		//指向第二个信号量
		semBuf.sem_num = 1;
		semBuf.sem_op = -1;
		semBuf.sem_flg = SEM_UNDO;
		semop(semid, &semBuf, 1);
		scanf("%d",data);
		semBuf.sem_num = 0;
		semBuf.sem_op = 1;
		semBuf.sem_flg = SEM_UNDO;
		semop(semid, &semBuf, 1);
	}
	return 0;
}
