//-----------------------------------------------------------------------------
// File: Util.cpp
// Desc: Game System Util 2.0
// Auth: Lyp
// Date: 2003/11/13
//
// Copyright (c) 1985-2007 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "util.h"


#define CHAR_POOL_SIZE	1024*64
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
Util::Util():m_dwIDHolder(1),m_nRandHolder(0),m_nRandCallCount(0)
{
	InitializeCriticalSection(&m_Lock);
	m_pCharPool = new CHAR[CHAR_POOL_SIZE];
	m_pTCharPool = new TCHAR[CHAR_POOL_SIZE];
}

Util::~Util()
{
	DeleteCriticalSection(&m_Lock);
	SAFE_DEL_ARRAY(m_pCharPool);
	SAFE_DEL_ARRAY(m_pTCharPool);
}

//-----------------------------------------------------------------------------
// �õ���һ�����е�ID
//-----------------------------------------------------------------------------
DWORD Util::GetFreeID()
{
	if( m_dwIDHolder >= 0x7fffffff )	// �������Ӧ�ú��ѷ���
		m_dwIDHolder = 1;
	return m_dwIDHolder++;
}


//-----------------------------------------------------------------------------
// �ø�������������ӳ�ʼ�������������
//-----------------------------------------------------------------------------
VOID Util::Randomize(DWORD dwSeed)
{
	m_nRandHolder = (INT)dwSeed;
	m_nRandCallCount = 0;
}


//-----------------------------------------------------------------------------
// �õ������
//-----------------------------------------------------------------------------
INT Util::Rand()
{
	EnterCriticalSection(&m_Lock);
	m_nRandCallCount++;
	INT nR = (((m_nRandHolder = m_nRandHolder * 214013L + 2531011L) >> 16) & 0x7fff);
	LeaveCriticalSection(&m_Lock);
	return nR;
}


//-----------------------------------------------------------------------------
// �������ʰٷ��������Ƿ�����
//-----------------------------------------------------------------------------
BOOL Util::Probability(INT nProbCent)
{
	INT nTemp = 0, nMax = 0;
	if( nProbCent<=0 )
		return FALSE;

	nMax = (INT)(((FLOAT)UTIL_PROBABILITY_MOD) / 100.0f * (FLOAT)nProbCent);
	nTemp = (this->Rand() % UTIL_PROBABILITY_MOD);
	if (nTemp < nMax)
		return TRUE;
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// ����������ֵ
//-----------------------------------------------------------------------------
FLOAT Util::Fluctuate(FLOAT fNum, FLOAT fIncPercent, FLOAT fDecPercent)
{
	FLOAT fMax = fNum * ( 100.0f + fIncPercent ) / 100.0f;
	FLOAT fMin = fNum * ( 100.0f - fDecPercent ) / 100.0f;
	FLOAT fResult = fMin;
	if( (INT)(fMax-fMin) == 0 )
		fResult = fMin;
	else
		fResult = fMin + (FLOAT)(Rand() % ((INT)(fMax - fMin)));

    return fResult;
}




//-----------------------------------------------------------------------------
// IntersectionRect ��Win98����ϵͳ����BUG, ����������дһ��
//-----------------------------------------------------------------------------
BOOL Util::RectIntersection(RECT& rcOut, RECT rc1, RECT rc2)
{
	if (rc1.left > rc2.right || rc2.left > rc1.right 
		|| rc1.top > rc2.bottom || rc2.top > rc1.bottom)
		return FALSE;	// �����β��ཻ

	// ��������ཻ����
	rcOut.left = rc1.left > rc2.left ? rc1.left : rc2.left;
	rcOut.top = rc1.top > rc2.top ? rc1.top : rc2.top;
	rcOut.right = rc1.right < rc2.right ? rc1.right : rc2.right;
	rcOut.bottom = rc1.bottom < rc2.bottom ? rc1.bottom : rc2.bottom;
	return TRUE;
}


//-----------------------------------------------------------------------------
// ����32λCRC,��һ���㷨���ٺܶ�
//-----------------------------------------------------------------------------
DWORD Util::Crc32(LPCSTR lpData)
{
	UINT unResult;

	//��ASC����127���ַ��� ������unsigned char, ��������
	LPBYTE pData = (LPBYTE)lpData;
	if( 0 == pData[0] ) 
		return (DWORD)GT_INVALID;

	unResult = *pData++ << 24;
	if( *pData )
	{
		unResult |= *pData++ << 16;
		if( *pData )
		{
			unResult |= *pData++ << 8;
			if( *pData ) unResult |= *pData++;
		}
	}
	unResult = ~ unResult;

	while( *pData )
	{
		unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
		pData++;
	}

	return ~unResult;
}



//-----------------------------------------------------------------------------
// ����32λCRC,��һ���㷨���ٺܶ�
//-----------------------------------------------------------------------------
DWORD Util::Crc32(LPCTSTR pData)
{
	INT nByteCount = _tcslen(pData)*sizeof(TCHAR);
	LPBYTE pByte = (LPBYTE)pData;

    UINT unResult;
	if( nByteCount <= 0 )
		return (DWORD)GT_INVALID;

    unResult = *pByte++ << 24;
    if( --nByteCount > 0 )
    {
        unResult |= *pByte++ << 16;
        if( --nByteCount > 0 )
        {
            unResult |= *pByte++ << 8;
            if( --nByteCount > 0 ) unResult |= *pByte++;
        }
    }
    unResult = ~ unResult;
    
    while( --nByteCount > 0 )
    {
        unResult = (unResult << 8 | *pByte) ^ globe_Crc32Table[unResult >> 24];
        pByte++;
    }
    
    return ~unResult;
}

//-----------------------------------------------------------------------------
// ����32λCRC,��һ���㷨���ٺܶ�
//-----------------------------------------------------------------------------
DWORD Util::Crc32(LPBYTE pData, INT nByteCount)
{
    UINT unResult;
	if( nByteCount <= 0 )
		return (DWORD)GT_INVALID;

    unResult = *pData++ << 24;
    if( --nByteCount > 0 )
    {
        unResult |= *pData++ << 16;
        if( --nByteCount > 0 )
        {
            unResult |= *pData++ << 8;
            if( --nByteCount > 0 ) unResult |= *pData++;
        }
    }
    unResult = ~ unResult;
    
    while( --nByteCount > 0 )
    {
        unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
        pData++;
    }
    
    return ~unResult;
}



//-----------------------------------------------------------------------------
// ��unicodeת��unicode8
//-----------------------------------------------------------------------------
LPCSTR Util::UnicodeToUnicode8(LPCTSTR szSrc)
{
	m_pCharPool[0] = 0;
	WideCharToMultiByte(CP_UTF8, 0, szSrc, -1, m_pCharPool, CHAR_POOL_SIZE, NULL, NULL);
	return m_pCharPool;
}

//-----------------------------------------------------------------------------
// ��unicodeת��ANSI
//-----------------------------------------------------------------------------
LPCSTR Util::UnicodeToAnsi(LPCTSTR szSrc)
{
	m_pCharPool[0] = 0;
	WideCharToMultiByte(CP_ACP, 0, szSrc, -1, m_pCharPool, CHAR_POOL_SIZE, NULL, NULL);
	return m_pCharPool;
}

//-----------------------------------------------------------------------------
// ��unicode8ת��unicode
//-----------------------------------------------------------------------------
LPCTSTR	Util::Unicode8ToUnicode(LPCSTR szSrc)
{
	m_pTCharPool[0] = 0;
	MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, m_pTCharPool, CHAR_POOL_SIZE);
	return m_pTCharPool;
}

