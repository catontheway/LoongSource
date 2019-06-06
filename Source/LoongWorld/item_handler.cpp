//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_handler.cpp
// author: Sxg
// actor:
// data: 2008-07-21
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- ��Ʒ/װ�����
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/msg_loot.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/msg_common.h"

#include "player_session.h"
#include "world_session.h"
#include "role.h"
#include "world.h"
#include "guild_mgr.h"
#include "role_mgr.h"

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Equip);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->ePosDst <= EEP_Start || pRecv->ePosDst >= EEP_End)
	{
		IMSG(_T("The equip position is invalid!"));
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��װ
	DWORD dwErrorCode = pRole->Equip(pRecv->n64Serial, pRecv->ePosDst);

	tagNS_Equip	send;
	send.n64Serial = pRecv->n64Serial;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUnequip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Unequip);
	
	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��װ
	DWORD dwErrorCode = pRole->Unequip(pRecv->n64Serial, pRecv->n16PosDst);
	if(E_Success == dwErrorCode)
	{
		return E_Success;
	}

	tagNS_Unequip send;
	send.n64Serial = pRecv->n64Serial;
	send.dwErrorCode = dwErrorCode;

	// �������ͻ��˻�װ���
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����ֶԻ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSwapWeapon(tagNetCmd* pCmd)
{
	//MGET_MSG(pRecv, pCmd, NC_SwapWeapon);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��װ
	DWORD dwErrorCode = pRole->SwapWeapon();
	if(E_Success == dwErrorCode)
	{
		return E_Success;
	}

	tagNS_SwapWeapon send;
	send.dwErrorCode = dwErrorCode;

	// �������ͻ��˻�װ���
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleIdentifyEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_IdentifyEquip);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pRole->GetItemMgr().IdentifyEquip(pRecv->n64SerialReel, 
												pRecv->n64SerialEquip, ELCLD_Equip_Identify);
	if(E_Success == dwErrorCode)
	{
		return E_Success;
	}

	tagNS_IdentifyEquip send;
	send.n64Serial = pRecv->n64SerialEquip;
	send.dwErrorCode = dwErrorCode;

	// �������ͻ��˻�װ���
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ͬһ�������ƶ���Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChangeItemPos(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemPosChange);

	if(pRecv->n16Num < 0)
	{
		return GT_INVALID;
	}
	
	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode;
	switch(pRecv->eConType)
	{
	case EICT_Equip:
		return pRole->MoveRing(pRecv->n64Serial, pRecv->n16PosDst);
		break;
	case EICT_RoleWare:
		{
			if(pRecv->dwNPCID == GT_INVALID)
			{
				break;
			}
			else
			{
				// ���NPCIDΪ-1������ʹ������ֿ�,������֤npc
				dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);

				if(dwErrorCode != E_Success)
				{
					return dwErrorCode;
				}
			}
		}
		break;

	case EICT_GuildWare:
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
		if(dwErrorCode == E_Success)
		{
			g_guildMgr.AddGuildWareEvent(pRole->GetID(), EVT_GuildWareMoveTo, pCmd->dwSize, pRecv);
		}
		return dwErrorCode;
		break;
	}

	return pRole->GetItemMgr().Move(pRecv->eConType, pRecv->n64Serial, 
								pRecv->n16Num, pRecv->n16PosDst, ELCLD_Item_Move);
}

//-----------------------------------------------------------------------------
// �������ƶ���Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChangeItemPosEx(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemPosChangeEx);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ����ͽ�ɫ�ֿ��йأ������ж�NPC
	if(EICT_RoleWare == pRecv->eConTypeSrc || EICT_RoleWare == pRecv->eConTypeDst)
	{
		DWORD dwErrorCode = E_Success;
		
		if(!GT_VALID(pRecv->dwNPCID)) // ���NPCIDΪ-1������ʹ������ֿ�,������֤npc 
		{
			// �ж�����ֿ�������Ʒ�Ƿ����

			// ��������
			tagItem* pItem = NULL;
			TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

			while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
			{
				if (!P_VALID(pItem))
				{
					continue;
				}
				
				if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
				{
					dwErrorCode = E_Success;
					break;
				}

				dwErrorCode = E_UseItem_ItemNotExist;
			}
		}	
		else
		{
			dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		}

		if(dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}
		
// 		DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
// 		if(dwErrorCode != E_Success)
// 		{
// 			return dwErrorCode;
// 		}
	}

	// ����Ͱ��ɲֿ��йأ������ж�NPC
	if(EICT_GuildWare == pRecv->eConTypeSrc || EICT_GuildWare == pRecv->eConTypeDst)
	{
		DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
		if(dwErrorCode == E_Success)
		{
			g_guildMgr.AddGuildWareEvent(pRole->GetID(), EVT_GuildWareMove2Other, pCmd->dwSize, pRecv);
		}
		return dwErrorCode;
	}

	return pRole->GetItemMgr().Move2Other(pRecv->eConTypeSrc, pRecv->n64Serial1, 
									pRecv->eConTypeDst, pRecv->n16PosDst, ELCLD_Item_Move2Other);
}

