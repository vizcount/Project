/* 
   SQLiteWrapper.h

   Copyright (C) 2004 Ren�� Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren�� Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#ifndef SQLITE_WRAPPER_H__
#define SQLITE_WRAPPER_H__

#include <string>
#include <vector>

#include "sqlite/sqlite3.h"

class SQLiteStatement {
  private:
    // SQLiteStatement's ctor only to be called by SQLiteWrapper
    friend class SQLiteWrapper;
    SQLiteStatement(std::string const& statement, sqlite3* db);

  public:
    SQLiteStatement();

    enum dataType {
      INT = SQLITE_INTEGER,
      FLT = SQLITE_FLOAT  ,
      TXT = SQLITE_TEXT   ,
      BLB = SQLITE_BLOB   ,
      NUL = SQLITE_NULL   ,
    };

	//ȡ����������ֵ
    dataType DataType(int pos_zero_indexed);


	//����Ϊȡ�ú�������ֵ
    int         ValueInt   (int pos_zero_indexed);
    std::string ValueString(int pos_zero_indexed);

//    SQLiteStatement(const SQLiteStatement&);
   ~SQLiteStatement();

    //SQLiteStatement& operator=(SQLiteStatement const&);

    //���º���Ϊ�󶨸������͵�ֵ
    bool Bind    (int pos_zero_indexed, std::string const& value);
    bool Bind    (int pos_zero_indexed, double             value);
    bool Bind    (int pos_zero_indexed, int                value);
    bool BindNull(int pos_zero_indexed);



	//ִ��sql���
    bool Execute();

	//�������
    bool NextRow();

    /*   Call Reset if not depending on the NextRow cleaning up.
         For example for select count(*) statements*/
    bool Reset();
    bool RestartSelect();

  private:
    //void DecreaseRefCounter();

    //int* ref_counter_; // not yet used...
    sqlite3_stmt* stmt_;
};

//����һ��������
class ResultRecord {
public:
	std::vector<std::string> fields_;
};

//�����
class ResultTable {
	friend class SQLiteWrapper;
public:
	ResultTable() : ptr_cur_record_(0) {}

	std::vector<ResultRecord> records_;

	ResultRecord* next() {
		if (ptr_cur_record_ < records_.size()) {
			return &(records_[ptr_cur_record_++]);
		}
		return 0;
	}

private:
	void reset() {
		records_.clear();
		ptr_cur_record_=0;
	}

private:
	unsigned int ptr_cur_record_;
};

/*
sqlite ������
*/
class SQLiteWrapper {
  public:
    SQLiteWrapper();
	~SQLiteWrapper();

	/*
	@brief �����ݿ�
	@param  db_file  ���ݿ�·��
	@return  trueΪ�򿪳ɹ�
	*/
    bool Open(std::string const& db_file);

	

	/*
	@brief ��ѯ�������
	@param  stmt  ��ѯsql���
	@param  [in/out]  ���ݷ��ؽ��
	@return  trueΪִ�гɹ�
	*/
    bool SelectStmt           (std::string const& stmt, ResultTable& res);

	/*
	@brief ֱ��ִ��sql��亯��
	@param  stmt  sql���
	@return  trueΪִ�гɹ�
	*/
    bool DirectStatement      (std::string const& stmt);

	/*
	@brief ִ��sql��亯��,һ����Ҫ��������ʱʹ��
	@param  stmt  sql���
	@return  NULLΪִ�д���
	*/
    SQLiteStatement* Statement(std::string const& stmt);

	/*
	@brief ����ʧ��ʱ���ɵ��ô˺������ش�������
	*/
    std::string LastError();

	void Close();

    // sql �������
    bool Begin   ();
    bool Commit  ();
    bool Rollback();




  private:

	
	//��ѯ���ص�����
    static int SelectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

    sqlite3* db_;
};

#endif