//-----------------------------------------------------------------------------
//!\file player.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief �����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/login.h"

#include "player.h"
#include "player_mgr.h"
#include "player_net_cmd_mgr.h"
#include "loongworld.h"
#include "loongworld_mgr.h"
#include "db.h"
#include "login_server.h"

//-------------------------------------------------------------------------------
// ��̬��������
//-------------------------------------------------------------------------------
PlayerNetCmdMgr Player::m_playerNetMgr;

//-------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------
Player::Player(DWORD dwClientID, DWORD dwCDIndex, DWORD dwIP, DWORD dwWorldID) : 
				m_Data(), m_bConnLost(FALSE), m_nVerifyCode(0), m_bProofEnd(FALSE),
				m_bProofing(FALSE), m_bNeedMibao(FALSE), m_dwKickTime(), m_bSelectWorldOk(FALSE)
{
	m_Data.dwClientID		=		dwClientID;
	m_Data.dwCDIndex		=		dwCDIndex;
	m_Data.dwAccountID		=		GT_INVALID;
	m_Data.dwFrobidMask		=		0;
	m_Data.bGuard			=		FALSE;
	m_Data.nAccOLSec		=		0;
	m_Data.dwIP				=		dwIP;
	m_Data.dwWorldNameCrc	=		dwWorldID;
	m_Data.byPrivilege		=		0;
}

//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
Player::~Player()
{
   
}

//-------------------------------------------------------------------------------
// ��֤����
//-------------------------------------------------------------------------------
VOID Player::ProofReturn(tagProofResult* pResult)
{
	m_Data.dwAccountID		=	pResult->dwAccountID;
	m_Data.bGuard			=	pResult->bGuard;
	m_Data.nAccOLSec		=	pResult->nGuardAccuTime;
	m_Data.byPrivilege		=	pResult->byPrivilege;
	m_Data.dwFrobidMask		=	pResult->dwFrobidMask;

	//memcpy(m_Data.szAccountName,pResult->szAccountName,X_SHORT_NAME); 

	m_bNeedMibao			=	pResult->bNeedMibao;

	SetProofEnd();

	// ����һ���˺���
	sPlayerMgr.CacheAccountName(GetAccountID(), GetAccoutName());
	sPlayerMgr.CacheGuard(GetAccountID(), GetPlayerData().bGuard);
	sPlayerMgr.MapAccountName2AccountID(GetAccoutName(), GetAccountID());
}

//-------------------------------------------------------------------------------
// ע����Ϣ������
//-------------------------------------------------------------------------------
VOID Player::RegisterPlayerMsg()
{
	m_playerNetMgr.Register("NLC_SafeProof",	&HandleProof,			_T("NLC_SafeProof"));
	m_playerNetMgr.Register("NLC_MiBao",		&HandleMibao,			_T("NLC_MiBao"));
}

//-------------------------------------------------------------------------------
// ע����Ϣ������
//-------------------------------------------------------------------------------
VOID Player::UnRegisterPlayerMsg()
{
	 m_playerNetMgr.UnRegisterAll();
}

//-------------------------------------------------------------------------------
// �������������Ϣ����
//-------------------------------------------------------------------------------
INT Player::HandleMessage()
{
    // �������Ƿ�Ͽ�����
    if( m_bConnLost )
        return CON_LOST;

    // ������Ϣ
	DWORD	dwSize		=	0;
	INT		nUnRecved	=	0;

	LPBYTE pMsg = RecvMsg(dwSize, nUnRecved);
	if( !P_VALID(pMsg) ) return 0;

	// ������Ϣ
    tagNetCmd* pCmd = (tagNetCmd*)pMsg;

    NETMSGHANDLER pHandler = m_playerNetMgr.GetHandler(pCmd, dwSize);

    if( NULL != pHandler )
        (this->*pHandler)(pCmd);
    
	// ������Ϣ
    ReturnMsg(pMsg);

	return 0;
}

