//-----------------------------------------------------------------------------
//!\file move_handler.cpp
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief �ƶ���Ϣ������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_pk.h"

#include "player_session.h"
#include "role.h"
#include "map.h"

//-----------------------------------------------------------------------------
// ����/�ر�PK����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSafeGuard(tagNetCmd* pCmd)
{
	tagNC_SafeGuard* pRecv = (tagNC_SafeGuard*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT nRet = E_Success;

	if( pRecv->bOpen )
		nRet = pRole->OpenSafeGuard(pRecv->n64ItemID);
	else
		nRet = pRole->CloseSafeGuard();

	// ���ظ��ͻ���
	tagNS_SafeGuard send;
	send.bOpen = pRecv->bOpen;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	return 0;
}

//----------------------------------------------------------------------------
// ��������״̬
//----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetPK(tagNetCmd* pCmd)
{
	tagNC_PK* pRecv = (tagNC_PK*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT nRet = pRole->SetPK(pRecv->bOnlyPK);

	// ���ظ��ͻ���
	tagNS_PK send;
	send.dwErrorCode = nRet;
	send.bReset = FALSE;
	send.bOnlyPK = pRecv->bOnlyPK;
	SendMessage(&send, send.dwSize);

	return 0;
}