/**
* linux下时间获取函数
* linux下时间保存方式有两种
* 1. 保存从1970年到现在过了多少秒
* 2. 用结构体分别保存年月日时分秒
struct tm
{
  int tm_sec;			// Seconds.	[0-60] (1 leap second) 
  int tm_min;			// Minutes.	[0-59] 
  int tm_hour;			// Hours.	[0-23] 
  int tm_mday;			// Day.		[1-31] 
  int tm_mon;			// Month.	[0-11] 
  int tm_year;			// Year	- 1900.  
  int tm_wday;			// Day of week.	[0-6] 
  int tm_yday;			// Days in year.[0-365]	
  int tm_isdst;			// DST.		[-1/0/1]

# ifdef	__USE_MISC
  long int tm_gmtoff;		 //Seconds east of UTC.  
  const char *tm_zone;		 //Timezone abbreviation.  
# else
  long int __tm_gmtoff;		 //Seconds east of UTC.  
  const char *__tm_zone;	 //Timezone abbreviation.  
# endif
};



为了获得更加精确的时间信息，例如精确到微秒，纳秒
保存秒数和微秒数
struct timeval
{
	__time_t tv_sec;		// Seconds.  
	__suseconds_t tv_usec;	// Microseconds.  
};
__time_t和__suseconds_t 在32位linux下都是long int的宏定义









*/

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
int main(int argc, char* argv[])
{
	/**
	对于32位linux系统，time_t本质上是 long int
	time_t用于保存从1970年1月1日0时0分起到现在的秒数
	因为只有32位，因此最多只能统计到2038年01月19日03时14分07秒
	*/
	time_t now;
	char* dt;
	
	/** 获取当前统计的秒数,以及打印时间 */
	now = time(0);
	//time(&now); //该语句与now = time(0) 效果一模一样
	//事实上 该函数的原型为 time_t time(&time_t *t) ,因此上面两种方法本质上是一样的
	cout << "time(0) = " << now << endl;
	cout << "ctime(&now) = " << ctime(&now) << endl;
	cout << "asctime(gmtime(&now)) = " << asctime(gmtime(&now)) << endl;
	cout << "asctime(localtime(&now)) = " << asctime(localtime(&now)) << endl;
	cout << "从结果来看，ctime(&now)得到的是本地时间的字符串;" << endl;
	cout << "而asctime(gmtime(&now))得到的是UTC时间字符串" << endl;
	cout << endl << endl;
	cout << "now = " << now << endl;
	cout << "mktime(localtime(&now)) = " << mktime(localtime(&now)) << endl;
	cout << "mktime(gmtime(&now)) = " << mktime(gmtime(&now)) << endl;
	cout << "asctime(gmtime(mktime(gmtime(&now)))) = ";
	cout << asctime(gmtime(mktime(gmtime(&now)))) << endl;
	//cout << "从结果看
	/** 
	asctime(gmtime(&now))的本质是
		gmtime()先把now转换为struct tm结构，得到的时间是一个没有经过时区转换的UTC时间
		asctime()把sttuct tm结构转换为人类可读的字符串
	localtime()与gmtime功能相同，只是进行了时区转换
	mktime()将struct tm结构的时间转换为time_t的时间，注意时区的影响，
	应该输入localtime的结构体
	*/
	
	
	/** 上面的函数都是在以秒为单位下的时间，有时我们需要更加精确的时间，如微秒，纳秒 */
	cout << endl << endl;
	
	
	
	
	


	return 0;
	
}


/** 网上搜集的其他与时间有关的资料

time_t time(time_t *time);
该函数返回系统的当前日历时间，自 1970 年 1 月 1 日以来经过的秒数。


char ctime(const time_t time);
该返回一个表示当地时间的字符串指针，字符串形式为
day month date hours:minutes:seconds year\n\0
周几 几月 几日 时：分：秒 年


struct tm localtime(const time_t time);
该函数返回一个指向表示本地时间的 tm 结构的指针。


clock_t clock(void);
该函数返回程序执行起（一般为程序的开头），处理器时钟所使用的时间。


char * asctime ( const struct tm * time );
该函数返回一个指向字符串的指针，字符串包含了 time 所指向结构中存储的信息，返回形式为
day month date hours:minutes:seconds year\n\0。
周几 几月 几日 时：分：秒 年


struct tm gmtime(const time_t time);
该函数返回 tm 结构的时间，用协调世界时（UTC）也被称为格林尼治标准时间（GMT）表示。


time_t mktime(struct tm *time);
返回time对应的距1970年的秒数


double difftime ( time_t time2, time_t time1 );
该函数返回 time1 和 time2 之间相差的秒数。


int gettimeofday(struct timeval *tv, struct timezone *tz);
返回当前距离1970年的秒数和微妙数，后面的tz是时区，一般不用


strftime() 函数将时间格式化
size_t strftime(
     char *strDest,
     size_t maxsize,
     const char *format,
     const struct tm *timeptr
);
我们可以根据format指向字符串中格式命令把timeptr中保存的时间信息放在strDest指向的字符串中，
最多向strDest中存放maxsize个字符。
该函数返回向strDest指向的字符串中放置的字符数。
函数strftime()的操作有些类似于sprintf()：
识别以百分号(%)开始的格式命令集合，格式化输出结果放在一个字符串中。
格式串中的其他字符原样放进串中。格式命令列在下面，它们是区分大小写的。
%a 星期几的简写
%A 星期几的全称
%b 月份的简写
%B 月份的全称
%c 标准的日期的时间串
%C 年份的后两位数字
%d 十进制表示的每月的第几天
%D 月/天/年
%e 在两字符域中，十进制表示的每月的第几天
%F 年-月-日
%g 年份的后两位数字，使用基于周的年
%G 年分，使用基于周的年
%h 简写的月份名
%H 24小时制的小时
%I 12小时制的小时
%j 十进制表示的每年的第几天
%m 十进制表示的月份
%M 十时制表示的分钟数
%n 新行符
%p 本地的AM或PM的等价显示
%r 12小时的时间
%R 显示小时和分钟：hh:mm
%S 十进制的秒数
%t 水平制表符
%T 显示时分秒：hh:mm:ss
%u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
%U 第年的第几周，把星期日做为第一天（值从0到53）
%V 每年的第几周，使用基于周的年
%w 十进制表示的星期几（值从0到6，星期天为0）
%W 每年的第几周，把星期一做为第一天（值从0到53）
%x 标准的日期串
%X 标准的时间串
%y 不带世纪的十进制年份（值从0到99）
%Y 带世纪部分的十制年份
%z，%Z 时区名称，如果不能得到时区名称则返回空字符。
%% 百分号

*/

































