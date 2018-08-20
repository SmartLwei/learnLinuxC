/*
FD_ZERO(fd_set* fds);			//将fds清零
FD_SET(int fd, fd_set* fds);	//设置fd在fds中的有效性
FD_ISSET(int fd, fd_set* fds);	//检查fd在fds中是否有效
FD_CLR(int fd, fd_set* fds);	//取消fd在fds中的有效性

int select(int nfds, fd_set* readFds, fd_set* writeFds, fd_set* exceptfds, struct timeval* timeout);
其中，nfds为最大的fd值+1
如果timeout之前三个Fds中的任何fd有响应，则返回非0值
如果timeout时间到了仍然没有fd有响应，则返回0
该函数会更改三个Fds及timeout的值，因此每次使用前都需要重新设置这些参数
*/

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <error.h>

int main()
{
	int keyboard;
	int ret, i;
	char c;
	fd_set readfd;
	struct timeval timeout;
	///dev/tty表示当前终端
	keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK);
	printf("keyboard = %d\n", keyboard);
	assert(keyboard > 0);
	while(1)
	{
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		FD_ZERO(&readfd);
		FD_SET(keyboard, &readfd);
		printf("please input a char\n");
		//select的第一个参数为 maxFd+1
		ret = select(keyboard+1, &readfd, NULL, NULL, &timeout);
		printf("select return value is %d\n", ret);
		if(FD_ISSET(keyboard, &readfd))
		{
			i = read(keyboard, &c, 1);
			if(c == '\n')
				continue;
			printf("The input is %c\n",c);
			if(c == 'q')
				break;
		}
	}
	return 0;
}


