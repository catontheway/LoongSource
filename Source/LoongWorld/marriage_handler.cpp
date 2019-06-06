//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: marriage_handler.cpp
// author: Sxg
// actor:
// data: 2009-09-10
// last:
// brief: ��ҽ��ͻ������Ϣ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "player_session.h"
#include "map.h"
#include "role.h"
#include "role_mgr.h"
#include "title_mgr.h"
#include "marriage.h"
#include "../WorldDefine/msg_marriage.h"
#include "../WorldDefine/msg_social.h"
#include "../ServerDefine/msg_marriage.h"
#include "../ServerDefine/role_data.h"
//////////////////////////////////////////////////////////////////////////
//�������
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandleGetMarriageReq(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_GetMarriageReq);

	if(GT_INVALID == pRecv->dwDstID)
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		IMSG(_T("Cant't find the role object of the session when tackling the GetMarriageReq message\r\n"));
		return GT_INVALID;
	}

	Role* pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcGetMarriageReq(pTarget, pRecv->dwDstID,pRecv->dwNpcID);
	if(E_Marriage_Accept == dwErrorCode)
	{
		if(P_VALID(pTarget))
		{
			// �������߷���Ϣ
			tagNS_GetMarriageReq send;
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
		tagNS_GetMarriageReqRes send;
		send.dwErrorCode	= dwErrorCode;
		send.dwSrcID		= pRole->GetID();
		send.dwDstID		= pRecv->dwDstID;
		SendMessage(&send, send.dwSize);
	}
	return dwErrorCode;
}

//////////////////////////////////////////////////////////////////////////
//���������ߵķ�����Ϣ ת����������
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandleGetMarriageReqRes(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_GetMarriageReqRes);

	if(GT_INVALID == pRecv->dwDstID)
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		IMSG(_T("Cant't find the role object of the session when tackling the GetMarriageReqRes message\r\n"));
		return GT_INVALID;
	}
	Role *pApplicant = NULL;
	DWORD dwErrorCode = pRole->ProcGetMarriageReqRes(pApplicant, pRecv->dwDstID, pRecv->dwErrorCode);

	// ����Ƿ���Ҫ��������Ϣ
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	//���߶�����Ϣ
	tagNS_GetMarriageReqRes send;
	send.dwSrcID = pRole->GetID();
	send.dwDstID = pRecv->dwDstID;
	send.dwErrorCode = dwErrorCode;
	
	SendMessage(&send, send.dwSize);
	if(P_VALID(pApplicant))
	{
		pApplicant->SendMessage(&send, send.dwSize);
	}

	if(E_Marriage_Accept == dwErrorCode)
	{	
		tagFriend* pFriRole = pRole->GetFriendPtr(pApplicant->GetID());
		if(P_VALID(pFriRole))
		{
			//�ƺ�
			pApplicant->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN, GT_INVALID,	pFriRole->dwFriVal);
			pRole->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN, GT_INVALID,	pFriRole->dwFriVal);
		}

		//����loverid
		pApplicant->UpdateLoverID(pRole->GetID());
		pRole->UpdateLoverID(pApplicant->GetID());

		// ֪ͨ�ͻ���
		tagNS_RemoteChangeSpouse sendBigVis;
		sendBigVis.dwRoleID	= pRole->GetID();
		sendBigVis.dwLoverID = pRole->GetLoverID();
		sendBigVis.bHaveWedding = pRole->IsRoleWedded();
		pRole->GetMap()->SendBigVisTileMsg(pRole, &sendBigVis, sendBigVis.dwSize);

		sendBigVis.dwRoleID	= pApplicant->GetID();
		sendBigVis.dwLoverID = pApplicant->GetLoverID();
		sendBigVis.bHaveWedding = pApplicant->IsRoleWedded();
		pApplicant->GetMap()->SendBigVisTileMsg(pApplicant, &sendBigVis, sendBigVis.dwSize);

		//��db����Ϣ ��¼�����Ϣ
		tagNDBC_RoleGetMarriage send;
		send.dwRoleID1 = pRole->GetID();
		send.dwRoleID2 = pRecv->dwDstID;
		g_dbSession.Send(&send,send.dwSize);

		//���ýű�
		g_ScriptMgr.GetRoleScript()->OnGetMarriage(pApplicant, pApplicant->GetID(), pRole->GetID());
	}
	return dwErrorCode;
}

