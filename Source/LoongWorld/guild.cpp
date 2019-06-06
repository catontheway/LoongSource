//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild.cpp
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: ����
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "guild.h"
#include "role.h"
#include "role_mgr.h"
#include "guild_mgr.h"
#include "guild_warehouse.h"
#include "guild_upgrade.h"

#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/msg_log.h"
#include "../ServerDefine/role_data.h"

#include "../WorldDefine/msg_guild.h"

const tagGuildPower		Guild::m_NullGuildPower;
const tagGuildKick		Guild::m_NullGuildKick;
const tagGuildAppoint	Guild::m_NullGuildAppoint;
//-------------------------------------------------------------------------------
// ��ȡ����Ȩ�ޱ�
//-------------------------------------------------------------------------------
const tagGuildPower& Guild::GetGuildPower(EGuildMemberPos eGuildPos) const
{
	if(!::IsGuildPosValid(eGuildPos))
	{
		ASSERT(0);
		IMSG(_T("\n\n\nCaution:\tThere is a critical error<invalid guildpos> in guild power table!\n\n"));
		return Guild::m_NullGuildPower;
	}

	return g_attRes.GetGuildPower(eGuildPos);
}

const tagGuildKick& Guild::GetGuildKick(EGuildMemberPos eGuildPos) const
{
	if(!::IsGuildPosValid(eGuildPos))
	{
		ASSERT(0);
		IMSG(_T("\n\n\nCaution:\tThere is a critical error<invalid guildpos> in guild kick table!\n\n"));
		return Guild::m_NullGuildKick;
	}

	return g_attRes.GetGuildKick(eGuildPos);
}

const tagGuildAppoint& Guild::GetGuildAppoint(EGuildMemberPos eGuildPos) const
{
	if(!::IsGuildPosValid(eGuildPos))
	{
		ASSERT(0);
		IMSG(_T("\n\n\nCaution:\tThere is a critical error<invalid guildpos> in guild appoint table!\n\n"));
		return Guild::m_NullGuildAppoint;
	}

	return g_attRes.GetGuildAppoint(eGuildPos);
}

//-------------------------------------------------------------------------------
// ����&����
//-------------------------------------------------------------------------------
Guild::Guild()
{
}

Guild::Guild(const tagGuildLoad *pGuildLoad)
{
	m_GuildPos.Init(pGuildLoad->byLevel);
	m_MemberMgr.Init(pGuildLoad->dwID);

	m_Att.dwID				= pGuildLoad->dwID;
	m_Att.dwFounderNameID	= pGuildLoad->dwFounderNameID;
	m_Att.dwLeaderRoleID	= pGuildLoad->dwLeaderRoleID;

	// ����״̬
	m_Att.dwSpecState		= pGuildLoad->dwSpecState;
	m_GuildState.SetState((EGuildSpecState)m_Att.dwSpecState);

	m_Att.byLevel			= pGuildLoad->byLevel;
	
	m_Att.nFund				= pGuildLoad->nFund;
	m_Att.nMaterial			= pGuildLoad->nMaterial;
	m_Att.nReputation		= pGuildLoad->nReputation;
	m_Att.n16Peace			= pGuildLoad->n16Peace;
	m_Att.n16MemberNum		= pGuildLoad->n16MemberNum;
	m_Att.n16Rank			= pGuildLoad->n16Rank;
	m_Att.byMinJoinLevel	= pGuildLoad->byMinJoinLevel;
	m_Att.nDailyCost		= pGuildLoad->nDailyCost;
	m_Att.nGroupPurchase	= pGuildLoad->nGroupPurchase;
	m_Att.byAffairRemainTimes = pGuildLoad->byAffairRemainTimes;
	m_Att.bCommendation		= pGuildLoad->bCommendation;

	// ������,��ּ,�ձ�
	m_Att.strName			= pGuildLoad->szName;
	m_Att.strTenet			= pGuildLoad->szTenet;
	m_Att.strSymbol			= pGuildLoad->szSymbolURL;
	
	// ��������
	for(INT32 i=0; i<MAX_GUILD_HOLDCITY; ++i)
	{
		m_Att.byHoldCity[i]	= pGuildLoad->byHoldCity[i];
	}

	// ������ʩ��ʼ��
	m_GuildUpgrade.Init(this, TRUE);

	// �����ʼ��(���������ݿ�����)
	m_GuildAffair.Init(this);

	// ���ɼ��ܳ�ʼ��
	m_GuildSkill.Init(this, TRUE);

	// �������̳�ʼ��
	m_GuildCommerce.Init(this, TRUE);

	// ���ɲֿ��ʼ��
	m_GuildWarehouse.Init(this, TRUE);
}

Guild::~Guild()
{
}

//-------------------------------------------------------------------------------
// �������ݿ����ݳ�ʼ��
//-------------------------------------------------------------------------------
VOID Guild::InitDBGuildMember(const tagGuildMember& sGuildMember)
{
	if(LoadMember(sGuildMember) != E_Success)
	{
		// ִ�е��ô�����������ݿ����ݿ��д���
		ASSERT(0);
		return;
	}

	// �������ݿ��г�Ա���ݣ���������
	tagGuildMember *pMember = GetMember(sGuildMember.dwRoleID);
	ASSERT(P_VALID(pMember) && sGuildMember.eGuildPos == pMember->eGuildPos);
	
	*pMember = sGuildMember;
}

