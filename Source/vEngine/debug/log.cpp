//-----------------------------------------------------------------------------
// File: log
// Desc: Game Tool log 1.0
// Auth: Lyp
// Date: 2003/12/18
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "log.h"

// ʹ����������
#include "..\system\disciomgr.h"

namespace vEngine {
#define MAX_LOG_SIZE 10240
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
Log::Log()
{
	m_dwFileHandle = INVALID_FILE_HANDLE;
}

Log::~Log()
{
	Close();
}


//-----------------------------------------------------------------------------
// ����log�ļ�
//-----------------------------------------------------------------------------
BOOL Log::Create(LPCTSTR szFileName)
{
	if( m_dwFileHandle != INVALID_FILE_HANDLE )
		m_pDiscIOMgr->Close( m_dwFileHandle );

	// ���û���ṩ�ļ���,��ʹ�ô���Ĭ���ļ���
	if( NULL == szFileName )
	{
		// ���ɺ��ʵ�log�ļ���
		TCHAR szTime[MAX_PATH], szTemp[MAX_PATH];
		GetModuleFileName(NULL, szTemp, MAX_PATH);

		// ��ȥ·��
		TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
		pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;
		_stprintf(m_szDefaultName, _T("log\\%s"), pResult);
		// ��ȥ�ļ���չ��
		pResult = _tcsrchr(m_szDefaultName, _T('.'));
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
			wsprintf(szTime, _T("[%d-%d-%d %02d%02d%02d %05d].log"),
					(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
					(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
					GetCurrentProcessId());
			_tcscat(m_szDefaultName, szTime);
		}
	
		szFileName = m_szDefaultName;
	}
	else
	{
		wsprintf(m_szDefaultName, szFileName);
	}


	m_dwFileHandle = m_pDiscIOMgr->Create(szFileName, GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);

	if( INVALID_FILE_HANDLE != m_dwFileHandle )
	{
		// add M$ BOM
		BYTE BOM[2] = {0xFF, 0xFE};
		DWORD dwWrite = 0;
		m_pDiscIOMgr->Write(m_dwFileHandle, BOM, sizeof(BOM), &dwWrite, NULL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//-----------------------------------------------------------------------------
// �ر��ļ�
//-----------------------------------------------------------------------------
VOID Log::Close()
{
	if( m_dwFileHandle != INVALID_FILE_HANDLE )
	{
		m_pDiscIOMgr->Close( m_dwFileHandle );
		m_dwFileHandle = INVALID_FILE_HANDLE;
	}
}

//-----------------------------------------------------------------------------
// ����򵥵���Ϣ
//-----------------------------------------------------------------------------
BOOL Log::Write(LPCTSTR lpFormat, ...)
{
	if( INVALID_FILE_HANDLE == m_dwFileHandle )
		return FALSE;

	TCHAR szBuf[MAX_LOG_SIZE];

	ZeroMemory(szBuf, sizeof(szBuf));
	va_list argptr = NULL;
	va_start(argptr, lpFormat);
	_vstprintf_s(szBuf, sizeof(szBuf)/sizeof(TCHAR), lpFormat, argptr);
	va_end(argptr);

	m_pDiscIOMgr->Seek( m_dwFileHandle, 0, GT_SEEK_END );
	return m_pDiscIOMgr->Write(m_dwFileHandle, szBuf, lstrlen(szBuf)*2);
}



//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
ExceptionLog::ExceptionLog()
{
	m_dwFileHandle = INVALID_FILE_HANDLE;
}

ExceptionLog::~ExceptionLog()
{
	Close();
}


//-----------------------------------------------------------------------------
// ����log�ļ�
//-----------------------------------------------------------------------------
BOOL ExceptionLog::Create()
{
	if( m_dwFileHandle != INVALID_FILE_HANDLE )
		m_pDiscIOMgr->Close( m_dwFileHandle );

	// ���ɺ��ʵ�log�ļ���
	TCHAR szTime[MAX_PATH], szTemp[MAX_PATH];
	GetModuleFileName(NULL, szTemp, MAX_PATH);

	// ��ȥ·��
	TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
	pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;
	_stprintf(m_szDefaultName, _T("log\\%s"), pResult);
	// ��ȥ�ļ���չ��
	pResult = _tcsrchr(m_szDefaultName, _T('.'));
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
		wsprintf(szTime, _T("[%d-%d-%d %02d%02d%02d %05d]crash.log"),
				(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
				(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
				GetCurrentProcessId());
		_tcscat(m_szDefaultName, szTime);
	}
	
	m_dwFileHandle = m_pDiscIOMgr->Create(m_szDefaultName, GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);

	return ( INVALID_FILE_HANDLE != m_dwFileHandle );
}


//-----------------------------------------------------------------------------
// �ر��ļ�
//-----------------------------------------------------------------------------
VOID ExceptionLog::Close()
{
	if( m_dwFileHandle != INVALID_FILE_HANDLE )
	{
		m_pDiscIOMgr->Close( m_dwFileHandle );
		m_dwFileHandle = INVALID_FILE_HANDLE;
	}
}


//-----------------------------------------------------------------------------
// ����򵥵���Ϣ
//-----------------------------------------------------------------------------
BOOL ExceptionLog::Write(LPCSTR lpFormat, ...)
{
	if( INVALID_FILE_HANDLE == m_dwFileHandle )
		return FALSE;

	CHAR szBuf[MAX_LOG_SIZE];
	ZeroMemory(szBuf, sizeof(szBuf));
	va_list argptr = NULL;
	va_start(argptr, lpFormat);
	vsprintf_s(szBuf, MAX_LOG_SIZE, lpFormat, argptr);
	va_end(argptr);

	m_pDiscIOMgr->Seek( m_dwFileHandle, 0, GT_SEEK_END );
	return m_pDiscIOMgr->Write(m_dwFileHandle, szBuf, strlen(szBuf));
}
} // namespace vEngine {



