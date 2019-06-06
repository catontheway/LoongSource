//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename:guild_warehouse.cpp
// author: Sunnee
// actor:
// data: 2009-06-23
// last:
// brief: ���ɲֿ������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/msg_func_npc.h"
#include "../WorldDefine/guild_define.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/item_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/msg_log.h"
#include "../ServerDefine/msg_guild.h"
#include "container_restrict.h"
#include "role.h"
#include "role_mgr.h"
#include "guild.h"
#include "guild_mgr.h"
#include "guild_warehouse.h"
#include "att_res.h"
#include "map.h"

//-------------------------------------------------------------------------------------------------------
// ����/��������
//-------------------------------------------------------------------------------------------------------
GuildWarehouse::GuildWarehouse()
{
	m_pGuild = NULL;
	m_pContainer = NULL;
	m_bInitOK = FALSE;
}

GuildWarehouse::~GuildWarehouse()
{
	SAFE_DEL(m_pContainer);
}

VOID GuildWarehouse::Init(Guild* pGuild, BOOL bRequest /*= FALSE*/)
{
	if (!P_VALID(pGuild))
	{
		return;
	}

	m_bInitOK		= FALSE;
	m_pGuild		= pGuild;
	INT16 nCurSize	= MGuildWareCurSpace(pGuild->GetGuildAtt().byLevel);
	m_pContainer	= new ItemContainer(EICT_GuildWare, nCurSize, MAX_GUILD_WARE_SPACE, GT_INVALID, pGuild->GetGuildAtt().dwID, new GuildWareRestrict);

	// ��ʼ���ֿ����ʱ��
	m_dwGuildWareTime	= g_world.GetWorldTime();
	m_dwLastSaveTime	= g_world.GetWorldTime();
	m_vecUpdateTime.resize(nCurSize);
	m_vecUpdateTime.assign(nCurSize, m_dwGuildWareTime);

	// ע���¼�
	RegisterGuildWareEventFunc();

	if (bRequest)
	{
		tagNDBC_LoadGuildWareItems send;
		send.dwGuildID = m_pGuild->GetGuildAtt().dwID;
		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		m_bInitOK = TRUE;
	}
}

VOID GuildWarehouse::ReInit()
{
	if (!P_VALID(m_pGuild))
	{
		ASSERT(m_pGuild);
		return;
	}
	else
	{
		// ȡ�õ�ǰ�ֿ�����
		INT16 nCurSize = m_pContainer->GetCurSpaceSize();

		// ���������С(���ɵȼ�ֻ�п���ʩ����һ�ַ�ʽ�������˴������ܳ��ָ���)
		INT16 n16ExtendSz = MGuildWareCurSpace(m_pGuild->GetGuildAtt().byLevel) - nCurSize;
		if (n16ExtendSz <= 0)
		{
			ASSERT(n16ExtendSz > 0);
			return;
		}

		// ���䱳��
		m_pContainer->IncreaseSize(n16ExtendSz);

		// ����ֿ����ʱ��
		nCurSize = m_pContainer->GetCurSpaceSize();
		m_dwGuildWareTime = g_world.GetWorldTime();
		m_vecUpdateTime.resize(nCurSize);
		m_vecUpdateTime.assign(nCurSize, m_dwGuildWareTime);
	}
}

VOID GuildWarehouse::Update()
{
	if (!m_bInitOK)
	{
		return;
	}

	EventMgr<GuildWarehouse>::Update();

	// ������µ���Ʒ
	SaveUpdateItem2DB();
}

