/*
C风格的文件读写
FILE *fopen( const char * filename, const char * mode );

 r 读， w 写（如果不存在则创建），a 追加写（如果不存在则创建）
 r+ 读写， w+ 读写(如果不存在则创建), a+ 追加写
 b 标识二进制
 r+ ,w+ 的区别：
 r+从头开始读写，写入的部分覆盖原来的部分，未覆盖的部分保留
 w+删除内容后从头开始写
 例如 
 "r", "w", "a", "r+", "w+", "rb", "wb", "ab", "rb+", "r+b", "wb+", "w+b", "ab+", "a+b"

*/



#include <stdio.h>
#include <unistd.h>

//创建文件
void CreateFile()
{
	unlink("test.txt");
	FILE *fp = fopen("test.txt", "w");
	fprintf(fp, "abcdefg\nhijklmn\nopq rst\nuvw xyz\n");
	fclose(fp);
}

int main(int argc, char* argv[])
{
	CreateFile();
    FILE *fp = NULL;
    char buff[255];
 
    fp = fopen("test.txt", "r");
    
    //fscanf()会将换行符留在流中
    fscanf(fp, "%s", buff);
    printf("1: %s\n", buff );
 
 	//fgets()读取到换行符为止，并且将换行符读取到buf中
    fgets(buff, 255, fp);
    printf("2: %s\n", buff );
   
    fgets(buff, 255, fp);
    printf("3: %s\n", buff );
    
    fgets(buff, 255, fp);
    printf("4: %s\n", buff );
   
    fgets(buff, 255, fp);
    printf("5: %s\n", buff );
    
    getchar();
    
    unlink("test.txt");
    fclose(fp);
}
