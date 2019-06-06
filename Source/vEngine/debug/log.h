//-----------------------------------------------------------------------------
// File: log
// Desc: Game Tool log 1.0
// Auth: Lyp
// Date: 2003/12/18
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {


//-----------------------------------------------------------------------------
// log
//-----------------------------------------------------------------------------
class VENGINE_API Log
{
public:

	BOOL Create(LPCTSTR szFileName=NULL);
	BOOL Write(LPCTSTR lpFormat, ...);
	VOID Close();
	Log();~Log();
private:
	TObjRef<DiscIOMgr>		m_pDiscIOMgr;	// ������VFSϵͳ
	DWORD					m_dwFileHandle;
	TCHAR					m_szDefaultName[1024];
};


//-----------------------------------------------------------------------------
// ר���ṩ���쳣ģ��ʹ�õ�log,�ⲿӦ���������logģ��
//-----------------------------------------------------------------------------
class ExceptionLog
{
public:

	BOOL Create();
	BOOL Write(LPCSTR lpFormat, ...);
	VOID Close();
	LPCTSTR	GetDefaultFileName() { return m_szDefaultName; }

	ExceptionLog();~ExceptionLog();

private:
	TObjRef<DiscIOMgr>		m_pDiscIOMgr;	// ������VFSϵͳ
	DWORD					m_dwFileHandle;
	TCHAR					m_szDefaultName[1024];
};


} // namespace vEngine {
