#pragma once
/*
#include "../WorldDefine/chat_define.h"
#include "state_mgr.h"
//-------------------------------------------------------------------------
// Channel��ʶ
//-------------------------------------------------------------------------
class ChannelTagData
{
	typedef State<DWORD, ESendChatChannel>	TagMask;
public:
	static const INT CHANNEL_NUM	= ECC_End;
	static const INT TAG_NUM		= ESCC_End;

public:
	BOOL	Init(const DWORD* pData);
	BOOL	GetData(DWORD* pData) const;
	BOOL	IsFit(ESendChatChannel eChatTag) const;
	BOOL	SetChannelTag(EChatChannel eChatChannel, ESendChatChannel eChatTag, BOOL bSet);

private:
	VOID	UpdateAllMask();

private:
	TagMask		m_AllMask;
	TagMask		m_ChannelMasks[ECC_End];
};

//-------------------------------------------------------------------------
// ������ʾ������ϵ���������
//-------------------------------------------------------------------------
class ChatData
{
public:
	BOOL	Init(BOOL bhasLeftMsg, const DWORD* pMaskData);
	BOOL	GetData(BOOL &bHasLeftMsg, DWORD* pData, DWORD &dwDataSize);
	BOOL	SetChannelTag(EChatChannel eChatChannel, ESendChatChannel eChatTag, BOOL bSet);
	BOOL	IsFit(ESendChatChannel eChatTag) const;

private:
	BOOL			m_bHasLeftMsg;
	ChannelTagData	m_TagData;
};
*/