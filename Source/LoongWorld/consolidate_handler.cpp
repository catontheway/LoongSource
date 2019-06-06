//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: exchange_handler.cpp
// author: Wjiang
// actor:
// data: 2008-08-22
// last:
// brief: װ��ǿ��
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "player_session.h"
#include "../ServerDefine/consolidate_define.h"
#include "../WorldDefine/msg_compose.h"
#include "../WorldDefine/compose_define.h"
#include "map.h"
#include "role.h"



//------------------------------------------------------------------------
// ��Ʒǿ�������ƣ�
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleRolePosyEquip(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ConsolidatePosy);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ������ϸ���
	INT nStuffNum = 1 + ((INT)pRecv->dwSize - sizeof(tagNC_ConsolidatePosy)) / sizeof(INT64);

	DWORD	dwErrorCode = pRole->PosyEquip(pRecv->dwNPCID, pRecv->dwFormulaID, pRecv->n64ItemID, \
												pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_ConsolidatePosy	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��Ʒǿ�����Կ̣�
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleEngraveEquip(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ConsolidateEngrave);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ������ϸ���
	INT nStuffNum = 1 + ((INT)pRecv->dwSize - sizeof(tagNC_ConsolidateEngrave)) / sizeof(INT64);

	DWORD	dwErrorCode = pRole->EngraveEquip(pRecv->dwNPCID, pRecv->dwFormulaID, pRecv->n64ItemID, \
													pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_ConsolidateEngrave	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��Ʒǿ������Ƕ��
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleInlayEquip(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Inlay);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = pRole->InlayEquip(pRecv->n64SrcItemID, pRecv->n64DstItemID, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_Inlay	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��Ʒǿ������ӡ��
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleBrandEquip(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Brand);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = pRole->BrandEquip(pRecv->n64SrcItemID, pRecv->n64DstItemID, pRecv->n64IMID, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_Brand	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLoongSoul(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_LoongSoul);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = pRole->LoongSoulEquip(pRecv->n64SrcItemID, pRecv->n64DstItemID, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_LoongSoul	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleQuench(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ConsolidateQuench);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ������ϸ���
	INT nStuffNum = 1 + ((INT)pRecv->dwSize - sizeof(tagNC_ConsolidateEngrave)) / sizeof(INT64);

	DWORD	dwErrorCode = pRole->QuenchEquip(pRecv->dwNPCID, pRecv->dwFormulaID, pRecv->n64ItemID, 
											pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_ConsolidateQuench	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// �����ϳ���Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleProduceItem(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Produce);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �ҵ��ϳ��䷽����
	const tagProduceProtoSer *pProduceProto = g_attRes.GetProduceProto(pRecv->dwFormulaID);
	if(!P_VALID(pProduceProto))
		return GT_INVALID;

	// ������ϸ���
	INT nStuffNum = 1 + ((INT)pRecv->dwSize - sizeof(tagNC_Produce)) / sizeof(INT64);
	
	DWORD	dwErrorCode = GT_INVALID;
	if(!MIsEquipment(pProduceProto->dwProItemTypeID) )
	{
		dwErrorCode = pRole->ProduceItem(pRecv->dwNPCID, Skill::GetIDFromTypeID(pRecv->dwSkillID), pRecv->n64ItemID, pRecv->dwFormulaID,\
			pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);
	}
	else 
	{
		dwErrorCode = pRole->ProduceEquip(pRecv->dwNPCID, Skill::GetIDFromTypeID(pRecv->dwSkillID), pRecv->n64ItemID, pRecv->dwFormulaID,\
			pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);
	}

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_Produce	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

// Jason 2009-12-6 ����װ��Ǳ��ֵ����Э��
DWORD	PlayerSession::HandleRoleAddEquipPotVal(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_AddPot);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwPotValAdded = 0;
	DWORD	dwErrorCode = GT_INVALID;
	if( MIsEquipment( pRecv->n64DstItemID ) )
	{
		dwErrorCode = pRole->RaiseEquipPotVal( pRecv->n64SrcItemID,pRecv->n64DstItemID,pRecv->n64IMID,dwPotValAdded );
	}

	//������ 0���ɹ��� 1�������� 2��ʧ�� 
	//if( E_Compose_Consolidate_Perfect == dwErrorCode )
	//	dwErrorCode = 1;
	//else if( E_Compose_Consolidate_Success == dwErrorCode )
	//	dwErrorCode = 0;
	//else
	//	dwErrorCode = 2;

	tagNS_AddPot msg;
	msg.dwErrorCode	= dwErrorCode;
	msg.dwPotValue	= dwPotValAdded;
	SendMessage  ( &msg,msg.dwSize );

	return dwErrorCode;
}

//------------------------------------------------------------------------
// �㻯,װ���ϳ�
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDeCompose(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Decomposition)

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �ҵ��ϳ��䷽����
	const tagDeComposeProtoSer *pComposeProto = g_attRes.GetDeComposeProto(pRecv->dwFormulaID);
	if(!P_VALID(pComposeProto))
		return GT_INVALID;

	DWORD	dwErrorCode = GT_INVALID;

	dwErrorCode = pRole->DeComposeItem(pRecv->dwNPCID, Skill::GetIDFromTypeID(pRecv->dwSkillID), pRecv->n64ItemID, pRecv->dwFormulaID,\
			pRecv->n64IMID, pRecv->n64Item, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_Decomposition	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChisel(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Chisel);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = GT_INVALID;

	dwErrorCode = pRole->ChiselEquip(pRecv->n64SrcItemID, pRecv->n64StuffID, pRecv->dwID);

	tagNS_Chisel	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ʱװȾɫ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDyeFashion(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_DyeFashion);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = GT_INVALID;

	dwErrorCode = pRole->DyeFashion(pRecv->n64DyeSN, pRecv->n64EquipSerial, pRecv->dwID);

	tagNS_DyeFashion send;
	send.dwErrCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}


DWORD PlayerSession::HandleGemRemoval(tagNetCmd * pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_GemRemoval);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = GT_INVALID;

	dwErrorCode = pRole->RemoveGemFromEquip(pRecv->dwNPCID, pRecv->n64EquipID, pRecv->n16GemNum,pRecv->n64GemRomovalSign);

	tagNS_GemRemoval send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}


DWORD PlayerSession::HandleGetGemRemovalInfo(tagNetCmd * pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_GetGemRemovalInfo);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = GT_INVALID;

	tagNS_GetGemRemovalInfo send;
	INT8 num  = 0;

	dwErrorCode = pRole->GetGemRemovalInfo(pRecv->dwNPCID, pRecv->n64EquipID, num,send.dwGemIDs);
	send.n16GemNum = num;

	send.dwErrCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

