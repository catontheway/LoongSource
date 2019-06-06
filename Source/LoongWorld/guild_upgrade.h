//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_upgrade.h
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: ���������Ͱ�����ʩ����
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/guild_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ������ʩ�ṹ
//-----------------------------------------------------------------------------
struct tagGuildFacilities : public tagGuildFacilitiesInfo
{
	INT16	nStep;
	INT32	nBaseExploit;

	tagGuildFacilities()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct tagFacilitiesLoad;
class Guild;

class GuildUpgrade
{
public:
	GuildUpgrade();
	~GuildUpgrade();

	VOID	Init(Guild* pGuild, BOOL bRequest = FALSE);
	BOOL	IsInitOK()	{ return m_bInit; }

	// ���´���ȱʧ��ʩ
	VOID	CreateFacilities();

	// ������ʩ��Ϣ
	BOOL	LoadFacilitiesInfo(const tagFacilitiesLoad* pInfo, INT nNum);

	// ��ȡ��ʩ��Ϣ�ӿ�
	DWORD	GetGuildFacilitiesInfo(tagGuildFacilitiesInfo* pInfo, EFacilitiesType eType = EFT_End);

	// �Ͻ���Ʒ�ӿ�
	DWORD	HandInItems(Role* pRole, EFacilitiesType eType);

	// ���浽���ݿ�
	VOID	SaveUpgradeInfo2DB(EFacilitiesType eType);

	// ������ݿ���������ʩ��Ϣ
	VOID	RemoveAllFacilitiesInfo();
	
	// ����������ʩ�ĵȼ�
	VOID	ChangeFacilitiesLevel(EFacilitiesType eType, BYTE byValue);

	// ���޻�ȡ
	INT32	GetMaxFund()		{ return 2000000*m_Facilities[EFT_Guild].byLevel + 2000000*m_Facilities[EFT_Fund].byLevel; }
	INT32	GetMaxMaterial()	{ return 10000*m_Facilities[EFT_Guild].byLevel + 20000*m_Facilities[EFT_Material].byLevel; }
	BYTE	GetMaxSkillLevel()	{ return 20 + (2 * m_Facilities[EFT_Academy].byLevel); }

private:
	BOOL					m_bInit;
	Guild*					m_pGuild;

	tagGuildFacilities		m_Facilities[EFT_End];
};

//-----------------------------------------------------------------------------
#pragma pack(pop)