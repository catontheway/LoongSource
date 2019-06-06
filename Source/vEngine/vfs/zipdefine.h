//-----------------------------------------------------------------------------
// File: zipreader
// Time: 2004-2-19
// Auth: CTCRST
//
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{

/* ZIP�ļ���ʽ����ṹ(Overall zipfile format):
[local file header1] <- ���������ĵ�
[file data 1] <- ���������ĵ�
[data_descriptor 1]
.
.
.
[local file header n]
[file data n]
[data_descriptor n]
[central directory]	<- ���������ĵ�
[zip64 end of central directory record]
[zip64 end of central directory locator]
[end of central directory record] <- ���������ĵ�
*/

#pragma pack(push)
#pragma pack(1)
struct tagZipLocalFileHeader
{
	DWORD	dwSignature;
	WORD	wVersionNeededToExtract;
	WORD	wGeneralPurposeBitFlag;
	WORD	wCompressionMethod;
	WORD	wLastModifyTime;
	WORD	wLastModifyDate;
	DWORD	dwCrc32;
	DWORD	dwCompressedSize;
	DWORD	dwUncompressedSize;
	WORD	wFileNameLength;
	WORD	wExtraFieldLength;
};


struct tagZipCentralDirectoryFileHeader
{
	DWORD	dwSignature;
	WORD	wVersionMadeBy;
	WORD	wVersionNeededToExtract;
	WORD	wGeneralPurposeBitFlag;
	WORD	wCompresssionMethod;
	WORD	wLastModifyTime;
	WORD	wLastModifyDate;
	DWORD	dwCrc32;
	DWORD	dwCompressedSize;
	DWORD	dwUncompressedSize;
	WORD	wFileNameLength;
	WORD	wExtraFieldLength;
	WORD	wFileCommentLength;
	WORD	wDiskNumberStart;
	WORD	wInternalFileAttrib;
	DWORD	dwExternalFileAttrib;
	DWORD	dwRlativeOffsetOfLocalHeader;
/*	VOID	FileName;		// file name (variable size) 
	VOID	ExtraField;		// extra field (variable size) 
	VOID	FileComment;	// file comment (variable size) */
};

const unsigned long f_dwSignatureOfZipEndOfCentralDirRecord = 0x06054b50L;
struct tagZipEndOfCentralDirRecord
{
	DWORD	dwSignature;
	WORD	wNumberOfThisDisk;
	WORD	wStartNumberOfTheDisk;
	WORD	wTotalEntriesOnTheDisk;
	WORD	wTotalEntries;
	DWORD	dwSizeOfTheCentralDir;
	DWORD	dwOffsetOfTheCentralDir;
	WORD	wZipFileCommentLength;
//	VOID	ZipFileComment;	(variable size) 
};

#pragma pack(pop)



// �����Լ������ZIP�е��ļ���Ŀ�ṹ
struct tagZipFileItem
{
	DWORD	dwFileNameCrc32;
	DWORD	dwFileHeaderOffset;
};


struct tagFileInZip
{
	PVOID	pMem;	// �ڴ濪ʼλ��

	// for seek/tell
	DWORD	dwFileSize;		// �ļ�ʵ�����ݴ�С
	DWORD	dwPointer;		// �ļ�ָ��

	BOOL	bValid;			// �Ƿ���Ч
};

}