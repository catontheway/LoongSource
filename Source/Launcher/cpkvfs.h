//-----------------------------------------------------------------------------
// File: CPKCore.h
// Desc: cpk file format
// Time: 2005-5-31
// Auth: lyp,tidy
//
// Copyright (c) 2005 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma	   once
#include "cpkdefine.h"

//-----------------------------------------------------------------------------
/*	CPK �ļ���װ���Ŀ�꣺
1.	����
2.	���ݶ���ѹ������
3.	Ŀ¼ʽ����
4.	��������ɾ�������ļ���Ŀ¼

��ƹ���
1.	�ñ�������¼�����ļ�����Ϣ����CRC(ͨ���ļ�������)��־�������ļ���
2.	CRC�ǰ����ļ���ȫ��(����������·��)����ģ�
3.	�����ļ���(������·��)�Լ�ExtraInfo�����ڸ��ļ�������Ϣ֮��
4.	һ��ʼ�ͷ���һЩ�������ļ�ʱ�Ϳ��Բ���Ҫ�ƶ������ļ���Ϣ��
5.	��������֮���ٷ���һ�������
6.	ɾ���ļ�ʱֻ��������Ϊ��Ч��
7.	����ļ�ʱ��Ѱ�ҿ��б����Լ������ļ��ռ���룬���û�к��ʵĿ���
�ļ��ռ䣬����ӵ����ļ�ĩβ��
8.	����Ŀ���е�һ���׶�ʱ�����Զ԰�����һ����Ƭ���������ų�������
������ӡ�ɾ�����޸��ļ���ɵĿռ���Ƭ�����һ����ԶԱ������һ��
���������Ա�ӿ��ļ������ٶȣ�
9.	���ù�������˼�룬ѹ������ѹ�����ܡ����ܡ������ļ���ʽ��ת������
�Կ����ǹ��ˣ�ֻҪ��ͨ�õĽӿڣ��Ϳ�����ܶ���ѹ����ʽ���ݣ�
11.����Ӱ����ɾ�ļ��������Գ��Խ��лָ�
12.���ڴ���8MB���ļ�����Ϊ��һ�㶼�Ƕ��������֣����Բ������κ�ѹ����
ʽ��ֱ�Ӵ��ڰ��С�
13.֧��Ƕ�װ�����cpk�ļ�����԰���cpk�ļ���֧�ֽ���
*/

//���ǹ涨����·��(�ɺ��ļ���)��ʼλ�úͽ���λ�ò��ܸ���'\\'������Ϊ��Ч!!!
//-----------------------------------------------------------------------------
// ���ٵ�������
//-----------------------------------------------------------------------------
//#define GT_CPK_RAW_BUFFER_SIZE          5242880  //104857600

class CMD5;
class CTSMemMgr;
class CMemMgr;
//-------------------------------------------------------------------------------------

class CPKVfs
{
public:
	CPKVfs();
	~CPKVfs();

public:		//****��ʼ���ͽ����ӿ�******//

	// �����µĹ����,������ھ�ֱ�Ӵ򿪣������ھʹ������򿪣���ʵ������load����
	BOOL	Create(LPCSTR pFileName);	
	//����cpk�ļ��������ھͷ��ش���
	BOOL	Load(LPCSTR pFileName);
	//ж��cpk�ļ���
	BOOL	Unload();
	BOOL	IncreaseTable();	//���ӱ���ռ�

public:		//*****��ȡ�ӿ�*****//

	// ��԰��ڵ����ļ��Ĳ���
	DWORD	Open(INT nTableIndex, DWORD dwLength = 0,  BOOL bCpk = false );
	DWORD	Open(LPCTSTR szFileName);	// return handle
	BOOL	Close(DWORD dwHandle);
	DWORD	Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle);
	DWORD	Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin);
	DWORD	Tell(DWORD dwHandle);

public:		//*****д��(���,ɾ���ȱ༭����)�����ӿ�*****//

	//����ļ���·��,����Լ��,�����ļ��Ѿ�����ʱ��������
	BOOL AddFile(LPCSTR pFullName, DWORD dwFatherCRC);
	BOOL AddFile(LPCSTR pFullName, DWORD dwFatherCRC, tagCPKTable *pTable, LPVOID p);
	// �ڰ���ָ��Ŀ¼����һ��Ŀ¼, ����CRC
	DWORD CreateDir(LPCSTR pSimpleDirName, DWORD dwFatherCRC);
	//���ַ�ʽ������ļ���ֻ�������·������·����ΪdwFatherCrc����Ŀ¼��Ϊ����������׼��
	DWORD AddDirCyc(LPCSTR pFullName, DWORD dwFatherCRC);
	// ɾ��������ļ�����Ŀ¼��ʹ������·��
	BOOL  RemoveFile(DWORD dwCRC);
	// ɾ���ļ���Ŀ¼����ɾ��������������
	BOOL	RemoveFileOrDir(INT nTableIndex);
	//*********��ʼ��
	// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
	//������Ǽ��һ��ԭ��:���ļ����ݲ�ѹ��(Ҳ�����Ѿ�ѹ����),��չ��Ϣ����
	//*********��ʼ��
	INT InitFileCPKToCPK(LPCSTR pFullName, DWORD dwFatherCRC, tagCPKTable *pTable);
	//*********д������
	// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
	//������Ǽ��һ��ԭ��:���ļ����ݲ�ѹ��(Ҳ�����Ѿ�ѹ����),��չ��Ϣ����
	//*********д������
	BOOL WriteFileCPKToCPK(INT nTableIndex, DWORD dwWritePos, LPVOID p, DWORD dwDataSize);
	//*********��������ļ���д����ͷ��
	// ������ĵ�ǰĿ¼����ļ�,���ַ�ʽ�Ƚ����⣬�������Ǵ�����һ���������ת�ƹ���
	//������Ǽ��һ��ԭ��:���ļ����ݲ�ѹ��(Ҳ�����Ѿ�ѹ����),��չ��Ϣ����
	//*********��������ļ���д����ͷ��
	BOOL FinishFileCPKToCPK(INT nTableIndex);


