
// TBRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBRecord.h"
#include "TBRecordDlg.h"
#include "afxdialogex.h"
#include "RecordDetailDlg.h"

#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTBRecordDlg 对话框



CTBRecordDlg::CTBRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTBRecordDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTBRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALLRECORD, m_ListRecord);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_StartDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_EndDate);
}

BEGIN_MESSAGE_MAP(CTBRecordDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CTBRecordDlg::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_QUERYBYTIME, &CTBRecordDlg::OnBnClickedButtonQuerybytime)
	ON_BN_CLICKED(IDC_BUTTON_ADDRECORD, &CTBRecordDlg::OnBnClickedButtonAddrecord)
	ON_MESSAGE(GETDATA,&CTBRecordDlg::GetData)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ALLRECORD, &CTBRecordDlg::OnDblclkListAllrecord)
	ON_COMMAND(ID_DEL_32771, &CTBRecordDlg::OnDel32771)
	ON_COMMAND(ID_NEW_32772, &CTBRecordDlg::OnNew32772)
	ON_COMMAND(ID_UPDATE_32773, &CTBRecordDlg::OnUpdate32773)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALLRECORD, &CTBRecordDlg::OnNMRClickListAllrecord)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_EXPORTEXCEL, &CTBRecordDlg::OnBnClickedButtonExportexcel)
END_MESSAGE_MAP()


// CTBRecordDlg 消息处理程序

BOOL CTBRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_ListRecord.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ListRecord.InsertColumn(0, _T("日期"), LVCFMT_CENTER, 90);
	m_ListRecord.InsertColumn(1, _T("姓名"), LVCFMT_CENTER, 90);
	m_ListRecord.InsertColumn(2, _T("手机号"), LVCFMT_CENTER, 100);
	m_ListRecord.InsertColumn(3, _T("物品"), LVCFMT_CENTER, 200);
	m_ListRecord.InsertColumn(4, _T("金额"), LVCFMT_CENTER, 80);
	m_ListRecord.InsertColumn(5, _T("付款"), LVCFMT_CENTER, 80);
	m_ListRecord.InsertColumn(6, _T("物流单号"), LVCFMT_CENTER, 150);
	m_ListRecord.InsertColumn(7, _T("订单号"), LVCFMT_CENTER, 170);
	m_ListRecord.InsertColumn(8, _T("编号"), LVCFMT_LEFT, 68);
	m_ListRecord.InsertColumn(9, _T("ID"), LVCFMT_CENTER, 0);

	m_Sqlite.InitDb();	//初始化数据库

	//获取信息	
	m_mapTBInfo.clear();
	m_mapTBInfo = m_Sqlite.GetVarInfo();

	ShowRecord();
	
	//最后一行获取焦点
	int iCnt = m_mapTBInfo.size();	
	m_ListRecord.SetItemState(iCnt-1,0,LVIS_SELECTED);
	m_ListRecord.EnsureVisible(iCnt-1, TRUE);
	//格式化日期
	m_StartDate.SetFormat(_T("yyyy-MM-dd"));
	m_EndDate.SetFormat(_T("yyyy-MM-dd"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTBRecordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTBRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTBRecordDlg::OnBnClickedButtonQuery()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strSearch = _T("");
	GetDlgItemText(IDC_EDIT_QUERY, strSearch);

	m_ListRecord.DeleteAllItems();

	float fTotleMoney = 0; //总金额
	float fApayMoney = 0;  //已付款金额
	float fNopayMoeny = 0; //未付款
	int iPos = 0;
	CString strTmp = _T("");
	for (auto v:m_mapTBInfo)
	{
		if ((CString(v.second.tbName).Find(strSearch)) != -1 || 
			(CString(v.second.tbWupin).Find(strSearch)) != -1 ||
			(CString(v.second.tbWuliuNum).Find(strSearch)) != -1 ||
			(CString(v.second.tbTel).Find(strSearch)) != -1 ||
			(CString(v.second.tbOrderNum).Find(strSearch)) != -1)
		{
			int i = 1;
			
			m_ListRecord.InsertItem(iPos, (CString)v.second.tbdate);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbName);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbTel);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbWupin);
			strTmp.Format(_T("%0.2f"), v.second.tbJinE);
			m_ListRecord.SetItemText(iPos, i++, strTmp);
			strTmp.Format(_T("%0.2f"), v.second.tbYiFuKuan);
			m_ListRecord.SetItemText(iPos, i++, strTmp);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbWuliuNum);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbOrderNum);
			strTmp.Format(_T("%d"), v.second.tbNumber);
			m_ListRecord.SetItemText(iPos, i++, strTmp);
			strTmp.Format(_T("%d"), v.first);
			m_ListRecord.SetItemText(iPos, i++, strTmp);

			fTotleMoney += v.second.tbJinE;
			fApayMoney += v.second.tbYiFuKuan;
			iPos++;
		}
	}
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fTotleMoney);
	SetDlgItemText(IDC_EDIT_TOTLEMONEY, strTmp);
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fApayMoney);
	SetDlgItemText(IDC_EDIT_APAYMONEY, strTmp);
	fNopayMoeny = fTotleMoney - fApayMoney;
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fNopayMoeny);
	SetDlgItemText(IDC_EDIT_NOTPAYMONEY, strTmp);
}


