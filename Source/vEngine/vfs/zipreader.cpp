//-----------------------------------------------------------------------------
// File: zipreader
// Time: 2004-2-19
// Auth: CTCRST
//
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "zipdefine.h"
// ��̳�(�����ӿ�)
#include "package.h"
#include "zipreader.h"

// ʹ����������
#include "..\system\disciomgr.h"
#include "..\filter\filtermgr.h"

namespace vEngine{
//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
ZipReader::ZipReader()
{
	m_bLoaded = FALSE;
	m_dwZipHandle = INVALID_FILE_HANDLE;
	m_dwZipFileSize = 0;
	ZeroMemory(m_szZipFileName, sizeof(m_szZipFileName));
	
	m_dwFileNumInZip = 0;
	m_dwCentralDirOffset = 0;
	m_lpFileTable = NULL;
	
	m_nOpenedFileNum = 0;
}

ZipReader::~ZipReader()
{
	Unload();
}

//-----------------------------------------------------------------------------
// load zip file
//-----------------------------------------------------------------------------
BOOL ZipReader::Load(LPCTSTR szZipFileName)
{
	ASSERT(szZipFileName);
	lstrcpy(m_szZipFileName, szZipFileName);

	if( m_bLoaded )	// �������ͷ�������
		this->Unload();

	m_dwZipHandle = m_pDiscIOMgr->Create(szZipFileName,	GENERIC_READ,	// ֻ��
		FILE_SHARE_READ | FILE_SHARE_WRITE,	NULL, OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

	if( m_dwZipHandle == INVALID_FILE_HANDLE )
	{
		ERR(_T("couldn't open package file %s"), szZipFileName);
		m_pDiscIOMgr->Close(m_dwZipHandle);
		return FALSE;
	}

	// ��Ҫ���ļ�ĩβȡ��ZIP���е��ļ���
	m_dwZipFileSize = m_pDiscIOMgr->GetSize(m_dwZipHandle);
	if( m_dwZipFileSize < sizeof(tagZipEndOfCentralDirRecord) )
	{
		ERR(_T("zip file too small %s"), szZipFileName);
		m_pDiscIOMgr->Close(m_dwZipHandle);
		return FALSE;
	}

	// ���Ŵ��ļ�ĩβ������Ϣ
	tagZipEndOfCentralDirRecord end_record;
	m_pDiscIOMgr->Seek(m_dwZipHandle, m_dwZipFileSize - sizeof(end_record));
	m_pDiscIOMgr->Read(m_dwZipHandle, &end_record, sizeof(end_record));

	// ���Ǻ��ʵ���Ϣ������ǰ����
	if( end_record.dwSignature != 
		f_dwSignatureOfZipEndOfCentralDirRecord )
	{
		DWORD dwSign = 0;
		DWORD dwPointer = m_dwZipFileSize - sizeof(end_record);
		while( dwPointer > 32 )	// ������Ҫ����Ϣ��������ô��ǰ
		{
			dwPointer--;
			m_pDiscIOMgr->Seek(m_dwZipHandle, dwPointer);
			m_pDiscIOMgr->Read(m_dwZipHandle, &dwSign, 4);
			if( dwSign == f_dwSignatureOfZipEndOfCentralDirRecord )
			{
				m_pDiscIOMgr->Seek(m_dwZipHandle, dwPointer);
				m_pDiscIOMgr->Read(m_dwZipHandle, &end_record, sizeof(end_record));
				break;
			}
		}
		ERR(_T("couldn't recognise the zip file %s"), szZipFileName);
		m_pDiscIOMgr->Close(m_dwZipHandle);
		return FALSE;
	}

	if( end_record.wTotalEntriesOnTheDisk != 
		end_record.wTotalEntries )
	{
		// ���ܴ���־�ѹ����Zip�ļ�
		ERR(_T("couldn't load split zip file %s"), szZipFileName);
		m_pDiscIOMgr->Close(m_dwZipHandle);
		return FALSE;
	}

	m_dwFileNumInZip = end_record.wTotalEntriesOnTheDisk;
	m_dwCentralDirOffset = end_record.dwOffsetOfTheCentralDir;

	
	// ���������ļ�������Ϣ���Լ��ٺ�̲�ѯ
	m_lpFileTable = malloc(m_dwFileNumInZip*sizeof(tagZipFileItem));
	if( !m_lpFileTable )
	{
		ERR(_T("couldn't malloc for filetable in load zipfile:%s"), szZipFileName);
		m_pDiscIOMgr->Close(m_dwZipHandle);
		return FALSE;
	}

	// ���ζ��������ļ�����Ϣ
	tagZipCentralDirectoryFileHeader file_header;
	TCHAR szFileName[MAX_PATH*2];
	m_pDiscIOMgr->Seek(m_dwZipHandle, m_dwCentralDirOffset);
	for(INT n=0; n<(INT)m_dwFileNumInZip; n++)
	{
		m_pDiscIOMgr->Read(m_dwZipHandle, &file_header, sizeof(file_header));
		if( file_header.wFileNameLength > MAX_FILENAME_LENGTH )
		{
			ERR(_T("Zipfile:%s have been destroyed"), szZipFileName );
			m_pDiscIOMgr->Close(m_dwZipHandle);
			return FALSE;
		}
		tagZipFileItem* pItem = &((tagZipFileItem*)m_lpFileTable)[n];
		pItem->dwFileHeaderOffset = file_header.dwRlativeOffsetOfLocalHeader;

		// ��ȡ�ļ���
		m_pDiscIOMgr->Read(m_dwZipHandle, szFileName, file_header.wFileNameLength);
		this->HandleFileNameString(szFileName, file_header.wFileNameLength);
		pItem->dwFileNameCrc32 = m_pUtil->Crc32(szFileName);

		// ����ExtraField/FileComment
		if( file_header.wExtraFieldLength + 
			file_header.wFileCommentLength > 0 )
		{
			m_pDiscIOMgr->Seek(m_dwZipHandle, file_header.wExtraFieldLength +		
				file_header.wFileCommentLength, GT_SEEK_CUR);
		}
	}

	m_bLoaded = TRUE;
	return TRUE;
}



//-----------------------------------------------------------------------------
// unload zip file
//-----------------------------------------------------------------------------
BOOL ZipReader::Unload()
{
	if( !m_bLoaded )
		return FALSE;

	// ���д򿪵��ļ��Ƿ񶼹ر��ˣ�û�йرտ϶������ڴ�й©
	ASSERT(m_nOpenedFileNum == 0);

	// �ر�zip�ļ����
	m_pDiscIOMgr->Close(m_dwZipHandle);
	m_dwZipHandle = INVALID_FILE_HANDLE;

	free(m_lpFileTable);
	m_lpFileTable = NULL;
	m_nOpenedFileNum = 0;

	m_bLoaded = FALSE;
	return TRUE;
}


//-----------------------------------------------------------------------------
// open single file in the zip package
//-----------------------------------------------------------------------------
DWORD ZipReader::Open(LPCTSTR szFileName)
{
	if( !m_bLoaded )
		return GT_INVALID;

	ASSERT(szFileName);
	INT n = 0;

	DWORD dwCrc = m_pUtil->Crc32(szFileName);

	// ����CRC���ϵ��ļ�
	tagZipFileItem* pItem;

	
	for( n=0; n<(INT)m_dwFileNumInZip; n++)
	{
		pItem = &((tagZipFileItem*)m_lpFileTable)[n];
		if( pItem->dwFileNameCrc32 == dwCrc )
		{
			goto _founded;
		}
	}
	
	// δ�ҵ�ָ���ļ�
	return GT_INVALID;

_founded:

	// ������ļ�ͷ��Ϣ
	tagZipLocalFileHeader file_header;
	m_pDiscIOMgr->Seek(m_dwZipHandle, pItem->dwFileHeaderOffset);
	m_pDiscIOMgr->Read(m_dwZipHandle, &file_header, sizeof(file_header));

	// ���ļ�ָ���ƶ����ļ�������Ϣ��
	m_pDiscIOMgr->Seek(m_dwZipHandle, file_header.wFileNameLength + 
		file_header.wExtraFieldLength, GT_SEEK_CUR);

	// ���ǲ���������ļ�
	if( file_header.wGeneralPurposeBitFlag & 1 )
	{
		ERR(_T("couldn't handle encrypted file %s in zip"), szFileName);
		return NULL;
	}

	// ѹ����������ʶ��
	if( file_header.wCompressionMethod != 0	// δѹ��
		&& file_header.wCompressionMethod != 8 )	// Deflated
	{
		ERR(_T("couldn't handle %s's compression method in zip"), szFileName);
		return NULL;
	}

	// ����һ���ļ�ָ����Ŀռ�
	tagFileInZip* fp = (tagFileInZip*)malloc(sizeof(tagFileInZip));
	if( !fp )
	{
		ERR(_T("malloc failed in open file %s in zip"), szFileName);
		return NULL;
	}

	// ����ԭ���ļ���Ϣ�Ŀռ�
	fp->dwFileSize = file_header.dwUncompressedSize;
	fp->pMem = malloc(fp->dwFileSize);
	if( !fp )
	{
		free(fp);
		ERR(_T("malloc failed in open file %s in zip"), szFileName);
		return NULL;
	}

	if( file_header.wCompressionMethod == 0	)// δѹ��
	{
		m_pDiscIOMgr->Read(m_dwZipHandle, fp->pMem, fp->dwFileSize);
	}
	else
	{
		// ����һ����ʱ�ռ�������ѹ
		LPVOID p = malloc(file_header.dwCompressedSize);
		if( !p )
		{
			free(fp->pMem);
			free(fp);
			ERR(_T("malloc failed in open file %s in zip"), szFileName);
			return NULL;
		}

		// ����ѹ����Ϣ
		m_pDiscIOMgr->Read(m_dwZipHandle, p, file_header.dwCompressedSize);

		// �����ˣ���ѹ���ļ�
		tagFilterParam param;
		param.bCompress = FALSE;
		param.bSuccess = FALSE;
		param.dwInBufferSize = file_header.dwCompressedSize;
		param.dwInDataSize = file_header.dwCompressedSize;
		param.dwOutBufferSize = fp->dwFileSize;
		param.dwOutDataSize = fp->dwFileSize;
		param.eFilter = FMF_ZLibDeflated;
		param.lpMemIn = p;
		param.lpMemOut = fp->pMem;
		
		// �����ļ���Ϣ
		m_pFilterMgr->Filter(&param);

		// ������ʱ�ռ�
		free(p);

		if( param.bSuccess == FALSE )
		{
			ERR(_T("uncompression %s failed in zip"), szFileName);
			free(fp->pMem);
			free(fp);
			return NULL;
		}
	}

	fp->bValid = TRUE;
	fp->dwPointer = 0;
	m_nOpenedFileNum++;
	return (DWORD)fp;
}



//-----------------------------------------------------------------------------
// close the file 
//-----------------------------------------------------------------------------
BOOL ZipReader::Close(DWORD dwHandle)
{
	tagFileInZip* fp = (tagFileInZip*)dwHandle;
	ASSERT(fp);

	if( !fp )
		return FALSE;

	free(fp->pMem);
	free(fp);
	m_nOpenedFileNum--;

	return TRUE;
}



//-----------------------------------------------------------------------------
// read the file 
//-----------------------------------------------------------------------------
DWORD ZipReader::Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle)
{
	tagFileInZip* fp = (tagFileInZip*)dwHandle;
	ASSERT(fp != NULL);
	ASSERT(fp->bValid);

	if( fp->dwPointer + dwSize > fp->dwFileSize )
		dwSize = fp->dwFileSize -  fp->dwPointer;

	if( fp->dwFileSize >= fp->dwPointer + dwSize )	// ��ֹԽ��
		memcpy(lpMemOut, (BYTE*)fp->pMem + fp->dwPointer, dwSize);

	fp->dwPointer += dwSize;
	
	// ���뷵��ʵ��"�ɹ�"��ȡ���ֽ���
	return dwSize;
}


//-----------------------------------------------------------------------------
// ��λ���е����ļ���ָ��,���ص�ǰ�ļ�ָ��λ��
//-----------------------------------------------------------------------------
DWORD ZipReader::Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin)
{
	tagFileInZip* fp = (tagFileInZip*)dwHandle;
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
// ���ص�ǰ�ļ�ָ��λ��
//-----------------------------------------------------------------------------
DWORD ZipReader::Tell(DWORD dwHandle)
{
	return ((tagFileInZip*)dwHandle)->dwPointer;
}


//-----------------------------------------------------------------------------
// �����е����ļ���Ŀ���ڴ棬����ԭ�ļ��ֽڴ�С��
// ��Open+Read�����Ϊֱ�Ӷ���ָ���ڴ棬����һ��Memcpy
// ����ڴ�Ŀ��lpMemOutΪ�գ��ͽ�����ԭ�ļ�(δѹ��ʱ)��С
//-----------------------------------------------------------------------------
DWORD ZipReader::LoadFile(LPVOID lpMemOut, LPCTSTR szFileName)
{
	if( !m_bLoaded )
		return (DWORD)GT_INVALID;

	ASSERT(szFileName);
	INT n=0;

	DWORD dwCrc = m_pUtil->Crc32(szFileName);

	// ����CRC���ϵ��ļ�
	tagZipFileItem* pItem;
	for( n=0; n<(INT)m_dwFileNumInZip; n++ )
	{
		pItem = &((tagZipFileItem*)m_lpFileTable)[n];
		if( pItem->dwFileNameCrc32 == dwCrc )
		{
			goto _load_founded;
		}
	}

	// δ�ҵ�ָ���ļ�
	if( NULL != lpMemOut )	// �ⲿֻ����ȡ���ļ���С���򲻲���������ʾ
	{
		ERR(_T("could not find file %s in zip"), szFileName);
	}
	return (DWORD)GT_INVALID;

_load_founded:

	// ������ļ�ͷ��Ϣ
	tagZipLocalFileHeader file_header;
	m_pDiscIOMgr->Seek(m_dwZipHandle, pItem->dwFileHeaderOffset);
	m_pDiscIOMgr->Read(m_dwZipHandle, &file_header, sizeof(file_header));

	// ���ļ�ָ���ƶ����ļ�������Ϣ��
	m_pDiscIOMgr->Seek(m_dwZipHandle, file_header.wFileNameLength + 
		file_header.wExtraFieldLength, GT_SEEK_CUR);

	// ���ǲ���������ļ�
	if( file_header.wGeneralPurposeBitFlag & 1 )
	{
		ERR(_T("couldn't handle encrypted file %s in zip"), szFileName);
		return (DWORD)GT_INVALID;
	}

	// ѹ����������ʶ��
	if( file_header.wCompressionMethod != 0	// δѹ��
		&& file_header.wCompressionMethod != 8 )	// Deflated
	{
		ERR(_T("couldn't handle %s's compression method in zip"), szFileName);
		return (DWORD)GT_INVALID;
	}

	// ����ԭ���ļ���Ϣ�Ŀռ�
	if( lpMemOut == NULL )
		return file_header.dwUncompressedSize;

	if( file_header.wCompressionMethod == 0	)// δѹ��
	{
		m_pDiscIOMgr->Read(m_dwZipHandle, lpMemOut, file_header.dwUncompressedSize);
	}
	else
	{
		// ����һ����ʱ�ռ�������ѹ
		LPVOID p = malloc(file_header.dwCompressedSize);
		if( !p )
		{
			ERR(_T("malloc failed in open file %s in zip"), szFileName);
			return (DWORD)GT_INVALID;
		}

		// ����ѹ����Ϣ
		m_pDiscIOMgr->Read(m_dwZipHandle, p, file_header.dwCompressedSize);

		// �����ˣ���ѹ���ļ�
		tagFilterParam param;
		param.bCompress = FALSE;
		param.bSuccess = FALSE;
		param.dwInBufferSize = file_header.dwCompressedSize;
		param.dwInDataSize = file_header.dwCompressedSize;
		param.dwOutBufferSize = file_header.dwUncompressedSize;
		param.dwOutDataSize = file_header.dwUncompressedSize;
		param.eFilter = FMF_ZLibDeflated;
		param.lpMemIn = p;
		param.lpMemOut = lpMemOut;
		
		// �����ļ���Ϣ
		m_pFilterMgr->Filter(&param);

		// ������ʱ�ռ�
		free(p);

		if( param.bSuccess == FALSE )
		{
			ERR(_T("uncompression %s failed in zip"), szFileName);
			return (DWORD)GT_INVALID;
		}
	}

	return file_header.dwUncompressedSize;
}


//-----------------------------------------------------------------------------
// ȫСд�����ַ�����������б�ܸĳɷ�б�ܣ���Ϊβ����ӽ�����"\0"
//-----------------------------------------------------------------------------
VOID ZipReader::HandleFileNameString(LPTSTR szString, DWORD dwLength)
{
	lstrcpy(szString, m_pUtil->Unicode8ToUnicode((LPCSTR)szString));
	for( INT n=0; n<(INT)dwLength; n++ )
	{
		if( szString[n] == _T('/') )
			szString[n] = _T('\\');
	}

	szString[dwLength] = _T('\0');
	_tcslwr(szString);
}



}