public:		//*****������ؽӿ�*****//

	// ����Ƿ��ǺϷ���CPK�ļ���
	BOOL IsValidCPK(LPCSTR pFileName);
	//�õ������Ϣ
	tagCPKHeader *GetCPKHeader() { return &m_CPKHeader; }
	tagCPKTable	 *GetCPKTable()  { return m_pCPKTable; }
	DWORD GetCPKFileHanlde() { return m_dwCPKFileHandle; }

	//��crc�õ���Ӧ�ı���
	INT		GetTableIndexFromeCrc(DWORD dwCrc);
	//�õ�ȫ·���ļ���,ͨ��crc
	BOOL	GetFullNameFromCRC(DWORD dwCRC, LPSTR szFullName);
	// ��CRC��ʶ�õ����ļ���
	BOOL	GetSimpleFileNameFromCRC(LPSTR pSimpleFileName, DWORD dwCRC);
	// �ɴ���ȫ��תΪ�����ļ���������֪����CRC, ��ȫ��תΪСд
	BOOL	ConvertFullNameToCPKFullName(LPSTR pOutName, LPCSTR pInName, DWORD dwFatherCRC);
	// ���Ӽ��ļ�����Ŀ¼����Ŀ��Ŀ¼��
	BOOL	LinkSimpleNameToDirName(LPSTR pFullName, LPCSTR pSimpleName);
	// �õ������ļ�����չ��(��"."��)
	BOOL	GetFileExtName(LPTSTR pExtName, INT nTableIndex);
	// �����ļ����Ƿ��ʾ���֮��Ҫ����cpk��������: data.cpk\system\gongfu.exe,
	// �ⲿ����ȷ�����ļ�,������Ŀ¼������,data.cpk�������޴�֪�����Ƿ�Ŀ¼
	BOOL	IsFileForCPK(LPCSTR szFullName, LPCSTR szCPKName, LPCSTR szFileName);

	// �õ�����ָ��Ŀ¼�µ�����һ�����ļ�
	BOOL GetFirstLevelChild(DWORD dwDirCRC, DWORD& dwChildNum, DWORD* pdwCRCArray=NULL);
	// �õ�����ָ��Ŀ¼�µ�����һ����Ŀ¼
	BOOL GetFirstLevelChildDir(DWORD dwDirCRC, DWORD& dwChildNum, DWORD* pdwCRCArray=NULL);
	// ��CRC��ʶ�õ������Ƿ���Ŀ¼
	BOOL IsDir(DWORD dwCRC);

private:		//*****˽�и����ӿ�*****//

	// ��Load Packageʱ����һ���Ա�õ�������Ƭ��Ϣ
	INT		GetAllFragment();
	//��ȡϵͳ��Ϣ
	DWORD	GetAllocationGranularity();
	// ����Ƿ������������CRC�ظ���CRC
	BOOL	HaveRepeatCRC(DWORD dwCRC);
	// ���㸽����Ϣ����
	DWORD	CalFileExtraInfoSize(LPCSTR pFileName);
	// ����������CRC�õ����е�Table��
	INT		GetFreeTableItem(DWORD dwCRC);
	// �õ�CPK�п��Է�ָ����С���ݵĿռ���ʼ��
	DWORD	FindCPKFreePlace(DWORD dwSize);
	// ���ļ�ͷд����̰�
	BOOL	WriteHeaderToDisc();
	// ���ļ���д����̰�
	BOOL	WriteTableToDisc();
	// ���ļ��в�������
	BOOL	AddRealDataToCPK(INT nTableIndex, LPCSTR pFileName,  VOID* pSrc, DWORD dwSrcFileSize);
	// �������ļ����õ����ļ���(����·��)
	BOOL	GetSimpleFileName(LPSTR pSimpleName, LPCSTR pFullName);
	// �Ƿ��ǲ���ѹ���ļ�  mp3/bik
	BOOL	IsUnCompressable(LPCSTR pFullName);

private:
	CHAR		m_szCPKFileName[MAX_PATH];		//��cpk�ļ���

	BOOL		m_bLoaded;	// �Ƿ���CPK����
	DWORD		m_dwCPKFileHandle;		//cpk�ļ����
	DWORD		m_dwCPKMappingHandle;

	//����ļ�����
	tagCPKHeader m_CPKHeader;	// �ļ�ͷ
	tagCPKTable *m_pCPKTable;	//����ռ�
	DWORD	m_dwMaxCPKTable;	//����ı���ռ�

	std::map< DWORD, DWORD >	m_mapValidTable;		//�ؼ���crc���洢��Ч��table
	INT*		m_pFragment;	// ��¼��Ƭ������Table��
	INT*		m_pIndexToFragment;// ��¼Table��������Ƭ��

	//ϵͳ�����Ϣ
	DWORD		m_dwAllocGranularity;

	CTSMemMgr*	m_pTSMemMgr;			//�ڴ��,���ⲿ����
	CMemMgr*	m_pMemMgr;

	Util*		m_pUtil;

};

