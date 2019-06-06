//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: mall_define.h
// author: 
// actor:
// data: 2009-1-5
// last:
// brief: �̳���ض���
//--------------------------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//--------------------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------------------
const INT	MALL_PACK_ITEM_NUM		= 5;	// ��Ʒ���������Ʒ������
const INT	Max_PackName_Length		= 10;	// ����������10����
const INT	Max_LeaveWord_Length		= 20;	//����20����
const INT	MAX_PromoteItem			= 5;	//�Ƽ���Ʒ����
const INT64 MAX_YB_NUM				= 999999;		// Ԫ�������˻����Ԫ����
const INT64 MAX_SILVER_NUM			= 9999999999;	// Ԫ�������˻����������

//--------------------------------------------------------------------------------------------
// ��̬�ṹ
//--------------------------------------------------------------------------------------------
struct tagMallItemProtoBase
{
	DWORD			dwID;							// ID
	INT				nPrice;							// ����
	INT				nSalePrice;						// ������
	DWORD			dwTimeSaleEnd;					// ������ֹʱ��
	DWORD			dwPresentID;					// ��ƷID
	BYTE			byNum;							// ��������
	BYTE			byPresentNum;					// ��Ʒ����
	BYTE			byExAssign;						// ������������
	BYTE			byExNum;						// ��Ҫ��������
	BYTE			bySmallGroupHeadcount;			// С���Ź�����
	BYTE			bySmallGroupDiscount;			// С���Ź��ۿ�[0��100]
	BYTE			byMediumGroupHeadcount;			// �����Ź�����
	BYTE			byMediumGroupDiscount;			// �����Ź��ۿ�
	BYTE			byBigGroupHeadcount;			// �����Ź�����
	BYTE			byBigGroupDiscount;				// �����Ź��ۿ�
	bool			bNew	: 1;					// �Ƿ�Ϊ��Ʒ
	bool			bHot	: 1;					// �Ƿ�����
	INT				nIndexInServer;					// �ڷ������е�λ��
	DWORD			dwPersistTime;					// �Ź�����ʱ��
};


struct tagMallItemProto: public tagMallItemProtoBase
{
	DWORD			dwTypeID;						// ��ƷTypeID
	INT8			n8Kind;							// ��������
	BYTE			byRank;							// ��������
	BYTE			byGroupPurchaseAmount;			// �Ź�����
	BYTE			byDummy[1];
};

struct tagMallPackProto: public tagMallItemProtoBase
{
	DWORD			dwTypeID[MALL_PACK_ITEM_NUM];	// ��ƷID
	INT				nItemPrice[MALL_PACK_ITEM_NUM];	// ������Ʒ�۸�(�ܼ�)
	BYTE			byItemNum[MALL_PACK_ITEM_NUM];	// ��Ʒ����
	INT8			n8ItemKind;						// ��Ʒ����(ID)����
	BYTE			byDummy[2];
	TCHAR			tszPackName[Max_PackName_Length];// �������
	tagMallPackProto()
	{
		ZeroMemory(this, sizeof(*this));
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			dwTypeID[i] = GT_INVALID;
		}
	}
};

struct tagMallFreeItem
{
	DWORD		dwTypeID;	// ��ƷID
	INT			nUnitPrice;	// ԭ����
	BYTE		byNum;		// ��ȡ����

	tagMallFreeItem()
	{
		Clear();
	}

	VOID Clear()
	{
		dwTypeID	= GT_INVALID;
		nUnitPrice	= 0;
		byNum		= 0;
	}
};

//--------------------------------------------------------------------------------------------
// ���������ݽṹ
//--------------------------------------------------------------------------------------------
//struct tagMallGoods
//{
//	DWORD	dwID;		// ��Ʒ����Ʒ��ID
//	BYTE	byCurNum;	// ��ǰ��������(GT_INVALID��ʾ������)
//	BYTE	byDummy[3];
//};

struct tagMallGoods
{
	union
	{
		const tagMallItemProto	*pMallItem;
		const tagMallPackProto	*pMallPack;
		const tagMallFreeItem	*pMallFreeItem;
	};

