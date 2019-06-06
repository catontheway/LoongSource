//-----------------------------------------------------------------------------
// File: mini_dump
// Auth: Lyp
// Date: 2003/12/11
// Last: 2004/3/7
// Desc: ��¼�쳣�����Ļ���
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "mini_dump.h"

// ʹ����������
#include "..\resource\resource.h"
#include "..\system\sysinfo.h"
#include "..\system\disciomgr.h"
#include "xzip.h"
#include "..\network\ftpclient.h"
#include "..\network\stream_transport.h"
#include <psapi.h>		// for EnumProcessModules


namespace vEngine {
TCHAR	Exception::m_szDumpFileName[1024] = _T("log\\LastCrash.dmp");
TCHAR	Exception::m_szSendLogIP[256] = {0};
TCHAR	Exception::m_szSendLogUser[256] = {0};
TCHAR	Exception::m_szSendLogPassword[256] = {0};

//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
Exception::Exception(UINT unCode, EXCEPTION_POINTERS* pExp)
{
	TCHAR mess[1024];
	TCHAR szFileName[1024];

	HANDLE hFile = ::CreateFile(m_szDumpFileName, GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;
	MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
	MiniDumpExceptionInfo.ExceptionPointers = pExp;
	MiniDumpExceptionInfo.ClientPointers = FALSE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,  MiniDumpNormal , 
		&MiniDumpExceptionInfo, NULL, NULL);

	_tcsncpy(szFileName, m_szDumpFileName, 1024);

	// �ȼ��log�ļ��Ƿ����
	DWORD dwSize = ::GetFileSize(hFile, NULL);
	::CloseHandle(hFile);
	if( dwSize == 0 || dwSize == GT_INVALID )
		return;

	// ��ȥ�ļ���չ��
	LPTSTR pResult = _tcsrchr(szFileName, _T('.'));
	if( pResult )
		_stprintf(pResult, _T(".zip"));

	HZIP hZip = NULL;
	hZip = CreateZip((LPVOID)(LPCTSTR)szFileName, 0, ZIP_FILENAME);
	if( !hZip )
		return;

	// zip is open, add file
	ZRESULT zr = ZipAdd(hZip, _T("Crash.dmp"), (LPVOID)m_szDumpFileName, 0, ZIP_FILENAME);
	if( zr != ZR_OK )
		return;

	CloseZip(hZip);

	// ɾ��ԭ�ļ�
	::DeleteFile( m_szDumpFileName );

	LoadString(g_hInstance, IDS_ERROR, mess, 1024);	// �Ƿ���
	if( ::MessageBox(NULL, mess, _T("Critical Error"), MB_APPLMODAL|MB_ICONERROR|MB_OKCANCEL|MB_DEFBUTTON1) == IDCANCEL )
		return;

	if( !m_szSendLogIP[0] )
		return;


	// ����Ŀ���ļ���
	TCHAR szDest[1024];
	LPTSTR pszDest = _tcsrchr(szFileName, _T('\\'));
	if( !pszDest )
		pszDest = szFileName;
	else
		pszDest++;

	_tcsncpy(szDest, pszDest, 1024);

	// FTP����
	TObjRef<StreamTransport>()->Init();

	nsFTP::CFTPClient ftpClient;
	nsFTP::CLogonInfo logonInfo(m_szSendLogIP, 21, m_szSendLogUser, m_szSendLogPassword);

	if( ftpClient.Login(logonInfo) )
	{
		ftpClient.UploadFile(szFileName, szDest);
		ftpClient.Logout();
	}

	return;
}



//-----------------------------------------------------------------------------
// ���������Log��Ϣ
//-----------------------------------------------------------------------------
VOID Exception::Init(LPCTSTR szIP, LPCTSTR szUser, LPCTSTR szPassword, LPCTSTR szVersion)
{
	// ���ɺ��ʵ�dump�ļ���
	TCHAR szTime[MAX_PATH], szTemp[MAX_PATH];
	GetModuleFileName(NULL, szTemp, MAX_PATH);

	// ��ȥ·��
	TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
	pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;
	_stprintf(m_szDumpFileName, _T("log\\%s"), pResult);
	// ��ȥ�ļ���չ��
	pResult = _tcsrchr(m_szDumpFileName, _T('.'));
	if( pResult )
		*pResult = '\0';	
	// ����ʱ��
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = _T('\0');
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// �������Լ�ʱ����ͽ���id
		wsprintf(szTime, _T("[%d-%d-%d %02d%02d%02d %05d]crash.dmp"),
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
			GetCurrentProcessId());

		if( szVersion )
			_tcscat(m_szDumpFileName, szVersion);

		_tcscat(m_szDumpFileName, szTime);
	}

	_tcscpy(m_szSendLogIP, szIP);
	_tcscpy(m_szSendLogUser, szUser);
	_tcscpy(m_szSendLogPassword, szPassword);
}









} // namespace vEngine {











