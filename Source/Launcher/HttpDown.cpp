// HttpSocket.cpp: implementation of the HttpSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HttpDown.h"
#include "launcher.h"

#include "DownloadInfo.h"
#include "launcherDlg.h"
#include "util.h"
#include "stream_transport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define RECV_TIME_OUT (1000*6)
//-----------------------------------------------------------------------------
// Construction/Destruction
//-----------------------------------------------------------------------------
HttpSocket::HttpSocket():m_Trunk(this)
{
	ZeroMemory(m_szRequestHeader, sizeof(m_szRequestHeader));
	ZeroMemory(m_szResponseHeader, sizeof(m_szResponseHeader));
	ZeroMemory(m_szShowInfo, sizeof(m_szShowInfo));
	ZeroMemory(m_szShowLog, sizeof(m_szShowLog));

	m_pStream= new StreamTransport;
	m_pUtil	= new Util;
	m_hThread = 0;
	m_bTerminateThread = FALSE;
	m_bResult = FALSE;
}

HttpSocket::~HttpSocket()
{
	SAFE_DEL(m_pUtil);
	SAFE_DEL(m_pStream);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL HttpSocket::Connect(const char *szHostName,int nPort)
{
	if(szHostName==NULL)
		return FALSE;

	///�������������IP��ַ
	hostent* pHostent = gethostbyname(szHostName);
	if( pHostent == NULL )
	{
		CString msg;
		msg.Format(g_StrTable[_T("BadHostName")], m_pUtil->AnsiToUnicode(szHostName));
		MessageBox(NULL, msg, g_StrTable[_T("Error")], MB_OK);
		return FALSE;
	}

	///����
	struct in_addr ip_addr;
	memcpy(&ip_addr, pHostent->h_addr_list[0], 4); // h_addr_list[0]��4���ֽ�,ÿ���ֽ�8λ

	struct sockaddr_in destaddr;
	memset((void *)&destaddr,0,sizeof(destaddr)); 
	destaddr.sin_family=AF_INET;
	destaddr.sin_port=htons(nPort);
	destaddr.sin_addr=ip_addr;


	///����������IP��ַ�ַ���
	char szIP[256];
	sprintf(szIP,"%d.%d.%d.%d",
		destaddr.sin_addr.S_un.S_un_b.s_b1,
		destaddr.sin_addr.S_un.S_un_b.s_b2,
		destaddr.sin_addr.S_un.S_un_b.s_b3,
		destaddr.sin_addr.S_un.S_un_b.s_b4);

	m_pStream->Init();
	m_pStream->TryToConnect(szIP, nPort);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ������������URL���HTTP����ͷ
//-----------------------------------------------------------------------------
INT HttpSocket::FormatRequestHeader(const char *pServer,const char *pObject, INT64 nFrom)
{
	ZeroMemory(m_szRequestHeader, sizeof(m_szRequestHeader));

	// ����,�����·��,�汾
	strcat(m_szRequestHeader,"GET ");
	strcat(m_szRequestHeader,pObject);
	strcat(m_szRequestHeader," HTTP/1.1");
	strcat(m_szRequestHeader,"\r\n");

	// ����
	strcat(m_szRequestHeader,"Host:");
	strcat(m_szRequestHeader,pServer);
	strcat(m_szRequestHeader,"\r\n");

	// ���յ���������
	strcat(m_szRequestHeader,"Accept:*/*");
	strcat(m_szRequestHeader,"\r\n");

	// ���������
	strcat(m_szRequestHeader,"User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
	strcat(m_szRequestHeader,"\r\n");

	// ��������,����
	strcat(m_szRequestHeader,"Connection:Keep-Alive");
	strcat(m_szRequestHeader,"\r\n");

	if( !m_bCache )
	{
		strcat(m_szRequestHeader,"Cache-Control: no-cache");
		strcat(m_szRequestHeader,"\r\n");
	}

	// �����������ʼ�ֽ�λ��(�ϵ������Ĺؼ�)
	if( nFrom > 0 )
	{
		char szTemp[20];
		strcat(m_szRequestHeader,"Range: bytes=");
		_i64toa(nFrom,szTemp,10);
		strcat(m_szRequestHeader,szTemp);
		strcat(m_szRequestHeader,"-");
		strcat(m_szRequestHeader,"\r\n");
	}

	// ������
	strcat(m_szRequestHeader,"\r\n");

	///���ؽ��
	return strlen(m_szRequestHeader);
}

//-----------------------------------------------------------------------------
///��������ͷ
//-----------------------------------------------------------------------------
BOOL HttpSocket::SendRequest(const char *pRequestHeader, long Length)
{
	if( !m_pStream->IsConnected() )
		return FALSE;

	m_pStream->Send((LPVOID)pRequestHeader, Length);
	return TRUE;
}

//-----------------------------------------------------------------------------
//��ȡHTTP����ķ���ͷ
//-----------------------------------------------------------------------------
BOOL HttpSocket::GetResponseHeader()
{
	INT nIndex = 0;
	ZeroMemory(m_szResponseHeader, sizeof(m_szResponseHeader));
	DWORD dwLastRecvTime = timeGetTime();
	while( nIndex < (sizeof(m_szResponseHeader)/sizeof(TCHAR)) )
	{
		DWORD dwMsgSize = 0;
		LPBYTE pRecv = m_pStream->Recv(dwMsgSize);
		if( !pRecv )
		{
			if( !m_pStream->IsConnected() || timeGetTime() - dwLastRecvTime > RECV_TIME_OUT*2)
				return FALSE;

			::WaitForSingleObject(m_pStream->GetRecvEvent(), 120);

			if( m_bTerminateThread )
				return FALSE;

			continue;
		}

		dwLastRecvTime = timeGetTime();
		memcpy(&m_szResponseHeader[nIndex], pRecv, dwMsgSize);
		m_pStream->FreeRecved(pRecv);

		nIndex += dwMsgSize;
		if(nIndex >= 4)
		{
			if( m_szResponseHeader[nIndex - 4] == '\r' 
				&& m_szResponseHeader[nIndex - 3] == '\n'
				&& m_szResponseHeader[nIndex - 2] == '\r'
				&& m_szResponseHeader[nIndex - 1] == '\n')
				break;
		}
		
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
INT64 HttpSocket::GetHttpFileSize()
{
	LPCSTR szContent1 = "content-range: bytes";
	LPCSTR szContent2 = "content-length";
	CStringA strValue = "-1";
	

	//ȡ��ĳ����ֵ
	CStringA strRespons = m_szResponseHeader;
	strRespons.MakeLower();

	int nPos = -1;
	nPos = strRespons.Find(szContent1, 0);
	if( nPos != -1 )	// �ҵ���
	{
		int nPos1 = strRespons.Find("/",nPos);
		int nPos2 = strRespons.Find("\r\n",nPos);
		strValue = strRespons.Mid(nPos1+1, nPos2-nPos1-1);
	}
	else
	{
		nPos = strRespons.Find(szContent2, 0);
		if( nPos != -1 )
		{
			int nPos1 = strRespons.Find(":",nPos);
			int nPos2 = strRespons.Find("\r\n",nPos);
			strValue = strRespons.Mid(nPos1+1, nPos2-nPos1-1);
		}
	}

	return _atoi64(strValue);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL HttpSocket::GetNewLocation(CStringA& strLocation)
{
	LPCSTR szContent = "location:";

	//ȡ����
	CStringA strRespons = m_szResponseHeader;
	strRespons.MakeLower();
	int nPos = -1;
	nPos = strRespons.Find(szContent, 0);
	if( nPos == -1 )	// û�ҵ�
		return FALSE;

	int nPos1 = strRespons.Find(":",nPos);
	int nPos2 = strRespons.Find("\r\n",nPos);
	strLocation = strRespons.Mid(nPos1+2, nPos2-nPos1-2);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ����Ŀ���ļ���
//-----------------------------------------------------------------------------
BOOL HttpSocket::ParseDestFileName(LPCTSTR szObjName)
{
	CString strTemp = szObjName;
	INT nPos = strTemp.ReverseFind(_T('/'));
	if( nPos == -1 )
		nPos = 0;
	else
		nPos += 1;

	m_strDestFileName = _T("launcher\\") + strTemp.Mid(nPos, strTemp.GetLength());
	return TRUE;
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int HttpSocket::GetServerState()
{
	char szState[3];
	szState[0] = m_szResponseHeader[9];
	szState[1] = m_szResponseHeader[10];
	szState[2] = m_szResponseHeader[11];

	return atoi(szState);
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
BOOL HttpSocket::Download(LPCTSTR szSrcFileName, BOOL bCache)
{
	m_bResult = FALSE;
	m_strSrcFileName = szSrcFileName;
	m_bCache = bCache;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&HttpSocket::DownloadThread), 0, 0, NULL);

	return TRUE;
}



//-----------------------------------------------------------------------------
// �����߳�
//-----------------------------------------------------------------------------
UINT HttpSocket::DownloadThread(void *pArg)
{
	DWORD dwServiceType;
	INTERNET_PORT nPort;
	CString strServer,strObject;
	std::vector<tstring> vecFileNames;
	m_pUtil->StringToToken(vecFileNames, m_strSrcFileName, _T(','));
	if( vecFileNames.empty() )
		return 0;

	INT nVecIndex = 0;
	m_strSrcFileName = vecFileNames[nVecIndex].c_str();

_download_thread_start:
	m_DownloadFile.Abort();
	TCHAR mess[256];
	_tcscpy(mess, m_strSrcFileName);
	AfxParseURL(mess, dwServiceType, strServer, strObject, nPort);
	ParseDestFileName(strObject);
	std::string strServerA = m_pUtil->UnicodeToAnsi(strServer);
	std::string strObjectA = m_pUtil->UnicodeToAnsi(strObject);

	BOOL bFileOpen = m_DownloadFile.Open(m_strDestFileName, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	if( bFileOpen )
		m_n64CompletedSize = m_DownloadFile.GetLength();

	long nLength = this->FormatRequestHeader(strServerA.c_str(),strObjectA.c_str(), m_n64CompletedSize);	
	ShowLog(m_pUtil->AnsiToUnicode(m_szRequestHeader));

	ShowInfo(g_StrTable[_T("TryConnect")]);
	this->Connect(strServerA.c_str(), nPort);
	while(!m_pStream->IsConnected())
	{
		if( !m_pStream->IsTryingToConnect() )
		{
			ShowInfo(g_StrTable[_T("ConnectFail")]);
			goto _download_thread_retry;
		}
		Sleep(10);

		if( m_bTerminateThread )
			goto _download_thread_exit;
	}

	this->SendRequest(m_szRequestHeader, nLength);	// ������������

	{
		CString strMsg = g_StrTable[_T("WaitForDownload")];
		strMsg += m_strSrcFileName;
		ShowInfo((LPCTSTR)strMsg);
	}

	if( FALSE == this->GetResponseHeader() )
	{
		if( m_bTerminateThread )
			goto _download_thread_exit;

		m_pStream->Disconnect();
		goto _download_thread_retry;
	}

	ShowLog(m_pUtil->AnsiToUnicode(m_szResponseHeader));

	INT nServerState = this->GetServerState();
	if( nServerState > 299 && nServerState < 399 )	// ��Ҫ�ض���
	{
		CStringA strNewLocation ;
		if( this->GetNewLocation(strNewLocation) )
		{
			m_strSrcFileName = m_pUtil->AnsiToUnicode(strNewLocation);
			m_pStream->Disconnect();
			goto _download_thread_start;
		}
	}

	m_n64FileSize = this->GetHttpFileSize();
	if( m_n64FileSize == m_n64CompletedSize )
		goto _download_thread_ok;	// �Ѿ��������

	if( m_n64FileSize == -1 || nServerState > 399 )	// �Ҳ����ļ���������߷��������ش���״̬
	{
		if( nServerState == 416 ) // �������뷶Χ
		{
			m_bResult = TRUE;
			goto _download_thread_ok;	// �Ѿ��������
		}

		m_pStream->Disconnect();
		goto _download_thread_retry;
	}

	if( !bFileOpen )
	{
		ShowInfo(g_StrTable[_T("WriteFail")]);
		return 0;
	}
	else
	{
		// �����̿ռ�
		INT64 n64Need = m_n64FileSize - m_n64CompletedSize + 4096;
		while( n64Need > m_pUtil->GetDriveFreeSpace(NULL) )
		{
			CString msg;
			msg.Format(_T("%s %I64dbytes, %s"),g_StrTable[_T("NotEnoughSpace")], n64Need, g_StrTable[_T("PleaseFreeSpace")]);
			INT nRet = ::MessageBox(NULL, msg, g_StrTable[_T("Warning")], MB_RETRYCANCEL);
			if( IDCANCEL == nRet )
			{
				_exit(0);	// ǿ���˳�
				goto _download_thread_exit;
			}
		}
	}

	{
		CString msg;
		msg.Format(g_StrTable[_T("Downloading")], m_strDestFileName);
		ShowInfo((LPCTSTR)msg);

		if( m_n64FileSize > INT_MAX )	// ������ļ�(����2G)�����
			PostMessage(m_hWndMain, WM_USER_PROGRESS_RANGE, (WPARAM)(INT)(m_n64FileSize/100), 0);
		else
			PostMessage(m_hWndMain, WM_USER_PROGRESS_RANGE, (WPARAM)(INT)m_n64FileSize, 0);
	}
	
	m_DownloadFile.Seek(m_n64CompletedSize, CFile::begin);
	m_pStream->SetRecv(m_n64FileSize-m_n64CompletedSize);

	DWORD dwLastRecvTime = timeGetTime();
	DWORD dwLastShowTime = timeGetTime();
	while(m_n64CompletedSize < m_n64FileSize)
	{
		if(timeGetTime() - dwLastShowTime > 50 )	// ��ʾ��Ϣ
		{
			dwLastShowTime = timeGetTime();
			INT nLeft = (INT)((m_n64FileSize - m_n64CompletedSize)/1024);
			PostMessage(m_hWndMain, WM_USER_DOWNLOAD_SPEED, m_pStream->GetBytesFlowIn(), nLeft);
			if( m_n64FileSize > INT_MAX )	// ������ļ�(����2G)�����
				PostMessage(m_hWndMain, WM_USER_PROGRESS_POS, (WPARAM)(INT)(m_n64CompletedSize/100), 0);
			else
				PostMessage(m_hWndMain, WM_USER_PROGRESS_POS, (WPARAM)(INT)m_n64CompletedSize, 0);
		}

		DWORD dwRecvSize = 0;
		LPBYTE pRecv = m_pStream->Recv(dwRecvSize);
		if(!pRecv)
		{
			if( !m_pStream->IsConnected() || timeGetTime() - dwLastRecvTime > RECV_TIME_OUT )	// �Ƿ񻹴�������״̬
			{
				ShowInfo(g_StrTable[_T("AbnormalDisconnect")]);
				m_pStream->Disconnect();
				m_DownloadFile.Close();
				goto _download_thread_retry;
			}

			if( m_bTerminateThread )
				goto _download_thread_exit;

			::WaitForSingleObject(m_pStream->GetRecvEvent(), 120);
			continue;
		}

		m_DownloadFile.Write(pRecv,dwRecvSize);
		m_n64CompletedSize += dwRecvSize;
		dwLastRecvTime = timeGetTime();


		m_pStream->FreeRecved(pRecv);
	}

_download_thread_ok:
	if( m_n64FileSize > INT_MAX )	// ������ļ�(����2G)�����
		PostMessage(m_hWndMain, WM_USER_PROGRESS_POS, (WPARAM)(INT)(m_n64FileSize/100), 0);
	else
		PostMessage(m_hWndMain, WM_USER_PROGRESS_POS, (WPARAM)(INT)m_n64FileSize, 0);
	ShowLog(_T("download ok\r\n"));
	m_bResult = TRUE;
	m_DownloadFile.Abort();
	m_pStream->Disconnect();
	return 0;

_download_thread_retry:
	m_DownloadFile.Abort();
	m_pStream->Disconnect();

	// �Ƿ� ���������ĵ�ַ���Գ���
	if( (INT)vecFileNames.size() > nVecIndex + 1)
	{
		nVecIndex++;
		m_strSrcFileName = vecFileNames[nVecIndex].c_str();
	}
	else
	{
		// �ӵ�һ����ַ���¿�ʼ��������
		nVecIndex = 0;
		m_strSrcFileName = vecFileNames[nVecIndex].c_str();
	}
	goto _download_thread_start;


_download_thread_exit:
	m_DownloadFile.Abort();
	m_pStream->Disconnect();
	return 0;

}


//-----------------------------------------------------------------------------
// ��ʾ LOG
//-----------------------------------------------------------------------------
VOID HttpSocket::ShowLog( LPCTSTR szText )
{
	_tcscpy(m_szShowLog, szText);
	PostMessage(m_hWndMain, WM_USER_LOG, (WPARAM)m_szShowLog, 0);
}


//-----------------------------------------------------------------------------
// ��ʾ info
//-----------------------------------------------------------------------------
VOID HttpSocket::ShowInfo( LPCTSTR szText )
{
	_tcscpy(m_szShowInfo, szText);
	PostMessage(m_hWndMain, WM_USER_INFO, (WPARAM)m_szShowInfo, 0);
}