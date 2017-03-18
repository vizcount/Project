/* 
   SQLiteWrapper.h

   Copyright (C) 2004 René Nyffenegger

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

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
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

	//取得数据类型值
    dataType DataType(int pos_zero_indexed);


	//以下为取得函数具体值
    int         ValueInt   (int pos_zero_indexed);
    std::string ValueString(int pos_zero_indexed);

//    SQLiteStatement(const SQLiteStatement&);
   ~SQLiteStatement();

    //SQLiteStatement& operator=(SQLiteStatement const&);

    //以下函数为绑定各种类型的值
    bool Bind    (int pos_zero_indexed, std::string const& value);
    bool Bind    (int pos_zero_indexed, double             value);
    bool Bind    (int pos_zero_indexed, int                value);
    bool BindNull(int pos_zero_indexed);



	//执行sql语句
    bool Execute();

	//结果遍历
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

//具体一条数据类
class ResultRecord {
public:
	std::vector<std::string> fields_;
};

//结果类
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
sqlite 操作类
*/
class SQLiteWrapper {
  public:
    SQLiteWrapper();
	~SQLiteWrapper();

	/*
	@brief 打开数据库
	@param  db_file  数据库路径
	@return  true为打开成功
	*/
    bool Open(std::string const& db_file);

	

	/*
	@brief 查询结果函数
	@param  stmt  查询sql语句
	@param  [in/out]  数据返回结果
	@return  true为执行成功
	*/
    bool SelectStmt           (std::string const& stmt, ResultTable& res);

	/*
	@brief 直接执行sql语句函数
	@param  stmt  sql语句
	@return  true为执行成功
	*/
    bool DirectStatement      (std::string const& stmt);

	/*
	@brief 执行sql语句函数,一般需要做变量绑定时使用
	@param  stmt  sql语句
	@return  NULL为执行错误
	*/
    SQLiteStatement* Statement(std::string const& stmt);

	/*
	@brief 操作失败时，可调用此函数返回错误描述
	*/
    std::string LastError();

	void Close();

    // sql 事务操作
    bool Begin   ();
    bool Commit  ();
    bool Rollback();




  private:

	
	//查询语句回调函数
    static int SelectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

    sqlite3* db_;
};

#endif