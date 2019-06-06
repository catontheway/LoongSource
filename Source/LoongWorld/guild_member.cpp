//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_member.cpp
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: ���ɳ�Ա
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "guild_member.h"
#include "role.h"
#include "role_mgr.h"
#include "db_session.h"
#include "../WorldDefine/guild_define.h"
#include "../ServerDefine/msg_guild.h"
//-------------------------------------------------------------------------------
// ����&����
//-------------------------------------------------------------------------------
GuildMember::GuildMember()
{
}

GuildMember::~GuildMember()
{
	tagGuildMember *pMember;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		SAFE_DEL(pMember);
	}

	m_mapMember.Clear();
}

//-------------------------------------------------------------------------------
// ��ӳ�Ա
//-------------------------------------------------------------------------------
VOID GuildMember::AddMember(DWORD dwInviteeID, EGuildMemberPos ePos, BOOL bSave2DB/* = TRUE*/)
{
	// ����Ƿ��Ѽ��� -- LoongDB˲��
	ASSERT(!m_mapMember.IsExist(dwInviteeID));
	
	tagGuildMember *pNewMember = new tagGuildMember(dwInviteeID, ePos);
	if(!P_VALID(pNewMember))
	{
		ASSERT(0);
		return;
	}

	m_mapMember.Add(dwInviteeID, pNewMember);

	if(bSave2DB)
	{
		tagNDBC_JoinGuild send;
		send.sGuildMemInfo.dwRoleID		= dwInviteeID;
		send.sGuildMemInfo.dwGuildID	= m_dwGuildID;
		send.sGuildMemInfo.n8GuildPos	= ePos;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------
// �����Ա
//-------------------------------------------------------------------------------
VOID GuildMember::LoadMember(const tagGuildMember& sGuildMember)
{
	if (m_mapMember.IsExist(sGuildMember.dwRoleID))
	{
		return;
	}

	tagGuildMember *pNewMember = new tagGuildMember(sGuildMember);
	if(!P_VALID(pNewMember))
	{
		ASSERT(0);
		return;
	}

	m_mapMember.Add(sGuildMember.dwRoleID, pNewMember);
}

//-------------------------------------------------------------------------------
// ɾ����Ա
//-------------------------------------------------------------------------------
VOID GuildMember::RemoveMember(DWORD dwRoleID, BOOL bSave2DB/* = TRUE*/)
{
	tagGuildMember *pMember = m_mapMember.Peek(dwRoleID);
	if(!P_VALID(pMember))
	{
		return;
	}

	m_mapMember.Erase(dwRoleID);

	if(bSave2DB)
	{
		tagNDBC_LeaveGuild send;
		send.dwRoleID	= dwRoleID;

		g_dbSession.Send(&send, send.dwSize);
	}

	SAFE_DEL(pMember);
}

//-------------------------------------------------------------------------------
// ���ó�Ա�İ���ְλ
//-------------------------------------------------------------------------------
VOID GuildMember::SetGuildPos(tagGuildMember *pMember, EGuildMemberPos ePos, BOOL bSave2DB/* = TRUE*/)
{
	ASSERT(P_VALID(pMember));
	ASSERT(::IsGuildPosValid(ePos));

	pMember->eGuildPos = ePos;

	if(bSave2DB)
	{
		tagNDBC_ChangeGuildPos send;
		send.dwRoleID	= pMember->dwRoleID;
		send.n8NewPos	= ePos;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------
// �����ڹ㲥
//-------------------------------------------------------------------------------
VOID GuildMember::SendGuildMsg(LPVOID pMsg, DWORD dwSize)
{
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if(!P_VALID(pRole))
		{
			continue;
		}

		pRole->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------
// �������а��ɳ�Ա��Ϣ
//-------------------------------------------------------------------------------
VOID GuildMember::SendAllMembers2Client(Role *p2Role)
{
	INT16 n16Num = m_mapMember.Size() - 1;	// ��ȥ�Լ�
	ASSERT(n16Num > 0);

	// �����ڴ�
	INT32 nMsgSz = sizeof(tagNS_GuildGetAllMembers) - sizeof(BYTE) + n16Num * sizeof(tagGuildMemInfo);
	MCREATE_MSG(pSend, nMsgSz, NS_GuildGetAllMembers);

	pSend->n16Num = 0;
	MTRANS_POINTER(pMemberInfo, pSend->byData, tagGuildMemInfo);
	
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		if(pMember->dwRoleID == p2Role->GetID())
		{
			continue;
		}

		pMemberInfo[pSend->n16Num].dwRoleID		= pMember->dwRoleID;
		pMemberInfo[pSend->n16Num].n8GuildPos	= pMember->eGuildPos;
		pMemberInfo[pSend->n16Num].nCurContrib	= pMember->nContribution;
		
		tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(pMember->dwRoleID);
		if(P_VALID(pRoleInfo))
		{
			pMemberInfo[pSend->n16Num].byLevel			= pRoleInfo->byLevel;
			pMemberInfo[pSend->n16Num].bySex			= pRoleInfo->bySex;
			pMemberInfo[pSend->n16Num].byClass			= pRoleInfo->byClass;		//
			pMemberInfo[pSend->n16Num].dwTimeLastLogout	= pRoleInfo->dwTimeLastLogout;
			pMemberInfo[pSend->n16Num].bOnline			= pRoleInfo->bOnline;
		}
		
		// �����߼���Ա�����߼�������
		ASSERT(P_VALID(pRoleInfo));

		++pSend->n16Num;
	}

	if(n16Num != pSend->n16Num)
	{
		ASSERT(n16Num == pSend->n16Num);
		IMSG(_T("\n\n\n\tCaution:\n\tGuild<id:%u> members maybe have problem, please check db!\n\n"), p2Role->GetGuildID());
	}

	p2Role->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------------------------------
// ���ð��ɲֿ����Ȩ��
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::SetGuildWarePrivilege( DWORD dwRoleID, BOOL bCanUse, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	if (bSave2DB && (pMember->bUseGuildWare != bCanUse))
	{
		pMember->bUseGuildWare = bCanUse;

		// ֪ͨ���ݿ�
		tagNDBC_GuildWarePri send;
		send.dwRoleID	= dwRoleID;
		send.bEnable	= bCanUse;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ���ɹ���
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::IncMemberContribution( DWORD dwRoleID, INT32 nContribution, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	if (nContribution <= 0)
		return;

	// ȡ�ó�Ա����
	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	// ���Ա仯
	pMember->nContribution += nContribution;
	if (pMember->nContribution > MAX_GUILDMEM_CURCONTRIB)
	{
		pMember->nContribution = MAX_GUILDMEM_CURCONTRIB;
	}
	pMember->nTotalContribution += nContribution;
	if (pMember->nTotalContribution > MAX_GUILDMEM_TOTALCONTRIB)
	{
		pMember->nTotalContribution = MAX_GUILDMEM_TOTALCONTRIB;
	}
	
	// ���浽���ݿ�
	if (bSave2DB)
	{
		tagNDBC_ChangeContrib send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;

		g_dbSession.Send(&send, send.dwSize);
	}

	// ͬ�����ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildContribution send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;
		pRole->SendMessage(&send, send.dwSize);
	}
}

VOID GuildMember::DecMemberContribution( DWORD dwRoleID, INT32 nContribution, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	if (nContribution <= 0)
		return;

	// ȡ�ó�Ա����
	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	// ���Ա仯
	pMember->nContribution -= nContribution;
	if (pMember->nContribution < 0)
	{
		pMember->nContribution = 0;
	}

	// ���浽���ݿ�
	if (bSave2DB)
	{
		tagNDBC_ChangeContrib send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;

		g_dbSession.Send(&send, send.dwSize);
	}

	// ͬ�����ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildContribution send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;
		pRole->SendMessage(&send, send.dwSize);
	}
}

VOID GuildMember::SetMemberExploit(DWORD dwRoleID, INT32 nExploit, BOOL bSave2DB /* = TRUE */)
{
	if (!GT_VALID(dwRoleID))
		return;

	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;
	
	pMember->nExploit += nExploit;
		
	if (pMember->nExploit > MAX_GUILDMEM_EXPLOIT)
		pMember->nExploit = MAX_GUILDMEM_EXPLOIT;
	if (pMember->nExploit < 0)
		pMember->nExploit = 0;

	if (bSave2DB)
	{
		// ֪ͨ���ݿ�
		tagNDBC_ChangeExploit send;
		send.dwRoleID	= dwRoleID;
		send.nExploit	= nExploit;

		g_dbSession.Send(&send, send.dwSize);
	}

	// ͬ�����ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildExploit send;
		send.dwRoleID			= dwRoleID;
		send.nExploit		= pMember->nExploit;
		pRole->SendMessage(&send, send.dwSize);
	}
}