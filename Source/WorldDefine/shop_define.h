//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: shop_define.h
// author: 
// actor:
// data: 2008-10-22
// last:
// brief: �̵���ض���
//--------------------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/cost_type_define.h"

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT MAX_SHOP_SHELF_NUM		= 4;	// �̵�������Tabҳ��
const INT MAX_SHOP_COMMON_ITEM		= 48;	// ���۵������ͨ��Ʒ������
const INT MAX_SHOP_RARE_ITEM		= 30;	// ���۵����ϡ����Ʒ������
const INT MAX_SHOP_ONE_SHELF		= 30;	// ��ҳ���ܸ���
const INT MAX_SHOP_TOTAL_ITEM		= MAX_SHOP_COMMON_ITEM + MAX_SHOP_RARE_ITEM;

//-----------------------------------------------------------------------------
// ö��
//-----------------------------------------------------------------------------
// ��Ʒ����(ϡ����Ʒ��ˢ��ʱ���)
enum EShopItemType
{
	ESIT_Normal 		= 0, 	// ��ͨ��Ʒ
	ESIT_Rare 			= 1, 	// ϡ����Ʒ
};

//-----------------------------------------------------------------------------
// ���ݽṹ
//-----------------------------------------------------------------------------
// ��ͨ��Ʒ
struct tagShopItemProto
{
	DWORD				dwTypeID;   	// �̶���ƷID
	INT32				nSilver;		// ���Ľ�Ǯ��
	INT32				nCostNum;		// ��������
	BYTE				byRepLevel;		// �����׶�Ҫ��
	BYTE				byShelf;		// ������ҳ

	tagShopItemProto() { ZeroMemory(this, sizeof(*this)); dwTypeID = GT_INVALID; }
};

// ϡ����Ʒ -- �޸ĸýṹʱ��ע��ǰ�沿���ֶ�˳��Ҫ�ͽṹtagShopItemProtoһ��
struct tagShopRareItemProto
{
	DWORD				dwTypeID;   	// �̶���ƷID
	INT32				nSilver;		// ���Ľ�Ǯ��
	INT32				nCostNum;		// ��������
	BYTE				byRepLevel;		// �����׶�Ҫ��
	BYTE				byShelf;		// ������ҳ
	BYTE				byQuality;		// ˢ��Ʒ��
	BYTE				byRefreshNum;	// ˢ������
	DWORD				dwRefreshTime;	// ˢ��ʱ����(��λ: ��)

	tagShopRareItemProto() { ZeroMemory(this, sizeof(*this)); dwTypeID = GT_INVALID; }
};

// �̵꾲̬�ṹ
struct tagShopProto
{
	DWORD					dwID;							// �̵���
	ECurCostType			eCostType;						// �̵�������������
	DWORD					dwItemTypeID;					// �һ�������Ʒtypeid
	INT16					n16Num[MAX_SHOP_SHELF_NUM];		// ��xҳ����Ʒ����
	// tbc:
	BOOL					bClanTreasury;					// ���屦��
	BOOL					bEquip;							// ��: ���۵�ȫ����װ��
															// ��: ������װ��
	
	tagShopItemProto		Item[MAX_SHOP_COMMON_ITEM];		// ��ͨ��Ʒ
	tagShopRareItemProto	RareItem[MAX_SHOP_RARE_ITEM];	// ����(ϡ��)��Ʒ
};

// �ͻ�������Ʒ��̬�ṹ
struct tagShopProtoClient : public tagShopProto
{
	TCHAR					szName[X_SHORT_NAME];						// �̵�����
	TCHAR					szShelf[MAX_SHOP_SHELF_NUM][X_SHORT_NAME];	// ����1����
};

// ��Ʒ(��װ��)�ṹ
struct tagShopItem
{
	DWORD					dwTypeID;		// ��ƷType ID
	INT16					n16RemainNum;	// ��ֵΪGT_INVALIDʱ����ʾ����Ʒ������
	INT16					n16Index;		// ����������Ʒ�±�
	BYTE					byProtoIndex;	// ��Ʒԭ������
	DWORD					dwRefreshTime;	// �ϴ�ˢ��ʱ��(��λ����)
	
	union	// ��Ӧ�ľ�̬�ṹ������ϡ����Ʒ�к���ͨ��Ʒ��ͬ���ֶ�ʱ��Ҳ����ֱ����pProto -- �޸�ʱӦע��
	{
		const tagShopItemProto		*pProto;
		const tagShopRareItemProto	*pRareProto;
	};
	
	// ����ģ���õĽӿڶ���
	DWORD GetKey() const { return dwTypeID; }
	INT16 GetNum() const { return n16RemainNum != GT_INVALID ? n16RemainNum : 1; }
	VOID SetPos(EItemConType eNewCon, INT16 n16NewIndex) { n16Index = n16NewIndex; }
	VOID SetUpdate(EUpdateDBStatus eNewUpdateStatus) { return; }
};

