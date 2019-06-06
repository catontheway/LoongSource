//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: return_roleselect_handler.cpp
// author: twan
// actor:
// data: 2009-08-26
// last:
// brief: �ͻ��˷��ؽ�ɫѡ��ʱ�������֮�����Ϣ����
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "player_session.h"
#include "map_creator.h"
#include "role.h"
#include "../WorldDefine/msg_return_roleselect.h"
#include "world_session.h"
#include "role_mgr.h"
//--------------------------------------------------------------------------
// �ͻ��˷��ص���ɫѡ�����
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleReturnRoleSelect(tagNetCmd* pCmd)
{
	//	��ȡ��ɫ
	Role* pRole = GetRole();
	tagNS_ReturnRoleSelect send;

	if(!P_VALID(pRole))
	{
		send.dwErrorCode = GT_INVALID;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}
	
	g_mapCreator.RoleReturnCharacter(pRole->GetID());

	send.dwErrorCode = E_Success;
	SendMessage(&send, send.dwSize);
	
	return E_Success;
}