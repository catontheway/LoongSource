//-----------------------------------------------------------------------------
//!\file game_guarder.h
//!\author xlguo
//!
//!\date 2009-03-02
//! last 
//!
//!\brief �����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "mutex.h"

//-----------------------------------------------------------------------------
// ipת��
//-----------------------------------------------------------------------------
#pragma pack(push,1)
struct tagUserIP 
{
	char			chUserIPFlag;
	unsigned int	uIP;

#define ClientIpFlag  0x01
	tagUserIP(unsigned int ip = 0):chUserIPFlag(ClientIpFlag),uIP(ip){}
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
// ����ҿͻ�����Ϣ
//-----------------------------------------------------------------------------
struct tagNS_GameGuarder;

//-----------------------------------------------------------------------------
// �˺ź���Ϣ�ķ�װ
//-----------------------------------------------------------------------------
struct tagAccountMsg
{
	DWORD				dwAccountID;
	tagNS_GameGuarder*	pMsg;
};

//-----------------------------------------------------------------------------
// ���͸�Login��������Ϣ
//-----------------------------------------------------------------------------
struct tagNWL_KickLog;

//-----------------------------------------------------------------------------
// ����Ұ�װ
//-----------------------------------------------------------------------------
class GameGuarder
{
public:
	GameGuarder();
	~GameGuarder();

	//-----------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------
	VOID		Switch(BOOL bOn){	if (bOn != m_bEnable)InterlockedExchange((LPLONG)&m_bEnable, bOn);	}

	//-----------------------------------------------------------------------------
	// ���߳�UPDATE
	//-----------------------------------------------------------------------------
	VOID		Update();

public:
	//-----------------------------------------------------------------------------
	// ���������
	//-----------------------------------------------------------------------------
	long		Login(DWORD dwAccountID, LPCSTR szAccount, DWORD dwIP);
	long		Logout(DWORD dwAccountID, LPCSTR szAccount);
	long		Ret(DWORD dwAccountID, const char* pBuffer);
	long		Transport(DWORD dwAccountID, const char* pBuffer, const int nLen);
	long		UserData(DWORD dwAccountID, const char* pBuffer, const int nLen);

private:
	//-----------------------------------------------------------------------------
	// �ڲ�����
	//-----------------------------------------------------------------------------
	long		Recv(char cMsgId,DWORD dwAccountID,const char * pBuffer,int nLen);
	
	//-----------------------------------------------------------------------------
	// ���������߳�ִ��
	//-----------------------------------------------------------------------------
	VOID		Wait2Send( INT nAccountID, INT nLen, LPCSTR pBuffer );
	VOID		Wait2Kick(INT nAccountID, DWORD dwKickTime, UINT16 u16ErrCode, UINT16 u16SealMark);
	
	//-----------------------------------------------------------------------------
	// ���߳�ִ��
	//-----------------------------------------------------------------------------
	static long	SendMsg(signed int nAccountID,const char * pBuffer,int nLen);
	static VOID SendKickOut(const tagNWL_KickLog* pKickLog);

	//-----------------------------------------------------------------------------
	// ���˻ص�
	//-----------------------------------------------------------------------------
	static long	Kick(signed int nAccountID,int nAction);
	
	//-----------------------------------------------------------------------------
	// ����ɾ����Ϣ
	//-----------------------------------------------------------------------------
	static tagNS_GameGuarder*	CreateGuardMsg( INT nLen );
	static VOID	DeleteGuardMsg( tagNS_GameGuarder* pMsg );

private:
	static long (*m_spfRec) (char cMsgId,signed int nId,const char * pBuffer,int nLen);
	static BOOL		m_bEnable;

private:
	TSafeList<tagNWL_KickLog*>	m_listKickAccount;

	std::list<tagAccountMsg>	m_listMsg;
	Mutex						m_LockMsg;
};

extern GameGuarder g_gameGuarder;
