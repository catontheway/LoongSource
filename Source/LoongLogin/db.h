//-----------------------------------------------------------------------------
//!\file db.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief ���ݿ���
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "../ServerDefine/login_define.h"

class Player;
struct tagFatigueInfo;

//-----------------------------------------------------------------------------
// ���ݿ���
//-----------------------------------------------------------------------------
class DB : public Singleton<DB>
{
public:
    //-------------------------------------------------------------------------
	// CONSTURCT
	//-------------------------------------------------------------------------
	DB() { }
	~DB() { Destroy(); }

    //-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
    BOOL	Init();
    VOID	Destroy();
    
	//-------------------------------------------------------------------------
	// �������ݿ��������
	//-------------------------------------------------------------------------
	VOID	PlayerLogin(DWORD dwAccountID, DWORD dwWorldNameCrc);
	VOID	PlayerLogout(DWORD dwAccountID);
	VOID	PlayerEnterWorld(DWORD dwAccountID, DWORD dwWorldNameCrc);
	VOID	PlayerOutWorld(DWORD dwAccountID, DWORD dwWorldNameCrc);
	VOID	FixPlayerLoginStatus(DWORD dwAccountID, EPlayerLoginStatus eDest);
	VOID	FixOneWorldPlayerLoginStatus( DWORD dwWorldNameCrc, EPlayerLoginStatus eSrc, EPlayerLoginStatus eDest );
	VOID	ResetAllPlayerLoginStatus(EPlayerLoginStatus eSrc, EPlayerLoginStatus eDest);
	VOID	ResetPlayersLoginStatus(DWORD* pdwAccountIDs, INT nNum, EPlayerLoginStatus eDest);

	BOOL	GetPlayerMibao(DWORD dwAccountID, CHAR szMibao[MIBAO_LEN]);


	BOOL	LoadAllFatigueInfo(TMap<DWORD, tagFatigueInfo*> &mapFatigueInfo);
//	VOID	UpdateFatigueTimeTable();
//	BOOL	LoadLoginStatus(TMap<DWORD, tagFatigueInfo*> &mapFatigueInfo);
//	VOID	SaveFatigueInfo(tagFatigueInfo* pToSave);
	BOOL	LoadCacheAccountData(TSafeMap<DWORD, tagAccountData*> &mapAccountData, TSafeMap<DWORD, DWORD> &mapNameCrc2AccountID);
	BOOL	LoadOnLineGuardAccountIDs(TMap<DWORD, DWORD> &mapAccounts);

	VOID	SaveFatigueInfo(tagFatigueInfo* pFatigueInfo);
	BOOL	LoadFatigueInfo(DWORD dwAccountNameCrc, tagFatigueInfo* pFatigueInfo);
	BOOL	IfWorldForbid(DWORD dwAccountID,DWORD dwWorldNameCrc);
	BOOL	IfHaveWorldForbid(DWORD dwAccountID);
	BOOL	IfIPForbid(DWORD dwIP);

	DWORD	ForbidAccount(DWORD dwAccountID, DWORD dwForbidM, DWORD dwWorldNameCrc = 0);
	DWORD	ForbidAccount(LPCTSTR szAccountName, DWORD dwForbidM, DWORD &dwAccountID, DWORD dwWorldNameCrc = 0);

	DWORD	RemoveAccountForbid(DWORD dwAccountID,DWORD dwForbidM,DWORD dwWorldNameCrc = 0);
	DWORD	RemoveAccountForbid(LPCTSTR szAccountName,DWORD dwForbidM,DWORD dwWorldNameCrc = 0);

	DWORD	SetAccountChenMi(DWORD dwAccountID,DWORD dwChenMi,DWORD dwWorldNameCrc = 0);

	DWORD	ForbidIP(DWORD dwIP);
	DWORD	RemoveIPForbid(DWORD dwIP);

//	tstring GetAccountName(DWORD dwAccountID);
	VOID	UpdateAccountLoginInfo( DWORD dwAccountID, DWORD &dwIP);
	VOID	LogPlayerAction( DWORD dwAccountID, LPCSTR szAccountName, DWORD dwIP, LPCSTR szAction );
	VOID	InsertKickLog(const CHAR* pAccountName, DWORD dwAccountID, DWORD dwTime, UINT16 u16ErrCode, BOOL bSeal);

	BOOL    UpdateWorldState(DWORD dwWorldID,INT iRoleNum, SHORT sState);
	BOOL    InsertWorldState(DWORD dwWorldID);
	BOOL    ClearWorldStateTable();//�������״̬��

	BOOL	InsertWorldStateLog(DWORD dwWorldID,INT iRoleNum, SHORT sState);
	//-------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------
	INT		GetAvailableStreamNum()			{ return m_DB.GetAvailableStreamNum(); }
	INT		GetAllStreamNum()				{ return m_DB.GetAllStreamNum(); }
	INT		GetUnhandledAsynStreamNum()		{ return m_DB.GetUnhandledAsynStreamNum(); }
	DWORD   GetWorldStateUpdateTime()		{ return m_dwWorldStateUpdateTime;}
	DWORD   GetWorldStateInsertTime()		{ return m_dwWorldStateInsertTime;}

private:
	DataBase		m_DB;					// Beton���ݿ�

	DWORD			m_dwWorldStateUpdateTime;		// ����״̬����ʱ�䣨���룩

	DWORD			m_dwWorldStateInsertTime;		// ����״̬����ʱ�䣨���룩

 //   //�����ݿ��в�ѯ�������
 //   BOOL QueryPlayerData(Player* pPlayer);
	////�����ݿ��в�ѯ����ʺ�����
	
 //   //�������ݿ����������
 //   BOOL UpdatePlayerData(Player* pPlayer);
 //   //��Log�������ݿ�
	
 //   //����ҳɹ���¼��Ϸ���磬�������ݿ��������
	//BOOL UpdatePlayerIntoWorld(EPlayerLoginStatus status, DWORD dwLoginTime, DWORD dwAccULSec, DWORD dwAccOLSec, DWORD dwAccountID);
 //   //������뿪��Ϸ���磬�������ݿ��������
 //   BOOL UpdatePlayerOutWorld(EPlayerLoginStatus status, DWORD dwLogoutTime, DWORD dwOnlineTime, DWORD dwAccOLSec, DWORD dwAccountID);

	////��LoongWorld����,��ո÷������ϵ�¼�����
	//VOID UpdatePlayer_LoongWorld(EPlayerLoginStatus status, DWORD dwWorldNameCrc, DWORD dwLogoutTime);
};

//Beton�ص�����
VOID DBCallBack(DataBase* pDB, INT nReason, INT nParam);

#define sBeton DB::getSingleton()







