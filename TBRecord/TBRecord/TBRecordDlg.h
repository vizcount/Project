
// TBRecordDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "SQLiteOperate.h"
#include "ExtractData.h"


// CTBRecordDlg 对话框
class CTBRecordDlg : public CDialogEx
{
// 构造
public:
	CTBRecordDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TBRECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 列表记录
	CListCtrl m_ListRecord;
	// 开始日期
	CDateTimeCtrl m_StartDate;
	// 结束日期
	CDateTimeCtrl m_EndDate;
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonQuerybytime();
	afx_msg void OnBnClickedButtonAddrecord();
	afx_msg void OnDblclkListAllrecord(NMHDR *pNMHDR, LRESULT *pResult);
	//从另一个窗口通过sendmessage返回添加的数据
	afx_msg LRESULT GetData(WPARAM wParam, LPARAM lParam);

private:
	//显示列表数据
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