//-------------------------------------------------------------------------------
// update
//-------------------------------------------------------------------------------
VOID Guild::Update()
{
	// �������뺯
	if(!m_mapInvite.Empty())
	{
		DWORD dwInviteeID = GT_INVALID;
		tagInvite *pInviter = NULL;
		MapInvite::TMapIterator iter = m_mapInvite.Begin();
		while(m_mapInvite.PeekNext(iter, dwInviteeID, pInviter))
		{
			if(--pInviter->nInviteCountDown <= 0)
			{
				m_mapInvite.Erase(dwInviteeID);
				delete pInviter;
			}
		}
	}

	// ������ɲֿ�
	m_GuildWarehouse.Update();
}

//-------------------------------------------------------------------------------
// check
//-------------------------------------------------------------------------------
BOOL Guild::IsGuildInitOK()
{
	BOOL bRet = TRUE;

	if(GetGuildMemberNum() <= 0)
	{
		ASSERT(0);
		ILOG->Write(_T("Guild<id: %u> members load failed! Please check db! \r\n"), m_Att.dwID);
		bRet = FALSE;
	}

	if (!m_GuildAffair.IsInitOK())
	{
		ASSERT(0);
		ILOG->Write(_T("Guild<id: %u> affair init failed! Please check! \r\n"), m_Att.dwID);
		bRet = FALSE;
	}

	if (!m_GuildUpgrade.IsInitOK())
	{
		ASSERT(0);
		ILOG->Write(_T("Guild<id: %u> facilities init failed! Please check! \r\n"), m_Att.dwID);
		bRet = FALSE;
	}

	if (!m_GuildSkill.IsInitOK())
	{
		ASSERT(0);
		ILOG->Write(_T("Guild<id: %u> skill init failed! Please check! \r\n"), m_Att.dwID);
		bRet = FALSE;
	}

	if (!m_GuildCommerce.IsInitOK())
	{
		ASSERT(0);
		ILOG->Write(_T("Guild<id: %u> commodity init failed! Please check! \r\n"), m_Att.dwID);
		bRet = FALSE;
	}

	if (!m_GuildWarehouse.IsInitOK())
	{
		ASSERT(0);
		ILOG->Write(_T("Guild<id: %u> warehouse init failed! Please check! \r\n"), m_Att.dwID);
		bRet = FALSE;
	}

	return bRet;
}

//-------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------
VOID Guild::InitCreate(Role* pCreator, const tstring& strGuildName, 
						DWORD dwGuildID, const tagGuildCfg &sGuildCfg)
{
	m_Att.Init(strGuildName, dwGuildID, pCreator->GetID(), pCreator->GetNameID(), 
				sGuildCfg.byGuildLevelBegin, sGuildCfg.n16GuildPeaceBegin, 
				sGuildCfg.nGuildFundBegin, sGuildCfg.nGuildMaterialBegin, 
				sGuildCfg.nGuildRepBegin, sGuildCfg.nGuildGroupPurchaseBegin);

	m_GuildPos.Init(m_Att.byLevel);
	m_MemberMgr.Init(dwGuildID);

	// ���ô���������
	AddMember(pCreator->GetID(), EGMP_BangZhu);
	pCreator->SetGuildID(dwGuildID);

	// ������ʩ
	m_GuildUpgrade.Init(this);
	m_GuildUpgrade.CreateFacilities();

	// �����ʼ��
	m_GuildAffair.Init(this);

	// ���ɼ��ܳ�ʼ��
	m_GuildSkill.Init(this);
	m_GuildSkill.CreateGuildSkill();

	// �������̳�ʼ��
	m_GuildCommerce.Init(this);

	// ���ɲֿ�(��ñ�֤�ֿ�����ʩ֮��,���������Ҫreinit)
	m_GuildWarehouse.Init(this);
}

//-------------------------------------------------------------------------------
// ��ɢ����
//-------------------------------------------------------------------------------
DWORD Guild::DismissGuild(Role* pRole)
{
	ASSERT(P_VALID(pRole));

	// ��ȡ���ɳ�Ա��Ϣ
	tagGuildMember *pMember = GetMember(pRole->GetID());
	if(!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}

	// �Ƿ���Ȩ��
	if(!GetGuildPower(pMember->eGuildPos).bDismissGuild)
	{
		return E_Guild_Power_NotEnough;
	}

	// �Ƿ���������Ա
	if(m_MemberMgr.GetNumber() > 1)
	{
		return E_Guild_MemberHasMore;
	}

	// ��ճ�Ա -- �ô���Ҫ���ã����޸����ݿ��
	RemoveMember(pRole->GetID(), pMember->eGuildPos);

	// ��������������ʱ�Զ����

	// ���ý�ɫ����
	pRole->SetGuildID(GT_INVALID);

	// ��հ��ɲֿ�
	m_GuildWarehouse.RemoveAllItems();

	// ���������ʩ��Ϣ
	m_GuildUpgrade.RemoveAllFacilitiesInfo();

	// ������ɼ�����Ϣ
	m_GuildSkill.RemoveGuildSkillInfo();

	// ���������Ϣ(���ɽ�ɢʱֻ�д���)
	m_GuildCommerce.AbandonCommerce(pRole->GetID(), TRUE);

	return E_Success;
}

