//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_upgrade.cpp
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: ���������Ͱ�����ʩ����
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/guild_define.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "../WorldDefine/msg_guild.h"
#include "../WorldDefine/guild_define.h"

#include "att_res.h"
#include "world.h"
#include "role.h"
#include "guild.h"
#include "guild_upgrade.h"

//-----------------------------------------------------------------------------
// ���������
//-----------------------------------------------------------------------------
GuildUpgrade::GuildUpgrade()
{
}

GuildUpgrade::~GuildUpgrade()
{
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
VOID GuildUpgrade::Init(Guild* pGuild, BOOL bRequest /*= FALSE*/)
{
	if (!P_VALID(pGuild))
	{
		ASSERT(pGuild);
		return;
	}

	m_bInit		= FALSE;
	m_pGuild	= pGuild;

	if (bRequest)
	{
		tagNDBC_LoadFacilitiesInfo send;
		send.dwGuildID = m_pGuild->GetGuildAtt().dwID;
		g_dbSession.Send(&send, send.dwSize);
	}
}

//-----------------------------------------------------------------------------
// ��ȡ��ʩ������Ϣ
//-----------------------------------------------------------------------------
DWORD GuildUpgrade::GetGuildFacilitiesInfo( tagGuildFacilitiesInfo* pInfo, EFacilitiesType eType /*= EFT_End*/ )
{
	// �ϲ��뱣֤pInfo�ṹ�Ĵ�С
	if (!P_VALID(pInfo))
		return GT_INVALID;

	if (!m_bInit)
		return GT_INVALID;

	if (eType != EFT_End)
	{
		IFASTCODE->MemCpy(pInfo, &m_Facilities[eType], sizeof(tagGuildFacilitiesInfo));
	}
	else
	{
		for (int n=EFT_Guild; n<EFT_End; n++)
		{
			IFASTCODE->MemCpy(&pInfo[n], &m_Facilities[n], sizeof(tagGuildFacilitiesInfo));
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// �Ͻ���Ʒ
//-----------------------------------------------------------------------------
DWORD GuildUpgrade::HandInItems( Role* pRole, EFacilitiesType eType )
{
	// �ϲ���Ҫ��֤�����ĺϷ���
	ASSERT(P_VALID(pRole));

	if (!m_bInit)
		return GT_INVALID;

	if (eType <= EFT_Null || eType >= EFT_End)
	{
		return GT_INVALID;
	}

	// �ж���ҵȼ�
	if (pRole->GetLevel() < 20)
	{
		return E_GuildUpgrade_Role_Level;
	}

	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}

	// �жϸ���ҵĵ�λ
	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bUpgrade)
	{
		return E_GuildWare_Power_NotEnough;
	}

	// �ж���ʩ�Ƿ�ﵽ�ȼ�����
	if (m_Facilities[eType].byLevel >= MAX_GUILD_LEVEL)
	{
		return E_GuildUpgrade_Level_Limit;
	}

	// �жϰ���״̬���ʽ��ʲ����
	if (m_pGuild->IsInGuildStateAny(EGDSS_Shortage | EGDSS_Distress | EGDSS_Chaos | EGDSS_Warfare))
	{
		return E_Guild_State_Limit;
	}
	const tagGuild& sGuildAtt = m_pGuild->GetGuildAtt();
	if (sGuildAtt.nFund < m_Facilities[eType].nNeedFund)
	{
		return E_Guild_Fund_NotEnough;
	}
	if (sGuildAtt.nMaterial < m_Facilities[eType].nMaterial)
	{
		return E_Guild_Material_NotEnough;
	}

	// �����ұ�������Ʒ�Ƿ���
	for (int n=0; n<MAX_UPGRADE_NEED_ITEM_TYPE; n++)
	{
		INT32 nItemNum = pRole->GetItemMgr().GetBagSameItemCount(m_Facilities[eType].dwItemID[n]);
		if (nItemNum < m_Facilities[eType].nNeedNum[n])
		{
			return E_GuildUpgrade_Item_NotEnough;
		}
	}

	// �۳������ʽ���ʲ�
	m_pGuild->DecGuildFund(pRole->GetID(), m_Facilities[eType].nNeedFund, ELCLD_Guild_Upgrade);
	m_pGuild->DecGuildMaterial(pRole->GetID(), m_Facilities[eType].nMaterial, ELCLD_Guild_Upgrade);

	// �۳������Ʒ
	for (int n=0; n<MAX_UPGRADE_NEED_ITEM_TYPE; n++)
	{
		pRole->GetItemMgr().RemoveFromRole(m_Facilities[eType].dwItemID[n], m_Facilities[eType].nNeedNum[n], ELCLD_Guild_Upgrade);
	}

	// ��������
	m_Facilities[eType].nProgress += m_Facilities[eType].nStep;

	// Reset��ʩ������Ϣ
	m_Facilities[eType].ResetItemInfo();

	// ������ҹ���ֵ
	m_pGuild->ChangeMemberContribution(pRole->GetID(), m_Facilities[eType].nBaseExploit, TRUE);

	// �ж���ʩ�Ƿ�����
	if (m_Facilities[eType].IsLevelUp())
	{
		// �����ȡ��һ������������Ʒ
		m_Facilities[eType].byLevel++;
		m_Facilities[eType].nProgress	= 0;
		if (m_Facilities[eType].byLevel >= MAX_GUILD_LEVEL)
		{
			// ������ʩ����
			m_Facilities[eType].byLevel		= MAX_GUILD_LEVEL;
			m_Facilities[eType].nFulfill	= 0;
			m_Facilities[eType].nNeedFund	= 0;
			m_Facilities[eType].nMaterial	= 0;
			m_Facilities[eType].nStep		= 0;
			m_Facilities[eType].nBaseExploit= 0;
		}
		else
		{
			g_attRes.GetGuildUpgradeItemInfo(eType, m_Facilities[eType].byLevel, m_Facilities[eType]);
		}
		if (eType == EFT_Guild)
		{
			m_pGuild->ReInitbyGuildUpgrade(m_Facilities[eType].byLevel);
		}

		// �㲥������ʩ������Ϣ
		tagNS_GuildUpgrade send;
		send.eType		= eType;
		send.byNewLevel = m_Facilities[eType].byLevel;
		m_pGuild->SendGuildMsg(&send, send.dwSize);
	}
	else
	{
		// �����ȡ������������Ʒ
		g_attRes.GetGuildUpgradeItemInfo(eType, m_Facilities[eType].byLevel, m_Facilities[eType]);
	}

	// ���浽���ݿ�
	SaveUpgradeInfo2DB(eType);

	return 0;
}

//-----------------------------------------------------------------------------
// ���浽���ݿ�
//-----------------------------------------------------------------------------
VOID GuildUpgrade::SaveUpgradeInfo2DB( EFacilitiesType eType )
{
	// ������ͺϷ���
	if (eType >= EFT_End || eType <= EFT_Null)
	{
		return;
	}

	// ���ɴ�������
	tagNDBC_FacilitiesUpgrade send;
	send.sFacilitiesInfo.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
	send.sFacilitiesInfo			= m_Facilities[eType];

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// ������ʩ��Ϣ
//-----------------------------------------------------------------------------
BOOL GuildUpgrade::LoadFacilitiesInfo( const tagFacilitiesLoad* pInfo, INT nNum )
{
	if (!P_VALID(pInfo) || nNum != MAX_GUILD_FACILITIES_TYPE)
	{
		return FALSE;
	}

	for (int m=0; m<nNum; m++)
	{
		// ������ͺϷ���
		EFacilitiesType eType = pInfo[m].eType;
		if (eType >= EFT_End || eType <= EFT_Null)
		{
			return FALSE;
		}

		// �������ݿⱣ������
		m_Facilities[eType].eType		= (EFacilitiesType)eType;
		m_Facilities[eType].byLevel		= pInfo[m].byLevel;
		if (eType == EFT_Guild)
		{
			m_pGuild->ReInitbyGuildUpgrade(pInfo[m].byLevel);
		}
		m_Facilities[eType].nProgress	= pInfo[m].n16Progress;
		for (int n=0; n<MAX_UPGRADE_NEED_ITEM_TYPE; n++)
		{
			m_Facilities[eType].dwItemID[n]	= pInfo[m].dwItemTypeID[n];
			m_Facilities[eType].nNeedNum[n]	= pInfo[m].nItemNeed[n];
		}

		// ������ʩ��������
		if (!g_attRes.GetGuildUpgradeBaseInfo(eType, pInfo[m].byLevel, m_Facilities[eType]))
		{
			m_Facilities[eType].ResetItemInfo();
		}	
	}

	// ��ʼ�����
	m_bInit = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ������ʩ
//-----------------------------------------------------------------------------
VOID GuildUpgrade::CreateFacilities()
{
	for (int n=EFT_Guild; n<EFT_End; n++)
	{
		// ����World������ʩ
		if (n == EFT_Guild)
		{
			m_Facilities[n].byLevel	= 1;
			m_pGuild->ReInitbyGuildUpgrade(1);
		}
		else
		{
			m_Facilities[n].byLevel = 0;
		}
		m_Facilities[n].nProgress	= 0;
		g_attRes.GetGuildUpgradeItemInfo((BYTE)n, m_Facilities[n].byLevel, m_Facilities[n]);

		// ֪ͨ���ݿ�
		tagNDBC_CreateFacilities send;
		send.sFacilitiesInfo.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
		send.sFacilitiesInfo			= m_Facilities[n];

		g_dbSession.Send(&send, send.dwSize);
	}

	// ��ʼ�����
	m_bInit = TRUE;
}

//-----------------------------------------------------------------------------
// ������ݿ�������ʩ��Ϣ
//-----------------------------------------------------------------------------
VOID GuildUpgrade::RemoveAllFacilitiesInfo()
{
	if (!m_bInit)
	{
		return;
	}

	tagNDBC_RemoveAllFacilities send;
	send.dwGuildID = m_pGuild->GetGuildAtt().dwID;

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// ������ʩ�ĵȼ�
//-----------------------------------------------------------------------------
VOID GuildUpgrade::ChangeFacilitiesLevel(EFacilitiesType eType, BYTE byValue)
{
	m_Facilities[eType].byLevel = byValue;
	if (m_Facilities[eType].byLevel > MAX_GUILD_LEVEL)
		m_Facilities[eType].byLevel = MAX_GUILD_LEVEL;
	if (m_Facilities[eType].byLevel < 0)
		m_Facilities[eType].byLevel = 0;

	if (eType == EFT_Guild)
	{
		m_pGuild->ReInitbyGuildUpgrade(m_Facilities[EFT_Guild].byLevel);
	}

	// Reset��ʩ������Ϣ
	m_Facilities[eType].ResetItemInfo();

	// �����ȡ������������Ʒ
	g_attRes.GetGuildUpgradeItemInfo(eType, m_Facilities[eType].byLevel, m_Facilities[eType]);

	// ���浽���ݿ�
	SaveUpgradeInfo2DB(eType);
}
