//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_handler.h
// author: 
// actor:
// data: 2009-04-17
// last:
// brief: ������Ϣ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "guild.h"
#include "guild_mgr.h"
#include "guild_CofC.h"
#include "../WorldDefine/msg_guild.h"
//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCreateGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildCreate);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = GT_INVALID;

	// NPC���
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Guild);
	if(E_Success == dwErrorCode)
	{
		INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_GuildCreate, szGuildName)) / sizeof(TCHAR);
		dwErrorCode = g_guildMgr.CreateGuild(pRole, pRecv->szGuildName, nNameTCHAR);
	}

	// ��ͻ��˷���
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ɢ����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDismissGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildDismiss);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = GT_INVALID;

	// NPC���
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Guild);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = g_guildMgr.DismissGuild(pRole, pRole->GetGuildID());
	}

	// ��ͻ��˷���
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleJoinGuildReq(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildJoinReq);
	
	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �Ƿ���Լ�����
	if(pRole->GetID() == pRecv->dwDstRoleID)
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����
	Role *pInvitee = NULL;
	DWORD dwErrorCode = pGuild->CanInviteJoin(pRole->GetID(), pRecv->dwDstRoleID, &pInvitee);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		ASSERT(P_VALID(pInvitee));

		tagNS_GuildJoinReq send;
		send.dwSrcRoleID	= pRole->GetID();
		send.dwGuildID		= pRole->GetGuildID();
		pInvitee->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��������ҷ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleJoinGuildReqRes(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildJoinReqRes);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRecv->dwDstGuildID);
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	if(pRecv->dwErrorCode != E_Success)
	{
		// �������������Ϣ
		pGuild->EraseInviteJoin(pRole->GetID());	

		// ����������Ƿ�������
		Role *pInviter = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
		if(P_VALID(pInviter))
		{
			// �������߷���Ϣ
			tagNS_GuildJoinReqRes	send;
			send.dwErrorCode = E_Guild_Join_BeRefused;
			send.dwInviterID = pRecv->dwDstRoleID;
			send.dwInviteeID = pRole->GetID();
			pInviter->SendMessage(&send, send.dwSize);
		}

		return E_Success;
	}

	// ����
	DWORD dwErrorCode = pGuild->InviteJoin(pRecv->dwDstRoleID, pRole->GetID());
	if(dwErrorCode != E_Success)
	{		
		// ��˫������Ϣ
		tagNS_GuildJoinReqRes	send;
		send.dwErrorCode = dwErrorCode;
		send.dwInviterID = pRecv->dwDstRoleID;
		send.dwInviteeID = pRole->GetID();
		SendMessage(&send, send.dwSize);

		// ����������Ƿ�������
		Role *pInviter = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
		if(P_VALID(pInviter))
		{
			pInviter->SendMessage(&send, send.dwSize);
		}
	}
	else
	{
		// ���ɹ㲥
		tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());
		if (P_VALID(pRoleInfo))
		{
			INT32 nRoleNameCnt = _tcsclen(pRoleInfo->szRoleName);
			INT32 nMsgSz = sizeof(tagNS_GuildJoinBroad) + nRoleNameCnt * sizeof(TCHAR);

			MCREATE_MSG(pSend, nMsgSz, NS_GuildJoinBroad);
			pSend->dwRoleID		= pRole->GetID();
			pSend->dwRoleNameID	= pRole->GetNameID();

			memcpy(pSend->szRoleName, pRoleInfo->szRoleName, (nRoleNameCnt + 1) * sizeof(TCHAR));

			pGuild->SendGuildMsg(pSend, pSend->dwSize);
			MDEL_MSG(pSend);
		}

		// ͬ������Χ���
		tagGuildMember* pMember = pGuild->GetMember(pRole->GetID());
		Map*			pMap	= pRole->GetMap();
		if (P_VALID(pMember) && P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send;
			send.dwGuildID		= pRecv->dwDstGuildID;
			send.dwRoleID		= pRole->GetID();
			send.n8GuildPos		= pMember->eGuildPos;

			pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �뿪����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleLeaveGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildLeave);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����
	DWORD dwErrorCode = pGuild->LeaveGuild(pRole);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// ���ɹ㲥
		tagNS_GuildLeaveBroad send;
		send.dwSrcRoleID = pRole->GetID();
		pGuild->SendGuildMsg(&send, send.dwSize);

		// �뿪��
		SendMessage(&send, send.dwSize);

		// ͬ������Χ���
		Map* pMap	= pRole->GetMap();
		if (P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= GT_INVALID;
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= EGMP_Null;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �Ӱ������߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleKickFromGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildKick);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �Ƿ���Լ�����
	if(pRole->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����
	DWORD dwErrorCode = pGuild->KickMember(pRole->GetID(), pRecv->dwRoleID);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildKickBroad send;
		send.dwDstRoleID = pRecv->dwRoleID;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// ��鱻�����Ƿ�����
		Role *pKicked = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if(P_VALID(pKicked))
		{
			pKicked->SendMessage(&send, send.dwSize);

			// ͬ������Χ���
			Map* pMap	= pKicked->GetMap();
			if (P_VALID(pMap))
			{
				tagNS_RemoteRoleGuildInfoChange send_broad;
				send_broad.dwGuildID	= GT_INVALID;
				send_broad.dwRoleID		= pKicked->GetID();
				send_broad.n8GuildPos	= EGMP_Null;

				pMap->SendBigVisTileMsg(pKicked, &send_broad, send_broad.dwSize);
			}
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �ƽ�����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleTurnoverGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildTurnover);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �Ƿ���Լ�����
	if(pRole->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����
	DWORD dwErrorCode = pGuild->TurnoverLeader(pRole->GetID(), pRecv->dwRoleID);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildTurnoverBroad send;
		send.dwSrcRoleID	= pRole->GetID();
		send.dwDstRoleID	= pRecv->dwRoleID;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// ͬ������Χ���(ԭ�������Ȱ���)
		tagGuildMember* pMember = pGuild->GetMember(pRole->GetID());
		Map*			pMap	= pRole->GetMap();
		if (P_VALID(pMember) && P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pRole->GetGuildID();
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= pMember->eGuildPos;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}

		// �ж��Ȱ����Ƿ�����
		Role* pNewLeader = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if (P_VALID(pNewLeader))
		{
			pMember = pGuild->GetMember(pNewLeader->GetID());
			pMap	= pNewLeader->GetMap();
			if (P_VALID(pMember) && P_VALID(pMap))
			{
				tagNS_RemoteRoleGuildInfoChange send_broad;
				send_broad.dwGuildID	= pNewLeader->GetGuildID();
				send_broad.dwRoleID		= pNewLeader->GetID();
				send_broad.n8GuildPos	= pMember->eGuildPos;

				pMap->SendBigVisTileMsg(pNewLeader, &send_broad, send_broad.dwSize);
			}
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ְ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDemissFromGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildDemiss);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����
	INT8 n8OldGuildPos = EGMP_BangZhong;
	DWORD dwErrorCode = pGuild->DemissPostion(pRole->GetID(), n8OldGuildPos);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildDemissBroad send;
		send.dwRoleID		= pRole->GetID();
		send.n8OldGuildPos	= n8OldGuildPos;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// ͬ������Χ���
		Map* pMap	= pRole->GetMap();
		if (P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pRole->GetGuildID();
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= EGMP_BangZhong;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����ְλ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAppointForGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildAppoint);

	// ��ȡ��ɫ
	Role *pAppointor = GetRole();
	if(!P_VALID(pAppointor))
	{
		return GT_INVALID;
	}

	// �Ƿ���Լ�����
	if(pAppointor->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pAppointor->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����
	DWORD dwErrorCode = pGuild->Appoint(pAppointor->GetID(), pRecv->dwRoleID, pRecv->ePos);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildAppointBroad send;
		send.dwSrcRoleID	= pAppointor->GetID();
		send.dwDstRoleID	= pRecv->dwRoleID;
		send.ePos			= pRecv->ePos;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// ͬ������Χ���
		Role* pDstRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if( P_VALID(pDstRole) && P_VALID(pDstRole->GetMap()) )
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pDstRole->GetGuildID();
			send_broad.dwRoleID		= pRecv->dwRoleID;
			send_broad.n8GuildPos	= pRecv->ePos;

			pDstRole->GetMap()->SendBigVisTileMsg(pDstRole, &send_broad, send_broad.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �޸İ�����ּ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleChangeGuildTenet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildTenetChange);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����
	INT32 nTenetCnt = (pRecv->dwSize - FIELD_OFFSET(tagNC_GuildTenetChange, szTenet)) / sizeof(TCHAR);
	DWORD dwErrorCode = pGuild->ModifyTenet(pRole->GetID(), pRecv->szTenet, nTenetCnt);
	if(dwErrorCode != E_Success)
	{
		// �ж��Ƿ���Ҫ����
		if (dwErrorCode != E_Return_NotNeedFeed)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
	}
	else
	{
		INT32 nMsgSz = sizeof(tagNS_GuildTenetChangeBroad) + nTenetCnt * sizeof(TCHAR);
		MCREATE_MSG(pSend, nMsgSz, NS_GuildTenetChangeBroad);
		pSend->dwRoleID	= pRole->GetID();
		memcpy(pSend->szTenet, pRecv->szTenet, (nTenetCnt + 1) * sizeof(TCHAR));
		pSend->szTenet[nTenetCnt] = _T('\0');
		pGuild->SendGuildMsg(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}


//********************** ��ȡ���������Ϣ��Ϣ���� **********************//

//-----------------------------------------------------------------------------
// ��ȡ�������г�Ա
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildMembers(tagNetCmd* pCmd)
{
	//MGET_MSG(p, pCmd, NC_GuildGetAllMembers);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// ִ�е��˴���˵�����ɹ��������� -- ������û�г�Աʱ�ſ���ɾ������
		ASSERT(0);
		return GT_INVALID;
	}

	pGuild->SendAllMembers2Client(pRole);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ����ָ����Ա��չ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildMemberEx(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_GuildGetMemberEx);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// ִ�е��˴���˵�����ɹ��������� -- ������û�г�Աʱ�ſ���ɾ������
		ASSERT(0);
		return GT_INVALID;
	}

	return pGuild->SendSpecMemberEx2Client(pRole, p->dwRoleID);
}

//-----------------------------------------------------------------------------
// ˢ�°���ָ����Ա��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRefreshGuildMember(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_GuildRefMember);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// ִ�е��˴���˵�����ɹ��������� -- ������û�г�Աʱ�ſ���ɾ������
		ASSERT(0);
		return GT_INVALID;
	}

	return pGuild->SendSpecMember2Client(pRole, p->dwRoleID);
}

