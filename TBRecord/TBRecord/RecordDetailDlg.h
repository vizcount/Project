#pragma once
#include "afxdtctl.h"


// CRecordDetailDlg �Ի���

class CRecordDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDetailDlg)

public:
	CRecordDetailDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordDetailDlg();

// �Ի�������
	enum { IDD = IDD_DETAIL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSubmit();
	afx_msg void OnBnClickedButtonExit();

	//��ʾҪչʾ������
	int ShowListData(TBRecordInfo tbInfo);
private:
	//���ֽ�CStringתChar*
	char* CStringToMutilChar(CString& str, int& chLength, WORD wPage = CP_ACP);
	//�����ı��򳤶�
	void LimitTextLen();

	//��ȡ�ı��������
	void GetEditData(TBRecordInfo &tInfo);

private:
	CDateTimeCtrl m_DateCtrl;

	int m_iId;

public:
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnBnClickedButtonPrintdetail();
};
