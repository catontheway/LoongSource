#pragma once


class Util;
class StreamTransport;
class DownloadInfo;

class HttpSocket  
{
public:

	BOOL		Download(LPCTSTR szSrcFileName, BOOL bCache=TRUE);
	LPCTSTR		GetDestFileName() { return m_strDestFileName; }
	DWORD		WaitForEnd(DWORD dwTime) { return ::WaitForSingleObject(m_hThread, dwTime); }

	VOID		Terminate() { m_bTerminateThread = TRUE; }
	BOOL		GetResult() { return m_bResult; }

	INT64		GetFileSize() { return m_n64FileSize; }
	INT64		GetCompletedSize() { return m_n64CompletedSize; }

	VOID		SetWndMain(HWND hWnd) { m_hWndMain = hWnd; }


	HttpSocket();
	virtual ~HttpSocket();


protected:	

	int		GetServerState();	// ���ط�����״̬�� -1��ʾ���ɹ�
	INT64	GetHttpFileSize();		// �����ļ����� -1��ʾ���ɹ�
	BOOL	GetNewLocation(CStringA& strLocation);
	
	BOOL	GetResponseHeader(); // ��ȡ����ͷ
	INT		FormatRequestHeader(const char *pServer,const char *pObject, INT64 nFrom=0);	//��ʽ������ͷ

	BOOL	SendRequest(const char* pRequestHeader = NULL,long Length = 0);
	BOOL	Connect(const char* szHostName,int nPort=80);

	UINT	DownloadThread(void *pArg);

	BOOL	ParseDestFileName(LPCTSTR szObjName);	// ����Ŀ���ļ���

	VOID	ShowLog(LPCTSTR	szText);
	VOID	ShowInfo(LPCTSTR szText);


	TSFPTrunk<HttpSocket>	m_Trunk;
	char					m_szRequestHeader[2048];		// ����ͷ
	char					m_szResponseHeader[2048];		// ��Ӧͷ

	Util*					m_pUtil;
	StreamTransport*		m_pStream;
	
	HWND					m_hWndMain;
	CString					m_strSrcFileName;
	CString					m_strDestFileName;

	CFile					m_DownloadFile;
	INT64					m_n64CompletedSize;
	INT64					m_n64FileSize;

	TCHAR					m_szShowInfo[1024*32];
	TCHAR					m_szShowLog[1024*32];

	HANDLE					m_hThread;
	volatile	BOOL		m_bTerminateThread;
	BOOL					m_bResult;
	BOOL					m_bCache;
};