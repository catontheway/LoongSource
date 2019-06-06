/********************************************************************
	created:	2008/12/31
	created:	31:12:2008   9:44
	filename: 	d:\loong_client\Client\SocialEvent.h
	file path:	d:\loong_client\Client
	file base:	SocialEvent
	file ext:	h
	author:		leili
	
	purpose:	����ϵͳ��Ϸ�¼�����
*********************************************************************/
#pragma once

#pragma pack(push, 1)

// ��Ϊ����
struct tagAddFriendEvent : public tagGameEvent
{
	DWORD	roleID;		// ���ID

	tagAddFriendEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};

// ���������
struct tagAddBlackEvent : public tagGameEvent
{
	DWORD	roleID;		// ���ID

	tagAddBlackEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};

// ʹ����������Ʒ
struct tagUseGiftEvent : public tagGameEvent
{
	INT64	giftGlobalID;
	DWORD	giftTypeID;

	tagUseGiftEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, giftGlobalID(GT_INVALID)
		, giftTypeID(GT_INVALID)
	{ } 
};

// �鿴�����Ϣ
struct tagDispPlayerState : public tagGameEvent
{
    DWORD   dwRoleID;     //!< ���ID

    tagDispPlayerState(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame)
        , dwRoleID(GT_INVALID) {}
};

#pragma pack(pop)