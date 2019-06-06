//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: class_handler.cpp
// author: zr
// actor:
// data: 2009-09-26
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- ��ɫְҵ���
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "Role.h"
#include "../WorldDefine/role_att.h"

//------------------------------------------------------------------------
// ����ְҵ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleSetClass(tagNetCmd* pCmd)
{
	tagNC_SetClass* pRecv = (tagNC_SetClass*) pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole)) return GT_INVALID;
	
	DWORD dwRet = pRole->RoleSetVocation(pRecv->dwNpcId, pRecv->eClass);

	// ֪ͨ�ͻ���
	tagNS_SetClass ret;
	ret.dwErrCode = (EClassErrorCode)dwRet;
	SendMessage(&ret, ret.dwSize);


	return dwRet;
}

//------------------------------------------------------------------------
// ����ר��ְҵ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleChangeClass(tagNetCmd* pCmd)
{
	tagNC_ChangeClass* pRecv = (tagNC_ChangeClass*) pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole)) return GT_INVALID;

	DWORD dwRet = pRole->ChangeVocation(pRecv->dwNpcId, pRecv->eClass);

	// ֪ͨ�ͻ���
	tagNS_ChangeClass ret;
	ret.dwErrCode = (EClassErrorCode)dwRet;
	SendMessage(&ret, ret.dwSize);

	return dwRet;
}