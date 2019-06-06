//-----------------------------------------------------------------------------
// File: VFS
// Desc: interface for audio
// Auth: Lyp
// Date: 2004/02/09
//
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "package.h"
#include "vfs.h"

// ʹ����������// ʹ����������
#include "cpkdefine.h"
#include "cpkfast.h"
#include "zipreader.h"

#include "..\system\disciomgr.h"


namespace vEngine{
//-----------------------------------------------------------------------------
// class VirtualFileSys �����ļ�ϵͳ
//-----------------------------------------------------------------------------
VirtualFileSys::VirtualFileSys()
{
	m_pPackage = NULL;
}
VirtualFileSys::~VirtualFileSys()
{
	SAFE_DEL(m_pPackage);
}

//-----------------------------------------------------------------------------
// ���õ�ǰ�������ļ���
//-----------------------------------------------------------------------------
BOOL VirtualFileSys::SetWorkingPackage(LPCTSTR szPackage)
{
	if( NULL == szPackage )
	{
		if( m_pPackage )
			m_pPackage->Unload();

		return TRUE;
	}

	DWORD dwMagicCode = GetMagicCode( szPackage );	// �õ������ͱ�ʶ
	if( GT_INVALID == dwMagicCode )
		return FALSE;

	switch( dwMagicCode )	// ѡ����ʵ������򿪰�
	{
	case GT_CPK_MAGIC:
		m_pPackage = new CPK;
		break;
	case GT_ZIP_MAGIC:
		m_pPackage = new ZipReader;
		break;
	}
	if( m_pPackage )
		return m_pPackage->Load(szPackage);

	return FALSE;
}

//-----------------------------------------------------------------------------
// ��ǰ�Ƿ��а���
//-----------------------------------------------------------------------------
BOOL VirtualFileSys::HasWorkingPackage()
{
	return (P_VALID(m_pPackage));
}


//-----------------------------------------------------------------------------
// fopen
//-----------------------------------------------------------------------------
DWORD VirtualFileSys::Open(LPCTSTR szFileName)
{
	DWORD dwHandle = INVALID_FILE_HANDLE;

	if( m_pPackage )
	{
		dwHandle = (DWORD)m_pPackage->Open(szFileName);
	}
	else
	{
		// û�м����κΰ���������ʹ����ͨ�ļ�����
		dwHandle = m_pDiscIOMgr->Create(szFileName,	GENERIC_READ,// | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

		if( dwHandle == INVALID_FILE_HANDLE )
			dwHandle = (DWORD)GT_INVALID;
	}

	return dwHandle;
}

//-----------------------------------------------------------------------------
// fclose
//-----------------------------------------------------------------------------
BOOL VirtualFileSys::Close(DWORD dwHandle)
{
	ASSERT(dwHandle);
	BOOL bResult = FALSE;

	if( m_pPackage )
	{
		bResult = m_pPackage->Close(dwHandle);
	}
	else
	{
		// û�м����κΰ���������ʹ����ͨ�ļ�����
		bResult = m_pDiscIOMgr->Close(dwHandle);
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// fread
//-----------------------------------------------------------------------------
DWORD VirtualFileSys::Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle)
{
	ASSERT(lpMemOut);
	ASSERT(dwHandle);
	DWORD dwResult = 0;

	if( m_pPackage )
	{
		dwResult = m_pPackage->Read(lpMemOut, dwSize, dwHandle);
	}
	else
	{
		// û�м����κΰ���������ʹ����ͨ�ļ�����
		dwResult = m_pDiscIOMgr->Read(dwHandle, lpMemOut, dwSize);
	}

	return dwResult;
}


//-----------------------------------------------------------------------------
// fread
//-----------------------------------------------------------------------------
DWORD VirtualFileSys::Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin)
{
	ASSERT(dwHandle);
	DWORD dwResult = 0;
	
	if( m_pPackage )
	{
		dwResult = m_pPackage->Seek(dwHandle, nOffset, dwOrigin);
	}	
	else
	{
		// û�м����κΰ���������ʹ����ͨ�ļ�����
		dwResult = m_pDiscIOMgr->Seek(dwHandle, nOffset, dwOrigin);
	}

	return dwResult;
}


//-----------------------------------------------------------------------------
// ftell
//-----------------------------------------------------------------------------
DWORD VirtualFileSys::Tell(DWORD dwHandle)
{
	ASSERT(dwHandle);
	DWORD dwResult = 0;

	if( m_pPackage )
	{
		dwResult = m_pPackage->Tell(dwHandle);
	}	
	else
	{
		// û�м����κΰ���������ʹ����ͨ�ļ�����
		dwResult = m_pDiscIOMgr->Seek(dwHandle, 0, GT_SEEK_CUR);
	}

	return dwResult;
}


//-----------------------------------------------------------------------------
// getfilesize
//-----------------------------------------------------------------------------
DWORD VirtualFileSys::GetSize(LPCTSTR szFileName)
{
	ASSERT(szFileName);
	DWORD dwResult = 0;

	if( m_pPackage )
	{
		dwResult = m_pPackage->LoadFile(NULL, szFileName);
	}	
	else
	{
		// û�м����κΰ���������ʹ����ͨ�ļ�����

		// ʹ��GetSize��ʹ��Seek(handle,0,SEEK_END)����ȫ
		dwResult = m_pDiscIOMgr->GetSize(szFileName);
	}
	return dwResult;
}



//-----------------------------------------------------------------------------
// �����а���˵,ʹ�ô˺�����ʹ��open\readҪ����
//-----------------------------------------------------------------------------
DWORD VirtualFileSys::LoadFile(LPVOID lpMemOut, LPCTSTR szFileName)
{
	ASSERT(szFileName);
	DWORD dwResult = 0;
	

	if( m_pPackage )
	{
		dwResult = m_pPackage->LoadFile(lpMemOut, szFileName);
	}	
	else
	{
		// û�м����κΰ���������ʹ����ͨ�ļ�����
		dwResult = m_pDiscIOMgr->LoadToMem(lpMemOut, szFileName);
	}

	return dwResult;
}



//-----------------------------------------------------------------------------
// �õ�����magic code
//-----------------------------------------------------------------------------
DWORD VirtualFileSys::GetMagicCode(LPCTSTR szFileName)
{
	DWORD dwHandle = m_pDiscIOMgr->Create(szFileName,
		GENERIC_READ, FILE_SHARE_READ, // ����Ӧ�ó���ɶ��˰�
		NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

	if( dwHandle == INVALID_FILE_HANDLE )
	{
		// �޷���ȷ���ļ�
		m_pLog->Write(_T("Couldn't open package file %s\r\n"), szFileName);
		return (DWORD)GT_INVALID;
	}

	DWORD dwMagic = (DWORD)GT_INVALID;
	
	if( 4 != m_pDiscIOMgr->Read(dwHandle, &dwMagic, 4) )
	{
		// ǰ���ֽ�û����ȷ����
		m_pLog->Write(_T("read first 4 bytes in file %s failed\r\n"), szFileName);
		dwMagic = (DWORD)GT_INVALID;
	}

	m_pDiscIOMgr->Close(dwHandle);
	return dwMagic;
}

}