//-------------------------------------------------------------------------------------------------------
// �¼�����
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::OnMoveToFunc( DWORD dwSenderID, VOID* pEventMessage )
{
	if (!m_bInitOK)
	{
		return;
	}

	// ������Ҫ�����ж���ҵĺϷ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwSenderID);
	if (!P_VALID(pRole))
	{
		// ��Ҳ�����
		return;
	}

	DWORD dwGuildID = pRole->GetGuildID();
	if (!GT_VALID(dwGuildID) || m_pGuild->GetGuildAtt().dwID != dwGuildID)
	{
		// ��Ҳ��ǰ��ɳ�Ա
		return;
	}

	// �ж����Ȩ��
	tagGuildMember* pMember = m_pGuild->GetMember(dwSenderID);
	if (!P_VALID(pMember))
	{
		// ��Ҳ���������ɳ�Ա
		return;
	}

	if (!pMember->bUseGuildWare && !m_pGuild->GetGuildPower(pMember->eGuildPos).bSetWareRights)
	{
		// ���û�в���Ȩ�ޣ����ش�����
		return;
	}

	MGET_MSG(pRecv, pEventMessage, NC_ItemPosChange);
	DWORD dwErrorCode = Move(pRole, pRecv->n64Serial, pRecv->n16Num, pRecv->n16PosDst, ELCLD_Item_Move);
	// ���ͻ�����Ϣ
}

VOID GuildWarehouse::OnMove2OtherFunc( DWORD dwSenderID, VOID* pEventMessage )
{
	if (!m_bInitOK)
	{
		return;
	}

	// ������Ҫ�����ж���ҵĺϷ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwSenderID);
	if (!P_VALID(pRole))
	{
		// ��Ҳ�����
		return;
	}

	DWORD dwGuildID = pRole->GetGuildID();
	if (!GT_VALID(dwGuildID) || m_pGuild->GetGuildAtt().dwID != dwGuildID)
	{
		// ��Ҳ��ǰ��ɳ�Ա
		return;
	}

	// �ж����Ȩ��
	tagGuildMember* pMember = m_pGuild->GetMember(dwSenderID);
	if (!P_VALID(pMember))
	{
		// ��Ҳ���������ɳ�Ա
		return;
	}

	if (!pMember->bUseGuildWare && !m_pGuild->GetGuildPower(pMember->eGuildPos).bSetWareRights)
	{
		// ���û�в���Ȩ�ޣ����ش�����
		return;
	}

	MGET_MSG(pRecv, pEventMessage, NC_ItemPosChangeEx);
	DWORD dwErrorCode = Move2Other(pRole, pRecv->eConTypeSrc, pRecv->n64Serial1, 
		pRecv->eConTypeDst, pRecv->n16PosDst, ELCLD_Item_Move2Other);
	// ������Ҳ������
}

