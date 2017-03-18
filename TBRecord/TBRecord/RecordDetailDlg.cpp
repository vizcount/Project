// RecordDetailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TBRecord.h"
#include "ExtractData.h"
#include "RecordDetailDlg.h"
#include "afxdialogex.h"


// CRecordDetailDlg �Ի���

IMPLEMENT_DYNAMIC(CRecordDetailDlg, CDialogEx)

CRecordDetailDlg::CRecordDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecordDetailDlg::IDD, pParent)
{
	m_iId = 0;
}

CRecordDetailDlg::~CRecordDetailDlg()
{
}

void CRecordDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, m_DateCtrl);
}


BEGIN_MESSAGE_MAP(CRecordDetailDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SUBMIT, &CRecordDetailDlg::OnBnClickedButtonSubmit)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CRecordDetailDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CRecordDetailDlg::OnBnClickedButtonUpdate)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CRecordDetailDlg::OnEnChangeEditName)
	ON_BN_CLICKED(IDC_BUTTON_PRINTDETAIL, &CRecordDetailDlg::OnBnClickedButtonPrintdetail)
END_MESSAGE_MAP()


// CRecordDetailDlg ��Ϣ�������


void CRecordDetailDlg::OnBnClickedButtonSubmit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TBRecordInfo tInfo;
	GetEditData(tInfo);

	::SendMessage(GetParent()->GetSafeHwnd(), GETDATA, 0,(LPARAM)&tInfo);

	OnOK();

/*
	m_StartDate.SetFormat(_T("yyyy-MM-dd"));
	CTime time(2016, 10, 22,0,0,0);
	m_DateCtrl.SetTime(&time);*/
}


void CRecordDetailDlg::OnBnClickedButtonExit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

char* CRecordDetailDlg::CStringToMutilChar(CString& str, int& chLength, WORD wPage /*= CP_ACP*/)
{
	char* pszMultiByte;
	int iSize = WideCharToMultiByte(wPage, 0, str, -1, NULL, 0, NULL, NULL);
	pszMultiByte = (char*)malloc((iSize + 1)/**sizeof(char)*/);
	memset(pszMultiByte, 0, iSize + 1);
	WideCharToMultiByte(wPage, 0, str, -1, pszMultiByte, iSize, NULL, NULL);
	chLength = iSize;
	return pszMultiByte;
}

int CRecordDetailDlg::ShowListData(TBRecordInfo tbInfo)
{
	LimitTextLen();
	CString strTmp = _T("");
	//����ʱ��
	int iYear, iMon, iDay;
	strTmp = (CString)tbInfo.tbdate;
	iYear = StrToInt(strTmp.Left(4));
	iMon = StrToInt(strTmp.Mid(6, 2));
	iDay = StrToInt(strTmp.Right(2));
	CTime time(iYear, iMon, iDay, 0, 0, 0);
	m_DateCtrl.SetTime(&time);
	
	strTmp.Empty();
	strTmp = (CString)tbInfo.tbName;
	SetDlgItemText(IDC_EDIT_NAME, strTmp);
	strTmp.Empty();
	strTmp = (CString)tbInfo.tbTel;
	SetDlgItemText(IDC_EDIT_TEL, strTmp);
	strTmp.Empty();
	strTmp = (CString)tbInfo.tbWupin;
	SetDlgItemText(IDC_EDIT_THINGS, strTmp);
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), tbInfo.tbJinE);
	SetDlgItemText(IDC_EDIT_AMOUNT, strTmp);
	strTmp.Empty();
	strTmp.Format(_T("%0.2f"), tbInfo.tbYiFuKuan);
	SetDlgItemText(IDC_EDIT_MONEY, strTmp);
	strTmp.Empty();
	strTmp = (CString)tbInfo.tbWuliuNum;
	SetDlgItemText(IDC_EDIT_LOGNUM, strTmp);
	strTmp.Empty();
	strTmp = (CString)tbInfo.tbOrderNum;
	SetDlgItemText(IDC_EDIT_ORDERNUM, strTmp);
	strTmp.Empty();
	strTmp = (CString)tbInfo.tbMemo;
	SetDlgItemText(IDC_EDIT_CONTENT, strTmp);

	SetDlgItemInt(IDC_EDIT_NUM, tbInfo.tbNumber);

	m_iId = tbInfo.id;
	GetDlgItem(IDC_BUTTON_SUBMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(TRUE);

	return 1;
}



