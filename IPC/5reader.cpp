/*
* 信号量使用方法
* int semget(key_t key, int nsems, int semflg);
* 成功返回信号量，失败返回-1
* 
* 改变信号量的值
* int semop(int semid, struct sembuf* sops, unsigned nsops);
* 其中
* struct sembuf
* {
* 	short sem_num; 	//除非使用一组信号量，否则为0
*	short sem_op; 	//信号量在一次操作中需要改变的数据，通常是两个数
*			//一个是-1，即P(等待)操作，一个是+1，即V(发送信号)操作
*	short sem_flg;	//通常为SEM——UNDO,使操作系统跟踪信号
			//并在进程没有释放该信号量而终止时，操作系统释放信号量
* }
*
* 控制信号量
* int semctl(int semid, int semnu, int cmd[, semun un])
* 如果有第四个参数,则通常为semnu参数
* union semnu
* {
* 	int val;
*	struct semid_ds *buf;
*	unsigned short *arry;
* }
* 
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
	//创建新的共享内存
	int shmid = shmget(SHM_KEY,sizeof(int),IPC_CREAT|0777);
	if(shmid < 0)
	{
		perror("shmget failed");
		return -1;
	}
	
	//将共享内存映射到程序的内存空间中
	void* sharedMem = shmat(shmid, NULL, 0);
	if(sharedMem == (void*)-1)
	{
		perror("shmat failed");
		return -1;
	}	
	int* data = (int *)sharedMem;
	
	//创建新的信号量(两个信号量)
	int semid = semget(SEM_KEY,2,IPC_CREAT|0777);
	if(semid == -1)
	{
		perror("semget failed");
		return -1;
	}

	//初始化两个信号量
	Semun semun;
	semun.val = 0;
	semctl(semid, 0, SETVAL, semun);
	semun.val = 1;
	semctl(semid, 1, SETVAL, semun);

	struct sembuf semBuf;
	while(1)
	{
		//sem_num = 0表示下面的操作指向第一个信号量
		semBuf.sem_num = 0;
		semBuf.sem_op = -1;
		semBuf.sem_flg = SEM_UNDO;
		semop(semid, &semBuf, 1);
		printf("the NUM:%d\n", *data);

		semBuf.sem_num = 1;
		semBuf.sem_op = 1;
		semBuf.sem_flg = SEM_UNDO;
		semop(semid, &semBuf, 1);
	}
	return 0;
}
