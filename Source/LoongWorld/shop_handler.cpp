//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: shop_handler.h
// author: Sxg
// actor:
// data: 2008-10-28
// last:
// brief: �̵���Ϣ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/shop_define.h"
#include "../WorldDefine/msg_shop.h"
#include "player_session.h"
#include "role.h"

//-----------------------------------------------------------------------------
// ��ȡ��Ʒ(��װ��)����ˢ����Ʒ�б�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetShopItems(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetShopItems);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->byShelf >= MAX_SHOP_SHELF_NUM)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pRole->GetShopItems(pRecv->dwNPCID, pRecv->byShelf);

	// ����������
	pRole->SendShopFeedbackMsg(dwErrorCode, pRecv->dwNPCID);
	
	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡװ����װ���б�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetShopEquips(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetShopEquips);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->byShelf >= MAX_SHOP_SHELF_NUM)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pRole->GetShopEquips(pRecv->dwNPCID, pRecv->byShelf);

	// ����������
	pRole->SendShopFeedbackMsg(dwErrorCode, pRecv->dwNPCID);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ������Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleBuyShopItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BuyShopItem);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->byShelf >= MAX_SHOP_SHELF_NUM
		|| pRecv->n16ItemNum < 0 || MIsEquipment(pRecv->dwTypeID))
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode =	pRole->BuyShopItem(pRecv->dwNPCID, pRecv->byShelf, 
										pRecv->dwTypeID, pRecv->n16ItemNum);

	// ����������
	pRole->SendShopFeedbackMsg(dwErrorCode, pRecv->dwNPCID);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleBuyShopEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BuyShopEquip);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->byShelf >= MAX_SHOP_SHELF_NUM)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pRole->BuyShopEquip(pRecv->dwNPCID, pRecv->byShelf, 
										pRecv->dwTypeID, pRecv->n64Serial);

	// ����������
	pRole->SendShopFeedbackMsg(dwErrorCode, pRecv->dwNPCID);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ������Ʒ&װ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSellToShop(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SellToShop);

	// ��Ϣ�Ϸ�����֤: ��

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pRole->SellToShop(pRecv->dwNPCID, pRecv->n64Serial);

	// ����������
	pRole->SendShopFeedbackMsg(dwErrorCode, pRecv->dwNPCID);

	return dwErrorCode;
}