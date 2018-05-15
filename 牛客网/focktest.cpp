#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

int main(void)
{
	cout << "start" << endl; 
	int i;
	for(i=0; i<2; i++)
	{
		fork();
		cout << ("-");
		//printf("-");效果是一样的
	}
	
	sleep(1);
	cout << endl;
	sleep(1);
	return 0;
}
/**
* 直观感觉是输出6个 -
* 而实际上一共输出了8个 -
* sleep()可以查看输出的细节，cout并没有立即输出 - ，而是在 endl那里输出的-
* 如果没有endl，则在程序退出时才输出 - 
          
                            进程1
                         /        \
                     进程1         进程2                                                       
                     /   \         /   \                                    
                 进程1  进程3   进程2   进程4                                                    
        进程1在分化出进程3时，因为cout或者printf中没有endl或者\n 
        因此 cout << ("-"); 语句实际上并没有输出任何内容到stdout
        而是将-写入输出缓冲区
        当创建进程3时，进程1中的缓冲区也被进程3给继承了
        同理，进程4也继承了进程2中的一个 -
        因此在程序结束时，或者输出换行符时，输出缓冲区的内容才被打印到stdout
        结构就是进程1-进程4各打印两个 - 
        
        
                                        

*/


























