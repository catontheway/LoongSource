//-----------------------------------------------------------------------------
//!\file to_rt_session.cpp
//!\author Lyp
//!
//!\date 2005-04-05
//! last 2005-04-05
//!
//!\brief �����������������������Ʒ���ĶԻ���
//!
//!	Copyright (c) 2005 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/msg_rt_s.h"
#include "../ServerDefine/msg_rt_errorcode.h"
#include "rt_session.h"
#include "login_server.h"
#include "loongworld_mgr.h"
#include "loongworld.h"
#include "db.h"
#include "proof_policy.h"

//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
RTSession::RTSession() : m_Trunk(this)
{
	m_bInitOK					=	FALSE;
	m_bTermConnect				=	FALSE;
	m_nSendInfoTickCountDown	=	SEND_INFO_INTERVAL;
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
RTSession::~RTSession()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL RTSession::Init()
{
	m_pThread		=	"Thread";
	m_pUtil			=	"Util";

	// ��ʼ����Ա����
	TObjRef<VarContainer> pVar = "LoginServerConfig";
	m_strIP				=	pVar->GetString(_T("ip gm_server"));
	m_dwPort			=	pVar->GetDword(_T("port gm_server"));
	m_dwSectionID		=	pVar->GetDword(_T("id section"));
	m_nZoneNum			=	pVar->GetInt(_T("num zone_server"));

	m_dwWorldID			=	new DWORD[m_nZoneNum];
	m_dwWorldNameCRC	=	new DWORD[m_nZoneNum];
	m_strWorldName		=   new tstring[m_nZoneNum];
	for (int n=0; n<m_nZoneNum; n++)
	{
		TCHAR szZone[MAX_PATH] = {_T('\0')};
		_stprintf(szZone, _T("zone%d"), n);

		m_dwWorldID[n]		=	pVar->GetDword(_T("id"), szZone);
		tstring tmpName		=	pVar->GetString(_T("name"), szZone);
		m_dwWorldNameCRC[n]	=	m_pUtil->Crc32(tmpName.c_str());

		m_strWorldName[n] = tmpName;
	}

	// ������Ϣ����
	CreateObj("ToRtNetCmdMgr", "NetCmdMgr");
	m_pMsgCmdMgr = "ToRtNetCmdMgr";

	// �������Ӷ��󣬲���ʼ��
	CreateObj("ToRT", "StreamTransport");
	m_pTran = "ToRT";
	if( !P_VALID(m_pTran) )
	{
		ERR(_T("Create ToRT(StreamTransport) obj failed!\r\n"));
		return FALSE;
	}
	m_pTran->Init();

	// ע��������������
	RegisterAllRTCommand();

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ConnectRT"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&RTSession::ThreadConnectRT), NULL))
	{
		return FALSE;
	}

	while( !m_pThread->IsThreadActive(_T("ConnectRT")) )
	{
		continue;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID RTSession::Destroy()
{
	// �ȴ������߳̽���
	InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectRT"), INFINITE);

	m_pTran->Destroy();
	KillObj("ToRT");

	// ע����Ϣ����
	m_pMsgCmdMgr->Destroy();
	KillObj("ToRtNetCmdMgr");

	SAFE_DEL_ARRAY(m_dwWorldID);
	SAFE_DEL_ARRAY(m_dwWorldNameCRC);
}



//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID RTSession::Update()
{
	UpdateSession();
	SendServerInfo();
}

//-----------------------------------------------------------------------------
// ������Ϣ
//-----------------------------------------------------------------------------
VOID RTSession::UpdateSession()
{
	if( NULL == m_pTran )
		return;

	if( !m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectRT")) )
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectRT"), INFINITE);
		InterlockedExchange((LONG*)&m_bInitOK, FALSE);
		// ����������½�����������߳�
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectRT"), (vEngine::THREADPROC)m_Trunk.sfp0(&RTSession::ThreadConnectRT), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectRT")))
		{
			continue;
		}

		return;
	}

	while(TRUE)
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = m_pTran->Recv(dwSize);
		if( !P_VALID(pRecv) )
			break;

		// ������Ϣ
		m_pMsgCmdMgr->HandleCmd((tagNetCmd*)pRecv, dwSize, GT_INVALID);

		// ������Դ
		m_pTran->FreeRecved(pRecv);


	}	
}

