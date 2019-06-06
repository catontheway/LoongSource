//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: social_mgr.h
// author: wjiang
// actor:
// data: 2008-12-03
// last:
// brief: �¼�������
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "event_mgr.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/ItemDefine.h"

class SocialMgr : public EventMgr<SocialMgr>
{
public:
	BOOL	Init();
	~SocialMgr() {}
	VOID	Update();
	
	// �������
	VOID	OnMakeFriendFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnCancelFriendFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnUpdateFriGrpFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnInsertBlkListFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnDeleteBlkListFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnSendGiftFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnSendGiftReplyFunc(DWORD dwSenderID, VOID* pEventMessage);

	// ͬ��tagRoleInfo����ҵȼ�
	VOID	OnSynRoleLevelFunc(DWORD dwSenderID, VOID* pEventMessage);

	VOID	SendLogoutToFriend(Role *pRole);
	VOID	SendLoginToFriend(Role *pRole);
private:
	static	VOID RegisterSocialEventFunc();

private:
	BYTE	CancelFriend(Role *pSrcRole, DWORD dwSrcRoleID, DWORD dwDestRoleID);
	VOID	UpdateGiftVec();								
	VOID	SendAllFriendMessage(Role *pRole, VOID *pMsg, DWORD dwSize);
	DWORD	CanSendGift(Role* pSrcRole, Role* pDestRole, DWORD dwSrcRoleID, DWORD dwDestRoleID, INT64 n64ItemID, tagItem* &pItem);

private:
	vector<tagSendGift>			vecSendGift;			// ���͵�����
};

extern SocialMgr g_socialMgr;