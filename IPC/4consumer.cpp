/*
* 共享内存用法
* 创建共享内存
* #include <sys/shm.h>
* int shmid = shmget(key_t key, int size, int flag);
* flag常用参数为 IPC_CREAT|0777
* 将共享内存映射到自身的内存空间中
* void* shmat(shmid, void* addr, int flag);
* 将共享内存东当前进程内存空间剥离
* int shmdt(const void* shmaddr);
* 删除共享内存
* shmctl(shmid, IPC_RMID, 0);
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>

//共享内存结构
typedef struct Shm
{
	int written;
	char text[2048];
}Shm;

int main()
{
	int shmid;
	//新建共享内存	
	shmid = shmget((key_t)1234, sizeof(Shm), 0777|IPC_CREAT);
	if(shmid == -1)
	{
		perror("shmget failed");
		return -1;
	}
	
	//将共享内存绑定到当前进程内存空间中
	void* sharedMem = shmat(shmid, (void*)0, 0);
	if(sharedMem == (void*)-1)
	{
		perror("shmat failed");
		return -1;
	}
	printf("Memory attached at %lX\n", (long)sharedMem);
	
	//当written被置位时，从共享内存中读取消息
	Shm* shm;
	shm = (Shm*) sharedMem;
	shm->written = 0;
	while(1)
	{
		printf("We are waiting for Msg\n");
		if(shm->written)
		{
			printf("Msg in shared memory is %s\n", shm->text);
			shm->written = 0;
			if(memcmp(shm->text, "quit", 4) == 0)
				break;
		}
		sleep(1);
	}
	
	//取消当前进程内存空间与共享内存的链接
	if(shmdt(sharedMem) == -1)
	{
		perror("shmdt failed");
		return -1;
	}
	
	//删除共享内存
	if(shmctl(shmid, IPC_RMID, 0) == -1)
	{
		perror("shctl failed");	
		return -1;
	}
	return 0;
}