//-------------------------------------------------------------------------------------------------------
// ע���Ӻ�����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::RegisterGuildWareEventFunc()
{
	RegisterEventFunc(EVT_GuildWareMoveTo,			&GuildWarehouse::OnMoveToFunc);
	RegisterEventFunc(EVT_GuildWareMove2Other,		&GuildWarehouse::OnMove2OtherFunc);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���ɲֿ�����
//-------------------------------------------------------------------------------------------------------
DWORD GuildWarehouse::GetGuildWareInfo( BYTE* pBuff, INT& nItemNum, DWORD& dwLastUpdateTime, INT16& nSzWare, INT32& nSize )
{
	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	// �ϲ㱣֤buff�ռ��㹻��
	if (!P_VALID(pBuff))
	{
		return GT_INVALID;
	}
	BYTE* pTmpBuff = pBuff;
	nItemNum = 0;
	nSize = 0;

	// �ж��Ƿ���Ҫ����
	if (m_dwGuildWareTime <= dwLastUpdateTime)
	{
		return E_GuildWare_NoChange;
	}

	// ȡ�õ�ǰ�ֿ�����
	nSzWare = m_pContainer->GetCurSpaceSize();

	// �������ɲֿ���Ʒ��Ϣ,ɸѡ��Ҫ���µ���Ʒ
	INT nCurSize = m_vecUpdateTime.size();
	for (INT16 n=0; n<nCurSize; n++)
	{
		if (m_vecUpdateTime[n] > dwLastUpdateTime)
		{
			// ��buff��д����Ʒ��Ϣ
			tagGuildWareUpdate* pTemp = (tagGuildWareUpdate*)pTmpBuff;
			pTemp->nIndex = n;

			tagItem* pItem = m_pContainer->GetItem(n);
			if (!P_VALID(pItem))
			{
				if (dwLastUpdateTime == 0)
				{
					// ��һ������
					continue;
				}
				//��Ʒ�Ƿ�������Ʒ��ʧ
				pTemp->eType = EGWU_Delete;
				nItemNum++;
				pTmpBuff += pTemp->Size();
				continue;
			}

			if (!MIsEquipment(pItem->dwTypeID))
			{
				IFASTCODE->MemCpy(pTemp->byData, pItem, sizeof(tagItem));
			}
			else
			{
				IFASTCODE->MemCpy(pTemp->byData, pItem, sizeof(tagEquip));
			}
			nItemNum++;

			pTmpBuff += pTemp->Size();
		}
	}

	nSize = (pTmpBuff - pBuff) * sizeof(BYTE);
	dwLastUpdateTime = m_dwGuildWareTime;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���ɲֿ��ڲ��ƶ�(n16Num==0����ȫ���ƶ�)
//-------------------------------------------------------------------------------------------------------
DWORD GuildWarehouse::Move( Role* pRole, INT64 n64Serial, INT16 n16Num, INT16 n16PosDst, DWORD dwCmdID )
{
	ASSERT(n16Num >= 0);
	ASSERT(P_VALID(pRole));

	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	// �ж����Ȩ��
	DWORD dwErrorCode = MoveAllowable(pRole);
	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(pRole->GetSession(), dwErrorCode);
		return dwErrorCode;
	}

	ItemContainer *pItemCon = m_pContainer;
	if(!P_VALID(pItemCon))
	{
		return GT_INVALID;
	}

	tagItem *pItem1 = pItemCon->GetItem(n64Serial);
	if(!P_VALID(pItem1))
	{
		return GT_INVALID;
	}

	dwErrorCode	= GT_INVALID;
	INT16 n16SrcPos		= pItem1->n16Index;	// n64Serialԭλ��

	tagItemMove	itemMove;

	if(0 == n16Num)
	{
		n16Num = pItem1->n16Num;
		dwErrorCode = pItemCon->MoveTo(n64Serial, n16PosDst, itemMove);
	}
	else if(n16Num > 0)
	{
		dwErrorCode = pItemCon->MoveTo(n64Serial, n16Num, n16PosDst, itemMove);
	}

	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
#ifdef _DEBUG
		IMSG(_T("\nMove item<roleid:%u, container type: %d, serial:%lld, error:%u> in container failed!\n\n"),
			pItemCon->GetOwnerID(), pItemCon->GetConType(), n64Serial, dwErrorCode);
#endif // _DEBUG

		return dwErrorCode;
	}

	// ��ͻ��˷���Ϣ
	tagNS_ItemPosChange send;
	send.eConType	= EICT_GuildWare;
	send.n64Serial1	= n64Serial;
	send.n64Serial2	= itemMove.pItem2 != NULL ? itemMove.pItem2->n64Serial : GT_INVALID;

	send.n16Num1	= itemMove.n16NumRes1;
	send.n16Num2	= itemMove.n16NumRes2;
	send.bCreateItem = itemMove.bCreateItem;

	if(itemMove.bChangePos)
	{
		send.n16PosDst1	= n16PosDst;
		send.n16PosDst2	= n16SrcPos;
	}
	else
	{
		send.n16PosDst1	= n16SrcPos;
		send.n16PosDst2	= n16PosDst;
	}

	if(itemMove.bOverlap)
	{
		send.n16PosDst1	= n16SrcPos;
		send.n16PosDst2	= n16PosDst;
	}

	SendMessage(pRole, &send, send.dwSize);

	// ����Ƿ���Ҫ��¼log
	if(itemMove.bCreateItem || itemMove.bOverlap)
	{
		LogItem(pRole->GetID(), *pItem1, itemMove.pItem2, -n16Num, dwCmdID);
	}

	// ����ƶ�����Ʒ���Ƿ���ڣ��粻����,����LoongDBͬ���ͻ����ڴ�
	if(0 == itemMove.n16NumRes1)
	{
		DeleteItemFromDB(n64Serial, pItem1->dwTypeID);

		// ����Ƿ����ͷŸ���Ʒ�ڴ�
		Destroy(pItem1);
	}

	// ����Ƿ񴴽����µ���Ʒ��
	if(itemMove.bCreateItem)
	{
		tagNDBC_NewItem send;
		IFASTCODE->MemCpy(&send.item, itemMove.pItem2, SIZE_ITEM);

		g_dbSession.Send(&send, send.dwSize);
	}

	// ����������λ��������ʱ��
	SetUpdateTime(n16SrcPos);
	SetUpdateTime(n16PosDst);

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ���ɲֿ�����ұ���֮���ƶ�
//-------------------------------------------------------------------------------------------------------
DWORD GuildWarehouse::Move2Other( Role* pRole, EItemConType eConTypeSrc, INT64 n64Serial1, EItemConType eConTypeDst, INT16 n16PosDst, DWORD dwCmdID )
{
	ASSERT(P_VALID(pRole));

	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	// �ж����Ȩ��
	DWORD dwErrorCode = MoveAllowable(pRole);
	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(pRole->GetSession(), dwErrorCode);
		return dwErrorCode;
	}

	ItemContainer *pConSrc = NULL;
	ItemContainer *pConDst = NULL;

	// �ϲ�Ӧ�ñ�֤������������һ���ǰ��ɲֿ�
	if (eConTypeSrc == EICT_Bag && eConTypeDst == EICT_GuildWare)
	{
		pConSrc = &(pRole->GetItemMgr().GetBag());
		pConDst = m_pContainer;
	}
	else if (eConTypeSrc == EICT_GuildWare && eConTypeDst == EICT_Bag)
	{
		pConSrc = m_pContainer;
		pConDst = &(pRole->GetItemMgr().GetBag());
	}
	else
	{
		return GT_INVALID;
	}

	if(!P_VALID(pConSrc) || !P_VALID(pConDst))
	{
		return GT_INVALID;
	}

	// ��ʼ�ƶ�
	tagItem *pItem1 = pConSrc->GetItem(n64Serial1);
	if(!P_VALID(pItem1))
	{
		return GT_INVALID;
	}
	
	tagItem  item1Old; //used for log
	memcpy(&item1Old, pItem1, sizeof(tagItem));

	dwErrorCode	= GT_INVALID;
	INT16 n16PosSrc1	= pItem1->n16Index;	// n64Serialԭλ��
	INT16 n16Num		= pItem1->n16Num;
	DWORD dwTypeID		= pItem1->dwTypeID;

	tagItemMove	itemMove;

	if(GT_INVALID == n16PosDst)	// �һ���û��ָ��Ŀ��λ��
	{
		dwErrorCode = pConSrc->MoveTo(n64Serial1, *pConDst, itemMove, n16PosDst);
	}
	else
	{
		dwErrorCode = pConSrc->MoveTo(n64Serial1, *pConDst, n16PosDst, itemMove);
	}

	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
#ifdef _DEBUG
		IMSG(_T("\nMove item<roleid:%u, container type: %d, serial:%lld, error:%u> in container failed!\n\n"),
			pConSrc->GetOwnerID(), pConSrc->GetConType(), n64Serial1, dwErrorCode);
#endif // _DEBUG

		return dwErrorCode;
	}

	// ��ͻ��˷���Ϣ
	tagNS_ItemPosChangeEx send;

	send.eConTypeSrc1	= eConTypeSrc;
	send.eConTypeSrc2	= eConTypeDst;
	send.n64Serial1		= n64Serial1;
	send.n64Serial2		= itemMove.pItem2 != NULL ? itemMove.pItem2->n64Serial : GT_INVALID;
	send.n16Num1		= itemMove.n16NumRes1;
	send.n16Num2		= itemMove.n16NumRes2;

	if(itemMove.bOverlap)
	{
		send.eConTypeDst1	= eConTypeSrc;
		send.eConTypeDst2	= eConTypeDst;
		send.n16PosDst1		= n16PosSrc1;
		send.n16PosDst2		= n16PosDst;
	}
	else
	{
		send.eConTypeDst1	= eConTypeDst;
		send.eConTypeDst2	= eConTypeSrc;
		send.n16PosDst1		= n16PosDst;
		send.n16PosDst2		= n16PosSrc1;
	}

	SendMessage(pRole, &send, send.dwSize);

	// ���񴥷�
	if( eConTypeSrc == EICT_GuildWare && eConTypeDst == EICT_Bag )
	{
		pRole->OnQuestEvent(EQE_Item, dwTypeID, n16Num, TRUE);
	}

	if( eConTypeSrc == EICT_Bag && eConTypeDst == EICT_GuildWare )
	{
		pRole->OnQuestEvent(EQE_Item, dwTypeID, n16Num, FALSE);
	}

	// ����Ƿ���Ҫ��¼log
	if(itemMove.pItem2)
	{
		if(itemMove.pItem2->eConType == item1Old.eConType) // swap
		{
			item1Old.n16Num = 0;
			LogItem(pRole->GetID(), item1Old, pItem1, -pItem1->n16Num, dwCmdID);
			memcpy(&item1Old, itemMove.pItem2, sizeof(tagItem));
			item1Old.n16Num = 0;
			item1Old.eConType = eConTypeDst;
			LogItem(pRole->GetID(), item1Old, itemMove.pItem2, -itemMove.pItem2->n16Num, dwCmdID);
		}
		else
		{
			item1Old.n16Num = pItem1->eConType==EICT_Ground? 0 : pItem1->n16Num;
			LogItem(pRole->GetID(), item1Old, itemMove.pItem2, item1Old.n16Num - n16Num, dwCmdID);
		}
	}
	else
	{
		item1Old.n16Num = 0;
		LogItem(pRole->GetID(), item1Old, pItem1, -n16Num, dwCmdID);
	}

	// ����ƶ�����Ʒ���Ƿ���ڣ��粻����,����LoongDBͬ���ͻ����ڴ�
	if(0 == itemMove.n16NumRes1 || pItem1->eConType == EICT_Ground)
	{
		DeleteItemFromDB(n64Serial1, pItem1->dwTypeID);

		// �ͷŸ���Ʒ�ڴ�
		Destroy(pItem1);
	}

	// ���°��ɲֿ��ʱ��
	if (eConTypeSrc == EICT_GuildWare)
	{
		SetUpdateTime(n16PosSrc1);
	}
	else if (eConTypeDst == EICT_GuildWare)
	{
		SetUpdateTime(n16PosDst);
	}
	else
	{
		ASSERT(0);
	}

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��ͻ��˷�����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::SendMessage( Role* pRole, LPVOID pMsg, DWORD dwSize )
{
	ASSERT(P_VALID(pRole));

	if (!P_VALID(pRole))
	{
		return;
	}

	PlayerSession *pSession = pRole->GetSession();
	if(P_VALID(pSession))
	{
		pSession->SendMessage(pMsg, dwSize);
	}

	ASSERT(P_VALID(pSession));
}