void CTBRecordDlg::OnBnClickedButtonQuerybytime()
{
	// TODO:  在此添加控件通知处理程序代码
	//先取时间
	CTime sDate;
	CTime fDate;
	CTimeSpan span;
	m_StartDate.GetTime(sDate);
	m_EndDate.GetTime(fDate);

	span = fDate - sDate;

	if (span.GetDays()<0)
	{
		return;
	}

	m_ListRecord.DeleteAllItems();

	int iPos = 0;
	float fTotleMoney = 0; //总金额
	float fApayMoney = 0;  //已付款金额
	float fNopayMoeny = 0; //未付款
	CString strTmp = _T("");
	for (auto v : m_mapTBInfo)
	{		
		//设置时间
		int iYear, iMon, iDay;
		strTmp = (CString)v.second.tbdate;
		iYear = StrToInt(strTmp.Left(4));
		iMon = StrToInt(strTmp.Mid(6, 2));
		iDay = StrToInt(strTmp.Right(2));
		strTmp.Empty();
		CTime cDate(iYear, iMon, iDay, 0, 0, 0);
		CTimeSpan Sp1, Sp2;
		Sp1 = cDate - sDate;
		Sp2 = fDate - cDate;
		if (Sp1.GetDays()>=0 && Sp2.GetDays()>=0)
		{
			int i = 1;

			m_ListRecord.InsertItem(iPos, (CString)v.second.tbdate);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbName);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbTel);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbWupin);
			strTmp.Format(_T("%0.2f"), v.second.tbJinE);
			m_ListRecord.SetItemText(iPos, i++, strTmp);
			strTmp.Format(_T("%0.2f"), v.second.tbYiFuKuan);
			m_ListRecord.SetItemText(iPos, i++, strTmp);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbWuliuNum);
			m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbOrderNum);
			strTmp.Format(_T("%d"), v.second.tbNumber);
			m_ListRecord.SetItemText(iPos, i++, strTmp);
			strTmp.Format(_T("%d"), v.first);
			m_ListRecord.SetItemText(iPos, i++, strTmp);

			fTotleMoney += v.second.tbJinE;
			fApayMoney += v.second.tbYiFuKuan;
			iPos++;
		}
	}

	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fTotleMoney);
	SetDlgItemText(IDC_EDIT_TOTLEMONEY, strTmp);
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fApayMoney);
	SetDlgItemText(IDC_EDIT_APAYMONEY, strTmp);
	fNopayMoeny = fTotleMoney - fApayMoney;
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fNopayMoeny);
	SetDlgItemText(IDC_EDIT_NOTPAYMONEY, strTmp);

}


void CTBRecordDlg::OnBnClickedButtonAddrecord()
{
	// TODO:  在此添加控件通知处理程序代码
	CRecordDetailDlg *pDetail = new CRecordDetailDlg;

	pDetail->DoModal();

	delete pDetail;
	
}


