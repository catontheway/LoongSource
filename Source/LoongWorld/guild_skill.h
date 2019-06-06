//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_skill.h
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: ���ɼ���
//-----------------------------------------------------------------------------
#pragma once

class Guild;
class Role;
struct tagGuildSkill;
struct tagGuildSkillInfo;

class GuildSkill
{
public:
	GuildSkill();
	~GuildSkill();

	VOID	Init(Guild* pGuild, BOOL bRequest = FALSE);
	VOID	Destroy();

	BOOL	IsInitOK()	{ return m_bInit; }

public:
	// ������ɼ�����Ϣ
	BOOL	LoadGuildSkillIInfo(tagGuildSkillInfo* pSkillInfo, INT nSkillNum);

	// ��ȡ���а��ɼ�����Ϣ
	DWORD	GetGuildSkillInfo(DWORD& dwSkillID, INT16& n16Progress, INT& nSkillNum, DWORD* pLevelInfo);

	// ��������
	DWORD	SkillUpgradeOnClock();

	// �Ͻ���Ʒ����
	DWORD	SkillUpgradeByHandIn(Role* pRole, INT64 n64Serial, DWORD& dwSkillID, INT16& n16Progress);

	// ѧϰ����
	DWORD	LearnGuildSkill(Role* pRole, DWORD dwSkillID, INT& nLevel, BOOL& bLearnError);

	// �������ɼ���
	VOID	CreateGuildSkill();

	// ���õ�ǰ��������
	DWORD	SetCurUpgradeGuildSkill(DWORD dwRoleID, DWORD dwSkillID, INT& nLevel, INT16& n16Progress);

	// ������а��ɼ�����Ϣ
	VOID	RemoveGuildSkillInfo();

private:
	// �����ݿ�ͨѶ
	VOID	SaveSkillInfo2DB(DWORD dwSkillID);
	VOID	ChangeResearchSkill(DWORD dwNewSkillID);
	
private:
	typedef TMap<DWORD, tagGuildSkill*>	MapGuildSkill;

	BOOL			m_bInit;
	Guild*			m_pGuild;

	// ����������Ϣ
	DWORD			m_dwCurSkill;
	MapGuildSkill	m_mapGuildSkill;
};