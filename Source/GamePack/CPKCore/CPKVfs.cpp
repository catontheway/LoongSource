//-----------------------------------------------------------------------------
// File: CPKCore.cpp
// Desc: game resource file system 3.0
// Time: 2005-5-31
// Auth: tidy
//
// Copyright (c) 2003 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "cpkvfs.h"
// ʹ����������
#include "Util.h"
#include "..\memmgr\memmgr.h"
#include "..\memmgr\ts_memmgr.h"
#include "..\filtermgr\filtermgr.h"
#include "disciomgr.h"
#include "..\md5\md5.h"
//-----------------------------------------------------------------------------
//���캯��
//-----------------------------------------------------------------------------
CPKVfs::CPKVfs(CTSMemMgr* pTSMem)
{
	m_bLoaded = FALSE;

	m_pTSMemMgr = pTSMem;
	m_dwAllocGranularity = this->GetAllocationGranularity();

	m_dwMaxCPKTable = GT_CPK_DEFAULT_FILE;
	m_pCPKTable = new tagCPKTable[m_dwMaxCPKTable];
	ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_dwMaxCPKTable);
	m_pFragment = new INT[m_dwMaxCPKTable];
	m_pIndexToFragment = new INT[m_dwMaxCPKTable];

	ZeroMemory(m_pFragment, sizeof(INT)*m_dwMaxCPKTable);
	ZeroMemory(m_pIndexToFragment, sizeof(INT)*m_dwMaxCPKTable);
}
//-----------------------------------------------------------------------------
//��������
//-----------------------------------------------------------------------------
CPKVfs::~CPKVfs()
{
	FREEARR(m_pCPKTable);
	FREEARR(m_pFragment);
	FREEARR(m_pIndexToFragment);
}
//-----------------------------------------------------------------------------
// �õ��ڴ����߶�
//-----------------------------------------------------------------------------
DWORD	CPKVfs::GetAllocationGranularity()
{
	SYSTEM_INFO si;
	::GetSystemInfo(&si);
	return si.dwAllocationGranularity;
}
//-----------------------------------------------------------------------------
// �����µĹ����,������ھ�ֱ�Ӵ򿪣������ھʹ������򿪣���ʵ������load����
//-----------------------------------------------------------------------------
BOOL	CPKVfs::Create(LPCTSTR pFileName)
{
	ASSERT( pFileName );
	
	if( TRUE == m_bLoaded && FALSE == this->Unload() )	//��ȷ��ж��
		return FALSE;
	
	m_mapValidTable.clear();
	// ������������Զ��򿪰�
	if (CDiscIOMgr::IsExist(pFileName) == TRUE)
	{
		return this->Load(pFileName);
	}

	lstrcpy(m_szCPKFileName, pFileName);	//�����ļ���

	m_CPKHeader.dwLable = GT_CPK_LABEL;
	m_CPKHeader.dwVersion = GT_CPK_VERSION;
	m_CPKHeader.dwTableStart = sizeof(m_CPKHeader);
	m_CPKHeader.dwSizeOfHeader = sizeof(m_CPKHeader);
	m_CPKHeader.dwDataStart = sizeof(m_CPKHeader);

	m_CPKHeader.dwFileNum = 0;
	m_CPKHeader.dwValidTableNum = 0;
	m_CPKHeader.dwMaxTableNum = GT_CPK_DEFAULT_FILE;
	m_CPKHeader.dwFragmentNum = 0;
	m_CPKHeader.dwPackageSize = m_CPKHeader.dwTableStart + 
		sizeof(tagCPKTable) * GT_CPK_DEFAULT_FILE;

	m_dwCPKFileHandle = CDiscIOMgr::Create(pFileName,
		GENERIC_READ | GENERIC_WRITE,
		0, // ��ռģʽ
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if (m_dwCPKFileHandle == INVALID_FILE_HANDLE)
	{
		Msg(_T("Couldn't open file"));
		return FALSE;
	}

	CDiscIOMgr::Write(m_dwCPKFileHandle, &m_CPKHeader, sizeof(m_CPKHeader));

	// ���յ�Tableд��CPK
	ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_dwMaxCPKTable);
	CDiscIOMgr::Write(m_dwCPKFileHandle, m_pCPKTable, 
		sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum);

	// �����ļ�ӳ��
	m_dwCPKMappingHandle = CDiscIOMgr::CreateMapping(m_dwCPKFileHandle, NULL,
		PAGE_READONLY, 0, NULL);

	if( m_dwCPKMappingHandle == NULL )
	{
		ERR(_T("couldn't create file mapping %s"), szFileName);
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		m_dwCPKFileHandle = INVALID_FILE_HANDLE;

		return FALSE;
	}
	else
	{
		// �ļ�ӳ���Ѿ�����
		if( ::GetLastError() == ERROR_ALREADY_EXISTS )
		{
			ERR(_T("File mapping %s has already exist"), szFileName);
			CDiscIOMgr::CloseMappingHandle(m_dwCPKMappingHandle);
			m_dwCPKMappingHandle = 0;
			CDiscIOMgr::Close(m_dwCPKFileHandle);
			m_dwCPKFileHandle = INVALID_FILE_HANDLE;
			return FALSE;
		}
	}

	m_bLoaded = TRUE;
	return TRUE;
}
//-----------------------------------------------------------------------------
//����cpk�ļ��������ھͷ��ش���
//-----------------------------------------------------------------------------
BOOL	CPKVfs::Load(LPCTSTR pFileName)
{
	if( TRUE == m_bLoaded && FALSE == this->Unload() )	//��ȷ��ж��
		return FALSE;

	ASSERT(pFileName);
	if( CDiscIOMgr::IsExist(pFileName) == FALSE )
		return FALSE;

	lstrcpy(m_szCPKFileName, pFileName);

	m_dwCPKFileHandle = CDiscIOMgr::Create(pFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, // �Ƕ�ռģʽ
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if (m_dwCPKFileHandle == INVALID_FILE_HANDLE )
	{
		Msg(_T("Couldn't open file %s"), pFileName);
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		ASSERT(0);
		return FALSE;
	}

	// �����ļ�ͷ
	ZeroMemory(&m_CPKHeader, sizeof(m_CPKHeader));
	if( FALSE == CDiscIOMgr::Read(m_dwCPKFileHandle, &m_CPKHeader, sizeof(m_CPKHeader)) )
	{
		Msg(_T("Could not load table from the CPK %s"), pFileName);
		return FALSE;
	}
	if (m_CPKHeader.dwLable != GT_CPK_LABEL)
	{
		Msg(_T("Unknow file format %s"), pFileName);
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		ASSERT(0);
		return FALSE;
	}
	

	// �����ļ�Table

	//���ǿ�������ռ乻����
	if( m_dwMaxCPKTable < m_CPKHeader.dwMaxTableNum )
	{
		FREEARR(m_pCPKTable);
		m_pCPKTable = new tagCPKTable[m_CPKHeader.dwMaxTableNum];
		ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum);
		m_dwMaxCPKTable = m_CPKHeader.dwMaxTableNum;

		FREEARR(m_pFragment);
		FREEARR(m_pIndexToFragment);

		m_pFragment = new INT[m_dwMaxCPKTable];
		m_pIndexToFragment = new INT[m_dwMaxCPKTable];

		ZeroMemory(m_pFragment, sizeof(INT)*m_dwMaxCPKTable);
		ZeroMemory(m_pIndexToFragment, sizeof(INT)*m_dwMaxCPKTable);
	}

	//��λ����λ��
	if( FALSE == CDiscIOMgr::Seek(m_dwCPKFileHandle, m_CPKHeader.dwTableStart) )
	{
		Msg(_T("Could not seek table from the CPK %s, seekpos=%ld"), \
				pFileName, m_CPKHeader.dwTableStart);

		return FALSE;
	}

	if( FALSE == CDiscIOMgr::Read(m_dwCPKFileHandle, m_pCPKTable,  
		sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum) )
	{
		Msg(_T("Could not load table from the CPK %s"), pFileName);
		return FALSE;
	}

	for(DWORD i=0; i<m_CPKHeader.dwMaxTableNum; i++)
	{
		if( IS_FILE_OR_DIR_VALID(m_pCPKTable[i].dwFlag) )
			m_mapValidTable.insert(std::make_pair(m_pCPKTable[i].dwCRC, i));
	}
	// �õ�������Ƭ��Ϣ
	GetAllFragment();

	// �����ļ�ӳ��
	m_dwCPKMappingHandle = CDiscIOMgr::CreateMapping(m_dwCPKFileHandle, NULL,
		PAGE_READONLY, 0, NULL);

	if( m_dwCPKMappingHandle == NULL )
	{
		ERR(_T("couldn't create file mapping %s"), szFileName);
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		m_dwCPKFileHandle = INVALID_FILE_HANDLE;

		return FALSE;
	}
	else
	{
		// �ļ�ӳ���Ѿ�����
		if( ::GetLastError() == ERROR_ALREADY_EXISTS )
		{
			ERR(_T("File mapping %s has already exist"), szFileName);
			CDiscIOMgr::CloseMappingHandle(m_dwCPKMappingHandle);
			m_dwCPKMappingHandle = 0;
			CDiscIOMgr::Close(m_dwCPKFileHandle);
			m_dwCPKFileHandle = INVALID_FILE_HANDLE;
			return FALSE;
		}
	}

	m_bLoaded = TRUE;
	return TRUE;
}
//-----------------------------------------------------------------------------
//ж��cpk�ļ���
//-----------------------------------------------------------------------------
BOOL	CPKVfs::Unload()
{
	if (m_bLoaded)
	{
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		m_dwCPKFileHandle = 0;

		ZeroMemory(&m_CPKHeader, sizeof(m_CPKHeader));
		ZeroMemory(m_szCPKFileName, sizeof(m_szCPKFileName));
		m_mapValidTable.clear();

		m_bLoaded = FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
//���ӱ���ռ�
//-----------------------------------------------------------------------------
BOOL CPKVfs::IncreaseTable()
{
	if (m_CPKHeader.dwFileNum + 1 < m_CPKHeader.dwMaxTableNum)
		return TRUE;

	DWORD dwTable = m_CPKHeader.dwMaxTableNum + GT_CPK_TABLE_ADD;
	if( m_dwMaxCPKTable >= dwTable )
	{
		ZeroMemory(m_pCPKTable+m_CPKHeader.dwMaxTableNum, \
			sizeof(tagCPKTable)*(m_dwMaxCPKTable-m_CPKHeader.dwMaxTableNum));
	}
	else
	{
		//����
		tagCPKTable *pTable = new tagCPKTable[dwTable];
		memcpy(pTable, m_pCPKTable, sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum);
		ZeroMemory(pTable+m_CPKHeader.dwMaxTableNum, \
			sizeof(tagCPKTable)*(dwTable-m_CPKHeader.dwMaxTableNum));

		FREEARR(m_pCPKTable);
		m_pCPKTable = pTable;


		INT *p1 = new INT[dwTable];
		INT *p2 = new INT[dwTable];
		ZeroMemory(p1, sizeof(INT)*dwTable);
		ZeroMemory(p2, sizeof(INT)*dwTable);
		memcpy(p1, m_pFragment, sizeof(INT)*m_CPKHeader.dwMaxTableNum);
		memcpy(p2, m_pIndexToFragment, sizeof(INT)*m_CPKHeader.dwMaxTableNum);

		FREEARR(m_pFragment);
		FREEARR(m_pIndexToFragment);
		m_pFragment = p1;
		m_pIndexToFragment = p2;

		m_dwMaxCPKTable = dwTable;
	}

	m_CPKHeader.dwMaxTableNum = dwTable;
	m_CPKHeader.dwPackageSize += sizeof(tagCPKTable)*GT_CPK_TABLE_ADD;

	return TRUE;
}
//-----------------------------------------------------------------------------
// ��Load Packageʱ����һ���Ա�õ�������Ƭ��Ϣ
//-----------------------------------------------------------------------------
INT CPKVfs::GetAllFragment()
{
	INT nFragment = 0;
	for( INT n=0; n<(INT)m_CPKHeader.dwMaxTableNum; n++ )
	{
		if( IS_FILE_REMOVED(m_pCPKTable[n].dwFlag) )	// ����Ƭ
		{
			m_pFragment[nFragment] = n;
			m_pIndexToFragment[n] = nFragment;
			nFragment++;
		}
		else
			m_pIndexToFragment[n] = GT_INVALID;
	}

	ASSERT((DWORD)nFragment == m_CPKHeader.dwFragmentNum);

	return nFragment;
}

//-----------------------------------------------------------------------------
//��crc�õ���Ӧ�ı���
//-----------------------------------------------------------------------------
INT		CPKVfs::GetTableIndexFromeCrc(DWORD dwCRC)
{
	// ��һ����ͬ��CRC����Ч��CRC�϶������
	// ���ֲ���
	INT nStart = 0, nEnd = m_CPKHeader.dwValidTableNum;
	INT nPos = 0;
	
	while( nEnd != nStart )
	{
		nPos = nStart + (nEnd-nStart)/2;
		if( dwCRC == m_pCPKTable[nPos].dwCRC
			&& IS_FILE_OR_DIR_VALID(m_pCPKTable[nPos].dwFlag) ) 
			return nPos;

		if( nEnd == nStart+1 )
			return GT_INVALID;

		if( dwCRC >=  m_pCPKTable[nPos].dwCRC )
			nStart = nPos;
		else
			nEnd = nPos;
	}

	return GT_INVALID;
}
//-----------------------------------------------------------------------------
//�����ļ����Ƿ��ʾ���֮��Ҫ����cpk��������: data.cpk\system\client.exe,
//�ⲿ����ȷ�����ļ�,������Ŀ¼������,data.cpk�������޴�֪�����Ƿ�Ŀ¼
//-----------------------------------------------------------------------------
BOOL	CPKVfs::IsFileForCPK(LPCTSTR szFullName, LPCTSTR szCPKName, LPCTSTR szFileName)
{
	if( NULL == szFullName ) return FALSE;

	INT nLen = lstrlen(szFullName);
	for(int i=0; szFullName[i] != '\0' && i < 500; i++)
	{
		if( szFullName[i] != '\\' )
			continue;
		if( i >= 5 )
		{
			if( (szFullName[i-1] == 'k' || szFullName[i-1] == 'K') &&
				(szFullName[i-2] == 'p' || szFullName[i-2] == 'P') &&
				(szFullName[i-3] == 'c' || szFullName[i-1] == 'C') &&
				(szFullName[i-4] == '.') )
			{
				if( NULL != szCPKName )
				{
					memcpy((void*)szCPKName, (void*)szFullName, i);
					((TCHAR*)szCPKName)[i] = 0;
				}
				if( NULL != szFileName )
				{
					strcpy((TCHAR*)szFileName, szFullName+i+1);
					((TCHAR*)szFileName)[nLen-i-1] = 0;
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}
//---------------------------------------------------------------------------------
DWORD	CPKVfs::Open(INT nTableIndex)
{
	ASSERT( IS_VALID_CPK_TABLE(nTableIndex) );

	// ����ʵ�ʿ�ʼӳ���λ��
	DWORD dwRealStartPos = m_pCPKTable[nTableIndex].dwStartPos;
	dwRealStartPos = dwRealStartPos - (dwRealStartPos % m_dwAllocGranularity);

	// ʵ��ӳ������Ҫӳ��λ�õ�ƫ��
	DWORD dwOffset = m_pCPKTable[nTableIndex].dwStartPos - dwRealStartPos;

	// ������Ҫӳ����ֽ�
	DWORD dwBytesToRead = m_pCPKTable[nTableIndex].dwPackedSize
		+ m_pCPKTable[nTableIndex].dwExtraInfoSize + dwOffset;

	// �����ļ�����
	CPKFile* fp = (CPKFile*)m_pTSMemMgr->Alloc(sizeof(CPKFile));
	if( !fp )
	{
		ERR(_T("could not malloc for fp in %s"), m_szCPKFileName);
		return 0;
	}
	ZeroMemory(fp->szFullName, sizeof(fp->szFullName));

	fp->bValid = TRUE;
	fp->lpMapAddress = NULL;
	// ӳ��
	fp->lpMapAddress = CDiscIOMgr::MapFile
		(m_dwCPKMappingHandle, FILE_MAP_READ, dwRealStartPos, dwBytesToRead);

	if( fp->lpMapAddress == NULL )
	{
		//ERR(_T("could not map file %s\r\ncpk:%s, start pos:%lu, bytes:%lu"), 
		//szFileName, m_szCPKFileName, dwRealStartPos, dwBytesToRead);

		fp->bValid = FALSE;
		return 0;
	}

	// ��дfp������Ϣ
	fp->pTableItem = &m_pCPKTable[nTableIndex];
	fp->lpStartAddress = (LPVOID)((BYTE*)fp->lpMapAddress + dwOffset);
	fp->bCompressed = IS_COMPRESSED( m_pCPKTable[nTableIndex].dwFlag );
	fp->dwFileSize = m_pCPKTable[nTableIndex].dwOriginSize;
	fp->dwOffset = dwOffset;
	fp->dwPointer = 0;	// �ļ�ָ���ʼ

	ASSERT( fp->dwFileSize>= 0 && fp->dwFileSize<0x40000000 );

	// �����ѹ���ļ�������Ҫ�Ƚ�ѹ��
	if( fp->bCompressed == TRUE && fp->dwFileSize > 0 )
	{
		// Ϊ��ѹ�����ڴ�
		fp->lpMem = m_pTSMemMgr->Alloc(fp->dwFileSize);
		if( fp->lpMem == NULL )
		{
			CDiscIOMgr::UnMapFile(fp->lpMapAddress);
			fp->bValid = FALSE;
			return 0;
		}

		// �����ˣ���ѹ���ļ�
		tagFilterParam param;
		param.bCompress = FALSE;
		param.bSuccess = FALSE;
		param.dwInBufferSize = m_pCPKTable[nTableIndex].dwPackedSize;
		param.dwInDataSize = m_pCPKTable[nTableIndex].dwPackedSize;
		param.dwOutBufferSize = m_pCPKTable[nTableIndex].dwOriginSize;
		param.dwOutDataSize = m_pCPKTable[nTableIndex].dwOriginSize;
		param.eFilter = GET_FILTER(m_pCPKTable[nTableIndex].dwFlag);
		param.lpMemIn = fp->lpStartAddress;
		param.lpMemOut = fp->lpMem;

		// ���й����ļ���Ϣ
		CFilterMgr::Filter(&param);

		if( FALSE == param.bSuccess )
		{
			ERR(_T("could not decompress"));
			CDiscIOMgr::UnMapFile(fp->lpMapAddress);
			m_pTSMemMgr->Free(fp->lpMem);
			fp->bValid = FALSE;
			return 0;
		}
	}
	else	// ����ļ�û�б�ѹ����
	{
		// ����ָ��Map�е��ļ�ʵ����ʼ��ַ
		fp->lpMem = fp->lpStartAddress;
	}
	GetFullNameFromCRC(fp->pTableItem->dwCRC, fp->szFullName);
//	::InterlockedIncrement((LONG*)&m_nOpenedFileNum);
	return (DWORD)fp;
}
//-----------------------------------------------------------------------------
// ��λ���е����ļ���ָ��,���ص�ǰ�ļ�ָ��λ��
//-----------------------------------------------------------------------------
DWORD	CPKVfs::Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin)
{
	CPKFile* fp = (CPKFile*)dwHandle;
	ASSERT( fp != NULL );
	ASSERT( fp->bValid );

	switch( dwOrigin )
	{
	case GT_SEEK_SET:
		fp->dwPointer = nOffset;
		break;
	case GT_SEEK_CUR:
		fp->dwPointer += nOffset;
		break;
	case GT_SEEK_END:
		fp->dwPointer = fp->dwFileSize + nOffset;
		break;
	default:
		ASSERT(0);
		return (DWORD)GT_INVALID;		
	}

	return fp->dwPointer;
}
//-----------------------------------------------------------------------------
// �����е����ļ�������
//-----------------------------------------------------------------------------
DWORD	CPKVfs::Read(LPVOID lpMemOut, DWORD dwSize, DWORD dwHandle)
{
	CPKFile* fp = (CPKFile*)dwHandle;
	ASSERT(fp != NULL);
	ASSERT(fp->bValid);


	if( fp->dwPointer + dwSize > fp->dwFileSize )
		dwSize = fp->dwFileSize -  fp->dwPointer;

	memcpy(lpMemOut, (BYTE*)fp->lpMem + fp->dwPointer, dwSize);


	fp->dwPointer += dwSize;

	// ���뷵��ʵ��"�ɹ�"��ȡ���ֽ���
	return dwSize;
}
//-----------------------------------------------------------------------------
// �رհ��еĵ����ļ�, ����ȡ���ļ�ӳ�䣬�������ļ����
//-----------------------------------------------------------------------------
BOOL	CPKVfs::Close(DWORD dwHandle)
{
	CPKFile* fp = (CPKFile*)dwHandle;
	ASSERT(fp != NULL);
	ASSERT(fp->bValid == TRUE);

	if( fp->lpMapAddress == 0 )
	{
		ERR(_T("invalid address in cpk close"));
		return FALSE;
	}

	BOOL bResult = CDiscIOMgr::UnMapFile(fp->lpMapAddress);
	if( FALSE == bResult )
	{
		ERR(_T("could not unmap file in cpk close"));
		fp->bValid = FALSE;
		return FALSE;
	}

	if( fp->bCompressed && fp->dwFileSize > 0 )
		m_pTSMemMgr->Free(fp->lpMem);

	m_pTSMemMgr->Free(fp);

	return TRUE;
}

//---------------------------------------------------------------------------------
//�õ�ȫ·���ļ���,ͨ��crc
//---------------------------------------------------------------------------------
BOOL	CPKVfs::GetFullNameFromCRC(DWORD dwCRC, LPCTSTR szFullName)
{
	static TCHAR szTempNameNew[MAX_PATH*2];
	static TCHAR szTempNameOld[MAX_PATH*2];
	BOOL bResult = FALSE;

	ZeroMemory(szTempNameNew, sizeof(szTempNameNew));
	ZeroMemory(szTempNameOld, sizeof(szTempNameOld));

	if(dwCRC == 0)	// ��Ŀ¼
	{
		lstrcpy((LPSTR)szFullName, _T(""));
		return TRUE;
	}

	//���ļ���
	if( FALSE == this->GetSimpleFileNameFromCRC(szTempNameOld, dwCRC) )
		return FALSE;

	INT nTableIndex = this->GetTableIndexFromeCrc(dwCRC);
	if( !IS_VALID_CPK_TABLE(nTableIndex) )
		return FALSE;
	dwCRC = (m_pCPKTable + nTableIndex)->dwFatherCRC;

	while(dwCRC != 0)
	{
		// �����ļ���ƴ��
		lstrcpy(szTempNameNew, _T("\\"));
		lstrcat(szTempNameNew, szTempNameOld); // new: \abc

		this->GetSimpleFileNameFromCRC(szTempNameOld, dwCRC);
		lstrcat(szTempNameOld, szTempNameNew); // old: abc\abc

		if (lstrlen(szTempNameOld) >= MAX_PATH)
		{
			ASSERT(0);
			Msg(_T("�ļ���̫�� %s"), szTempNameOld);
			return FALSE;
		}

		nTableIndex = this->GetTableIndexFromeCrc(dwCRC);
		ASSERT(nTableIndex != GT_INVALID);
		dwCRC = (m_pCPKTable + nTableIndex)->dwFatherCRC;

	}

	lstrcpy((LPSTR)szFullName, szTempNameOld);

	return TRUE;
}
//-----------------------------------------------------------------------------
// ��CRC��ʶ�õ����ļ���
//-----------------------------------------------------------------------------
BOOL CPKVfs::GetSimpleFileNameFromCRC(LPTSTR pSimpleFileName, DWORD dwCRC)
{
	BOOL bResult = FALSE;
	ASSERT(pSimpleFileName);

	if (dwCRC == 0)
	{
		Msg(_T("CRC == 0 in function GetFileNameFromCRC"));
		return FALSE;
	}

	INT nTableIndex = this->GetTableIndexFromeCrc(dwCRC);
	if( !IS_VALID_CPK_TABLE(nTableIndex) )
		return FALSE;

	// �Ӱ��еõ����ļ���(û��·��)

	static TCHAR szTemp[MAX_PATH];

	tagCPKTable* pTableItem = m_pCPKTable + nTableIndex;
	if( FALSE == IS_FILE_OR_DIR_VALID(pTableItem->dwFlag) )
	{
		DbgMsg(_T("Table %d is not valid"), nTableIndex);
		return FALSE;
	}

	// �����ļ�������Ϣ�����а����ļ�����Ϣ
	DWORD dwReadPos = pTableItem->dwStartPos + pTableItem->dwPackedSize;
	DWORD dwBytesToRead = pTableItem->dwExtraInfoSize;

	CDiscIOMgr::Seek(m_dwCPKFileHandle, dwReadPos);
	CDiscIOMgr::Read(m_dwCPKFileHandle, szTemp, dwBytesToRead);

	lstrcpy(pSimpleFileName, szTemp);

	return TRUE;

}
//-----------------------------------------------------------------------------
// ����Ƿ��ǺϷ���CPK�ļ���
//-----------------------------------------------------------------------------
BOOL CPKVfs::IsValidCPK(LPCTSTR pFileName)
{
	ASSERT(pFileName);
	if( CDiscIOMgr::IsExist(pFileName) == FALSE )
	{
		Msg(_T("File %s doesn't exsit"), pFileName);
		return FALSE;
	}
	//Ȼ�������ж���չ���Ƿ���.cpk
	static TCHAR szFullName[MAX_PATH];
	LPTSTR lpExtName = NULL;

	lstrcpy(szFullName, pFileName);
	lpExtName = _tcsrchr(szFullName, _T('.'));

	if( !lpExtName )
		return FALSE;

	if( _tcsicmp(lpExtName, _T(".cpk")) != 0 )
		return FALSE;

	DWORD dwHandle = CDiscIOMgr::Create(pFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if (dwHandle == (DWORD)INVALID_HANDLE_VALUE )
	{
		Msg(_T("Couldn't open file %s"), pFileName);
		return FALSE;
	}

	// �����ļ�ͷ
	tagCPKHeader header;
	ZeroMemory(&header, sizeof(header));
	CDiscIOMgr::Read(dwHandle, &header, sizeof(header));
	CDiscIOMgr::Close(dwHandle);

	// ����ļ���־
	if( header.dwLable != GT_CPK_LABEL )
	{
		//Msg(_T("Unknow file format %s"), pFileName);
		return FALSE;
	}

	// ����ļ��汾��
	if( header.dwVersion != GT_CPK_VERSION )
	{
		//Msg(_T("Wrong file version %lu"), header.dwVersion);
		return FALSE;
	}

	// ����ļ�Table����ʼλ��
	if( header.dwTableStart == 0 )
	{
		//Msg(_T("Wrong table start pos"));
		return FALSE;
	}

	// �������ļ���
	if( header.dwFileNum > header.dwMaxTableNum )
	{
		//Msg(_T("Wrong files num in package"));
		return FALSE;
	}

	// ��������Ч�����
	if( header.dwValidTableNum > header.dwMaxTableNum )
	{
		//Msg(_T("Wrong valid table item num in package"));
		return FALSE;
	}

	// ����ļ�������Ч�����Ĺ�ϵ
	if( header.dwFileNum > header.dwValidTableNum )
	{
		//Msg(_T("File num is more than table item num in package"));
		return FALSE;
	}

	// �Ժ�汾�������ļ����Ƿ��С��������

	return TRUE;
}
//-----------------------------------------------------------------------------
// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
//������Ǽ��һ��ԭ��:���ļ����ݲ�ѹ��(Ҳ�����Ѿ�ѹ����),��չ��Ϣ����
//-----------------------------------------------------------------------------
BOOL CPKVfs::AddFile(LPCTSTR pFullName, DWORD dwFatherCRC, tagCPKTable *pTable, LPVOID p)
{
	ASSERT(pFullName);

	TCHAR *szName = (TCHAR *)p + pTable->dwPackedSize;

	// �Ƿ��ܷ����ļ�
	if( FALSE == IncreaseTable() )
		return FALSE;

	// ���ļ���ƴ��ȫ·����ת��ΪСд�ټ���CRC
	TCHAR szFullName[MAX_PATH];
	this->ConvertFullNameToCPKFullName(szFullName, pFullName, dwFatherCRC);

	// ��Ҫ�������ظ��ļ������
	DWORD dwCRC = CRC32(szFullName);
	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		RemoveFile(dwCRC);
	}

	// �õ�һ�����еı���ռ�
	int nTableIndex = GetFreeTableItem(dwCRC);
	if (nTableIndex == GT_INVALID)
	{
		Msg(_T("Couldn't find any free table item, AddFile failed"));
		return FALSE;
	}

	// ������Ϣ�ܳ���
	m_pCPKTable[nTableIndex].dwExtraInfoSize = pTable->dwExtraInfoSize;
	DWORD dwDataSize = pTable->dwPackedSize + m_pCPKTable[nTableIndex].dwExtraInfoSize;

	// �ҵ��ļ��ɲ������ݵĵط�
	DWORD dwWritePos = 0;
	dwWritePos = this->FindCPKFreePlace(dwDataSize);

	// ��д���ݱ���Table
	m_pCPKTable[nTableIndex].dwCRC = dwCRC;
	m_pCPKTable[nTableIndex].dwFlag = pTable->dwFlag;
	m_pCPKTable[nTableIndex].dwFatherCRC = dwFatherCRC;
	m_pCPKTable[nTableIndex].dwOriginSize = pTable->dwOriginSize;
	m_pCPKTable[nTableIndex].dwPackedSize = pTable->dwPackedSize;
	m_pCPKTable[nTableIndex].dwStartPos = dwWritePos;

	// �ļ�������һ
	m_CPKHeader.dwFileNum++;
	// ��Ч���������һ
	m_CPKHeader.dwValidTableNum++;


	// ���ļ��в�������
	// �ƶ��ļ�ָ��
	DWORD dwDistanceToMove = m_pCPKTable[nTableIndex].dwStartPos;
	CDiscIOMgr::Seek(m_dwCPKFileHandle, dwDistanceToMove);
	DWORD dwDestFileSize = CDiscIOMgr::GetSize(m_dwCPKFileHandle);
	if (dwDistanceToMove >= dwDestFileSize)
		CDiscIOMgr::SetEOF(m_dwCPKFileHandle);

	// д��ʵ����Ϣ
	CDiscIOMgr::Write(m_dwCPKFileHandle, p, dwDataSize);			


	// ���ļ�ͷ������д�����
	this->WriteHeaderToDisc();
	this->WriteTableToDisc();

	m_mapValidTable.insert(std::pair<DWORD, DWORD>(dwCRC, nTableIndex));

	return TRUE;
}
//-----------------------------------------------------------------------------
// ������ĵ�ǰĿ¼����ļ�
//-----------------------------------------------------------------------------
BOOL CPKVfs::AddFile(LPCTSTR pFullName, DWORD dwFatherCRC)
{
	ASSERT(pFullName);

	EFMFilter eFilter = FMF_Null;
	if( FALSE == IncreaseTable() )
		return FALSE;

	// ���ļ���ƴ��ȫ·����ת��ΪСд�ټ���CRC
	TCHAR szFullName[MAX_PATH];
	this->ConvertFullNameToCPKFullName(szFullName, pFullName, dwFatherCRC);

	// ��Ҫ�������ظ��ļ������
	DWORD dwCRC = CRC32(szFullName);
	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		RemoveFile(dwCRC);
	}

	// �õ�һ�����еı���ռ�
	int nTableIndex = GetFreeTableItem(dwCRC);
	if (nTableIndex == GT_INVALID)
	{
		Msg(_T("Couldn't find any free table item, AddFile failed"));
		return FALSE;
	}

	// ��Դ�ļ�
	DWORD dwSrcHandle = CDiscIOMgr::Create(pFullName,
		GENERIC_READ,
		FILE_SHARE_READ, // ��ռģʽ
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if (dwSrcHandle == (DWORD)INVALID_HANDLE_VALUE )
	{
		Msg(_T("Adding single file : couldn't open file %s"), pFullName);
		return FALSE;
	}


	// ����ļ�����
	BOOL bResult = FALSE;
	DWORD dwSrcFileSize = CDiscIOMgr::GetSize(dwSrcHandle);
	DWORD dwFileFlag = 0;
	VOID* pSrc = NULL;
	VOID* pDest = NULL;
	pSrc = m_pTSMemMgr->Alloc(dwSrcFileSize);
	pDest = m_pTSMemMgr->Alloc(dwSrcFileSize);

	if( dwSrcFileSize >= GT_CPK_RAW_BUFFER_SIZE 
		|| IsUnCompressable(pFullName) )	// mp3/bink donot take compress
	{
		// �����ļ������ڴ�
		eFilter = FMF_Changeless;
		dwFileFlag = 0x1 | (eFilter<<16) | 0x4;	// ��Ч | ��ѹ�� | ���ļ�
		bResult = CDiscIOMgr::_LoadToMem(pSrc, dwSrcHandle, dwSrcFileSize);
		if( bResult == FALSE )
		{
			Msg(_T("Couldn't Load File %s, AddFile failed"), pFullName);
			CDiscIOMgr::Close(dwSrcHandle);
			m_pTSMemMgr->Free(pSrc);
			m_pTSMemMgr->Free(pDest);

			return FALSE;
		}
	}
	else
	{
		eFilter = FMF_MiniLzo1040;
		dwFileFlag = 0x1 | (eFilter<<16); // ��Ч | LZOѹ��

		// ��Դ�ļ������ڴ�
		bResult = CDiscIOMgr::_LoadToMem(pSrc, dwSrcHandle, dwSrcFileSize);
		if (bResult == FALSE)
		{
			Msg(_T("Couldn't Load File %s, AddFile failed"), pFullName);
			CDiscIOMgr::Close(dwSrcHandle);
			m_pTSMemMgr->Free(pSrc);
			m_pTSMemMgr->Free(pDest);
			return FALSE;
		}
	}


	// ��д�������������
	tagFilterParam param;
	param.eFilter = eFilter;
	param.bCompress = TRUE;
	param.lpMemIn = pSrc;
	param.dwInBufferSize = dwSrcFileSize;
	param.lpMemOut = pDest;
	param.dwOutBufferSize = dwSrcFileSize;
	param.dwInDataSize = dwSrcFileSize;

	// �����ļ�
	CFilterMgr::Filter(&param);
	if (param.bSuccess == FALSE)
	{
		m_pTSMemMgr->Free(pSrc);
		m_pTSMemMgr->Free(pDest);
		return FALSE;
	}

	// ������Ϣ�ܳ���
	m_pCPKTable[nTableIndex].dwExtraInfoSize = this->CalFileExtraInfoSize(pFullName);
	DWORD dwDataSize = param.dwOutDataSize + m_pCPKTable[nTableIndex].dwExtraInfoSize;

	// �ҵ��ļ��ɲ������ݵĵط�
	DWORD dwWritePos = 0;
	dwWritePos = this->FindCPKFreePlace(dwDataSize);

	// ��д���ݱ���Table
	m_pCPKTable[nTableIndex].dwCRC = dwCRC;
	m_pCPKTable[nTableIndex].dwFlag = dwFileFlag;
	m_pCPKTable[nTableIndex].dwFatherCRC = dwFatherCRC;
	m_pCPKTable[nTableIndex].dwOriginSize = param.dwInDataSize;
	m_pCPKTable[nTableIndex].dwPackedSize = param.dwOutDataSize;
	m_pCPKTable[nTableIndex].dwStartPos = dwWritePos;

	// �ļ�������һ
	m_CPKHeader.dwFileNum++;
	// ��Ч���������һ
	m_CPKHeader.dwValidTableNum++;


	// ���ļ��в�������
	if( (EFMFilter)(m_pCPKTable[nTableIndex].dwFlag >> 16) == FMF_Changeless )
	{//��ѹ��
		bResult = AddRealDataToCPK(nTableIndex, pFullName, pSrc, dwSrcFileSize);
	}
	else
	{//�Ѿ�ѹ����
		bResult = AddRealDataToCPK(nTableIndex, pFullName, pDest, \
									m_pCPKTable[nTableIndex].dwPackedSize);
	}
	if (bResult == FALSE)
	{
		Msg(_T("Couldn't add data to cpk, file name : %s"),	pFullName);
		ASSERT(0);
		CDiscIOMgr::Close(dwSrcHandle);
		m_pTSMemMgr->Free(pSrc);
		m_pTSMemMgr->Free(pDest);
		return FALSE;
	}
	// ���ļ�ͷ������д�����
	this->WriteHeaderToDisc();
	this->WriteTableToDisc();

	m_pTSMemMgr->Free(pSrc);
	m_pTSMemMgr->Free(pDest);

	CDiscIOMgr::Close(dwSrcHandle);

	return TRUE;

}
//-----------------------------------------------------------------------------
// ɾ��������ļ�����Ŀ¼��ʹ������·��
//-----------------------------------------------------------------------------
BOOL CPKVfs::RemoveFile(DWORD dwCRC)
{
	// �ҵ����е��ļ�Table��
	INT nTableIndex = this->GetTableIndexFromeCrc(dwCRC);
	if( nTableIndex == GT_INVALID )
	{
		return FALSE;
	}

	// ����ݹ���ú���
	this->RemoveFileOrDir(nTableIndex);

	// ��Header\Table�浽����
	this->WriteHeaderToDisc();
	this->WriteTableToDisc();

	return TRUE;
}
//-----------------------------------------------------------------------------
// �ɴ���ȫ��תΪ�����ļ���������֪����CRC, ��ȫ��תΪСд
//-----------------------------------------------------------------------------
BOOL CPKVfs::ConvertFullNameToCPKFullName(LPTSTR pOutName, LPCTSTR pInName, 
									   DWORD dwFatherCRC)
{
	ASSERT(pOutName);
	ASSERT(pInName);
	BOOL bResult = FALSE;

	static TCHAR szSimpleName[MAX_PATH];
	static TCHAR szCPKFullName[MAX_PATH];

	bResult = this->GetFullNameFromCRC(dwFatherCRC, szCPKFullName);
	ASSERT(bResult);
	if (FALSE == bResult)
		return FALSE;

	bResult = this->GetSimpleFileName(szSimpleName, pInName);
	ASSERT(bResult);
	if (FALSE == bResult)
		return FALSE;

	if (lstrlen(szCPKFullName) + lstrlen(szSimpleName) > MAX_PATH)
	{
		ASSERT(0);
		Msg(_T("�ļ���̫��%s%s"), szCPKFullName, szSimpleName);
		return FALSE;
	}

	// ����ĺ������Զ�ȫ��תΪСд
	this->LinkSimpleNameToDirName(szCPKFullName, szSimpleName);
	lstrcpy(pOutName, szCPKFullName);
	return TRUE;
}
//-----------------------------------------------------------------------------
// ���Ӽ��ļ�����Ŀ¼����Ŀ��Ŀ¼��
//-----------------------------------------------------------------------------
BOOL CPKVfs::LinkSimpleNameToDirName(LPTSTR pFullName, LPCTSTR pSimpleName)
{
	ASSERT(pFullName);
	ASSERT(pSimpleName);

	INT nFullNameLen = lstrlen(pFullName);

	// ���Լ���ַ��������Ƿ��ܹ�����
	if( nFullNameLen >= MAX_PATH - 10 )
	{
		Msg(_T("�����ļ���̫��"));
		ASSERT(0);
		return FALSE;
	}

	// �Ȳ���Ŀ¼������û��"\"����
	if( nFullNameLen>0 && pFullName[nFullNameLen-1] != _T('\\') )
		lstrcat(pFullName, _T("\\"));

	lstrcat(pFullName, pSimpleName);
	_tcslwr(pFullName);	// ת��Сд
	return TRUE;
}

//-----------------------------------------------------------------------------
// ����Ƿ������������CRC�ظ���CRC
//-----------------------------------------------------------------------------
BOOL CPKVfs::HaveRepeatCRC(DWORD dwCRC)
{
	std::map<DWORD, DWORD>::iterator it;
	it = m_mapValidTable.find(dwCRC);
	if( it != m_mapValidTable.end() )
	{
		return TRUE;
	}
	return FALSE;			
}
//-----------------------------------------------------------------------------
// �ڰ���ָ��Ŀ¼����һ����Ŀ¼, ����CRC
//-----------------------------------------------------------------------------
DWORD CPKVfs::CreateDir(LPCTSTR pSimpleDirName, DWORD dwFatherCRC)
{
	ASSERT(pSimpleDirName);

	if( FALSE == IncreaseTable() )
		return FALSE;

	// ��Ŀ¼��ת��ΪСд�ټ���CRC
	TCHAR szFullDirName[MAX_PATH];
	this->GetFullNameFromCRC(dwFatherCRC, szFullDirName);
	this->LinkSimpleNameToDirName(szFullDirName, pSimpleDirName);
	DWORD dwCRC = CRC32(szFullDirName);

	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		//��Ŀ¼�Ѿ�����
		return dwCRC;
	}


	// �õ�һ�����еı���ռ�
	int nTableIndex = GetFreeTableItem(dwCRC);
	if (nTableIndex == GT_INVALID)
	{
		Msg(_T("Couldn't find any free table item"));
		return GT_INVALID;
	}

	// ������Ϣ�ܳ���
	m_pCPKTable[nTableIndex].dwExtraInfoSize = 
		this->CalFileExtraInfoSize(pSimpleDirName);	

	DWORD dwDataSize = m_pCPKTable[nTableIndex].dwExtraInfoSize;

	// �ҵ��ļ��ɲ������ݵĵط�
	DWORD dwWritePos = 0;
	dwWritePos = this->FindCPKFreePlace(dwDataSize);

	// ��д���ݱ���Table
	m_pCPKTable[nTableIndex].dwCRC = dwCRC;
	m_pCPKTable[nTableIndex].dwFlag = 0x1 | 0x2;	// Ŀ¼
	m_pCPKTable[nTableIndex].dwFatherCRC = dwFatherCRC;
	m_pCPKTable[nTableIndex].dwOriginSize = 0;
	m_pCPKTable[nTableIndex].dwPackedSize = 0;
	m_pCPKTable[nTableIndex].dwStartPos = dwWritePos;

	// �ļ�������һ
	m_CPKHeader.dwFileNum++;
	// ��Ч���������һ
	m_CPKHeader.dwValidTableNum++;


	// ���ļ��в�������
	this->AddRealDataToCPK(nTableIndex, pSimpleDirName, NULL, 0);
	// ���ļ�ͷ������д�����
	this->WriteHeaderToDisc();
	this->WriteTableToDisc();

	return m_pCPKTable[nTableIndex].dwCRC;

}

//-----------------------------------------------------------------------------
//���ַ�ʽ������ļ���ֻ�������·������·����ΪdwFatherCrc����Ŀ¼��Ϊ����������׼��
//-----------------------------------------------------------------------------
DWORD CPKVfs::AddDirCyc(LPCTSTR pFullName, DWORD dwFatherCRC)
{
	ASSERT(pFullName);
	//����һ��һ������·��,����c:\bin\tidy, c:\, c:\bin\tidy

	TCHAR szPathFile[MAX_PATH];
	INT i=0, j=0;
	for(i=0, j=0; pFullName[i] != '\0'; i++)
	{
		if( pFullName[i] == '.' )
		{ //�ļ��������Զ�����
			return dwFatherCRC;
		}

		if( pFullName[i] == '\\' && i > 0 )
		{
			ZeroMemory(szPathFile, sizeof(szPathFile));
			strncpy(szPathFile, pFullName+j, i-j);
			dwFatherCRC = CreateDir(szPathFile, dwFatherCRC);

			j = i+1;
		}
	}

	ZeroMemory(szPathFile, sizeof(szPathFile));
	strncpy(szPathFile, pFullName+j, i-j);
	return CreateDir(szPathFile, dwFatherCRC);
}
//-----------------------------------------------------------------------------
// ���ļ��в�������
//-----------------------------------------------------------------------------
BOOL CPKVfs::AddRealDataToCPK(INT nTableIndex, LPCTSTR pFileName,  VOID* pSrc, 
						   DWORD dwSrcFileSize)
{
	ASSERT(pFileName);
	ASSERT(IS_VALID_CPK_TABLE(nTableIndex));
	ASSERT(!IS_INVALID_HANDLE(m_dwCPKFileHandle));

	static TCHAR szFileName[MAX_PATH];

	BOOL bResult = FALSE;
	DWORD dwDistanceToMove = 0;
	DWORD dwBytesToWrite = 0;

	static TCHAR szSimpleName[MAX_PATH];
	this->GetSimpleFileName(szSimpleName, pFileName);
	dwBytesToWrite = (DWORD)lstrlen(szSimpleName);

	// �ƶ��ļ�ָ��
	dwDistanceToMove = m_pCPKTable[nTableIndex].dwStartPos;
	CDiscIOMgr::Seek(m_dwCPKFileHandle, dwDistanceToMove);
	DWORD dwDestFileSize = CDiscIOMgr::GetSize(m_dwCPKFileHandle);
	if (dwDistanceToMove >= dwDestFileSize)
		CDiscIOMgr::SetEOF(m_dwCPKFileHandle);

	//����ʵ����Ϣ��md5
	tagFileExtraInfo ExtraInfo;

	// д��ʵ����Ϣ
	if( m_pCPKTable[nTableIndex].dwPackedSize > 0 )
	{
		CMD5::MD5ForBuffer((BYTE*)pSrc, dwSrcFileSize, ExtraInfo.byMD5);
		CDiscIOMgr::Write(m_dwCPKFileHandle, pSrc, dwSrcFileSize);			
	}
	else
	{ //û���ļ�����,û��ϵ,���Ǽ������ֵ�md5
		CMD5::MD5ForString((TCHAR*)szSimpleName, ExtraInfo.byMD5);
	}


	// д�븽����Ϣ
	CDiscIOMgr::Write(m_dwCPKFileHandle, szSimpleName, dwBytesToWrite);

	// ���ļ���������ֽڽ�������, Ϊ��Unicode����
	CDiscIOMgr::Write(m_dwCPKFileHandle, _T("\0\0\0\0"), GT_CPK_LEN_OF_STRING_END);

	dwBytesToWrite = sizeof(ExtraInfo);
	CDiscIOMgr::Write(m_dwCPKFileHandle, &ExtraInfo, dwBytesToWrite);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���㸽����Ϣ����
//-----------------------------------------------------------------------------
DWORD CPKVfs::CalFileExtraInfoSize(LPCTSTR pFileName)
{
	DWORD dwSize = 0;
	dwSize += lstrlen(pFileName) + GT_CPK_LEN_OF_STRING_END;
	dwSize += sizeof(tagFileExtraInfo);
	return dwSize;
}
//-----------------------------------------------------------------------------
// ����������CRC�õ����е�Table��
//-----------------------------------------------------------------------------
INT	CPKVfs::GetFreeTableItem(DWORD dwCRC)
{
	/*
	if( m_CPKHeader.dwValidTableNum >= m_CPKHeader.dwMaxTableNum -1 )
	{
		ASSERT(0);

		// TODO: ��Ҫ���޿ռ���Ƭ��һ����С�Ϳռ���Ƭ����ת�ɿ�����
		return GT_INVALID;
	}

	for(INT n=0; n<m_CPKHeader.dwMaxTableNum; n++)
	{
		if( IS_FILE_OR_DIR_VALID(m_pCPKTable[n].dwFlag) )
			continue;
		return n;
	}/**/

	if( m_CPKHeader.dwValidTableNum >= m_CPKHeader.dwMaxTableNum -1 )
	{
		ASSERT(0);

		// TODO: ��Ҫ���޿ռ���Ƭ��һ����С�Ϳռ���Ƭ����ת�ɿ�����
		return GT_INVALID;
	}

	// �߼�ȷ����Ч���������Ƿ��ǿ�λ
	ASSERT( !IS_TABLE_VALID(m_pCPKTable[m_CPKHeader.dwValidTableNum].dwFlag) );

	// ����Ч�������ҵ����ʵĲ����
	for( INT n=0; (DWORD)n<m_CPKHeader.dwValidTableNum; n++ )
	{
		if( dwCRC < m_pCPKTable[n].dwCRC )
		{
			// ������Table����ƶ�
			for( INT m=m_CPKHeader.dwValidTableNum; m>n; m-- )
			{
				memcpy(&m_pCPKTable[m], &m_pCPKTable[m-1], sizeof(tagCPKTable));

				// ����Ƿ�����Ƭ������ǣ���Ҫ������Ƭ��Ϣ�ĸ���
				if( m_pIndexToFragment[m-1] != GT_INVALID )
				{
					m_pFragment[m_pIndexToFragment[m-1]] = m;
					m_pIndexToFragment[m] = m_pIndexToFragment[m-1];
					m_pIndexToFragment[m-1] = GT_INVALID;
				}
			}

			// ZeroMemory(&m_pCPKTable[n], sizeof(tagCPKTable)); ����Ҫ
			return n;
		}
	}

	// ������CRC������CRC�����Է������
	// ZeroMemory(&m_pCPKTable[m_CPKHeader.dwValidTableNum], sizeof(tagCPKTable)); ����Ҫ
	return (INT)m_CPKHeader.dwValidTableNum;

}
//-----------------------------------------------------------------------------
// �õ�CPK�п��Է�ָ����С���ݵĿռ���ʼ��
//-----------------------------------------------------------------------------
DWORD CPKVfs::FindCPKFreePlace(DWORD dwSize)
{
	INT nFrag = GT_INVALID;
	DWORD dwPos = 0;

	// �ȼ����û��������dwSize�Ŀռ���Ƭ
	for( INT n=0; (DWORD)n<m_CPKHeader.dwFragmentNum; n++ )
	{
		nFrag = m_pFragment[n];

		ASSERT( IS_VALID_CPK_TABLE(nFrag) );
		ASSERT( IS_FILE_REMOVED(m_pCPKTable[nFrag].dwFlag) );	// ȷ������Ƭ

		if( m_pCPKTable[nFrag].dwPackedSize >= dwSize )
		{
			dwPos = m_pCPKTable[nFrag].dwStartPos;

			m_pCPKTable[nFrag].dwStartPos += dwSize;
			m_pCPKTable[nFrag].dwPackedSize -= dwSize;	// ��Ƭ��С
			return dwPos;
		}
	}

	// ���û����ֱ�Ӹ��ӵ�����table�ļ�֮ǰ
	DWORD dwPackSize = m_CPKHeader.dwPackageSize;
	m_CPKHeader.dwPackageSize += dwSize;
	m_CPKHeader.dwTableStart = m_CPKHeader.dwPackageSize - \
		sizeof(tagCPKTable) * m_CPKHeader.dwMaxTableNum;

	return dwPackSize - sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum;

}
//-----------------------------------------------------------------------------
// ɾ���ļ���Ŀ¼����ɾ��������������
// ���Ż�
//-----------------------------------------------------------------------------
BOOL CPKVfs::RemoveFileOrDir(INT nTableIndex)
{
	ASSERT(IS_VALID_CPK_TABLE(nTableIndex));
	BOOL bDir = IS_DIR( m_pCPKTable[nTableIndex].dwFlag );
	DWORD dwCRC = m_pCPKTable[nTableIndex].dwCRC;

	m_mapValidTable.erase(m_pCPKTable[nTableIndex].dwCRC);
	// ��־�ļ�Ϊ�ռ���Ƭ
	SET_FILE_REMOVED(m_pCPKTable[nTableIndex].dwFlag);

	// �߼�ȷ��
	ASSERT(m_CPKHeader.dwFragmentNum < m_CPKHeader.dwMaxTableNum);
	ASSERT(m_CPKHeader.dwFragmentNum == 
		m_CPKHeader.dwValidTableNum - m_CPKHeader.dwFileNum);

	// ��¼��Ƭ
	m_pFragment[m_CPKHeader.dwFragmentNum] = nTableIndex;
	m_pIndexToFragment[nTableIndex] = m_CPKHeader.dwFragmentNum;

	m_CPKHeader.dwFileNum--; // �ļ�������һ
	m_CPKHeader.dwFragmentNum++;	// ��Ƭ��Ŀ��һ

	// ����Ƭ���д���
	// ����Ƭ�����пռ��¼��dwPackSize
	m_pCPKTable[nTableIndex].dwPackedSize += m_pCPKTable[nTableIndex].dwExtraInfoSize;
	m_pCPKTable[nTableIndex].dwExtraInfoSize = 0;
	m_pCPKTable[nTableIndex].dwFatherCRC = 0;

	if( FALSE == bDir )
		return TRUE;	//ֱ�ӷ���

	// Ѱ���ӽڵ㲢ɾ������Ϊ��δ���ո��ڵ���������ֻ��˳�����
	for( INT n=0; n<(INT)m_CPKHeader.dwValidTableNum; n++ )
	{
		if( FALSE == IS_FILE_OR_DIR_VALID(m_pCPKTable[n].dwFlag) )
			continue;

		if( m_pCPKTable[n].dwFatherCRC == dwCRC )
		{
			// �ݹ�ɾ��
			RemoveFileOrDir(n);	
		}
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
// ���ļ�ͷд����̰�
//-----------------------------------------------------------------------------
BOOL CPKVfs::WriteHeaderToDisc()
{

	// ��CPK Headerд��

	DWORD dwDistanceToMove = 0; 
	CDiscIOMgr::Seek(m_dwCPKFileHandle, dwDistanceToMove);
	DWORD dwBytesToWrite = sizeof(tagCPKHeader);
	CDiscIOMgr::Write(m_dwCPKFileHandle, &m_CPKHeader, dwBytesToWrite);

	return TRUE;

}

//-----------------------------------------------------------------------------
// ���ļ���д����̰�
//-----------------------------------------------------------------------------
BOOL CPKVfs::WriteTableToDisc()
{
	// ��Tableд��
	DWORD dwDistanceToMove = m_CPKHeader.dwTableStart; 

	CDiscIOMgr::Seek(m_dwCPKFileHandle, dwDistanceToMove);
	DWORD dwBytesToWrite = sizeof(tagCPKTable) * m_CPKHeader.dwMaxTableNum;
	CDiscIOMgr::Write(m_dwCPKFileHandle, m_pCPKTable, dwBytesToWrite);

	return TRUE;
}
//-----------------------------------------------------------------------------
// �������ļ����õ����ļ���(����·��)
//-----------------------------------------------------------------------------
BOOL CPKVfs::GetSimpleFileName(LPTSTR pSimpleName, LPCTSTR pFullName)
{
	ASSERT(pSimpleName);
	ASSERT(pFullName);

	LPCTSTR lpSimpleName = _tcsrchr(pFullName, _T('\\'));
	if (NULL == lpSimpleName)
	{
		lstrcpy(pSimpleName, pFullName);
	}
	else
	{
		lstrcpy(pSimpleName, lpSimpleName+1);
	}
	return TRUE;
}
//-----------------------------------------------------------------------------
// �Ƿ��ǲ���ѹ���ļ�  mp3/bik
//-----------------------------------------------------------------------------
BOOL CPKVfs::IsUnCompressable(LPCTSTR pFullName)
{
	static TCHAR szFullName[MAX_PATH];
	LPTSTR lpExtName = NULL;

	lstrcpy(szFullName, pFullName);
	lpExtName = _tcsrchr(szFullName, _T('.'));

	if( !lpExtName )
		return FALSE;

	if( _tcsicmp(lpExtName, _T(".mp3")) == 0 
		|| _tcsicmp(lpExtName, _T(".bik")) == 0 )
		return TRUE;
	else
		return FALSE;
}
//-----------------------------------------------------------------------------
// �õ������ļ�����չ��(��"."��)
//-----------------------------------------------------------------------------
BOOL CPKVfs::GetFileExtName(LPTSTR pExtName, INT nTableIndex)
{
	ASSERT(IS_VALID_CPK_TABLE(nTableIndex));
	ASSERT(pExtName);

	static TCHAR szTemp[MAX_PATH];
	static TCHAR szFullName[MAX_PATH];
	LPTSTR lpExtName = NULL;

	tagCPKTable* pTableItem = m_pCPKTable+nTableIndex;

	if( FALSE == IS_FILE_OR_DIR_VALID(pTableItem->dwFlag) )
	{
		DbgMsg(_T("Table %d is not valid"), nTableIndex);
		return FALSE;
	}

	// table item ����ָ��һ��Ŀ¼
	if (TRUE == IS_DIR(pTableItem->dwFlag))
	{
		lstrcpy(pExtName, _T(""));
		return TRUE;
	}

	// �����ļ�������Ϣ�����а����ļ�����Ϣ
	DWORD dwReadPos = pTableItem->dwStartPos + pTableItem->dwPackedSize;
	DWORD dwBytesToRead = pTableItem->dwExtraInfoSize;

	CDiscIOMgr::Seek(m_dwCPKFileHandle, dwReadPos);
	CDiscIOMgr::Read(m_dwCPKFileHandle, szTemp, dwBytesToRead);

	// Ѱ����չ��
	ZeroMemory(szFullName, sizeof(szFullName));
	lstrcpy(szFullName, szTemp);
	lpExtName = _tcsrchr(szFullName, _T('.'));
	if (NULL == lpExtName)
		lstrcpy(pExtName, _T(""));
	else
		lstrcpy(pExtName, lpExtName);

	return TRUE;
}
//-----------------------------------------------------------------------------