LRESULT CTBRecordDlg::GetData(WPARAM wParam, LPARAM lParam)
{
	TBRecordInfo *tInfo = (TBRecordInfo *)lParam;

	if (wParam == 0)
	{		
		if (m_Sqlite.SaveInfo(*tInfo))
		{
			//获取ID
			tInfo->id = m_Sqlite.GetID();
			if (tInfo->id == 0)
			{
				MessageBox(_T("查询数据库存在问题，请联系开发者，"), _T("查询出错"), MB_OK | MB_ICONERROR);
				return 0;
			}
			//压入到容器中
			m_mapTBInfo[tInfo->id]= *tInfo;
			ShowRecord();
		}
		else
		{
			MessageBox(_T("保存到数据库失败"), _T("错误提示"), MB_OK | MB_ICONERROR);
		}
	}
	else if (wParam == 1)
	{
		if (m_Sqlite.UpdateInfo(*tInfo))
		{

			m_mapTBInfo[tInfo->id] = *tInfo;
			ShowRecord();
			/*std::vector<TBRecordInfo>::iterator iter = m_vTBInfo.begin();
			int i = 0;
			for (; iter != m_vTBInfo.end(); iter++)
			{
				if (iter->id == tInfo->id)
				{					
					m_vTBInfo.erase(iter);					
					m_vTBInfo.insert(m_vTBInfo.begin()+i, *tInfo);
					ShowRecord();
					break;
				}
				i++;
			}*/
		}
	}
	
	//最后一行获取焦点
	int iCnt = m_mapTBInfo.size();
	m_ListRecord.SetItemState(iCnt - 1, 0, LVIS_SELECTED);
	m_ListRecord.EnsureVisible(iCnt - 1, TRUE);
	
	return S_OK;
}


void CTBRecordDlg::OnDblclkListAllrecord(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	OnUpdate32773();

	*pResult = 0;
}

int CTBRecordDlg::ShowRecord()
{
	m_ListRecord.DeleteAllItems();

	float fTotleMoney = 0; //总金额
	float fApayMoney = 0;  //已付款金额
	float fNopayMoeny = 0; //未付款
	int iPos = 0;
	CString strTmp = _T("");
	for (auto v : m_mapTBInfo)
	{
		int i = 1;
		m_ListRecord.InsertItem(iPos, (CString)v.second.tbdate);
		m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbName);
		m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbTel);
		m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbWupin);
		strTmp.Format(_T("%0.2f"), v.second.tbJinE);
		m_ListRecord.SetItemText(iPos, i++, strTmp);
		strTmp.Format(_T("%0.2f"), v.second.tbYiFuKuan);
		m_ListRecord.SetItemText(iPos, i++, strTmp);
		m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbWuliuNum);
		m_ListRecord.SetItemText(iPos, i++, (CString)v.second.tbOrderNum);
		strTmp.Format(_T("%d"), v.second.tbNumber);
		m_ListRecord.SetItemText(iPos, i++, strTmp);
		strTmp.Format(_T("%d"), v.first);
		m_ListRecord.SetItemText(iPos, i++, strTmp);

		fTotleMoney += v.second.tbJinE;
		fApayMoney += v.second.tbYiFuKuan;
		iPos++;
	}

	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fTotleMoney);
	SetDlgItemText(IDC_EDIT_TOTLEMONEY, strTmp);
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fApayMoney);
	SetDlgItemText(IDC_EDIT_APAYMONEY, strTmp);
	fNopayMoeny = fTotleMoney - fApayMoney;
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), fNopayMoeny);
	SetDlgItemText(IDC_EDIT_NOTPAYMONEY, strTmp);
	return 1;
}


void CTBRecordDlg::OnDel32771()
{
	// TODO:  在此添加命令处理程序代码
	if (m_ListRecord.GetItemCount()<= 0)
	{
		return;
	}

	POSITION ps;
	int nIndex;
	ps = m_ListRecord.GetFirstSelectedItemPosition();
	nIndex = m_ListRecord.GetNextSelectedItem(ps);

	//获取所选队列在容器中的索引
	int nPos = _tstoi(m_ListRecord.GetItemText(nIndex, 9));

	/*nIndex = 0;
	for (auto v : m_vTBInfo)
	{
		if (v.id == nPos)
		{
			tInfo = v;
			break;
		}
		nIndex++;
	}*/

	m_Sqlite.DeleteInfo(nPos);

	/*m_vTBInfo.erase(m_vTBInfo.begin()+nIndex);*/
	m_mapTBInfo.erase(nPos);
	ShowRecord();

	//最后一行获取焦点
	int iCnt = m_mapTBInfo.size();
	m_ListRecord.SetItemState(iCnt - 1, 0, LVIS_SELECTED);
	m_ListRecord.EnsureVisible(iCnt - 1, TRUE);
}


void CTBRecordDlg::OnNew32772()
{
	// TODO:  在此添加命令处理程序代码
	OnBnClickedButtonAddrecord();
}


