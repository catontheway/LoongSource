//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: stall_handler.cpp
// author: Sxg
// actor:
// data: 2008-12-26
// last:
// brief: ��̯��Ϣ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_stall.h"
#include "player_session.h"
#include "role.h"
#include "vip_stall.h"

//-----------------------------------------------------------------------------
// ׼����̯
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallStart(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Stall);
		return 0;
	}

	MGET_MSG(pRecv, pCmd, NC_StallStart);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_StallStart send;
	send.dwErrorCode = pRole->StartStall();
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��Ʒ�ϼ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallSetGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallSetGoods);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_StallSetGoods send;
	send.dwErrorCode = pRole->SetStallGoods(pRecv->n64Serial, pRecv->n64UnitPrice, pRecv->byIndex);
	SendMessage(&send, send.dwSize);

	// �Ƿ���Ҫ����Χ��ҹ㲥
	if(pRole->IsSetGoodsFinish() && E_Success == send.dwErrorCode && P_VALID(pRole->GetMap()))
	{
		tagNS_StallSetRefresh send;
		send.dwStallRoleID = pRole->GetID();
		send.byIndex = pRecv->byIndex;

		if( P_VALID(pRole->GetMap()) )
		{
			pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
		}

		// ֪ͨVIP̯λ������Ʒ
		tagVIPStall* pVIPStall = g_VIPStall.GetVIPStallInfo(pRole->GetID());
		if (P_VALID(pVIPStall))
		{
			g_VIPStall.AddEvent(pRole->GetID(), EVT_ChangeStallGoods, 0, NULL);
		}
	}

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��Ʒ�¼�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallUnsetGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallUnsetGoods);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_StallUnsetGoods send;
	send.dwErrorCode = pRole->UnsetStallGoods(pRecv->byIndex);
	SendMessage(&send, send.dwSize);

	// ���⴦��
	if(pRole->IsSetGoodsFinish() && E_Success == send.dwErrorCode)
	{
		// ���̯λ���Ƿ�����Ʒ
		if(pRole->IsNoGoodsInStall())
		{
			// û�У�����̯
			pRole->SendCloseStall();
		}
		else if(P_VALID(pRole->GetMap()))
		{
			// �У�������Χ��ҹ㲥
			tagNS_StallUnsetRefresh send;
			send.dwStallRoleID = pRole->GetID();
			send.byIndex = pRecv->byIndex;

			if( P_VALID(pRole->GetMap()) )
			{
				pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
			}

			// ֪ͨVIP̯λ������Ʒ
			tagVIPStall* pVIPStall = g_VIPStall.GetVIPStallInfo(pRole->GetID());
			if (P_VALID(pVIPStall))
			{
				g_VIPStall.AddEvent(pRole->GetID(), EVT_ChangeStallGoods, 0, NULL);
			}
		}
	}
	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ���ñ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallSetTitle(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallSetTitle);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	MGET_MSG_STRING(strTitle, pRecv->szData, pRecv->dwSize, NC_StallSetTitle, szData);

	tagNS_StallSetTitle send;
	send.dwErrorCode = pRole->SetStallTitle(strTitle.c_str());
	SendMessage(&send, send.dwSize);

	if (send.dwErrorCode == E_Success)
	{
		// ֪ͨVIP̯λ������
		tagVIPStall* pVIPStall = g_VIPStall.GetVIPStallInfo(pRole->GetID());
		if (P_VALID(pVIPStall))
		{
			// ���ﱣ֤�ַ����Ľ�����
			g_VIPStall.AddEvent(pRole->GetID(), EVT_SetStallTitle, sizeof(TCHAR)*(strTitle.length()+1), (LPVOID)(strTitle.c_str()));
		}
	}

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ���ù��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallSetAdText(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallSetAdText);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	MGET_MSG_STRING(strAdText, pRecv->szData, pRecv->dwSize, NC_StallSetAdText, szData);
	
	tagNS_StallSetAdText send;
	send.dwErrorCode = pRole->SetStallAdText(strAdText.c_str());
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ���ù��Ĺ㲥��־λ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallSetAdFlag(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallSetAdFlag);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_StallSetAdFlag send;
	send.dwErrorCode = pRole->SetStallAdFlag(pRecv->bFlag);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ֪ͨ��Χ��ң��Լ���ʼ��̯
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallSetFinish(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallSetFinish);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!P_VALID(pRole->GetMap()))
	{
		return GT_INVALID;
	}

	tagNS_StallSetFinish send;
	send.dwErrorCode	= pRole->SetStallFinish();
	send.dwRoleID		= pRole->GetID();
	send.byStallLevel	= pRole->GetStallModeLevel();

	if( P_VALID(pRole->GetMap()) )
	{
		pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
	}

	if( E_Success == send.dwErrorCode )
	{
		pRole->GetMoveData().StopMoveForce();
	}

	// ֪ͨVIP̯λ����״̬
	tagVIPStall* pVIPStall = g_VIPStall.GetVIPStallInfo(pRole->GetID());
	if (P_VALID(pVIPStall))
	{
		EVIPStallStatus eStatus = EVSS_Open;
		g_VIPStall.AddEvent(pRole->GetID(), EVT_SetStallStatus, 1, &eStatus);
	}

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��̯
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallClose(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallClose);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!P_VALID(pRole->GetMap()))
	{
		return GT_INVALID;
	}

	return pRole->SendCloseStall();
}

