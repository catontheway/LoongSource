//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_mgr.h
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: ���ɹ�����
//-------------------------------------------------------------------------------
#pragma once
#include "event_mgr.h"
#include "../WorldDefine/guild_define.h"
#include "../ServerDefine/guild_define.h"

class Guild;
class Role;

struct tagGuildLoad;
struct tagGuildMemLoad;
//-------------------------------------------------------------------------------
class GuildMgr
{
public:
	GuildMgr();
	~GuildMgr();

	BOOL	Init();
	VOID	Update();
	VOID	Destroy();

	INT32	GetGuildNum();
	Guild*	GetGuild(DWORD dwGuildID);

	DWORD	CreateGuild(Role* pCreator, LPCTSTR strGuildName, INT32 nNumTCHAR);
	DWORD	DismissGuild(Role* pLeader, DWORD dwGuildID);

	// ��LoongDB���Ӻ����а�����س�ʼ��
	VOID	InitDBGuild(const tagGuildLoad* pGuildLoad);
	VOID	InitDBGuildMember(const tagGuildMemLoad *pGuildMemberLoad, const INT32 nNum);
	BOOL	IsGuildInitOK();

	// �ر�LoongWorldʱ���������а��ɻ�����Ϣ
	VOID	SendAllGuild2DB();

	// �ͻ������Ӻ󣬳�ʼ�����ڰ�����Ϣ
	VOID	InitGuildInitState(Role *pRole, 
							OUT tagGuildBase &sGuildBase, 
							OUT tagGuildMember &sGuildMember);
	
	// �ͻ���������ʧ�ܺ�����Ϣ
	VOID	SendGuildProcFailed2Client(PlayerSession *pSession, DWORD dwErrorCode);

	// ɾ����ɫʱ�����������
	DWORD	ClearRoleRemove(DWORD dwRoleID);

	// ������Ϣ
	const tagGuildCfg& GetGuildCfg() const;

	// ����ɲֿ���Ӳ����¼�
	VOID AddGuildWareEvent(DWORD dwSender, EEventType eType, DWORD dwSize, VOID* pMsg);

	// ���ɼ�����������
	VOID UpgradeGuildSkill();

	// ÿ��2:00�۳����Ĳ����ð���״̬
	VOID DailyGuildReset();

private:
	Guild*	CreateGuild(Role* pCreator, const tstring& strGuildName, DWORD dwNewGuildID);

	// Ҫ��������ݿ�
	VOID	SendCreateGuild2DB(const tagGuild& sGuild);
	VOID	SendDismissGuild2DB(DWORD dwGuildID);

private:
	typedef TMap<DWORD, Guild*>		MapGuild;	// <dwGuildID, pGuild>

	tagGuildCfg			m_GuildCfg;
	MapGuild			m_mapGuild;
};



/*************************** ����ʵ�� *******************************/

//-------------------------------------------------------------------------------
// ��ȡ������Ϣ
//-------------------------------------------------------------------------------
inline const tagGuildCfg& GuildMgr::GetGuildCfg() const
{
	return m_GuildCfg;
}

//-------------------------------------------------------------------------------
// ��ȡ���ɶ������
//-------------------------------------------------------------------------------
inline INT32 GuildMgr::GetGuildNum()
{
	return m_mapGuild.Size();
}

//-------------------------------------------------------------------------------
// ��ȡ���ɶ���
//-------------------------------------------------------------------------------
inline Guild* GuildMgr::GetGuild(DWORD dwGuildID)
{
	return m_mapGuild.Peek(dwGuildID);
}

//-------------------------------------------------------------------------------
extern GuildMgr g_guildMgr;