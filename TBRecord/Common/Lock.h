///////////////////////////////////////////////////////////////////////////////
//
// 版权所有(C), 2007, 商讯网信息有限公司
//
// 版本：1.0
// 文件说明：临界区操作类
// 生成日期：2007-4-10
// 作者：刘兆兵
//
// 修改历史：
// 1. 日期：
//   作者：
//   修改内容：
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

    //加锁
    void Lock();
    
    //解锁
    void Unlock();

private:

    CRITICAL_SECTION m_crit;
};

//用于局部使用，自动加锁和解锁的类型
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