	BYTE	byCurNum;	// ��ǰ��������(GT_INVALID��ʾ������)
	BYTE	byDummy[3];
};

//--------------------------------------------------------------------------------------------
// �ͻ������������Ʒ����ͬ���ṹ
//--------------------------------------------------------------------------------------------
struct tagMallUpdate
{
	DWORD		dwID;					//��ƷID
	BYTE		byRemainNum;			//ʣ�����
};

//--------------------------------------------------------------------------------------------
// �����Ź�
//--------------------------------------------------------------------------------------------
enum EGPSCOPE
{
	EGPS_NULL		= -1,
	EGPS_SMALL,							//С���Ź�
	EGPS_MEDIUM,						//�����Ź�
	EGPS_BIG,							//�����Ź�
};

struct tagGroupPurchase
{
	DWORD			dwGuildID;			//����ID
	DWORD			dwRoleID;			//������ID
	DWORD			dwMallID;			//��ƷID
	INT32			nPrice;				//�Ź��۸�
	INT16			nParticipatorNum;	//��Ӧ����
	INT16			nRequireNum;		//�Ź���������
	DWORD			dwRemainTime;		//ʣ��ʱ��
	TList<DWORD>*	listParticipators;	//��Ӧ���б�

	tagGroupPurchase()
	{
		ZeroMemory(this, sizeof(*this));
	}

	~tagGroupPurchase()
	{
		SAFE_DEL(listParticipators);
	}
};

enum EYBOTYPE							//Ԫ����������
{
	EYBOT_BUY,							//�չ�
	EYBOT_SELL,							//����
};

enum EYBOMODE							//Ԫ������״̬
{
	EYBOM_Submit,						//ί����
	EYBOM_Cancel,						//����
	EYBOM_Close,						//�ر�
};

//--------------------------------------------------------------------------------------------
// Ԫ�����׶���
//--------------------------------------------------------------------------------------------
struct tagYuanBaoOrder
{
	DWORD				dwID;					//����ID
	DWORD				dwRoleID;				//���ID
	EYBOTYPE			eYBOType;				//��������
	INT					nPrice;					//����
	INT					nNum;					//����
	EYBOMODE			eYBOMode;				//Ԫ������״̬
	tagDWORDTime		dwStartTime;			//��ʼʱ��
	tagDWORDTime		dwEndTime;				//����ʱ��
	INT					nAvgPrice;				//�ɽ�����
	INT					nDealNum;				//�ɽ�����
	tagYuanBaoOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwStartTime = GT_INVALID;
		dwEndTime = GT_INVALID;
	}

	INT		GetRemainNum() { return nNum - nDealNum; }	
};

//--------------------------------------------------------------------------------------------
// ����Ԫ�����׶���(�������ݿ�ʱʹ�ã�
//--------------------------------------------------------------------------------------------
struct tagYBOrder
{
	DWORD				dwID;
	DWORD				dwRoleID;
	INT16				nType;
	INT					nPrice;
	INT					nNum;
	INT16				nYBMode;
	DWORD				dwStartTime;
	tagYBOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwStartTime = GT_INVALID;
	}
};

// Ԫ�����׼۸��б�
struct tagYBPriceList
{
	INT					nPrice;
	INT					nNum;
};
 
// Ԫ�������˻�
struct tagYBAccount
{
	DWORD				dwRoleID;				//���ID
	INT					nYuanBao;				//Ԫ������
	INT					nGold;					//�������
	INT					nSilver;				//��������
	BOOL				bSellOrder;				//�Ƿ��г��۶���
	BOOL				bBuyOrder;				//�Ƿ����չ�����
};

// ������ر�Ԫ�����׶���
struct tagCompleteOrder
{
	DWORD				dwID;
	INT16				nYBOMode;
	DWORD				dwEndTime;
	tagCompleteOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwEndTime = GT_INVALID;
	}
};

// ����Ԫ�����׶���
struct tagRefresh
{	
	DWORD				dwID;
	INT					nAvgPrice;
	INT					nDealNum;
	tagRefresh()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
	}
};



//--------------------------------------------------------------------------------------------
#pragma pack(pop)