#include <stdio.h>

// FILE *fopen( const char * filename, const char * mode );
// int


// r 读， w 写（如果不存在则创建），a 追加写（如果不存在则创建）
// r+ 读写， w+ 读写(如果不存在则创建), a+ 追加写
// b 标识二进制
// r+ ,w+ 的区别：
// r+从头开始读写，写入的部分覆盖原来的部分，为覆盖的部分保留
// w+删除内容后从头开始写
// 例如 
// "r", "w", "a", "r+", "w+", "rb", "wb", "ab", "rb+", "r+b", "wb+", "w+b", "ab+", "a+b"


int main(int argc, char* argv[])
{
    FILE *fp = NULL;
    char buff[255];
 
    fp = fopen("testIn.txt", "r");
    fscanf(fp, "%s", buff);
    printf("1: %s\n", buff );
 
    fgets(buff, 255, (FILE*)fp);
    printf("2: %s\n", buff );
   
    fgets(buff, 255, (FILE*)fp);
    printf("3: %s\n", buff );
    fclose(fp);
}
