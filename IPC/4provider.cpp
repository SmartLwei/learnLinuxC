/*
* 共享内存用法
* 创建共享内存
* #include <sys/shm.h>
* int shmid = shmget(key_t key, int size, int flag);
* flag常用参数为 IPC_CREAT | 0777
* 将共享内存映射到自身的内存空间中
* void* shmat(shmid, void* addr, int flag);
* 将共享内存东当前进程内存空间剥离
* int shmdt(const void* shmaddr);
* 删除共享内存
* shmctl(shmid, IPC_RMID, 0);
*
*
*在内核中共享内存的实现——仅供参考
struct shmid_ds
{
	struct ipc_perm shm_perm;	//操作权限
	int shm_segsz;			//段的大小（以字节为单位
	time_t shm_atime;		//最后一个进程附加到该段的时间
	time_t shm_dtime;		//最后一个进程离开该段的时间
	time_t shm_ctime;		//最后一个进程修改该段的时间
	unsigned short shm_cpid; 	//创建该段进程的pid
	unsigned short shm_lpid;	//在该段上操作的最后1个进程的pid
	short shm_nattch;		//当前附加到该段的进程的个数
	//下面是私有的
        unsigned short shm_npages;	//段的大小（以页为单位）
	unsigned long *shm_pages;	//指向frames->SHMMAX的指针数组
	struct vm_area_struct *attaches;	//对共享段的描述
};
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
	//获取共享内存
	shmid = shmget((key_t)1234,sizeof(Shm), 0777);
	if(shmid == -1)
	{
		perror("shmget failed");
		return -1;
	}
	
	//将共享内存导入到当前进程内存中
	void* sharedMem =  shmat(shmid, (void*)0, 0);
	if(sharedMem == (void*)-1)
	{
		perror("shmat failed");
		return -1;
	}
	printf("Memory attached at %lX\n",(long)sharedMem);
	
	//向共享内存中写入数据，并置位
	Shm* shm;
	shm = (Shm*)sharedMem;
	while(1)
	{
		while(shm->written == 1)
			usleep(100000);
		printf("Please Input Msg\n");
		fgets(shm->text, 1024, stdin);
		shm->written = 1;
		if(memcmp(shm->text, "quit", 4) == 0)
			break;	
	}

	//取消与共享内存的链接
	if(shmdt(sharedMem) == -1)
	{
		perror("shmdt failed");
		return -1;
	}
	
	return 0;
}
