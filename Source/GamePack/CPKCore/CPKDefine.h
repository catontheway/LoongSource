//-----------------------------------------------------------------------------
// File: CPKDefine.h
// Desc: game resource file system 2.0  system define
// Time: 2003-5-12
// Auth: CTCRST
//
// Copyright (c) 2003 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#ifndef GT_CPK_DEFINE
#define GT_CPK_DEFINE
#pragma once

#define GT_INVALID					-1
#define GT_INVALID_ID				0xFFFFFFFFL
								
#define GT_CPK_LABEL				0x1A545352L
#define GT_CPK_VERSION				1
#define GT_CPK_MAX_FILE				0x0007FFFFL
#define GT_CPK_MAX_OPEN_CPK			16
#define GT_CPK_MAX_OPEN_FILE		32

#define GT_CPK_DEFAULT_FILE			(5000)	// ÿ����ʼ�����ļ���������������
#define GT_CPK_TABLE_ADD			5000		//ÿ�����ݵı�����

#define GT_CPK_RAW_BUFFER_SIZE		(8L*1024*1024)
#define GT_CPK_PACK_BUFFER_SIZE		((8L*1024*1024) + (8L*1024*1024/64) + 16 + 3)
#define GT_CPK_LEN_OF_STRING_END	2

#define INVALID_FILE_HANDLE (DWORD)INVALID_HANDLE_VALUE
#define INVALID_MAP_HANDLE (DWORD)INVALID_HANDLE_VALUE

// table : �ļ���
// block : �ļ����ݿ�
// 1.��һ���ļ�С��4KBʱ����ʹ��ѹ��
//-----------------------------------------------------------------------------
// base struct
//-----------------------------------------------------------------------------
#pragma pack(push, pack_1)
#pragma pack(4)

// CPK�ļ�ͷ�ṹ
struct tagCPKHeader
{
	DWORD	dwLable;		// ���ͱ�־
	DWORD	dwVersion;		// �汾
	DWORD	dwTableStart;	// �ļ�����ʼ�ֽ�
	DWORD	dwDataStart;	// ���ݿ���ʼ��ַ
	DWORD	dwFileNum;		// ��ǰ�ļ�����	
	DWORD	dwIsFormatted;	// �ļ��Ƿ������
	DWORD	dwSizeOfHeader;	// �ļ�ͷ��С
	DWORD	dwValidTableNum;// ��ЧTable�������������Ч�ļ�����Ƭ
	DWORD	dwMaxTableNum;	// ���Table�����
	DWORD	dwFragmentNum;	// ��Ƭ��Ŀ��Ӧ��ΪdwValidTableNum - dwFileNum
	DWORD	dwPackageSize;	// ��ǰ����С
	DWORD	dwReserved[20];	
}; // 32*4


#define IS_VALID_CPK_TABLE(n) \
		(n>=(INT)0 && n<(INT)m_CPKHeader.dwMaxTableNum)



// CPK�ļ���ṹ
struct tagCPKTable
{
	DWORD	dwCRC;				// CRC����
	DWORD	dwFlag;				// ��־
	DWORD	dwFatherCRC;		// ��Ŀ¼CRC
	DWORD	dwStartPos;			// ��ʼλ��
	DWORD	dwPackedSize;		// ѹ���󳤶�
	DWORD	dwOriginSize;		// ԭ�ļ�����
	DWORD	dwExtraInfoSize;	// ������Ϣ����
};





// DWORD dwFlag:
// 0000 0000-0000 0000-0000 0000-0000 0001 (0x0001)��Ч
// 0000 0000-0000 0000-0000 0000-0000 0010 (0x0002)Ŀ¼
// 0000 0000-0000 0000-0000 0000-0000 0100 (0x0004)���ļ�
// 0000 0000-0000 0000-0000 0000-0001 0000 (0x0010)�ļ��ѱ�ɾ��
// 0000 0000-0000 0000-1000 0000-0000 0000 (0x8000)CRC���ظ�
// ��ʮ��λΪ���������

#define IS_TABLE_VALID(f)		((f) & 0x00000001)
#define IS_DIR(f)				((f) & 0x00000002)
#define IS_LARGE_FILE(f)		((f) & 0x00000004)
#define IS_FILE_CRC_REPEAT(f)	((f) & 0x00008000)
#define GET_FILTER(f)			((EFMFilter)(((f) & 0xffff0000) >> 16))
#define IS_COMPRESSED(f)		(GET_FILTER(f) != FMF_Changeless)


// ���ļ���ɾ��ʱ������һ�±�ǣ��˱���ͱ���˿ռ���Ƭ
// �Ժ�������ļ�ʱ��������Щ�ռ���Ƭ���ҵ��˲���ռ�
// ע�Ⲣ���Ǳ���ռ䣬���ǰ�����Ϣ�ռ䡣
// ֻ�е���Щ����ռ���ָ�����ƬС��һ���̶�ʱ��
// ����ռ�Żᱻ�ͷ�
#define IS_FILE_REMOVED(f)		((f) &  0x00000010)
#define SET_FILE_REMOVED(f)		((f) |= 0x00000010)