// װ���ṹ
struct tagShopEquip
{
	tagEquip				*pEquip;		// ���ɵ�װ��
	INT16					n16RemainNum;	// ��ֵΪGT_INVALIDʱ����ʾ����Ʒ������
	INT16					n16Index;		// ����������Ʒ�±�
	BYTE					byProtoIndex;	// ��Ʒԭ������
	DWORD					dwRefreshTime;	// �ϴ�ˢ��ʱ��(��λ����)

	union	// ��Ӧ�ľ�̬�ṹ������ϡ����Ʒ�к���ͨ��Ʒ��ͬ���ֶ�ʱ��Ҳ����ֱ����pProto -- �޸�ʱӦע��
	{
		const tagShopItemProto		*pProto;
		const tagShopRareItemProto	*pRareProto;
	};

	// ����ģ���õĽӿڶ���
	DWORD GetKey() const {		return GT_VALID(n16RemainNum) ? pRareProto->dwTypeID : pProto->dwTypeID; 	}
	INT16 GetNum() const { return n16RemainNum != GT_INVALID ? n16RemainNum : 1; }
	VOID SetPos(EItemConType eNewCon, INT16 n16NewIndex) { n16Index = n16NewIndex; }
	VOID SetUpdate(EUpdateDBStatus eNewUpdateStatus) { return; }
};

// �����䱦(��װ��)�ṹ
struct tagTreasuryItem:public tagShopItem
{
	DWORD	dwNameID;		// ����ID
	VOID	SetAct(DWORD dwNameID)
	{
		this->dwNameID = dwNameID;
	}
	BOOL	IsAct() const
	{
		return GT_VALID(dwNameID);
	}
};

// �����䱦(װ��)�ṹ
struct tagTreasuryEquip:public tagShopEquip
{
	DWORD	dwNameID;		// ����ID
	VOID	SetAct(DWORD dwNameID)
	{
		this->dwNameID = dwNameID;
	}
	BOOL	IsAct() const
	{
		return GT_VALID(dwNameID);
	}
};


// ��Ʒ(��װ��)�ṹ -- ���ͻ��˷�����Ϣ��
struct tagMsgShopItem
{
	DWORD					dwTypeID;		// ��ƷType ID
	INT16					n16RemainNum;	// ��ֵΪGT_INVALIDʱ����ʾ����Ʒ������
	INT16					n16Index;		// ����������Ʒ�±�
	BYTE					byProtoIndex;	// ��Ʒԭ������
};

// װ���ṹ -- ���ͻ��˷�����Ϣ��
struct tagMsgShopEquip
{
	tagEquip				Equip;			// ���ɵ�װ��
	INT16					n16RemainNum;	// ��ֵΪGT_INVALIDʱ����ʾ����Ʒ������
	INT16					n16Index;		// ����������Ʒ�±�
	BYTE					byProtoIndex;	// ��Ʒԭ������
};

// ��Ʒ�۸�ṹ
struct tagItemCost
{
	INT64					n64CostSilver;		// ��Ǯ
	ECurCostType			eCostType;			// ����۸�����
	INT16					nCostNum;			// ����۸�
	INT16					nRepLevel;			// ��������
	DWORD					dwCostItemTypeID;	// ������Ʒԭ��ID

	tagItemCost(){n64CostSilver = nCostNum = nRepLevel = 0; eCostType = ECCT_Null;dwCostItemTypeID = GT_INVALID;}
	tagItemCost(INT nSilver, ECurCostType eType, INT nNum, INT nRep, DWORD dwID)
	{n64CostSilver = nSilver; nCostNum = nNum; nRepLevel = nRep; eCostType = eType; dwCostItemTypeID = dwID;}
};

static VOID Destroy(tagShopItem *pItem)
{
	SAFE_DEL(pItem);
}

static VOID Destroy(tagShopEquip *pItem)
{
	SAFE_DEL(pItem);
}

//-----------------------------------------------------------------------------
// ��
//-----------------------------------------------------------------------------
// �ж���Ʒ�Ƿ�Ϊϡ��(ˢ��)��Ʒ
#define MIsShopRareGoods(pShopGoods)		(pShopGoods->n16RemainNum != GT_INVALID)

// װ�����ۼ۸�Ʒ������ϵ��
#define MCalPriceFactor(fFactor, nQlty)	\
	switch(nQlty)			\
	{						\
	case EIQ_White:			\
		fFactor = 1.0f;		\
		break;				\
	case EIQ_Yellow:		\
		fFactor = 1.5f;		\
		break;				\
	case EIQ_Green:			\
		fFactor = 2.5f;		\
		break;				\
	case EIQ_Blue:			\
		fFactor = 3.5f;		\
		break;				\
	case EIQ_Orange:		\
		fFactor = 5.0f;		\
		break;				\
	case EIQ_Purple:		\
		fFactor = 6.0f;		\
		break;				\
	default:				\
		fFactor = 0.1f;		\
		break;				\
	}


//-----------------------------------------------------------------------------
#pragma pack(pop) 