//-----------------------------------------------------------------------------
// ��ANSIת��unicode
//-----------------------------------------------------------------------------
LPCTSTR Util::AnsiToUnicode(LPCSTR szSrc)
{
	m_pCharPool[0] = 0;
	MultiByteToWideChar(CP_ACP, 0, szSrc, -1, m_pTCharPool, CHAR_POOL_SIZE);
	return m_pTCharPool;
}

//-----------------------------------------------------------------------------
// ȥ��tstring��β�Ŀո�
//-----------------------------------------------------------------------------
VOID Util::CutSpaceOfStringHeadAndTail(tstring& string)
{
	// ������:����ַ�����ֻ�пո�,���ܽ��д˲���
	if( string.find_first_not_of(_T(" ")) != -1 )
	{
		string.assign(string, string.find_first_not_of(_T(" ")),
			string.find_last_not_of(_T(" "))-string.find_first_not_of(_T(" "))+1);
	}
	else
	{
		string.clear();
	}
}


//-----------------------------------------------------------------------------
// �������ַ�תΪtoken;
//-----------------------------------------------------------------------------
VOID Util::StringToToken(std::vector<tstring>& token, LPCWSTR szString, WCHAR chSeparator)
{
	ASSERT(szString);
	tstring str;
	token.clear();

	bool bIntoQuotMark = false;	// �Ƿ�������Ų���
	for(LPCTSTR pStr = szString; *pStr; pStr++)
	{
		if( *pStr == chSeparator )
		{
			if( !str.empty() )
			{
				if( bIntoQuotMark )
					str.append(chSeparator, 1);	// ����������ķָ���
				else
				{
					token.push_back(str);
					str.clear();
				}
			}
		}
		else if( *pStr == _T('"') )
		{
			bIntoQuotMark = !bIntoQuotMark;
		}
		else
		{
			str.append(pStr, 1);
		}
	}

	if(!str.empty())	// ����ʣ����ַ�
		token.push_back(str);
}



