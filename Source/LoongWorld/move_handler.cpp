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
#include "player_session.h"
#include "role.h"
#include "map.h"
#include "..\WorldDefine\action.h"

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleWalk(tagNetCmd* pCmd)
{
 	tagNC_MouseWalk* pRecv = (tagNC_MouseWalk*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	MoveData::EMoveRet eRet = MoveData::EMR_Success;
	if( pRecv->bSwim )
	{
		eRet = pRole->MoveAction(&MoveData::StartRoleSwim, pRecv->srcPos, pRecv->dstPos);
		//eRet = pRole->GetMoveData().StartRoleSwim(pRecv->srcPos, pRecv->dstPos);
	}
	else
	{
		eRet = pRole->MoveAction(&MoveData::StartRoleWalk, pRecv->srcPos, pRecv->dstPos);
		//eRet = pRole->GetMoveData().StartRoleWalk(pRecv->srcPos, pRecv->dstPos);
	}

	// ����ƶ�ʧ�ܣ����Ϳͻ���ʧ��
	if( MoveData::EMR_Success != eRet )
	{
		tagNS_MoveFailed send;
		send.curPos = pRole->GetCurPos();
		send.faceTo = pRole->GetFaceTo();

		EMoveState eMoveState = pRole->GetMoveState();
		send.bSwim = ( eMoveState == EMS_Swim || eMoveState == EMS_SwimStand );

		SendMessage(&send, send.dwSize);
	}

	return 0;	
}

//-----------------------------------------------------------------------------
// ��Ծ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleJump(tagNetCmd* pCmd)
{
	tagNC_Jump* pRecv = (tagNC_Jump*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	MoveData::EMoveRet eRet = pRole->MoveAction(&MoveData::StartRoleJump, pRecv->srcPos, pRecv->dir);
	//MoveData::EMoveRet eRet = pRole->GetMoveData().StartRoleJump(pRecv->srcPos, pRecv->dir);

	// ����ƶ�ʧ�ܣ����Ϳͻ���ʧ��
	if( MoveData::EMR_Success != eRet )
	{
		tagNS_MoveFailed send;
		send.curPos = pRole->GetCurPos();
		send.faceTo = pRole->GetFaceTo();

		EMoveState eMoveState = pRole->GetMoveState();
		send.bSwim = ( eMoveState == EMS_Swim || eMoveState == EMS_SwimStand );

		SendMessage(&send, send.dwSize);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDrop(tagNetCmd* pCmd)
{
	tagNC_Drop* pRecv = (tagNC_Drop*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	MoveData::EMoveRet eRet = pRole->MoveAction(&MoveData::StartRoleDrop, pRecv->srcPos, pRecv->dir);
	//MoveData::EMoveRet eRet = pRole->GetMoveData().StartRoleDrop(pRecv->srcPos, pRecv->dir);

	// ����ƶ�ʧ�ܣ����Ϳͻ���ʧ��
	if( MoveData::EMR_Success != eRet )
	{
		tagNS_MoveFailed send;
		send.curPos = pRole->GetCurPos();
		send.faceTo = pRole->GetFaceTo();

		EMoveState eMoveState = pRole->GetMoveState();
		send.bSwim = ( eMoveState == EMS_Swim || eMoveState == EMS_SwimStand );

		SendMessage(&send, send.dwSize);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ��ֱ����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleVDrop(tagNetCmd* pCmd)
{
	tagNC_VDrop* pRecv = (tagNC_VDrop*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	MoveData::EMoveRet eRet = pRole->MoveAction(&MoveData::StartRoleVDrop, pRecv->srcPos);
	//MoveData::EMoveRet eRet = pRole->GetMoveData().StartRoleVDrop(pRecv->srcPos);

	// ����ƶ�ʧ�ܣ����Ϳͻ���ʧ��
	if( MoveData::EMR_Success != eRet )
	{
		tagNS_MoveFailed send;
		send.curPos = pRole->GetCurPos();
		send.faceTo = pRole->GetFaceTo();

		EMoveState eMoveState = pRole->GetMoveState();
		send.bSwim = ( eMoveState == EMS_Swim || eMoveState == EMS_SwimStand );

		SendMessage(&send, send.dwSize);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSlide(tagNetCmd* pCmd)
{
	tagNC_Slide* pRecv = (tagNC_Slide*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	MoveData::EMoveRet eRet = pRole->MoveAction(&MoveData::StartRoleSlide, pRecv->srcPos);
	//MoveData::EMoveRet eRet = pRole->GetMoveData().StartRoleSlide(pRecv->srcPos);

	
	// ����ƶ�ʧ�ܣ����Ϳͻ���ʧ��
	if( MoveData::EMR_Success != eRet )
	{
		tagNS_MoveFailed send;
		send.curPos = pRole->GetCurPos();
		send.faceTo = pRole->GetFaceTo();

		EMoveState eMoveState = pRole->GetMoveState();
		send.bSwim = ( eMoveState == EMS_Swim || eMoveState == EMS_SwimStand );

		SendMessage(&send, send.dwSize);
	}

	return 0;
}

//------------------------------------------------------------------------------
// ֹͣ�ƶ�
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleStopWalk(tagNetCmd* pCmd)
{
	tagNC_StopWalk* pRecv = (tagNC_StopWalk*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	MoveData::EMoveRet eRet = MoveData::EMR_Success;
	if( pRecv->bSwim )
	{
		eRet = pRole->MoveAction(&MoveData::StopRoleSwim, pRecv->curPos);
	}
	else
	{
		eRet = pRole->MoveAction(&MoveData::StopRoleMove, pRecv->curPos);
	}

	// ����ƶ�ʧ�ܣ����Ϳͻ���ʧ��
	if( MoveData::EMR_Success != eRet )
	{
		tagNS_MoveFailed send;
		send.curPos = pRole->GetCurPos();
		send.faceTo = pRole->GetFaceTo();

		EMoveState eMoveState = pRole->GetMoveState();
		send.bSwim = ( eMoveState == EMS_Swim || eMoveState == EMS_SwimStand );

		SendMessage(&send, send.dwSize);
	}

	return 0;
}