void CTBRecordDlg::OnUpdate32773()
{
	// TODO:  在此添加控件通知处理程序代码
	TBRecordInfo tInfo;

	if (m_ListRecord.GetItemCount() <= 0)
	{
		return;
	}
	POSITION ps;
	int nIndex;
	ps = m_ListRecord.GetFirstSelectedItemPosition();
	nIndex = m_ListRecord.GetNextSelectedItem(ps);

	//获取所选队列在容器中的索引
	int nPos = _tstoi(m_ListRecord.GetItemText(nIndex, 9));

	CRecordDetailDlg *pDlg = new CRecordDetailDlg;
	pDlg->Create(IDD_DETAIL_DLG);
	pDlg->ShowListData(m_mapTBInfo[nPos]);
	pDlg->ShowWindow(SW_SHOW);

}


void CTBRecordDlg::OnNMRClickListAllrecord(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint pt(LOWORD(dwPos), HIWORD(dwPos));

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU1));
		CMenu* popup = menu.GetSubMenu(0);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}

	*pResult = 0;
}


void CTBRecordDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	//----最上面一列------
	GetDlgItem(IDC_STATIC_QUERY)->MoveWindow(10, 10,72, 30,FALSE);
	GetDlgItem(IDC_EDIT_QUERY)->MoveWindow(80, 10, cx / 10, 30,FALSE);
	GetDlgItem(IDC_BUTTON_QUERY)->MoveWindow(cx / 10 + 100, 10, cx / 14, 30,FALSE);
	GetDlgItem(IDC_BUTTON_QUERYBYTIME)->MoveWindow(cx / 10 * 9 - 20, 10, cx / 10, 30,FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_END)->MoveWindow(cx / 5 * 4 - 40, 10, cx / 10, 30,FALSE);
	GetDlgItem(IDC_STATIC_line)->MoveWindow(cx / 5 * 4 - 70, 10, 30, 30,FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_START)->MoveWindow(cx / 10 * 7 - 70, 10, cx / 10, 30,FALSE);
	//---列表框列-------	
	GetDlgItem(IDC_LIST_ALLRECORD)->EnableWindow(FALSE);
	GetDlgItem(IDC_LIST_ALLRECORD)->MoveWindow(3, 60, cx - 7, cy - 120, FALSE);
	m_ListRecord.SetColumnWidth(0,cx / 11 );
	m_ListRecord.SetColumnWidth(1, cx / 11 );
	m_ListRecord.SetColumnWidth(2, cx / 11);
	m_ListRecord.SetColumnWidth(3, cx * 5 / 22);
	m_ListRecord.SetColumnWidth(4, cx / 11);
	m_ListRecord.SetColumnWidth(5, cx / 11);
	m_ListRecord.SetColumnWidth(6, cx*3 / 22);
	m_ListRecord.SetColumnWidth(7, cx*3 / 22);
	m_ListRecord.SetColumnWidth(8, cx / 22);
	/*m_ListRecord.SetColumnWidth(9, cx / 22);*/
	GetDlgItem(IDC_LIST_ALLRECORD)->EnableWindow(TRUE);

	//最底层一列
	GetDlgItem(IDC_BUTTON_ADDRECORD)->MoveWindow(3, cy-40, cx / 10, 30,FALSE);
	GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->MoveWindow(cx / 10 * 3/2, cy - 40, cx / 10, 30, FALSE);
	GetDlgItem(IDC_STATIC_ALLPAY)->MoveWindow(cx / 10*3, cy-40, 80, 30,FALSE);
	GetDlgItem(IDC_EDIT_TOTLEMONEY)->MoveWindow(cx / 10*3 + 80, cy-40, cx / 10, 30,FALSE);
	GetDlgItem(IDC_STATIC_YUANO)->MoveWindow(cx /5* 2  + 82, cy-40, 20, 30,FALSE);
	GetDlgItem(IDC_STATIC_ALREADY)->MoveWindow(cx /5* 2 + 132, cy-40, 80, 30,FALSE);
	GetDlgItem(IDC_EDIT_APAYMONEY)->MoveWindow(cx /5* 2 + 212, cy-40, cx / 10, 30,FALSE);
	GetDlgItem(IDC_STATIC_YUANT)->MoveWindow(cx /2 + 214, cy-40, 20, 30,FALSE);
	GetDlgItem(IDC_STATIC_NOPAY)->MoveWindow(cx / 2 + 264, cy-40, 80, 30,FALSE);
	GetDlgItem(IDC_EDIT_NOTPAYMONEY)->MoveWindow(cx / 2 + 344, cy-40, cx / 10, 30,FALSE);
	GetDlgItem(IDC_STATIC_YUANH)->MoveWindow(cx / 5*3 + 346, cy-40, 20, 30,FALSE);

	/*m_ListRecord.ShowScrollBar(SB_HORZ, FALSE);*/

	Invalidate();

}

