#include "stdafx.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "channel_mgr.h"
#include "chat_mgr.h"
#include "role.h"
#include "ps_bomb.h"
#include "role_mgr.h"
#include "map.h"
#include "guild.h"
#include "guild_mgr.h"
#include "title_mgr.h"
#include "chat_logger.h"

ChannelMgr g_channelMgr;
// struct SendFilter
// {
// 	SendFilter(ESendChatChannel eChannel)
// 		:m_eChannel(eChannel){}
// 	BOOL operator()(Unit* pUnit)
// 	{
// 		Role* pRole = dynamic_cast<Role*>(pUnit);
// 		if (!P_VALID(pRole))
// 		{
// 			return FALSE;
// 		}
// 		
// 		return TRUE;//pRole->GetChatData()->IsFit(m_eChannel);
// 	}
// 
// private:
// 	ESendChatChannel m_eChannel;
// };

DWORD ChannelMgr::SendMsgCmd(MsgCmd* pMsgCmd)
{
	MTRANS_ELSE_RET(pSender, pMsgCmd->GetSender(), Role, E_SystemError);
	MTRANS_ELSE_RET(pMap, pSender->GetMap(), Map, E_SystemError);
	MTRANS_ELSE_RET(pSend, pMsgCmd->GetSendMsg(), tagNetCmd, E_SystemError);
	MTRANS_ELSE_RET(pRecv, pMsgCmd->GetRecvMsg(), tagNetCmd, E_SystemError);
	ESendChatChannel eChannel = pMsgCmd->GetChannelType();

	//��������
	if ( pMsgCmd->GetContentSize() > GetContentSizeLimit(pSend, eChannel) )
		return E_RoleChat_TooLength;


	// ��������Ƶ����ͬ�ֱ���
	switch( eChannel )
	{
		// ��ǰƵ��
	case ESCC_Current:
		{
			// ���ͻ��˷�����Ϣ
			tagNC_RoleChat* pRolcChat = static_cast<tagNC_RoleChat*>(pRecv);
			if(!g_pSGuarder.OnMsg(pRolcChat->szMsg, 0))
			{
				pMap->ForEachUnitInBigVisTile(pSender, SendAction(eChannel, pSend));
				//pMap->SendBigVisTileMsg(pSender, (LPVOID)pSend, pSend->dwSize);
			}
		}
		break;
		// ˽��
	case ESCC_Pvt:
		{
			//�ж�����Ƿ����
			MTRANS_ELSE_RET(pDestRoleInfo, g_roleMgr.GetRoleInfo(pMsgCmd->GetReceiverID()), tagRoleInfo, E_RoleChat_Pvt_NoRoleName);
			//�ж�����Ƿ�����
			MTRANS_ELSE_RET(pDestRole, g_roleMgr.GetRolePtrByID(pMsgCmd->GetReceiverID()), Role, E_RoleChat_Pvt_RemoteRoleLeave);
			pDestRole->SendMessage(pSend, pSend->dwSize);
			if (pSender != pDestRole)
			{
				pSender->SendMessage(pSend, pSend->dwSize);	
			}			
		}
		break;
		// ����
	case ESCC_World:
		{
			// �ж�ʱ����
			if (!pSender->TalkToWorld())
				return E_RoleChat_World_Frequently;

			// ��ҵȼ��ж�
			if (pSender->GetLevel() < WORLD_CHANNEL_ROLE_LVL)
				E_RoleChat_World_RoleNo10;

			//������Ƶ��������Ҫ�۳� 100 ��
			//�õ�������ϵ��� ����
			INT64 currSilver = pSender->GetCurMgr().GetBagSilver();
			if ( currSilver < WORLD_CHANNEL_DEC_SLIVER )
			{
				//������
				return E_RoleChat_World_NoMoney;
			}
			else
			{
				//�۳���ұ����� 100��
				pSender->GetCurMgr().DecBagSilver(WORLD_CHANNEL_DEC_SLIVER, ELCLD_CHAT_WORLD_CHANNEL);
			}
			// ���ͻ��˷�����Ϣ
			g_roleMgr.ForEachRoleInWorld(SendAction(eChannel, pSend));
			//g_roleMgr.SendWorldMsg((LPVOID)pSend, pSend->dwSize);
		}
		break;
		//����
	case ESCC_Affiche:
		{
			//�жϷ������ǲ���GMȨ��
			if (pSender->GetSession()->IsPrivilegeEnough(1) == FALSE)
				return E_RoleChat_Not_GM;

			g_roleMgr.ForEachRoleInWorld(SendAction(eChannel, pSend));
			//g_roleMgr.SendWorldMsg((LPVOID)pSend, pSend->dwSize);
		}
		break;
		// ����
	case ESCC_Troop:
		{
			//������Ҷ���ID�ҵ����Ӧ�Ķ���,���������г�Ա������Ϣ
			DWORD dwTeamID = pSender->GetTeamID();
			const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
			if (P_VALID(pTeam))
			{
				pTeam->ForEachRoleInTeam(SendAction(eChannel, pSend));
			}
			//g_groupMgr.SendTeamMessage(dwTeamID, (LPVOID)pSend, pSend->dwSize);
		}
		break;
		// ����
	case ESCC_CastTalk:
		{
			TList<tagItem*> listItems;
			INT nNum = pSender->GetItemMgr().GetBagSameItemList(listItems, TYPEID_IM_CASTTALK);

			//�޵���
			if (listItems.Empty())
				return E_RoleChat_CastTalk_NoIMItem;

			// ʹ����Ʒ
			TList<tagItem*>::TListIterator itr = listItems.Begin();
			pSender->GetItemMgr().ItemUsedFromBag((*itr)->GetKey(), (INT16)1, (DWORD)ELCLD_CAST_TALK_IM);

			// �ƺ��¼�����
			pSender->GetTitleMgr()->SigEvent(ETE_SHOUT, GT_INVALID, GT_INVALID);

			// ���ͻ��˷�����Ϣ
			g_roleMgr.ForEachRoleInWorld(SendAction(eChannel, pSend));
			//g_roleMgr.SendWorldMsg((LPVOID)pSend, pSend->dwSize);
		}
		break;
		// ����
	case ESCC_Guild:
		{
			// ���ͻ��˷�����Ϣ
			MTRANS_ELSE_RET(pGuild, g_guildMgr.GetGuild(pSender->GetGuildID()), Guild, E_RoleChat_Guild_NoJoin);
			pGuild->ForEachRoleInGuild(SendAction(eChannel, pSend));
			//pGuild->SendGuildMsg(pSend, pSend->dwSize);
		}
		break;
	}

	TheChatLogger.Log(*pMsgCmd);
	return E_Success;
}

