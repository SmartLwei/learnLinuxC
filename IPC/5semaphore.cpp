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



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>

// 联合体，用于semctl初始化
union semun
{
	int              val; /*for SETVAL*/
	struct semid_ds *buf;
	unsigned short  *array;
};

// 初始化信号量
int init_sem(int sem_id, int value)
{
	union semun tmp;
	tmp.val = value;
	if (semctl(sem_id, 0, SETVAL, tmp) == -1)
	{
		perror("Init Semaphore Error");
		return -1;
	}
	return 0;
}

// P操作:
//    若信号量值为1，获取资源并将信号量值-1 
//    若信号量值为0，进程挂起等待
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
//    释放资源并将信号量值+1
//    如果有进程正在挂起等待，则唤醒它们
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
	union semun tmp;
	if (semctl(sem_id, 0, IPC_RMID, tmp) == -1)
	{
		perror("Delete Semaphore Error");
		return -1;
	}
	return 0;
}


int main()
{
    // 信号量集ID
    int sem_id;  
   key_t key;
   pid_t pid;

   // 获取key值
   if ((key = ftok(".", 'z')) < 0)
   {
       perror("ftok error");
       exit(1);
   }

   // 创建信号量集，其中只有一个信号量
   if ((sem_id = semget(key, 1, IPC_CREAT | 0666)) == -1)
   {
       perror("semget error");
       exit(1);
   }

   // 初始化：初值设为0资源被占用
   init_sem(sem_id, 0);

   if ((pid = fork()) == -1)
       perror("Fork Error");
else if (pid == 0) /*子进程*/
    {
        sleep(2);
        printf("Process child: pid=%d\n", getpid());
        sem_v(sem_id);  /*释放资源*/
    }
else  /*父进程*/
    {
        sem_p(sem_id);   /*等待资源*/
        printf("Process father: pid=%d\n", getpid());
        sem_v(sem_id);   /*释放资源*/
        del_sem(sem_id); /*删除信号量集*/
    }
    return 0;
}
