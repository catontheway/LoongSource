//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: mall.h
// author: Sxg
// actor:
// data: 2009-1-22
// last:
// brief: �̳��� -- �ɶ�̬�����̳���Ʒ
//-----------------------------------------------------------------------------
#pragma once

class Role;
class GuildPurchase;

struct tagItem;
//-----------------------------------------------------------------------------
// �����д�����������
//-----------------------------------------------------------------------------
struct tagMallItemSell
{
	tagItem		*pItem;
	tagItem		*pPresent;
	INT			nYuanBaoNeed;		//�һ�ʱ������������
	INT32		nExVolumeAssign;
	BYTE		byRemainNum;
	
	tagMallItemSell() { ZeroMemory(this, sizeof(*this)); }
};

struct tagMallPackSell
{
	tagItem		*pItem[MALL_PACK_ITEM_NUM];
	tagItem		*pPresent;
	INT			nYuanBaoNeed;		//�һ�ʱ������������
	INT32		nExVolumeAssign;
	BYTE		byRemainNum;
	
	tagMallPackSell() { ZeroMemory(this, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// �̳���Ʒ����
//----------------------------------------------------------------------------
enum EMallItemType
{
	EMIT_Item,			// ��Ʒ
	EMIT_Pack,			// �����Ʒ
	EMIT_FreeItem,		// �����Ʒ

	EMIT_End
};

//-----------------------------------------------------------------------------
// �̳���
//-----------------------------------------------------------------------------
class Mall
{
public:
	Mall();
	~Mall();

	// �򿪹ر��̳�
	BOOL Init();
	VOID Destroy();

	// ������Դ�������´��̳�
	DWORD ReInit();

	// �Ź���Ϣ����
	VOID Update();

public:
	BOOL IsInitOK()			const { return m_bInitOK; }
	INT  GetItemNum()		const { return m_nItemNum; }
	INT  GetPackNum()		const { return m_nPackNum; }
	INT	 GetFreeItemNum()	const { return (m_pMallFreeItem->pMallFreeItem->dwTypeID != GT_INVALID) ? 1 : 0; }

	DWORD GetMallTime()		const { return m_dwLoadTime; }

	const tagMallGoods* GetMallItem(BYTE byIndex, EMallItemType eType = EMIT_Item);

public:
	// ͬ��
	DWORD GetAllItems(OUT LPVOID pData);
	DWORD GetAllPacks(OUT LPVOID pData);
	DWORD GetFreeItem(OUT LPVOID pData);
	DWORD UpdateAllItems(OUT LPVOID pData, OUT INT &nRefreshNum);
	DWORD UpdateAllPacks(OUT LPVOID pData, OUT INT &nRefreshNum);
		
	// ����
	DWORD SellItem(Role *pRole, DWORD dwToRoleID, DWORD dwCmdID, DWORD dwID, INT nIndex, 
				INT nUnitPrice, INT16 n16BuyNum, OUT tagMallItemSell &itemSell);
	DWORD SellPack(Role *pRole, DWORD dwToRoleID, DWORD dwCmdID, DWORD dwID, BYTE byIndex, 
				INT nUnitPrice, OUT tagMallPackSell &packSell, BOOL bNeedCheckBagSpace);

	// �һ�
	DWORD ExchangeItem(Role *pRole, DWORD dwCmdID, DWORD dwID, BYTE byIndex, 
		INT nPrice, INT16 n16BuyNum, OUT tagMallItemSell &itemSell);
	DWORD ExchangePack(Role *pRole, DWORD dwCmdID, DWORD dwID, BYTE byIndex, 
		INT nPrice, OUT tagMallPackSell &packSell);

	// ��ѷ���
	DWORD GrantFreeItem(Role *pRole, DWORD dwID, OUT tagMallItemSell &itemSell);

	// �����Ź���Ϣ����
	DWORD LoadAllGPInfo(INT nGPInfoNum, LPVOID pData);
	DWORD LaunchGroupPurchase(Role *pRole, DWORD dwID, BYTE byScope,
		BYTE byIndex, INT nUnitPrice);
	DWORD RespondGroupPurchase(Role *pRole, DWORD dwGuildID, DWORD dwID, DWORD dwRoleID,
		INT nPrice);
	DWORD GetAllSimGPInfo(Role* pRole);
	DWORD GetParticipators(Role* pRole, DWORD dwGuildID, DWORD dwID, DWORD dwRoleID);
	VOID RemoveGuildPurchaseInfo(DWORD dwGuildID);

private:
	VOID InitItem();
	VOID InitPack();
	BOOL CheckPack();

public:
	// log
	VOID LogMallSell(DWORD dwBuyRoleID, DWORD dwToRoleID, 
					 const tagItem& item, INT64 n64Serial, INT16 n16Num, 
					 DWORD dwFstGainTime, INT nCostYuanBao, INT nCostExVolume, DWORD dwCmdID);
	VOID LogMallSellPack(DWORD dwPackID, DWORD dwBuyRoleID, DWORD dwToRoleID, INT nCostYuanBao);

private:
	BOOL			m_bInitOK;			// �̳ǿ���״̬
	DWORD			m_dwLoadTime;		// ��ȡ�̳���Ʒԭ��ʱ��(tagDwordTime)
	DWORD			m_dwTimeKeeper;		// ��ʱ��
	BYTE			m_byMinuteTime;		// ���Ӽ�ʱ

private:
	INT				m_nItemNum;			// ��Ʒ����
	INT				m_nPackNum;			// ��Ʒ������
	tagMallGoods	*m_pMallItem;		// ��Ʒ����ָ��
	tagMallGoods	*m_pMallPack;		// ��Ʒ������ָ��
	tagMallGoods	*m_pMallFreeItem;	// �����ȡ��Ʒ

private:
	// �����Ź���Ϣ����
	TMap<DWORD, GuildPurchase*>	m_mapGuildPurchase;

private:
	// ͳ��
	INT				m_nFreeGrantNum;
};

extern Mall g_mall;