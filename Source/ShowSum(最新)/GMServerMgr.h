#pragma once
#include "GMServer.h"

#define CONFIG_FILE_PATH		"../Game/server_gm_config/"
#define CONFIG_INI				"config_showsum"

class GMServer;

class GMServerMgr
{
	friend class Interface;

public:
	GMServerMgr();
	~GMServerMgr();

	BOOL							Init();
	void							Destroy(){}
	void							Update();

	TMap<INT, GMServer*>			GetGMServerMap()			{ return m_mapGMServer; }
	INT								GetGMServerCount()			{ return m_nGMServerCount; }
	INT								GetSum()					{ return m_nSumOnline; }

	/*void							SendMsg(GMServer* pGMServer, LPVOID pMsg, DWORD dwSize);
	void							RecvMsg(){};*/

	void							GetGMServerDynInfo();
	FLOAT							GetRate()					{ return m_fRate; }
	void							SetSum(INT nSum)			{ m_nSumOnline = nSum; }

	DWORD							HandleGMServerDynInfo();

private:
	TSFPTrunk<GMServerMgr>			m_Trunk;
	//TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<Thread>					m_pThread;
	TObjRef<VarContainer>			m_pVar;
	TObjRef<Log>					m_pLog;
	TObjRef<Util>					m_pUtil;
	Mutex							m_lock;

	TMap<INT, GMServer*>			m_mapGMServer;
	INT								m_nGMServerCount;
	INT								m_nSumOnline;

	FLOAT							m_fRate;				// ��ʾ����
	DWORD							m_dwInterval;			// ˢ�¼��
	DWORD							m_dwGetInfoInterval;	// ˢ�¼��
	DWORD							m_dwLastUpdateTime;
};

extern GMServerMgr g_GMServerMgr;