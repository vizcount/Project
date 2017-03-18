#pragma once

#define MAX_USER_PATH 256
#define MIN_LENGTH 20

/*
@breif 淘宝结构信息
*/
struct TBRecordInfo
{
	int  id;						//序号
	char tbdate[MIN_LENGTH];			//日期
	char tbName[MIN_LENGTH];			//用户名
	char tbWupin[MAX_USER_PATH];		//物品
	float tbJinE;		//金额
	float tbYiFuKuan;	//已付款
	char tbWuliuNum[MIN_LENGTH];	    //物流单号
	char tbOrderNum[MIN_LENGTH * 2];	//订单号
	int  tbNumber;						//编号
	char tbMemo[100000];				//备注
	char tbTel[MIN_LENGTH];				//手机号

	TBRecordInfo()
	{
		memset(tbdate, 0, sizeof(MIN_LENGTH));
		memset(tbName, 0, sizeof(MIN_LENGTH));
		memset(tbWupin, 0, sizeof(MAX_USER_PATH));
		memset(tbWuliuNum, 0, sizeof(MIN_LENGTH));
		memset(tbOrderNum, 0, sizeof(MIN_LENGTH));
		memset(tbMemo, 0, sizeof(100000));
		memset(tbTel, 0, sizeof(100000));

		tbJinE = 0;
		tbYiFuKuan = 0;
		tbNumber = 0;
		id = 0;
	}

};