//------------------------------------------------------------------------
// ��ָ��˳��������������Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleReorderItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemReorder);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��Ϣ�Ϸ��������
	if(pRecv->n16ItemNum <= 0 
		|| (pRecv->n16ItemNum > SPACE_ALL_BAG && pRecv->n16ItemNum > SPACE_ALL_WARE))
	{
		return GT_INVALID;
	}

	// ������Ϣ
	INT nSzMsg = sizeof(tagNS_ItemReorder) + (pRecv->n16ItemNum - 1) * sizeof(INT16);
	MCREATE_MSG(pSend, nSzMsg, NS_ItemReorder);

	// ����
	DWORD dwErrorCode = E_Success;
	if(GT_INVALID == pRecv->dwNPCID)	
	{
		if(pRecv->eContainerType == EICT_Bag)	// ����
		{
			pSend->eConType = EICT_Bag;
			dwErrorCode = pRole->GetItemMgr().ReorderBag(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
		}
		else if (pRecv->eContainerType == EICT_RoleWare)
		{
			pSend->eConType = EICT_RoleWare;
			dwErrorCode = pRole->GetItemMgr().ReorderRoleWare(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
		}
	}
	else	// �ֿ�
	{
		pSend->eConType = EICT_RoleWare;
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		if(E_Success == dwErrorCode)
		{
			dwErrorCode = pRole->GetItemMgr().ReorderRoleWare(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
		}
	}

	// ������Ϣ
	pSend->n16ItemNum = pRecv->n16ItemNum;
	pSend->dwErrorCode = dwErrorCode;

	if(dwErrorCode != E_Success)
	{
		pSend->dwSize = sizeof(tagNS_ItemReorder);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}
//------------------------------------------------------------------------
// ʹ�ü�¼ʽ���ͷ����ô�������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetTransportMap(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetTransportMap);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	//���õ�
	pRole->SetItemTransportPoint(pRecv);

	return E_Success;
}
//------------------------------------------------------------------------
// ʹ�ü�¼ʽ���ͷ�����
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUseTransportMap(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UseTransportMap);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwTypeID	=	GT_INVALID;
	bool bImmediate =	false;
	INT	nRet		=	E_Success;

	// �����Ʒ�Ƿ��ڱ�����
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID); 
	if( !P_VALID(pItem) ) 
	{
		nRet = E_UseItem_ItemNotExist;
	}
	else
	{
		//���״̬����
		DWORD dwSelfStateFlag = pRole->GetStateFlag();
		if( (dwSelfStateFlag & pItem->pProtoType->dwStateLimit) != dwSelfStateFlag )
		{
			nRet = E_UseItem_SelfStateLimit;
		}
		else if(pItem->pProtoType->eSpecFunc != EISF_TransportTool)
		{
			nRet = E_UseItem_SpecFunError;
		}
		else
		{
			// ������Ʒ��ʧ
			nRet = pRole->GetItemMgr().ItemUsedFromBag(pRecv->n64ItemID, 1, (DWORD)ELCLD_Item_Use);
		}
	}

	if( E_Success != nRet)
	{
		tagNS_UseItem send;
		send.dwSrcRoleID = pRole->GetID();
		send.dwTarRoleID = pRole->GetID();
		send.n64ItemID = pRecv->n64ItemID;
		send.dwTypeID = dwTypeID;
		send.dwSerial = 0;
		send.dwErrorCode = DWORD(nRet);
		send.bInmmediate = bImmediate;

		SendMessage(&send, send.dwSize);
	}
	else
	{
		tagNS_UseItem send;
		send.dwSrcRoleID = pRole->GetID();
		send.dwTarRoleID = pRole->GetID();
		send.n64ItemID = pRecv->n64ItemID;
		send.dwTypeID = dwTypeID;
		send.dwSerial = 0;
		send.nSpellTime = 0;
		send.dwErrorCode = E_Success;
		send.bInmmediate = bImmediate;

		//ʹ�ü�¼ʽ���ͷ�
		pRole->UseNoteTakingItemTransport();
	}

	return E_Success;
}


