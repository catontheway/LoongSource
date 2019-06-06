// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

/*
#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif
*/
#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���


#pragma warning(disable:4355)// Level 3: "this": ���ڻ���Ա��ʼֵ�趨���б�
// �����̺߳�������
typedef UINT (WINAPI* THREADPROC)(LPVOID);


INT ErrMsg(LPCTSTR lpFormat, ...);

#define IMSG ErrMsg

#define X_HUGE_STRING		512
#define X_LONG_STRING		256
#define X_SHORT_STRING		32
#define GT_INVALID				(-1)
#define GT_VALID(n)				(((INT)(n)) != GT_INVALID)
#define P_VALID(n)				( (((INT)(n)) != GT_INVALID) && ((n) != NULL) )

#define GT_SEEK_SET				0
#define GT_SEEK_CUR				1
#define GT_SEEK_END				2

#ifndef SAFE_DEL
#define SAFE_DEL(p)				{ if(p) { delete(p);		(p) = NULL; } }
#endif
#ifndef SAFE_DEL_ARRAY
#define SAFE_DEL_ARRAY(p)		{ if(p) { delete[](p);		(p) = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p) = NULL; } }
#endif
#ifndef SAFE_CLOSE_HANDLE
#define SAFE_CLOSE_HANDLE(h)	{ if(h) { CloseHandle(h);	(h) = NULL; } }
#endif
#ifndef ASSERT
#define ASSERT(f)				assert((f))
#endif

CONST INT GT_NET_BLOCK_TIME = 1000*150;	// 150 ms

#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>	// for _beginthreadex
#include <mmsystem.h>
#include <tchar.h>
#include <assert.h>
#include <afxinet.h>


#include <sstream>
#include <vector>
#include <list>
#include <map>

#include "sfp_trunk.h"
#include "msg_queue_ts.h"


#ifndef tstring
#ifdef _UNICODE
#define tstring std::wstring
#define tstringstream std::wstringstream
#define tstreambuf std::wstreambuf
#else
#define tstring std::string
#define tstringstream std::stringstream
#define tstreambuf std::streambuf
#endif
#endif
typedef unsigned long int		UINT4;


#include "util.h"
#include "var_container.h"
#include "StringTable.h"


extern const unsigned long	globe_Crc32Table[256];

extern StringTable g_StrTable;

class HttpSocket;
class CPKVfs;
class CTSMemMgr;


