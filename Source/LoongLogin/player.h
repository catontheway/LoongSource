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
#pragma once

#include "../WorldDefine/login.h"

#include "Mutex.h"
#include "player_net_cmd_mgr.h"

class PlayerNetCmdMgr;

//-----------------------------------------------------------------------------
// �����
//-----------------------------------------------------------------------------
class Player  
{
public:
	//-------------------------------------------------------------------------
	// ���캯��
	//-------------------------------------------------------------------------
	Player(DWORD dwClientID, DWORD dwCDIndex, DWORD dwIP, DWORD dwWorldID);
	~Player();

	//-------------------------------------------------------------------------
	// ����Get
	//-------------------------------------------------------------------------
	DWORD			GetAccountID()				{ return m_Data.dwAccountID; }
	LPCSTR			GetAccoutName()				{ return m_Data.szAccountName; }
	LPCTSTR			GetAccoutNameT();			
	DWORD			GetClientID()				{ return m_Data.dwClientID; }
	DWORD			GetCDIndex()				{ return m_Data.dwCDIndex; }
	BYTE			GetPrivilege()				{ return m_Data.byPrivilege; }
	DWORD			GetLoginWorldNameCrc()		{ return m_Data.dwWorldNameCrc; }
	INT				GetVerifyCode()				{ return m_nVerifyCode; }
	tagPlayerData&	GetPlayerData()				{ return m_Data; }

	DWORD			GetPreLoginTime()			{ return m_dwPreLoginTime;}
	DWORD			GetPreLoginIP()				{ return m_dwPreLoginIP;}


	BOOL			GetForbidMask()				{ return m_Data.dwFrobidMask; }
	BOOL			IsGuard()					{ return m_Data.bGuard; }

	//-------------------------------------------------------------------------
	// �����Ϣ���
	//-------------------------------------------------------------------------
	VOID			ProofReturn(tagProofResult* pResult);

	//-------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------
	VOID			SetConnLost()				{ InterlockedExchange((LPLONG)(&m_bConnLost), TRUE); }
	BOOL			IsConnLost()				{ return m_bConnLost; }

	//-------------------------------------------------------------------------
	// ������Ϣ���
	//-------------------------------------------------------------------------
	static VOID		RegisterPlayerMsg();
	static VOID		UnRegisterPlayerMsg();

	INT				HandleMessage();
	VOID			SendMessage(LPVOID pMsg, DWORD dwSize);
	
	//-------------------------------------------------------------------------
	// ��֤���
	//-------------------------------------------------------------------------
	VOID			SetPreLoginTime(tagDWORDTime dwTime)	{ m_dwPreLoginTime = dwTime; }
	VOID			SetPreLoginIP(DWORD dwIP)		{ m_dwPreLoginIP = dwIP; }


	VOID			SetKickTime(tagDWORDTime dwTime)	{ m_dwKickTime = dwTime; }
	tagDWORDTime	GetKickTime()						{ return m_dwKickTime; }

	BOOL			IsNeedMibao()						{ return m_bNeedMibao; }

	BOOL			IsProofing()						{ return m_bProofing; }
	BOOL			IsProofEnd()						{ return m_bProofEnd; }
	VOID			SetProofing()						{ m_bProofing = TRUE; m_bProofEnd = FALSE; }
	VOID			SetProofEnd()						{ m_bProofEnd = TRUE; m_bProofing = FALSE; }

	BOOL			IsSelectWorldOk()					{ return m_bSelectWorldOk; }
	VOID			SetSelectWorldOK()					{ m_bSelectWorldOk = TRUE; }

	BOOL			GenerateMibao(CHAR szMibao[MAX_MIBAO]);
	BOOL			CheckMibao(DWORD dwResultCrc);


private:
	//-----------------------------------------------------------------------
	// ��Ϣ������
	//-----------------------------------------------------------------------
	DWORD			HandleProof(tagNetCmd* pCmd);
	DWORD			HandleMibao(tagNetCmd* pCmd);

	//-----------------------------------------------------------------------
	// ��Ϣ���
	//-----------------------------------------------------------------------
	LPBYTE			RecvMsg(DWORD& dwSize, INT& nUnRecved);
	VOID			ReturnMsg(LPBYTE pMsg);
	VOID            SendMsg(LPBYTE pMsg, DWORD dwSize);

	//-----------------------------------------------------------------------
	// ��������
	//-----------------------------------------------------------------------
	BOOL			CheckName(string& str);

private:
	static PlayerNetCmdMgr		m_playerNetMgr;			// ��Ϣ������

private:
	tagPlayerData				m_Data;					// �������

	volatile BOOL				m_bConnLost;			// �����Ƿ��Ѿ��Ͽ�

	INT							m_nVerifyCode;			// ��֤��
	BOOL						m_bNeedMibao;			// �Ƿ���Ҫ�ܱ���֤

	BOOL						m_bProofing;			// �Ƿ����ڽ�����֤
	BOOL						m_bProofEnd;			// �Ƿ��Ѿ���֤���
	BOOL						m_bSelectWorldOk;		// �Ƿ��Ѿ�ѡ����Ϸ����ɹ�

	tagDWORDTime				m_dwKickTime;			// ��Ҫ���ߵ��ĵ���ʱ

	tagDWORDTime				m_dwPreLoginTime;		//�ϴε�¼ʱ��
	DWORD						m_dwPreLoginIP;			//�ϴε�¼ip

	TCHAR						m_cAccountNameT[X_SHORT_NAME]; //����Ӧ���Ͱ���û���
};





