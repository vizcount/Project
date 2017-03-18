#include "stdafx.h"
#include "SQLiteOperate.h"


SQLiteOperate::SQLiteOperate(void)
{
}


SQLiteOperate::~SQLiteOperate(void)
{
}

/*
	@brief 初始化数据库
	@return true成功初始化
	*/
void SQLiteOperate::InitDb()
{
	memset(szDbPath, 0, sizeof(szDbPath));
	GetModuleFileName(NULL, szDbPath, MAX_PATH);

	PathAppend(szDbPath, _T("..\\..\\"));
	PathAppend(szDbPath, _T("db\\tbData.db"));

	char *pBuf = WideToMulti(szDbPath, CP_UTF8);

	if (pBuf != NULL)
	{
		sqlite.Open(pBuf);


		if (!IsExistTable("TBRecordData"))          //初始化表格
		{
			InitTables();
		}

		delete []pBuf;
	}

}

/*
	@brief  创建数据库表格
	*/
void SQLiteOperate::InitTables()
{
	try
	{
		std::string  strSql;

		strSql = "create table TBRecordData(id INTEGER PRIMARY KEY AUTOINCREMENT \
					, tbdate VARCHAR(20)\
					, tbName VARCHAR(20)\
					, tbTel  VARCHAR(20)\
					, tbWupin VARCHAR(256)\
					, tbJinE DOUBLE\
					, tbYiFuKuan DOUBLE\
					, tbWuliuNum VARCHAR(20)\
					, tbOrderNum VARCHAR(40)\
					, tbNumber int\
					, tbMemo  VARCHAR(100000));";

		sqlite.DirectStatement(strSql);
	}
	catch (...)
	{}
}

/*
	@brief  判断表是否存在
	@param  pTableName 表名
	@return true为存在
	*/
bool SQLiteOperate::IsExistTable( const char *pTableName )
{
	bool bRes = false;

	try
	{
		std::string  strSql;
		ResultTable  resTable;

		strSql = "SELECT * FROM sqlite_master where type='table' and name='";
		strSql.append(pTableName);
		strSql.append("'");

		SelectStmt(strSql, resTable);

		if (resTable.next() != NULL)
		{
			bRes = true;
		}

	}
	catch (...)
	{

	}


	return bRes;
}


/*
	@brief 查询结果函数
	@param  stmt  查询sql语句
	@param  [in/out]  数据返回结果
	@return  true为执行成功
	*/
bool SQLiteOperate::SelectStmt( std::string const& stmt, ResultTable& res )
{
	CLocalLock local(&m_Lock);

	return sqlite.SelectStmt(stmt, res);
}

/*
	@brief 直接执行sql语句函数
	@param  stmt  sql语句
	@return  true为执行成功
	*/
bool SQLiteOperate::DirectStatement( std::string const& stmt )
{
	CLocalLock local(&m_Lock);

	return sqlite.DirectStatement(stmt);
}

/*
	@brief  删除用户信息
	*/
void SQLiteOperate::DeleteInfo(int nsCode)
{
	try
	{
		CStringA  strSql;

		strSql.Format("delete from TBRecordData where id = %d",nsCode);
		
		DirectStatement(strSql.GetString());

	}
	catch (...)
	{
		//g_log.Trace(LOGL_TOP, LOGT_PROMPT, __TFILE__, __LINE__, _T("删除用户信息出错"));
	}
}

/*
	@brief  宽字符转多字节
	@param  要转化的宽字符串
	@return 返回多字节
	*/
char * SQLiteOperate::WideToMulti( const wchar_t *pWide, DWORD dwCode /*= CP_ACP*/ )
{
	char *pChar = NULL;
	int  iWlen = 0;

	if (pWide == NULL
		|| (iWlen = wcslen(pWide)) == 0)
	{
		return pChar;
	}

	int iLen = WideCharToMultiByte( dwCode, 0, pWide, iWlen, NULL, NULL, NULL, NULL );
	if (iLen > 0)
	{
		pChar = new char[iLen + 1];
		if (pChar != NULL)
		{
			memset(pChar, 0, iLen+1);
			WideCharToMultiByte( dwCode, 0, pWide, iWlen, pChar, iLen, NULL, NULL );
		}
	}

	return pChar;
}


/*
	@brief  多字节转宽字符
	@param  要转化的多字符串
	@return 返回宽字节
	*/
wchar_t * SQLiteOperate::MultitoWide( const char *pMulti, DWORD dwCode /*= CP_ACP*/ )
{
	wchar_t *pWide = NULL;
	int iAlen = 0;

	if (pMulti == NULL
		|| (iAlen = strlen(pMulti)) == 0)
	{
		return pWide;
	}

	int iLen = MultiByteToWideChar( dwCode, 0, pMulti, iAlen, NULL, NULL );
	if (iLen > 0)
	{
		pWide = new wchar_t[iLen + 1];
		if (pWide != NULL)
		{
			memset(pWide, 0, (iLen+1)*sizeof(wchar_t));
			MultiByteToWideChar( dwCode, 0, pMulti, iAlen, pWide, iLen );
		}

	}

	return pWide;
}


