//-----------------------------------------------------------------------------
// File: VFS
// Desc: interface for audio
// Auth: Lyp
// Date: 2004/02/09
//
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{

class VENGINE_API IVFS
{
public:
	// ���õ�ǰ�������ļ���
	virtual BOOL SetWorkingPackage(LPCTSTR szPackage)=0;
	// ��ǰ�Ƿ��а���
	virtual BOOL HasWorkingPackage()=0;

	virtual DWORD	Open(LPCTSTR szFileName)=0;
	virtual BOOL	Close(DWORD dwHandle)=0;
	virtual DWORD	Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle)=0;
	virtual DWORD	Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin)=0;
	virtual DWORD	Tell(DWORD dwHandle)=0;

	// �Ż�����
	virtual DWORD	GetSize(LPCTSTR szFileName)=0;	// ����ļ������ڣ��򷵻�GT_INVALID
	virtual DWORD	LoadFile(LPVOID lpMemOut, LPCTSTR szFileName)=0;

	IVFS(){}
	virtual ~IVFS(){}
};


//-----------------------------------------------------------------------------
// class VirtualFileSys �����ļ�ϵͳ
//-----------------------------------------------------------------------------
class VENGINE_API VirtualFileSys : public IVFS
{
public:
	// ���õ�ǰ�������ļ���, szPackage == NULL��ʾUnload
	BOOL SetWorkingPackage(LPCTSTR szPackage);
	// ��ǰ�Ƿ��а���
	BOOL HasWorkingPackage();

	DWORD	Open(LPCTSTR szFileName);
	BOOL	Close(DWORD dwHandle);
	DWORD	Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle);
	DWORD	Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin);
	DWORD	Tell(DWORD dwHandle);

	// �õ�ָ���ļ���С
	DWORD	GetSize(LPCTSTR szFileName);	// ����ļ������ڣ��򷵻�GT_INVALID
	// �����е����ļ���Ŀ���ڴ棬����ԭ�ļ��ֽڴ�С��
	// ��Open+Read�����Ϊֱ�Ӷ���ָ���ڴ棬����һ��Memcpy
	DWORD	LoadFile(LPVOID lpMemOut, LPCTSTR szFileName);

	VirtualFileSys();
	~VirtualFileSys();


private:
	TObjRef<DiscIOMgr>		m_pDiscIOMgr;
	TObjRef<Log>			m_pLog;
	Package*				m_pPackage;
	
	DWORD GetMagicCode(LPCTSTR szFileName);
};



}