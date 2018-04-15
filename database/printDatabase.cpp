/**
* 打印sqlite3创建的数据库文件
* 使用方式，首先编译生成可执行文件
* 然后将数据库文件作为函数的第一个参数传递进去
* 将需要导出的文件名（程序新建该文件）作为第二个参数
* 导出后，建议将文件中的内容拷贝到excel中分析
*/


#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

//以十六进制方式打印字符串
static int printh(const unsigned char* buf, int len, FILE* file = stdout)
{
	int i;
	fprintf(file,"0X");
	for (i = 0; i < len; i++)
	{
		fprintf(file,"%02X", buf[i]);
		//if ((i + 2) % 20 == 0)
			//fprintf(file,"\n");
	}
	//if ((len + 2) % 20 != 0)
		//fprintf(file,"\n");
	return len;
}

//打印数据库的特定表格
static int printTable(sqlite3 *db, const char* tableName, FILE* file = stdout)
{
	int ncols = 0;
	int lines = 0;
	int i;
	int value_ret = 0;
	char sql[100] = { 0 };
	fprintf(file,"\n\n准备打印数据库的%s表\n", tableName);
	sqlite3_stmt *stmt;
	sprintf(sql, "select * from %s;", tableName);
	sqlite3_prepare(db, sql, -1, &stmt, NULL);

	//获取表的列数
	ncols = sqlite3_column_count(stmt);

	value_ret = sqlite3_step(stmt);

	//打印每一列的列名
	for (i = 0; i < ncols; i++)
	{
		fprintf(file,"%s", sqlite3_column_name(stmt, i));
		if(i!=ncols-1)
		    fprintf(file,"\t");
	}
	fprintf(file,"\n");

	//打印每一列的类型
	for (i = 0; i < ncols; i++)
	{
		switch (sqlite3_column_type(stmt, i))
		{
		case SQLITE_INTEGER:
			fprintf(file,"INT");
			break;
		case SQLITE_FLOAT:
			fprintf(file,"FLOAT");
			break;
		case SQLITE_TEXT:
			fprintf(file,"TEXT");
			break;
		case SQLITE_BLOB:
			fprintf(file,"BLOB");
			break;
		case SQLITE_NULL:
			fprintf(file,"NULL");
			break;
		default:
			fprintf(file,"\nerror in %d\n", __LINE__);
			break;
		}
		if(i!=ncols-1)
		    fprintf(file,"\t");
	}
	fprintf(file,"\n");

	//打印数据
	while (value_ret == SQLITE_ROW)
	{
		lines++;
		for (i = 0; i < ncols; i++)
		{
			//根据类型确定打印参数
			switch (sqlite3_column_type(stmt, i))
			{
			case SQLITE_INTEGER:
				fprintf(file,"%d", sqlite3_column_int(stmt, i));
				break;
			case SQLITE_FLOAT:
				fprintf(file,"%f", sqlite3_column_double(stmt, i));
				break;
			case SQLITE_TEXT:
				fprintf(file,"%s", sqlite3_column_text(stmt, i));
				break;
			case SQLITE_BLOB:
				printh((unsigned char*)sqlite3_column_blob(stmt, i), sqlite3_column_bytes(stmt, i),file);
				break;
			case SQLITE_NULL:
				fprintf(file,"NULL");
				break;
			default:
				fprintf(file,"\nerror in %d\n", __LINE__);
				break;
			}
			if(i!=ncols-1)
		        fprintf(file,"\t");
		}
		fprintf(file,"\n");
		//准备处理下一行
		value_ret = sqlite3_step(stmt);
	}
	fprintf(file,"lines = %d\n", lines);
	return lines;
}

int main(int argc, char* argv[])
{
	int value_ret = 0;
	int i;
	int tableNumber = 0;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	const char* sql;
	char* sqlErrMsg;
	vector<string> tableName;
	FILE *file = stdout;
	
	if(argc==1)
	{
	    printf("please use ./printDatabase databaseName\n");
	    return -1;
	}
	value_ret = sqlite3_open(argv[1], &db);
	if (value_ret != SQLITE_OK)
	{
		fprintf(stderr, "%d Can't open database:%s\n", __LINE__, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	if(argc==3)
	{
	    file = fopen(argv[2],"w+");	    
	}
	
	//打印数据库中所有的表
	//获取所有的表名
	sql = "select name from sqlite_master where type = 'table'";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	value_ret = sqlite3_step(stmt);
	while (value_ret == SQLITE_ROW)
	{
		tableNumber++;
		tableName.push_back(string((char*)sqlite3_column_text(stmt, 0)));
		value_ret = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	//打印所有的表
	for (i = 0; i < tableNumber; i++)
		printTable(db, tableName[i].c_str(),file);
	
	
	if(argc==3)
	{
	    fclose(file);  
	}
	sqlite3_close(db);
	return 0;
}