void CRecordDetailDlg::OnBnClickedButtonUpdate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	LimitTextLen();

	TBRecordInfo tInfo;

	GetEditData(tInfo);

	tInfo.id = m_iId;
	m_iId = 0;
	::SendMessage(GetParent()->GetSafeHwnd(), GETDATA, 1, (LPARAM)&tInfo);

	OnOK();
}

void CRecordDetailDlg::LimitTextLen()
{
	((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetLimitText(19);
	((CEdit*)GetDlgItem(IDC_EDIT_TEL))->SetLimitText(11);
	((CEdit*)GetDlgItem(IDC_EDIT_THINGS))->SetLimitText(255);
	((CEdit*)GetDlgItem(IDC_EDIT_ORDERNUM))->SetLimitText(39);
	((CEdit*)GetDlgItem(IDC_EDIT_LOGNUM))->SetLimitText(19);
	((CEdit*)GetDlgItem(IDC_EDIT_CONTENT))->SetLimitText(9999);
}



void CRecordDetailDlg::OnEnChangeEditName()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	LimitTextLen();
}

void CRecordDetailDlg::GetEditData(TBRecordInfo &tInfo)
{
	CString strDate, strName, strTel, strThings, strAmount, strMoney, strWuliuNum, strOrder, strMemo;
	int iNumber = -1;
	//����
	CTime Date;
	m_DateCtrl.GetTime(Date);
	strDate = Date.Format(_T("%Y-%m-%d"));
	//����
	GetDlgItemText(IDC_EDIT_NAME, strName);
	GetDlgItemText(IDC_EDIT_TEL, strTel);
	GetDlgItemText(IDC_EDIT_THINGS, strThings);
	GetDlgItemText(IDC_EDIT_AMOUNT, strAmount);
	GetDlgItemText(IDC_EDIT_MONEY, strMoney);
	GetDlgItemText(IDC_EDIT_LOGNUM, strWuliuNum);
	GetDlgItemText(IDC_EDIT_ORDERNUM, strOrder);
	GetDlgItemText(IDC_EDIT_CONTENT, strMemo);
	iNumber = GetDlgItemInt(IDC_EDIT_NUM);

	int iLen = -1;
	char* szDate = CStringToMutilChar(strDate, iLen);
	memcpy(tInfo.tbdate, szDate, iLen);
	char* szName = CStringToMutilChar(strName, iLen);
	memcpy(tInfo.tbName, szName, iLen);
	free(szName);
	char* szTel = CStringToMutilChar(strTel, iLen);
	memcpy(tInfo.tbTel, szTel, iLen);
	free(szTel);
	char* szThings = CStringToMutilChar(strThings, iLen);
	memcpy(tInfo.tbWupin, szThings, iLen);
	free(szThings);
	tInfo.tbJinE = _tstof(strAmount);
	tInfo.tbYiFuKuan = _tstof(strMoney);
	char* szWuliu = CStringToMutilChar(strWuliuNum, iLen);
	memcpy(tInfo.tbWuliuNum, szWuliu, iLen);
	free(szWuliu);
	char* szOrder = CStringToMutilChar(strOrder, iLen);
	memcpy(tInfo.tbOrderNum, szOrder, iLen);
	free(szOrder);
	char* szMemo = CStringToMutilChar(strMemo, iLen);
	memcpy(tInfo.tbMemo, szMemo, iLen);
	free(szMemo);
	tInfo.tbNumber = iNumber;
	
}


void CRecordDetailDlg::OnBnClickedButtonPrintdetail()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strMessage;	CString strPrintDevice;	
	//����	
	CString strDate, strName, strTel, strThings, strAmount, strMoney, strWuliuNum, strOrder, strMemo;	
	CTime Date;	m_DateCtrl.GetTime(Date);	strDate = Date.Format(_T("%Y-%m-%d"));		//����	
	GetDlgItemText(IDC_EDIT_NAME, strName);	
	GetDlgItemText(IDC_EDIT_TEL, strTel);	
	GetDlgItemText(IDC_EDIT_THINGS, strThings);	
	GetDlgItemText(IDC_EDIT_AMOUNT, strAmount);	
	GetDlgItemText(IDC_EDIT_MONEY, strMoney);	
	GetDlgItemText(IDC_EDIT_LOGNUM, strWuliuNum);	
	GetDlgItemText(IDC_EDIT_ORDERNUM, strOrder);	
	GetDlgItemText(IDC_EDIT_CONTENT, strMemo);	
	int nLimName = 35;	
	int nLimMoney = 40;	
	int nLimOrder = 32;	
	
	for (int i = 37; i < strMemo.GetLength();)	
	{			
		if (i<strThings.GetLength())		
		{			
			strThings.Insert(i, _T("\r\n "));		
		}				
		strMemo.Insert(i, _T("\r\n "));
		i += 46;	
	}	
	
	strMessage.Format(_T("%*s�Ա������굥\r\n\r\n ���ڣ�%s%*s ������%s\r\n\r\n ��Ʒ����%s\r\n\r\n ��%s%*s �Ѹ��%s\r\n\r\n �ֻ��ţ�%s%*s �������ţ�%s\r\n\r\n �����ţ�%s\r\n\r\n ��ע��%s\r\n "),
		nLimName, strMessage, strDate, nLimName, strMessage, strName, strThings, strAmount, nLimMoney, strMessage, strMoney, strTel, nLimOrder, strMessage, strWuliuNum, strOrder, strMemo);
	DWORD dwFlags = PD_PAGENUMS | PD_HIDEPRINTTOFILE;	
	CPrintDialog print(FALSE,dwFlags,NULL);		
	if (IDOK == print.DoModal())
	{
		CDC dc;
		dc.Attach(print.GetPrinterDC());
		DOCINFO di;
		di.cbSize = sizeof(di);
		di.lpszDocName = _T("��ӡ����");
		di.lpszDatatype = NULL;
		di.lpszOutput = NULL;
		di.fwType = 0;
		dc.StartDoc(&di);
		dc.StartPage();
		dc.SetMapMode(MM_TEXT);
		CRect recPrint(0, 0, dc.GetDeviceCaps(LOGPIXELSX), dc.GetDeviceCaps(LOGPIXELSY));
		dc.DPtoLP(&recPrint);
		dc.SetWindowOrg(0, 0);
		CFont newFont;
		VERIFY(newFont.CreatePointFont(130, _T("����"), &dc));
		CFont *oldFont = dc.SelectObject(&newFont);
		dc.SetTextAlign(TA_TOP | TA_LEFT);

		CString strPrint;
		int nIndex = 0;
		int x = 50;
		int y = 150;
		CSize textSize;
		textSize = dc.GetTextExtent(_T("00"), 2);
		//���ݵ�ǰ����Ŀ��ߣ������Դ˸߶�Ϊ�и�		
		while ((nIndex = strMessage.Find(_T("\r\n"))) > -1)
		{
			strPrint = strMessage.Left(nIndex);
			strMessage = strMessage.Mid(nIndex + 2);

			dc.TextOutW(x, y, strPrint);

			y += textSize.cy + 150;												//����һ�У��и�Ϊ����߶�
		}

		dc.SelectObject(oldFont);
		newFont.DeleteObject();
		dc.EndPage();
		dc.EndDoc();
		DeleteDC(dc.Detach());
	}
}
