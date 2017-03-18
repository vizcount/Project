///////////////////////////////////////////////////////////////////////////////
//
// ��Ȩ����(C), 2007, ��Ѷ����Ϣ���޹�˾
//
// �汾��1.0
// �ļ�˵�����ٽ���������
// �������ڣ�2007-4-10
// ���ߣ����ױ�
//
// �޸���ʷ��
// 1. ���ڣ�
//   ���ߣ�
//   �޸����ݣ�
// 2. 
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCK_H__EDA667BF_93B4_47B6_B1DC_A507D62563E5__INCLUDED_)
#define AFX_LOCK_H__EDA667BF_93B4_47B6_B1DC_A507D62563E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLock  
{
public:

    CLock();
	virtual ~CLock();

    //����
    void Lock();
    
    //����
    void Unlock();

private:

    CRITICAL_SECTION m_crit;
};

//���ھֲ�ʹ�ã��Զ������ͽ���������
class CLocalLock
{
public:
	//CLocalLock(){};
    CLocalLock(CLock * pLock);
    ~CLocalLock();

private:
    
    CLock * m_pLock;
};

#endif // !defined(AFX_LOCK_H__EDA667BF_93B4_47B6_B1DC_A507D62563E5__INCLUDED_)
