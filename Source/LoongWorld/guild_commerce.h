//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_commerce.h
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: ������̹���
//-----------------------------------------------------------------------------
#pragma once

class Guild;
class GuildCommodity;

struct tagGuildCommerceInfo;

class RankCompare
{
public:
	bool operator()(const tagCommerceRank* lhs, const tagCommerceRank* rhs)
	{
		return lhs->nTael > rhs->nTael;
	}
};

class GuildCommerce
{
public:
	GuildCommerce();
	~GuildCommerce();

	BOOL	Init(Guild* pGuild, BOOL bRequest = FALSE);
	VOID	Destory();

	BOOL	IsInitOK()		{ return m_bInitOK; }

	// ��������������Ϣ
	DWORD	LoadCommerceInfo(tagGuildCommerceInfo* pInfo, INT nInfoNum);

	// ��������������Ϣ(���ʼ���ɹ�����޹�)
	DWORD	LoadCommerceRankInfo(tagCommerceRank* pInfo, INT nInfoNum);

	// ��ȡ��������
	DWORD	AcceptCommerce(Role* pRole);

	// �Ͻ�����
	DWORD	CompleteCommerce(Role* pRole, INT32& nFund);

	// ��������
	DWORD	AbandonCommerce(DWORD dwRoleID, BOOL bClearRank = FALSE);

	// ��ȡ�̻���ֵ
	DWORD	GetCommodityGoodInfo(Role* pRole, tagCommerceGoodInfo* pGoodInfo, INT& nGoodNum, INT32& nTael, INT& nLevel);

	// ȡ�����̳�ʼ��Ϣ
	DWORD	GetCommerceBeginningInfo(DWORD dwRoleID, INT& nLevel, tagTaelInfo& sTaelInfo);

	// �����̻�
	DWORD	BuyGoods(Role* pRole, DWORD dwNPCID, DWORD dwGoodID, BYTE byBuyNum);

	// �����̻�
	DWORD	SellGoods(Role* pRole, DWORD dwNPCID, DWORD dwGoodID, BYTE bySellNum);

	// ��ȡ���а�ͼν�״̬
	DWORD	GetCommerceRankInfo(tagCommerceRank* pRankInfo, INT& nRankNum, BOOL& bCommend);

	// ����/�ر����̼ν�
	DWORD	SwitchCommendation(DWORD dwRoleID, BOOL bSwitchON);

	// �������̽���(ͬʱ���Rank��¼)
	VOID	ProvideBonus();

public:
	GuildCommodity*	GetCommodity(DWORD dwRoleID)	{ return m_mapCommodity.Peek(dwRoleID); }
	VOID	Add2CommerceRank(DWORD dwRoleID, INT32 nTael, INT nTimes = GT_INVALID, BOOL bSave2DB = TRUE);
	VOID	RemoveCommerceRank(DWORD dwRoleID);

private:
	typedef TMap<DWORD, GuildCommodity*>		MapCommodity;
	typedef std::vector<tagCommerceRank*>		VectorRank;

	BOOL				m_bInitOK;
	Guild*				m_pGuild;

	BOOL				m_bCommend;

	MapCommodity		m_mapCommodity;
	VectorRank			m_vecRank;
};