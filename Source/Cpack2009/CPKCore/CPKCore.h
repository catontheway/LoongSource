//-----------------------------------------------------------------------------
// File: CPKCore.h
// Desc: game resource file system 2.0
// Time: 2003-5-12
// Auth: CTCRST
//
// Copyright (c) 2003 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#ifndef GT_CPK_CORE
#define GT_CPK_CORE
#pragma once

#include "cpkdefine.h"

class CPK;
class CPKMgr;
class MemPool;
//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
class CPK
{
public:
//-----------------------------------------------------------------------------
// ��׼����
//-----------------------------------------------------------------------------
	// �����µĹ����
	BOOL Create(LPCTSTR pFileName);
	BOOL Load(LPCTSTR pFileName);
	BOOL Unload();
	BOOL IncreaseTable();	//���ӱ���ռ�

	//�õ���Ƭ����
	DWORD GetFragmentNum(); 
	//�õ�������Ƭ�ռ��С
	DWORD GetFragmentSpace();

	// ����Ƿ��ǺϷ���CPK�ļ���
	BOOL IsValidCPK(LPCTSTR pFileName);

	BOOL AddFile(LPCTSTR pFullName, DWORD dwFatherCRC);
	BOOL RemoveFile(LPCTSTR pFullName);
	BOOL ExtractFile(LPCTSTR pDestDir, LPCTSTR pFullName);

	// ���ð��еĵ�ǰĿ¼
	BOOL  SetCurDir(LPCTSTR pFullDirName);
	// �õ����е�ǰĿ¼��CRC,�����֪Ŀ¼��������GetCRCFromCPKFullName�õ�CRC
	DWORD GetCurDirCRC() { return m_dwCurDirCRC; }

	// �õ�����ָ���ļ�����Ŀ¼�ĸ�Ŀ¼CRC
	DWORD GetFatherCRC(DWORD dwCRC);
	// �õ�����ָ��Ŀ¼�µ�����һ�����ļ�
	BOOL GetFirstLevelChild(DWORD dwDirCRC, DWORD& dwChildNum, DWORD* pdwCRCArray=NULL);
	// �õ�����ָ��Ŀ¼�µ�����һ����Ŀ¼
	BOOL GetFirstLevelChildDir(DWORD dwDirCRC, DWORD& dwChildNum, DWORD* pdwCRCArray=NULL);

	// �ɾ���·�����ļ����õ�����CRC��ʶ��������в����ڴ��ļ��򷵻�FALSE
	BOOL GetCRCFromCPKFullName(DWORD& dwCRC, LPCTSTR pFullName);
	// ��CRC��ʶ�õ����ļ���
	BOOL GetSimpleFileNameFromCRC(LPTSTR pSimpleFileName, DWORD dwCRC);
	// ��CRC��ʶ�õ�CPK���д�·���ļ���
	BOOL GetFullNameFromCRC(LPSTR pFullName, DWORD dwCRC);
	// ��CRC��ʶ��FatherCRC�õ�CPK���д����·�����ļ���
	BOOL GetRelativeNameFromCRC(LPSTR pNameOut, DWORD dwCRC, DWORD dwFatherCRC);
	
	// �õ��ļ���С
	BOOL GetFileSize(DWORD& dwPackSize, DWORD& dwOriginSize, DWORD dwCRC);
	// ��CRC��ʶ�õ������Ƿ���Ŀ¼
	BOOL IsDir(DWORD dwCRC);
	// ����Ƿ������ĳ����
	BOOL IsLoaded() { return m_bLoaded; }

	//���ַ�ʽ������ļ���ֻ�������·������·����ΪdwFatherCrc����Ŀ¼��Ϊ����������׼��
	DWORD AddDirCyc(LPCTSTR pFullName, DWORD dwFatherCRC);

	// �ɱ�����ŵõ�CPK���и��ļ���md5У��ֵ
	BOOL GetMD5FromTableIndex(BYTE digest[16], DWORD dwTableIndex);

	CPK();
	~CPK();


//-----------------------------------------------------------------------------
// �Ǳ��Ż�����
//-----------------------------------------------------------------------------

	// �õ�����ָ��Ŀ¼�µ�����һ����Ŀ¼���ļ�
	BOOL _GetFirstLevelChild(DWORD dwDirCRC, DWORD& dwChildNum);

	// ��TableIndex�õ����ļ���
	__forceinline BOOL _GetSimpleFileName(LPTSTR pFileName, INT nTableIndex)
	{
		return GetSimpleFileName(pFileName, nTableIndex);
	}
	
	// �õ�CPKTable
	__forceinline tagCPKTable _GetTable(INT nIndex)
	{ 
		ASSERT(IS_VALID_CPK_TABLE(nIndex));
		return m_pCPKTable[nIndex];
	}

