#include "StdAfx.h"
#include "fatigue_server.h"
#include "../ServerDefine/msg_fatigue.h"
#include "db.h"

struct UpdateFatigueInfo
{
	UpdateFatigueInfo(FatigueTimer* pFatigueTimer)
		:m_pFatigueTimer(pFatigueTimer){}
	FatigueTimer* m_pFatigueTimer;

	VOID operator()(tagFatigueInfo* pFatigueInfo)
	{
		if (!P_VALID(pFatigueInfo))
			return;

		pFatigueInfo->Update();

		if (pFatigueInfo->NeedSave())
		{
			sBeton.SaveFatigueInfo(pFatigueInfo);
			pFatigueInfo->SetSaved();
		}

		if (pFatigueInfo->NeedNotify())
		{
			//notify
			m_pFatigueTimer->NotifyOLGuardPlayers(pFatigueInfo);
		}
	}
};

struct ResetFatigueInfo
{
	ResetFatigueInfo(){}

	VOID operator()(tagFatigueInfo* pFatigueInfo)
	{
		if (!P_VALID(pFatigueInfo))
			return;
		pFatigueInfo->bLastOnLine = pFatigueInfo->bOnLine;
		pFatigueInfo->bOnLine = FALSE;
	}
};

VOID FatigueTimer::Update()
{
	// �������������Ϣ
	ForEachFatigueInfo(UpdateFatigueInfo(this));
}

VOID FatigueTimer::Reset()
{
	// �������������Ϣ
	ForEachFatigueInfo(ResetFatigueInfo());
}

VOID FatigueTimer::RecordOLGuardAccounts( tagNC_ANTIENTHRALL* pOLAccounts )
{

	for (INT i=0; i<(INT)pOLAccounts->dwAccountNum; ++i)
	{
		LPSTR szAccountName = pOLAccounts->szAccountName + i * 16 * sizeof(CHAR);
		DWORD dwAccountNameCrc = m_pUtil->Crc32(szAccountName);

		// cache
		LPSTR szAccountName2Cache = m_mapCrcName.Peek(dwAccountNameCrc);
		if (!P_VALID(szAccountName2Cache))
		{
			szAccountName2Cache = new CHAR[16];
			memcpy(szAccountName2Cache, szAccountName, sizeof(CHAR)*16);
			m_mapCrcName.Add(dwAccountNameCrc, szAccountName2Cache);
		}					

		tagFatigueInfo* pFatigueInfo = m_mapAllFatigue.Peek(dwAccountNameCrc);
		if (!P_VALID(pFatigueInfo))
		{
			pFatigueInfo = new tagFatigueInfo;

			if (!sBeton.LoadFatigueInfo(dwAccountNameCrc, pFatigueInfo))
			{
				pFatigueInfo->dwAccountNameCrc = dwAccountNameCrc;
				pFatigueInfo->nAccOnLineTimeMin = 0;
				pFatigueInfo->nAccOffLineTimeMin = 0;
			}

			m_mapAllFatigue.Add(pFatigueInfo->dwAccountNameCrc, pFatigueInfo);
		}

		pFatigueInfo->bOnLine = TRUE;
	}
}

VOID FatigueTimer::NotifyOLGuardPlayers( tagFatigueInfo* pFagitueInfo )
{
	LPSTR szAccountName = m_mapCrcName.Peek(pFagitueInfo->dwAccountNameCrc);
	if (!P_VALID(szAccountName))
		return;

	tagNS_ANTIENTHRALL send;
	memcpy(send.szAccountName, szAccountName, sizeof(CHAR) * 16);
	send.byColor = pFagitueInfo->Status() + 1;
	send.dwAccOLSeconds = pFagitueInfo->nAccOnLineTimeMin * 60;
	m_pFatigueServer->SendMsg(&send, send.dwSize);
}

VOID FatigueTimer::NotifyLoginPlayer( LPCSTR szAccountName )
{
	DWORD dwAccountNameCrc = m_pUtil->Crc32(szAccountName);
	tagFatigueInfo* pFatigueInfo = m_mapAllFatigue.Peek(dwAccountNameCrc);
	if (P_VALID(pFatigueInfo))
	{
		pFatigueInfo->bLastOnLine = FALSE;
	}
}

