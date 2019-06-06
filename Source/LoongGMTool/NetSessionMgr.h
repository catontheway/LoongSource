#pragma once
#include "Singleton.h"
#include "..\ServerDefine\rt_define.h"
class Util;

enum ELoginState
{
	ELS_NULL	= 0,
	ELS_Connect,
	ELS_Connect_Fail,
	ELS_Connect_Success,
};

class NetSession;
class NetSessionMgr : public Singleton<NetSessionMgr>
{
public:
	NetSessionMgr( void );
	~NetSessionMgr( void );

	bool Init();
	void Destroy();

	//--
	void CreateThread();

	//--
	BOOL IsConnected();
	BOOL IsTryingToConnect();
	VOID TryToConnect(LPCSTR szIP, INT nPort=0);
	VOID StopTryingConnect();
	void SendMsg(LPVOID msg, DWORD size);

	//--��¼״̬
	void SetLoginState(ELoginState eState) { m_eState = eState; }
	ELoginState GetLoginState() { return m_eState; }

	//--
	NetSession*	GetNetSessionPtr()	{ return m_pSession; }

	//--��ǰGMȨ��
	void SetPrivilege( EPrivilege ePri ) { m_ePrivilege = ePri; }
	EPrivilege GetPrivilege() { return m_ePrivilege; }

	void InsertPrivilegeMap( std::pair<DWORD, DWORD> pairPri );
	bool VerifyGMPrivilege( DWORD dwMsgCrc );

private:
	//�����߳�
	static UINT ThreadNetMsgUpdate(LPVOID param);

protected:
	NetSession*							m_pSession;

	HANDLE								thread_update_handle_;
	volatile BOOL						thread_update_terminate_;

	ELoginState							m_eState;
	EPrivilege							m_ePrivilege;					//��ǰGM��Ȩ��
	std::map<DWORD, DWORD>				m_mapPrivilege;					//��Ϣ��Ȩ�ޱ�

};

#define sNetSessionMgr NetSessionMgr::GetSingleton()
#define sNetSessionMgrPtr NetSessionMgr::GetSingletonPtr()