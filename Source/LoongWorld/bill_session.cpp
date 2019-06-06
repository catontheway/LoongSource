//-----------------------------------------------------------------------------
//!\file to bill_session.cpp
//!\author Sunnee
//!
//!\date 2009-07-04
//! last 2009-07-04
//!
//!\brief �������뻮���������ĶԻ���
//!
//!	Copyright (c) 2005 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/msg_bill_s.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/bill_define.h"
#include "bill_session.h"
#include "world_session.h"
#include "player_session.h"
#include "role.h"
#include "../ServerDefine/log_cmdid_define.h"

//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
BillSession::BillSession() : m_Trunk(this)
{
	m_bInitOK					=	FALSE;
	m_bTermConnect				=	FALSE;
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
BillSession::~BillSession()
{
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL BillSession::Init()
{
	m_pThread		=	"Thread";
	m_pUtil			=	"Util";

	// ��ʼ����Ա����
	TObjRef<VarContainer> pVar = "VarContainer";
	m_strIP			=	pVar->GetString(_T("ip bill_server"));
	m_dwPort		=	pVar->GetDword(_T("port bill_server"));
	m_dwSectionID	=	pVar->GetDword(_T("section_id world"));
	m_dwWorldID		=	pVar->GetDword(_T("id world"));

	// ������Ϣ����
	CreateObj("ToBillNetCmdMgr", "NetCmdMgr");
	m_pMsgCmdMgr = "ToBillNetCmdMgr";

	// �������Ӷ��󣬲���ʼ��
	CreateObj("ToBill", "StreamTransport");
	m_pTran = "ToBill";
	if( !P_VALID(m_pTran) )
	{
		ERR(_T("Create ToBill(StreamTransport) obj failed!\r\n"));
		return FALSE;
	}
	m_pTran->Init();

	// ע��������������
	RegisterAllBillCommand();

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ConnectBill"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&BillSession::ThreadConnectBill), NULL))
	{
		return FALSE;
	}

	while( !m_pThread->IsThreadActive(_T("ConnectBill")) )
	{
		continue;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID BillSession::Destroy()
{
	// �ȴ������߳̽���
	InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectBill"), INFINITE);

	m_pTran->Destroy();
	KillObj("ToBill");

	// ע����Ϣ����
	m_pMsgCmdMgr->Destroy();
	KillObj("ToBillNetCmdMgr");
}



//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID BillSession::Update()
{
	UpdateSession();
}

//-----------------------------------------------------------------------------
// ������Ϣ
//-----------------------------------------------------------------------------
VOID BillSession::UpdateSession()
{
	if( NULL == m_pTran )
		return;

	if( !m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectBill")) )
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectBill"), INFINITE);

		// ����������½�����������߳�
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectBill"), (vEngine::THREADPROC)m_Trunk.sfp0(&BillSession::ThreadConnectBill), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectBill")))
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
// �����߳�(���Ӽ�ط�����)
//-----------------------------------------------------------------------------
DWORD BillSession::ThreadConnectBill()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTermConnect )
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_pUtil->UnicodeToUnicode8(m_strIP.c_str()), m_dwPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to Bill Server at %s: %d\r\n"), m_strIP.c_str(), m_dwPort);


		tagNWB_Login send;
		send.dwWorldID		=	m_dwWorldID;
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
VOID BillSession::RegisterAllBillCommand()
{
	m_pMsgCmdMgr->Register("NBW_Login",			(NETMSGPROC)m_Trunk.sfp2(&BillSession::HandleServerLogin),	_T("Bill Server Certification Reply"));
	m_pMsgCmdMgr->Register("NBW_SafePickBaiBao",(NETMSGPROC)m_Trunk.sfp2(&BillSession::HandlePickBaiBao),	_T("Notice DB & Client to Reflesh BaiBao"));
}

//----------------------------------------------------------------------------------------------
// ��������֤������Ϣ
//----------------------------------------------------------------------------------------------
DWORD BillSession::HandleServerLogin(tagNetCmd* pMsg, DWORD)
{
	tagNBW_Login* pRecv = (tagNBW_Login*)pMsg;

	if (pRecv->dwErrorCode == E_Success)
	{
		InterlockedExchange((LPLONG)&m_bInitOK, TRUE);
	}
	else
	{
		m_pTran->Disconnect();
		InterlockedExchange((LPLONG)&m_bInitOK, FALSE);
	}
	return 0;
}
//----------------------------------------------------------------------------------------------
// ֪ͨ�ͻ��˰ٱ���Ԫ�������仯
//----------------------------------------------------------------------------------------------
DWORD BillSession::NoticeTheBaiBaoYuanbaoChange(tagNetCmd* pMsg)
{
	tagNBW_SafePickBaiBao* pRecv = (tagNBW_SafePickBaiBao*)pMsg;

	// �ҵ�ָ���˺ŵ��������
	PlayerSession* pPlayerSession = g_worldSession.FindSession(pRecv->dwAccountID);

	// �˺Ų�����
	if (!P_VALID(pPlayerSession))
		return GT_INVALID;

	// �������Ƿ���Ϸ��
	Role* pRole = pPlayerSession->GetRole();

	// ����������Ұٱ�Ԫ��������(����ֻ���������Ԫ������)		
	if(pRecv->dwIncYuanbao <= 0)
		return GT_INVALID;

	if (P_VALID(pRole))
	{	
		//�õ���ǰԪ����
		pRole->GetCurMgr().IncBaiBaoYuanBao(pRecv->dwIncYuanbao, ELCID_BaiBao_Bill_YuanBao, FALSE);
	}
	// �Ƿ���ѡ�˽���
	else if (P_VALID(g_worldSession.FindGlobalSession(pRecv->dwAccountID)))
	{
		pPlayerSession->SetBaiBaoYB(pRecv->dwIncYuanbao + pRecv->nCurYuanBao);
	}

	return E_Success;
}
//----------------------------------------------------------------------------------------------
// ˢ�°ٱ�������
//----------------------------------------------------------------------------------------------
DWORD BillSession::HandlePickBaiBao( tagNetCmd* pMsg, DWORD )
{
	tagNBW_SafePickBaiBao* pRecv = (tagNBW_SafePickBaiBao*)pMsg;

	// �ҵ�ָ���˺ŵ��������
	PlayerSession* pPlayerSession = g_worldSession.FindSession(pRecv->dwAccountID);
	
	// �˺Ų�����
	if (!P_VALID(pPlayerSession))
		return GT_INVALID;

	switch (pRecv->eBillType)
	{
	case EBBT_Yuanbao:
		{
			/* ���ڳ���ʱ�����ɾ��
			// ���°ٱ�����Ԫ��������
			tagNDBC_LoadBaiBaoYuanBao SendYuanBao;
			SendYuanBao.dwAccountID = pRecv->dwAccountID;
			g_dbSession.Send(&SendYuanBao, SendYuanBao.dwSize);
			*/

			//�ı�Ԫ������ ����֪ͨ�ͻ���
			NoticeTheBaiBaoYuanbaoChange(pRecv);
		}
		break;

	case EBBT_Item:
		{
			// ����Ϸ��
			Role* pRole = pPlayerSession->GetRole();
			if (P_VALID(pRole))
			{
				// ����ٱ������п�λ�����������Ƿ���δ�������Ʒ
				if(pRole->GetItemMgr().GetBaiBaoFreeSize() > 0)
				{
					tagNDBC_LoadBaiBao send;
					send.n64Serial = pRecv->n64Serial;
					send.dwAccountID = pRecv->dwAccountID;
					send.nFreeSpaceSize = pRole->GetItemMgr().GetBaiBaoFreeSize();

					g_dbSession.Send(&send, send.dwSize);
				}
			}
		}
		break;
	}

	return 0;
}

BillSession g_billSession;