/*
@brief 保存变量数据
@param varinfo 变量信息结构
*/
bool SQLiteOperate::SaveInfo(TBRecordInfo const& TBInfo)
{
	try
	{
		bool bRes =false;
		CStringA  strSql;

		strSql.Format("insert into TBRecordData(tbdate,tbName,tbTel,tbWupin,tbJinE,tbYiFuKuan,tbWuliuNum,tbOrderNum,tbNumber,tbMemo) \
					  values('%s','%s','%s','%s',%f,%f,'%s','%s',%d,'%s')"
				, TBInfo.tbdate
				, TBInfo.tbName
				, TBInfo.tbTel
				, TBInfo.tbWupin
				, TBInfo.tbJinE
				, TBInfo.tbYiFuKuan
				, TBInfo.tbWuliuNum
				, TBInfo.tbOrderNum
				, TBInfo.tbNumber
				, TBInfo.tbMemo
				);
		return DirectStatement(strSql.GetString());	
	}
	catch (...)
	{
		return false;
	}
}

int SQLiteOperate::GetID()
{
	CStringA  strSql;
	ResultTable resTable;
	ResultRecord *resRecord = NULL;

	strSql.Format("SELECT id from TBRecordData order by id desc");

	SelectStmt(strSql.GetString(), resTable);
	if ((resRecord = resTable.next())!= NULL)
	{
		return atoi(resRecord->fields_[0].c_str());
	}

	return 0;
}


/*
@brief 获取变量数据
@param varinfo 变量数据
*/
std::map<int,TBRecordInfo> SQLiteOperate::GetVarInfo()
{
	try
	{
		CStringA  strSql;
		ResultTable resTable;
		ResultRecord *resRecord = NULL;
		std::map<int ,TBRecordInfo> mapVar;
		mapVar.clear();
		strSql.Format("SELECT id,tbdate,tbName,tbTel,tbWupin,tbJinE,tbYiFuKuan,tbWuliuNum,tbOrderNum,tbNumber,tbMemo FROM TBRecordData order by id");

		SelectStmt(strSql.GetString(), resTable);
		while((resRecord = resTable.next()) != NULL)
		{
			TBRecordInfo tInfo;
			tInfo.id = atoi(resRecord->fields_[0].c_str());
			strncpy(tInfo.tbdate, resRecord->fields_[1].c_str(), MIN_LENGTH - 1);
			strncpy(tInfo.tbName, resRecord->fields_[2].c_str(), MIN_LENGTH - 1);
			strncpy(tInfo.tbTel, resRecord->fields_[3].c_str(), MIN_LENGTH - 1);
			strncpy(tInfo.tbWupin, resRecord->fields_[4].c_str(), MAX_USER_PATH - 1);
			tInfo.tbJinE = atof(resRecord->fields_[5].c_str());
			tInfo.tbYiFuKuan = atof(resRecord->fields_[6].c_str());
			strncpy(tInfo.tbWuliuNum, resRecord->fields_[7].c_str(), MIN_LENGTH - 1);
			strncpy(tInfo.tbOrderNum, resRecord->fields_[8].c_str(), MIN_LENGTH*2 - 1);
			tInfo.tbNumber = atoi(resRecord->fields_[9].c_str());
			strncpy(tInfo.tbMemo, resRecord->fields_[10].c_str(), 100000 - 1);

			mapVar[tInfo.id]=tInfo;
		}
		return mapVar;
	}
	catch (...)
	{
		
	}

}

bool SQLiteOperate::UpdateInfo(TBRecordInfo const& TBInfo)
{
	try
	{
		bool bRes = false;
		CStringA  strSql;

		strSql.Format("update TBRecordData set tbdate='%s',tbName='%s',tbTel='%s',tbWupin='%s',tbJinE=%f,tbYiFuKuan=%f,tbWuliuNum='%s',\
					  tbOrderNum='%s',tbNumber=%d,tbMemo='%s' where id =%d"
															  , TBInfo.tbdate
															  , TBInfo.tbName
															  , TBInfo.tbTel
															  , TBInfo.tbWupin
															  , TBInfo.tbJinE
															  , TBInfo.tbYiFuKuan
															  , TBInfo.tbWuliuNum
															  , TBInfo.tbOrderNum
															  , TBInfo.tbNumber
															  , TBInfo.tbMemo
															  , TBInfo.id
															  );
		bRes = DirectStatement(strSql.GetString());

		return bRes;
	}
	catch (...)
	{
		return false;
	}
}

