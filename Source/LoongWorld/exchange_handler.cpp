//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: exchange_handler.cpp
// author: Sxg
// actor:
// data: 2008-08-22
// last:
// brief: ��Ҽ佻����Ϣ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "player_session.h"
#include "../WorldDefine/exchange_define.h"
#include "../WorldDefine/msg_exchange.h"
#include "title_mgr.h"
#include "item_mgr.h"
#include "map.h"
#include "role.h"

//------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExchangeReq(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ExchangeReq);

	if(GT_INVALID == pRecv->dwDstID)
	{
		return GT_INVALID;
	}
	
	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	Role* pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcExchangeReq(pTarget, pRecv->dwDstID);
	if(E_Success == dwErrorCode)
	{
		if(P_VALID(pTarget))
		{
			// �������߷���Ϣ
			tagNS_ExchangeReq send;
			send.dwSrcID = pRole->GetID();
			pTarget->SendMessage(&send, send.dwSize);
		}
		else
		{
			// ��Զ����ִ�е���
			ASSERT(P_VALID(pTarget));
		}
	}
	else if(dwErrorCode != GT_INVALID)
	{
		//// �������߷���
		//tagNS_ExchangeFinish send;
		//send.dwErrorCode	= dwErrorCode;
		//send.dwFailedRoleID	= pRecv->dwDstID;
		//SendMessage(&send, send.dwSize);

		tagNS_ExchangeReqRes send;
		send.dwErrorCode	= dwErrorCode;
		send.dwSrcID		= pRole->GetID();
		send.dwDstID		= pRecv->dwDstID;
		SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��Ҷ������׵ķ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExchangeReqRes(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ExchangeReqRes);

	if(GT_INVALID == pRecv->dwDstID)
	{
		return GT_INVALID;
	}
	
	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pApplicant = NULL;
	DWORD dwErrorCode = pRole->ProcExchangeReqRes(pApplicant, pRecv->dwDstID, pRecv->dwErrorCode);

	// ����Ƿ���Ҫ���ý���״̬
	if(dwErrorCode != E_Exchange_Accept)
	{
		pRole->GetExchMgr().SetTgtRoleID(GT_INVALID);
	}

	// ����Ƿ���Ҫ��������Ϣ
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	// Ϊ�򻯷��������߼��������������˫��������Ϣ���ɿͻ��˴���
	tagNS_ExchangeReqRes send;
	send.dwSrcID = pRole->GetID();
	send.dwDstID = pRecv->dwDstID;
	send.dwErrorCode = dwErrorCode;

	if(E_Exchange_Accept == pRecv->dwErrorCode)
	{
		SendMessage(&send, send.dwSize);
	}

	if(P_VALID(pApplicant))
	{
		pApplicant->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��ӽ�����Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExchangeAdd(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ExchangeAdd);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role	*pTarget	= NULL;
	tagItem *pItem		= NULL;
	INT32	nInsIndex	= GT_INVALID;
	DWORD	dwErrorCode = pRole->ProcExchangeAdd(pTarget, pItem, nInsIndex, pRecv->n64Serial);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}
	
	if(E_Success == dwErrorCode)
	{
		tagNS_ExchangeAdd_2Dst send2Dst;
		send2Dst.dwSrcID	= pRole->GetID();
		send2Dst.nIndex		= nInsIndex;

		// �жϽ��׵���װ��������Ʒ
		if(MIsEquipment(pItem->dwTypeID))
		{
			IFASTCODE->MemCpy(send2Dst.byData, pItem, SIZE_EQUIP);
		}
		else
		{
			IFASTCODE->MemCpy(send2Dst.byData, pItem, SIZE_ITEM);
			send2Dst.dwSize = send2Dst.dwSize - SIZE_EQUIP + SIZE_ITEM;
		}

		pTarget->SendMessage(&send2Dst, send2Dst.dwSize);
	}

	tagNS_ExchangeAdd_2Src send2Src;
	send2Src.dwDstID		= P_VALID(pTarget) ? pTarget->GetID() : GT_INVALID;
	send2Src.n64Serial		= pRecv->n64Serial;
	send2Src.dwErrorCode	= dwErrorCode;
	send2Src.nIndex			= nInsIndex;
	
	SendMessage(&send2Src, send2Src.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ȡ��������Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExchangeDec(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ExchangeDec);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcExchangeDec(pTarget, pRecv->n64Serial);
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}
	
	tagNS_ExchangeDec send;
	send.dwSrcID	= pRole->GetID();
	send.dwDstID	= P_VALID(pTarget) ? pTarget->GetID() : GT_INVALID;
	send.n64Serial	= pRecv->n64Serial;

	// �ӽ����嵥��ɾ��
	send.dwErrorCode = dwErrorCode;

	SendMessage(&send, send.dwSize);

	if(P_VALID(pTarget))
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// �޸Ľ��׽�Ǯ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExchangeMoney(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ExchangeMoney);

	if(pRecv->n64Money < 0)
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcExchangeMoney(pTarget, pRecv->n64Money);
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_ExchangeMoney send;
	send.dwSrcID	= pRole->GetID();
	send.n64Money	= pRecv->n64Money;
	send.dwErrorCode= dwErrorCode;

	SendMessage(&send, send.dwSize);
	if(P_VALID(pTarget))
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExchangeLock(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ExchangeLock);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcExchangeLock(pTarget);
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_ExchangeLock send;
	send.dwSrcID = pRole->GetID();
	send.dwErrorCode = dwErrorCode;

	SendMessage(&send, send.dwSize);
	if(P_VALID(pTarget))
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ���ȡ������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExchangeCancel(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ExchangeCancel);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcExchangeCancel(pTarget);

	tagNS_ExchangeCancel send;
	send.dwSrcID = pRole->GetID();

	SendMessage(&send, send.dwSize);
	if(P_VALID(pTarget) && E_Success == dwErrorCode)
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return E_Success;
}

//------------------------------------------------------------------------
// ȷ�Ͻ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExchangeVerify(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ExchangeVerify);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwFailedRoleID = GT_INVALID;
	DWORD dwErrorCode = pRole->ProcExchangeVerify(pTarget, dwFailedRoleID);
	if(GT_INVALID == dwErrorCode)
	{
		// ������ͻ��˷���
		return dwErrorCode;
	}

	// �ƺ��¼�����
	if (P_VALID(pTarget))
	{
		pRole->GetTitleMgr()->SigEvent(ETE_ROLE_TRANSACTION_SUCCESS, GT_INVALID, GT_INVALID);
		pTarget->GetTitleMgr()->SigEvent(ETE_ROLE_TRANSACTION_SUCCESS, GT_INVALID, GT_INVALID);
	}	

	tagNS_ExchangeFinish send;
	send.dwErrorCode	= dwErrorCode;
	send.dwFailedRoleID	= dwFailedRoleID;

	SendMessage(&send, send.dwSize);
	if(P_VALID(pTarget))
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}