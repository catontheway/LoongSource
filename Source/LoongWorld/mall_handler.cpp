//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: mall_handler.cpp
// author: Sxg
// actor:
// data: 2009-2-3
// last:
// brief: �̳���Ϣ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_mall.h"
#include "player_session.h"
#include "role.h"
#include "mall.h"
#include "world_net_cmd_mgr.h"

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const UINT32 MAX_REQUEST_GPINFO_PER_TICK = 100;

//-----------------------------------------------------------------------------
// ��ȡ�̳���Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallGet(tagNetCmd* pCmd)
{
	//MGET_MSG(pRecv, pCmd, NC_MallGet);

	// ��Ϣ�Ϸ�����֤: ��

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_OpenMall send;
	send.dwErrorCode = pRole->GetMallAll(send.dwMallTime);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����̳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallUpdate(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MallUpdate);

	// ��Ϣ�Ϸ�����֤
	if(GT_INVALID == pRecv->dwMallTime)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	//tagNS_MallUpdate send;
	//send.dwMallTime = GT_INVALID;
	//pRole->UpdateMallAll(send.dwMallTime, pRecv->dwMallTime);
	//SendMessage(&send, send.dwSize);

	tagNS_OpenMall send;
	send.dwErrorCode = E_Success;
	pRole->UpdateMallAll(send.dwMallTime, pRecv->dwMallTime);
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ������Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallBuyItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MallBuyItem);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->n16BuyNum <= 0 || pRecv->nPrice <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_MallBuyItem send;
	send.dwErrorCode = pRole->BuyMallItem(pRecv->dwTypeID, pRecv->nPrice, pRecv->n16BuyNum, pRecv->nIndexInServer);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ������Ʒ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallBuyPack(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MallBuyPack);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->n16BuyNum != 1 || pRecv->nPrice <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_MallBuyPack send;
	send.dwErrorCode = pRole->BuyMallPack(pRecv->dwTypeID, pRecv->nPrice, pRecv->nIndexInServer);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����������Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallPresentItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MallPresentItem);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->n16BuyNum <= 0 || pRecv->nPrice <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	MGET_MSG_STRING(strLeave, pRecv->szLeave, pRecv->dwSize, NC_MallPresentItem, szLeave);

	tagNS_MallPresentItem send;
	send.dwErrorCode = pRole->BuyMallItem(pRecv->dwRoleTgtID, strLeave.c_str(), 
								pRecv->dwTypeID, pRecv->nPrice, pRecv->n16BuyNum, pRecv->nIndexInServer);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����������Ʒ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallPresentPack(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MallPresentPack);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->n16BuyNum <= 0 || pRecv->nPrice <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	MGET_MSG_STRING(strLeave, pRecv->szLeave, pRecv->dwSize, NC_MallPresentPack, szLeave);

	tagNS_MallPresentPack send;
	send.dwErrorCode = pRole->BuyMallPack(pRecv->dwRoleTgtID, strLeave.c_str(), 
								pRecv->dwTypeID, pRecv->nPrice, pRecv->nIndexInServer);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ�����Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallFreeGetItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MallFreeGetItem);

	// ��Ϣ�Ϸ�����֤: ��

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_MallFreeGetItem send;
	send.dwErrorCode = pRole->GetMallFreeItem(pRecv->dwTypeID);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����Ź�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallLaunchGroupPurchase(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_LaunchGroupPurchase);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->nPrice <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_LaunchGroupPurchase send;
	send.dwErrorCode = g_mall.LaunchGroupPurchase(pRole, pRecv->dwTypeID, pRecv->byScope, pRecv->nIndexInServer, pRecv->nPrice);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��Ӧ�Ź�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallRespondGroupPurchase(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_RespondGroupPurchase);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->nPrice <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_RespondGroupPurchase send;
	send.nGroupPurchaseKey = pRecv->dwRoleID;
	send.nGroupPurchaseKey = (send.nGroupPurchaseKey << 32) | pRecv->dwTypeID;

	send.dwErrorCode = g_mall.RespondGroupPurchase(pRole, pRecv->dwGuildID, pRecv->dwTypeID, pRecv->dwRoleID, pRecv->nPrice);

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ������ڰ��ɵ��Ź���Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallGetGroupPurchaseInfo(tagNetCmd* pCmd)
{
	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �жϷ������Ƿ�æ
	UINT32 nCount = g_worldNetCmdMgr.GetRunTimesPerTick(pCmd->dwID);
	if (nCount < 0)
	{
		IMSG(_T("Msg runtimes status error!\r\n"));
		return GT_INVALID;
	}

	if (nCount > MAX_REQUEST_GPINFO_PER_TICK)
	{
		tagNS_GetSimGPInfo send;
		send.dwErrorCode = E_GroupPurchase_ServerBusy;
		SendMessage(&send, send.dwSize);
		return send.dwErrorCode;
	}

	return g_mall.GetAllSimGPInfo(pRole);
}

