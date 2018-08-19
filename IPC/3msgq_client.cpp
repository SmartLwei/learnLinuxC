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
#include <string.h>

//会话预定的唯一的key
#define KEY "/etc/passwd"

//会话用的数据结构
typedef struct Msg
{
	long mType;
	char mText[1024];
}Msg;

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("use ./process sendOrReceive msgType [msg]\n");
		printf("for example\n");
		printf("./process 0 999 means try to receive 999Type msg\n");
		printf("./process 1 888 HelloWorld means send 888Type msg and text is HelloWorld\n");
		return -1;
	}
	
	int service = atoi(argv[1]);
	if(service != 0 && service != 1)
	{
		printf("the first parameter must be 0 or 1");
		return -1;
	}

	int msqid;
	key_t key;
	Msg msg;

	//打开消息队列
	if((key = ftok(KEY,'z')) < 0)
	{
		perror("ftok error");
		return -1;
	}
	printf("key = %d\n",key);
	if((msqid = msgget(key, 0777)) == -1)
	{
		perror("msgget failed\n");
		exit(1);
	}
	
	
	//发送或者接收消息
	int len = 0;
	int temp = atoi(argv[2]);
	switch(service)
	{
	case 0:
		//接收消息
		memset(&msg, 0, sizeof(msg));
		printf("I'm waiting for %dType msg\n", temp);
		len = msgrcv(msqid, &msg, 1024, temp, 0);
		printf("recvLen = %d\n", len);
		if(len > 0)
		{
			printf("MsgType = %ld\n", msg.mType);
			printf("recvMsg: %s\n", msg.mText);
		}
	break;
	case 1:
		//发送消息
		memset(&msg, 0, sizeof(msg));
		msg.mType = atoi(argv[2]);
		memcpy(msg.mText, argv[3], strlen(argv[3]));
		len = msgsnd(msqid, &msg, strlen(argv[3]), 0);
		printf("msgsnd return Value = %d\n", len);
	break;
	default:
		printf("unknown serveice type\n");
		return -1;
	
	}
	return 0;
}
