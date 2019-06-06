//-----------------------------------------------------------------------------
//!\file vcard_handler.cpp
//!\author xlguo
//!
//!\date 2009-02-03
//! last 2008-02-03
//!
//!\brief ��ɫ�����������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "player_session.h"
#include "role_mgr.h"
#include "role.h"
#include "vcard.h"
#include "..\WorldDefine\msg_role_vcard.h"
#include "..\ServerDefine\msg_vcard.h"

//-----------------------------------------------------------------------------
// ��ý�ɫ����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetVCard(tagNetCmd* pCmd)
{

	MGET_MSG(pRecv, pCmd, NC_GetRoleVCard);
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	// �����
	if(pRecv->dwRoleID == GetRole()->GetID())
		GetRole()->GetVCard().SendVCard2Client(GetRole()->GetID());
	// �������
	else
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		// �������
		if (P_VALID(pRole) )
		{
			tagRoleVCard* pVCard = &pRole->GetVCard();
			// �����ɼ�
			if(pVCard->customVCard.bVisibility)
				pVCard->SendVCard2Client(GetRole()->GetID());
			// �������ɼ�
			else
				GetRole()->GetVCard().NotifyClientGetVCard(pRecv->dwRoleID, E_VCard_NotVisible);
		}
		// �������
		else
			GetRole()->GetVCard().SendLoadOffLineVCard(pRecv->dwRoleID, GetRole()->GetID());
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ���ý�ɫ�����Զ�����Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetVCard(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetRoleVCard);
	if( !P_VALID(m_pRole) ) return GT_INVALID;
	
	if( pRecv->dwRoleID != GetRole()->GetID())
	{	// �޸ı�Ȩ��
		GetRole()->GetVCard().NotifyClientSetVCard(pRecv->dwRoleID, E_VCard_NoPrivilege);
		return GT_INVALID;
	}

	tagRoleVCard* pVCard = &GetRole()->GetVCard();

	// ��������
	pVCard->SetCustomData(&pRecv->customVCardData);

	// �������ݿ�
	pVCard->SendSaveDB();
		
	// ֪ͨ�ͻ���
	pVCard->NotifyClientSetVCard(pRecv->dwRoleID, E_VCard_Success);

	return 0;
}

//-----------------------------------------------------------------------------
// ��ȡ������ҵ�ͷ��url
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetHeadPicUrl(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetRoleHeadPicURL);

	if( !P_VALID(GetRole()) ) return GT_INVALID;

	Role*pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pRole))
		GetRole()->GetVCard().SendNullUrlToMe(pRecv->dwRoleID);
	else
		pRole->GetVCard().SendHeadUrlTo(GetRole()->GetID());
	
	return 0;
}