//-----------------------------------------------------------------------------
// ��ȡָ���Ź���Ϣ�е���Ӧ���б�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallGetParticipators(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetParticipators);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	return g_mall.GetParticipators(pRole, pRecv->dwGuildID, pRecv->dwTypeID, pRecv->dwRoleID);
}

//-----------------------------------------------------------------------------
// �̳���Ʒ�һ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallItemExchange(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MallItemExchange);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->n16BuyNum <= 0 || pRecv->nPrice <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_MallItemExchange send;
	send.dwErrorCode = pRole->MallItemExchange(pRecv->dwMallID, pRecv->nPrice, pRecv->n16BuyNum, pRecv->nIndexInServer);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// �̳Ǵ����Ʒ�һ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMallPackExchange(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MallPackExchange);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->nPrice <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_MallPackExchange send;
	send.dwErrorCode = pRole->MallPackExchange(pRecv->dwMallID, pRecv->nPrice, pRecv->nIndexInServer);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��Ԫ�������˻���Ԫ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSaveYB2Account(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SaveYB2Account);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->nNum <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_SaveYB2Account send;
	send.dwErrorCode = pRole->SaveYB2Account(pRole->GetID(), pRecv->nNum);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��Ԫ�������˻����Ǯ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSaveSilver2Account(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SaveSilver2Account);

	if(pRecv->nNum <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_SaveSilver2Account send;
	send.dwErrorCode = pRole->SaveSilver2Account(pRole->GetID(), pRecv->nNum);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��Ԫ�������˻�ȡԪ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDepositYBAccount(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DepositAccountYB);

	if(pRecv->nNum <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_DepositAccountYB send;
	send.dwErrorCode = pRole->DepositYBAccout(pRole->GetID(), pRecv->nNum);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��Ԫ�������˻�ȡ��Ǯ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDepositSilver(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DepositAccountSilver);

	if(pRecv->nNum <= 0)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_DepositAccountSilver send;
	send.dwErrorCode = pRole->DepositSilverAccount(pRole->GetID(), pRecv->nNum);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ͬ��Ԫ�����׳�ʼ����Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetYBTradeInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SynYBTradeInfo);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	pRole->GetYBTradeInfo();

	return E_Success;
}

//-----------------------------------------------------------------------------
// ����ύ����Ԫ������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSubmitSellOrder(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SubmitSellOrder);

	// ��ȡ����
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_SubmitSellOrder	send;
	send.dwErrorCode = pRole->SubmitSellOrder(pRole->GetID(), pRecv->nNum, pRecv->nPrice);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����ύ�չ�Ԫ������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSubmitBuyOrder(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SubmitBuyOrder);

	// ��ȡ����
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_SubmitBuyOrder	send;
	send.dwErrorCode = pRole->SubmitBuyOrder(pRole->GetID(), pRecv->nNum, pRecv->nPrice);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ���ɾ���ύ�Ķ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDeleteOrder(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DeleteOrder);

	// ��ȡ����
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_DeleteOrder	send;
	send.dwErrorCode = pRole->DeleteOrder(pRole->GetID(), pRecv->dwOrderID, pRecv->eYBOType);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��һ�ȡһ���ڵĽ��׶���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetYBOrder(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetYuanbaoOrder);

	// ��ȡ����
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	pRole->GetYBOrder(pRole->GetID());

	return E_Success;
}