//-------------------------------------------------------------------------------
// ������Ϣ
//-------------------------------------------------------------------------------
VOID Player::SendMessage(LPVOID pMsg, DWORD dwSize)
{
	if( !P_VALID(pMsg) || dwSize == 0 )
		return;

	// �����Ѿ��жϣ����ٷ�����
	if( m_bConnLost ) return;

	SendMsg((LPBYTE)pMsg, dwSize);
}

//-------------------------------------------------------------------------------
// ������Ϣ
//-------------------------------------------------------------------------------
LPBYTE Player::RecvMsg(DWORD& dwSize, INT& nUnRecved)
{
	return sPlayerMgr.RecvMsg(dwSize, nUnRecved, m_Data.dwCDIndex);
}

//-------------------------------------------------------------------------------
// �黹��Ϣ
//-------------------------------------------------------------------------------
VOID Player::ReturnMsg(LPBYTE pMsg)
{
	sPlayerMgr.ReturnMsg(pMsg);
}

//-------------------------------------------------------------------------------
// ������Ϣ
//-------------------------------------------------------------------------------
VOID Player::SendMsg(LPBYTE pMsg, DWORD dwSize)
{
	sPlayerMgr.SendMsg(m_Data.dwCDIndex, pMsg, dwSize);
}


//-----------------------------------------------------------------------------
// �����֤��Ϣ
//-----------------------------------------------------------------------------
DWORD Player::HandleProof(tagNetCmd* pCmd)
{
    tagNLC_SafeProof* pRecv = (tagNLC_SafeProof*)pCmd;

	// ��ֹ�ַ������
	pRecv->szUserName[X_SHORT_NAME-1] = '\0';
	pRecv->szPsd[MAX_MD5_ARRAY-1] = '\0';
	pRecv->szGUID[X_LONG_NAME-1] = '\0';

	// ���г�ʼ��֤
	INT nRet = E_Success;

	// ��֤���Ҫ��¼����Ϸ�����Ƿ����
	LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pRecv->dwWorldNameCrc);
	if( !P_VALID(pWorld) )
	{
		nRet = E_SelectWorld_GameWorldName_Wrong;
	}

	// �鿴���Ҫ��¼��LoongWorld�Ƿ�����
	else if( pWorld->GetStatus() != EWS_Well )
	{
		nRet = E_SelectWorld_Server_Maintenance;
	}

	// �鿴��֤�����Ƿ�һ��
	else if( pRecv->dwType != sPlayerMgr.GetProofPolicy() )
	{
		nRet = E_ProofResult_Wrong_Type;
	}

	// �鿴�汾���Ƿ�һ��
	else if( pRecv->dwCurVersionID != sPlayerMgr.GetCurrVer() )
	{
		nRet = E_ProofResult_Wrong_Build_Number;
	}

	// ���������֤�������Ѿ���֤���
	else if( IsProofing() || IsProofEnd() )
	{
		nRet = E_ProofResult_Proofing;
	}
	// ����û����Ϸ�
	else
	{
		string strUserName = pRecv->szUserName;

		if( CheckName(strUserName) )
		{
			nRet = E_ProofResult_Account_No_Match;
		}
		else
		{
			strUserName.copy(m_Data.szAccountName, X_SHORT_NAME);
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP,NULL,m_Data.szAccountName,-1,m_cAccountNameT,X_SHORT_NAME);
#else
			strcpy(m_cAccountNameT,m_Data.szAccountName);
#endif
		}
	}

	// ����д����򷵻ؿͻ���
	if( E_Success != nRet )
	{
		tagNLS_ProofResult send;
		send.dwErrorCode = nRet;
		SendMessage(&send, send.dwSize);
	}
	// ���û�д��������ʺ���֤
	else
	{
		sPlayerMgr.Proof(GetClientID(), GetAccoutName(), pRecv->szPsd, pRecv->szGUID, m_Data.dwIP);
	}

	return 0;
}

