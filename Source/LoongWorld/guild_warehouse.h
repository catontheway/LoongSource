//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_warehouse.h
// author: Sunnee
// actor:
// data: 2009-06-23
// last:
// brief: ���ɲֿ������ -- ������Ʒ����,��ͻ���ͬ��,����¼��Ʒ
//-----------------------------------------------------------------------------
#pragma once

#include "event_mgr.h"
#include "container.h"

struct	tagGuildBase;
class	Guild;
//-----------------------------------------------------------------------------

class GuildWarehouse : public EventMgr<GuildWarehouse>
{
public:
	GuildWarehouse();
	~GuildWarehouse();

	VOID	Init(Guild* pGuild, BOOL bRequest = FALSE);
	VOID	Update();

	BOOL	IsInitOK()	{ return m_bInitOK; }

	// ��������ʱ�������ô�С
	VOID	ReInit();

	// ��ʼ��ʱ����
	VOID	LoadWarehouseItems(const BYTE* pData, INT nItemNum);
	// ���ɽ�ɢʱɾ��������Ʒ
	VOID	RemoveAllItems();

	// �¼�����
	VOID	OnMoveToFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnMove2OtherFunc(DWORD dwSenderID, VOID* pEventMessage);

	// ��ͼ�ϲ��¼�
	DWORD	GetGuildWareInfo(BYTE* pBuff, INT& nItemNum, DWORD& dwLastUpdateTime, INT16& nSzWare, INT32& nSize);
	DWORD	SendGuildWarePriList2Client(Role* p2Role);

	// �ⲿ�ӿ�
	DWORD	AddItem(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB = TRUE);

private:
	// �ƶ�����
	DWORD MoveAllowable(Role* pRole);
	DWORD Move(Role* pRole, INT64 n64Serial, INT16 n16Num, INT16 n16PosDst, DWORD dwCmdID);
	DWORD Move2Other(Role* pRole, EItemConType eConTypeSrc, INT64 n64Serial1, 
		EItemConType eConTypeDst, INT16 n16PosDst, DWORD dwCmdID);

private:
	// ���ݿ�ͨѶ
	VOID InsertItem2DB(tagItem &item);
	VOID DeleteItemFromDB(INT64 n64Serial, INT32 dwTypeID);
	VOID SaveUpdateItem2DB();

private:
	// �ͻ���ͨѶ
	VOID SendMessage(Role* pRole, LPVOID pMsg, DWORD dwSize);

private:
	// ��¼Log
	BOOL IsItemNeedLog(const tagItem &item) const { return item.pProtoType->bNeedLog; }
	__forceinline VOID LogItem(DWORD dwRoleID, const tagItem &item1, const tagItem *pItem2, INT16 n16OptNum, DWORD dwCmdID);

private:
	static	VOID RegisterGuildWareEventFunc();

private:
	// ����ʱ�����
	VOID SetUpdateTime(INT16 nPos);				// ��¼��Ӧ��λ��������ʱ��

private:
	BOOL				m_bInitOK;
	Guild*				m_pGuild;

	ItemContainer*		m_pContainer;			// �ŵ����ɲֿ��е���Ʒ,����Ϊ����(m_dwOwnerID <==> GuildID)

	// ����ʱ�����
	DWORD				m_dwLastSaveTime;		// ��󱣴�ʱ��
	DWORD				m_dwGuildWareTime;		// ���ɲֿ�������ʱ��
	std::vector<DWORD>	m_vecUpdateTime;		// �ֿ�ÿ����λ��������ʱ��
};