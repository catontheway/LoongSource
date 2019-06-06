//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: social_mgr.cpp
// author: wjiang
// actor:
// data: 2008-06-16
// last:
// brief: �������ϵ������
//-------------------------------------------------------------------------------------------------------

#include "StdAfx.h"
#include "social_mgr.h"
#include "player_session.h"
#include "unit.h"
#include "role.h"
#include "role_mgr.h"
#include "db_session.h"
#include "title_mgr.h"

#include "title_mgr.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/msg_social.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/log_cmdid_define.h"

//-------------------------------------------------------------------------------------------------------
// �������ʱ��
//-------------------------------------------------------------------------------------------------------
BOOL SocialMgr::Init()
{
	vecSendGift.clear();
	RegisterSocialEventFunc();
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::Update()
{
	EventMgr<SocialMgr>::Update();
	
	// ���·��͵�����
	UpdateGiftVec();
}

//-------------------------------------------------------------------------------------------------------
// ���·��͵�����
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::UpdateGiftVec()
{
	vector<tagSendGift>::iterator it = vecSendGift.begin();
	while( it != vecSendGift.end())
	{
		--it->nStoreTick;
		// 30��������Ӧ,ɾ�����͵�����
		if(it->nStoreTick < 0 )
		{
			tagNS_SendGiftBroadcast	 send;
			send.dwSrcRoleID = it->dwSrcRoleID;
			send.dwDestRoleID = it->dwDestRoleID;
			send.dwTypeID = it->dwTypeID;
			send.bResult = FALSE;
			g_roleMgr.SendWorldMsg(&send, send.dwSize);
			it = vecSendGift.erase(it);
			continue;
		}
		++it;
	}
}

//-------------------------------------------------------------------------------------------------------
// ע������ദ����
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::RegisterSocialEventFunc()
{
	RegisterEventFunc(EVT_MakeFriend,		&SocialMgr::OnMakeFriendFunc);
	RegisterEventFunc(EVT_CancelFriend,		&SocialMgr::OnCancelFriendFunc);
	RegisterEventFunc(EVT_FriendGrp,		&SocialMgr::OnUpdateFriGrpFunc);
	RegisterEventFunc(EVT_InsertBlkList,	&SocialMgr::OnInsertBlkListFunc);
	RegisterEventFunc(EVT_DeleteBlkList,	&SocialMgr::OnDeleteBlkListFunc);
	RegisterEventFunc(EVT_SendGift,			&SocialMgr::OnSendGiftFunc);
	RegisterEventFunc(EVT_SendGiftRpy,		&SocialMgr::OnSendGiftReplyFunc);
	RegisterEventFunc(EVT_SynRoleLevel,		&SocialMgr::OnSynRoleLevelFunc);
}

//-------------------------------------------------------------------------------------------------------
// �������ʱ������Ϣ������
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::SendLoginToFriend(Role *pRole)
{
	tagNS_LoginToFriend send;
	send.dwRoleID = pRole->GetID();
	SendAllFriendMessage(pRole, &send, send.dwSize);	
}

//-------------------------------------------------------------------------------------------------------
// �����ʱ������Ϣ������
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::SendLogoutToFriend(Role *pRole)
{
	tagNS_LogoutToFriend send;
	send.dwRoleID = pRole->GetID();
	SendAllFriendMessage(pRole, &send, send.dwSize);	
}

//-------------------------------------------------------------------------------------------------------
// �����Ѷȴ���0��˫����ѷ�����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::SendAllFriendMessage(Role *pRole, VOID *pMsg, DWORD dwSize)
{
	if(!P_VALID(pRole))
		return;

	for(INT i = 0; i < MAX_FRIENDNUM; ++i)
	{
		DWORD dwFriendID = pRole->GetFriend(i).dwFriendID;
		DWORD dwFriVal = pRole->GetFriend(i).dwFriVal;
		if(dwFriendID !=  GT_INVALID && dwFriVal > 0)
		{
			Role* pFriend = g_roleMgr.GetRolePtrByID(dwFriendID);
			if(P_VALID(pFriend))
				pFriend->SendMessage(pMsg, dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ��Ӻ���
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::OnMakeFriendFunc(DWORD dwSenderID, VOID *pEventMessage)
{
	tagNC_RoleMakeFriend* pRecv = (tagNC_RoleMakeFriend*)pEventMessage;

	DWORD dwSrcRoleID = dwSenderID;
	DWORD dwDestRoleID = pRecv->dwDestRoleID;
	Role* pDestRole = (Role*)GT_INVALID;
	DWORD dwFriVal = 0;
	BOOL  bOnline = FALSE;
	INT	  nLevel = 1;

	Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);
	
	if(!P_VALID(pSrcRole))
		return;

	DWORD	dwErrorCode = E_Success;

	// ���ܼ��Լ�Ϊ����
	if( dwDestRoleID == dwSrcRoleID )
		dwErrorCode = E_Friend_Target_Not_Sel;	

	// ��ɫID������
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwDestRoleID);

	if(!P_VALID(pRoleInfo))
		dwErrorCode = E_Friend_Target_Not_Exist;
	else
	{
		bOnline = pRoleInfo->bOnline;
		nLevel = pRoleInfo->byLevel;
	}

	// �������ڵ�����޷������Ϊ����
	for(INT i = 0; i < MAX_BLACKLIST; ++i)
	{
		if(pSrcRole->GetBlackList(i) == dwDestRoleID)
			dwErrorCode = E_Friend_Target_Black_List;
	}

	// �ú����Ѵ����ں����б�
	tagFriend *pFriend = pSrcRole->GetFriendPtr(dwDestRoleID);
	if(P_VALID(pFriend))
		dwErrorCode = E_Friend_Target_Already_Exit;

	if(dwErrorCode == E_Success)
	{
		for (INT i = 0; i < MAX_FRIENDNUM; ++i)
		{
			if(pSrcRole->GetFriend(i).dwFriendID == GT_INVALID)
			{
				pSrcRole->SetFriend(i, dwDestRoleID);
				
				tagNDBC_InsertFriend send;
				send.dwRoleID = dwSrcRoleID;
				send.FriendSave.dwFriendID = dwDestRoleID;
				send.FriendSave.nGroupID = 1;
				g_dbSession.Send(&send, send.dwSize);

				pDestRole = g_roleMgr.GetRolePtrByID(dwDestRoleID);
				if(P_VALID(pDestRole))
				{
					// ����Ƿ�Ϊ˫�����
					tagFriend *pFriendDest = pDestRole->GetFriendPtr(dwSrcRoleID);
					if(P_VALID(pFriendDest))
					{
						pSrcRole->SetFriend(i, dwDestRoleID, 1);
						dwFriVal = pFriendDest->dwFriVal += 1;

						tagNDBC_InsertFriVal sendDB;
						sendDB.dwRoleID = (dwSrcRoleID < dwDestRoleID) ? dwSrcRoleID : dwDestRoleID;
						sendDB.FriendshipSave.dwFriendID = (dwSrcRoleID > dwDestRoleID) ? dwSrcRoleID : dwDestRoleID;
						sendDB.FriendshipSave.nFriVal = dwFriVal;
						g_dbSession.Send(&sendDB, sendDB.dwSize);
					}

					tagNS_MakeFriNotice sendNotice;
					sendNotice.dwRoleID = dwSrcRoleID;
					pDestRole->SendMessage(&sendNotice, sendNotice.dwSize);
				}
				
				break;
			}

			// �����б������Ƿ�ﵽ����100��
			if( i == (MAX_ENEMYNUM - 1))
				dwErrorCode = E_Friend_Max_Num;
			
		}
	}

	// �ƺ��¼�����
	pSrcRole->GetTitleMgr()->SigEvent(ETE_FRIEND_MAKE, GT_INVALID, GT_INVALID);

	tagNS_RoleMakeFriend send;
	send.dwDestRoleID = dwDestRoleID;
	send.bOnline = bOnline;
	send.nLevel = nLevel;
	send.dwErrorCode = dwErrorCode;
	pSrcRole->GetSession()->SendMessage(&send, send.dwSize);

	// ���Ѷȸı�,ͬ�����ͻ���
	if(dwFriVal > 0)
	{
		tagNS_UpdateFriValue sendD;
		sendD.dwRoleID = dwDestRoleID;
		sendD.nFriVal = dwFriVal;
		pSrcRole->SendMessage(&sendD, sendD.dwSize);

		tagNS_UpdateFriValue sendS;
		sendS.dwRoleID = dwSrcRoleID;
		sendS.nFriVal = dwFriVal;
		pDestRole->SendMessage(&sendS, sendS.dwSize);
	}
	
	return;
}

//-------------------------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::OnCancelFriendFunc(DWORD dwSenderID, VOID *pEventMessage)
{
	tagNC_RoleCancelFriend* pRecv = (tagNC_RoleCancelFriend*)pEventMessage;

	DWORD dwSrcRoleID = dwSenderID;
	DWORD dwDestRoleID = pRecv->dwDestRoleID;
	BYTE  byGroup = 1;

	Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);

	if(!P_VALID(pSrcRole))
		return;

	DWORD	dwErrorCode = E_Success;
	//˫��Ϊ���ޣ����ɱ�ɾ��
	if(pSrcRole->GetLoverID() == pRecv->dwDestRoleID)
	{
		dwErrorCode = E_Friend_CanNotDel_Spouse;
	}
	//˫��Ϊʦͽ�����ɱ�ɾ�� 
	//˫��Ϊ���£����ɱ�ɾ��

	// ��ɫID������
	tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(dwDestRoleID);

	if(!P_VALID(pRoleInfo))
		dwErrorCode = E_Friend_Target_Not_Exist;

	if(dwErrorCode == E_Success)
	{
		byGroup = CancelFriend(pSrcRole, dwSrcRoleID, dwDestRoleID);
	}

	tagNS_RoleCancelFriend send;
	send.dwDestRoleID = dwDestRoleID;
	send.byGroup = byGroup;
	send.dwErrorCode = dwErrorCode;
	pSrcRole->GetSession()->SendMessage(&send, send.dwSize);
}

BYTE SocialMgr::CancelFriend(Role *pSrcRole, DWORD dwSrcRoleID, DWORD dwDestRoleID)
{
	BYTE  byGroup = 1;
	DWORD dwFriVal = 0;

	for (INT i = 0; i < MAX_FRIENDNUM; ++i)
	{
		if(pSrcRole->GetFriend(i).dwFriendID == dwDestRoleID)
		{
			byGroup = pSrcRole->GetFriend(i).byGroup;
			dwFriVal = pSrcRole->GetFriend(i).dwFriVal;
			pSrcRole->SetFriend(i, GT_INVALID);

			tagNDBC_DeleteFriend send;
			send.dwRoleID = dwSrcRoleID;
			send.dwFriendID = dwDestRoleID;
			g_dbSession.Send(&send, send.dwSize);

			if(dwFriVal > 0)
			{
				// Ϊ�˴洢���ݿⷽ��,СID�ŵ�dwRoleID
				tagNDBC_DeleteFriVal sendFV;
				sendFV.dwRoleID = (dwSrcRoleID < dwDestRoleID) ? dwSrcRoleID : dwDestRoleID;
				sendFV.dwFriendID = (dwSrcRoleID < dwDestRoleID) ? dwDestRoleID : dwSrcRoleID;
				g_dbSession.Send(&sendFV, sendFV.dwSize);

				// �����ɾ���ĺ�������,��������Ѷ�Ϊ0
				Role* pDestRole = g_roleMgr.GetRolePtrByID(dwDestRoleID);

				if(P_VALID(pDestRole))
				{
					tagFriend *pFriend = pDestRole->GetFriendPtr(dwSrcRoleID);
					if(P_VALID(pFriend))
					{
						pFriend->dwFriVal = 0;
						tagNS_UpdateFriValue send;
						send.dwRoleID = dwSrcRoleID;
						send.nFriVal = 0;
						pDestRole->SendMessage(&send, send.dwSize);
					}
				}				
			}

			break;
		}
	}
	return byGroup;
}

//-------------------------------------------------------------------------------------------------------
// ���º��ѷ���
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::OnUpdateFriGrpFunc(DWORD dwSenderID, VOID* pEventMessage)
{
	tagNC_UpdateFriGroup* pRecv = (tagNC_UpdateFriGroup*)pEventMessage;

	DWORD	dwSrcRoleID = dwSenderID;
	DWORD	dwDestRoleID = pRecv->dwDestRoleID;
	BYTE	byNewGroup = pRecv->byGroup;
	BYTE	byOldGroup = 1;

	Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);

	if(!P_VALID(pSrcRole))
		return;

	DWORD	dwErrorCode = E_Success;

	if(byNewGroup < 1 || byNewGroup > 4)
		dwErrorCode = E_Friend_Group_Not_Exit;
	
	if(dwErrorCode == E_Success)
	{
		for (INT i = 0; i < MAX_FRIENDNUM; ++i)
		{
			if(pSrcRole->GetFriend(i).dwFriendID == dwDestRoleID)
			{
				byOldGroup = pSrcRole->GetFriend(i).byGroup;
				pSrcRole->SetFriend(i, dwDestRoleID, 0, byNewGroup);

				tagNDBC_UpdateFriend send;
				send.dwRoleID = dwSrcRoleID;
				send.FriendSave.dwFriendID = dwDestRoleID;
				send.FriendSave.nGroupID = byNewGroup;
				g_dbSession.Send(&send, send.dwSize);
					
				break;
			}

			// ���Ǻ���
			if( i == (MAX_ENEMYNUM - 1))
				dwErrorCode = E_Friend_Not_Friend;
		}
	}
	
	tagNS_UpdateFriGroup send;
	send.dwDestRoleID = dwDestRoleID;
	send.byOldGroup = byOldGroup;
	send.byNewGroup = byNewGroup;
	send.dwErrorCode = dwErrorCode;
	pSrcRole->GetSession()->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ������ҵ�������
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::OnInsertBlkListFunc(DWORD dwSenderID, VOID* pEventMessage)
{
	tagNC_MoveToBlackList* pRecv = (tagNC_MoveToBlackList*)pEventMessage;

	DWORD	dwSrcRoleID = dwSenderID;
	DWORD	dwDestRoleID = pRecv->dwDestRoleID;
	BYTE	byOldGroup = 0;

	Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);

	if(!P_VALID(pSrcRole))
		return;

	DWORD	dwErrorCode = E_Success;

	// ���ܼ��Լ���������
	if( dwDestRoleID == dwSrcRoleID )
		dwErrorCode = E_Black_Targer_Not_Sel;	

	// ��ɫID������
	tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(dwDestRoleID);

	if(!P_VALID(pRoleInfo))
		dwErrorCode = E_Friend_Target_Not_Exist;


	//˫��Ϊ���ޣ����ɱ�ɾ��
	if(pSrcRole->GetLoverID() == pRecv->dwDestRoleID)
	{
		dwErrorCode = E_Friend_CanNotDel_Spouse;
	}
	//˫��Ϊ���ޣ����ɱ�ɾ��
	//˫��Ϊʦͽ�����ɱ�ɾ�� 
	//˫��Ϊ���£����ɱ�ɾ��

	// �������Ƿ����ں�������
	for(INT i = 0; i < MAX_BLACKLIST; ++i)
	{
		if(pSrcRole->GetBlackList(i) == dwDestRoleID)
			dwErrorCode = E_Black_Target_Already_Exit;
	}

	if(dwErrorCode == E_Success)
	{
		for(INT i = 0; i < MAX_BLACKLIST; ++i)
		{
			if(pSrcRole->GetBlackList(i) == GT_INVALID)
			{
				pSrcRole->SetBlackList(i, dwDestRoleID);

				// �������ں����б�����ҴӺ����б�ɾ��
				tagFriend *pFriend = pSrcRole->GetFriendPtr(dwDestRoleID);
				if(P_VALID(pFriend))
				{
					byOldGroup = pFriend->byGroup;
					CancelFriend(pSrcRole, dwSrcRoleID, dwDestRoleID);
				}

				tagNDBC_InsertBlack		send;
				send.dwRoleID = dwSenderID;
				send.dwBlackID = dwDestRoleID;
				g_dbSession.Send(&send, send.dwSize);

				break;
			}

			if(i == (MAX_BLACKLIST - 1))
				dwErrorCode = E_Black_Max_Num;
		}
	}

	tagNS_MoveToBlackList	send;
	send.dwDestRoleID = dwDestRoleID;
	send.byOldGroup = byOldGroup;
	send.dwErrorCode = dwErrorCode;
	pSrcRole->GetSession()->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ����ҴӺ�������ɾ��
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::OnDeleteBlkListFunc(DWORD dwSenderID, VOID* pEventMessage)
{
	tagNC_DeleteBlackList* pRecv = (tagNC_DeleteBlackList*)pEventMessage;

	DWORD	dwSrcRoleID = dwSenderID;
	DWORD	dwDestRoleID = pRecv->dwDestRoleID;

	Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);

	if(!P_VALID(pSrcRole))
		return;

	DWORD	dwErrorCode = E_Success;

	// ��ɫID������
	tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(dwDestRoleID);

	if(!P_VALID(pRoleInfo))
		dwErrorCode = E_Friend_Target_Not_Exist;

	if(dwErrorCode == E_Success)
	{
		for(INT i = 0; i < MAX_BLACKLIST; ++i)
		{
			if(pSrcRole->GetBlackList(i) == dwDestRoleID)
			{
				pSrcRole->SetBlackList(i, GT_INVALID);

				tagNDBC_DeleteBlack		send;
				send.dwRoleID = dwSrcRoleID;
				send.dwBlackID = dwDestRoleID;
				g_dbSession.Send(&send, send.dwSize);

				break;
			}

			if(i == (MAX_BLACKLIST - 1))
				dwErrorCode = E_Black_Target_Not_Exit;
		}
	}

	tagNS_DeleteBlackList	send;
	send.dwDestRoleID = dwDestRoleID;
	send.dwErrorCode = dwErrorCode;
	pSrcRole->GetSession()->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::OnSendGiftFunc(DWORD dwSenderID, VOID* pEventMessage)
{
	tagNC_SendGift* pRecv = (tagNC_SendGift*)pEventMessage;

	DWORD	dwSrcRoleID = dwSenderID;
	DWORD	dwDestRoleID = pRecv->dwDestRoleID;
	INT64	n64ItemID = pRecv->n64ItemID;
	DWORD	dwTypeID = GT_INVALID;
	tagItem *pItem = (tagItem*)GT_INVALID;

	Role	*pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);
	Role	*pDestRole = g_roleMgr.GetRolePtrByID(dwDestRoleID);

	if(!P_VALID(pSrcRole))
		return;

	DWORD	dwErrorCode = E_Success;

	dwErrorCode = CanSendGift(pSrcRole, pDestRole, dwSrcRoleID, dwDestRoleID, n64ItemID, pItem);

	if(dwErrorCode == E_Success)
	{
		// ɾ������
		dwTypeID = pItem->dwTypeID;
		pSrcRole->GetItemMgr().DelFromBag(n64ItemID, (DWORD)ELCLD_Send_Gift, (INT16)1);

		// �����������
		tagSendGift SendGift(dwSrcRoleID, dwDestRoleID, dwTypeID);
		vecSendGift.push_back(SendGift);

		tagNS_SendGiftToFri send;
		send.dwSrcRoleID = dwSrcRoleID;
		send.dwTypeID = dwTypeID;
		pDestRole->GetSession()->SendMessage(&send, send.dwSize);
	}

	tagNS_SendGiftToSender send;
	send.dwErrorCode = dwErrorCode;
	pSrcRole->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// �Ƿ��������
//-------------------------------------------------------------------------------------------------------
DWORD SocialMgr::CanSendGift(Role* pSrcRole, Role* pDestRole, DWORD dwSrcRoleID, DWORD dwDestRoleID, INT64 n64ItemID, tagItem* &pItem)
{
	// �жϺ���ID�Ƿ�����
	if(!P_VALID(pDestRole))
		return E_Gift_Friend_Not_Online;

	// ��ɫID������
	tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(dwDestRoleID);

	if(!P_VALID(pRoleInfo))
		return E_Friend_Target_Not_Exist;
	
	// �����Ƿ��ڱ�����
	pItem = pSrcRole->GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pItem))
		return  E_Gift_Not_Exit;

	// ����Ƿ�Ϊ����
	if(pItem->pProtoType->eSpecFunc != EISF_FriendGift)
		return E_Gift_Not_Gift;

	// ֻ���������������
	tagFriend *pSrcFriend = pSrcRole->GetFriendPtr(dwDestRoleID);
	tagFriend *pDestFriend = pDestRole->GetFriendPtr(dwSrcRoleID);
	if(!P_VALID(pSrcFriend) || !P_VALID(pDestFriend))
		return E_Gift_Not_Both_Friend;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �յ������ظ�
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::OnSendGiftReplyFunc(DWORD dwSenderID, VOID* pEventMessage)
{
	tagNC_SendGiftReply* pRecv = (tagNC_SendGiftReply*)pEventMessage;

	DWORD		dwSrcRoleID = pRecv->dwSrcRoleID;
	DWORD		dwDestRoleID = dwSenderID;
	DWORD		dwTypeID = pRecv->dwTypeID;
	BOOL		bResult = pRecv->bResult;
	tagFriend	*pSrcFriend = (tagFriend*)GT_INVALID;
	tagFriend   *pDestFriend = (tagFriend*) GT_INVALID;

	Role		*pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);
	Role		*pDestRole = g_roleMgr.GetRolePtrByID(dwDestRoleID);

	// ��ҽ�������
	if(bResult)
	{
		if(P_VALID(pSrcRole)) 
			pSrcFriend = pSrcRole->GetFriendPtr(dwDestRoleID);
		if(P_VALID(pDestRole))
			pDestFriend = pDestRole->GetFriendPtr(dwSrcRoleID);


		// ��ѯ�������
		BOOL	bSendGift = FALSE;					//	����������Ƿ�����Ӧ������
		vector<tagSendGift>::iterator it = vecSendGift.begin();
		while(it != vecSendGift.end())
		{
			if(it->dwSrcRoleID == dwSrcRoleID && it->dwDestRoleID == dwDestRoleID && it->dwTypeID == dwTypeID)
			{
				bSendGift = TRUE;
				it = vecSendGift.erase(it);
				continue;
			}
			++it;
		}

		if(!bSendGift)
			return;

		// �����˫����ѣ�����˫�����Ѷ�
		tagItemProto *pItemProto = g_attRes.GetItemProto(dwTypeID);
		if(!P_VALID(dwTypeID))
			return;

		BOOL bInsert = TRUE;
		DWORD dwFriVal = 0;
		if(P_VALID(pSrcFriend))
		{
			if(pSrcFriend->dwFriVal > 0)
				bInsert = FALSE;

			dwFriVal = pSrcFriend->dwFriVal += pItemProto->nSpecFuncVal1;

			tagNS_UpdateFriValue send;
			send.dwRoleID = dwDestRoleID;
			send.nFriVal = dwFriVal;
			pSrcRole->SendMessage(&send, send.dwSize);

			//���³ƺ�	
			if(pSrcRole->GetLoverID() == pSrcFriend->dwFriendID)
				pSrcRole->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN,  GT_INVALID, dwFriVal);
		}

		if(P_VALID(pDestFriend))
		{
			if(pDestFriend->dwFriVal > 0)
				bInsert = FALSE;

			dwFriVal = pDestFriend->dwFriVal += pItemProto->nSpecFuncVal1;

			tagNS_UpdateFriValue send;
			send.dwRoleID = dwSrcRoleID;
			send.nFriVal = dwFriVal;
			pDestRole->SendMessage(&send, send.dwSize);

			//���³ƺ�
			if(pDestRole->GetLoverID() == pDestFriend->dwFriendID)
				pDestRole->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN,  GT_INVALID, dwFriVal);
		}

		if(bInsert)
		{
			tagNDBC_InsertFriVal sendDB;
			sendDB.dwRoleID = (dwSrcRoleID < dwDestRoleID) ? dwSrcRoleID : dwDestRoleID;
			sendDB.FriendshipSave.dwFriendID = (dwSrcRoleID > dwDestRoleID) ? dwSrcRoleID : dwDestRoleID;
			sendDB.FriendshipSave.nFriVal = dwFriVal;
			g_dbSession.Send(&sendDB, sendDB.dwSize);
		}
		else{
			tagNDBC_UpdateFriVal sendDB;
			sendDB.dwRoleID = (dwSrcRoleID < dwDestRoleID) ? dwSrcRoleID : dwDestRoleID;
			sendDB.FriendshipSave.dwFriendID = (dwSrcRoleID > dwDestRoleID) ? dwSrcRoleID : dwDestRoleID;
			sendDB.FriendshipSave.nFriVal = dwFriVal;
			g_dbSession.Send(&sendDB, sendDB.dwSize);
		}
	}
	
	// modfiy by dch
	// ����Ĵ������Ƿ�Ҫ�㲥
	INT nIsBroadcast(0);
	tagItemProto *pItemProto = g_attRes.GetItemProto(dwTypeID);
	if(!P_VALID(dwTypeID))
		return;

	nIsBroadcast = pItemProto->nSpecFuncVal2;
	if ( 1 == nIsBroadcast)
	{
		tagNS_SendGiftBroadcast	 send;
		send.dwSrcRoleID = dwSrcRoleID;
		send.dwDestRoleID = dwDestRoleID;
		send.dwTypeID = dwTypeID;
		send.bResult = bResult;
		g_roleMgr.SendWorldMsg(&send, send.dwSize);
	}
	
}

//-------------------------------------------------------------------------------------------------------
// ͬ��tagRoleInfo�е���ҵȼ�
//-------------------------------------------------------------------------------------------------------
VOID SocialMgr::OnSynRoleLevelFunc(DWORD dwSenderID, VOID* pEventMessage)
{
	tagNS_RoleLevelChange* pRecv = (tagNS_RoleLevelChange*)pEventMessage;

	tagRoleInfo	*pRoleInfo = g_roleMgr.GetRoleInfo(dwSenderID);
	if(P_VALID(pRoleInfo))
	{
		pRoleInfo->byLevel = pRecv->nLevel;
	}
}


SocialMgr g_socialMgr;