//////////////////////////////////////////////////////////////////////////
//�������
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandleBreakMarriageReq(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_BreakMarriageReq);

	if(GT_INVALID == pRecv->dwDstID)
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		IMSG(_T("Cant't find the role object of the session when tackling the BreakMarriageReq message\r\n"));
		return GT_INVALID;
	}

	Role* pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcBreakMarriageReq(pTarget, pRecv->dwDstID);
	if(E_Marriage_Accept == dwErrorCode)
	{
		if(P_VALID(pTarget))
		{
			// �������߷���Ϣ
			tagNS_BreakMarriageReq send;
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
		tagNS_BreakMarriageReqRes send;
		send.dwErrorCode	= dwErrorCode;
		send.dwSrcID		= pRole->GetID();
		send.dwDstID		= pRecv->dwDstID;
		SendMessage(&send, send.dwSize);
	}
	return dwErrorCode;
}
//////////////////////////////////////////////////////////////////////////
//�������ķ���
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandleBreakMarriageReqRes(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_BreakMarriageReqRes);

	if(GT_INVALID == pRecv->dwDstID)
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		IMSG(_T("Cant't find the role object of the session when tackling the BreakMarriageReqRes message\r\n"));
		return GT_INVALID;
	}
	Role *pApplicant = NULL;
	DWORD dwErrorCode = pRole->ProcBreakMarriageReqRes(pApplicant, pRecv->dwDstID, pRecv->dwErrorCode);

	// ����Ƿ���Ҫ��������Ϣ
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	//���߶�����Ϣ
	tagNS_BreakMarriageReqRes send;
	send.dwSrcID = pRole->GetID();
	send.dwDstID = pRecv->dwDstID;
	send.dwErrorCode = dwErrorCode;

	pRole->SendMessage(&send, send.dwSize);
	if(P_VALID(pApplicant))
	{
		pApplicant->SendMessage(&send, send.dwSize);
	}

	if(E_Marriage_Accept == dwErrorCode)
	{	
//////////////////////////////////////////////////////////////////////////
		tagFriend* pFriRole = pApplicant->GetFriendPtr(pRole->GetID());
		if(P_VALID(pFriRole))
		{
			//�ƺ�
			pApplicant->GetTitleMgr()->SigRemoveEvent(ETE_MARRY_JOIN, GT_INVALID);
			pRole->GetTitleMgr()->SigRemoveEvent(ETE_MARRY_JOIN, GT_INVALID);
		}
//////////////////////////////////////////////////////////////////////////
		if(P_VALID(pFriRole))
		{
			pFriRole->dwFriVal = 0;
			//���ͻ���ͬ���Ѻö�
			tagNS_UpdateFriValue sendfri;
			sendfri.dwRoleID = pRole->GetID();
			sendfri.nFriVal = 0;
			pApplicant->SendMessage(&sendfri,sendfri.dwSize);

			//�Ѻöȹ���
			pFriRole = pRole->GetFriendPtr(pApplicant->GetID());
			pFriRole->dwFriVal = 0;
			//���ͻ���ͬ���Ѻö�
			sendfri.dwRoleID = pApplicant->GetID();
			sendfri.nFriVal = 0;
			pRole->SendMessage(&sendfri,sendfri.dwSize);
		}
//////////////////////////////////////////////////////////////////////////
		// ֪ͨ�ͻ���
		tagNS_RemoteChangeSpouse sendBigVis;
		sendBigVis.dwRoleID	= pRole->GetID();
		sendBigVis.dwLoverID = GT_INVALID;
		sendBigVis.bHaveWedding = pRole->IsRoleWedded();
		pRole->GetMap()->SendBigVisTileMsg(pRole, &sendBigVis, sendBigVis.dwSize);

		sendBigVis.dwRoleID	= pApplicant->GetID();
		sendBigVis.dwLoverID = GT_INVALID;
		sendBigVis.bHaveWedding = pApplicant->IsRoleWedded();
		pApplicant->GetMap()->SendBigVisTileMsg(pApplicant, &sendBigVis, sendBigVis.dwSize);
//////////////////////////////////////////////////////////////////////////
		//����loverid
		pApplicant->UpdateLoverID(GT_INVALID);
		pRole->UpdateLoverID(GT_INVALID);
//////////////////////////////////////////////////////////////////////////
		//��db����Ϣ�����Ѻö�����
		tagNDBC_UpdateFriVal sendDB;
		sendDB.dwRoleID = (pApplicant->GetID() < pRole->GetID()) ? pApplicant->GetID() : pRole->GetID();
		sendDB.FriendshipSave.dwFriendID = (pApplicant->GetID() > pRole->GetID()) ? pApplicant->GetID() : pRole->GetID();
		sendDB.FriendshipSave.nFriVal = 0;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
//////////////////////////////////////////////////////////////////////////
		//��db����Ϣ ��¼�����Ϣ
		tagNDBC_RoleBreakMarriage send;
		send.dwRoleID1 = pRole->GetID();
		send.dwRoleID2 = pRecv->dwDstID;
		g_dbSession.Send(&send,send.dwSize);
//////////////////////////////////////////////////////////////////////////
		//���ýű�
		g_ScriptMgr.GetRoleScript()->OnBreakMarriage(pApplicant, pApplicant->GetID(), pRole->GetID());

	}
	return dwErrorCode;
}
//////////////////////////////////////////////////////////////////////////
//������������
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandleCelebrateWeddingReq(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_CelebrateWeddingReq);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		IMSG(_T("Cant't find the role object of the session when tackling the CelebrateWeddingReq message\r\n"));
		return GT_INVALID;
	}

	//
	Role *pTarget = NULL;
	Role *pOfficiatorRole = NULL;
	DWORD dwErrorCode = pRole->ProcCelebrateWeddingReq(pTarget,pOfficiatorRole,pRecv->dwOfficiatorNameCrc,pRecv->dwNpcID);

	// ����Ƿ���Ҫ��������Ϣ
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}
	
	//���һ��ok �������˷�������Ϣ
	if(E_Wedding_Success == dwErrorCode)
	{	
		//���ʱ�򽫷���˫����������set������ϵͳ���������
		//���ȴ������˻ظ� ��������ϵͳ�Զ����� ��������������
		g_MarriageMgr.SetApplicantRoleID(pRole->GetID());
		g_MarriageMgr.SetTargetRoleID(pTarget->GetID());

		//��ʱ����������ʼ ��Ҫ�ȴ������˵ķ�����Ϣ
		//����g_MarriageMgr.update�м��3���˵�״̬
		g_MarriageMgr.BegWedding();
		//
		if(P_VALID(pOfficiatorRole))
		{	
			g_MarriageMgr.SetOfficiatorRoleID(pOfficiatorRole->GetID());
			// ת����������
			tagNS_CelebrateWeddingReq send;
			send.dwOfficiatorNameCrc = pRecv->dwOfficiatorNameCrc; 
			pOfficiatorRole->SendMessage(&send, send.dwSize);
		}
		//ϵͳ���ֻ���
		else 
		{
			//ֱ�ӷ���ȥ �ȴ������update��Ϣ
			tagNC_CelebrateWeddingReqRes send;
			send.dwErrorCode = E_Wedding_Success;
			pRole->SendMessage(&send, send.dwSize);
		}
	}
	//�ظ�������
	else if(dwErrorCode != GT_INVALID)
	{
		// ����������
		tagNS_CelebrateWeddingReqRes send;
		send.dwErrorCode = dwErrorCode; 
		pRole->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//////////////////////////////////////////////////////////////////////////
//�����˵ķ���
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandleCelebrateWeddingReqRes(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_CelebrateWeddingReqRes);

	//���������
	Role* pOfficiator = GetRole();
	if(!P_VALID(pOfficiator))
	{
		IMSG(_T("Cant't find the role object of the session when tackling the CelebrateWeddingReqRes message\r\n"));
		return GT_INVALID;
	}

	Role *pApplicant = NULL;
	DWORD dwErrorCode = pOfficiator->ProcCelebrateWeddingReqRes(pApplicant,pRecv->dwErrorCode);

	if(dwErrorCode != GT_INVALID && P_VALID(pOfficiator))
	{
		//ֱ�ӷ���ȥ���ȴ������˷���update��Ϣ
		tagNC_CelebrateWeddingReqRes send;
		send.dwErrorCode = dwErrorCode;
		pApplicant->SendMessage(&send, send.dwSize);
	}
	else
	{
		//���ʾ��������� ���� ���л���ͼ�� Ӧ�ý�������
		g_MarriageMgr.EndWedding();
	}
	return dwErrorCode;
};
/*
//////////////////////////////////////////////////////////////////////////
//����������ʱ����Ϣ�������˷��͡�
//����ϵͳ���ֻ���ʱ����Ϣ�ɻ��������˷��͡�
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandleUpdateWeddingStepReq(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NS_UpdateWeddingStepReq);




}
*/