//-----------------------------------------------------------------------------
// ���ͷ��������
//-----------------------------------------------------------------------------
VOID RTSession::SendServerInfo()
{
	if( !m_pTran->IsConnected() || !m_bInitOK ) return;

	if( --m_nSendInfoTickCountDown > 0 )
		return;

	m_nSendInfoTickCountDown = SEND_INFO_INTERVAL;

	tagNSC_WorldInfo send;
	send.eStatus		= EWS_Well;
	send.nOnlineNum		= 0;
	send.nMaxOnlineNum	= 0;
	
	// ȡ����������World����Ϣ
	for (int n=0; n<m_nZoneNum; n++)
	{
		LoongWorld* pLoongWorld = sLoongWorldMgr.GetLoongWorld(m_dwWorldNameCRC[n]);
		if (!P_VALID(pLoongWorld))
		{
			IMSG(_T("Loong World id<%d> error!\r\n"), m_dwWorldID[n]);
			break;
		}

		if (pLoongWorld->GetStatus() != EWS_Well)
		{
			send.eStatus = EWS_SystemError;
		}
		else
		{
			send.nOnlineNum += pLoongWorld->GetCurrOnlineDisplay();
			send.nMaxOnlineNum += pLoongWorld->GetMaxOnlineNum();
		}
	}

	// ��֤����������״̬
	ProofPolicy* pPolicy = sPlayerMgr.GetProofPolicyPtr();
	if (P_VALID(pPolicy) && pPolicy->GetProofServerStatus() != EWS_Well)
	{
		send.eStatus = EWS_ProofError;
	}

	m_pTran->Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// �����߳�(���Ӽ�ط�����)
//-----------------------------------------------------------------------------
DWORD RTSession::ThreadConnectRT()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTermConnect )
	{
		if( !m_pTran->IsConnected() )
		{		
			if( !m_pTran->IsConnected() )
			{
				m_pTran->TryToConnect(m_pUtil->UnicodeToUnicode8(m_strIP.c_str()), m_dwPort);
			}			

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to RT Server at %s: %d\r\n"), m_strIP.c_str(), m_dwPort);


		tagNSC_LoginService send;
		send.dwSectionID	=	m_dwSectionID;
		send.dwWorldID		=	GT_INVALID;
		send.eType			=	EST_Login;
		m_pTran->Send(&send, send.dwSize);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	return 0;
}


//----------------------------------------------------------------------------------------------
// ע�����е���������
//----------------------------------------------------------------------------------------------
VOID RTSession::RegisterAllRTCommand()
{
	m_pMsgCmdMgr->Register("NSS_LoginService",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleServerLogin),	_T("RT Server Certification Reply"));
	m_pMsgCmdMgr->Register("NSS_Close",				(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleCloseServer),	_T("Close Server"));

	//��ͣ����
	m_pMsgCmdMgr->Register("NSS_ForbidAccount",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleForbidAccount),	_T("Forbid One Account"));
	m_pMsgCmdMgr->Register("NSS_ForbidIP",			(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleForbidIP),	_T("Forbid One IP"));


}

//----------------------------------------------------------------------------------------------
// ��������֤������Ϣ
//----------------------------------------------------------------------------------------------
DWORD RTSession::HandleServerLogin(tagNetCmd* pMsg, DWORD)
{
	tagNSS_LoginService* pRecv = (tagNSS_LoginService*)pMsg;

	InterlockedExchange((LPLONG)&m_bInitOK, TRUE);
	return 0;
}

