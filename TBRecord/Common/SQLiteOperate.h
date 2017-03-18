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
	@brief ��ʼ�����ݿ�
	*/
	void InitDb();

	/*
	@brief  �������ݿ���
	*/
	void  InitTables();

	/*
	@brief  �жϱ��Ƿ����
	@param  pTableName ����
	@return trueΪ����
	*/
	bool  IsExistTable(const char *pTableName);


	/*
	@brief ��ѯ�������
	@param  stmt  ��ѯsql���
	@param  [in/out]  ���ݷ��ؽ��
	@return  trueΪִ�гɹ�
	*/
    bool SelectStmt(std::string const& stmt, ResultTable& res);

	/*
	@brief ֱ��ִ��sql��亯��
	@param  stmt  sql���
	@return  trueΪִ�гɹ�
	*/
    bool DirectStatement(std::string const& stmt);


	/*
	@brief �����������
	@param TBRecordInfo ��¼��Ϣ�ṹ
	*/
	bool SaveInfo(TBRecordInfo const& TBInfo);

	/*
	@brief �޸ı�������
	@param TBRecordInfo ��¼��Ϣ�ṹ
	*/
	bool UpdateInfo(TBRecordInfo const& TBInfo);

	/*
	@brief ��ȡ����id
	*/
	int GetID();

	/*
	@brief ��ȡ��������
	@param TBRecordInfo ��������
	*/
	std::map<int,TBRecordInfo> GetVarInfo();

	/*
	@brief  ɾ���û���Ϣ
	@param  Flag  ɾ���� 0: lanmuData 1:varData 
	*/
	void DeleteInfo(int nsCode);

	/*
	@brief  ���ַ�ת���ֽ�
	@param  Ҫת���Ŀ��ַ���
	@return ���ض��ֽ�
	*/
	char *WideToMulti(const wchar_t *pWide, DWORD dwCode = CP_ACP);

	/*
	@brief  ���ֽ�ת���ַ�
	@param  Ҫת���Ķ��ַ���
	@return ���ؿ��ֽ�
	*/
	wchar_t *MultitoWide(const char *pMulti, DWORD dwCode = CP_ACP);



private:
	SQLiteWrapper  sqlite;
	CLock   m_Lock;

	TCHAR   szDbPath[MAX_PATH];   //���ݿ�·��
};