FatigueTimer::~FatigueTimer()
{
	std::list<LPSTR> listNames;
	m_mapCrcName.ExportAllValue(listNames);

	while(!listNames.empty())
	{
		LPSTR pToDel = listNames.front();
		listNames.pop_front();
		SAFE_DEL(pToDel);
	}

	listNames.clear();
	m_mapCrcName.Clear();
}
//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL FatigueServer::Init()
{
	m_pUtil		=	"Util";
	m_pThread	=	"Thread";

	// ��ȡ�����ļ�
	if(!InitConfig())
		return FALSE;

	sBeton.LoadAllFatigueInfo(m_FatigueTimer.GetFatigueInfoMap());

	// ��ʼ������
	CreateObj("StreamServerFatigueServer", "StreamServer");
	m_pSession = "StreamServerFatigueServer";
	if( !P_VALID(m_pSession) ) return FALSE;
	m_pSession->Init((TCPLOGINCALLBACK)m_Trunk.sfp2(&FatigueServer::LoginCallBack),
		(TCPLOGOUTCALLBACK)m_Trunk.sfp1(&FatigueServer::LogoutCallBack), m_nPort);

	// ���������߳�
	if(!m_pThread->CreateThread(_T("FatigueServerThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&FatigueServer::ThreadUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("FatigueServerThreadUpdate")))
	{
		continue;
	}

	return TRUE;
}

//-------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------
BOOL FatigueServer::InitConfig()
{
	// ���������ļ�
	CreateObj("FatigueSessionVar", "VarContainer");
	TObjRef<VarContainer> pVar = "FatigueSessionVar";

	if( !P_VALID(pVar) ) return FALSE;

	TObjRef<Util> pUtil = "Util";
	TCHAR tszPath[MAX_PATH];
	ZeroMemory(tszPath, sizeof(tszPath));
	if (!pUtil->GetIniPath(tszPath, _T("server_config\\login\\login"))||
		!pVar->Load("VirtualFileSys", tszPath))
	{
		ERR(_T("�����ļ�δ�ҵ�"));
		return FALSE;
	}

	m_nPort = pVar->GetInt(_T("port fatigue_guard"));

	// ɾ�������ļ�����
	KillObj("FatigueSessionVar");

	return TRUE;
}

//-------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------
VOID FatigueServer::Destroy()
{
	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("FatigueServerThreadUpdate"), INFINITE);

	// ��������
	m_pSession->Destroy();
	KillObj("StreamServerFatigueServer");
}

//-------------------------------------------------------------------------
// �����߳�
//-------------------------------------------------------------------------
VOID FatigueServer::ThreadUpdate()
{
	DWORD dwTime = 0;
#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateUpdate )
		{
			m_FatigueTimer.Reset();

			// ������Ϣ
			UpdateSession();

			m_FatigueTimer.Update();
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		//����ǵ��ԣ����׳��쳣
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif

	_endthreadex(0);
}


//-------------------------------------------------------------------------
// ������Ϣ
//-------------------------------------------------------------------------
VOID FatigueServer::UpdateSession()
{
	if( NULL == m_pSession || !GT_VALID(m_dwSectionID))
	{
		Sleep(100);
		return;
	}

	DWORD	dwSize = 0;
	INT		nUnRecved = 0;
	tagNetCmd* pCmd = (tagNetCmd*)m_pSession->Recv(m_dwSectionID, dwSize, nUnRecved);
	

	while (!P_VALID(pCmd) || pCmd->dwID != m_pUtil->Crc32("NC_ANTIENTHRALL"))
	{
		if (!P_VALID(pCmd))
		{
			Sleep(100);
		}
		else if (pCmd->dwID == m_pUtil->Crc32("NC_ANTIQUERYSTATE"))
		{
			HandleQueryState(pCmd);
			m_pSession->FreeRecved(m_dwSectionID, (LPBYTE)pCmd);
		}

		pCmd = (tagNetCmd*)m_pSession->Recv(m_dwSectionID, dwSize, nUnRecved);
	}

	if (pCmd->dwID == m_pUtil->Crc32("NC_ANTIENTHRALL"))
		HandleOnLineAccounts(pCmd);
	m_pSession->FreeRecved(m_dwSectionID, (LPBYTE)pCmd);

}

DWORD FatigueServer::HandleOnLineAccounts( tagNetCmd* pCmd)
{
	tagNC_ANTIENTHRALL* pRecv = (tagNC_ANTIENTHRALL*)pCmd;

	m_FatigueTimer.RecordOLGuardAccounts(pRecv);

	return 0;
}

DWORD FatigueServer::HandleQueryState( tagNetCmd* pCmd)
{
	tagNC_ANTIQUERYSTATE* pRecv = (tagNC_ANTIQUERYSTATE*)pCmd;

	m_FatigueTimer.NotifyLoginPlayer(pRecv->szAccountName);

	return 0;
}

DWORD FatigueServer::LoginCallBack( LPBYTE pByte, DWORD dwSize )
{
	tagNC_LOGIN_CM* pRecv = (tagNC_LOGIN_CM*)pByte;

	if (GT_VALID(m_dwSectionID) || m_pUtil->Crc32("NC_LOGIN_CM") != pRecv->dwID)
		return GT_INVALID;
	else
		m_dwSectionID = pRecv->dwSectionID;
	return m_dwSectionID;
}

DWORD FatigueServer::LogoutCallBack( LPBYTE pByte, DWORD dwSize )
{
	DWORD dwSectionID = (DWORD)pByte;

	if (GT_VALID(m_dwSectionID))
	{
		m_dwSectionID = GT_INVALID;
		return 0;
	}

	return GT_INVALID;	
}

FatigueServer::FatigueServer() : m_Trunk(this), m_FatigueTimer(this)
{
	m_nPort = 0;
	m_bTerminateUpdate = FALSE;
	m_bTerminateConnect = FALSE;
	m_dwSectionID = GT_INVALID;
}

VOID FatigueServer::SendMsg( PVOID pMsg, DWORD dwSize )
{
	if(!GT_VALID(m_dwSectionID))
		return;
	m_pSession->Send(m_dwSectionID, pMsg, dwSize);
}
