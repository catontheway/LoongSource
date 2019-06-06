#pragma once
#include "Singleton.h"

struct tagFatigueInfo;
struct tagNC_ANTIENTHRALL;
class FatigueServer;

//-------------------------------------------------------------------------
// ������Timer
//-------------------------------------------------------------------------
class FatigueTimer
{
	//accountnamecrc, fatigueinfo
	typedef TMap<DWORD, tagFatigueInfo*>	FatigueInfoMap;

public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	FatigueTimer(FatigueServer* pFatigueServer)
		:m_pUtil("Util"), m_pFatigueServer(pFatigueServer){}
	~FatigueTimer();


	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	VOID	Update();
	VOID	Reset();

	//-------------------------------------------------------------------------
	// �����ӿ�
	//-------------------------------------------------------------------------
	VOID	RecordOLGuardAccounts(tagNC_ANTIENTHRALL* pMsg);
	VOID	NotifyOLGuardPlayers(tagFatigueInfo* pFagitueInfo);
	VOID	NotifyLoginPlayer(LPCSTR szAccountName);

	template<typename OPERATION>
	VOID	ForEachFatigueInfo(OPERATION oper);
	FatigueInfoMap &GetFatigueInfoMap(){return m_mapAllFatigue;}

private:
	TObjRef<Util>				m_pUtil;
	TSafeMap<DWORD, LPSTR>		m_mapCrcName;		// ��Ϸ������crc����Ϸ��������ӳ��

	FatigueInfoMap				m_mapAllFatigue;	// ���г�����Ϣ�б�
	FatigueServer*				m_pFatigueServer;	// fatigueserver
};

template<typename OPERATION>
VOID FatigueTimer::ForEachFatigueInfo( OPERATION oper )
{
	FatigueInfoMap::TMapIterator itr = m_mapAllFatigue.Begin();
	tagFatigueInfo* pFatigueInfo = NULL;
	// �����з��������
	while(m_mapAllFatigue.PeekNext(itr, pFatigueInfo))
	{
		if (!P_VALID(pFatigueInfo))
			break;
		oper(pFatigueInfo);
	}
}

//-------------------------------------------------------------------------
// �����Է�����
//-------------------------------------------------------------------------
class FatigueServer : public Singleton<FatigueServer>
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	FatigueServer();
	~FatigueServer() {Destroy();}

public:
	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init();
	VOID	Destroy();

	VOID	SendMsg(PVOID pMsg, DWORD dwSize);

private:
	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	VOID	ThreadUpdate();

	//-------------------------------------------------------------------------
	// �����߳�
	//-------------------------------------------------------------------------
	DWORD	LoginCallBack(LPBYTE pByte, DWORD dwSize);
	DWORD	LogoutCallBack(LPBYTE pByte, DWORD dwSize);

	//-------------------------------------------------------------------------
	// ��ʼ������
	//-------------------------------------------------------------------------
	BOOL	InitConfig();

	//-------------------------------------------------------------------------
	// ������Ϣ
	//-------------------------------------------------------------------------
	VOID	UpdateSession();


	//-------------------------------------------------------------------------
	// ��Ϣ������
	//-------------------------------------------------------------------------
	DWORD	HandleOnLineAccounts(tagNetCmd* pCmd);
	DWORD	HandleQueryState( tagNetCmd* pCmd);

private:
	TSFPTrunk<FatigueServer>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Thread>				m_pThread;
	TObjRef<StreamServer>		m_pSession;

	//-------------------------------------------------------------------------
	// �߳̽�����־
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateUpdate;
	volatile BOOL				m_bTerminateConnect;

	//-------------------------------------------------------------------------
	// ���Ӳ���
	//-------------------------------------------------------------------------
	INT							m_nPort;				// ����������֤�������Ķ˿�

	FatigueTimer				m_FatigueTimer;
	DWORD						m_dwSectionID;

};

#define sFatigueServer FatigueServer::getSingleton()

