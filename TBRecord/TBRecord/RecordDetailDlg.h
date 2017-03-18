#pragma once
#include "afxdtctl.h"


// CRecordDetailDlg 对话框

class CRecordDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDetailDlg)

public:
	CRecordDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordDetailDlg();

// 对话框数据
	enum { IDD = IDD_DETAIL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSubmit();
	afx_msg void OnBnClickedButtonExit();

	//显示要展示的数据
	int ShowListData(TBRecordInfo tbInfo);
private:
	//宽字节CString转Char*
	char* CStringToMutilChar(CString& str, int& chLength, WORD wPage = CP_ACP);
	//限制文本框长度
	void LimitTextLen();

	//读取文本框的数据
	void GetEditData(TBRecordInfo &tInfo);

private:
	CDateTimeCtrl m_DateCtrl;

	int m_iId;

public:
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnBnClickedButtonPrintdetail();
};
