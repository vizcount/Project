#pragma once

#define MAX_USER_PATH 256
#define MIN_LENGTH 20

/*
@breif �Ա��ṹ��Ϣ
*/
struct TBRecordInfo
{
	int  id;						//���
	char tbdate[MIN_LENGTH];			//����
	char tbName[MIN_LENGTH];			//�û���
	char tbWupin[MAX_USER_PATH];		//��Ʒ
	float tbJinE;		//���
	float tbYiFuKuan;	//�Ѹ���
	char tbWuliuNum[MIN_LENGTH];	    //��������
	char tbOrderNum[MIN_LENGTH * 2];	//������
	int  tbNumber;						//���
	char tbMemo[100000];				//��ע
	char tbTel[MIN_LENGTH];				//�ֻ���

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



