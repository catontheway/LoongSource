//-----------------------------------------------------------------------------
//!\file HttpDownload.h
//!\author Lyp
//!
//!\date 2003-03-17
//! last 2009-04-27
//!
//!\brief Http����
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

class VENGINE_API HttpDownload
{
public:

	BOOL		Download(LPCTSTR szSrcFileName, BOOL bCache=TRUE);
	LPCTSTR		GetDestFileName() { return m_strDestFileName.c_str(); }
	DWORD		WaitForEnd(DWORD dwTime) { return ::WaitForSingleObject(m_hThread, dwTime); }

	VOID		Terminate() { m_bTerminateThread = TRUE; }
	BOOL		GetResult() { return m_bResult; }

	INT64		GetFileSize() { return m_n64FileSize; }
	INT64		GetCompletedSize() { return m_n64CompletedSize; }

	HttpDownload();
	~HttpDownload();


protected:	

	TSFPTrunk<HttpDownload>	m_Trunk;
	TObjRef<DiscIOMgr>		m_pDiscIOMgr;
	CHAR					m_szRequestHeader[4096];		// ����ͷ
	CHAR					m_szResponseHeader[4096];		// ��Ӧͷ

	Util*					m_pUtil;
	StreamTransport*		m_pStream;

	tstring					m_strSrcFileName;
	tstring					m_strDestFileName;

	DWORD					m_dwFileHandle;
	INT64					m_n64CompletedSize;
	INT64					m_n64FileSize;

	HANDLE					m_hThread;
	BOOL					m_bTerminateThread;
	BOOL					m_bResult;
	BOOL					m_bCache;


	INT		GetServerState();	// ���ط�����״̬�� -1��ʾ���ɹ�
	INT64	GetHttpFileSize();		// �����ļ����� -1��ʾ���ɹ�
	BOOL	GetNewLocation(std::string &strLocation);

	BOOL	GetResponseHeader(); // ��ȡ����ͷ
	INT		FormatRequestHeader(LPCSTR pServer, LPCSTR pObject, INT64 nFrom=0);	//��ʽ������ͷ

	BOOL	SendRequest(LPCSTR pRequestHeader=NULL, long Length=0);
	BOOL	Connect(LPCSTR szHostName, INT nPort=80);

	UINT	DownloadThread(LPVOID pArg);

	BOOL	ParseDestFileName(LPCTSTR szObjName);	// ����Ŀ���ļ���

	BOOL	AfxParseURL(LPCTSTR pstrURL, DWORD& dwServiceType, tstring& strServer, 
		tstring& strObject, INTERNET_PORT& nPort);
	BOOL	_AfxParseURLWorker(LPCTSTR pstrURL, LPURL_COMPONENTS lpComponents, 
		DWORD& dwServiceType, INTERNET_PORT& nPort, DWORD dwFlags);
};

}