//-------------------------------------------------------------------------------
// �ܷ���ӳ�Ա -- bInsert��== TRUE������������б�==FALSE�����б���ɾ��
//-------------------------------------------------------------------------------
DWORD Guild::CanInviteJoin(DWORD dwInviterID, DWORD dwInviteeID, 
						   Role **ppInvitee, BOOL bInsert/* = TRUE*/)
{
	// �ҵ�������
	tagGuildMember *pInviter = GetMember(dwInviterID);
	if(!P_VALID(pInviter))
	{
		return GT_INVALID;
	}

	// �ж�Ȩ��
	if(!GetGuildPower(pInviter->eGuildPos).bInviteJoin)
	{
		return E_Guild_Power_NotEnough;
	}

	// �Ƿ�ﵽ��������
	if(m_GuildPos.IsMemberFull())
	{
		return E_Guild_Member_Full;
	}

	// �ҵ���������
	Role *pInvitee = g_roleMgr.GetRolePtrByID(dwInviteeID);
	if(!P_VALID(pInvitee))
	{
		return E_Role_Not_Online;
	}

	// �Ƿ��Ѽ������
	if(pInvitee->IsInGuild())
	{
		// �����ط����߼�����
		ASSERT(P_VALID(g_guildMgr.GetGuild(pInvitee->GetGuildID())));

		return E_Guild_Join_AlreadyIn;
	}

	// �ȼ��ж�
	if(pInvitee->GetLevel() < g_guildMgr.GetGuildCfg().nJoinRoleMinLevel)
	{
		return E_Guild_Join_LevelLimit;
	}

	if(bInsert)	// ���뵽�������б�
	{
		// �Ƿ����ڴ������б���
		if(m_mapInvite.IsExist(dwInviteeID))
		{
			return E_Guild_Join_BeInvited;
		}
		
		tagInvite *pInvite = new tagInvite(dwInviterID);
		ASSERT(P_VALID(pInvite));
		m_mapInvite.Add(dwInviteeID, pInvite);
	}
	else
	{
		// �Ƿ����ڴ������б���
		if(!m_mapInvite.IsExist(dwInviteeID))
		{
			return E_Guild_Join_NotBeInvited;
		}

		tagInvite *pInviter = m_mapInvite.Peek(dwInviteeID);
		ASSERT(P_VALID(pInviter));
		delete pInviter;
		
		m_mapInvite.Erase(dwInviteeID);
	}

	if(ppInvitee != NULL)
	{
		*ppInvitee = pInvitee;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------
// ɾ��ָ��������
//-------------------------------------------------------------------------------
BOOL Guild::EraseInviteJoin(DWORD dwInviteeID)
{
	return m_mapInvite.Erase(dwInviteeID);
}

//-------------------------------------------------------------------------------
// ��ӳ�Ա
//-------------------------------------------------------------------------------
DWORD Guild::InviteJoin(DWORD dwInviterID, DWORD dwInviteeID)
{
	Role *pInvitee = NULL;

	// �ٴ��ж�
	DWORD dwErrorCode = CanInviteJoin(dwInviterID, dwInviteeID, &pInvitee, FALSE);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ��ӵ���Ա�б��У�������Ϊ����
	AddMember(dwInviteeID, EGMP_BangZhong);

	// ���ý�ɫ�����ֶ�
	pInvitee->SetGuildID(m_Att.dwID);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��ְ
//-------------------------------------------------------------------------------
DWORD Guild::Appoint(DWORD dwAppointorID, DWORD dwAppointeeID, EGuildMemberPos eGuildPos)
{
	// ��ó�Ա����
	tagGuildMember *pAppointor = GetMember(dwAppointorID);
	if(!P_VALID(pAppointor))
	{
		// ִ�е��ˣ�˵���߼����д���
		ASSERT(0);
		return GT_INVALID;
	}

	tagGuildMember *pAppointee = GetMember(dwAppointeeID);
	if(!P_VALID(pAppointee))
	{
		return E_Guild_MemberNotIn;
	}

	// ��ְǰ��ְλ�Ƿ���ͬ
	if(pAppointee->eGuildPos == eGuildPos)
	{
		return E_Guild_Appoint_SamePos;
	}
	
	// Ȩ��
	if(!GetGuildAppoint(pAppointor->eGuildPos).BitSetGuildAppoint[eGuildPos]
		|| !GetGuildAppoint(pAppointor->eGuildPos).BitSetGuildAppoint[pAppointee->eGuildPos])
	{
		return E_Guild_Power_NotEnough;
	}

	// ִ������Ҫ��
	DWORD dwErrorCode = m_GuildPos.SpecGuildPosLimit(dwAppointeeID, eGuildPos);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}
	
	// ���ְλ����Ա�Ƿ�����
	if(m_GuildPos.IsPosFull(eGuildPos))
	{
		return E_Guild_Pos_Full;
	}

	// ����
	return ChangeGuildPos(pAppointee, eGuildPos);
}

//-------------------------------------------------------------------------------
// �޳���Ա
//-------------------------------------------------------------------------------
DWORD Guild::KickMember(DWORD dwRoleID, DWORD dwKickID)
{
	// ��ȡ���ɳ�Ա����
	tagGuildMember *pMember = GetMember(dwRoleID);
	if(!P_VALID(pMember))
	{
		return GT_INVALID;
	}

	tagGuildMember *pBeKicked = GetMember(dwKickID);
	if(!P_VALID(pBeKicked))
	{
		return GT_INVALID;
	}

	// Ȩ��
	if(!GetGuildKick(pMember->eGuildPos).BitSetGuildKick[pBeKicked->eGuildPos])
	{
		return E_Guild_Power_NotEnough;
	}

	// ����
	RemoveMember(dwKickID, pBeKicked->eGuildPos);

	// �����ɫ���ߣ������ɫ��Ϣ�а�����Ϣ
	Role *pRoleKicked = g_roleMgr.GetRolePtrByID(dwKickID);
	if(P_VALID(pRoleKicked))
	{
		pRoleKicked->SetGuildID(GT_INVALID);
	}

	// ��������ߣ�����������߳�ʼ��������Ϣʱ����

	return E_Success;
}

//-------------------------------------------------------------------------------
// �ƽ�����
//-------------------------------------------------------------------------------
DWORD Guild::TurnoverLeader(DWORD dwOldLeaderID, DWORD dwNewLeaderID)
{
	// ��ȡ��������
	tagGuildMember *pOldLeader = GetMember(dwOldLeaderID);
	if(!P_VALID(pOldLeader))
	{
		return GT_INVALID;
	}

	tagGuildMember *pNewLeader = GetMember(dwNewLeaderID);
	if(!P_VALID(pNewLeader))
	{
		return GT_INVALID;
	}

	// Ȩ��
	if(!GetGuildPower(pOldLeader->eGuildPos).bTurnoverLeader)
	{
		return E_Guild_Power_NotEnough;
	}
	
	/**********************************************************/
	/* �ƽ�
	/**********************************************************/
	//// .1ԭ������ȥְ��
	//m_MemberMgr.SetGuildPos(pOldLeader, EGMP_BangZhong, TRUE);

	//// .2�°�����ȥԭְ��
	//if(::IsInTang(pNewLeader->eGuildPos))
	//{
	//	EGuildTangType eTang;
	//	ETangMemberPos eTangPos;
	//	::GetTangType(pNewLeader->eGuildPos, eTang, eTangPos);
	//	m_Tang[eTang].RemoveMember(dwNewLeaderID, eTangPos);
	//}

	//// .3�°�����ְ
	//m_MemberMgr.SetGuildPos(pNewLeader, EGMP_BangZhu);

	// .1
	ChangeGuildPos(pOldLeader, EGMP_BangZhong);
	ChangeGuildPos(pNewLeader, EGMP_BangZhu);

	// .2�������ΰ���
	SetGuildBangzhu(dwNewLeaderID);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��ְ
//-------------------------------------------------------------------------------
DWORD Guild::DemissPostion(DWORD dwRoleID, OUT INT8 &n8OldGuildPos)
{
	// ��ȡ���ɳ�Ա����
	tagGuildMember *pMember = GetMember(dwRoleID);
	if(!P_VALID(pMember))
	{
		return GT_INVALID;
	}

	// �ж�Ȩ��
	if(!GetGuildPower(pMember->eGuildPos).bDemissPostion)
	{
		return E_Guild_CanNot_Demiss;
	}

	// ��ȡԭְλ����
	n8OldGuildPos = pMember->eGuildPos;
	
	// ��ȥְ��
	ChangeGuildPos(pMember, EGMP_BangZhong);
	
	return E_Success;
}

//-------------------------------------------------------------------------------
// �������
//-------------------------------------------------------------------------------
DWORD Guild::LeaveGuild(Role* pRole)
{
	ASSERT(P_VALID(pRole));

	// ��ȡ���ɳ�Ա����
	tagGuildMember *pMember = GetMember(pRole->GetID());
	if(!P_VALID(pMember))
	{
		return GT_INVALID;
	}

	// �ж�Ȩ��
	if(!GetGuildPower(pMember->eGuildPos).bLeaveGuild)
	{
		return E_Guild_CanNot_Leave;
	}

	// ���ý�ɫ��������
	pRole->SetGuildID(GT_INVALID);
	
	// �ӹ�������ɾ��
	RemoveMember(pRole->GetID(), pMember->eGuildPos);

	return E_Success;
}

//-------------------------------------------------------------------------------
// �޸İ�����ּ
//-------------------------------------------------------------------------------
DWORD Guild::ModifyTenet(DWORD dwRoleID, LPCTSTR strNewTenet, INT32 nNumTCHAR)
{
	// ��ȡ���ɳ�Ա����
	tagGuildMember *pMember = GetMember(dwRoleID);
	if(!P_VALID(pMember))
	{
		return GT_INVALID;
	}

	// Ȩ��
	if(!GetGuildPower(pMember->eGuildPos).bModifyTenet)
	{
		return E_Guild_Power_NotEnough;
	}

	/**********************************************************/
	/* �ַ������
	/**********************************************************/
	tstring strTmp(strNewTenet, nNumTCHAR);

	// .1��ԭ�����Ƿ���ͬ
	if(m_Att.strTenet == strTmp)
	{
		// ����ǰ����ͬ�����ô����Ҳ��÷������ͻ���
		return E_Return_NotNeedFeed;
	}

	// .2���ȼ��
	if(g_attRes.GetVariableLen().nGuildTenet < nNumTCHAR)
	{
		return E_Filter_Text_TooLong;
	}

	// .3����
	m_Att.strTenet = strTmp;

	// �����ݿ������������Ϣ
	INT32 nMsgSz = sizeof(tagNDBC_ChangeGuildTenet) + nNumTCHAR * sizeof(TCHAR);
	
	MCREATE_MSG(pSend, nMsgSz, NDBC_ChangeGuildTenet);
	pSend->dwGuildID = m_Att.dwID;
	memcpy(pSend->szNewTenet, strNewTenet, nNumTCHAR * sizeof(TCHAR));
	pSend->szNewTenet[nNumTCHAR] = _T('\0');

	g_dbSession.Send(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return E_Success;
}

//-------------------------------------------------------------------------------
// �޸İ��ɻձ�//?? -- ��һ�׶ι���
//-------------------------------------------------------------------------------
DWORD Guild::ModifySgin(DWORD dwRoleID, LPCTSTR strNewURL, INT32 nNumTCHAR)
{
	return GT_INVALID;
}

//-------------------------------------------------------------------------------
// �޸İ����Ź�ָ��
//-------------------------------------------------------------------------------
DWORD Guild::ModifyGroupPurchase(BOOL bInc)
{
	INT32 nGroupPurchase = m_Att.nGroupPurchase;

	if (bInc)
	{
		if(m_Att.nGroupPurchase < MAX_GUILD_GROUP_PURCHASE)
			m_Att.nGroupPurchase++;
		else
			m_Att.nGroupPurchase = MAX_GUILD_GROUP_PURCHASE;
	}
	else
	{
		if(m_Att.nGroupPurchase > 0)
			m_Att.nGroupPurchase--;
		else
			m_Att.nGroupPurchase = 0;
	}

	if (nGroupPurchase != m_Att.nGroupPurchase)
	{
		SaveGuildAtt2DB();
	}
	return E_Success;
}

//-------------------------------------------------------------------------------
// ��ȡָ����Ա��չ��Ϣ
//-------------------------------------------------------------------------------
DWORD Guild::SendSpecMemberEx2Client(Role *p2Role, DWORD dwSpecRoleID)
{
	ASSERT(P_VALID(p2Role));

	tagGuildMember *pSpecMember = GetMember(dwSpecRoleID);
	if(!P_VALID(pSpecMember))
	{
		// ��������²���ִ�е��ô� -- �޸Ŀͻ�����Ϣ��
		return GT_INVALID;
	}

	tagNS_GuildGetMemberEx send;
	send.dwRoleID						= dwSpecRoleID;
	send.sGuildMemInfoEx.nExploit		= pSpecMember->nExploit;
	send.sGuildMemInfoEx.nTotalContrib	= pSpecMember->nTotalContribution;
	send.sGuildMemInfoEx.nKnowledge		= GT_INVALID;

	Role *pSpecRole = g_roleMgr.GetRolePtrByID(dwSpecRoleID);
	if(P_VALID(pSpecRole))
	{
		send.sGuildMemInfoEx.nKnowledge = pSpecRole->GetAttValue(ERA_Knowledge);
	}

	p2Role->SendMessage(&send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ����ָ����Ա����ϸ��Ϣ
//-------------------------------------------------------------------------------
DWORD Guild::SendSpecMember2Client(Role *p2Role, DWORD dwSpecRoleID)
{
	ASSERT(P_VALID(p2Role));

	tagGuildMember *pSpecMember = GetMember(dwSpecRoleID);
	if(!P_VALID(pSpecMember))
	{
		// ��������²���ִ�е��ô� -- �޸Ŀͻ�����Ϣ��
		return GT_INVALID;
	}

	tagNS_GuildRefMember send;
	send.sGuildMemInfo.dwRoleID			= dwSpecRoleID;
	send.sGuildMemInfo.n8GuildPos		= pSpecMember->eGuildPos;
	send.sGuildMemInfo.nCurContrib		= pSpecMember->nContribution;

	send.sGuildMemInfoEx.nTotalContrib	= pSpecMember->nTotalContribution;
	send.sGuildMemInfoEx.nExploit		= pSpecMember->nExploit;

	Role *pSpecRole = g_roleMgr.GetRolePtrByID(dwSpecRoleID);
	if(P_VALID(pSpecRole))		// ����
	{
		send.sGuildMemInfo.byLevel			= (BYTE)pSpecRole->GetLevel();
		send.sGuildMemInfo.bySex			= pSpecRole->GetSex();
		send.sGuildMemInfo.byClass			= pSpecRole->GetClass();
		send.sGuildMemInfo.dwTimeLastLogout	= GT_INVALID;
		send.sGuildMemInfo.bOnline			= true;

		send.sGuildMemInfoEx.nKnowledge		= pSpecRole->GetAttValue(ERA_Knowledge);
	}
	else	// ����
	{
		tagRoleInfo *pSpecRoleInfo = g_roleMgr.GetRoleInfo(dwSpecRoleID);
		if(!P_VALID(pSpecRoleInfo))
		{
			// �����߼���Ա�����߼�������
			ASSERT(0);
			return GT_INVALID;
		}

		send.sGuildMemInfo.byLevel			= pSpecRoleInfo->byLevel;
		send.sGuildMemInfo.bySex			= pSpecRoleInfo->bySex;
		send.sGuildMemInfo.byClass			= pSpecRoleInfo->byClass;		//?? ��û��ְҵ
		send.sGuildMemInfo.dwTimeLastLogout	= pSpecRoleInfo->dwTimeLastLogout;
		send.sGuildMemInfo.bOnline			= false;

		send.sGuildMemInfoEx.nKnowledge		= GT_INVALID;
	}

	p2Role->SendMessage(&send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��ȡ��������
//-------------------------------------------------------------------------------
DWORD Guild::SendGuildName2Client(PlayerSession *pSession)
{
	ASSERT(P_VALID(pSession));
	
	INT32 nNumTCHAR = GetGuildAtt().strName.size();
	ASSERT(nNumTCHAR > 0);

	INT32 nMsgSz = sizeof(tagNS_GetGuildName) + nNumTCHAR * sizeof(TCHAR);

	MCREATE_MSG(pSend, nMsgSz, NS_GetGuildName);
	pSend->dwGuildID	= GetGuildAtt().dwID;
	_tcscpy(pSend->szGuildName, GetGuildAtt().strName.c_str());
	pSession->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��ȡ������ּ -- û�У��򲻸��ͻ��˷���
//-------------------------------------------------------------------------------
DWORD Guild::SendGuildTenet2Client(PlayerSession *pSession)
{
	ASSERT(P_VALID(pSession));
	
	INT32 nNumTCHAR = GetGuildAtt().strTenet.size();
	if(nNumTCHAR <=0)
	{
		return E_Success;
	}

	INT32 nMsgSz = sizeof(tagNS_GetGuildTenet) + nNumTCHAR * sizeof(TCHAR);

	MCREATE_MSG(pSend, nMsgSz, NS_GetGuildTenet);
	_tcscpy(pSend->szGuildTenet, GetGuildAtt().strTenet.c_str());
	pSession->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��ȡ������ʩ������Ϣ
//-------------------------------------------------------------------------------
DWORD Guild::SendFacilitiesInfo2Client(Role* p2Role)
{
	ASSERT(P_VALID(p2Role));

	tagGuildMember* pMember = m_MemberMgr.GetMember(p2Role->GetID());
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}

	// �ж����Ȩ��
	if (!GetGuildPower(pMember->eGuildPos).bUpgrade)
	{
		return E_Guild_Power_NotEnough;
	}

	// ������ʩ��Ϣ
	tagNS_GetGuildUpgradeInfo send;

	DWORD dwErrorCode = m_GuildUpgrade.GetGuildFacilitiesInfo(send.sFacilitiesInfo);

	if (E_Success == dwErrorCode)
	{
		// ����������Ϣ��С
		p2Role->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-------------------------------------------------------------------------------
// ɾ����ɫʱ�����������
//-------------------------------------------------------------------------------
DWORD Guild::ClearRoleRemove(DWORD dwRoleID)
{
	// ��ȡ���ɳ�Ա����
	tagGuildMember *pMember = GetMember(dwRoleID);
	if(!P_VALID(pMember))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	// �ж�Ȩ��
	if(!GetGuildPower(pMember->eGuildPos).bLeaveGuild)
	{
		return E_Guild_CanNot_Leave;
	}

	// �ӹ�������ɾ��
	RemoveMember(dwRoleID, pMember->eGuildPos);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ����������Ե����ݿ�
//-------------------------------------------------------------------------------
VOID Guild::SaveGuildAtt2DB()
{
	tagNDBC_SaveGuildAtt send;
	send.pGuildBase = (tagGuildBase)m_Att;

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------
// ���ð��ɲֿ����Ȩ��
//-------------------------------------------------------------------------------
DWORD Guild::SetGuildWarePrivilege( Role* pRole, DWORD dwRoleID, BOOL bCanUse )
{
	//�ϲ㱣֤��ҺͰ��ɵĺϷ���
	ASSERT(P_VALID(pRole));

	// ��ҷǷ�
	if (!GT_VALID(dwRoleID))
	{
		return GT_INVALID;
	}

	// �жϲ����ߵ�Ȩ��
	tagGuildMember* pMember = GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		// �쳣,���ڰ����޴���
		ASSERT(pMember);
		return E_Guild_MemberNotIn;
	}

	if (!GetGuildPower(pMember->eGuildPos).bSetWareRights)
	{
		return E_Guild_Power_NotEnough;
	}

	// ��鱻������
	pMember = GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		// �������ⲻ�ڸð�����
		return E_Guild_MemberNotIn;
	}
	else if (GetGuildPower(pMember->eGuildPos).bSetWareRights)
	{
		return E_Guild_Power_NotEnough;
	}

	m_MemberMgr.SetGuildWarePrivilege(dwRoleID, bCanUse);

	return E_Success;
}

//-------------------------------------------------------------------------------
// �������Ա��
//-------------------------------------------------------------------------------
BOOL Guild::IncGuildFund( DWORD dwRoleID, INT32 nFund, DWORD dwCmdID )
{
	if (nFund <= 0)
	{
		return FALSE;
	}

	m_Att.nFund += nFund;
	INT32 nMaxFund = m_GuildUpgrade.GetMaxFund();
	if (m_Att.nFund > nMaxFund)
	{
		nFund = m_Att.nFund - nMaxFund;
		m_Att.nFund = nMaxFund;
	}

	LogGuildFund(dwRoleID, nFund, m_Att.nFund, dwCmdID);

	if (IsInGuildState(EGDSS_Distress))
	{
		UnsetGuildState(EGDSS_Distress);
		if (m_Att.nFund < m_Att.nDailyCost*3)
		{
			SetGuildState(EGDSS_Shortage);
		}
	}
	else if (IsInGuildState(EGDSS_Shortage))
	{
		if (m_Att.nFund >= m_Att.nDailyCost*3)
		{
			UnsetGuildState(EGDSS_Shortage);
		}
	}

	// ���浽���ݿ�
	SaveGuildAtt2DB();

	return TRUE;
}

BOOL Guild::IncGuildMaterial( DWORD dwRoleID, INT32 nMaterial, DWORD dwCmdID )
{
	if (nMaterial <= 0)
	{
		return FALSE;
	}

	m_Att.nMaterial += nMaterial;

	INT32 nMaxMaterial = m_GuildUpgrade.GetMaxMaterial();
	if (m_Att.nMaterial > nMaxMaterial)
	{
		nMaterial = m_Att.nMaterial - nMaxMaterial;
		m_Att.nMaterial = nMaxMaterial;
	}
	LogGuildMaterial(dwRoleID, nMaterial, m_Att.nMaterial, dwCmdID);

	// ���浽���ݿ�
	SaveGuildAtt2DB();

	// �ʲ����ڲ��漰����״̬״̬

	return TRUE;
}

BOOL Guild::IncGuildReputation( DWORD dwRoleID, INT32 nReputation, DWORD dwCmdID )
{
	if (nReputation <= 0)
	{
		return FALSE;
	}

	m_Att.nReputation += nReputation;

	if (m_Att.nReputation > MAX_GUILD_REP)
	{
		nReputation = m_Att.nReputation - MAX_GUILD_REP;
		m_Att.nReputation = MAX_GUILD_REP;
	}
	LogGuildReputation(dwRoleID, nReputation, m_Att.nReputation, dwCmdID);

	// �����漰�˰��ɵ����Σ�����g_GuildMgr�ӿ�


	// ���浽���ݿ�
	SaveGuildAtt2DB();

	return TRUE;
}

BOOL Guild::IncGuildPeace( DWORD dwRoleID, INT16 nPeace, DWORD dwCmdID )
{
	if (nPeace <= 0)
	{
		return FALSE;
	}

	m_Att.n16Peace += nPeace;

	if (m_Att.n16Peace > MAX_GUILD_PEACE)
	{
		nPeace = m_Att.n16Peace - MAX_GUILD_PEACE;
		m_Att.n16Peace = MAX_GUILD_PEACE;
	}

	// ���°�������
	m_Att.nDailyCost = (INT32)MGuildDailyCost(m_Att.byLevel, m_Att.n16Peace);

	//??������Log

	if (IsInGuildState(EGDSS_Chaos))
	{
		if (m_Att.n16Peace >= 500)
		{
			UnsetGuildState(EGDSS_Chaos);
		}
	}

	// ���浽���ݿ�
	SaveGuildAtt2DB();

	return TRUE;
}

BOOL Guild::DecGuildFund( DWORD dwRoleID, INT32 nFund, DWORD dwCmdID )
{
	if (nFund <= 0)
	{
		return FALSE;
	}

	m_Att.nFund -= nFund;
	if (m_Att.nFund < 0)
	{
		nFund += m_Att.nFund;
		m_Att.nFund = 0;
	}
	LogGuildFund(dwRoleID, -nFund, m_Att.nFund, dwCmdID);

	if (!IsInGuildStateAny(EGDSS_Distress | EGDSS_Shortage))
	{
		if (m_Att.nFund == 0)
		{
			SetGuildState(EGDSS_Distress);
		}
		else if (m_Att.nFund < m_Att.nDailyCost*3)
		{
			SetGuildState(EGDSS_Shortage);
		}
	}
	else if (IsInGuildState(EGDSS_Shortage))
	{
		if (m_Att.nFund == 0)
		{
			UnsetGuildState(EGDSS_Shortage);
			SetGuildState(EGDSS_Distress);
		}
	}

	// ���浽���ݿ�
	SaveGuildAtt2DB();
	
	return TRUE;
}

BOOL Guild::DecGuildMaterial( DWORD dwRoleID, INT32 nMaterial, DWORD dwCmdID )
{
	if (nMaterial <= 0)
	{
		return FALSE;
	}

	m_Att.nMaterial -= nMaterial;
	if (m_Att.nMaterial < 0)
	{
		nMaterial += m_Att.nMaterial;
		m_Att.nMaterial = 0;
	}
	LogGuildMaterial(dwRoleID, -nMaterial, m_Att.nMaterial, dwCmdID);

	// ���浽���ݿ�
	SaveGuildAtt2DB();

	return TRUE;
}

BOOL Guild::DecGuildReputation( DWORD dwRoleID, INT32 nReputation, DWORD dwCmdID )
{
	if (nReputation <= 0)
	{
		return FALSE;
	}

	m_Att.nReputation -= nReputation;
	if (m_Att.nReputation < 0)
	{
		nReputation += m_Att.nReputation;
		m_Att.nReputation = 0;
	}
	LogGuildReputation(dwRoleID, -nReputation, m_Att.nReputation, dwCmdID);

	// ������а�������


	// ���浽���ݿ�
	SaveGuildAtt2DB();

	return TRUE;
}

BOOL Guild::DecGuildPeace( DWORD dwRoleID, INT16 nPeace, DWORD dwCmdID )
{
	if (nPeace <= 0)
	{
		return FALSE;
	}

	m_Att.n16Peace -= nPeace;
	if (m_Att.n16Peace < 0)
	{
		nPeace += m_Att.n16Peace;
		m_Att.n16Peace = 0;
	}

	// ���°�������
	m_Att.nDailyCost = (INT32)MGuildDailyCost(m_Att.byLevel, m_Att.n16Peace);

	//??������Log

	if (!IsInGuildState(EGDSS_Chaos))
	{
		if (m_Att.n16Peace < 500)
		{
			SetGuildState(EGDSS_Chaos);
		}
	}

	// ���浽���ݿ�
	SaveGuildAtt2DB();

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��¼LOG
//-----------------------------------------------------------------------------
VOID Guild::LogGuildFund( DWORD dwRoleID, INT32 nFund, INT32 nTotalFund, DWORD dwCmdID )
{
	tagNDBC_LogFund send;
	
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if (!P_VALID(pRoleInfo))
	{
		return;
	}

	send.sLogFund.dwAccountID	= pRoleInfo->dwAccountID;
	send.sLogFund.dwCmdID		= dwCmdID;
	send.sLogFund.dwGuildID		= m_Att.dwID;
	send.sLogFund.dwRoleID		= dwRoleID;
	send.sLogFund.nFund			= nFund;
	send.sLogFund.nTotalFund	= nTotalFund;

	g_dbSession.Send(&send, send.dwSize);
}

VOID Guild::LogGuildMaterial( DWORD dwRoleID, INT32 nMaterial, INT32 nTotalMaterial, DWORD dwCmdID )
{
	tagNDBC_LogMaterial send;

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if (!P_VALID(pRoleInfo))
	{
		return;
	}

	send.sLogMaterial.dwAccountID		= pRoleInfo->dwAccountID;
	send.sLogMaterial.dwCmdID			= dwCmdID;
	send.sLogMaterial.dwGuildID			= m_Att.dwID;
	send.sLogMaterial.dwRoleID			= dwRoleID;
	send.sLogMaterial.nMaterial			= nMaterial;
	send.sLogMaterial.nTotalMaterial	= nTotalMaterial;

	g_dbSession.Send(&send, send.dwSize);
}

VOID Guild::LogGuildReputation( DWORD dwRoleID, INT32 nReputation, INT32 nTotalReputation, DWORD dwCmdID )
{
	tagNDBC_LogReputation send;

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if (!P_VALID(pRoleInfo))
	{
		return;
	}

	send.sLogReputation.dwAccountID			= pRoleInfo->dwAccountID;
	send.sLogReputation.dwCmdID				= dwCmdID;
	send.sLogReputation.dwGuildID			= m_Att.dwID;
	send.sLogReputation.dwRoleID			= dwRoleID;
	send.sLogReputation.nReputation			= nReputation;
	send.sLogReputation.nTotalReputation	= nTotalReputation;

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------
// ÿ�����ð�������״̬���۳�����
//-------------------------------------------------------------------------------
VOID Guild::DailyGuildReset()
{
	// �۳�����ÿ���ʽ�Ͱ�������
	DecGuildFund(GT_INVALID, m_Att.nDailyCost, ELCLD_Guild_DailyCost);
	DecGuildPeace(GT_INVALID, MGuildPeaceCost(m_Att.byLevel), ELCLD_Guild_DailyCost);

	// �������̹��ױȽϴ�����
	m_GuildCommerce.ProvideBonus();

	// ���ð���״̬��������
	m_GuildAffair.ResetAffairCounter();

	//??����һ�°��������Ϣ
	SaveGuildAtt2DB();
}

//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
VOID Guild::ReInitbyGuildUpgrade( BYTE byNewLevel /*= 0*/ )
{
	// �жϲ����ĺϷ���
	if (byNewLevel > MAX_GUILD_LEVEL)
	{
		return;
	}

	if (m_Att.byLevel == byNewLevel)
	{
		return;
	}

	// ��¼ԭ�ȼ�
	BYTE byOldLevel = m_Att.byLevel;

	// ���ð����µȼ�
	if (byNewLevel == 0)
	{
		m_Att.byLevel++;
	}
	else
	{
		m_Att.byLevel = byNewLevel;
	}

	// ���¼�������ɵȼ��������
	m_GuildPos.Init(m_Att.byLevel);
	m_GuildWarehouse.ReInit();

	m_Att.nDailyCost = (INT32)MGuildDailyCost(m_Att.byLevel, m_Att.n16Peace);
	ReCalAffairRemainTimes(MGuildAffairTimes(byOldLevel) - m_Att.byAffairRemainTimes);

	// ���浽���ݿ�
	SaveGuildAtt2DB();
}

//-------------------------------------------------------------------------------
// ���¼������ʣ�෢������
//-------------------------------------------------------------------------------
VOID Guild::ReCalAffairRemainTimes( BYTE bySpreadTimes )
{
	BYTE byTimes = m_Att.byAffairRemainTimes;
	m_Att.byAffairRemainTimes	= MGuildAffairTimes(m_Att.byLevel) - bySpreadTimes;

	if (byTimes != m_Att.byAffairRemainTimes)
	{
		SaveGuildAtt2DB();
	}
}

//-------------------------------------------------------------------------------
// �������Ƿ�ռ����ָ������
//-------------------------------------------------------------------------------
BOOL Guild::IsHoldCity( BYTE byCityIndex )
{
	if (byCityIndex == 0)
	{
		return TRUE;
	}

	for (int n=0; n<MAX_GUILD_HOLDCITY; n++)
	{
		if (m_Att.byHoldCity[n] == byCityIndex)
		{
			return TRUE;
		}
	}

	return FALSE;
}