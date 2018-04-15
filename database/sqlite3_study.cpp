#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

//以十六进制方式打印字符串
static int printh(const unsigned char* buf, int len)
{
	int i;
	printf("0X");
	for (i = 0; i < len; i++)
	{
		printf("%02X", buf[i]);
		//if ((i + 2) % 20 == 0)
			//printf("\n");
	}
	//if ((len + 2) % 20 != 0)
		//printf("\n");
	return len;
}

//sqlite3_exec()回调函数
//打印sqlite3_exec()的返回结果
static int callback(void* in, int argc, char** argv, char** ColName)
{
	int i;
	printf("enter callback function\n");
	printf("sqlite3_exec return data is\n");
	for (i = 0; i < argc; i++)
	{
		printf("%s\t", ColName[i]);
	}
	printf("\n");
	for (i = 0; i < argc; i++)
	{
		printf("%s\t", argv[i]);
	}
	printf("\n");
	return 0;
}

//打印数据库的特定表格
static int printTable(sqlite3 *db, const char* tableName, FILE* file = stdout)
{
	int ncols = 0;
	int lines = 0;
	int i;
	int value_ret = 0;
	char sql[100] = { 0 };
	printf("准备打印数据库的%s表\n", tableName);
	sqlite3_stmt *stmt;
	sprintf(sql, "select * from %s;", tableName);
	sqlite3_prepare(db, sql, -1, &stmt, NULL);

	//获取表的列数
	ncols = sqlite3_column_count(stmt);

	value_ret = sqlite3_step(stmt);

	//打印每一列的列名
	for (i = 0; i < ncols; i++)
	{
		printf("%s\t", sqlite3_column_name(stmt, i));
	}
	printf("\b\n");

	//打印每一列的类型
	for (i = 0; i < ncols; i++)
	{
		switch (sqlite3_column_type(stmt, i))
		{
		case SQLITE_INTEGER:
			printf("INT\t");
			break;
		case SQLITE_FLOAT:
			printf("FLOAT\t");
			break;
		case SQLITE_TEXT:
			printf("TEXT\t");
			break;
		case SQLITE_BLOB:
			printf("BLOB\t");
			break;
		case SQLITE_NULL:
			printf("NULL\t");
			break;
		default:
			printf("\nerror in %d\n", __LINE__);
			break;
		}
	}
	printf("\b\n");

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
				printf("%d\t", sqlite3_column_int(stmt, i));
				break;
			case SQLITE_FLOAT:
				printf("%f\t", sqlite3_column_double(stmt, i));
				break;
			case SQLITE_TEXT:
				printf("%s\t", sqlite3_column_text(stmt, i));
				break;
			case SQLITE_BLOB:
				printh((unsigned char*)sqlite3_column_blob(stmt, i), sqlite3_column_bytes(stmt, i));
				break;
			case SQLITE_NULL:
				printf("NULL\t");
				break;
			default:
				printf("\nerror in %d\n", __LINE__);
				break;
			}
		}
		printf("\b\n");
		//准备处理下一行
		value_ret = sqlite3_step(stmt);
	}
	printf("lines = %d\n", lines);
	return lines;
}

