//-----------------------------------------------------------------------------
// File: CPKCore.cpp
// Desc: game resource file system 2.0
// Time: 2003-5-12
// Auth: CTCRST
//
// Copyright (c) 2003 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "cpkcore.h"

#include "Util.h"
#include "disciomgr.h"
#include "..\memmgr\memmgr.h"
#include "..\filtermgr\filtermgr.h"
#include "..\md5\md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = __FILE__;
#endif

#define   COMPRESS_SIZE				50 * 1024 * 1024

//-----------------------------------------------------------------------------
// CPK construction / destruction
//-----------------------------------------------------------------------------
CPK::CPK()
{
	ZeroMemory(this, sizeof(*this));
	
	Reset();

	m_pRawBuffer = CMemMgr::Alloc(GT_CPK_RAW_BUFFER_SIZE);
	
	
	m_dwPackBufferSize = COMPRESS_SIZE + 2 * 1024 * 1024;;
	m_pPackBuffer = CMemMgr::Alloc(m_dwPackBufferSize );
	m_dwRawBufferSize = GT_CPK_RAW_BUFFER_SIZE;
	

	m_pMD5 = new CMD5;
	m_dwAllocGranularity = this->GetAllocationGranularity();

	m_dwMaxCPKTable = GT_CPK_DEFAULT_FILE;
	m_pCPKTable = new tagCPKTable[m_dwMaxCPKTable];
	ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_dwMaxCPKTable);
	m_pFragment = new INT[m_dwMaxCPKTable];
	m_pIndexToFragment = new INT[m_dwMaxCPKTable];

	// ��¼��Ƭ������Table��
	memset(m_pFragment, -1, sizeof(INT)*m_dwMaxCPKTable);	
	// ��¼Table��������Ƭ��
	memset(m_pIndexToFragment, -1, sizeof(INT)*m_dwMaxCPKTable);

}

CPK::~CPK() 
{
	CMemMgr::Free(m_pRawBuffer);
	CMemMgr::Free(m_pPackBuffer);
	m_dwRawBufferSize = 0;
	m_dwPackBufferSize = 0;

	FREE(m_pMD5);
	FREEARR(m_pCPKTable);
	FREEARR(m_pFragment);
	FREEARR(m_pIndexToFragment);
}


VOID CPK::Reset()
{
	ZeroMemory(m_szCPKFileName, sizeof(m_szCPKFileName));
	ZeroMemory(m_szCurDir, sizeof(m_szCurDir));

	
	ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_dwMaxCPKTable);
	// ��¼��Ƭ������Table��
	memset(m_pFragment, -1, sizeof(INT)*m_dwMaxCPKTable);	
	// ��¼Table��������Ƭ��
	memset(m_pIndexToFragment, -1, sizeof(INT)*m_dwMaxCPKTable);

	m_dwCurDirCRC = 0;
	m_dwCPKHeaderAddr = 0;

}


