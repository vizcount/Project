
// TBRecord.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTBRecordApp: 
// �йش����ʵ�֣������ TBRecord.cpp
//

class CTBRecordApp : public CWinApp
{
public:
	CTBRecordApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTBRecordApp theApp;