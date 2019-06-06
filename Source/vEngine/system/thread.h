//-----------------------------------------------------------------------------
//!\file thread.h
//!\author Lyp
//!
//!\date 2004-08-09
//! last 2004-08-09
//!
//!\brief �̹߳�����
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {


//-----------------------------------------------------------------------------
//!\brief �̹߳�����
//!
//!
//-----------------------------------------------------------------------------
class VENGINE_API Thread
{
public:

	//! ����ָ�����ֵ��߳�
	BOOL CreateThread(LPCTSTR szName, THREADPROC pFn, LPVOID Param);
	//! �ȴ�ָ�����ֵ��߳��˳�,���ָ��ʱ����û���˳��򷵻�FALSE
	//! ע��,ʹ�ô˺���������Լ������߳��˳��Ļ���,�˺�����������֪ͨ�߳�
	BOOL WaitForThreadDestroy(LPCTSTR szName, DWORD dwTimeOut);
	// destroy all thread ��Զ�ȴ�
	VOID WaitForAllThreadDestroy();

	// ����س��Ƿ�������
	BOOL IsThreadActive(LPCTSTR szName);

	// �õ�ָ���̵߳�handle
	HANDLE GetThreadHandle(LPCTSTR szName);
	

	Thread();
	~Thread();

private:
	TObjRef<Util>				m_pUtil;
	TSafeMap<DWORD, HANDLE>		m_mapHandle;

};






} // namespace vEngine {