//-----------------------------------------------------------------------------
// ���̯λ����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallGetTitle(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallGetTitle);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ���map
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// �ҵ�̯��
	Role *pStallRole = pMap->FindRole(pRecv->dwStallRoleID);
	if(!P_VALID(pStallRole))
	{
		return GT_INVALID;
	}

	INT nChNumTitle = 0;
	
	tagNS_StallGetTitle send;
	send.dwStallRoleID = pRecv->dwStallRoleID;
	send.dwErrorCode = pStallRole->GetStallTitle(send.szTitle);

	// ���¼�����Ϣ��С
	if(E_Success == send.dwErrorCode)
	{
		send.dwSize -= ((STALL_MAX_TITLE_NUM - _tcslen(send.szTitle) - 1) * sizeof(TCHAR));
	}
	else
	{
		send.dwSize -= ((STALL_MAX_TITLE_NUM - 1) * sizeof(TCHAR));
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ���̯λ��������Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallGet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallGet);
	
	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ���map
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// �ҵ�̯��
	Role *pStallRole = pMap->FindRole(pRecv->dwStallRoleID);
	if(!P_VALID(pStallRole))
	{
		//tagNS_StallGet send;
		//send.dwErrorCode	= E_Stall_Cannot_Find_StallRole;
		//send.dwStallRoleID	= pRecv->dwStallRoleID;
		//SendMessage(&send, send.dwSize);

		//return send.dwErrorCode;

		return GT_INVALID;
	}

	// �жϾ���
	if(!pRole->IsInDistance(*pStallRole, MAX_EXCHANGE_DISTANCE))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	// Ϊ������Ϣ����ռ�
	INT	nSzMsg = pStallRole->CalStallGoodsMemUsed();
	if(0 == nSzMsg)
	{
		// ̯λ��û����Ʒ
		return GT_INVALID;
	}

	nSzMsg += (sizeof(tagNS_StallGet) - 1);
	MCREATE_MSG(pSend, nSzMsg, NS_StallGet);

	DWORD dwErrorCode = pStallRole->GetStallGoods(pSend->byData, pSend->byNum, nSzMsg);
	pSend->dwErrorCode		= dwErrorCode;
	pSend->dwStallRoleID	= pRecv->dwStallRoleID;
	
	// ���¼����С
	pSend->dwSize = nSzMsg + (sizeof(tagNS_StallGet) - 1);

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����̯λ�ϵ���Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallBuy(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallBuy);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ���map
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// �ҵ�̯��
	Role *pStallRole = pMap->FindRole(pRecv->dwStallRoleID);
	if(!P_VALID(pStallRole))
	{
		return GT_INVALID;
	}

	// �жϾ���
	if(!pRole->IsInDistance(*pStallRole, MAX_EXCHANGE_DISTANCE))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	INT16 n16RemainNum = 0;
	DWORD dwErrorCode = pStallRole->BuyStallGoods(pRole, 
							pRecv->n64Serial, pRecv->n64UnitPrice, pRecv->n16Num, pRecv->byIndex, n16RemainNum);
	
	if(dwErrorCode != E_Success)
	{
		tagNS_StallBuy send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
	}
	else
	{
		// ���̯λ���Ƿ�����Ʒ
		if(pStallRole->IsNoGoodsInStall())
		{
			// û�У�����̯
			pStallRole->SendCloseStall();
		}
		else
		{
			// ������Ʒ����ʱ��
			g_VIPStall.AddEvent(pRole->GetID(), EVT_ChangeStallGoods, 0, NULL);

			// �У�������Χ�㲥���º����Ʒ����
			tagNS_StallBuyRefresh send;
			send.dwStallRoleID = pRecv->dwStallRoleID;
			send.byIndex = pRecv->byIndex;
			send.n16Num = n16RemainNum;
			pMap->SendBigVisTileMsg(pStallRole, &send, send.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ̯λ��ָ��λ�õ���Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStallGetSpec(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StallGetSpec);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ���map
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// �ҵ�̯��
	Role *pStallRole = pMap->FindRole(pRecv->dwStallRoleID);
	if(!P_VALID(pStallRole))
	{
		return GT_INVALID;
	}

	// �жϾ���
	if(!pRole->IsInDistance(*pStallRole, MAX_EXCHANGE_DISTANCE))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	// �Ƿ���̯
	if(!pStallRole->IsInRoleState(ERS_Stall))
	{
		return GT_INVALID;
	}

	// Ϊ������Ϣ����ռ�
	INT	nSzMsg = (sizeof(tagNS_StallGetSpec) - 1) + (sizeof(tagMsgStallGoods) - 1 + SIZE_EQUIP);
	MCREATE_MSG(pSend, nSzMsg, NS_StallGetSpec);

	DWORD dwErrorCode = pStallRole->GetStallSpecGoods(pRecv->byIndex, pSend->byData, nSzMsg);
	pSend->dwErrorCode		= dwErrorCode;
	pSend->dwStallRoleID	= pRecv->dwStallRoleID;

	// ���¼����С
	pSend->dwSize = nSzMsg + (sizeof(tagNS_StallGetSpec) - 1);

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ����VIP̯λ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetAllVIPStallInfo(tagNetCmd* pCmd)
{
	tagNS_GetAllVIPStallInfo send;

	send.dwErrorCode = g_VIPStall.GetAllVIPStallInfo(send.VIPStallInfo, send.dwRequestTime);

	// !!��ô��Ƚ�Σ�գ�����
	if (send.dwErrorCode != E_Success)
	{
		// ���¼����С
		send.dwSize = sizeof(DWORD);
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����VIP̯λ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpdateVIPStallInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UpdateVIPStallInfo);
	DWORD dwRequestTime = pRecv->dwRequestTime;

	// �����㹻��Ŀռ�
	INT nSzMsg = sizeof(tagNS_UpdateVIPStallInfo) + sizeof(tagVIPStall)*(VIP_STALL_MAX_NUM-1);
	MCREATE_MSG(pSend, nSzMsg, NS_UpdateVIPStallInfo);

	pSend->dwErrorCode = g_VIPStall.GetUpdatedStallInfo(pSend->VIPStallInfo, pSend->nInfoNum, dwRequestTime);

	// �ɹ�ʱ����ʱ�䣬ʧ��Ҳ������
	pSend->dwRequestTime = dwRequestTime;

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNS_UpdateVIPStallInfo) + sizeof(tagVIPStall) * (pSend->nInfoNum - 1);

	SendMessage(pSend, pSend->dwSize);

	return pSend->dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����VIP̯λ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleApplyVIPStall(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pCmd, NC_ApplyVIPStall);

	tagNS_ApplyVIPStall send;

	send.dwErrorCode = g_VIPStall.ApplyVIPStall(pRole, pRecv->byIndex, pRecv->nRent);

	SendMessage(&send, send.dwSize);

	if (send.dwErrorCode == E_Success)
	{
		// ȡ��̯λ��Ϣ
		tagVIPStall* pStallInfo = g_VIPStall.GetVIPStallInfo(pRecv->byIndex);
		if (!P_VALID(pStallInfo))	
			return GT_INVALID;
		
		// ����һ��̯λ��Ϣ
		tagNS_UpdateVIPStallInfo updateMsg;
		updateMsg.dwErrorCode		= E_Success;
		updateMsg.dwRequestTime		= pStallInfo->dwLastUpdateTime;
		updateMsg.nInfoNum			= 1;
		IFASTCODE->MemCpy(updateMsg.VIPStallInfo, pStallInfo, sizeof(tagVIPStall));
	}

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡĳһ��VIP̯λ�ϵ���Ʒ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSpecVIPStallGet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SpecVIPStallGet);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �ҵ�̯��
	Role *pStallRole = g_VIPStall.GetVIPStallRole(pRecv->byIndex);
	if(!P_VALID(pStallRole))
	{
		return GT_INVALID;
	}

	// ���ɲ����Լ���̯λ
	if (pStallRole->GetID() == pRole->GetID())
	{
		tagNS_StallGet send;
		send.dwErrorCode = E_Stall_VIP_Self_Stall;
		SendMessage(&send, send.dwSize);

		return send.dwErrorCode;
	}

	// Ϊ������Ϣ����ռ�
	INT	nSzMsg = pStallRole->CalStallGoodsMemUsed();
	if(0 == nSzMsg)
	{
		// ̯λ��û����Ʒ
		return GT_INVALID;
	}

	//nSzMsg += (sizeof(tagNS_SpecVIPStallGet) - 1);
	//MCREATE_MSG(pSend, nSzMsg, NS_SpecVIPStallGet);

	//DWORD dwRequestTime		= pRecv->dwRequestTime;
	//DWORD dwErrorCode		= g_VIPStall.GetVIPStallGoods(pSend->byData, pSend->byNum, nSzMsg, dwRequestTime, pRecv->byIndex);
	//pSend->dwErrorCode		= dwErrorCode;
	//pSend->byIndex			= pRecv->byIndex;
	//pSend->dwRequestTime	= dwRequestTime;

	//// ���¼����С
	//pSend->dwSize = nSzMsg + (sizeof(tagNS_SpecVIPStallGet) - 1);

	//SendMessage(pSend, pSend->dwSize);

	//MDEL_MSG(pSend);

	nSzMsg += (sizeof(tagNS_StallGet) - 1);
	MCREATE_MSG(pSend, nSzMsg, NS_StallGet);

	DWORD dwErrorCode = pStallRole->GetStallGoods(pSend->byData, pSend->byNum, nSzMsg);
	pSend->dwErrorCode		= dwErrorCode;
	pSend->dwStallRoleID	= pStallRole->GetID();

	// ���¼����С
	pSend->dwSize = nSzMsg + (sizeof(tagNS_StallGet) - 1);

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����VIP̯λ�ϵ���Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleBuyVIPStallGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_VIPStallBuy);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ���ɲ����Լ���̯λ
	if (pRecv->dwStallRoleID == pRole->GetID())
	{
		tagNS_VIPStallBuy send;
		send.dwErrorCode = E_Stall_VIP_Self_Stall;
		SendMessage(&send, send.dwSize);

		return send.dwErrorCode;
	}

	// �ҵ�̯λ
	tagVIPStall* pVIPStallInfo = g_VIPStall.GetVIPStallInfo(pRecv->dwStallRoleID);
	if (!P_VALID(pVIPStallInfo))
	{
		return GT_INVALID;
	}

	// �ҵ�̯��
	Role *pStallRole = g_VIPStall.GetVIPStallRole(pVIPStallInfo->byStallIndex);
	if(!P_VALID(pStallRole))
	{
		return GT_INVALID;
	}

	INT16 n16RemainNum = 0;
	DWORD dwErrorCode = pStallRole->BuyStallGoods(pRole, 
		pRecv->n64Serial, pRecv->n64UnitPrice, pRecv->n16Num, pRecv->byIndex, n16RemainNum);

	if(dwErrorCode != E_Success)
	{
		tagNS_VIPStallBuy send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
	}
	else
	{
		// ���̯λ���Ƿ�����Ʒ
		if(pStallRole->IsNoGoodsInStall())
		{
			// û�У�����̯
			pStallRole->SendCloseStall();
		}
		else
		{
			// ������Ʒ����ʱ��
			g_VIPStall.AddEvent(pRole->GetID(), EVT_ChangeStallGoods, 0, NULL);

			// �У�ֻ�������ҷ��͸��º����Ʒ����
			tagNS_StallBuyRefresh send;
			send.dwStallRoleID = pRecv->dwStallRoleID;
			send.byIndex = pRecv->byIndex;
			send.n16Num = n16RemainNum;
			
			SendMessage(&send, send.dwSize);
		}
	}

	return dwErrorCode;
}