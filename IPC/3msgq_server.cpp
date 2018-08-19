/*
 * 消息队列用法
 * 创建或者获取消息队列
 * int msgget(key_t key, int msgflg);
 * 其中msgflg常用参数
 * IPC_CREATE 如果没有该队列，则创建该队列
 * IPC_EXCL 如果没有该队列，则返回-1，若存在，则返回0
 * key可以当成端口号来理解，可以由ftok()生成
 *
 * 从队列中读取消息
 * 如果msgtyp为0，表示读取消息队列中的所有消息
 * ssize_t msgrcv(int msqid, void* buf, size_t bufLen, long msgtyp, int msgflg);
 * 向队列中写消息
 * int msgsnd(int msqid, const void* buf, size_t bufLen, int msgflg);
 * msgflg常用参数
 * IPC_NOWAIT 如果队列已满，则写函数不阻塞，而是立即返回-1，
 * 如果队列为空，读函数不阻塞，而是立即返回-1
 * 否则阻塞等待
 *
 * 设置消息队列属性
 * int msgctl(int msgqid, int cmd, struct msqid_ds *buf);
 * cmd常用参数
 * IPC_STAT
 * IPC_SET
 * IPC_RMID 从内核中删除该消息队列
 *
 *
 * 内核中实现方式——仅供参考
struct msqid_ds
{
	struct ipc_perm msg_perm;
	struct msg *msg_first;      	// first message on queue,unused 
	struct msg *msg_last;       	// last message in queue,unused
	__kernel_time_t msg_stime;  	// last msgsnd time
	__kernel_time_t msg_rtime;  	// last msgrcv time
	__kernel_time_t msg_ctime;  	// last change time
	unsigned long  msg_lcbytes; 	// Reuse junk fields for 32 bit
	unsigned long  msg_lqbytes; 	// ditto
	unsigned short msg_cbytes;  	// current number of bytes on queue
	unsigned short msg_qnum;    	// number of messages in queue
	unsigned short msg_qbytes;  	// max number of bytes on queue
	__kernel_ipc_pid_t msg_lspid;   // pid of last msgsnd
	__kernel_ipc_pid_t msg_lrpid;   // last receive pid
};
 * */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <memory.h>

//会话约定的唯一key
#define KEY "/etc/passwd"

//会话用的消息结构
typedef struct Msg
{
	long mType;
	char mText[1024];
}Msg;

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("please use ./process mType\n");
		printf("mType is msg Type that this server listens\n");
		printf("when server listened a \"quit\" msg, the server quit\n");
		printf("for example\n");
		printf("./process 999 start the server\n");
		printf("you can send a msg that have Type 999 and Text \"quit\" to quit the server\n");
		return -1;
	}
	int msqid;
	key_t key;
	Msg msg;
		
	//获取key值
	if((key = ftok(KEY,'z')) < 0)
	{
		perror("ftok error");
		return -1;
	}
	//还可以这样获取key
	//key = (key_t)1234;
	printf("key = %d\n",key);
	
	//创建消息队列
	if((msqid = msgget(key, IPC_CREAT|0777)) == -1)
	{
		perror("msgget error");
		return -1;
	}
	
	//打印消息队列ID和金称号
	printf("msqid = %d\n",msqid);
	printf("pid = %d\n",getpid());
	
	int writeLen = 0;
	int readLen = 0;

	//读取消息类型为999的消息
	while(1)
	{
		memset(&msg, 0, sizeof(msg));
		printf("msg with type999 is:");
		readLen = msgrcv(msqid, &msg, 1024, 999, IPC_NOWAIT);
		if(readLen == -1)
		{
			printf("NULL\n");
		}
		else
		{
			printf("%s\n", msg.mText);
			//如果999类型的消息为quit，则关闭消息队列
			if(memcmp("quit", msg.mText, 4) == 0)
				break;
		}
		sleep(1);
	}
	
	//关闭消息队列
	if(msgctl(msqid, IPC_RMID, NULL) == -1)
	{
		perror("msgctl(msgqid, IPC_RMID,0) failed\n");
		return -1;
	} 
	return 0;
}