//-----------------------------------------------------------------------------
// ��ȡ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildName);

	Guild *pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	return pGuild->SendGuildName2Client(this);
}

//-----------------------------------------------------------------------------
// ��ȡ������ּ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildTenet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildTenet);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	return pGuild->SendGuildTenet2Client(this);
}

//-----------------------------------------------------------------------------
// ��ȡ���ɲֿ�����Ʒ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildWareItems(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildWare);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success != dwErrorCode)
	{
		return GT_INVALID;
	}

	// �����㹻��Ļ�����
	MCREATE_MSG(pSend, TEMP_GUILD_BUFF_SIZE, NS_GetGuildWare);
	pSend->dwLastUpdateTime = pRecv->dwLastUpdateTime;

	INT32 nItemSize = 0;
	pSend->dwErrorCode = pGuild->GetGuildWarehouse().GetGuildWareInfo(pSend->byData, pSend->nItemNum, pSend->dwLastUpdateTime, pSend->nSzGuildWare, nItemSize);

	// ����������Ϣ��С
	pSend->dwSize = sizeof(tagNS_GetGuildWare) + nItemSize;

	// ���͸��ͻ���
	SendMessage(pSend, pSend->dwSize);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ���ɲֿ����Ȩ���б�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildWarePriList(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildWarePriList);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->GetGuildWarehouse().SendGuildWarePriList2Client(pRole);
	}

	if (GT_VALID(dwErrorCode) && (E_Success != dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return GT_INVALID;
}

//-----------------------------------------------------------------------------
// ���ָ����Ա�İ��ɲֿ����Ȩ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGuildWarePrivilege(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildWarePri);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->SetGuildWarePrivilege(pRole, pRecv->dwRoleID, pRecv->bCanUse);
	}
	
	if (!GT_VALID(dwErrorCode))
	{
		return GT_INVALID;
	}

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// ֪ͨ�����߸����б�
		tagNS_GuildWarePri send;
		send.dwRoleID	= pRecv->dwRoleID;
		send.bCanUse	= pRecv->bCanUse;
		SendMessage(&send, send.dwSize);

		// ֪ͨ��������Ȩ�ޱ��
		Role* pMember = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if (P_VALID(pMember))
		{
			pMember->SendMessage(&send, send.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ������ʩ������Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildFacilitiesInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildUpgradeInfo);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildMgr);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->SendFacilitiesInfo2Client(pRole);
	}

	if (GT_VALID(dwErrorCode) && (E_Success != dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����Ͻɰ�����ʩ����������Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleHandInItems(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_HandInItems);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildMgr);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->GetGuildFacilities().HandInItems(pRole, pRecv->eType);
	}

	if (GT_VALID(dwErrorCode))
	{
		if (E_Success != dwErrorCode)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
		else
		{
			// ���ر仯�����ʩ��Ϣ
			tagNS_UpdateFacilitiesInfo send;
			pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&send.sFacilitiesInfo, pRecv->eType);
			pRole->SendMessage(&send, send.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ������������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSpreadGuildAffair(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SpreadGuildAffair);
	if (!GT_VALID(pRecv->dwBuffID))
	{
		return GT_INVALID;
	}

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildAffair().SpreadGuildAffair(pRole, pRecv->dwBuffID);

	if (GT_VALID(dwErrorCode) && (dwErrorCode != E_Success))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ȡ�ð��ɼ�����Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildSkillInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	MCREATE_MSG(pSend, TEMP_GUILD_BUFF_SIZE, NS_GetGuildSkillInfo);
	DWORD dwErrorCode = pGuild->GetGuildSkill().GetGuildSkillInfo(pSend->dwCurSkillID, pSend->n16Progress,
		pSend->nSkillNum, pSend->dwSkillInfo);

	// ���¼�����Ϣ��С
	if (dwErrorCode == E_Success)
	{
		pSend->dwSize = sizeof(tagNS_GetGuildSkillInfo) + (pSend->nSkillNum - 1) * sizeof(DWORD);
		SendMessage(pSend, pSend->dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �Ͻɰ��ɼ��ܵ伮
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpgradeGuildSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildSkillUpgrade);
	if (!GT_VALID(pRecv->n64ItemSerial))
	{
		return GT_INVALID;
	}

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_GuildSkillUpgrade send;
	DWORD dwErrorCode = pGuild->GetGuildSkill().SkillUpgradeByHandIn(pRole, pRecv->n64ItemSerial, send.dwSkillID, send.n16Progress);
	if (E_Success == dwErrorCode)
	{
		SendMessage(&send, send.dwSize);
	}
	else if (GT_VALID(dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ѧϰ���ɼ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleLearnGuildSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_LearnGuildSkill);
	if (!GT_VALID(pRecv->dwSkillID))
	{
		return GT_INVALID;
	}

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_LearnGuildSkill send;
	BOOL bLearnError = FALSE;
	INT nLevel = 0;
	DWORD dwErrorCode = pGuild->GetGuildSkill().LearnGuildSkill(pRole, pRecv->dwSkillID, bLearnError, nLevel);

	if (GT_VALID(dwErrorCode))
	{
		if (bLearnError)
		{
			send.dwErrorCode	= dwErrorCode;
			send.dwSkillID		= pRecv->dwSkillID * 100 + nLevel;
			SendMessage(&send, send.dwSize);
		}
		else if (dwErrorCode != E_Success)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
	}
	
	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ���õ�ǰ�о����ɼ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSetResearchSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetResearchSkill);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_SetResearchSkill send;
	INT nLevel = 0;
	DWORD dwErrorCode = pGuild->GetGuildSkill().SetCurUpgradeGuildSkill(pRole->GetID(), pRecv->dwSkillID, nLevel, send.n16Progress);
	if (!GT_VALID(dwErrorCode))
	{
		return GT_INVALID;
	}

	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		if (GT_VALID(pRecv->dwSkillID))
		{
			send.dwSkillID	= pRecv->dwSkillID * 100 + nLevel;
		}
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ͬ�����ɻ�����Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSyncGuildInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_SyncGuildInfo send;
	send.sGuildInfo = (tagGuildBase)pGuild->GetGuildAtt();

	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ�����̻���Ϣ	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCofCInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCofCInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	GuildCofC* pCofC = pMap->GetCofC(pRecv->dwNPCID);
	if (!P_VALID(pCofC))
	{
		// ����Ҫ֪ͨ�ͻ���
		return GT_INVALID;
	}

	return pCofC->SendCommerceGoodsInfo2Role(pRole);
}

//-----------------------------------------------------------------------------
// �ر��̻����		-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCloseCofC(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_CloseCofC);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	GuildCofC* pCofC = pMap->GetCofC(pRecv->dwNPCID);
	if (!P_VALID(pCofC))
	{
		return GT_INVALID;
	}

	// �Ƴ��۲����б�
	pCofC->RemoveObserver(pRole->GetID());

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����̻��̻�		-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleBuyCofCGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BuyCofCGoods);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().BuyGoods(pRole, pRecv->dwNPCID, pRecv->dwGoodID, pRecv->byBuyNum);
	if ((E_Success != dwErrorCode) && GT_VALID(dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����̻����̻�	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSellCofCGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SellCofCGoods);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().SellGoods(pRole, pRecv->dwNPCID, pRecv->dwGoodID, pRecv->bySellNum);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ����������Ϣ	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCommodityInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCommodityInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT nMsgSize = sizeof(tagNS_GetCommodityInfo) + sizeof(tagCommerceGoodInfo) * (MAX_COMMODITY_CAPACITY - 1);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCommodityInfo);
	// ��ȡ�̻���Ϣ
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommodityGoodInfo(pRole, pSend->sGoodInfo, pSend->nGoodNum, pSend->nCurTael, pSend->nLevel);

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// ������Ϣ��С
		pSend->dwSize = sizeof(tagNS_GetCommodityInfo) + sizeof(tagCommerceGoodInfo) * (pSend->nGoodNum - 1);
		SendMessage(pSend, pSend->dwSize);
	}

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ���̳�ʼ��Ϣ	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetTaelInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_GetTaelInfo send;
	tagTaelInfo sTaelInfo;
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommerceBeginningInfo(pRole->GetID(), send.nLevel, sTaelInfo);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ������������	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCommerceRank(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT nMsgSize = sizeof(tagNS_GetCommerceRank) + sizeof(tagCommerceRank) * (MAX_COMMERCE_RANK_INFO - 1);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCommerceRank);
	// ��ȡ������Ϣ
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommerceRankInfo(pSend->sRankInfo, pSend->nRankNum, pSend->bCommend);

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// ������Ϣ��С
		pSend->dwSize = sizeof(tagNS_GetCommerceRank) + sizeof(tagCommerceRank) * (pSend->nRankNum - 1);
		SendMessage(pSend, pSend->dwSize);
	}

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAcceptCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().AcceptCommerce(pRole);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		INT nLevel;
		tagTaelInfo sTaelInfo;
		tagNS_AcceptCommerce send;
		dwErrorCode = pGuild->GetGuildCommerce().GetCommerceBeginningInfo(pRole->GetID(), nLevel, sTaelInfo);
		if (dwErrorCode == E_Success)
		{
			send.nBeginningTael = sTaelInfo.nBeginningTael;
			SendMessage(&send, send.dwSize);
		}
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// �Ͻ�����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCompleteCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT32 nFund = 0;
	DWORD dwErrorCode = pGuild->GetGuildCommerce().CompleteCommerce(pRole, nFund);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_CompleteCommerce send;
		send.dwRoleID	= pRole->GetID();
		send.nFund		= nFund;
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAbandonCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().AbandonCommerce(pRole->GetID());
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// �������̼ν�״̬
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSwitchCommendation(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SwitchCommendation);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().SwitchCommendation(pRole->GetID(), pRecv->bSwitchON);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_SwitchCommendation send;
		send.dwRoleID	= pRole->GetID();
		send.bSwitchON	= pRecv->bSwitchON;
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return E_Success;
}