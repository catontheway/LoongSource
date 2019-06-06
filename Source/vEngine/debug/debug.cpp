//-----------------------------------------------------------------------------
// File: debug
// Desc: Game Tool debug 2.0
// Auth: Lyp
// Date: 2003/12/11
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "debug.h"

// ʹ����������
#include "exception.h"


namespace vEngine {
//-----------------------------------------------------------------------------
// ��ʾһ�������Ϣ������ȡϵͳ������Ϣ
//-----------------------------------------------------------------------------
INT Debug::ErrMsg(LPCTSTR lpFormat, ...)
{
	TCHAR szBuf[1024];
	LPVOID lpMsgBuf;
	DWORD dwErr;
	INT nResult;

	lpMsgBuf = NULL;

	// ���Ȼ��ϵͳ�������
	dwErr = ::GetLastError();

	va_list argptr;
	va_start(argptr, lpFormat);
	_vstprintf_s(szBuf, sizeof(szBuf)/sizeof(TCHAR), lpFormat, argptr);
	va_end(argptr);

	if( dwErr != 0 )	// ��ϵͳ����õ�������Ϣ
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM 
			| FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		wsprintf(szBuf, _T("%s\r\nSystemCode:%lu Info:%s"), szBuf, dwErr, (CONST PTCHAR)lpMsgBuf);
		LocalFree( lpMsgBuf );
	}

	lstrcat(szBuf, _T("\r\nContinue?"));
	nResult = ::MessageBox(NULL, szBuf, _T(" Message:"), MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON1);
	if( IDNO == nResult )	// �����׳�һ���ṹ���쳣���Ա��¼��ǰ��ջ����Ϣ
	{
		// Microsoft C++ exception code
		CONST UINT C_PLUS_PLUS_EXCEPTION	= 0xe06d7363;
		RaiseException(C_PLUS_PLUS_EXCEPTION, 0, 0, NULL);
	}

	return nResult;
}




}	// vEngine
