#pragma once
#include "../WorldDefine/chat_define.h"
#include "../WorldDefine/chat.h"
#include "../WorldDefine/msg_show_item.h"
#include "channel_mgr.h"


//-------------------------------------------------------------------------
// ��Ϣ����
//-------------------------------------------------------------------------
class MsgCmd
{
public:
	//---------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------
	virtual ~MsgCmd(){}

	//---------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------
	static MsgCmd*		CreateMsgCmd(PVOID pMsg, Role* pSender);
	static VOID DeleteMsgCmd(MsgCmd* &pMsgCmd);

	//---------------------------------------------------------------------
	// �ӿ�
	//---------------------------------------------------------------------
	virtual DWORD		Parse()=0;
	
	//---------------------------------------------------------------------
	// һЩԪ��Ϣ
	//---------------------------------------------------------------------
	tagNetCmd*			GetSendMsg()		const	{	return m_pSend;			}
	tagNetCmd*			GetRecvMsg()		const	{	return m_pRecv;			}
	DWORD				GetReceiverID()		const	{	return m_dwDstRoleID;	}
	ESendChatChannel	GetChannelType()	const	{	return m_eChatTag;		}
	DWORD				GetContentSize()	const	{	return m_dwContentSize;	}
	BYTE*				GetContent()		const	{	return m_pByContent;	}
	Role*				GetSender()			const	{	return m_pSender;		}
	VOID				SetSender(Role* pSender)	{	m_pSender = pSender;	}

protected:
	MsgCmd(tagNetCmd* pRecv);

	DWORD			m_dwDstRoleID;			// �ռ���id
	ESendChatChannel	m_eChatTag;			// Ƶ������

	Role*			m_pSender;				// ������
	DWORD			m_dwContentSize;		// ��Ϣ���ݴ�С
	BYTE*			m_pByContent;			// ��Ϣ����

	tagNetCmd*		m_pRecv;				// ԭʼ��Ϣ
	tagNetCmd*		m_pSend;				// Ҫ���͵���Ϣ

	BYTE			m_byBuffer[1024 * 10];	// ������Ϣ������
};

//-------------------------------------------------------------------------
// ������Ϣ����
//-------------------------------------------------------------------------
class ChatCmd : public MsgCmd
{
	friend class MsgCmd;

public:
	virtual DWORD		Parse();

private:
	ChatCmd(tagNetCmd* pMsg)
		:MsgCmd(pMsg){}
};

//-------------------------------------------------------------------------
// չʾװ������
//-------------------------------------------------------------------------
class EquipCmd : public MsgCmd
{	
	friend class MsgCmd;
public:
	virtual DWORD		Parse();

private:
	EquipCmd(tagNetCmd* pMsg)
		:MsgCmd(pMsg){}

	DWORD	m_byContainerType;
};

//-------------------------------------------------------------------------
// չʾ��Ʒ����
//-------------------------------------------------------------------------
class ItemCmd : public MsgCmd
{
	friend class MsgCmd;
public:
	virtual DWORD		Parse();
private:
	ItemCmd(tagNetCmd* pMsg)
		:MsgCmd(pMsg){}
};


//-------------------------------------------------------------------------
// ��Ϣ������
//-------------------------------------------------------------------------
class MsgMgr
{
public:
	DWORD		LoadLeftMsg(DWORD dwRoleID);

	DWORD		HandleChatMsg(PVOID pMsg, Role* pSender);

 	DWORD		HandleLoadLeftMsg(PVOID pMsg);

private:
	//---------------------------------------------------------------------
	// ����
	//---------------------------------------------------------------------
	DWORD		SaveLeftMsg(tagNetCmd* pMsg, DWORD dwMsgSize, DWORD dwRoleID);
	
	
	ChannelMgr	m_ChannelMgr;
};


extern MsgMgr g_msgMgr;