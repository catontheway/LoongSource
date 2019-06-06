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
#include "filtermgr.h"
#include "disciomgr.h"
#include "md5.h"
//-----------------------------------------------------------------------------
//���캯��
//-----------------------------------------------------------------------------
CPKVfs::CPKVfs()
{
	m_bLoaded = FALSE;
	m_pUtil = new Util;

	m_dwAllocGranularity = this->GetAllocationGranularity();

	m_dwMaxCPKTable = GT_CPK_DEFAULT_FILE;
	m_pCPKTable = new tagCPKTable[m_dwMaxCPKTable];
	ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_dwMaxCPKTable);
	m_pFragment = new INT[m_dwMaxCPKTable];
	m_pIndexToFragment = new INT[m_dwMaxCPKTable];

	memset(m_pFragment, -1, sizeof(INT)*m_dwMaxCPKTable);
	memset(m_pIndexToFragment, -1, sizeof(INT)*m_dwMaxCPKTable);
}
//-----------------------------------------------------------------------------
//��������
//-----------------------------------------------------------------------------
CPKVfs::~CPKVfs()
{
	SAFE_DEL_ARRAY(m_pCPKTable);
	SAFE_DEL_ARRAY(m_pFragment);
	SAFE_DEL_ARRAY(m_pIndexToFragment);
	SAFE_DEL( m_pUtil );

	m_mapValidTable.clear();
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
BOOL CPKVfs::Create(LPCSTR pFileName)
{
	ASSERT( pFileName );
	if( m_bLoaded && !this->Unload() )	//��ȷ��ж��
		return FALSE;

	m_mapValidTable.clear();
	// ������������Զ��򿪰�
	if( CDiscIOMgr::IsExist(pFileName) )
	{
		return this->Load(pFileName);
	}

	strcpy(m_szCPKFileName, pFileName);	//�����ļ���

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
		ErrMsg(g_StrTable[_T("OpenFileFailed")], m_pUtil->AnsiToUnicode(pFileName));
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
		ErrMsg(g_StrTable[_T("CreateFileMappingFailed")], m_pUtil->AnsiToUnicode(pFileName));
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		m_dwCPKFileHandle = INVALID_FILE_HANDLE;

		return FALSE;
	}
	else
	{
		// �ļ�ӳ���Ѿ�����
		if( ::GetLastError() == ERROR_ALREADY_EXISTS )
		{
			ErrMsg(g_StrTable[_T("FileMappingAlreadyExist")], m_pUtil->AnsiToUnicode(pFileName));
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
BOOL CPKVfs::Load(LPCSTR pFileName)
{
	if( TRUE == m_bLoaded && FALSE == this->Unload() )	//��ȷ��ж��
		return FALSE;

	m_mapValidTable.clear();

	ASSERT(pFileName);
	if( CDiscIOMgr::IsExist(pFileName) == FALSE )
		return FALSE;

	strcpy(m_szCPKFileName, pFileName);

	m_dwCPKFileHandle = CDiscIOMgr::Create(pFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, // �Ƕ�ռģʽ
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if (m_dwCPKFileHandle == INVALID_FILE_HANDLE )
	{
		ErrMsg(g_StrTable[_T("OpenFileFailed")], m_pUtil->AnsiToUnicode(pFileName));
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		ASSERT(0);
		return FALSE;
	}

	// �����ļ�ͷ
	ZeroMemory(&m_CPKHeader, sizeof(m_CPKHeader));
	if( FALSE == CDiscIOMgr::Read(m_dwCPKFileHandle, &m_CPKHeader, sizeof(m_CPKHeader)) )
	{
		ErrMsg(g_StrTable[_T("ReadCPKHeadFailed")], m_pUtil->AnsiToUnicode(pFileName));
		return FALSE;
	}
	if (m_CPKHeader.dwLable != GT_CPK_LABEL)
	{
		ErrMsg(g_StrTable[_T("UnknowCPKFormat")], m_pUtil->AnsiToUnicode(pFileName));
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		ASSERT(0);
		return FALSE;
	}


	// �����ļ�Table
	// ���ǿ�������ռ乻����
	if( m_dwMaxCPKTable < m_CPKHeader.dwMaxTableNum )
	{
		SAFE_DEL_ARRAY(m_pCPKTable);
		m_pCPKTable = new tagCPKTable[m_CPKHeader.dwMaxTableNum];
		ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum);
		m_dwMaxCPKTable = m_CPKHeader.dwMaxTableNum;

		SAFE_DEL_ARRAY(m_pFragment);
		SAFE_DEL_ARRAY(m_pIndexToFragment);

		m_pFragment = new INT[m_dwMaxCPKTable];
		m_pIndexToFragment = new INT[m_dwMaxCPKTable];

		memset(m_pFragment, -1, sizeof(INT)*m_dwMaxCPKTable);
		memset(m_pIndexToFragment, -1, sizeof(INT)*m_dwMaxCPKTable);
	}

	//��λ����λ��
	if( FALSE == CDiscIOMgr::Seek(m_dwCPKFileHandle, m_CPKHeader.dwTableStart) )
	{
		ErrMsg(g_StrTable[_T("CPKSeekError")],
			m_pUtil->AnsiToUnicode(pFileName), m_CPKHeader.dwTableStart);

		return FALSE;
	}

	if( FALSE == CDiscIOMgr::Read(m_dwCPKFileHandle, m_pCPKTable,  
		sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum) )
	{
		ErrMsg(g_StrTable[_T("ReadCPKTableFailed")], m_pUtil->AnsiToUnicode(pFileName));
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
		ErrMsg(g_StrTable[_T("CreateFileMappingFailed")], m_pUtil->AnsiToUnicode(pFileName));
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		m_dwCPKFileHandle = INVALID_FILE_HANDLE;

		return FALSE;
	}
	else
	{
		// �ļ�ӳ���Ѿ�����
		if( ::GetLastError() == ERROR_ALREADY_EXISTS )
		{
			ErrMsg(g_StrTable[_T("FileMappingAlreadyExist")], m_pUtil->AnsiToUnicode(pFileName));
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
		CDiscIOMgr::CloseMappingHandle(m_dwCPKMappingHandle);
		m_dwCPKMappingHandle = 0;
		CDiscIOMgr::Close(m_dwCPKFileHandle);
		m_dwCPKFileHandle = INVALID_FILE_HANDLE;

		ZeroMemory(&m_CPKHeader, sizeof(m_CPKHeader));
		ZeroMemory(m_szCPKFileName, sizeof(m_szCPKFileName));
		ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_dwMaxCPKTable);

		memset(m_pFragment, 0xFF, sizeof(INT)*m_dwMaxCPKTable);
		memset(m_pIndexToFragment, 0xFF, sizeof(INT)*m_dwMaxCPKTable);

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
	if (m_CPKHeader.dwValidTableNum + 1 < m_CPKHeader.dwMaxTableNum)
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

		SAFE_DEL_ARRAY(m_pCPKTable);
		m_pCPKTable = pTable;


		INT *p1 = new INT[dwTable];
		INT *p2 = new INT[dwTable];
		memset(p1, 0xFF, sizeof(INT)*dwTable);
		memset(p2, 0xFF, sizeof(INT)*dwTable);

		memcpy(p1, m_pFragment, sizeof(INT)*m_CPKHeader.dwMaxTableNum);
		memcpy(p2, m_pIndexToFragment, sizeof(INT)*m_CPKHeader.dwMaxTableNum);

		SAFE_DEL_ARRAY(m_pFragment);
		SAFE_DEL_ARRAY(m_pIndexToFragment);
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
// �����ļ����Ƿ��ʾ���֮��Ҫ����cpk��������: data.cpk\system\gongfu.exe,
// �ⲿ����ȷ�����ļ�,������Ŀ¼������,data.cpk�������޴�֪�����Ƿ�Ŀ¼
//-----------------------------------------------------------------------------
BOOL CPKVfs::IsFileForCPK(LPCSTR szFullName, LPCSTR szCPKName, LPCSTR szFileName)
{
	if( NULL == szFullName ) 
		return FALSE;

	INT nLen = strlen(szFullName);
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
					((char*)szCPKName)[i] = 0;
				}
				if( NULL != szFileName )
				{
					strcpy((char*)szFileName, szFullName+i+1);
					((char*)szFileName)[nLen-i-1] = 0;
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}


//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
DWORD CPKVfs::Open(INT nTableIndex, DWORD dwLength, BOOL bCpk )
{
	ASSERT( IS_VALID_CPK_TABLE(nTableIndex) );

	// ����ʵ�ʿ�ʼӳ���λ��
	DWORD dwRealStartPos = m_pCPKTable[nTableIndex].dwStartPos + dwLength;
	dwRealStartPos = dwRealStartPos - (dwRealStartPos % m_dwAllocGranularity);

	// ʵ��ӳ������Ҫӳ��λ�õ�ƫ��
	DWORD dwOffset = m_pCPKTable[nTableIndex].dwStartPos + dwLength - dwRealStartPos;

	// ������Ҫӳ����ֽ�
	DWORD dwBytesToRead = m_pCPKTable[nTableIndex].dwPackedSize
		+ m_pCPKTable[nTableIndex].dwExtraInfoSize + dwOffset;

	// ����Ǵ��ļ���ֿ�ӳ��,���¼�����Ҫӳ����ֽ�
	dwBytesToRead -= dwLength;
	if(  IS_COMPRESSED( m_pCPKTable[nTableIndex].dwFlag ) == false
		&& IS_LARGE_FILE( m_pCPKTable[nTableIndex].dwFlag  ) )
	{
		if( dwBytesToRead >= GT_CPK_RAW_BUFFER_SIZE )
			dwBytesToRead = GT_CPK_RAW_BUFFER_SIZE + dwOffset;
	}

	// �����ļ�����
	CPKFile* fp = (CPKFile*)malloc(sizeof(CPKFile));
	if( !fp )
	{
		ErrMsg(g_StrTable[_T("NotEnoughMemoryForCreateFP")], m_pUtil->AnsiToUnicode(m_szCPKFileName));
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
	fp->dwMapSize = dwBytesToRead - dwOffset;

	GetFullNameFromCRC(fp->pTableItem->dwCRC, fp->szFullName);

	ASSERT( fp->dwFileSize>= 0 && fp->dwFileSize<0x40000000 );

	// �����ѹ���ļ�������Ҫ�Ƚ�ѹ��
	if( fp->bCompressed == TRUE && fp->dwFileSize > 0 && !bCpk )
	{
		// Ϊ��ѹ�����ڴ�
		fp->lpMem = malloc(fp->dwFileSize);
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
			ErrMsg(g_StrTable[_T("DecompressFailed")]);
			CDiscIOMgr::UnMapFile(fp->lpMapAddress);
			free(fp->lpMem);
			fp->bValid = FALSE;
			return 0;
		}
	}
	else	// ����ļ�û�б�ѹ����
	{
		// ����ָ��Map�е��ļ�ʵ����ʼ��ַ
		fp->lpMem = fp->lpStartAddress;
	}

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
BOOL CPKVfs::Close(DWORD dwHandle)
{
	CPKFile* fp = (CPKFile*)dwHandle;
	ASSERT(fp != NULL);
	ASSERT(fp->bValid == TRUE);

	if( fp->lpMapAddress == 0 )
		return FALSE;

	if( fp->bCompressed && fp->dwFileSize > 0  && fp->lpStartAddress != fp->lpMem )
		free( fp->lpMem );

	BOOL bResult = CDiscIOMgr::UnMapFile(fp->lpMapAddress);
	if( FALSE == bResult )
	{
		fp->bValid = FALSE;
		return FALSE;
	}

	free(fp);
	return TRUE;
}


//---------------------------------------------------------------------------------
// ͨ��crc�õ�ȫ·���ļ���
//---------------------------------------------------------------------------------
BOOL CPKVfs::GetFullNameFromCRC(DWORD dwCRC, LPSTR szFullName)
{
	static CHAR szTempNameNew[MAX_PATH*2];
	static CHAR szTempNameOld[MAX_PATH*2];
	BOOL bResult = FALSE;

	ZeroMemory(szTempNameNew, sizeof(szTempNameNew));
	ZeroMemory(szTempNameOld, sizeof(szTempNameOld));

	if( dwCRC == 0 )	// ��Ŀ¼
	{
		strcpy(szFullName, "");
		return TRUE;
	}

	//���ļ���
	if( FALSE == this->GetSimpleFileNameFromCRC(szTempNameOld, dwCRC) )
	{
		return FALSE;
	}

	INT nTableIndex = this->GetTableIndexFromeCrc(dwCRC);
	if( !IS_VALID_CPK_TABLE(nTableIndex) )
	{
		return FALSE;
	}

	dwCRC = (m_pCPKTable + nTableIndex)->dwFatherCRC;

	while(dwCRC != 0)
	{
		// �����ļ���ƴ��
		strcpy(szTempNameNew, "\\");
		strcat(szTempNameNew, szTempNameOld); // new: \abc

		this->GetSimpleFileNameFromCRC(szTempNameOld, dwCRC);
		strcat(szTempNameOld, szTempNameNew); // old: abc\abc

		if (strlen(szTempNameOld) >= MAX_PATH)
		{
			ErrMsg(g_StrTable[_T("TooLongFileName")], m_pUtil->AnsiToUnicode(szTempNameOld));
			return FALSE;
		}

		nTableIndex = this->GetTableIndexFromeCrc(dwCRC);
		ASSERT(nTableIndex != GT_INVALID);
		dwCRC = (m_pCPKTable + nTableIndex)->dwFatherCRC;

	}

	strcpy(szFullName, szTempNameOld);
	return TRUE;
}
//-----------------------------------------------------------------------------
// ��CRC��ʶ�õ����ļ���
//-----------------------------------------------------------------------------
BOOL CPKVfs::GetSimpleFileNameFromCRC(LPSTR pSimpleFileName, DWORD dwCRC)
{
	BOOL bResult = FALSE;
	ASSERT(pSimpleFileName);

	if (dwCRC == 0)
		return FALSE;

	INT nTableIndex = this->GetTableIndexFromeCrc(dwCRC);
	if( !IS_VALID_CPK_TABLE(nTableIndex) )
		return FALSE;

	// �Ӱ��еõ����ļ���(û��·��)
	static CHAR szTemp[MAX_PATH];

	tagCPKTable* pTableItem = m_pCPKTable + nTableIndex;
	if( FALSE == IS_FILE_OR_DIR_VALID(pTableItem->dwFlag) )
		return FALSE;

	// �����ļ�������Ϣ�����а����ļ�����Ϣ
	DWORD dwReadPos = pTableItem->dwStartPos + pTableItem->dwPackedSize;
	DWORD dwBytesToRead = pTableItem->dwExtraInfoSize;

	CDiscIOMgr::Seek(m_dwCPKFileHandle, dwReadPos);
	CDiscIOMgr::Read(m_dwCPKFileHandle, szTemp, dwBytesToRead);

	strcpy(pSimpleFileName, szTemp);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����Ƿ��ǺϷ���CPK�ļ���
//-----------------------------------------------------------------------------
BOOL CPKVfs::IsValidCPK(LPCSTR pFileName)
{
	ASSERT(pFileName);
	if( CDiscIOMgr::IsExist(pFileName) == FALSE )
	{
		ErrMsg(g_StrTable[_T("FileNotFound")], m_pUtil->AnsiToUnicode(pFileName));
		return FALSE;
	}
	//Ȼ�������ж���չ���Ƿ���.cpk
	static CHAR szFullName[MAX_PATH];
	LPSTR lpExtName = NULL;

	strcpy(szFullName, pFileName);
	lpExtName = strrchr(szFullName, '.');

	if( !lpExtName )
		return FALSE;

	if( _stricmp(lpExtName, ".cpk") != 0 )
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
		ErrMsg(g_StrTable[_T("OpenFileFailed")], m_pUtil->AnsiToUnicode(pFileName));
		return FALSE;
	}

	// �����ļ�ͷ
	tagCPKHeader header;
	ZeroMemory(&header, sizeof(header));
	CDiscIOMgr::Read(dwHandle, &header, sizeof(header));
	CDiscIOMgr::Close(dwHandle);

	// ����ļ���־
	if( header.dwLable != GT_CPK_LABEL )
		return FALSE;


	// ����ļ��汾��
	if( header.dwVersion != GT_CPK_VERSION )
		return FALSE;

	// ����ļ�Table����ʼλ��
	if( header.dwTableStart == 0 )
		return FALSE;

	// �������ļ���
	if( header.dwFileNum > header.dwMaxTableNum )
		return FALSE;

	// ��������Ч�����
	if( header.dwValidTableNum > header.dwMaxTableNum )
		return FALSE;

	// ����ļ�������Ч�����Ĺ�ϵ
	if( header.dwFileNum > header.dwValidTableNum )
		return FALSE;

	// �Ժ�汾�������ļ����Ƿ��С��������
	return TRUE;
}
//-----------------------------------------------------------------------------
//*********��ʼ��
// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
//������Ǽ��һ��ԭ��:���ļ����ݲ�ѹ��(Ҳ�����Ѿ�ѹ����),��չ��Ϣ����
//*********��ʼ��
//-----------------------------------------------------------------------------
INT CPKVfs::InitFileCPKToCPK(LPCSTR pFullName, DWORD dwFatherCRC, tagCPKTable *pTable)
{
	ASSERT(pFullName);


	// �Ƿ��ܷ����ļ�
	if( FALSE == IncreaseTable() )
		return FALSE;

	// ���ļ���ƴ��ȫ·����ת��ΪСд�ټ���CRC
	CHAR szFullName[MAX_PATH];
	this->ConvertFullNameToCPKFullName(szFullName, pFullName, dwFatherCRC);

	// ��Ҫ�������ظ��ļ������
	DWORD dwCRC = m_pUtil->Crc32(szFullName);
	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		RemoveFile(dwCRC);
	}

	// �õ�һ�����еı���ռ�
	int nTableIndex = GetFreeTableItem(dwCRC);
	if (nTableIndex == GT_INVALID)
	{
		ErrMsg(g_StrTable[_T("FindFreeCPKItemFailed")]);
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

	return nTableIndex;

}
//-----------------------------------------------------------------------------
//*********д������
// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
//������Ǽ��һ��ԭ��:���ļ����ݲ�ѹ��(Ҳ�����Ѿ�ѹ����),��չ��Ϣ����
//*********д������
//-----------------------------------------------------------------------------
BOOL CPKVfs::WriteFileCPKToCPK(INT nTableIndex, DWORD dwWritePos, LPVOID p, DWORD dwDataSize)
{
	tagCPKTable *pTable = m_pCPKTable + nTableIndex;

	// ���ļ��в�������
	// �ƶ��ļ�ָ��
	DWORD dwDistanceToMove = m_pCPKTable[nTableIndex].dwStartPos + dwWritePos;
	CDiscIOMgr::Seek(m_dwCPKFileHandle, dwDistanceToMove);
	DWORD dwDestFileSize = CDiscIOMgr::GetSize(m_dwCPKFileHandle);
	if (dwDistanceToMove >= dwDestFileSize)
		CDiscIOMgr::SetEOF(m_dwCPKFileHandle);

	// д��ʵ����Ϣ
	return CDiscIOMgr::Write(m_dwCPKFileHandle, p, dwDataSize);
}
//-----------------------------------------------------------------------------
//*********��������ļ���д����ͷ��
// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
//������Ǽ��һ��ԭ��:���ļ����ݲ�ѹ��(Ҳ�����Ѿ�ѹ����),��չ��Ϣ����
//*********��������ļ���д����ͷ��
//-----------------------------------------------------------------------------
BOOL CPKVfs::FinishFileCPKToCPK(INT nTableIndex)
{
	// �ļ�������һ
	m_CPKHeader.dwFileNum++;
	// ��Ч���������һ
	m_CPKHeader.dwValidTableNum++;


	// ���ļ�ͷ������д�����
	this->WriteHeaderToDisc();
	this->WriteTableToDisc();

	m_mapValidTable.insert(std::pair<DWORD, DWORD>(m_pCPKTable[nTableIndex].dwCRC, nTableIndex));
	return TRUE;
}
//-----------------------------------------------------------------------------
// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
//������Ǽ��һ��ԭ��:���ļ����ݲ�ѹ��(Ҳ�����Ѿ�ѹ����),��չ��Ϣ����
//-----------------------------------------------------------------------------
BOOL CPKVfs::AddFile(LPCSTR pFullName, DWORD dwFatherCRC, tagCPKTable *pTable, LPVOID p)
{
	ASSERT(pFullName);

	char *szName = (char *)p + pTable->dwPackedSize;

	// �Ƿ��ܷ����ļ�
	if( FALSE == IncreaseTable() )
		return FALSE;

	// ���ļ���ƴ��ȫ·����ת��ΪСд�ټ���CRC
	CHAR szFullName[MAX_PATH];
	this->ConvertFullNameToCPKFullName(szFullName, pFullName, dwFatherCRC);

	// ��Ҫ�������ظ��ļ������
	DWORD dwCRC = m_pUtil->Crc32(szFullName);
	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		RemoveFile(dwCRC);
	}

	// �õ�һ�����еı���ռ�
	int nTableIndex = GetFreeTableItem(dwCRC);
	if (nTableIndex == GT_INVALID)
	{
		ErrMsg(g_StrTable[_T("FindFreeCPKItemFailed")]);
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
BOOL CPKVfs::AddFile(LPCSTR pFullName, DWORD dwFatherCRC)
{
	ASSERT(pFullName);

	EFMFilter eFilter = FMF_Null;
	if( FALSE == IncreaseTable() )
		return FALSE;

	// ���ļ���ƴ��ȫ·����ת��ΪСд�ټ���CRC
	CHAR szFullName[MAX_PATH];
	this->ConvertFullNameToCPKFullName(szFullName, pFullName, dwFatherCRC);

	// ��Ҫ�������ظ��ļ������
	DWORD dwCRC = m_pUtil->Crc32(szFullName);
	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		RemoveFile(dwCRC);
	}

	// �õ�һ�����еı���ռ�
	int nTableIndex = GetFreeTableItem(dwCRC);
	if (nTableIndex == GT_INVALID)
	{
		ErrMsg(g_StrTable[_T("FindFreeCPKItemFailed")]);
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
		ErrMsg(g_StrTable[_T("OpenFileFailed")], m_pUtil->AnsiToUnicode(pFullName));
		return FALSE;
	}


	// ����ļ�����
	BOOL bResult = FALSE;
	DWORD dwSrcFileSize = CDiscIOMgr::GetSize(dwSrcHandle);
	DWORD dwFileFlag = 0;
	VOID* pSrc = NULL;
	VOID* pDest = NULL;
	pSrc = malloc(dwSrcFileSize);
	pDest = malloc(dwSrcFileSize);

	if( dwSrcFileSize >= GT_CPK_RAW_BUFFER_SIZE 
		|| IsUnCompressable(pFullName) )	// mp3/bink donot take compress
	{
		// �����ļ������ڴ�
		eFilter = FMF_Changeless;
		dwFileFlag = 0x1 | (eFilter<<16) | 0x4;	// ��Ч | ��ѹ�� | ���ļ�
		bResult = CDiscIOMgr::_LoadToMem(pSrc, dwSrcHandle, dwSrcFileSize);
		if( bResult == FALSE )
		{
			ErrMsg(g_StrTable[_T("ReadFileFailed")], m_pUtil->AnsiToUnicode(pFullName));
			CDiscIOMgr::Close(dwSrcHandle);
			free(pSrc);
			free(pDest);

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
			ErrMsg(g_StrTable[_T("ReadFileFailed")], m_pUtil->AnsiToUnicode(pFullName));
			CDiscIOMgr::Close(dwSrcHandle);
			free(pSrc);
			free(pDest);
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
		free(pSrc);
		free(pDest);
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
		ErrMsg(g_StrTable[_T("FailInAddFileToCPK")], m_pUtil->AnsiToUnicode(pFullName));
		ASSERT(0);
		CDiscIOMgr::Close(dwSrcHandle);
		free(pSrc);
		free(pDest);
		return FALSE;
	}
	// ���ļ�ͷ������д�����
	this->WriteHeaderToDisc();
	this->WriteTableToDisc();
	m_mapValidTable.insert(std::pair<DWORD, DWORD>(dwCRC, nTableIndex));


	free(pSrc);
	free(pDest);

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
BOOL CPKVfs::ConvertFullNameToCPKFullName(LPSTR pOutName, LPCSTR pInName, 
										  DWORD dwFatherCRC)
{
	ASSERT(pOutName);
	ASSERT(pInName);
	BOOL bResult = FALSE;

	static CHAR szSimpleName[MAX_PATH];
	static CHAR szCPKFullName[MAX_PATH];

	bResult = this->GetFullNameFromCRC(dwFatherCRC, szCPKFullName);

	ASSERT(bResult);

	if (FALSE == bResult)
		return FALSE;

	bResult = this->GetSimpleFileName(szSimpleName, pInName);
	ASSERT(bResult);
	if (FALSE == bResult)
		return FALSE;

	if (strlen(szCPKFullName) + strlen(szSimpleName) > MAX_PATH)
	{
		ErrMsg(g_StrTable[_T("TooLongFileName")], m_pUtil->AnsiToUnicode(szSimpleName));
		return FALSE;
	}

	// ����ĺ������Զ�ȫ��תΪСд
	this->LinkSimpleNameToDirName(szCPKFullName, szSimpleName);
	strcpy(pOutName, szCPKFullName);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ���Ӽ��ļ�����Ŀ¼����Ŀ��Ŀ¼��
//-----------------------------------------------------------------------------
BOOL CPKVfs::LinkSimpleNameToDirName(LPSTR pFullName, LPCSTR pSimpleName)
{
	ASSERT(pFullName);
	ASSERT(pSimpleName);

	INT nFullNameLen = strlen(pFullName);

	// ���Լ���ַ��������Ƿ��ܹ�����
	if( nFullNameLen >= MAX_PATH - 10 )
	{
		ErrMsg(g_StrTable[_T("TooLongFileName")], m_pUtil->AnsiToUnicode(pFullName));
		return FALSE;
	}

	// �Ȳ���Ŀ¼������û��"\"����
	if( nFullNameLen>0 && pFullName[nFullNameLen-1] != '\\' )
		strcat(pFullName, "\\");

	strcat(pFullName, pSimpleName);
	_strlwr(pFullName);	// ת��Сд
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
DWORD CPKVfs::CreateDir(LPCSTR pSimpleDirName, DWORD dwFatherCRC)
{
	ASSERT(pSimpleDirName);

	if( FALSE == IncreaseTable() )
		return FALSE;

	// ��Ŀ¼��ת��ΪСд�ټ���CRC
	CHAR szFullDirName[MAX_PATH];
	this->GetFullNameFromCRC(dwFatherCRC, szFullDirName);
	this->LinkSimpleNameToDirName(szFullDirName, pSimpleDirName);
	DWORD dwCRC = m_pUtil->Crc32(szFullDirName);

	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		//��Ŀ¼�Ѿ�����
		return dwCRC;
	}


	// �õ�һ�����еı���ռ�
	int nTableIndex = GetFreeTableItem(dwCRC);
	if (nTableIndex == GT_INVALID)
	{
		ErrMsg(g_StrTable[_T("FindFreeCPKItemFailed")]);
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

	m_mapValidTable.insert(std::pair<DWORD, DWORD>(dwCRC, nTableIndex));

	return dwCRC;

}

//-----------------------------------------------------------------------------
//���ַ�ʽ������ļ���ֻ�������·������·����ΪdwFatherCrc����Ŀ¼��Ϊ����������׼��
//-----------------------------------------------------------------------------
DWORD CPKVfs::AddDirCyc(LPCSTR pFullName, DWORD dwFatherCRC)
{
	ASSERT(pFullName);
	//����һ��һ������·��,����c:\bin\tidy, c:\, c:\bin\tidy

	CHAR szPathFile[MAX_PATH];
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
BOOL CPKVfs::AddRealDataToCPK(INT nTableIndex, LPCSTR pFileName,  VOID* pSrc, 
							  DWORD dwSrcFileSize)
{
	ASSERT(pFileName);
	ASSERT(IS_VALID_CPK_TABLE(nTableIndex));
	ASSERT(!IS_INVALID_HANDLE(m_dwCPKFileHandle));

	static CHAR szFileName[MAX_PATH];

	BOOL bResult = FALSE;
	DWORD dwDistanceToMove = 0;
	DWORD dwBytesToWrite = 0;

	static CHAR szSimpleName[MAX_PATH];
	this->GetSimpleFileName(szSimpleName, pFileName);
	dwBytesToWrite = (DWORD)strlen(szSimpleName);

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
		CMD5::MD5ForString((char*)szSimpleName, ExtraInfo.byMD5);
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
DWORD CPKVfs::CalFileExtraInfoSize(LPCSTR pFileName)
{
	DWORD dwSize = 0;
	dwSize += strlen(pFileName) + GT_CPK_LEN_OF_STRING_END;
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
	ASSERT( IS_VALID_CPK_TABLE(nTableIndex) );

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
	{
		return TRUE;	//ֱ�ӷ���
	}

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
BOOL CPKVfs::GetSimpleFileName(LPSTR pSimpleName, LPCSTR pFullName)
{
	ASSERT(pSimpleName);
	ASSERT(pFullName);

	LPCSTR lpSimpleName = strrchr(pFullName, '\\');
	if (NULL == lpSimpleName)
	{
		strcpy(pSimpleName, pFullName);
	}
	else
	{
		strcpy(pSimpleName, lpSimpleName+1);
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// �Ƿ��ǲ���ѹ���ļ�  mp3/bik
//-----------------------------------------------------------------------------
BOOL CPKVfs::IsUnCompressable(LPCSTR pFullName)
{
	static CHAR szFullName[MAX_PATH];
	LPSTR lpExtName = NULL;

	strcpy(szFullName, pFullName);
	lpExtName = strrchr(szFullName, _T('.'));

	if( !lpExtName )
		return FALSE;

	if( _stricmp(lpExtName, ".mp3") == 0 || _stricmp(lpExtName, ".bik") == 0 )
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
		ErrMsg(g_StrTable[_T("InvalidTableIndex")], nTableIndex);
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
// �õ�����ָ��Ŀ¼������һ�����ļ�
// ����������pdwCRCArray��ָ���˫������
// ���pdwCRCArray==NULL, �����CRC��ֻ���㲢���nChildNum
//-----------------------------------------------------------------------------
BOOL CPKVfs::GetFirstLevelChild(DWORD dwDirCRC, DWORD& dwChildNum, DWORD* pdwCRCArray)
{
	INT n = 0;
	DWORD dwCounter = 0;

	for (n=0; n<(INT)m_CPKHeader.dwValidTableNum; n++)
	{
		// ���Ӷ���
		if( dwDirCRC != m_pCPKTable[n].dwFatherCRC )
			continue;

		// �Ƿ�
		if( FALSE == IS_FILE_OR_DIR_VALID(m_pCPKTable[n].dwFlag) )
			continue;

		// ��Ŀ¼
		if( IS_DIR(m_pCPKTable[n].dwFlag) )
			continue;

		if (pdwCRCArray)
			pdwCRCArray[dwCounter] = m_pCPKTable[n].dwCRC;

		dwCounter++;
	}

	dwChildNum = dwCounter;
	return TRUE;
}
//-----------------------------------------------------------------------------
// �õ�����ָ��Ŀ¼�µ�����һ����Ŀ¼
// ����������pdwCRCArray��ָ���˫������
// ���pdwCRCArray==NULL, �����CRC��ֻ���㲢���nChildNum
//-----------------------------------------------------------------------------
BOOL CPKVfs::GetFirstLevelChildDir(DWORD dwDirCRC, DWORD& dwChildNum, 
								   DWORD* pdwCRCArray)
{
	INT n = 0;
	DWORD dwCounter = 0;

	for( n=0; n<(INT)m_CPKHeader.dwValidTableNum; n++ )
	{
		// ��Ŀ¼
		if( FALSE == IS_DIR(m_pCPKTable[n].dwFlag) )
			continue;

		// ���Ӷ���
		if( dwDirCRC != m_pCPKTable[n].dwFatherCRC )
			continue;

		// �Ƿ�
		if( FALSE == IS_FILE_OR_DIR_VALID(m_pCPKTable[n].dwFlag) )
			continue;

		if( pdwCRCArray )
			pdwCRCArray[dwCounter] = m_pCPKTable[n].dwCRC;

		dwCounter++;	// ���뵥������
	}

	dwChildNum = dwCounter;
	return TRUE;
}
//-----------------------------------------------------------------------------
// ��CRC��ʶ�õ������Ƿ���Ŀ¼
//-----------------------------------------------------------------------------
BOOL CPKVfs::IsDir(DWORD dwCRC)
{

	if( dwCRC == 0 )	// ������Ŀ¼
		return TRUE;

	// ��CRC�ҵ���Ӧ����
	INT n = GetTableIndexFromeCrc(dwCRC);

	if( IS_DIR((m_pCPKTable+n)->dwFlag) )
		return TRUE;

	return FALSE;
}
//-----------------------------------------------------------------------------