DWORD ChannelMgr::GetContentSizeLimit( tagNetCmd* pNetCmd, BYTE byChannelType )
{
	static const tagNS_RoleChat			msgChat;
	static const tagNS_RoleShowEquip	msgShowEquip;
	static const tagNS_RoleShowItem		msgShowItem;

	DWORD dwSizeLimit = 1024;
	if (msgChat.dwID == pNetCmd->dwID)
	{
		switch(ESendChatChannel(byChannelType))
		{
		case ESCC_CastTalk:
			dwSizeLimit = WORLD_CHANNEL_MSG_LEN;
			break;
		case ESCC_World:
			dwSizeLimit = MAX_CHAT_LEN;
			break;
		case ESCC_Pvt:
		case ESCC_Guild:
		case ESCC_Troop:
		case ESCC_Current:
			dwSizeLimit = MAX_CHAT_LEN;
			break;
		case ESCC_Leave:
		case ESCC_Combat:
		case ESCC_Affiche:
		case ESCC_GM:
		case ESCC_System:
		default:
			break;
		}
	}
	else if (msgShowEquip.dwID == pNetCmd->dwID)
	{
		dwSizeLimit = sizeof(tagEquip);
	}
	else if (msgShowItem.dwID == pNetCmd->dwID)
	{
		dwSizeLimit = sizeof(tagItem);
	}

	return dwSizeLimit;
}