//导出到Excel
void CTBRecordDlg::OnBnClickedButtonExportexcel()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->EnableWindow(FALSE);
	CFileDialog dlg(FALSE, _T("*.xls"), nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.xls)|(*.xls||"), NULL);

	if (IDOK != dlg.DoModal())
	{
		GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->EnableWindow(TRUE);
		return;
	}

	CString strFileName = dlg.GetPathName();
	COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CApplication app;
	CWorkbook book;
	CWorkbooks books;
	CWorksheet sheet;
	CWorksheets sheets;
	CRange range;
	CFont0 font;

	if (!app.CreateDispatch(_T("Excel.Application")))
	{
		GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->EnableWindow(TRUE);
		MessageBox(_T("加载Excel对象失败"));
		return;
	}

	books = app.get_Workbooks();
	book = books.Add(covOptional);

	sheets = book.get_Worksheets();
	sheet = sheets.get_Item(COleVariant((short)1));

	CHeaderCtrl *pMyHeaderCtrl;
	pMyHeaderCtrl = m_ListRecord.GetHeaderCtrl(); //获得列表头；

	int iRow, iCol;
	int cols = pMyHeaderCtrl->GetItemCount();
	int rows = m_ListRecord.GetItemCount();

	HDITEM   hdi;

	TCHAR     lpBuffer[256];

	bool       fFound = false;



	hdi.mask = HDI_TEXT;

	hdi.pszText = lpBuffer;

	hdi.cchTextMax = 256;

	CString   colname;

	CString strTemp;

	for (iCol = 0; iCol < cols; iCol++)//将列表的标题头写入EXCEL

	{

		GetCellName(1, iCol + 1, colname);

		range = sheet.get_Range(COleVariant(colname), COleVariant(colname));

		pMyHeaderCtrl->GetItem(iCol, &hdi);

		range.put_Value2(COleVariant(hdi.pszText));

		int   nWidth = m_ListRecord.GetColumnWidth(iCol) / 9;

		//得到第iCol+1列  

		/*range.AttachDispatch(range.get_Item(_variant_t((long)(iCol + 1)), vtMissing).pdispVal, true);*/

		//设置列宽 

		range.put_ColumnWidth(_variant_t((long)nWidth));

	}

	range = sheet.get_Range(COleVariant(_T("A1 ")), COleVariant(colname));

	range.put_RowHeight(_variant_t((long)20));//设置行的高度

	font = range.get_Font();

	font.put_Bold(covTrue);

	range.put_VerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108



	COleSafeArray   saRet;

	DWORD   numElements[] = { rows, cols };       //5x2   element   array

	saRet.Create(VT_BSTR, 2, numElements);

	range = sheet.get_Range(COleVariant(_T("A2 ")), covOptional);

	range = range.get_Resize(COleVariant((short)rows), COleVariant((short)cols));

	long   index[2];

	range = sheet.get_Range(COleVariant(_T("A2 ")), covOptional);

	range = range.get_Resize(COleVariant((short)rows), COleVariant((short)cols));

	for (iRow = 1; iRow <= rows; iRow++)//将列表内容写入EXCEL

	{

		for (iCol = 1; iCol <= cols; iCol++)

		{

			index[0] = iRow - 1;

			index[1] = iCol - 1;

			CString   szTemp;

			szTemp = m_ListRecord.GetItemText(iRow - 1, iCol - 1);

			BSTR   bstr = szTemp.AllocSysString();

			saRet.PutElement(index, bstr);

			SysFreeString(bstr);

		}

	}

	range.put_Value2(COleVariant(saRet));

	saRet.Detach();

	book.SaveCopyAs(COleVariant(strFileName));

	//       cellinterior.ReleaseDispatch();

	book.put_Saved(true);

	book.ReleaseDispatch();

	books.ReleaseDispatch();

	app.Quit();

	app.ReleaseDispatch();

	GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->EnableWindow(TRUE);
}

void CTBRecordDlg::GetCellName(int nRow, int nCol, CString &strName)
{
	int nSeed = nCol;

	CString strRow;

	char cCell = 'A' + nCol - 1;

	strName.Format(_T("%c"), cCell);
	strRow.Format(_T("%d "), nRow);

	strName += strRow;
}