//-----------------------------------------------------------------------------------------------
// �������ر�������Ϣ
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleCloseServer(tagNetCmd* pMsg, DWORD)
{
	g_login.Shutdown();
	return 0;
}
//-------------------------------------------------------------------------
// �����ַ���
//-------------------------------------------------------------------------
BOOL RTSession::Forbid_DeleteBlankOfString(tstring &str)
{
	// ȥ���ո�
	INT nFind = 0;
	
	if((nFind = str.find_first_not_of(' ')) != str.npos)
	{
		str = str.substr(nFind);
	}

	if((nFind = str.find(' ')) != str.npos)
	{
		str = str.substr(0,nFind);
	}

	// ����û����Ƿ�Ϊ��
	return !str.empty();
}
//-----------------------------------------------------------------------------------------------
// ��ͣ/��� ָ��IP
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleForbidIP(tagNetCmd* pMsg, DWORD)
{
	//������Ϣ ��ͣ
	tagNSS_ForbidIP* pRecv = (tagNSS_ForbidIP*)pMsg;
	DWORD dwRet = 0;


	if(pRecv->bForbid == TRUE)
	{
		dwRet = sBeton.ForbidIP(pRecv->dwIP);
	}
	else
	{
		dwRet = sBeton.RemoveIPForbid(pRecv->dwIP);
	}

	tagNSC_ForbidIP send;
	send.dwClientID = pRecv->dwClientID;
	send.dwErrorCode = dwRet;
	m_pTran->Send(&send, send.dwSize);

	return 0;
}
//-----------------------------------------------------------------------------------------------
// �õ�Ҫ��ͣ����Ϸ��������crc
//-----------------------------------------------------------------------------------------------
DWORD RTSession::Forbid_GetForbidWorldCrc(tagNetCmd* pMsg,vector <DWORD> &vecWorldCrc)
{
	DWORD	dwWorldNameCrc	= 0;
	DWORD	dwErrorCode		= E_RT_Success;
	tagNSS_ForbidAccount* pRecv = (tagNSS_ForbidAccount*)pMsg;

	//������ܵ���������Ϊ�� ���ͣ���������е���Ϸ����
	if(_tcscmp(_T(""),pRecv->szWorld)  == 0)
	{
		for (int n=0; n<m_nZoneNum; n++)
		{
			vecWorldCrc.push_back(m_dwWorldNameCRC[n]);	
		}
		return dwErrorCode;
	}
	//�����ָ����Ϸ����
	else
	{
		//��worldid��һ��worldnamecrc
		for (int n=0; n<m_nZoneNum; n++)
		{
			if(	_tcscmp(m_strWorldName[n].c_str(),pRecv->szWorld)  == 0)
			{
				vecWorldCrc.push_back(m_dwWorldNameCRC[n]);	
				break;
			}
		}
		//��ʾû�ҵ�
		if(vecWorldCrc.size() == 0)
		{
			dwErrorCode = E_RT_Error;
		}
	}
	return dwErrorCode;
}
//-----------------------------------------------------------------------------------------------
// �õ�Ҫ��ͣ�˺�  ���ŷָ�
//-----------------------------------------------------------------------------------------------
VOID RTSession::Forbid_GetForbidAccountName(tstring &strMultiAccountName,vector <tstring> &vecAccountName)
{
	int iprepos = 0;
	int icurpos = 0;
	for(;;)
	{
		tstring strAccountName;

		icurpos = strMultiAccountName.find(',',iprepos);
		//���һ������
		if(icurpos == -1)
		{
			strAccountName = strMultiAccountName.substr(iprepos,strMultiAccountName.length() - iprepos);
			Forbid_DeleteBlankOfString(strAccountName);

			vecAccountName.push_back(strAccountName);
			break;
		}

		strAccountName = strMultiAccountName.substr(iprepos,icurpos-iprepos);
		Forbid_DeleteBlankOfString(strAccountName);

		vecAccountName.push_back(strAccountName);
		iprepos = icurpos + 1;
	}
}
//-----------------------------------------------------------------------------------------------
// ��ͣ/��� ָ���˺� ��ָ����Ϸ����
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleForbidAccount(tagNetCmd* pMsg, DWORD)
{
	//������Ϣ ��ͣ����
	tagNSS_ForbidAccount* pRecv = (tagNSS_ForbidAccount*)pMsg;
	DWORD	dwRet			= FALSE;
	DWORD	dwErrorCode		= E_RT_Success;
	vector <DWORD> vecWorldCrc;

	//FOR SAFE
	DWORD dwAccountByte = pRecv->dwSize - sizeof(pRecv->dwSize) - sizeof(pRecv->dwID) - sizeof(pRecv->dwClientID) - 
		sizeof(pRecv->szWorld)	- sizeof(pRecv->bForbid);
	pRecv->szAccount[dwAccountByte/sizeof(TCHAR) - 1] = _T('\0');

	//Ŀǰ�����ŷָ�
	tstring strMultiAccountName = pRecv->szAccount;

	if(strMultiAccountName.length() == 0)
	{
		dwErrorCode = E_RT_Error;
	}
	if(dwErrorCode == E_RT_Error)
	{
		//����gmserver�����
		tagNSC_ForbidAccount send;	
		send.dwClientID		=	pRecv->dwClientID;
		send.dwErrorCode	=	dwErrorCode;
		_tcscpy(send.szAccount,_T(""));
		m_pTran->Send(&send, send.dwSize);
		return 0;
	}


	//�õ�Ҫ��ͣ����Ϸ��������crc
	dwErrorCode = Forbid_GetForbidWorldCrc(pRecv,vecWorldCrc);
	if(dwErrorCode == E_RT_Error)
	{
		//����gmserver�����
		tagNSC_ForbidAccount send;	
		send.dwClientID		=	pRecv->dwClientID;
		send.dwErrorCode	=	dwErrorCode;
		_tcscpy(send.szAccount,_T(""));
		m_pTran->Send(&send, send.dwSize);
		return 0;
	}

	//�õ�Ҫ��ͣ�˺�  ���ŷָ�
	vector <tstring> vecAccountName;
	Forbid_GetForbidAccountName(strMultiAccountName,vecAccountName);

	//�Ƿ����в������ɹ�
	std::map <tstring,DWORD> mpReturnCode;
	BOOL bAllSuccess = TRUE;

	for(int iWorldLoop = 0; iWorldLoop<vecWorldCrc.size(); iWorldLoop++)
	{
		for(int iAccountLoop = 0; iAccountLoop<vecAccountName.size(); iAccountLoop++)
		{
			//����Ƿ�ָͣ���˻�
			if(pRecv->bForbid == TRUE)
			{
				DWORD dwAccountID = GT_INVALID;
				dwRet = sBeton.ForbidAccount(vecAccountName[iAccountLoop].c_str(),EPLM_GMTool,dwAccountID,vecWorldCrc[iWorldLoop]);		
				dwErrorCode = dwRet;
				if(dwErrorCode == E_RT_Success && dwAccountID != GT_INVALID)
				{
					LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(vecWorldCrc[iWorldLoop]);
					if( P_VALID(pWorld) )
					{
						tagNLW_KickPlayer send;
						send.dwAccountID = dwAccountID;
						pWorld->SendKickPlayerMsg(send);
					}
				}
			}
			else if(pRecv->bForbid == FALSE)
			{
				dwRet = sBeton.RemoveAccountForbid(vecAccountName[iAccountLoop].c_str(),EPLM_GMTool,vecWorldCrc[iWorldLoop]);
				dwErrorCode = dwRet;
			}
			if(dwErrorCode != E_RT_Success)
			{
				bAllSuccess = FALSE;
				mpReturnCode.insert(make_pair(vecAccountName[iAccountLoop],dwErrorCode));
			}
		}
	}
	//������в������ɹ� ֻ����һ���ɹ�
	if(bAllSuccess == TRUE)
	{
		//����gmserver�����
		tagNSC_ForbidAccount send;	
		send.dwClientID		=	pRecv->dwClientID;
		send.dwErrorCode	=	dwErrorCode;
		_tcscpy(send.szAccount,_T(""));
		m_pTran->Send(&send, send.dwSize);
	}
	//����mpReturnCode ֻ����ʧ�ܵ�
	else
	{
		std::map <tstring,DWORD>::iterator itReturnCode = mpReturnCode.begin();
		for(;itReturnCode != mpReturnCode.end(); ++itReturnCode)
		{
			//����gmserver�����
			tagNSC_ForbidAccount send;	
			send.dwClientID		=	pRecv->dwClientID;
			send.dwErrorCode	=	itReturnCode->second;
			if(itReturnCode->first.length() >= X_SHORT_NAME)
			{
				tstring str = itReturnCode->first.substr(0,X_SHORT_NAME-1);
				_tcscpy(send.szAccount,str.c_str());
			}
			else
			{
				_tcscpy(send.szAccount,itReturnCode->first.c_str());
			}

			m_pTran->Send(&send, send.dwSize);
		}
	}

	return 0;
}