	// �õ���ʱ�����е�����
	__forceinline INT _GetTempArray(INT nIndex)
	{
		ASSERT(IS_VALID_CPK_TABLE(nIndex));
		return m_pTempArray[nIndex];	
	}
	// ��CRC�ҵ���Ӧ����
	__forceinline INT _GetTableIndexFromCRC(DWORD dwCRC)
	{
		return GetTableIndexFromCRC(dwCRC);
	}

private:

	// CRC���
	static DWORD CrcTable[256];
	static VOID  InitCrcTable();
	static DWORD Crc(LPCTSTR pData);


	// �ڴ��
	MemPool* m_pMemPool;

	// �������Ϣ�й�
	tagCPKHeader m_CPKHeader;	// �ļ�ͷ
	tagCPKTable *m_pCPKTable;	//����ռ�

	DWORD	m_dwMaxCPKTable;

	BOOL	m_bLoaded;	// �Ƿ���CPK����, ���û�У������в�����Ч
	DWORD	m_dwCPKHandle;
	TCHAR	m_szCPKFileName[MAX_PATH];
	TCHAR	m_szCurDir[MAX_PATH];
	DWORD	m_dwCurDirCRC;
	
	// ѹ����ѹ��������
	LPVOID	m_pRawBuffer;	// Ϊдģʽ���Ļ���
	LPVOID	m_pPackBuffer;	// Ϊѹ��׼���Ļ���
	DWORD	m_dwRawBufferSize;	// Ϊдģʽ���Ļ���Ĵ�С
	DWORD	m_dwPackBufferSize; // Ϊѹ��׼���Ļ���Ĵ�С

	INT*	m_pFragment;	// ��¼��Ƭ������Table��
	INT*	m_pIndexToFragment;// ��¼Table��������Ƭ��

	// ������ʾ�ļ��������
	DWORD	m_dwFilesToProceed;		// ��Ҫ������ļ���
	DWORD	m_dwFilesCurProceed;	// �Ѿ�������ļ���

	// ��Ҫ������_GetFirstLevelChildʹ��
	INT*	m_pTempArray;
	INT		m_nArrayNum;

	VOID Reset();

	// ����������CRC�õ����е�Table��
	INT	GetFreeTableItem(DWORD dwCRC);	
	// �õ���������������
	DWORD FindCPKFreePlace(DWORD dwSize);	
	// ����Ƿ������������CRC�ظ���CRC
	BOOL HaveRepeatCRC(DWORD dwCRC);

	// �ڰ���ָ��Ŀ¼����һ����Ŀ¼, ����CRC
	DWORD CreateDir(LPCTSTR pSimpleDirName, DWORD dwFatherCRC);

	// ��ӵ����ļ���ָ��Ŀ¼
	BOOL AddSingleFile(LPCTSTR pFullName, DWORD dwFatherCRC);
	// ����ļ���Ŀ¼��ָ��Ŀ¼���������µ������������ݹ�
	BOOL AddFileOrDir(LPCTSTR pFullName, DWORD dwFatherCRC, BOOL bDir);

	// �ͷŵ����ļ���ָ��Ŀ¼
	BOOL ExtractSingleFile(LPCTSTR pDestDir, DWORD dwCRC);
	// �ͷ��ļ���Ŀ¼��ָ��Ŀ¼���������µ������������ݹ�
	BOOL ExtractFileOrDir(LPCTSTR pDestDir, DWORD dwCRC, DWORD dwFatherSrcCRC);

	// ɾ���ļ���Ŀ¼����ɾ�����������������ݹ�
	BOOL RemoveFileOrDir(INT nTableIndex);

	// �ɴ���ȫ��תΪ�����ļ���������֪����CRC
	BOOL ConvertFullNameToCPKFullName(LPTSTR pOutName, LPCTSTR pInName, DWORD dwFatherCRC);
	// �������ļ����õ����ļ���(����·��)
	BOOL GetSimpleFileName(LPTSTR pSimpleName, LPCTSTR pFullName);
	// ���Ӽ��ļ�����Ŀ¼����Ŀ��Ŀ¼��
	BOOL LinkSimpleNameToDirName(LPTSTR pFullName, LPCTSTR pSimpleName);
	// �Ӱ��еõ����ļ���(û��·��)
	BOOL GetSimpleFileName(LPTSTR pFileName, INT nTableIndex);
	
	// ���㸽����Ϣ����
	DWORD CalFileExtraInfoSize(LPCTSTR pFileName);

	// ���ļ��в�������
	BOOL AddRealDataToCPK(INT nTableIndex, LPCTSTR pFileName,  VOID* pSrc, 
							DWORD dwSrcFileSize, BYTE *pDigest=NULL);

	// �����ļ����ҵ���Ӧ�ı���
	INT GetTableIndex(LPCTSTR pFullName);
	// ��CRC�ҵ���Ӧ����
	INT GetTableIndexFromCRC(DWORD dwCRC);


	BOOL WriteHeaderToDisc();
	BOOL WriteTableToDisc();

};













#endif // GT_PACK_CORE