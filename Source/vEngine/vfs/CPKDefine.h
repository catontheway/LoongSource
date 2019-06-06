//-----------------------------------------------------------------------------
// File: CPKDefine.h
// Desc: game resource file system 1.0  system define
// Time: 2003-1-2
// Auth: CTCRST
//
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{

const int CPK_MAX_PATH			= 260;

const long POLYNOMIAL			= 0x04C11DB7L;	// CRC����
const long GT_CPK_LABEL			= 0x1A545352L;
const long GT_CPK_MAGIC			= GT_CPK_LABEL;
const long GT_CPK_VERSION		= 1;
const long GT_CPK_MAX_FILE		= 0x0007FFFFL;


const long GT_CPK_RAW_BUFFER_SIZE	= (8L*1024*1024);
const long GT_CPK_PACK_BUFFER_SIZE	= ((8L*1024*1024) + (8L*1024*1024/64) + 16 + 3);
const long GT_CPK_LEN_OF_STRING_END	= 2;


// table : �ļ���
// block : �ļ����ݿ�
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




#define IS_VALID_CPK_TABLE(n) (n>=(INT)0 && n<(INT)m_CPKHeader.dwMaxFileNum)

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

#define IS_FILE_VALID(f)		((f) & 0x00000001)
#define IS_DIR(f)				((f) & 0x00000002)
#define IS_LARGE_FILE(f)		((f) & 0x00000004)
#define IS_FILE_CRC_REPEAT(f)	((f) & 0x00008000)
#define GET_FILTER(f)			((EFM_FILTER)(((f) & 0xffff0000) >> 16))
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



// ���ļ���������ļ�������Ϣ
struct tagFileExtraInfo
{
	FILETIME	CreateTime;			// ����ʱ��
	FILETIME	LastAccessTime;		// ���ķ���ʱ��
	FILETIME	LastWriteTime;		// �����޸�ʱ��
	FILETIME	AddToPackageTime;	// �������ʱ��
	FILETIME	DelFromPackageTime;	// �Ӱ���ɾȥ��ʱ��
	DWORD		dwReserved[4];		// ����
};


#ifdef  MAX_PATH
#undef  MAX_PATH
#endif
// ����һ����ȫ��MAX_PATH��С
#define MAX_PATH (_MAX_PATH + sizeof(tagFileExtraInfo) + 8)

#pragma pack(pop, pack_1)
}