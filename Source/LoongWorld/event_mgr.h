//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: event_mgr.cpp
// author: wjiang
// actor:
// data: 2008-12-02
// last:
// brief: �¼�������
//-------------------------------------------------------------------------------------------------------

#pragma once

#include "mutex.h"

//-------------------------------------------------------------------------------------------------------
// �¼�����
//-------------------------------------------------------------------------------------------------------
enum  EEventType
{
	EVT_NULL				=	-1,

	//---------------------------------------------------------
	// �������
	//---------------------------------------------------------
	EVT_MakeFriend			=	0,
	EVT_CancelFriend		=	1,
	EVT_FriendGrp			=	2,
	EVT_InsertBlkList		=	3,
	EVT_DeleteBlkList		=	4,
	EVT_SendGift			=	5,
	EVT_SendGiftRpy			=	6,

	//---------------------------------------------------------
	// �������
	//---------------------------------------------------------
	EVT_JoinTeam			=	7,
	EVT_JoinTeamRepley		=	8,
	EVT_KickMember			=	9,
	EVT_LeaveTeam			=	10,
	EVT_SetPickMol			=	11,
	EVT_ChangeLeader		=	12,
	EVT_ChangeLevel			=	13,
	EVT_ChangeRein			=	14,

	EVT_SynRoleLevel		=   16,

	EVT_AddAllRoleBuff		=	17,

	EVT_GuildWareMoveTo		=	18,	
	EVT_GuildWareMove2Other	=	19,

	//----------------------------------------------------------	
	// VIP̯λ���	
	//----------------------------------------------------------	
	EVT_SetStallTitle		=	20,		// ����̯λ����	
	EVT_SetStallStatus		=	21,		// ����̯λ״̬	
	EVT_ChangeStallGoods	=	22,		// ���̯λ��Ʒ	

	//----------------------------------------------------------	
	// �ű����	
	//----------------------------------------------------------	
	EVT_Script_Reload		=	101,
	EVT_CreateTeam			=	102,
	EVT_AddMember			=	103,

	//����������Э��
	// Jason 2009-11-25
	EVT_ApplyToJoinTeam		= 104,
	EVT_ApplyReply			= 105,

	EVT_End,
};

//--------------------------------------------------------------------------------------------------------
// ��Ϸ���¼�
//--------------------------------------------------------------------------------------------------------
template<class T>
class EventMgr;

class EventObj
{
	template<class T> friend class EventMgr;

private:
	EventObj(DWORD dwSender, VOID* pMsg, EEventType eType, DWORD dwSize)
	{
		// ��ʼ��
		m_dwSender = dwSender;
		m_dwSize = dwSize;
		m_eType = eType;

		if( 0 == dwSize )
		{
			m_pMsg = NULL;
		}
		else
		{
			m_pMsg = new BYTE[dwSize];
			memcpy(m_pMsg, pMsg, dwSize);
		}
	}

	~EventObj() { SAFE_DEL_ARRAY(m_pMsg); }

	DWORD		GetSender() { return m_dwSender; }
	VOID*		GetEvent() { return m_pMsg; }
	DWORD		GetEventSize() { return m_dwSize; }
	EEventType	GetEventType() { return m_eType; }

private:
	DWORD				m_dwSender;
	VOID*				m_pMsg;
	EEventType			m_eType;
	DWORD				m_dwSize;
};

//-------------------------------------------------------------------------------------------------------
// ����ĳ�Ա����ֻ�������̵߳��ã�AddEvent���⣩
//-------------------------------------------------------------------------------------------------------
template<class T>
class EventMgr
{
public:
	typedef VOID (T::*EVENTMESSAGEFUNC)(DWORD dwSender, VOID* pMsg);
	typedef std::map<EEventType, EVENTMESSAGEFUNC>	EventFuncMap;

public:
	BOOL Init();
	~EventMgr();

	VOID Update();		
	VOID AddEvent(DWORD dwSender, EEventType eType, DWORD dwSize, VOID* pMsg);

protected:
	static VOID RegisterEventFunc(EEventType EventType, EVENTMESSAGEFUNC EventMessageFunc);

private:
	TList<EventObj*>				m_listEventObj;	
	Mutex							m_Lock;
	static EventFuncMap				m_mapEventFunc;	// �¼����������ͼ���Ӧ�Ĵ�����
};

template<class T>
typename EventMgr<T>::EventFuncMap EventMgr<T>::m_mapEventFunc; 

//-------------------------------------------------------------------------------------------------------
// ע���¼�������
//-------------------------------------------------------------------------------------------------------
template<class T>
VOID EventMgr<T>::RegisterEventFunc(EEventType eEventType, EVENTMESSAGEFUNC EventMessageFunc)
{
	m_mapEventFunc.insert(make_pair(eEventType,	EventMessageFunc));
}

//-------------------------------------------------------------------------------------------------------
// �¼��������ʼ��
//-------------------------------------------------------------------------------------------------------
template<class T>
BOOL EventMgr<T>::Init()
{
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
template<class T>
EventMgr<T>::~EventMgr()
{
	EventObj* pEvent = m_listEventObj.PopFront();
	while( P_VALID(pEvent) )
	{
		SAFE_DEL(pEvent);
		pEvent = m_listEventObj.PopFront();
	}
}


//-------------------------------------------------------------------------------------------------------
// �������¼�����
//-------------------------------------------------------------------------------------------------------
template<class T>
VOID EventMgr<T>::Update()
{
	EventObj* pOjb = m_listEventObj.PopFront();
	while( P_VALID(pOjb) )
	{
		EventFuncMap::iterator it = m_mapEventFunc.find(pOjb->GetEventType());
		if( it != m_mapEventFunc.end() )
		{
			EVENTMESSAGEFUNC handler = it->second;
			(((T*)this)->*handler)(pOjb->GetSender(), pOjb->GetEvent());
		}
		SAFE_DEL(pOjb);

		pOjb = m_listEventObj.PopFront();
	}
}

//-------------------------------------------------------------------------------------------------------
// �����¼�
//-------------------------------------------------------------------------------------------------------
template<class T>
VOID EventMgr<T>::AddEvent(DWORD dwSender, EEventType eType, DWORD dwSize, VOID* pMsg)
{
	ASSERT( eType > EVT_NULL && eType < EVT_End );

	EventObj* pObj = new EventObj(dwSender, pMsg, eType, dwSize);

	m_Lock.Acquire();
	m_listEventObj.PushBack(pObj);
	m_Lock.Release();
}