DWORD PlayerSession::HandleRoleReorderItemEx(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemReorderEx);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��Ϣ�Ϸ��������
	if(pRecv->n16ItemNum <= 0 
		|| (pRecv->n16ItemNum > SPACE_ALL_BAG && pRecv->n16ItemNum > SPACE_ALL_WARE))
	{
		return GT_INVALID;
	}

	// ������Ϣ
	INT nSzMsg = sizeof(tagNS_ItemReorderEx) + pRecv->n16ItemNum * sizeof(tagItemOrder) - sizeof(BYTE);
	MCREATE_MSG(pSend, nSzMsg, NS_ItemReorderEx);

	// ����
	DWORD dwErrorCode = E_Success;
	if( GT_INVALID == pRecv->dwNPCID)	// ����
	{
		if( pRecv->eContainerType == EICT_Bag )
		{
			pSend->eConType = EICT_Bag;
			dwErrorCode = pRole->GetItemMgr().ReorderBagEx(pRecv->byData, pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
		}
		else if( pRecv->eContainerType == EICT_RoleWare )		//����ֿ�
		{
			pSend->eConType = EICT_RoleWare;
			dwErrorCode = pRole->GetItemMgr().ReorderRoleWareEx(pRecv->byData, pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
		}
	}
	else	// �ֿ�
	{
		pSend->eConType = EICT_RoleWare;
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		if(E_Success == dwErrorCode)
		{
			dwErrorCode = pRole->GetItemMgr().ReorderRoleWareEx(pRecv->byData, 
							pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
		}
	}

	// ������Ϣ
	pSend->dwErrorCode = dwErrorCode;

	if(pSend->n16ItemNum != pRecv->n16ItemNum)
	{
		IMSG(_T("\n\nCaution:\n\tRole<id: %u> maybe modified bag order msg!\n"), pRole->GetID());
		ASSERT(pSend->n16ItemNum == pRecv->n16ItemNum);

		pSend->dwSize = sizeof(tagNS_ItemReorderEx) + pSend->n16ItemNum * sizeof(tagItemOrder) - sizeof(BYTE);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ʰȡ��Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePickUpItem(tagNetCmd* pCmd)
{
	tagNC_RolePickUpItem* pRecv = (tagNC_RolePickUpItem*)pCmd;

	if( !P_VALID(GetRole()) )
		return GT_INVALID;

	DWORD dwRtv =GetRole()->CanPickUpItem(pRecv->n64Serial);
	if (E_Success == dwRtv)
	{
		dwRtv = GetRole()->PickUpItem(pRecv->n64Serial);
	}
	tagNS_RolePickUpItem send;
	send.dwRoleID		= GetRole()->GetID();
	send.n64Serial		= pRecv->n64Serial;
	send.dwErrorCode	= dwRtv;
	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------
// �ӵ���Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePutDownItem(tagNetCmd* pCmd)
{
	tagNC_RolePutDownItem* pRecv = (tagNC_RolePutDownItem*)pCmd;

	if( !P_VALID(GetRole()) )
		return GT_INVALID;

	tagNS_RolePutDownItem send;
	send.dwErrorCode = GetRole()->PutDownItem(pRecv->n64Serial);
	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------
// ���Ҽ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_SetBagPsd);
	
	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_BagPsd send;
	
	if(IsHaveBagPsd())	// ������
	{
		send.dwErrorCode = E_BagPsd_Exist;
	}
	else	// δ���ã�����
	{
		SetBagPsd(p->dwBagPsdCrc);
		send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
		send.dwErrorCode	= E_Success;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// ȡ�����Ҽ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUnsetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_UnsetBagPsd);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(!IsHaveBagPsd())
	{
		send.dwErrorCode = E_BagPsd_NoExist;
	}
	else
	{
		if(GetSafeCode() != p->dwSafeCodeCrc)
		{
			send.dwErrorCode = E_BagPsd_SafeCode_Error;
		}
		else
		{
			SetBagPsd(GT_INVALID);
			send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
			send.dwErrorCode = E_Success;
		}
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// ��֤��������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCheckBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_OldBagPsd);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;
	send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
	send.dwErrorCode	= E_BagPsd_OK;

	if(GetBagPsd() != p->dwOldBagPsdCrc)
	{
		send.dwErrorCode = E_BagPsd_Error;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// �޸���������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleResetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_ResetBagPsd);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(!IsHaveBagPsd())
	{
		send.dwErrorCode = E_BagPsd_NoExist;
	}
	else
	{
		if(GetBagPsd() != p->dwOldBagPsdCrc)
		{
			send.dwErrorCode = E_BagPsd_Error;
		}
		else if(p->dwOldBagPsdCrc == p->dwNewBagPsdCrc)
		{
			send.dwErrorCode = GT_INVALID;
		}
		else
		{
			SetBagPsd(p->dwNewBagPsdCrc);
			send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
			send.dwErrorCode = E_Success;
		}
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// �򿪱�������Ҫ��������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleOpenBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_OpenBagPsd);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(IsHaveBagPsd() && GetBagPsd() != p->dwBagPsdCrc)
	{
		send.dwErrorCode = E_BagPsd_Error;
	}
	else
	{
		pRole->SetRoleStateEx(ERSE_BagPsdPass, TRUE);
		send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
		send.dwErrorCode	= E_Success;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// �����������뼰���������Ϣ
//------------------------------------------------------------------------
VOID PlayerSession::SetBagPsd(DWORD dwNewPsdCrc)
{
	m_sAccountCommon.dwBagPsdCrc = dwNewPsdCrc;

	// ��dbserver����Ϣ
	tagNDBC_ChangeBagPsd send;
	send.dwAccountID = GetSessionID();
	send.dwNewBagPsd = dwNewPsdCrc;
	g_dbSession.Send(&send, send.dwSize);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return;
	}
	
	// �������������ý�ɫ״̬
	pRole->m_RoleStateEx.SetState(ERSE_BagPsdPass);
	pRole->m_RoleStateEx.SetState(ERSE_BagPsdExist);

	if(GT_INVALID == dwNewPsdCrc)
	{
		pRole->m_RoleStateEx.UnsetState(ERSE_BagPsdExist);
	}
}

//------------------------------------------------------------------------
// ���������ҵ�װ����Ϣ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleGetSomeoneEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetRemoteRoleEquipInfo);

	DWORD dwRoleID = pRecv->dwRoleID;
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole)) return GT_INVALID; 

	INT nIndex = 0;
	INT nMaxIndex = EEP_MaxEquip;
	BYTE* pTemp = NULL;

	
	INT nMaxSize = sizeof(tagEquip)*nMaxIndex + sizeof(DWORD) + sizeof(INT) + sizeof(BYTE);
	MCREATE_MSG(pSend, nMaxSize,NS_GetRemoteRoleEquipInfo);

	pSend->dwRoleID = pRecv->dwRoleID;
	pSend->nEquipNum = 0;
	pTemp = pSend->byEquip;

	for(; nIndex < nMaxIndex; ++nIndex)
	{	
		tagEquip* pEquipInfo = pRole->GetItemMgr().GetEquipBarEquip((INT16)nIndex);
		if (!P_VALID(pEquipInfo)) continue;

		memcpy(pTemp, pEquipInfo, sizeof(tagEquip));

		pTemp += sizeof(tagEquip);

		(pSend->nEquipNum)++;
		pEquipInfo = NULL;
	}

	(*pTemp) = _T('\0');
	pSend->dwSize = sizeof(tagNS_GetRemoteRoleEquipInfo) + pSend->nEquipNum * sizeof(tagEquip);

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
	return E_Success;
}

//-----------------------------------------------------------------------------
// ������ֿ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSideWareOpen(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_WareOpen);

	DWORD dwRet = E_Success;

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_WareOpen	send;
	send.dwErrorCode = pRole->OpenSideWare(pRecv->n64ItemID);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}
