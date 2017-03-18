
// TBRecordDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "SQLiteOperate.h"
#include "ExtractData.h"


// CTBRecordDlg �Ի���
class CTBRecordDlg : public CDialogEx
{
// ����
public:
	CTBRecordDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TBRECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// �б��¼
	CListCtrl m_ListRecord;
	// ��ʼ����
	CDateTimeCtrl m_StartDate;
	// ��������
	CDateTimeCtrl m_EndDate;
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonQuerybytime();
	afx_msg void OnBnClickedButtonAddrecord();
	afx_msg void OnDblclkListAllrecord(NMHDR *pNMHDR, LRESULT *pResult);
	//����һ������ͨ��sendmessage������ӵ�����
	afx_msg LRESULT GetData(WPARAM wParam, LPARAM lParam);

private:
	//��ʾ�б�����
	int ShowRecord();
	void GetCellName(int nRow, int nCol, CString &strName);

private:
	SQLiteOperate m_Sqlite;
	std::map<int, TBRecordInfo> m_mapTBInfo;
public:
	afx_msg void OnDel32771();
	afx_msg void OnNew32772();
	afx_msg void OnUpdate32773();
	afx_msg void OnNMRClickListAllrecord(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonExportexcel();
};
