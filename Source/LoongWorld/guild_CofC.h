//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_CofC.h
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: �̻�
//-----------------------------------------------------------------------------
#pragma once

struct tagCofCSellGood
{
	DWORD		dwGoodID;
	INT32		nCost;
	BYTE		byRemainNum;
};

struct tagCofCBuyGood
{
	DWORD		dwGoodID;
	INT32		nCost;
};

class GuildCofC
{
public:
	GuildCofC();
	~GuildCofC();

public:
	static GuildCofC* Create(DWORD dwNPCID, DWORD dwCofCID);
	static VOID	 Delete(GuildCofC *&pCofC) { SAFE_DEL(pCofC); }

public:
	BOOL	Init(DWORD dwNPCID, DWORD dwCofCID);
	VOID	Update();
	VOID	Destory();

	// ��ʱˢ���̻�
	VOID	RefreshSellGoods();
	VOID	RefreshBuyGoods();

	// ȡ���̵���������Ʒ��Ϣ
	BYTE	GetGoodsNum()	{ return m_mapGoodSell.Size(); }
	VOID	GetGoodsInfo(DWORD dwRoleID, OUT tagCommerceGoodInfo* pGoods, INT& byNum);

	// �̵������̻�
	DWORD	SellItem(INT32 nTael, DWORD dwGoodID, BYTE byBuyNum, OUT INT32 &nCostTael);
	// ��ȡָ���̻��ڸ��̵���չ���
	INT32	GetGoodPrice(DWORD dwGoodID);

	// �Ƴ��۲����б�
	VOID	RemoveObserver(DWORD dwRoleID)	{ m_listObserver.Erase(dwRoleID); }

	// �����̻��Լ������������̻�
	DWORD	SendCommerceGoodsInfo2Role(Role* pRole, DWORD dwGoodID = GT_INVALID);

	// ȡ���̻�����
	DWORD	GetCofCID()		{ return m_dwCofCID; }
	BYTE	GetHolderCity()	{ return m_byHolderCity; }

private:
	BOOL	IsInitOK() const { return m_bInitOK; }
	VOID	CalRefreshPrice(INT32& nCurPrice, INT32 nLowPrice, INT32 nHighPrice);
	VOID	SendMsg2Observer(LPVOID pMsg, DWORD dwSize);

private:
	typedef TMap<DWORD, tagCofCSellGood*>	MapSellGood;
	typedef TMap<DWORD, tagCofCBuyGood*>	MapBuyGood;
	typedef TList<DWORD>					ListObserver;

	BOOL			m_bInitOK;

	DWORD			m_dwNPCID;
	DWORD			m_dwCofCID;
	BYTE			m_byHolderCity;

	MapSellGood		m_mapGoodSell;
	MapBuyGood		m_mapGoodBuy;
	ListObserver	m_listObserver;

	tagDWORDTime	m_dwSyncTime;
};