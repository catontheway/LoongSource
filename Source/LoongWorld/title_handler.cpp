//-----------------------------------------------------------------------------
//!\file title_handler.cpp
//!\author xlguo
//!
//!\date 2009-01-20
//! last 2009-02-02
//!
//!\brief �ƺ���Ϣ�������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/msg_role_title.h"


#include "player_session.h"
#include "unit.h"
#include "role.h"
#include "role_mgr.h"
#include "title_mgr.h"
//-----------------------------------------------------------------------------
// ���ý�ɫʹ��ĳ�ƺ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleActiveTitle(tagNetCmd* pCmd)
{
	tagNC_ActiveRoleTitle* pRecv = (tagNC_ActiveRoleTitle*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;
	
	// ֻ�����ñ���ɫ
	if (GetRole()->GetID() != pRecv->dwRoleID)
		return GT_INVALID;

	// ����ƺ�
	TitleMgr* pTitleMgr = GetRole()->GetTitleMgr();
	DWORD dwRtv = pTitleMgr->ActiviteTitle(pRecv->u16TitleID);


	// ���ͽ��
	tagNS_ActiveRoleTitle send;
	send.dwRoleID = GetRole()->GetID();
	send.u16TitleID = pTitleMgr->GetActiviteTitle();
	send.dwErrorCode = dwRtv;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------
// �����ý�ɫӵ�е����гƺ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetTitles(tagNetCmd* pCmd)
{
	tagNC_GetRoleTitles* pRecv = (tagNC_GetRoleTitles*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	Role* pRole = pRecv->dwRoleID == GetRole()->GetID() ? GetRole() : GetOtherInMap(pRecv->dwRoleID);
	if ( !P_VALID(pRole) )
		return 0;

	TitleMgr* pTitleMgr = pRole->GetTitleMgr();

// 	// �鿴Ȩ��
// 	if ( pRole->GetID() != m_pRole->GetID() && !pTitleMgr.Visibility() )
// 	{	// ���ɷ���
// 		tagNS_GetRoleTitles send;
// 		send.dwErrorCode = E_Title_NotVisible;
// 		send.dwRoleID = pRecv->dwRoleID;
// 		send.u16TitleNum = 0;
// 		SendMessage(&send, send.dwSize);
// 	}
// 	else
// 	{	// �ɷ���
		DWORD dwTitlesSize = pTitleMgr->GetObtainedTitlesNum() * sizeof(UINT16);
		DWORD dwMsgSize = sizeof(tagNS_GetRoleTitles) - 1 + (dwTitlesSize > 0 ? dwTitlesSize : 1);

		ASSERT(dwMsgSize >= sizeof(tagNS_GetRoleTitles));

		MCREATE_MSG(pSend, dwMsgSize, NS_GetRoleTitles);
		//tagNS_GetRoleTitles* pSend;
		UINT16* pu16 = reinterpret_cast<UINT16 *>(pSend->byData);
		DWORD dwRtv = pTitleMgr->GetObtainedTitleIDs(pu16, pSend->u16TitleNum);
		pSend->dwErrorCode = dwRtv;
		pSend->dwRoleID = pRecv->dwRoleID;
		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
//	}

	return 0;
}