//-----------------------------------------------------------------------------
// ���ע�����ָ����ֵ�Ƿ����
//-----------------------------------------------------------------------------
BOOL Util::CheckReg(HKEY key, LPCTSTR subKey)
{
	HKEY hKey;
	long ret0 = RegOpenKeyEx(key, subKey, 0, KEY_READ, &hKey);
	if ( ret0 == ERROR_SUCCESS )
	{
		RegCloseKey(hKey);
		return true;
	}

	RegCloseKey(hKey);
	return false;
}



//-----------------------------------------------------------------------------
// ��ָ����ֵд��ע�����ָ��·��,���ָ��·��û��,�򴴽���д��
//-----------------------------------------------------------------------------
BOOL Util::WriteReg(LPCTSTR KEY_ROOT/*ע����е�·��*/, LPCTSTR KEY_NAME, LPCTSTR sz)
{
	if( KEY_ROOT == NULL || KEY_NAME == NULL ) 
		return FALSE;

	HKEY hKey;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;

	long ret0 = 0;
	TCHAR buf[256]={0};
	if ( !CheckReg(HKEY_LOCAL_MACHINE, KEY_ROOT) )
	{
		ret0 = RegCreateKeyEx(HKEY_LOCAL_MACHINE, KEY_ROOT, 0, NULL,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
		if ( ret0 != ERROR_SUCCESS )
			return FALSE;

		RegCloseKey(hKey);
	}

	// -- д��
	RegOpenKeyEx( HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_WRITE, &hKey );
	ret0 = RegSetValueEx(hKey, KEY_NAME, NULL, REG_SZ, (const BYTE*)sz, lstrlen(sz)+sizeof(sz[0]));
	if ( ret0 != ERROR_SUCCESS )
		return FALSE;

	RegCloseKey(hKey);
	return TRUE;
}



//-----------------------------------------------------------------------------
// ��ȡע�����ָ��·���ļ�ֵ
//-----------------------------------------------------------------------------
BOOL Util::ReadReg(LPCTSTR KEY_ROOT, LPCTSTR KEY_NAME, DWORD& key_value)
{
	HKEY hKey;
	long ret0 = RegOpenKeyEx( HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_QUERY_VALUE, &hKey );
	if( ret0 != ERROR_SUCCESS )
		return FALSE;

	DWORD dwBufLen=32;
	RegQueryValueEx( hKey, KEY_NAME, NULL, NULL, (LPBYTE)&key_value, &dwBufLen);
	RegCloseKey(hKey);

	return TRUE;
}


//-----------------------------------------------------------------------------
// ��ע���
//-----------------------------------------------------------------------------
BOOL Util::ReadReg(LPCTSTR KEY_ROOT, LPCTSTR KEY_NAME, LPTSTR outstr)
{
	ASSERT( outstr != NULL );
	HKEY hKey;
	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_READ, &hKey);
	if ( ret0 != ERROR_SUCCESS )
	{
		outstr[0] = 0;
		return FALSE;
	}
	DWORD dwLen = MAX_PATH, dwType = REG_SZ;
	// if char* (LPBYTE)char*
	// if char [] (LPBYTE)&char
	RegQueryValueEx(hKey, KEY_NAME, NULL, &dwType, (LPBYTE)outstr, &dwLen);

	RegCloseKey(hKey);
	return TRUE;
}



//-------------------------------------------------------------------------------
// �õ����̵�ʣ��ռ�
//-------------------------------------------------------------------------------
INT64 Util::GetDriveFreeSpace(LPCTSTR szDriver)
{
	INT64 n64FreeBytesToUse = 0;
	typedef INT( WINAPI *GETDISKFREESPACE_PROC)(LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);
	GETDISKFREESPACE_PROC pGetDiskFreeSpaceEx = 
		(GETDISKFREESPACE_PROC)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetDiskFreeSpaceExA");

	if( pGetDiskFreeSpaceEx )
	{
		ULARGE_INTEGER TotalNumberOfBytes;
		ULARGE_INTEGER TotalNumberOfFreeBytes;

		BOOL bRet = (pGetDiskFreeSpaceEx)(szDriver,
			(PULARGE_INTEGER)&n64FreeBytesToUse,
			(PULARGE_INTEGER)&TotalNumberOfBytes,
			(PULARGE_INTEGER)&TotalNumberOfFreeBytes);
	}
	else 
	{
		DWORD dwSectPerClust = 0;
		DWORD dwBytesPerSect = 0;
		DWORD dwFreeClusters = 0;
		DWORD dwTotalClusters = 0;

		BOOL bRet = GetDiskFreeSpace (szDriver, 
			&dwSectPerClust, 
			&dwBytesPerSect,
			&dwFreeClusters, 
			&dwTotalClusters);

		// Process GetDiskFreeSpace results.
		n64FreeBytesToUse = dwFreeClusters * dwBytesPerSect * dwSectPerClust;
	}

	return n64FreeBytesToUse;
}