//-------------------------------------------------------------------------------------------------------
// �����Ʒ�Ƿ���Ҫ��¼log������Ҫ������LoongDB������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::LogItem(DWORD dwRoleID, const tagItem &item1, const tagItem *pItem2, INT16 n16OptNum, DWORD dwCmdID)
{
	if(!(IsItemNeedLog(item1) || (P_VALID(pItem2) && IsItemNeedLog(*pItem2))))
	{
		return;
	}

	tagNDBC_LogItem send;
	send.sLogItem.dwRoleID		= dwRoleID;
	send.sLogItem.dwTypeID		= item1.pProtoType->dwTypeID;

	send.sLogItem.n64Serial1	= item1.n64Serial;
	send.sLogItem.n8ConType1	= item1.eConType;
	send.sLogItem.n16ResNum1	= item1.n16Num;

	send.sLogItem.n16OptNum		= n16OptNum;
	send.sLogItem.dwCmdID		= dwCmdID;

	if(P_VALID(pItem2))
	{
		send.sLogItem.n64Serial2	= pItem2->n64Serial;
		send.sLogItem.n8ConType2	= pItem2->eConType;
		send.sLogItem.n16ResNum2	= pItem2->n16Num;
	}
	else
	{
		send.sLogItem.n64Serial2	= 0;
		send.sLogItem.n8ConType2	= EICT_Null;
		send.sLogItem.n16ResNum2	= 0;
	}

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ����ָ����λ��ʱ��
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::SetUpdateTime( INT16 nPos )
{
	m_vecUpdateTime[nPos] = m_dwGuildWareTime = g_world.GetWorldTime();
}

//-------------------------------------------------------------------------------------------------------
// ��������Ʒ
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::InsertItem2DB(tagItem &item)
{
	if(MIsEquipment(item.dwTypeID))
	{
		tagNDBC_NewEquip send;
		IFASTCODE->MemCpy(&send.equip, &item, SIZE_EQUIP);
		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		tagNDBC_NewItem send;
		IFASTCODE->MemCpy(&send.item, &item, SIZE_ITEM);
		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ɾ����Ʒ
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::DeleteItemFromDB(INT64 n64Serial, INT32 dwTypeID)
{
	if(!MIsEquipment(dwTypeID))
	{
		tagNDBC_DelItem send;
		send.n64Serial = n64Serial;

		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		tagNDBC_DelEquip send;
		send.n64Serial = n64Serial;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ���������Ʒ
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::SaveUpdateItem2DB()
{
	// ���ֿ���Ʒ�Ƿ����˱仯
	if (m_dwGuildWareTime <= m_dwLastSaveTime)
	{
		return;
	}

	// �����㹻��Ŀռ�
	INT nMsgSize = sizeof(tagNDBC_SaveGuildWare) + m_pContainer->GetCurSpaceSize() * sizeof(tagItemUpdate);
	MCREATE_MSG(pSend, nMsgSize, NDBC_SaveGuildWare);
	MTRANS_POINTER(pItemUpdate, pSend->byData, tagItemUpdate);
	pSend->nItemNum = 0;

	tagItem* pTemp	= NULL;
	for(INT16 i=0; i<m_pContainer->GetCurSpaceSize(); ++i)
	{
		pTemp = m_pContainer->GetItem(i);
		if(P_VALID(pTemp) && pTemp->eStatus != EUDBS_Null)
		{
			pItemUpdate[pSend->nItemNum].byConType	= pTemp->eConType;
			pItemUpdate[pSend->nItemNum].dwOwnerID	= pTemp->dwOwnerID;
			pItemUpdate[pSend->nItemNum].dwAccountID= pTemp->dwAccountID;
			pItemUpdate[pSend->nItemNum].n16Index	= pTemp->n16Index;
			pItemUpdate[pSend->nItemNum].n16Num		= pTemp->n16Num;
			pItemUpdate[pSend->nItemNum].n64Serial	= pTemp->n64Serial;
			pItemUpdate[pSend->nItemNum].nUseTimes	= pTemp->nUseTimes;

			pTemp->SetUpdate(EUDBS_Null);

			pSend->nItemNum++;
		}
	}

	if (pSend->nItemNum > 0)
	{
		// ����������Ϣ��С
		pSend->dwSize = sizeof(tagNDBC_SaveGuildWare) + pSend->nItemNum * sizeof(tagItemUpdate);

		// ������Ϣ
		g_dbSession.Send(pSend, pSend->dwSize);

		// ��¼����ʱ��
		m_dwLastSaveTime = m_dwGuildWareTime;
	}
}

//-------------------------------------------------------------------------------------------------------
// ������ɲֿ���Ʒ
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::LoadWarehouseItems( const BYTE* pData, INT nItemNum )
{
	INT32 nItemSize		= sizeof(tagItem);
	INT32 nEquipSize	= sizeof(tagEquip);

	DWORD dwErrorCode = GT_INVALID;
	TList<tagItem *> listItem;
	const tagItem	*pTmpItem	= NULL;
	tagItem			*pNewItem	= NULL;

	pTmpItem = (const tagItem *)pData;
	for(INT32 i=0; i<nItemNum; ++i)
	{
		if(!MIsEquipment(pTmpItem->dwTypeID))
		{
			pNewItem = new tagItem;
			IFASTCODE->MemCpy(pNewItem, pTmpItem, nItemSize);
			pNewItem->pProtoType = g_attRes.GetItemProto(pTmpItem->dwTypeID);

			pTmpItem = (const tagItem*)((BYTE*)pTmpItem + nItemSize);
		}
		else
		{
			pNewItem = new tagEquip;
			IFASTCODE->MemCpy(pNewItem, pTmpItem, nEquipSize);
			pNewItem->pProtoType = g_attRes.GetEquipProto(pTmpItem->dwTypeID);

			pTmpItem = (tagEquip*)((BYTE*)pTmpItem + nEquipSize);
		}

		if(!P_VALID(pNewItem->pProtoType))
		{
			ASSERT(P_VALID(pNewItem->pProtoType));
			MAttResCaution(_T("item/equip"), _T("typeid"), pTmpItem->dwTypeID);
			IMSG(_T("The item(SerialNum: %lld) hasn't found proto type!\n"), pTmpItem->n64Serial);
			Destroy(pNewItem);
			continue;
		}

		pNewItem->eStatus = EUDBS_Null;
		pNewItem->pScript = g_ScriptMgr.GetItemScript( pNewItem->dwTypeID);

		// ����ֿ�������
		dwErrorCode = m_pContainer->Add(pNewItem, pNewItem->n16Index, FALSE, FALSE);
		if(dwErrorCode != E_Success)
		{
			if(dwErrorCode != E_Item_Place_NotFree)
			{
				ASSERT(0);
				Destroy(pNewItem);
				continue;
			}

			listItem.PushBack(pNewItem);
		}
	}

	// ����Ƿ�����λ���ظ����µĲ�����ӵ�������
	while(listItem.Size() != 0)
	{
		INT16 nIndex;
		tagItemMove ItemMove;
		pNewItem = listItem.PopFront();

		// ����Ǳ�����ֿ��е���Ʒ�������λ
		switch(pNewItem->eConType)
		{
		case EICT_GuildWare:
			dwErrorCode = m_pContainer->Add(pNewItem, nIndex, ItemMove, FALSE, FALSE);
			if(E_Con_NotEnoughSpace == dwErrorCode)
			{
				IMSG(_T("Because container<eType: %d> is full, item<serial: %lld> load failed!\n"), 
					EICT_GuildWare, pNewItem->n64Serial);
				IMSG(_T("Make some places and login again can solve this problem!\n"));
			}
			break;

		default:
			ASSERT(0);
			Destroy(pNewItem);
			continue;
		}
	}

	// ��ʼ�����
	m_bInitOK = TRUE;
}

//-------------------------------------------------------------------------------------------------------
// �ж�����Ƿ���Բ������ɲֿ�
//-------------------------------------------------------------------------------------------------------
DWORD GuildWarehouse::MoveAllowable( Role* pRole )
{
	ASSERT(P_VALID(pRole));

	DWORD dwErrorCode = E_Success;

	// �ж���Ұ���
	DWORD dwGuildID = pRole->GetGuildID();
	if (!GT_VALID(dwGuildID) || m_pGuild->GetGuildAtt().dwID != dwGuildID)
	{
		dwErrorCode = E_Guild_MemberNotIn;
	}

	// �ж����Ȩ��
	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		// �쳣,��Ҳ��ڰ�����,����������Ұ�������
		pRole->SetGuildID(GT_INVALID);
		dwErrorCode = E_Guild_MemberNotIn;
	}
	if (!pMember->bUseGuildWare && !m_pGuild->GetGuildPower(pMember->eGuildPos).bSetWareRights)
	{
		// ���û�в���Ȩ��
		dwErrorCode = E_Guild_Power_NotEnough;
	}

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ɾ���ֿ���������Ʒ
//-------------------------------------------------------------------------------------------------------
VOID GuildWarehouse::RemoveAllItems()
{
	for(INT16 i=0; i<m_pContainer->GetCurSpaceSize(); ++i)
	{
		tagItem *pItem = m_pContainer->GetItem(i);

		if (P_VALID(pItem))
		{
			m_pContainer->Remove(pItem->n64Serial, TRUE, FALSE);

			// ֪ͨ���ݿ�
			DeleteItemFromDB(pItem->n64Serial, pItem->dwTypeID);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ���Ͱ��ɲֿ����Ȩ���б�
//-------------------------------------------------------------------------------------------------------
DWORD GuildWarehouse::SendGuildWarePriList2Client( Role* p2Role )
{
	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	ASSERT(P_VALID(p2Role));
	if (!P_VALID(p2Role))
	{
		return GT_INVALID;
	}

	INT16 nNum = m_pGuild->GetGuildMemberNum();
	ASSERT(nNum > 0);
	if (nNum <= 0)
	{
		return GT_INVALID;
	}

	tagGuildMember* pMember = m_pGuild->GetMember(p2Role->GetID());
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}

	// �ж����Ȩ��
	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bSetWareRights)
	{
		return E_Guild_Power_NotEnough;
	}

	// �����ڴ�
	INT32 nMsgSz = sizeof(tagNS_GetGuildWarePriList) + (nNum - 1) * sizeof(tagGuildWarePri);
	MCREATE_MSG(pSend, nMsgSz, NS_GetGuildWarePriList);

	pSend->nNum = 0;
	MTRANS_POINTER(pPriInfo, pSend->sGuildWarePri, tagGuildWarePri);

	// ��ð��ɳ�Ա��
	MapGuildMember& mapMember = m_pGuild->GetGuildMemberMap();

	// �����б���Ϣ
	pMember = NULL;
	MapGuildMember::TMapIterator iter = mapMember.Begin();
	while (mapMember.PeekNext(iter, pMember))
	{
		if (!P_VALID(pMember))
		{
			continue;
		}

		if (m_pGuild->GetGuildPower(pMember->eGuildPos).bSetWareRights)
		{
			continue;
		}

		pPriInfo[pSend->nNum].dwRoleID	= pMember->dwRoleID;
		pPriInfo[pSend->nNum].bCanUse	= pMember->bUseGuildWare;
		pSend->nNum++;
	}

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNS_GetGuildWarePriList) + (pSend->nNum - 1) * sizeof(tagGuildWarePri);

	// ������Ϣ
	p2Role->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ֿ��з�����Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD GuildWarehouse::AddItem( tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB /*= TRUE*/ )
{
	ASSERT(P_VALID(pItem));
	ASSERT(P_VALID(pItem->n64Serial));

	INT16		n16Index;
	DWORD		dwErrorCode;
	tagItemMove	itemMove;
	INT16		n16AddNum = pItem->n16Num;

	// ����Ƿ��л�ȡʱ��
	if(0 == pItem->dw1stGainTime)
	{
		pItem->dw1stGainTime = g_world.GetWorldTime();
	}

	// ��������
	dwErrorCode = m_pContainer->Add(pItem, n16Index, itemMove);

	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
		IMSG(_T("\nAdd item to guild warehouse failed! \nroleid: %u, item serial: %lld, container type: %d!!!!\n\n"),
			m_pContainer->GetOwnerID(), pItem->n64Serial, m_pContainer->GetConType());

		return dwErrorCode;
	}

	INT16 n16Num = itemMove.n16NumRes1 != 0 ? itemMove.n16NumRes1: itemMove.n16NumRes2;

	// ���������Ʒ,��û�к�������Ʒ�ѵ�,����LoongDB����Ϣ
	if(bInsert2DB && !itemMove.bOverlap)
	{
		// �󶨴���
		if(EBS_Unknown == pItem->byBind)
		{
			pItem->Bind();
		}

		InsertItem2DB(*pItem);
		pItem->SetUpdate(EUDBS_Null);
	}
	
	// ����Ƿ���Ҫ��¼log
	LogItem(GT_INVALID, *pItem, itemMove.pItem2, n16AddNum, dwCmdID);

	// ����Ƿ����ͷŸ���Ʒ�ڴ�
	if(itemMove.bOverlap)
	{
		Destroy(pItem);
		pItem = itemMove.pItem2;
	}

	// ���¸���λ�ĸ���ʱ��
	SetUpdateTime(n16Index);

	return dwErrorCode;
}