// �������Ƿ������Ч�ļ�����Ŀ¼�������ǿռ���Ƭ
#define IS_FILE_OR_DIR_VALID(f) (((f) & 0x00000001) && !IS_FILE_REMOVED(f))

#define GT_SEEK_SET				0
#define GT_SEEK_CUR				1
#define GT_SEEK_END				2

// ���ļ���������ļ�������Ϣ
struct tagFileExtraInfo
{
	FILETIME	CreateTime;			// ����ʱ��
	FILETIME	LastAccessTime;		// ���ķ���ʱ��
	FILETIME	LastWriteTime;		// �����޸�ʱ��
	FILETIME	AddToPackageTime;	// �������ʱ��
	FILETIME	DelFromPackageTime;	// �Ӱ���ɾȥ��ʱ��
	BYTE		byMD5[16];			//���ļ���md5У��ֵ
};


struct CPKFile
{
	TCHAR	szFullName[MAX_PATH];
	tagCPKTable* pTableItem;	// ָ��������

	// memory
	PVOID	lpMapAddress;	// Map��ʼλ��
	PVOID	lpStartAddress;	// Map�е��ļ���ʼλ��
	DWORD	dwOffset;		// ʵ��Map��ʼ��ַ���ļ���ʼ��ַ��ƫ��
	PVOID	lpMem;			// ָ���ļ�ʵ������

	// for seek/tell
	DWORD	dwFileSize;		// �ļ�ʵ�����ݴ�С
	DWORD	dwPointer;		// �ļ�ָ��

	BOOL	bCompressed;	// �ļ��Ƿ�ѹ��
	BOOL	bValid;			// �Ƿ���Ч
};

#ifdef  MAX_PATH
#undef  MAX_PATH
#endif
// ����һ����ȫ��MAX_PATH��С
#define MAX_PATH (_MAX_PATH + sizeof(tagFileExtraInfo) + 8)

#pragma pack(pop, pack_1)



//-----------------------------------------------------------------------------
// Error code
//-----------------------------------------------------------------------------
// For short
#define csi			const static int
#define css			const static TCHAR* const
#define ERR(m)	    do{CUtil::ErrMsg(MB_ICONERROR | MB_OK, \
						_T("Error : "#m"\nFile : %s\nLine : %d"), __FILE__, \
						__LINE__); \
						ASSERT(m>0); ASSERT(0);}while(0)

// �޴�
csi CE_NOERROR						= 0x00000000;

// �ڲ��߼����
csi CE_CPK_ALREADY_LOADED			= 0x00000001;
csi CE_CPK_HASNOT_LOADED			= 0x00000002;
csi CE_UNKNOW_CPK_FORMAT			= 0x00000003;
csi CE_VERSION_UNSUPPROT			= 0x00000004;
csi CE_FUTURE_VERSION				= 0x00000005;
csi CE_CANNOT_LOAD_TABLE			= 0x00000006;
csi CE_CPK_WAS_NOT_FORMATTED		= 0x00000007;
csi CE_CANNOT_OPEN_MORE_FILES		= 0x00000008;
csi CE_CANNOT_FIND_FILE				= 0x00000009;	// �ڰ����Ҳ���ָ���ļ�
csi CE_CANNOT_COMPRESS				= 0x0000000A;	// ���ܽ���ѹ��
csi CE_CANNOT_DECOMPRESS			= 0x0000000B;	// ���ܽ��н�ѹ


// �ڴ����
csi CE_INVALID_ADDRESS				= 0x10000001;	// �Ƿ��ڴ��ַ
csi CE_CANNOT_CREATE_MAIN_HEAP		= 0x10000002;	// ���ܽ�������ջ
csi CE_CANNOT_DESTROY_MAIN_HEAP		= 0x10000002;	// ������������ջ
csi CE_CANNOT_ALLOC_MEM				= 0x10000004;	// ���������ڴ�
csi CE_CANNOT_FREE_MEM				= 0x10000005;	// �����ͷ��ڴ�
csi CE_CANNOT_REALLOC_MEM			= 0x10000006;	// �������·����ڴ�


// ���̲������
csi CE_CANNOT_OPEN_FILE				= 0x20000001;
csi CE_FILE_DOES_NOT_EXIST			= 0x20000002;
csi CE_CANNOT_READ_DISC_FILE		= 0x20000003;
csi CE_CANNOT_CREATE_FILE_MAPPING	= 0x20000004;
csi CE_MAPPING_ALREADY_EXISTS		= 0x20000005;
csi CE_CANNOT_MAP_FILE				= 0x20000006;	// MapViewOfFileʧ��
csi CE_CANNOT_UNMAP_FILE			= 0x20000007;	// UnmapViewOfFileʧ��


// �ⲿ�ӿ����
csi CE_LZO_INIT_FAILED				= 0x30000001;	// lzo��ʼ��ʧ��


#endif // GT_CPK_DEFINE