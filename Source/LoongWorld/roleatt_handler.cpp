//-----------------------------------------------------------------------------
//!\file roleatt_handler.cpp
//!\author Aslan
//!
//!\date 2008-07-28
//! last 2008-07-28
//!
//!\brief �ƶ���Ϣ������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/role_info.h"
#include "../WorldDefine/selectrole.h"
#include "../WorldDefine/filter.h"
#include "../ServerDefine/role_data.h"


#include "pet_pocket.h"
#include "pet_soul.h"
#include "player_session.h"
#include "role.h"
#include "creature.h"
#include "role_mgr.h"
#include "map.h"
#include "pet.h"
#include "guild.h"
#include "guild_mgr.h"
#include "title_mgr.h"

//------------------------------------------------------------------------------
// ��ȡ��������
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleGetRoleInitAtt(tagNetCmd* pCmd)
{
	tagNC_GetRoleInitState* pRecv = (tagNC_GetRoleInitState*)pCmd;

	if( pRecv->eType <= ERIT_Null || pRecv->eType >= ERIT_End )
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;


	switch(pRecv->eType)
	{
	case ERIT_Att:
		pRole->SendInitStateAtt();
		break;
	case ERIT_Skill:
		pRole->SendInitStateSkill();
		break;
	case ERIT_Item:
		pRole->SendInitStateItem();
		pRole->SendInitStateSuit();
		pRole->SendInitStateLongHun();
		break;
	case ERIT_CompleteQuest:
		pRole->SendInitStateCompleteQuest();
		break;
	case ERIT_IncompleteQuest:
		pRole->SendInitStateIncompleteQuest();
		break;
	case ERIT_Money:
		pRole->SendInitStateMoney();
		break;
	case ERIT_Reputation:
		pRole->SendInitStateReputation();
		break;
	case ERIT_FrindAndEnemy:
		pRole->SendFriendAndEnemy();
		break;
	case ERIT_Guild:
		pRole->SendInitStateGuild();
		break;
	default:
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ��ȡԶ��������������ԣ��ǲ���Ҫ��Щ����������һֱ�ڷ���ô�죩
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetRemoteUnitAtt(tagNetCmd* pCmd)
{
	tagNC_GetRemoteRoleState* pRecv = (tagNC_GetRemoteRoleState*)pCmd;

	Role* pSelf = GetRole();
	if( !P_VALID(pSelf) || GT_INVALID == pSelf->GetVisTileIndex() ) return GT_INVALID;

	Map* pMap = pSelf->GetMap();
	if( !P_VALID(pMap) ) return GT_INVALID;

	if( pRecv->nRoleNum > 50 ) return GT_INVALID;	// ��෢50��

	// ˳�����ѯ��ͼ�е�ID
	for(INT n = 0; n < pRecv->nRoleNum; n++)
	{
		if( !P_VALID(pRecv->dwRoleID[n]) )
			break;

		// ��������ﻹ������
		if( IS_PLAYER(pRecv->dwRoleID[n]) )
		{
			Role* pRemoteRole = pMap->FindRole(pRecv->dwRoleID[n]);
			if( P_VALID(pRemoteRole) )
			{
				INT nBuffNum = pRemoteRole->GetBuffNum();
				DWORD dwSize = sizeof(tagNS_GetRemoteRoleState) + ((nBuffNum > 0) ? (nBuffNum - 1)*sizeof(tagBuffMsgInfo) : 0);

				// ����Զ����������ͬ����Ϣ
				MCREATE_MSG(pSend, dwSize, NS_GetRemoteRoleState);

				pSend->RoleData.dwID = pRemoteRole->GetID();
				pSend->RoleData.nLevel = pRemoteRole->GetLevel();

				pSend->RoleData.byStallLevel = pRemoteRole->GetStallModeLevel();
				pSend->RoleData.eClassType = pRemoteRole->GetClass();
				pSend->RoleData.eClassTypeEx = pRemoteRole->GetClassEx();

				pSend->RoleData.dwLoverID = pRemoteRole->GetLoverID();
				pSend->RoleData.bHaveWedding = pRemoteRole->IsRoleWedded();

				// λ��
				Vector3 vPos = pRemoteRole->GetCurPos();
				pSend->RoleData.fPos[0] = vPos.x;
				pSend->RoleData.fPos[1] = vPos.y;
				pSend->RoleData.fPos[2] = vPos.z;

				// ����
				Vector3 vFace = pRemoteRole->GetFaceTo();
				pSend->RoleData.fFaceTo[0] = vFace.x;
				pSend->RoleData.fFaceTo[1] = vFace.y;
				pSend->RoleData.fFaceTo[2] = vFace.z;

				// ״̬
				pSend->RoleData.dwState = pRemoteRole->GetState();
				pSend->RoleData.dwRoleState = pRemoteRole->GetRoleState();
				pSend->RoleData.ePKState = pRemoteRole->GetPKState();

				// ����
				pSend->RoleData.dwGuildID = pRemoteRole->GetGuildID();
				pSend->RoleData.n8GuildPos = EGMP_Null;
				if(pRemoteRole->IsInGuild())
				{
					Guild *pGuild = g_guildMgr.GetGuild(pRemoteRole->GetGuildID());
					if(P_VALID(pGuild))
					{
						tagGuildMember *pMember = pGuild->GetMember(pRemoteRole->GetID());
						if(P_VALID(pMember))
						{
							pSend->RoleData.n8GuildPos = pMember->eGuildPos;
						}
					}
				}

				// ��˳�������
				PetSoul* pSoul = pRemoteRole->GetPetPocket()->GetMountPetSoul();
				if (P_VALID(pSoul))
				{
					pSend->RoleData.dwMountPetID = pSoul->GetID();
					pSend->RoleData.dwMountPetTypeID = pSoul->GetProtoID();
				}
				else
				{
					pSend->RoleData.dwMountPetID = GT_INVALID;
					pSend->RoleData.dwMountPetTypeID = GT_INVALID;
				}

				if (pRemoteRole->GetPetPocket()->IsRideAfter())
				{
					pSend->RoleData.dwMountPetID = pRemoteRole->GetPetPocket()->GetHostPetID();
				}

				// ����
				pSend->RoleData.nAtt[ERRA_MaxHP]		= pRemoteRole->GetAttValue(ERA_MaxHP);
				pSend->RoleData.nAtt[ERRA_HP]			= pRemoteRole->GetAttValue(ERA_HP);
				pSend->RoleData.nAtt[ERRA_MaxMP]		= pRemoteRole->GetAttValue(ERA_MaxMP);
				pSend->RoleData.nAtt[ERRA_MP]			= pRemoteRole->GetAttValue(ERA_MP);
				pSend->RoleData.nAtt[ERRA_Rage]			= pRemoteRole->GetAttValue(ERA_Rage);
				pSend->RoleData.nAtt[ERRA_Speed_XZ]		= pRemoteRole->GetAttValue(ERA_Speed_XZ);
				pSend->RoleData.nAtt[ERRA_Speed_Y]		= pRemoteRole->GetAttValue(ERA_Speed_Y);
				pSend->RoleData.nAtt[ERRA_Speed_Swim]	= pRemoteRole->GetAttValue(ERA_Speed_Swim);
				pSend->RoleData.nAtt[ERRA_Speed_Mount]	= pRemoteRole->GetAttValue(ERA_Speed_Mount);
				pSend->RoleData.nAtt[ERRA_Shape]		= pRemoteRole->GetAttValue(ERA_Shape);
				pSend->RoleData.nAtt[ERRA_Hostility]	= pRemoteRole->GetAttValue(ERA_Hostility);
				

				// ��Զ����ҹ�����Ϣ����
				pSend->RoleData.sRemoteOpenSet			= pRemoteRole->GetRemoteOpenSet();

				// ��ǰ�ƺ�
				pSend->RoleData.u16CurActTitleID		= pRemoteRole->GetTitleMgr()->GetActiviteTitle();

				// ���
				pSend->RoleData.sDisplaySet				= pRemoteRole->GetDisplaySet();
				IFASTCODE->MemCpy(&pSend->RoleData.Avatar, pRemoteRole->GetAvatar(), sizeof(tagAvatarAtt));
				IFASTCODE->MemCpy(&pSend->RoleData.AvatarEquip, &pRemoteRole->GetAvatarEquip(), sizeof(tagAvatarEquip));

				// ״̬�б�
				pSend->RoleData.nBuffNum = nBuffNum;
				if( nBuffNum > 0 )
				{
					pRemoteRole->GetAllBuffMsgInfo(pSend->RoleData.Buff, nBuffNum);
				}

				SendMessage(pSend, pSend->dwSize);

				MDEL_MSG(pSend);
			}
		}

		else if( IS_CREATURE(pRecv->dwRoleID[n]))
		{
			Creature* pCreature = pMap->FindCreature(pRecv->dwRoleID[n]);
			if( P_VALID(pCreature) )
			{
				INT nBuffNum = pCreature->GetBuffNum();
				DWORD dwSize = sizeof(tagNS_GetRemoteCreatureState) + ((nBuffNum > 0) ? (nBuffNum - 1)*sizeof(tagBuffMsgInfo) : 0);

				// ����Զ����������ͬ����Ϣ
				MCREATE_MSG(pSend, dwSize, NS_GetRemoteCreatureState);
				
				pSend->CreatureData.dwID = pCreature->GetID();
				pSend->CreatureData.dwTypeID = pCreature->GetTypeID();
				pSend->CreatureData.nLevel = pCreature->GetLevel();
				
				pSend->CreatureData.bOpen = pCreature->GetDoorStatus();
				pSend->CreatureData.dwMapObjID = pCreature->GetMapObjID();
			
				// λ��
				Vector3 vPos = pCreature->GetCurPos();
				pSend->CreatureData.fPos[0] = vPos.x;
				pSend->CreatureData.fPos[1] = vPos.y;
				pSend->CreatureData.fPos[2] = vPos.z;

				// ״̬
				pSend->CreatureData.dwState = pCreature->GetState();

				// ����
				pSend->CreatureData.dwTaggedOwner = pCreature->GetTaggedOwner();

				// ����
				Vector3 vFace = pCreature->GetFaceTo();
				pSend->CreatureData.fFaceTo[0] = vFace.x;
				pSend->CreatureData.fFaceTo[1] = vFace.y;
				pSend->CreatureData.fFaceTo[2] = vFace.z;

				// ����
				pSend->CreatureData.nAtt[ERRA_MaxHP]		= pCreature->GetAttValue(ERA_MaxHP);
				pSend->CreatureData.nAtt[ERRA_HP]			= pCreature->GetAttValue(ERA_HP);
				pSend->CreatureData.nAtt[ERRA_MaxMP]		= pCreature->GetAttValue(ERA_MaxMP);
				pSend->CreatureData.nAtt[ERRA_MP]			= pCreature->GetAttValue(ERA_MP);
				pSend->CreatureData.nAtt[ERRA_Rage]			= pCreature->GetAttValue(ERA_Rage);
				pSend->CreatureData.nAtt[ERRA_Speed_XZ]		= pCreature->GetAttValue(ERA_Speed_XZ);
				pSend->CreatureData.nAtt[ERRA_Speed_Y]		= pCreature->GetAttValue(ERA_Speed_Y);
				pSend->CreatureData.nAtt[ERRA_Shape]		= pCreature->GetAttValue(ERA_Shape);

				// ״̬�б�
				pSend->CreatureData.nBuffNum = nBuffNum;
				if( nBuffNum > 0 )
				{
					pCreature->GetAllBuffMsgInfo(pSend->CreatureData.Buff, nBuffNum);
				}

				SendMessage(pSend, pSend->dwSize);

				MDEL_MSG(pSend);

				//-- �ٴη����ߣ���ֹƯ��
				BYTE	byMsg[1024] = {0};
				DWORD	dwSize1 = pMap->CalMovementMsgEx(pMap->FindUnit(pCreature->GetID()), byMsg, 1024);
				SendMessage(byMsg, dwSize1);
			}
		}
		else if (IS_PET(pRecv->dwRoleID[n]))
		{
			Pet* pPet = pMap->FindPet(pRecv->dwRoleID[n]);
			if (P_VALID(pPet))
			{
				tagNS_GetRemotePetState send;
				send.PetData.dwID		= pPet->GetID();
				send.PetData.dwProtoID	= pPet->GetTypeID();
				send.PetData.uState.byPetState		= pPet->GetPetState();
				Role* pMaster			= pPet->GetMaster();
				send.PetData.dwRoleID	= P_VALID(pMaster) ? pMaster->GetID() : GT_INVALID;

				SendMessage(&send, send.dwSize);
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ������ҵõ������˵�RoleID
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetID(tagNetCmd *pCmd)
{
	tagNC_RoleGetID* pRecv = (tagNC_RoleGetID*)pCmd;
	DWORD dwNameCrc = pRecv->dwRoleNameCrc;
	DWORD dwRoleID = g_roleMgr.GetRoleIDByNameCrc(dwNameCrc);

	tagNS_RoleGetID send;
	send.dwRoleNameCrc = dwNameCrc;
	send.dwRoleID = dwRoleID;
	if (GT_VALID(dwRoleID))
	{
		g_roleMgr.GetRoleNameByNameID(dwRoleID, send.szName);
	}

	SendMessage(&send, send.dwSize);
	return 0;
}

//-----------------------------------------------------------------------------
// ͨ��NameID�õ���ҵ�����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetNameByNameID(tagNetCmd* pCmd)
{
	
	MGET_MSG(pRecv, pCmd, NC_GetNamebyNameID);
	
	tagNS_GetNamebyNameID send;
	send.bResult = TRUE;
	send.dwNameID = pRecv->dwNameID;
	g_roleMgr.GetRoleNameByNameID(pRecv->dwNameID, send.szName);

	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------
// �ͻ��˻�ȡ����������
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetSomeName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_RoleGetSomeName);

	INT		nNum = pRecv->nNum;

	if(nNum <= 0)
		return 0;

	if(nNum > 50)
		return 0;

	DWORD	dwSize = sizeof(tagNS_RoleGetSomeName) + (nNum - 1) * sizeof(tagRoleIDName);
	MCREATE_MSG(pSend, dwSize, NS_RoleGetSomeName);

	pSend->nUserData = pRecv->nUserData;
	pSend->nNum = nNum;

	for(INT n = 0; n < nNum; ++n)
	{
		pSend->IdName[n].dwID = pRecv->dwAllID[n];
		g_roleMgr.GetRoleNameByID(pRecv->dwAllID[n], pSend->IdName[n].szName);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}

//------------------------------------------------------------------------------
// Ͷ��
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleBidAttPoint(tagNetCmd* pCmd)
{
	tagNC_RoleBidAttPoint* pRecv = (tagNC_RoleBidAttPoint*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	DWORD dwErrorCode = m_pRole->BidAttPoint(pRecv->nAttPointAdd);

	tagNS_RoleBidAttPoint send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// ϴ��
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleClearAttPoint(tagNetCmd* pCmd)
{
	tagNC_RoleClearAttPoint* pRecv = (tagNC_RoleClearAttPoint*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	DWORD dwErrorCode = m_pRole->ClearAttPoint(pRecv->n64ItemID);

	tagNS_RoleClearAttPoint send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// ��ɫ�����ʾģʽ����
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetFashion(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_Fashion);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��ȡ��ͼ
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}
	
	// ����Ƿ�͵�ǰ��ʾģʽ��ͬ
	if(pRole->GetDisplaySet().bFashionDisplay == p->bFashion)
	{
		return 0;
	}

	// ������ʾģʽ
	pRole->SetFashionMode(p->bFashion);

	// ����Χ��ҷ����µ������Ϣ
	tagNS_AvatarEquip send;
	send.dwRoleID		= pRole->GetID();
	send.bFashion		= p->bFashion;
	send.sAvatarEquip	= pRole->GetAvatarEquip();
	pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// ��ɫ�����ʾ��λ����
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetDisplay(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_RoleSetDisplay);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��ȡ��ͼ
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// ����Ƿ�͵�ǰ��λ����������ͬ
	const tagDisplaySet& sDisplaySet = pRole->GetDisplaySet();
	if(sDisplaySet.bHideBack == p->sDisplaySet.bHideBack
		&& sDisplaySet.bHideFace == p->sDisplaySet.bHideFace
		&& sDisplaySet.bHideHead == p->sDisplaySet.bHideHead)
	{
		return 0;
	}

	// ���ò�λ��ʾ����
	pRole->SetDisplaySet(p->sDisplaySet.bHideHead, p->sDisplaySet.bHideFace, p->sDisplaySet.bHideBack);

	// ����Χ��ҷ�����Ϣ
	tagNS_RoleSetDisplay send;
	send.dwRoleID		= pRole->GetID();
	send.sDisplaySet	= sDisplaySet;
	pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// ��ɫ����(ʹ��������)
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleChangeRoleName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_LPRename);

	DWORD dwErrorCode = E_Success;

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �����ߵĺϷ���
	tagItem *pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64RenameItemID);
	if(!P_VALID(pItem))
	{
		dwErrorCode = E_UseItem_ItemNotExist;
		//return dwErrorCode;
	}

	if(pItem->pProtoType->eSpecFunc != EISF_RenameItem)
	{
		dwErrorCode = E_UseItem_SpecFunError;
		//return dwErrorCode;
	}

	// ������Ʒ
	//DWORD dwErrorCode = GetItemMgr().ItemUsedFromBag(n64ItemSerial, 1, ELCLD_Item_Use);
	//if(dwErrorCode != E_Success)
	//{
	//	return E_UseSkill_CostLimit;
	//}

	// ������ֺϷ���
	TCHAR buf[X_SHORT_NAME] = {0};
	_tcsncpy(buf, pRecv->szNewName, X_SHORT_NAME);
	buf[X_SHORT_NAME-1] = _T('\0');
	_tcslwr(buf);
	DWORD dwNameCrc = IUTIL->Crc32(buf);

	if ( g_roleMgr.GetRoleIDByNameCrc(dwNameCrc) != (DWORD)GT_INVALID )
	{
		//RoleName�Ѿ�����
		dwErrorCode = E_CreateRole_NameExist;
	}
	else
	{
		// ������ֳ���,�Ϸ����ɿͻ��˱�֤
		dwErrorCode = Filter::CheckName(buf, g_attRes.GetVariableLen().nRoleNameMax, g_attRes.GetVariableLen().nRoleNameMin, g_attRes.GetNameFilterWords());
	}

	// ��¼������
	TCHAR szOldName[X_SHORT_NAME] = {0};
	g_roleMgr.GetRoleNameByID( pRole->GetID(), szOldName );

	// ����
	pRole->ReName(buf);

	tagNS_LPRename send;
	send.dwID = pRole->GetID();
	send.dwErrorCode = dwErrorCode;
	_tcscpy_s(send.szNewName, X_SHORT_NAME, buf);
	_tcscpy_s(send.szOldName, X_SHORT_NAME, szOldName );
	SendMessage(&send, send.dwSize);

	//if ( E_Success != dwErrorCode )
	//{
	//	return FALSE;
	//}

	//// �����ݿ�ĸ�����¼������Ӽ�¼
	//pRole->SaveNewName2DB();

	return TRUE;
}