//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: treasure_chest_handler.cpp
// author: twan
// actor:
// data: 2009-07-16
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- �����������
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "role.h"
#include "player_session.h"
#include "db_session.h"
#include "world_session.h"
#include "item_creator.h"
#include "../WorldDefine/msg_digbao.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "TreasureChest_mgr.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "script_mgr.h"


//--------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleOpenTreasureChest(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_TreasureChest);

	//	��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ�����еı���id��Կ��id
	tagItem* pItemChest = pRole->GetItemMgr().GetBagItem(pRecv->n64ChestID);
	tagItem* pItemKey	= pRole->GetItemMgr().GetBagItem(pRecv->n64KeyID);
	if ( !P_VALID(pItemChest) || !P_VALID(pItemKey)) 
	{
		tagNS_TreasureChest send;
		send.dwErrorCode = GT_INVALID;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	DWORD dwChestID =  pItemChest->dwTypeID;
	DWORD dwKeyID = pItemKey->dwTypeID;

	// ��¼��ҵı����Կ��id
	pRole->SetChestSerial(pRecv->n64ChestID);
	pRole->SetKeySerial(pRecv->n64KeyID);
	pRole->SetChestTypeID(dwChestID);
	pRole->SetKeyTypeID(dwKeyID);

	// ���ýű�����
	g_ScriptMgr.GetRoleScript()->OnOpenChest(pRole, dwChestID, dwKeyID);
	return E_Success;
}

//--------------------------------------------------------------------------
// ֹͣ���ѡ����Ʒ������ȷ�����ѡ�е���Ʒ
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleStopTreasureChest(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StopTreasureChest);

	//	��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwChestID =  pRole->GetChestTypeID();
	DWORD dwKeyID = pRole->GetKeyTypeID();

	// ���ýű�����
	g_ScriptMgr.GetRoleScript()->OnStopChest(pRole, dwChestID, dwKeyID);
	return E_Success;
}

//--------------------------------------------------------------------------
// ����һ��
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleAgainTreasureChest(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_AgainTreasureChest);

	//	��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ���ýű�����
	g_ScriptMgr.GetRoleScript()->OnAgainChest(pRole);
	return E_Success;
}

//--------------------------------------------------------------------------
// ��õ���
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleChestGetItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ChestGetItem);

	//	��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwItemID = pRole->GetChestItem().dwTypeID;
	INT nNum = pRole->GetChestItem().nNum;
	DWORD dwChestID = pRole->GetChestTypeID();

	// ���������Ʒ����
	if (dwItemID != pRecv->dwTypeID)
	{
		tagNS_ChestGetItem send;
		send.dwErrorCode = GT_INVALID;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// ���ýű�����
	g_ScriptMgr.GetRoleScript()->OnGetItem(pRole, dwChestID, dwItemID, nNum);
	return E_Success;
}
	