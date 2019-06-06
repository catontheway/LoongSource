//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_purchase.h
// author: sunnee
// actor:
// data: 2009-07-13
// last:
// brief: �����Ź���
//-------------------------------------------------------------------------------------------------------
#pragma once

struct tagGroupPurchase;
struct tagSimGPInfo;
struct tagMallItemSell;
struct tagItem;
class Guild;

class GuildPurchase
{
public:
	GuildPurchase();
	~GuildPurchase();

	BOOL Init(DWORD dwGuildID);
	VOID Update(DWORD dwTime);
	VOID Destory();

	// ��Ϣ����
	BOOL	Add(tagGroupPurchase* pGPInfo, BOOL bNotify2DB = TRUE);
	BOOL	Remove(tagGroupPurchase* pGPInfo, BOOL bNotify2DB = TRUE);
	BOOL	IsEmpty()	{ return m_mapGPInfo.Empty(); }

	// ��ȡ�Ź���Ϣ
	DWORD	GetAllSimGPInfo(INT &nGPInfoNum, tagSimGPInfo* pData);
	DWORD	GetParticipators(DWORD dwID, DWORD dwRoleID, DWORD *pData);

	INT		GetGroupPurchaseInfoNum();
	INT		GetParticipatorNum(DWORD dwID, DWORD dwRoleID);

	// ����/��Ӧ�Ź�
	DWORD	LaunchGroupPurchase(Role *pRole, DWORD dwID, BYTE byScope,
		BYTE byIndex, INT nUnitPrice, OUT tagGroupPurchase* &pGPInfo, OUT DWORD& dwItemTypeID);
	DWORD	RespondGroupPurchase(Role *pRole, DWORD dwID, DWORD dwRoleID,
		INT nPrice, OUT tagGroupPurchase* &pGPInfo);

	// �����Ź���Ʒ
	DWORD	CreateGPItems(DWORD dwID, OUT tagMallItemSell &itemSell);

	// ɾ���Ź���Ϣ
	VOID	RemoveGroupPurchaseInfo(tagGroupPurchase* &pGPInfo, BOOL bSuccess = TRUE);
	VOID	RemoveGroupPurchaseInfo();

	// �������Ԫ��
	VOID	ReturnCost2Participator(tagGroupPurchase* pGPInfo);

	// ������Ӧ���б�
	VOID	UpdateGPInfo2DB(tagGroupPurchase* pGPInfo);

	// �Ź��ɹ�����
	DWORD	DoSuccessStuff(tagGroupPurchase* pGPInfo);

private:
	// ���ݿ����
	VOID AddGPInfo2DB(tagGroupPurchase* pGPInfo);
	VOID RemoveGPInfo2DB(tagGroupPurchase* pGPInfo);
	VOID RemoveGuildGPInfo2DB();

private:
	// ���ɼ�ֵ
	INT64	GetKey(DWORD dwRoleID, DWORD dwID);
	
private:
	typedef TMap<INT64, tagGroupPurchase*> MapGPInfo;
	MapGPInfo				m_mapGPInfo;

	Guild*					m_pGuild;
};