DWORD Player::HandleMibao(tagNetCmd* pCmd)
{
	tagNLC_MiBao* pRecv = (tagNLC_MiBao*)pCmd;

	// ������֤
	INT nRet = E_Success;

	if( !IsProofEnd() )
	{
		nRet = E_ProofResult_Mibao_Error;
	}
	else if( !CheckMibao(pRecv->dwMBCrc) )
	{
		nRet = E_ProofResult_Mibao_Error;
	}

	// ���������ȷ
	if( E_Success == nRet )
	{
		// ���뵽��Ϸ��������б�
		sPlayerMgr.RemovePlayerFromLogining(GetClientID());

		// �����ʺ�
		sPlayerMgr.AddAccount(GetAccountID(), GetClientID());

		//����Ҽ��뵽LoongWorld������б���
		sLoongWorldMgr.AddToWorld(this, GetLoginWorldNameCrc());

	}
	// ������ش���
	else
	{
		// ���͸��ͻ�����֤����
		tagNLS_ProofResult send;
		send.dwErrorCode = nRet;
		SendMessage(&send, send.dwSize);
	}

	return 0;
}

//-------------------------------------------------------------------------------
// �滻�������еĻس��ͻ���,ͬʱ����û����Ƿ�Ϊ��
//-------------------------------------------------------------------------------
BOOL Player::CheckName(string& str)
{
	// ȥ���س�
	INT nFind = 0;
	while( (nFind = str.find('\r', nFind)) != str.npos )
		str.replace(nFind, 1, "");

	// ȥ������
	nFind = 0;
	while( (nFind = str.find('\n', nFind)) != str.npos )
		str.replace(nFind, 1, "");

	// ȥ��tab
	nFind = 0;
	while( (nFind = str.find('\t', nFind)) != str.npos )
		str.replace(nFind, 1, "");

	// ȥ���ո�
	if((nFind = str.find(' ')) != str.npos)
	{
		str = str.substr(0,nFind);
	}

	// ����û����Ƿ�Ϊ��
	return str.empty();
}

//--------------------------------------------------------------------------------
// ������֤���ܱ�
//--------------------------------------------------------------------------------
BOOL Player::GenerateMibao(CHAR szMibao[MAX_MIBAO])
{
	// �����ݿ������ѯ����ҵ��ܱ�ֵ
	CHAR szMibaoInDB[MIBAO_LEN] = {'\0'};
	if( !sBeton.GetPlayerMibao(GetAccountID(), szMibaoInDB) )
	{
		return FALSE;
	}

	CHAR szValue[MAX_MIBAO] = {'\0'};
	// �ܱ��ĸ�ʽ���Զ��ŷָ����ַ����У�8*10�ľ���
	// ���磺97,55,87,21,90,33,19,36,55,90,26,96,24,61,32,27,70,64,86,69,97,54,36,21,
	// 18,58,55,96,37,32,75,64,08,87,08,74,33,13,34,90,70,14,09,98,93,37,19,75,21,68,51,
	// 46,59,41,86,69,13,93,00,15,48,36,57,50,16,98,24,57,38,63,91,28,53,06,35,40,61,59,94,15,
	for(INT n = 0; n < 3; ++n)
	{
		INT nRand = IUTIL->Rand() % 80;		// ���ֵ

		INT nX = nRand / 10;	// x�ᣬ��'A'��'H'
		INT nY = nRand % 10;	// y�ᣬ��'0'��'9'

		// ���ܱ�������д�����к�
		szMibao[n*2]	=	'A' + nX;
		szMibao[n*2+1]	=	'0' + nY;

		// ��ֵ��д���ܱ���ֵ
		szValue[n*2]	=	szMibaoInDB[nRand*3];
		szValue[n*2+1]	=	szMibaoInDB[nRand*3+1];
	}

	// ��szValue���CRC����������
	m_Data.dwMibaoCrc = IUTIL->Crc32((LPBYTE)szValue, MAX_MIBAO);


	return TRUE;
}

//--------------------------------------------------------------------------------
// ����ܱ�
//--------------------------------------------------------------------------------
BOOL Player::CheckMibao(DWORD dwResultCrc)
{
	return m_Data.dwMibaoCrc == dwResultCrc;
}
//--------------------------------------------------------------------------------
// ����Ӧ���Ͱ汾
//--------------------------------------------------------------------------------
LPCTSTR	Player::GetAccoutNameT()
{
	return m_cAccountNameT;
}