//-----------------------------------------------------------------------------
// �����µ�ѹ����
//-----------------------------------------------------------------------------
BOOL CPK::Create(LPCTSTR pFileName)
{
	ASSERT(pFileName);


	if( m_bLoaded )	// �������ͷ�������
		this->Unload();
	else 
		this->Reset();

	// ������������Զ��򿪰�
	if (CDiscIOMgr::IsExist(pFileName) == TRUE)
	{
		this->Load(pFileName);
		return TRUE;
	}

	lstrcpy(m_szCPKFileName, pFileName);

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
		sizeof(tagCPKTable) * m_CPKHeader.dwMaxTableNum;


	m_dwCPKHandle = CDiscIOMgr::Create(pFileName,
		GENERIC_READ | GENERIC_WRITE,
		0, // ��ռģʽ
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if (m_dwCPKHandle == (DWORD)INVALID_HANDLE_VALUE)
	{
		Msg(_T("Couldn't open file"));
		return FALSE;
	}
	
	CDiscIOMgr::Write(m_dwCPKHandle, &m_CPKHeader, sizeof(m_CPKHeader));

	// ���յ�Tableд��CPK
	ZeroMemory(m_pCPKTable, sizeof(tagCPKTable)*m_dwMaxCPKTable);
	CDiscIOMgr::Write(m_dwCPKHandle, m_pCPKTable, 
		sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum);

	m_bLoaded = TRUE;
	return TRUE;
	
}




//-----------------------------------------------------------------------------
// ����ѹ����
//-----------------------------------------------------------------------------
BOOL CPK::Load(LPCTSTR pFileName)
{
	if( m_bLoaded )	// �������ͷ�������
		this->Unload();
	else 
		this->Reset();

	ASSERT(pFileName);
	if (CDiscIOMgr::IsExist(pFileName) == FALSE)
		return FALSE;

	lstrcpy(m_szCPKFileName, pFileName);

	m_dwCPKHandle = CDiscIOMgr::Create(pFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, // �Ƕ�ռģʽ
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if (m_dwCPKHandle == (DWORD)INVALID_HANDLE_VALUE)
	{
		Msg(_T("Couldn't open file %s"), pFileName);
		CDiscIOMgr::Close(m_dwCPKHandle);
		ASSERT(0);
		return FALSE;
	}

	// �����ļ�ͷ
	ZeroMemory(&m_CPKHeader, sizeof(m_CPKHeader));
	CDiscIOMgr::Read(m_dwCPKHandle, &m_CPKHeader, sizeof(m_CPKHeader));
	if (m_CPKHeader.dwLable != GT_CPK_LABEL)
	{
		Msg(_T("Unknow file format %s"), pFileName);
		CDiscIOMgr::Close(m_dwCPKHandle);
		ASSERT(0);
		return FALSE;
	}

	//��λ����λ��
	if( FALSE == CDiscIOMgr::Seek(m_dwCPKHandle, m_CPKHeader.dwTableStart) )
	{
		Msg(_T("Could not seek table from the CPK %s, seekpos=%ld"), \
			pFileName, m_CPKHeader.dwTableStart);

		return FALSE;
	}
	
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

		// ��¼��Ƭ������Table��
		memset(m_pFragment, -1, sizeof(INT)*m_dwMaxCPKTable);	
		// ��¼Table��������Ƭ��
		memset(m_pIndexToFragment, -1, sizeof(INT)*m_dwMaxCPKTable);
	}

	// �����ļ�Table
	BOOL bResult = CDiscIOMgr::Read(m_dwCPKHandle, m_pCPKTable, 
		sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum);
	if (FALSE == bResult)
	{
		Msg(_T("Could not load table from the CPK %s"), pFileName);
		return FALSE;
	}
	
	// �õ�������Ƭ��Ϣ
	GetAllFragment();

	m_bLoaded = TRUE;
	return TRUE;
}




//-----------------------------------------------------------------------------
// ж��ѹ����
//-----------------------------------------------------------------------------
BOOL CPK::Unload()
{
	if (m_bLoaded)
	{
		CDiscIOMgr::Close(m_dwCPKHandle);
		m_dwCPKHandle = 0;

		ZeroMemory(&m_CPKHeader, sizeof(m_CPKHeader));
		m_dwCurDirCRC = 0;
		m_bLoaded = FALSE;
		this->Reset();

		return TRUE;
	}

	return FALSE;
}
//-----------------------------------------------------------------------------
//���ӱ���ռ�
//-----------------------------------------------------------------------------
BOOL CPK::IncreaseTable()
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
// ����Ƿ��ǺϷ���CPK�ļ���
//-----------------------------------------------------------------------------
BOOL CPK::IsValidCPK(LPCTSTR pFileName)
{
	ASSERT(pFileName);
	if( CDiscIOMgr::IsExist(pFileName) == FALSE )
	{
		Msg(_T("File %s doesn't exsit"), pFileName);
		return FALSE;
	}

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
// �ڰ���ָ��Ŀ¼����һ����Ŀ¼, ����CRC
//-----------------------------------------------------------------------------
DWORD CPK::CreateDir(LPCTSTR pSimpleDirName, DWORD dwFatherCRC)
{
	ASSERT(pSimpleDirName);

	if( FALSE == IncreaseTable() )
		return FALSE;

	// ��Ŀ¼��ת��ΪСд�ټ���CRC
	TCHAR szFullDirName[MAX_PATH];
	this->GetFullNameFromCRC(szFullDirName, dwFatherCRC);
	this->LinkSimpleNameToDirName(szFullDirName, pSimpleDirName);
	DWORD dwCRC = CRC32(szFullDirName);

	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		TCHAR szTempFullName[MAX_PATH];
		this->GetFullNameFromCRC(szTempFullName, dwCRC);
	//	Msg(_T("���ظ���CRC:%lu, Ŀ¼:%s, �ظ�Ŀ¼:%s"), 
	//		dwCRC, szFullDirName, szTempFullName);

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
DWORD CPK::AddDir(LPCTSTR pFullName, DWORD dwFatherCRC)
{
	ASSERT(pFullName);
	//����һ��һ������·��,����c:\bin\tidy, c:\, c:\bin\tidy

	TCHAR szPathFile[MAX_PATH];
	INT i=0, j=0;
	for(i=0, j=0; pFullName[i] != '\0'; i++)
	{
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
//������ĺ�����ͬ,���ַ�ʽ����ļ�����·��Ҳ��Ҫ����,��·����ΪdwFatherCrc����Ŀ¼
//-----------------------------------------------------------------------------
BOOL CPK::AddFileAndDir(LPCTSTR pFullName, DWORD dwFatherCRC)
{
	ASSERT(pFullName);
	//����һ��һ������·��,����c:\bin\tidy, c:\, c:\bin\tidy
	
	TCHAR szPathFile[MAX_PATH];
	for(int i=0, j=0; pFullName[i] != '\0'; i++)
	{
		if( pFullName[i] == '\\' && i > 0 )
		{
			ZeroMemory(szPathFile, sizeof(szPathFile));
			strncpy(szPathFile, pFullName+j, i-j);
			dwFatherCRC = CreateDir(szPathFile, dwFatherCRC);

			j = i+1;
		}
	}

	return AddFile(pFullName, dwFatherCRC);
}
//-----------------------------------------------------------------------------
// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
//-----------------------------------------------------------------------------
BOOL CPK::AddFile(LPCTSTR pFullName, DWORD dwFatherCRC, tagCPKTable *pTable, LPVOID p)
{
	ASSERT(pFullName);

	TCHAR *szName = (TCHAR *)p + pTable->dwPackedSize;

	// �Ƿ��ܷ����ļ�
	if( FALSE == IncreaseTable() )
		return FALSE;	

	BOOL bDir = CDiscIOMgr::IsDir(pFullName);
	if( bDir == TRUE )
	{
		DWORD dwResult = CreateDir(pFullName, dwFatherCRC);

		return dwResult != GT_INVALID ? TRUE : FALSE;
	}

	// ���ļ���ƴ��ȫ·����ת��ΪСд�ټ���CRC
	TCHAR szFullName[MAX_PATH];
	this->ConvertFullNameToCPKFullName(szFullName, pFullName, dwFatherCRC);

	// ��Ҫ�������ظ��ļ������
	DWORD dwCRC = CRC32(szFullName);
	if( this->HaveRepeatCRC(dwCRC) == TRUE )
	{
		TCHAR szTempFullName[MAX_PATH];
		this->GetFullNameFromCRC(szTempFullName, dwCRC);
		//Msg(_T("���ظ���CRC:%lu, �ļ�:%s, �ظ��ļ�:%s"),
		//	dwCRC, szFullName, szTempFullName);

		return FALSE;
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
	CDiscIOMgr::Seek(m_dwCPKHandle, dwDistanceToMove);
	DWORD dwDestFileSize = CDiscIOMgr::GetSize(m_dwCPKHandle);
	if (dwDistanceToMove >= dwDestFileSize)
		CDiscIOMgr::SetEOF(m_dwCPKHandle);

	// д��ʵ����Ϣ
	CDiscIOMgr::Write(m_dwCPKHandle, p, dwDataSize);			


	// ���ļ�ͷ������д�����
	this->WriteHeaderToDisc();
	this->WriteTableToDisc();

	return TRUE;
}
//-----------------------------------------------------------------------------
// ������ĵ�ǰĿ¼����ļ�
//-----------------------------------------------------------------------------
BOOL CPK::AddFile(LPCTSTR pFullName, DWORD dwFatherCRC)
{
	ASSERT(pFullName);

	// �Ƿ��ܷ����ļ�
	if( FALSE == IncreaseTable() )
		return FALSE;

	BOOL bDir = CDiscIOMgr::IsDir(pFullName);

	m_dwFilesCurProceed = 0;
	BOOL bResult = this->AddFileOrDir(pFullName, dwFatherCRC, bDir);


	// ���ļ�ͷ������д�����
	this->WriteHeaderToDisc();
	this->WriteTableToDisc();

	return bResult;
}





//-----------------------------------------------------------------------------
// �ͷ��ļ�
//-----------------------------------------------------------------------------
BOOL CPK::ExtractFile(LPCTSTR pDestDir, LPCTSTR pFullName)
{
	ASSERT(pDestDir);
	ASSERT(pFullName);

	DWORD dwCRC = 0;
	this->GetCRCFromCPKFullName(dwCRC, pFullName);



	DWORD dwFatherCrc = this->GetFatherCRC(dwCRC);
	BOOL bResult = this->ExtractFileOrDir(pDestDir, dwCRC, dwFatherCrc);
	
	return bResult;

}



//-----------------------------------------------------------------------------
// ɾ��������ļ�����Ŀ¼��ʹ������·��
//-----------------------------------------------------------------------------
BOOL CPK::RemoveFile(LPCTSTR pFullName)
{
	ASSERT(pFullName);

	// �ҵ����е��ļ�Table��
	INT nTableIndex = this->GetTableIndex(pFullName);
	if( nTableIndex == GT_INVALID )
	{
		Msg(_T("Couldn't find file : %s in this package"), pFullName);
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
// �ı䵱ǰĿ¼
//-----------------------------------------------------------------------------
BOOL CPK::SetCurDir(LPCTSTR pFullDirName)
{
	ASSERT(pFullDirName);
	
	if( lstrlen(pFullDirName) == 0 )	// ��Ŀ¼
	{
		// ��¼��Ŀ¼��
		lstrcpy(m_szCurDir, pFullDirName);
		// ��¼��Ŀ¼CRC
		m_dwCurDirCRC = 0;
		return TRUE;
	}

	if( lstrlen(pFullDirName) > MAX_PATH )
	{
		Msg(_T("Too long directory name : %s"), pFullDirName);
		return FALSE;
	}

	// ȫ��ת��Сд
	INT nTableIndex = GetTableIndex(pFullDirName);
	if( nTableIndex == GT_INVALID )
	{
		Msg(_T("Couldn't found directory : %s"), pFullDirName);
		return FALSE;
	}
	
	// ��¼��ǰĿ¼��
	lstrcpy(m_szCurDir, pFullDirName);
	// ��¼��ǰĿ¼CRC
	m_dwCurDirCRC = m_pCPKTable[nTableIndex].dwCRC;

	return TRUE;
}




//-----------------------------------------------------------------------------
// �õ�����ָ���ļ�����Ŀ¼�ĸ�Ŀ¼CRC
//-----------------------------------------------------------------------------
DWORD CPK::GetFatherCRC(DWORD dwCRC)
{
	DWORD dwFatherCRC = 0;

	if( dwCRC == 0 )// ��Ŀ¼û�и�Ŀ¼
		return 0;

	INT nTableIndex = this->GetTableIndexFromCRC(dwCRC);
	if( nTableIndex == GT_INVALID )
	{
		Msg(_T("Couldn't find file which CRC=%lu"), dwCRC);
		return 0;
	}

	return m_pCPKTable[nTableIndex].dwFatherCRC;
}

//-----------------------------------------------------------------------------
// �õ�����ָ��Ŀ¼������һ�����ļ�
// ����������pdwCRCArray��ָ���˫������
// ���pdwCRCArray==NULL, �����CRC��ֻ���㲢���nChildNum
//-----------------------------------------------------------------------------
BOOL CPK::GetFirstLevelChild(DWORD dwDirCRC, DWORD& dwChildNum, DWORD* pdwCRCArray)
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
// �Ǳ��Ż��������õ�����ָ��Ŀ¼������һ�����ļ�
//-----------------------------------------------------------------------------
BOOL CPK::_GetFirstLevelChild(DWORD dwDirCRC, DWORD& dwChildNum)
{

	dwChildNum = 0;
	for( INT n=0; n<(INT)m_CPKHeader.dwValidTableNum; n++ )
	{
		// ���Ӷ���
		if( dwDirCRC != m_pCPKTable[n].dwFatherCRC )
			continue;

		// �Ƿ�
		if( FALSE == IS_FILE_OR_DIR_VALID(m_pCPKTable[n].dwFlag) )
			continue;

		if( IS_DIR(m_pCPKTable[n].dwFlag) )
			m_nTempArray[dwChildNum++] = n;
	}

	for( INT n=0; n<(INT)m_CPKHeader.dwValidTableNum; n++ )
	{
		// ���Ӷ���
		if( dwDirCRC != m_pCPKTable[n].dwFatherCRC )
			continue;

		// �Ƿ�
		if( FALSE == IS_FILE_OR_DIR_VALID(m_pCPKTable[n].dwFlag) )
			continue;

		if( !IS_DIR(m_pCPKTable[n].dwFlag) )
			m_nTempArray[dwChildNum++] = n;
	}

	return TRUE;
}



//-----------------------------------------------------------------------------
// �õ�����ָ��Ŀ¼�µ�����һ����Ŀ¼
// ����������pdwCRCArray��ָ���˫������
// ���pdwCRCArray==NULL, �����CRC��ֻ���㲢���nChildNum
//-----------------------------------------------------------------------------
BOOL CPK::GetFirstLevelChildDir(DWORD dwDirCRC, DWORD& dwChildNum, 
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
// �ɾ���·�����ļ����õ�CRC��ʶ��������в����ڴ��ļ��򷵻�FALSE
//-----------------------------------------------------------------------------
BOOL CPK::GetCRCFromCPKFullName(DWORD& dwCRC, LPCTSTR pFullName)
{
	ASSERT(pFullName);
	if (lstrlen(pFullName) > MAX_PATH)
	{
		Msg(_T("File name too long : %s"), pFullName);
		return FALSE;
	}

	if (lstrlen(pFullName) == 0)	// ��Ŀ¼
	{
		dwCRC = 0;
		return TRUE;
	}

	// �ȵõ�Table���
	INT nTableIndex = this->GetTableIndex(pFullName);
	if (nTableIndex == GT_INVALID)
	{
		Msg(_T("File : %s not found in the package"), pFullName);
		ASSERT(0);
		return FALSE;
	}

	dwCRC = m_pCPKTable[nTableIndex].dwCRC;
	return TRUE;
}




//-----------------------------------------------------------------------------
// ��CRC��ʶ�õ����ļ���
//-----------------------------------------------------------------------------
BOOL CPK::GetSimpleFileNameFromCRC(LPTSTR pSimpleFileName, DWORD dwCRC)
{
	BOOL bResult = FALSE;
	ASSERT(pSimpleFileName);

	if (dwCRC == 0)
	{
		Msg( _T("CRC == 0 in function GetFileNameFromCRC"));
		return FALSE;
	}

	INT nTableIndex = this->GetTableIndexFromCRC(dwCRC);

	// �Ӱ��еõ����ļ���(û��·��)
	bResult = this->GetSimpleFileName(pSimpleFileName, nTableIndex);
	if (bResult == FALSE)
	{
		Msg(_T("Could not get the simple file name"));
		return FALSE;
	}
	return TRUE;

}

//-----------------------------------------------------------------------------
// �ɱ�����ŵõ�CPK���и��ļ���md5У��ֵ
//-----------------------------------------------------------------------------
BOOL CPK::GetMD5FromTableIndex(BYTE digest[16], DWORD dwTableIndex)
{
	
	if( FALSE == IS_FILE_OR_DIR_VALID(m_pCPKTable[dwTableIndex].dwFlag) )
		return FALSE;

	DWORD dwReadPos = m_pCPKTable[dwTableIndex].dwStartPos + m_pCPKTable[dwTableIndex].dwPackedSize + \
		m_pCPKTable[dwTableIndex].dwExtraInfoSize - 16;
	CDiscIOMgr::Seek(m_dwCPKHandle, dwReadPos);
	CDiscIOMgr::Read(m_dwCPKHandle, digest, 16);

	return TRUE;
}
//-----------------------------------------------------------------------------
// ��CRC��ʶ�õ�CPK���и��ļ���md5У��ֵ
//-----------------------------------------------------------------------------
BOOL CPK::GetMD5FromCrc(BYTE digest[16], DWORD dwCrc)
{
	INT nTableIndex =  GetTableIndexFromCRC(dwCrc);
	if( GT_INVALID == nTableIndex )
		return FALSE;
	if( FALSE == IS_FILE_OR_DIR_VALID(m_pCPKTable[nTableIndex].dwFlag) )
		return FALSE;

	DWORD dwReadPos = m_pCPKTable[nTableIndex].dwStartPos + m_pCPKTable[nTableIndex].dwPackedSize + \
						m_pCPKTable[nTableIndex].dwExtraInfoSize - 16;
	CDiscIOMgr::Seek(m_dwCPKHandle, dwReadPos);
	CDiscIOMgr::Read(m_dwCPKHandle, digest, 16);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��CRC��ʶ�õ�CPK���д�·���ļ�ȫ��
//-----------------------------------------------------------------------------
BOOL CPK::GetFullNameFromCRC(LPTSTR pFullName, DWORD dwCRC)
{
	ASSERT(pFullName);
	static TCHAR szTempNameNew[MAX_PATH*2];
	static TCHAR szTempNameOld[MAX_PATH*2];
	BOOL bResult = FALSE;
	INT nTableIndex = GT_INVALID;

	ZeroMemory(szTempNameNew, sizeof(szTempNameNew));
	ZeroMemory(szTempNameOld, sizeof(szTempNameOld));

	if (dwCRC == 0)	// ��Ŀ¼
	{
		lstrcpy(pFullName, _T(""));
		return TRUE;
	}

	bResult = this->GetSimpleFileNameFromCRC(szTempNameOld, dwCRC);
	if (FALSE == bResult)
	{
		ASSERT(0);
		Msg(_T("û���ڰ����ҵ�ָ���ļ� CRC%lu"), dwCRC);
		return FALSE;
	}

	nTableIndex = this->GetTableIndexFromCRC(dwCRC);
	ASSERT(nTableIndex != GT_INVALID);
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

		nTableIndex = this->GetTableIndexFromCRC(dwCRC);
		ASSERT(nTableIndex != GT_INVALID);
		dwCRC = (m_pCPKTable + nTableIndex)->dwFatherCRC;

	}

	lstrcpy(pFullName, szTempNameOld);
	return TRUE;
}




//-----------------------------------------------------------------------------
// ��CRC��ʶ��FatherCRC�õ�CPK���д����·�����ļ���
//-----------------------------------------------------------------------------
BOOL CPK::GetRelativeNameFromCRC(LPSTR pNameOut, DWORD dwCRC, DWORD dwFatherCRC)
{
	ASSERT(pNameOut);
	static TCHAR szTempNameNew[MAX_PATH*2];
	static TCHAR szTempNameOld[MAX_PATH*2];
	BOOL bResult = FALSE;
	INT nTableIndex = GT_INVALID;

	ZeroMemory(szTempNameNew, sizeof(szTempNameNew));
	ZeroMemory(szTempNameOld, sizeof(szTempNameOld));

	if (dwCRC == 0)	// ��Ŀ¼
	{
		lstrcpy(pNameOut, _T(""));
		return TRUE;
	}

	if (dwCRC == dwFatherCRC)	// ͬһĿ¼
	{
		lstrcpy(pNameOut, _T(""));
		return TRUE;
	}

	bResult = this->GetSimpleFileNameFromCRC(szTempNameOld, dwCRC);
	if (FALSE == bResult)
	{
		ASSERT(0);
		Msg(_T("û���ڰ����ҵ�ָ���ļ� CRC%lu"), dwCRC);
		return FALSE;
	}

	nTableIndex = this->GetTableIndexFromCRC(dwCRC);
	ASSERT(nTableIndex != GT_INVALID);
	dwCRC = (m_pCPKTable + nTableIndex)->dwFatherCRC;

	while(dwCRC != dwFatherCRC)
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

		nTableIndex = this->GetTableIndexFromCRC(dwCRC);
		ASSERT(nTableIndex != GT_INVALID);
		dwCRC = (m_pCPKTable + nTableIndex)->dwFatherCRC;
	}

	lstrcpy(pNameOut, szTempNameOld);
	return TRUE;
}

//-----------------------------------------------------------------------------
// ����������CRC�õ����е�Table��
//-----------------------------------------------------------------------------
INT	CPK::GetFreeTableItem(DWORD dwCRC)
{
	if( m_CPKHeader.dwValidTableNum >= m_CPKHeader.dwMaxTableNum -1 )
	{
		ASSERT( 0 );


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
DWORD CPK::FindCPKFreePlace(DWORD dwSize)
{
	INT nFrag = GT_INVALID;
	DWORD dwPos = 0;

	// �ȼ����û��������dwSize�Ŀռ���Ƭ,���������ҵ��Ŀռ�
	//for( INT n=0; (DWORD)n<m_CPKHeader.dwFragmentNum; n++ )
	//{
	//	nFrag = m_pFragment[n];

	//	ASSERT( IS_VALID_CPK_TABLE(nFrag) );
	//	ASSERT( IS_FILE_REMOVED(m_pCPKTable[nFrag].dwFlag) );	// ȷ������Ƭ

	//	if( m_pCPKTable[nFrag].dwPackedSize >= dwSize )
	//	{
	//		dwPos = m_pCPKTable[nFrag].dwStartPos;

	//		m_pCPKTable[nFrag].dwStartPos += dwSize;
	//		m_pCPKTable[nFrag].dwPackedSize -= dwSize;	// ��Ƭ��С
	//		return dwPos;
	//	}
	//}

	DWORD dwMinDiff = -1, dwDiffTemp = 0, dwIndex = 0;
	// �ȼ����û��������dwSize�Ŀռ���Ƭ������������Ĵ�С������Ŀռ�
	for( INT n=0; (DWORD)n<m_CPKHeader.dwFragmentNum; n++ )
	{
		nFrag = m_pFragment[n];

		ASSERT( IS_VALID_CPK_TABLE(nFrag) );
		ASSERT( IS_FILE_REMOVED(m_pCPKTable[nFrag].dwFlag) );	// ȷ������Ƭ

		if( m_pCPKTable[nFrag].dwPackedSize >= dwSize )
		{
			dwDiffTemp = m_pCPKTable[nFrag].dwPackedSize - dwSize;

			//�������������С���������Ƭ
			if( dwMinDiff == -1 )
			{
				dwMinDiff = dwDiffTemp;
				dwIndex = nFrag;
			}
			else if( dwDiffTemp < dwMinDiff )
			{
				dwMinDiff = dwDiffTemp;
				dwIndex = nFrag;
			}
		}
	}

	//����Ƭ���д���������Ƭ�Ĵ�С��������Ƭ����ʼλ��
	if( dwMinDiff != -1 )
	{
		dwPos = m_pCPKTable[dwIndex].dwStartPos;

		m_pCPKTable[dwIndex].dwStartPos += dwSize;
		m_pCPKTable[dwIndex].dwPackedSize -= dwSize;	// ��Ƭ��С
		return dwPos;
	}

	// ���û����ֱ�Ӹ��ӵ�����table�ļ�֮ǰ
	DWORD dwPackSize = m_CPKHeader.dwPackageSize;
	m_CPKHeader.dwPackageSize += dwSize;
	m_CPKHeader.dwTableStart = m_CPKHeader.dwPackageSize - \
		sizeof(tagCPKTable) * m_CPKHeader.dwMaxTableNum + m_dwCPKHeaderAddr;  //����λ��(�����)

	return m_dwCPKHeaderAddr + dwPackSize - sizeof(tagCPKTable)*m_CPKHeader.dwMaxTableNum;

}




//-----------------------------------------------------------------------------
// ����Ƿ������������CRC�ظ���CRC
//-----------------------------------------------------------------------------
BOOL CPK::HaveRepeatCRC(DWORD dwCRC)
{
	INT nTableIndex = this->GetTableIndexFromCRC(dwCRC);
	if (nTableIndex == GT_INVALID)
		return FALSE;
	else
		return TRUE;
}


//-----------------------------------------------------------------------------
// ��ӵ����ļ���ָ��Ŀ¼
//-----------------------------------------------------------------------------
BOOL CPK::AddSingleFile(LPCTSTR pFullName, DWORD dwFatherCRC)
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
		TCHAR szTempFullName[MAX_PATH];
		this->GetFullNameFromCRC(szTempFullName, dwCRC);
		//Msg(_T("���ظ���CRC:%lu, �ļ�:%s, �ظ��ļ�:%s"),
		//	dwCRC, szFullName, szTempFullName);

		return FALSE;
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
//==================================================================================
	pSrc = CMemMgr::Alloc( dwSrcFileSize );
	bResult = CDiscIOMgr::_LoadToMem( pSrc, dwSrcHandle, dwSrcFileSize );
	if( bResult == FALSE )
	{
		Msg(_T("Couldn't Load File %s, AddFile failed"), pFullName);
		CDiscIOMgr::Close(dwSrcHandle);
		CMemMgr::Free(pSrc);
		return FALSE;
	}

	// ��д�������������
	tagFilterParam param;

	param.lpMemIn = pSrc;
	param.dwInBufferSize = dwSrcFileSize;
	param.dwInDataSize = dwSrcFileSize;	

	//Ԥ���ж��Ƿ�ѹ��
	if( dwSrcFileSize <  COMPRESS_SIZE )
	{
		param.bCompress = TRUE;
		param.eFilter = FMF_MiniLzo1040;
		dwFileFlag = 0x1 | (param.eFilter<<16); // ��Ч | LZOѹ��
	}
	else 
	{
		param.bCompress = FALSE;
		param.eFilter = FMF_Changeless;
		dwFileFlag = 0x1 | (param.eFilter<<16) | 0x4;	// ��Ч | ��ѹ�� | ���ļ�

	}

	//��������ڴ�ָ���λ��
	if( param.bCompress == TRUE )
	{
		param.lpMemOut = m_pPackBuffer;
		param.dwOutBufferSize = m_dwPackBufferSize;
	}
	else
	{
		param.lpMemOut = pSrc;
		param.dwOutBufferSize = dwSrcFileSize;
	}

	// �����ļ�
	CFilterMgr::Filter(&param);
	if (param.bSuccess == FALSE)
	{
		CMemMgr::Free( pSrc );
		/*if( pOut != NULL )
		CMemMgr::Free( pOut );*/

		return FALSE;
	}

	//�ж�ѹ���Ƿ�����Ծ����Ƿ�ȡ��ѹ��
	if( param.bCompress == TRUE )
	{
		DWORD dwPercent = ((double)param.dwOutDataSize / (double)dwSrcFileSize) * 100;
		if(dwPercent > 95 )
		{
			param.bCompress = FALSE;
			param.eFilter = FMF_Changeless;
			dwFileFlag = 0x1 | (param.eFilter<<16) | 0x4;	// ��Ч | ��ѹ�� | ���ļ�

			param.lpMemOut = pSrc;
			param.dwOutBufferSize = dwSrcFileSize;
			param.dwOutDataSize = dwSrcFileSize;
		}
	}
//==================================================================================
	//if( dwSrcFileSize >= GT_CPK_RAW_BUFFER_SIZE 
	//	|| IsUnCompressable(pFullName) )	// mp3/bink donot take compress	
	//{
	//	// �����ļ������ڴ�
	//	eFilter = FMF_Changeless;
	//	dwFileFlag = 0x1 | (eFilter<<16) | 0x4;	// ��Ч | ��ѹ�� | ���ļ�
	//	pSrc = CMemMgr::Alloc(dwSrcFileSize);
	//	bResult = CDiscIOMgr::_LoadToMem(pSrc, dwSrcHandle, dwSrcFileSize);
	//	if( bResult == FALSE )
	//	{
	//		Msg(_T("Couldn't Load File %s, AddFile failed"), pFullName);
	//		CDiscIOMgr::Close(dwSrcHandle);
	//		CMemMgr::Free(pSrc);
	//		return FALSE;
	//	}
	//}
	//else
	//{
	//	eFilter = FMF_MiniLzo1040;
	//	dwFileFlag = 0x1 | (eFilter<<16); // ��Ч | LZOѹ��

	//	// ��Դ�ļ������ڴ�
	//	bResult = CDiscIOMgr::_LoadToMem(m_pRawBuffer, dwSrcHandle, dwSrcFileSize);
	//	if (bResult == FALSE)
	//	{
	//		Msg(_T("Couldn't Load File %s, AddFile failed"), pFullName);
	//		CDiscIOMgr::Close(dwSrcHandle);
	//	}
	//	pSrc = m_pRawBuffer;
	//}


	//// ��д�������������
	//tagFilterParam param;
	//param.eFilter = eFilter;
	//param.bCompress = TRUE;
	//param.lpMemIn = pSrc;
	//param.dwInBufferSize = m_dwRawBufferSize;
	//param.lpMemOut = m_pPackBuffer;
	//param.dwOutBufferSize = m_dwPackBufferSize;
	//param.dwInDataSize = dwSrcFileSize;	
	//	
	//
	//// �����ļ�
	//CFilterMgr::Filter(&param);
	//if (param.bSuccess == FALSE)
	//	return FALSE;

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

	BYTE digest[16];
	CMD5::MD5ForBuffer((BYTE*)pSrc, dwSrcFileSize, digest);

	// ���ļ��в�������
	bResult = AddRealDataToCPK(nTableIndex, pFullName, param.lpMemOut, param.dwOutDataSize, digest);
	if (bResult == FALSE)
	{
		Msg(_T("Couldn't add data to cpk, file name : %s"),	pFullName);
		ASSERT(0);
	}


	// �ͷ�
	if(pSrc != m_pRawBuffer)
		CMemMgr::Free(pSrc);

	CDiscIOMgr::Close(dwSrcHandle);

	return bResult;

}



//-----------------------------------------------------------------------------
// ����ļ���Ŀ¼��ָ��Ŀ¼���������µ������������ݹ�
//-----------------------------------------------------------------------------
BOOL CPK::AddFileOrDir(LPCTSTR pFullName, DWORD dwFatherCRC, BOOL bDir)
{
	static TCHAR szTempInfo[MAX_PATH];
	TCHAR szSimpleName[MAX_PATH];
	TCHAR szTemp[MAX_PATH];
	WIN32_FIND_DATA FindData;
	DWORD dwCRC = 0;
	BOOL bResult = FALSE;

	if( bDir )
	{
		this->GetSimpleFileName(szSimpleName, pFullName);
		dwCRC = this->CreateDir(szSimpleName, dwFatherCRC);
		if( dwCRC == GT_INVALID )
			return FALSE;


		// �������µ��ļ�������
		lstrcpy(szTemp, pFullName);
		lstrcat(szTemp, _T("\\*.*"));
		DWORD dwHandle = CDiscIOMgr::FindFirstFile(szTemp, &FindData);
		if( dwHandle == (DWORD)INVALID_HANDLE_VALUE )
			return FALSE;

		// Ŀ¼Ҳ��һ���ļ�
		m_dwFilesCurProceed++;

		while( CDiscIOMgr::FindNextFile(dwHandle, &FindData) )
		{
			if( FindData.cFileName[0] != _T('.') )
			{
				lstrcpy(szTemp, pFullName);
				lstrcat(szTemp, _T("\\"));
				lstrcat(szTemp, FindData.cFileName);
				bResult = this->AddFileOrDir(szTemp, dwCRC, FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				if( bResult == FALSE )
					break;
			}
		}

		CDiscIOMgr::FindClose(dwHandle);
	}
	else
	{
		m_dwFilesCurProceed++;


		// ���뵥���ļ�
		bResult = this->AddSingleFile(pFullName, dwFatherCRC);
		//return FALSE;
	}

	return bResult;
}




//-----------------------------------------------------------------------------
// �ͷŵ����ļ���ָ��Ŀ¼
//-----------------------------------------------------------------------------
BOOL CPK::ExtractSingleFile(LPCTSTR pDestDir, DWORD dwCRC)
{
	ASSERT(pDestDir);
	ASSERT(dwCRC);

	// �ҵ����е��ļ�Table��
	INT nTableIndex = this->GetTableIndexFromCRC(dwCRC);
	if (nTableIndex == GT_INVALID)
	{
		Msg(_T("Couldn't find file in package, CRC:%lu"), dwCRC);
		return FALSE;
	}

	// �����ļ���
	TCHAR szExtraInfo[MAX_PATH];
	TCHAR szFileName[MAX_PATH];
	DWORD dwDistanceToMove = m_pCPKTable[nTableIndex].dwStartPos + 
		m_pCPKTable[nTableIndex].dwPackedSize;

	CDiscIOMgr::Seek(m_dwCPKHandle, dwDistanceToMove);
	DWORD dwBytesToRead = m_pCPKTable[nTableIndex].dwExtraInfoSize;
	BOOL bResult = CDiscIOMgr::Read(m_dwCPKHandle, szExtraInfo, dwBytesToRead);
	if (bResult == 0)
		return FALSE;

	// ���������ļ���
	lstrcpy(szFileName, pDestDir);
	lstrcat(szFileName, _T("\\"));
	lstrcat(szFileName, szExtraInfo);

	// �����½�������ļ�
	DWORD dwHandle = CDiscIOMgr::Create(szFileName,
		GENERIC_READ | GENERIC_WRITE,
		0, // ��ռģʽ
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if (IS_INVALID_HANDLE(dwHandle))
	{
		Msg(_T("Couldn't write file %s"), szFileName);
		return FALSE;
	}


	LPVOID lpInBuffer = m_pPackBuffer;
	LPVOID lpOutBuffer = m_pRawBuffer;
	DWORD dwInBufferSize = m_dwPackBufferSize;
	DWORD dwOutBufferSize = m_dwRawBufferSize;

	if( IS_LARGE_FILE((m_pCPKTable + nTableIndex)->dwFlag) )
	{
		lpInBuffer = CMemMgr::Alloc(m_pCPKTable[nTableIndex].dwOriginSize);
		if (lpInBuffer == NULL)
		{
			ERR(CE_CANNOT_ALLOC_MEM);
			return FALSE;
		}
		lpOutBuffer = lpInBuffer;
		dwInBufferSize = m_pCPKTable[nTableIndex].dwPackedSize;
		dwOutBufferSize = m_pCPKTable[nTableIndex].dwOriginSize;
	}


	// ���������ļ�����
	dwDistanceToMove = m_pCPKTable[nTableIndex].dwStartPos;
	CDiscIOMgr::Seek(m_dwCPKHandle, dwDistanceToMove);
	dwBytesToRead = m_pCPKTable[nTableIndex].dwPackedSize;
	CDiscIOMgr::Read(m_dwCPKHandle, lpInBuffer, dwBytesToRead);

	if( IS_LARGE_FILE((m_pCPKTable + nTableIndex)->dwFlag) == false )
	{
		// �����ˣ���ѹ��С�ļ�
		tagFilterParam param;
		param.bCompress = FALSE;
		param.bSuccess = FALSE;
		param.lpMemIn = lpInBuffer;
		param.dwInBufferSize = dwInBufferSize;
		param.dwInDataSize = m_pCPKTable[nTableIndex].dwPackedSize;
		param.lpMemOut = lpOutBuffer;
		param.dwOutBufferSize = dwOutBufferSize;
		param.dwOutDataSize = m_pCPKTable[nTableIndex].dwOriginSize;
		param.eFilter = (EFMFilter)((m_pCPKTable[nTableIndex].dwFlag & 0xffff0000) >> 16);
		
		// �����ļ���Ϣ
		CFilterMgr::Filter(&param);
		if (param.bSuccess == FALSE)
			return FALSE;
	}

	// ������д���´������ļ�
	CDiscIOMgr::Write(dwHandle, lpOutBuffer, m_pCPKTable[nTableIndex].dwOriginSize);
	CDiscIOMgr::Close(dwHandle);

	if( IS_LARGE_FILE((m_pCPKTable + nTableIndex)->dwFlag) )
	{
		if (CMemMgr::Free(lpInBuffer) == FALSE)
		{
			ERR(CE_CANNOT_FREE_MEM);
		}
	}

	return TRUE;
}



//-----------------------------------------------------------------------------
// �ͷ��ļ���Ŀ¼��ָ��Ŀ¼���������µ������������ݹ�
// dwCRC : ��Ҫ�ͷŵ�Ŀ¼�����ļ�
// dwFatherSrcCRC : �ʼ�ͷŵ�Ŀ¼����������Ŀ¼��
// ������Ҫ�����и�Ŀ¼�µ�testĿ¼�µ�abcĿ¼�����е��ļ���Ŀ¼�ͷŵ�C:\temp
// �� dwFatherSrcCRC Ϊ abc Ŀ¼�� CRC
//-----------------------------------------------------------------------------
BOOL CPK::ExtractFileOrDir(LPCTSTR pDestDir, DWORD dwCRC, DWORD dwFatherSrcCRC)
{
	ASSERT(pDestDir);

	TCHAR szFullName[MAX_PATH*2];
	TCHAR szTemp[MAX_PATH];
	BOOL bResult = FALSE;

	if (this->IsDir(dwCRC))
	{
		// �ȼ��Ŀ¼�Ƿ��Ѿ����������û�У��ʹ���Ŀ¼
		lstrcpy(szFullName, pDestDir);
		this->GetRelativeNameFromCRC(szTemp, dwCRC, dwFatherSrcCRC);
		// ���Ƿ���Ҫ���"\"
		if (szFullName[lstrlen(szFullName)-1] != _T('\\'))
			lstrcat(szFullName, _T("\\"));
		lstrcat(szFullName, szTemp);
		::CreateDirectory(szFullName, NULL);

		DWORD dwChildNum = 0;
		DWORD dwChildFileNum = 0;
		DWORD dwChildDirNum = 0;
		this->GetFirstLevelChild(dwCRC, dwChildFileNum);
		this->GetFirstLevelChildDir(dwCRC, dwChildDirNum);
		
		DWORD *pChildFileCRCArray = new DWORD[dwChildFileNum];
		DWORD *pChildDirCRCArray = new DWORD[dwChildDirNum];

		this->GetFirstLevelChild(dwCRC, dwChildFileNum, pChildFileCRCArray);
		this->GetFirstLevelChildDir(dwCRC, dwChildDirNum, pChildDirCRCArray);

		m_dwFilesToProceed += dwChildFileNum + dwChildDirNum;	// ��Ҫ������ļ���

		// ���ͷ�Ŀ¼������Ŀ¼
		INT n = 0;
		for (n=0; n<(INT)dwChildDirNum; n++)
		{
			lstrcpy(szFullName, pDestDir);
			
			// ���Ƿ���Ҫ���"\"
			if (szFullName[lstrlen(szFullName)-1] != _T('\\'))
				lstrcat(szFullName, _T("\\"));
			
			// �õ����Ŀ¼��
			this->GetRelativeNameFromCRC(szTemp, pChildDirCRCArray[n], dwFatherSrcCRC);
			lstrcat(szFullName, szTemp);

_retry:			
			// �ڴ����ϴ���Ŀ¼
			m_dwFilesCurProceed++;	// �Ѿ�������ļ���
			bResult = ::CreateDirectory(szFullName, NULL);
			if (bResult == FALSE)
			{
				DWORD dwErr = ::GetLastError();
				switch(dwErr)
				{
				case 183:
					Msg(_T("Directory has already exist %s, continue"), szFullName);
					break;
				
				default:
					INT nResult = Msg(MB_OKCANCEL, 
						_T("Couldn't create directory %s, retry?"), szFullName);

					if (nResult == IDOK)
						goto _retry;

					ASSERT(0);
				}
			}

			this->ExtractFileOrDir(pDestDir, pChildDirCRCArray[n], dwFatherSrcCRC);
		}

		lstrcpy(szFullName, pDestDir);
		// ���Ƿ���Ҫ���"\"
		if (szFullName[lstrlen(szFullName)-1] != _T('\\'))
			lstrcat(szFullName, _T("\\"));
		
		// �õ����Ŀ¼��
		this->GetRelativeNameFromCRC(szTemp, dwCRC, dwFatherSrcCRC);
		lstrcat(szFullName, szTemp);

		// �ͷ�Ŀ¼�������ļ�
		for (n=0; n<(INT)dwChildFileNum; n++)
			this->ExtractFileOrDir(szFullName, pChildFileCRCArray[n], dwFatherSrcCRC);

		delete []pChildFileCRCArray;
		delete []pChildDirCRCArray;
		pChildFileCRCArray = NULL;
		pChildDirCRCArray = NULL;

		bResult = TRUE;
	}
	else
	{
		m_dwFilesCurProceed++;	// �Ѿ�������ļ���
		bResult = this->ExtractSingleFile(pDestDir, dwCRC);
		
		if (bResult == FALSE)
		{
			Msg(_T("Couldn't extract file, crc:%lu"), dwCRC);
			ASSERT(0);
			return bResult;
		}
	}


	return bResult;
	
}



//-----------------------------------------------------------------------------
// ɾ���ļ���Ŀ¼����ɾ��������������
// ���Ż�
//-----------------------------------------------------------------------------
BOOL CPK::RemoveFileOrDir(INT nTableIndex)
{
	ASSERT(IS_VALID_CPK_TABLE(nTableIndex));

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


	// Ѱ���ӽڵ㲢ɾ������Ϊ��δ���ո��ڵ���������ֻ��˳�����
	DWORD dwCRC = m_pCPKTable[nTableIndex].dwCRC;
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
// �������ļ����õ����ļ���(����·��)
//-----------------------------------------------------------------------------
BOOL CPK::GetSimpleFileName(LPTSTR pSimpleName, LPCTSTR pFullName)
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
// ���Ӽ��ļ�����Ŀ¼����Ŀ��Ŀ¼��
//-----------------------------------------------------------------------------
BOOL CPK::LinkSimpleNameToDirName(LPTSTR pFullName, LPCTSTR pSimpleName)
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
// �ɴ���ȫ��תΪ�����ļ���������֪����CRC, ��ȫ��תΪСд
//-----------------------------------------------------------------------------
BOOL CPK::ConvertFullNameToCPKFullName(LPTSTR pOutName, LPCTSTR pInName, 
									   DWORD dwFatherCRC)
{
	ASSERT(pOutName);
	ASSERT(pInName);
	BOOL bResult = FALSE;

	static TCHAR szSimpleName[MAX_PATH];
	static TCHAR szCPKFullName[MAX_PATH];

	bResult = this->GetFullNameFromCRC(szCPKFullName, dwFatherCRC);
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
// �õ��ļ���С
//-----------------------------------------------------------------------------
BOOL CPK::GetFileSize(DWORD& dwPackSize, DWORD& dwOriginSize, DWORD dwCRC)
{
	if (dwCRC == 0)	// ������Ŀ¼
		return TRUE;

	// ��CRC�ҵ���Ӧ����
	INT n = GetTableIndexFromCRC(dwCRC);

	if (n == GT_INVALID)
		return FALSE;

	tagCPKTable* pTableItem = m_pCPKTable + n;
	ASSERT(pTableItem);

	if ( IS_DIR(pTableItem->dwFlag) )
	{
		dwPackSize = 0;
		dwOriginSize = 0;
	}
	else
	{
		dwPackSize = pTableItem->dwPackedSize;
		dwOriginSize = pTableItem->dwOriginSize;
	}

	return TRUE;
}



//-----------------------------------------------------------------------------
// ��CRC��ʶ�õ������Ƿ���Ŀ¼
//-----------------------------------------------------------------------------
BOOL CPK::IsDir(DWORD dwCRC)
{

	if( dwCRC == 0 )	// ������Ŀ¼
		return TRUE;

	// ��CRC�ҵ���Ӧ����
	INT n = GetTableIndexFromCRC(dwCRC);

	if( IS_DIR((m_pCPKTable+n)->dwFlag) )
		return TRUE;

	return FALSE;
}




//-----------------------------------------------------------------------------
// �õ������ļ�����չ��(��"."��)
//-----------------------------------------------------------------------------
BOOL CPK::GetFileExtName(LPTSTR pExtName, INT nTableIndex)
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

	CDiscIOMgr::Seek(m_dwCPKHandle, dwReadPos);
	CDiscIOMgr::Read(m_dwCPKHandle, szTemp, dwBytesToRead);

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
// �Ӱ��еõ����ļ���(û��·�����ļ���)
//-----------------------------------------------------------------------------
BOOL CPK::GetSimpleFileName(LPTSTR pFileName, INT nTableIndex)
{
	ASSERT(IS_VALID_CPK_TABLE(nTableIndex));
	ASSERT(pFileName);

	static TCHAR szTemp[MAX_PATH];

	tagCPKTable* pTableItem = m_pCPKTable+nTableIndex;
	if( FALSE == IS_FILE_OR_DIR_VALID(pTableItem->dwFlag) )
	{
		DbgMsg(_T("Table %d is not valid"), nTableIndex);
		return FALSE;
	}

	// �����ļ�������Ϣ�����а����ļ�����Ϣ
	DWORD dwReadPos = pTableItem->dwStartPos + pTableItem->dwPackedSize;
	DWORD dwBytesToRead = pTableItem->dwExtraInfoSize;

	CDiscIOMgr::Seek(m_dwCPKHandle, dwReadPos);
	CDiscIOMgr::Read(m_dwCPKHandle, szTemp, dwBytesToRead);

	lstrcpy(pFileName, szTemp);
	return TRUE;
}






//-----------------------------------------------------------------------------
// ���㸽����Ϣ����
//-----------------------------------------------------------------------------
DWORD CPK::CalFileExtraInfoSize(LPCTSTR pFileName)
{
	static TCHAR szSimpleName[MAX_PATH];

	ASSERT(pFileName);
	lstrcpy(szSimpleName, pFileName);
	this->GetSimpleFileName(szSimpleName, pFileName);
	DWORD dwSize = 0;
	dwSize += lstrlen(szSimpleName) + GT_CPK_LEN_OF_STRING_END;
	dwSize += sizeof(tagFileExtraInfo);
	return dwSize;
}



//-----------------------------------------------------------------------------
// ���ļ��в�������
//-----------------------------------------------------------------------------
BOOL CPK::AddRealDataToCPK(INT nTableIndex, LPCTSTR pFileName,  VOID* pSrc, 
						   DWORD dwSrcFileSize, BYTE *pDigest)
{
	ASSERT(pFileName);
	ASSERT(IS_VALID_CPK_TABLE(nTableIndex));
	ASSERT(!IS_INVALID_HANDLE(m_dwCPKHandle));

	static TCHAR szFileName[MAX_PATH];

	BOOL bResult = FALSE;
	DWORD dwDistanceToMove = 0;
	DWORD dwBytesToWrite = 0;

	// �ƶ��ļ�ָ��
	dwDistanceToMove = m_pCPKTable[nTableIndex].dwStartPos;
	CDiscIOMgr::Seek(m_dwCPKHandle, dwDistanceToMove);
	DWORD dwDestFileSize = CDiscIOMgr::GetSize(m_dwCPKHandle);
	if (dwDistanceToMove >= dwDestFileSize)
		CDiscIOMgr::SetEOF(m_dwCPKHandle);


	tagFileExtraInfo ExtraInfo;
	// д��ʵ����Ϣ
	if( m_pCPKTable[nTableIndex].dwPackedSize > 0 )
	{
		memcpy(ExtraInfo.byMD5, pDigest, 16);
		CDiscIOMgr::Write(m_dwCPKHandle, pSrc, dwSrcFileSize);			
	}
	else
	{ //û���ļ�����,û��ϵ,���Ǽ������ֵ�md5
		CMD5::MD5ForString((char*)pFileName, ExtraInfo.byMD5);
	}


	// д�븽����Ϣ
	static TCHAR szSimpleName[MAX_PATH];
	this->GetSimpleFileName(szSimpleName, pFileName);
	dwBytesToWrite = (DWORD)lstrlen(szSimpleName);
	CDiscIOMgr::Write(m_dwCPKHandle, szSimpleName, dwBytesToWrite);

	// ���ļ���������ֽڽ�������, Ϊ��Unicode����
	CDiscIOMgr::Write(m_dwCPKHandle, _T("\0\0\0\0"), GT_CPK_LEN_OF_STRING_END);

	dwBytesToWrite = sizeof(ExtraInfo);
	CDiscIOMgr::Write(m_dwCPKHandle, &ExtraInfo, dwBytesToWrite);

	return TRUE;
}




//-----------------------------------------------------------------------------
// �����ļ����ҵ���Ӧ�ı���
//-----------------------------------------------------------------------------
INT CPK::GetTableIndex(LPCTSTR pFullName)
{
	static TCHAR szFullName[MAX_PATH];

	// ���ļ���ת��ΪСд�ټ���CRC
	lstrcpy(szFullName, pFullName);
	_tcslwr(szFullName);

	DWORD dwCRC = CRC32(szFullName);
	return this->GetTableIndexFromCRC(dwCRC);
}


DWORD CPK::GetMapStartPos(DWORD dwCRC)
{
	SYSTEM_INFO si;
	::GetSystemInfo(&si);
	DWORD dwG = si.dwAllocationGranularity;

	DWORD dwStartPos = m_pCPKTable[GetTableIndexFromCRC(dwCRC)].dwStartPos;
	dwStartPos = dwStartPos - (dwStartPos % dwG);
	return dwStartPos;

}



//-----------------------------------------------------------------------------
// ��CRC�ҵ���Ӧ����
// ���Ż�
//-----------------------------------------------------------------------------
INT CPK::GetTableIndexFromCRC(DWORD dwCRC)
{
	/*
	// �򵥵�˳�����
	for( INT n=0; n<(INT)m_CPKHeader.dwValidTableNum; n++ )
	{
		if( (m_pCPKTable+n)->dwCRC == dwCRC
			&& IS_FILE_OR_DIR_VALID((m_pCPKTable+n)->dwFlag) ) 
		{
			return n;
		}
	}*/

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
// ��Load Packageʱ����һ���Ա�õ�������Ƭ��Ϣ
//-----------------------------------------------------------------------------
INT CPK::GetAllFragment()
{
	INT nFragment = 0;
	for( INT n=0; (DWORD)n<m_CPKHeader.dwValidTableNum; n++ )
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
// ���ļ�ͷд����̰�
//-----------------------------------------------------------------------------
BOOL CPK::WriteHeaderToDisc()
{
	// ��CPK Headerд��
	CDiscIOMgr::Seek(m_dwCPKHandle, m_dwCPKHeaderAddr);
	DWORD dwBytesToWrite = sizeof(tagCPKHeader);
	CDiscIOMgr::Write(m_dwCPKHandle, &m_CPKHeader, dwBytesToWrite);

	return TRUE;

}



//-----------------------------------------------------------------------------
// ���ļ���д����̰�
//-----------------------------------------------------------------------------
BOOL CPK::WriteTableToDisc()
{
	// ��Tableд��
	DWORD dwDistanceToMove = m_CPKHeader.dwTableStart; 

	CDiscIOMgr::Seek(m_dwCPKHandle, dwDistanceToMove);
	DWORD dwBytesToWrite = sizeof(tagCPKTable) * m_CPKHeader.dwMaxTableNum;
	CDiscIOMgr::Write(m_dwCPKHandle, m_pCPKTable, dwBytesToWrite);

	return TRUE;
}





//-----------------------------------------------------------------------------
// �Ƿ��ǲ���ѹ���ļ�  mp3/bik
//-----------------------------------------------------------------------------
BOOL CPK::IsUnCompressable(LPCTSTR pFullName)
{
	return FALSE;
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
//��ʼӳ��
//-----------------------------------------------------------------------------
DWORD CPK::CreateMap()
{
	// û���κ�CPK�ļ�����
	if( FALSE == m_bLoaded )
		return NULL;

	return CDiscIOMgr::CreateMapping(m_dwCPKHandle, NULL, PAGE_READONLY, 0, NULL);
	
}
VOID	CPK::CloseMap(DWORD dwFileMap)
{
	CDiscIOMgr::CloseMappingHandle(dwFileMap);
}
//-----------------------------------------------------------------------------
//Ӱ����е�ĳ���ļ���ȡ���ļ����ݵĵ�ַ
//-----------------------------------------------------------------------------
LPVOID CPK::MapFile(DWORD dwFileMap, DWORD dwTableIndex, LPVOID &pMapAdd)
{
	// û���κ�CPK�ļ�����
	if( FALSE == m_bLoaded || NULL == dwFileMap )
		return NULL;
	if( FALSE == IS_FILE_OR_DIR_VALID(m_pCPKTable[dwTableIndex].dwFlag) )
		return NULL;

	DWORD dwOffset = m_pCPKTable[dwTableIndex].dwStartPos % m_dwAllocGranularity;
	DWORD dwStartPos = m_pCPKTable[dwTableIndex].dwStartPos - dwOffset;
	DWORD dwRealToRead = m_pCPKTable[dwTableIndex].dwPackedSize+ \
						m_pCPKTable[dwTableIndex].dwExtraInfoSize + dwOffset;
	pMapAdd = CDiscIOMgr::MapFile(dwFileMap, FILE_MAP_READ, dwStartPos, dwRealToRead);

	return (LPVOID)((DWORD)pMapAdd + dwOffset);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
VOID CPK::UnMapFile(LPVOID p)
{
	CDiscIOMgr::UnMapFile(p);
}
//-----------------------------------------------------------------------------
// �õ��ڴ����߶�
//-----------------------------------------------------------------------------
DWORD CPK::GetAllocationGranularity()
{

	SYSTEM_INFO si;
	::GetSystemInfo(&si);
	return si.dwAllocationGranularity;
}

