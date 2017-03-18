#pragma once
#include "ExtractData.h"
#include "SQLiteWrapper.h"

#include "Lock.h"
#include <map>

class SQLiteOperate
{
public:
	SQLiteOperate(void);
	~SQLiteOperate(void);


	/*
	@brief 初始化数据库
	*/
	void InitDb();

	/*
	@brief  创建数据库表格
	*/
	void  InitTables();

	/*
	@brief  判断表是否存在
	@param  pTableName 表名
	@return true为存在
	*/
	bool  IsExistTable(const char *pTableName);


	/*
	@brief 查询结果函数
	@param  stmt  查询sql语句
	@param  [in/out]  数据返回结果
	@return  true为执行成功
	*/
    bool SelectStmt(std::string const& stmt, ResultTable& res);

	/*
	@brief 直接执行sql语句函数
	@param  stmt  sql语句
	@return  true为执行成功
	*/
    bool DirectStatement(std::string const& stmt);


	/*
	@brief 保存变量数据
	@param TBRecordInfo 记录信息结构
	*/
	bool SaveInfo(TBRecordInfo const& TBInfo);

	/*
	@brief 修改变量数据
	@param TBRecordInfo 记录信息结构
	*/
	bool UpdateInfo(TBRecordInfo const& TBInfo);

	/*
	@brief 获取最大的id
	*/
	int GetID();

	/*
	@brief 获取变量数据
	@param TBRecordInfo 变量数据
	*/
	std::map<int,TBRecordInfo> GetVarInfo();

	/*
	@brief  删除用户信息
	@param  Flag  删除表 0: lanmuData 1:varData 
	*/
	void DeleteInfo(int nsCode);

	/*
	@brief  宽字符转多字节
	@param  要转化的宽字符串
	@return 返回多字节
	*/
	char *WideToMulti(const wchar_t *pWide, DWORD dwCode = CP_ACP);

	/*
	@brief  多字节转宽字符
	@param  要转化的多字符串
	@return 返回宽字节
	*/
	wchar_t *MultitoWide(const char *pMulti, DWORD dwCode = CP_ACP);



private:
	SQLiteWrapper  sqlite;
	CLock   m_Lock;

	TCHAR   szDbPath[MAX_PATH];   //数据库路径
};

