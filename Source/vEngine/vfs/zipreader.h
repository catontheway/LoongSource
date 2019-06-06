//-----------------------------------------------------------------------------
// File: zipreader
// Time: 2004-2-19
// Auth: CTCRST
//
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{

CONST DWORD GT_ZIP_MAGIC = 0x04034b50;
CONST INT MAX_FILENAME_LENGTH = 1000;
//-----------------------------------------------------------------------------
// �ʺϿ��ٶ�ȡZIP�ļ�,����������ļ�
// ZIP�ļ�����INTEL Litter-Endian��ʽ
//-----------------------------------------------------------------------------
class ZipReader : public Package
{
public:
	BOOL	Load(LPCTSTR szZipFileName);
	BOOL	Unload();

	// ��԰��ڵ����ļ��Ĳ���
	DWORD	Open(LPCTSTR szFileName);	// return handle
	BOOL	Close(DWORD dwHandle);
	DWORD	Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle);
	DWORD	Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin);
	DWORD	Tell(DWORD dwHandle);


	// �����е����ļ���Ŀ���ڴ棬����ԭ�ļ��ֽڴ�С��
	// ��Open+Read�����Ϊֱ�Ӷ���ָ���ڴ棬����һ��Memcpy
	// ����ڴ�Ŀ��lpMemOutΪ�գ��ͽ�����ԭ�ļ�(δѹ��ʱ)��С
	DWORD	LoadFile(LPVOID lpMemOut, LPCTSTR szFileName);

	ZipReader();~ZipReader();

private:
	BOOL		m_bLoaded;	// �Ƿ���Zip����
	DWORD		m_dwZipHandle;
	DWORD		m_dwZipFileSize;
	TCHAR		m_szZipFileName[MAX_PATH];	// for debug 
	
	DWORD		m_dwFileNumInZip;
	DWORD		m_dwCentralDirOffset;
	PVOID		m_lpFileTable;
	
	INT			m_nOpenedFileNum;

	// ȫСд�����ַ�����������б�ܸĳɷ�б�ܣ���Ϊβ����ӽ�����"\0"
	VOID		HandleFileNameString(LPTSTR szString, DWORD dwLength);
	

};











}	// namespace vEngine{