//从文件数据库拷贝到内存数据库（加快速度）,ifSave = 0
//或者从内存数据库拷贝到文件数据库（备份）, ifSave = 1
static int db_backup(sqlite3* memorydb, int ifSave)
{
	int value_ret;
	sqlite3 *filedb;
	sqlite3_backup *backupdb;
	sqlite3 *dbFrom;
	sqlite3 *dbTo;

	//打开一个文件数据库
	value_ret = sqlite3_open("testDatabase.db", &filedb);
	if (value_ret != SQLITE_OK)
	{
		fprintf(stderr, "%d Can't open database:%s\n", __LINE__, sqlite3_errmsg(filedb));
		sqlite3_close(filedb);
		return -1;
	}
	dbFrom = ifSave ? memorydb : filedb;
	dbTo = ifSave ? filedb : memorydb;

	backupdb = sqlite3_backup_init(dbTo, "main", dbFrom, "main");
	if (backupdb)
	{
		sqlite3_backup_step(backupdb, -1);
		sqlite3_backup_finish(backupdb);
		sqlite3_close(filedb);
		return 0;
	}
	else
	{
		sqlite3_close(filedb); 
		sqlite3_backup_finish(backupdb);
		return -1;
	}
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
	//测试用的blob向量
	char blobValue[20] = { 0x31,0x32,0x33,0x34,0x35,\
		0x36,0x37,0x38,0x39,0x3a,\
		0x3b,0x3b,0x3d,0x3e,0x3f,\
		0x40,0x41,0x42,0x43,0x44 };
	vector<string> tableName;


	//打开或者新建数据库
	value_ret = sqlite3_open(":memory:", &db);
	if (value_ret != SQLITE_OK)
	{
		fprintf(stderr, "%d Can't open database:%s\n", __LINE__, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}


	//创建表
	sql = "create table testTable("\
		"ID INT PRIMAEY KEY,"\
		"DOUBLE_VALUE DOUBLE,"\
		"TEXT_VALUE TEXT,"\
		"BLOB_VALUE BLOB);";
	value_ret = sqlite3_exec(db, sql, NULL, NULL, &sqlErrMsg);
	if (value_ret != SQLITE_OK)
	{
		fprintf(stderr, "%d SQL error: %s\n", __LINE__, sqlErrMsg);
		sqlite3_free(sqlErrMsg);
	}


	//插入数据
	sql = "insert into testTable values("\
		"1,1.01,'string1',?);";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_blob(stmt, 1, (void *)blobValue, 20, SQLITE_STATIC);
	sqlite3_step(stmt);
	//sqlite3_reset(stmt)也可以
	sqlite3_finalize(stmt);
	//插入数据
	sql = "insert into testTable values("\
		"1,2.01,'string2',?);";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_blob(stmt, 1, (void *)&blobValue[5], 10, SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_reset(stmt);
	//插入数据
	sql = "insert into testTable values("\
		"1,3.01,'string3',?);";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	//SQLITE_STATIC和SQLITE_TRANSIENT都可以
	sqlite3_bind_blob(stmt, 1, (void *)&blobValue[10], 5, SQLITE_TRANSIENT);
	sqlite3_step(stmt);
	sqlite3_reset(stmt);
	//插入数据
	sql = "insert into testTable values("\
		"1,4.01,'string4',?);";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_blob(stmt, 1, (void *)&blobValue[15], 5, SQLITE_TRANSIENT);
	sqlite3_step(stmt);
	sqlite3_reset(stmt);
	//插入数据
	sql = "insert into testTable values("\
		"1,5.01,'string5',?);";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_blob(stmt, 1, (void *)blobValue, 4, SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	//插入数据
	sql = "insert into testTable values("\
		"1,NULL,NULL,?);";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_blob(stmt, 1, (void *)blobValue, 6, SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);




	//获取表中的数据
	sql = "select * from testTable;";
	value_ret = sqlite3_exec(db, sql, callback, NULL, &sqlErrMsg);
	if (value_ret != SQLITE_OK)
	{
		fprintf(stderr, "%d SQL error: %s\n", __LINE__, sqlErrMsg);
		sqlite3_free(sqlErrMsg);
	}


	//通过自定义函数打印表格
	printf("\n\n添加数据后，表的内容为\n");
	printTable(db, "testTable");


	//更新表格中的内容
	sql = "UPDATE testTable set ID = 2 Where TEXT_VALUE = 'string2';"\
		"UPDATE testTable set ID = 3 Where TEXT_VALUE = 'string3';"\
		"UPDATE testTable set ID = 4 Where DOUBLE_VALUE = 4.01;"\
		"UPDATE testTable set ID = 6,DOUBLE_Value=6.01, TEXT_value = 'string6' Where TEXT_VALUE IS NULL;";
	value_ret = sqlite3_exec(db, sql, callback, NULL, &sqlErrMsg);
	if (value_ret != SQLITE_OK)
	{
		fprintf(stderr, "%d SQL error: %s\n", __LINE__, sqlErrMsg);
		sqlite3_free(sqlErrMsg);
	}
	//通过blob类型值进行查找并更新表
	sql = "UPDATE testTable set ID = 5 Where BLOB_VALUE = ?;";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_blob(stmt, 1, (void*)blobValue, 4, SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	//通过自定义函数打印表格
	printf("\n\n更新数据后，表的内容为\n");
	printTable(db, "testTable");



	//删除一行
	sql = "delete from testTable where ID = 3";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	//删除一行
	//sql = "delete from testTable where BLOB_VALUE = ?;";
	//sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	//sqlite3_bind_blob(stmt, 1, (void*)blobValue, 4, SQLITE_STATIC);
	//sqlite3_step(stmt);
	//sqlite3_finalize(stmt);


	//通过自定义函数打印表格
	printTable(db, "testTable");


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
	printf("删除几行后,数据库为\n");
	printf("打印数据库中所有的表\n");
	for (i = 0; i < tableNumber; i++)
		printTable(db, tableName[i].c_str());


	//删除表格
	//sql = "drop table testTable;";
	//value_ret = sqlite3_exec(db, sql, callback, NULL, &sqlErrMsg);
	//if (value_ret != SQLITE_OK)
	//{
	//	fprintf(stderr, "%d SQL error: %s\n", __LINE__, sqlErrMsg);
	//	sqlite3_free(sqlErrMsg);
	//}

	//备份数据库
	db_backup(db, 1);

	sqlite3_close(db);
	getchar();
	return 0;
}
