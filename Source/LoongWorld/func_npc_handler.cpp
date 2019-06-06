//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: func_npc_handler.cpp
// author: Sxg
// actor:
// data: 2008-11-10
// last:
// brief: ����ְ��NPC��Ϣ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/func_npc_define.h"
#include "../WorldDefine/msg_func_npc.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_common.h"
#include "role.h"
#include "creature.h"

//-----------------------------------------------------------------------------
// ��վ(Ǭ��ʯ)��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDak(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Dak);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->nIndex < 0 || pRecv->nIndex >= MAX_DAK_SITE_NUM)
	{
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_Dak	send;
	send.dwErrorCode	= pRole->ProcDak(pRecv->dwNPCID, pRecv->nIndex, pRecv->dwMapID);

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// �򿪲ֿ�
//-----------------------------------------------------------------------------
//DWORD PlayerSession::HandleRoleWareOpen(tagNetCmd* pCmd)
//{
//	MGET_MSG(pRecv, pCmd, NC_);
//
//	// ��ȡ����
//	Role* pRole = GetRole();
//	if(!P_VALID(pRole))
//	{
//		return GT_INVALID;
//	}
//
//	tagNS_	send;
//	send.dwErrorCode = pRole->(pRecv);
//
//	SendMessage(&send, send.dwSize);
//
//	return send.dwErrorCode;
//}

//-----------------------------------------------------------------------------
// �ֿ�����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleWareExtend(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_WareExtend);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_WareExtend send;
	send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
	if(E_Success == send.dwErrorCode)
	{
		send.dwErrorCode = pRole->GetItemMgr().ExtendRoleWare(pRecv->n64Serial);
		send.n16WareNum = pRole->GetItemMgr().GetWareCurSize();
	}

	SendMessage(&send, send.dwSize);

	if(E_Success == send.dwErrorCode)
	{
		tagNDBC_WareSizeUpdate sendDB;
		sendDB.dwAccountID	= GetSessionID();
		sendDB.n16WareSize	= pRole->GetItemMgr().GetWareCurSize();
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleBagExtand(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BagExtend);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_BagExtend send;
	send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
	if(E_Success == send.dwErrorCode)
	{
		send.dwErrorCode = pRole->GetItemMgr().ExtendBag(pRecv->n64Serial);
		send.n16BagNum = pRole->GetItemMgr().GetBagCurSize();
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// ��ɫ�ֿ��д��Ǯ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSaveSilver(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SaveSilver);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��Ǯ�Ϸ��Լ��
	if(pRecv->n64SilverSave <= 0
		|| pRecv->n64SilverSave > pRole->GetCurMgr().GetBagSilver() 
		|| pRecv->n64SilverSave > pRole->GetCurMgr().GetCanIncWareSilver())
	{
		return GT_INVALID;
	}

	tagNS_SaveSilver send;
	if (!GT_VALID(pRecv->dwNPCID))
		send.dwErrorCode = E_Success;
	else
		send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);

	if(E_Success == send.dwErrorCode)
	{
		pRole->GetCurMgr().DecBagSilver(pRecv->n64SilverSave, ELCID_RoleWare_SaveSilver);
		pRole->GetCurMgr().IncWareSilver(pRecv->n64SilverSave, ELCID_RoleWare_SaveSilver);
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// ��ɫ�ֿ��д�ȡ��Ǯ&Ԫ��
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetSilver(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetSilver);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��Ǯ�Ϸ��Լ��
	if(pRecv->n64SilverGet <= 0
		|| pRecv->n64SilverGet > pRole->GetCurMgr().GetWareSilver() 
		|| pRecv->n64SilverGet > pRole->GetCurMgr().GetCanIncBagSilver())
	{
		return GT_INVALID;
	}

	tagNS_GetSilver send;
	if (!GT_VALID(pRecv->dwNPCID))
		send.dwErrorCode = E_Success;
	else
		send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);

	if(E_Success == send.dwErrorCode)
	{
		pRole->GetCurMgr().DecWareSilver(pRecv->n64SilverGet, ELCID_RoleWare_GetSilver);
		pRole->GetCurMgr().IncBagSilver(pRecv->n64SilverGet, ELCID_RoleWare_GetSilver);
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// ��ɫ�ֿ��д�Ԫ��
//------------------------------------------------------------------------
//DWORD PlayerSession::HandleRoleSaveYuanBao(tagNetCmd* pCmd)
//{
//	MGET_MSG(pRecv, pCmd, NC_SaveYuanBao);
//
//	// ��ȡ����
//	Role* pRole = GetRole();
//	if(!P_VALID(pRole))
//	{
//		return GT_INVALID;
//	}
//
//	// ��Ǯ�Ϸ��Լ��
//	if(pRecv->nYuanBaoSave <= 0
//		|| pRecv->nYuanBaoSave > pRole->GetCurMgr().GetBagYuanBao() 
//		|| pRecv->nYuanBaoSave > pRole->GetCurMgr().GetCanIncWareYuanBao())
//	{
//		return GT_INVALID;
//	}
//
//	tagNS_SaveYuanBao send;
//	send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
//	if(E_Success == send.dwErrorCode)
//	{
//		pRole->GetCurMgr().DecBagYuanBao(pRecv->nYuanBaoSave, ELCID_RoleWare_SaveYuanBao);
//		pRole->GetCurMgr().IncWareYuanBao(pRecv->nYuanBaoSave, ELCID_RoleWare_SaveYuanBao);
//	}
//
//	SendMessage(&send, send.dwSize);
//
//	return send.dwErrorCode;
//}

//------------------------------------------------------------------------
// ��ɫ�ֿ��д�ȡ��Ǯ&Ԫ��
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetYuanBao(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetYuanBao);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��Ǯ�Ϸ��Լ��
	if(pRecv->nYuanBaoGet <= 0
		|| pRecv->nYuanBaoGet > pRole->GetCurMgr().GetBaiBaoYuanBao() 
		|| pRecv->nYuanBaoGet > pRole->GetCurMgr().GetCanIncBagYuanBao())
	{
		return GT_INVALID;
	}

	pRole->GetCurMgr().DecBaiBaoYuanBao(pRecv->nYuanBaoGet, ELCID_RoleWare_GetYuanBao);
	pRole->GetCurMgr().IncBagYuanBao(pRecv->nYuanBaoGet, ELCID_RoleWare_GetYuanBao);

	//tagNS_GetYuanBao send;
	//send.dwErrorCode = E_Success;
	//SendMessage(&send, send.dwSize);

	return E_Success;
}

//------------------------------------------------------------------------
// ʹ��ĥʯ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleAbrase(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Abrase);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_Abrase send;
	send.dwErrorCode = pRole->AbraseWeapon(pRecv->n64AbraserSerial, pRecv->n64WeaponSerial, send.nAttackTimes);
	send.n64WeaponSerial = pRecv->n64WeaponSerial;

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}