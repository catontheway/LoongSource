//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_mall.h
// author: 
// actor:
// data: 2009-02-1
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ -- �̳���Ʒ
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

#include "msg_common_errorcode.h"
#include "mall_define.h"

//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	//E_Mall_				= ,	// 
	E_Mall_Close						= 1,	// �̳ǹر�

	E_Mall_Item_NotEnough				= 10,	// ��Ʒ�������㹺�����
	E_Mall_Pack_NotEnough				= 11,	// ��Ʒ���������㹺�����

	E_Mall_ID_Error						= 12,	// ID����ȷ
	E_Mall_YuanBao_Error				= 13,	// Ԫ�����۲���ȷ
	E_Mall_Free_NotExist				= 14,	// �����������Ʒ
	E_Mall_ExVolume_Error				= 15,	// ���㵥�۲���ȷ
	E_Mall_Exchange_NotAllowable		= 16,	// ��Ʒ���ɶһ�

	E_Mall_CreateItem_Failed			= 20,	// ��Ʒ����ʧ��
	E_Mall_CreatePres_Failed			= 21,	// ��Ʒ����ʧ��

	E_Mall_Free_AlreadyGet				= 25,	// �����Ʒ����ȡ

	// Ԫ������
	E_Trade_BagYB_NotEnough				= 40,	// ������Ԫ������
	E_Trade_BagSilver_NotEnough			= 41,	// �����н�Ǯ����
	E_Trade_AccountYB_NotEnough			= 42,	// �˻���Ԫ������
	E_Trade_AccountSilver_NotEnough		= 43,	// �˻��н�Ǯ����
	E_Trade_SellOrder_Exit				= 44,	// ֻ���ύһ��Ԫ�����۶���
	E_Trade_BuyOrder_Exit				= 45,	// ֻ���ύһ��Ԫ���չ�����
	E_Trade_Tax_NotEnough				= 46,	// ���������Ѳ���

	// �����Ź�
	E_GroupPurchase_NotMember			= 50,	// ���Ǳ����ɳ�Ա
	E_GroupPurchase_ItemNotAllowable	= 51,	// ����Ʒ���ɽ����Ź�
	E_GroupPurchase_NotInGuild			= 52,	// ���û�м������
	E_GroupPurchase_AlreadyInitiate		= 53,	// �Ѿ��Ը���Ʒ������Ź�
	E_GroupPurchase_IsInitiate			= 54,	// ������Ӧ�Լ�������Ź�
	E_GroupPurchase_AlreadyRespond		= 55,	// �Ѿ���Ӧ�����Ź�
	E_GroupPurchase_AlreadyEnd			= 56,	// �Ź��Ѿ�����
	E_GroupPurchase_NoInfo				= 57,	// �Ź���Ϣ������
	E_GroupPurchase_ServerBusy			= 58,	// ������æ
};

//-----------------------------------------------------------------------------
// �����ýṹ
//-----------------------------------------------------------------------------
struct tagSimGPInfo
{
	DWORD			dwGuildID;
	DWORD			dwRoleID;
	DWORD			dwMallID;
	INT32			nPrice;				//�Ź��۸�
	INT16			nParticipatorNum;	//��Ӧ����
	INT16			nRequireNum;		//�Ź���������
	DWORD			dwRemainTime;		//ʣ��ʱ��
};

//----------------------------------------------------------------------------
// ��Ϣ
//----------------------------------------------------------------------------

//--ͬ������
CMD_START(NS_OpenMall)
	DWORD		dwErrorCode;
	DWORD		dwMallTime;
CMD_END

CMD_START(NC_MallGet)					//�������ѯ���Ƿ���Դ��̳�
CMD_END	

CMD_START(NS_MallItem)
	INT			nItemNum;
	BYTE		byData[1];				// tagMallItemProto
CMD_END

CMD_START(NS_MallFreeItem)
	tagMallFreeItem		freeItem;
CMD_END

CMD_START(NS_MallPack)
	INT			nPackNum;
	BYTE		byData[1];
CMD_END

//--����
CMD_START(NC_MallUpdate)
	DWORD		dwMallTime;
CMD_END

CMD_START(NS_MallUpdate)
	DWORD		 dwMallTime;
CMD_END

CMD_START(NS_MallUpdateItem)
	INT			nItemNum;
	BYTE		byData[1];
CMD_END

CMD_START(NS_MallUpdatePack)
	INT			nItemNum;
	BYTE		byData[1];
CMD_END


//--����
CMD_START(NC_MallBuyItem)
	DWORD		dwTypeID;				// �̳���ƷID
	INT			nPrice;					// ��Ʒ�۸�(��ҹ���ʱ�ĵ���)
	INT16		n16BuyNum;				// �������
	INT			nIndexInServer;			// �ڷ������е�λ��			
CMD_END

CMD_START(NS_MallBuyItem)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_MallBuyPack)
	DWORD		dwTypeID;				// �̳���ƷID
	INT			nPrice;					// ��Ʒ�۸�(��ҹ���ʱ�ĵ���)
	INT16		n16BuyNum;				// �������
	INT			nIndexInServer;		// �ڷ������е�λ��			
CMD_END

CMD_START(NS_MallBuyPack)
	DWORD		dwErrorCode;
CMD_END


//--����
CMD_START(NC_MallPresentItem)
	DWORD		dwTypeID;				// �̳���ƷID
	INT			nPrice;					// ��Ʒ�۸�(��ҹ���ʱ�ĵ���)
	DWORD		dwRoleTgtID;			// Ҫ�͵����ID
	INT16		n16BuyNum;				// �������
	INT			nIndexInServer;		// �ڷ������е�λ��		
	TCHAR		szLeave[Max_LeaveWord_Length];
CMD_END

CMD_START(NS_MallPresentItem)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_MallPresentPack)
	DWORD		dwTypeID;				// �̳���ƷID
	INT			nPrice;					// ��Ʒ�۸�(��ҹ���ʱ�ĵ���)
	DWORD		dwRoleTgtID;			// Ҫ�͵����ID
	INT16		n16BuyNum;				// �������
	INT			nIndexInServer;		// �ڷ������е�λ��		
	TCHAR		szLeave[Max_LeaveWord_Length];
CMD_END

CMD_START(NS_MallPresentPack)
	DWORD		dwErrorCode;
CMD_END

//--�����ȡ
CMD_START(NC_MallFreeGetItem)
	DWORD		dwTypeID;
CMD_END

CMD_START(NS_MallFreeGetItem)
	DWORD		dwErrorCode;
CMD_END

//--�����Ź�
CMD_START(NC_LaunchGroupPurchase)
	DWORD		dwTypeID;				// �̳���ƷID
	INT			nPrice;					// ��Ʒ�۸�(��ҹ���ʱ�ĵ���)
	BYTE		byScope;				// �Ź���ģ
	INT			nIndexInServer;			// �ڷ������е�λ��
CMD_END

CMD_START(NS_LaunchGroupPurchase)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_RespondGroupPurchase)
	DWORD		dwGuildID;				// �Ź���Ϣ��������
	DWORD		dwTypeID;				// �̳���ƷID
	DWORD		dwRoleID;				// ������ID
	INT			nPrice;					// ��Ʒ�۸�(�Ź��ܼ۸�)
CMD_END

CMD_START(NS_RespondGroupPurchase)
	DWORD		dwErrorCode;
	INT64		nGroupPurchaseKey;		// �Ź���Ϣ��ֵ(RoleID<<32 | MallID)
CMD_END

CMD_START(NC_GetSimGPInfo)
CMD_END

CMD_START(NS_GetSimGPInfo)
	DWORD				dwErrorCode;
	INT32				nGroupPurchase;		// �����Ź�ָ��
	INT					nGPInfoNum;			// �Ź���Ϣ����
	tagSimGPInfo		simData[1];			// �������Ź���Ϣ
CMD_END

CMD_START(NC_GetParticipators)
	DWORD		dwGuildID;				// �Ź���Ϣ��������
	DWORD		dwTypeID;				// �̳���ƷID
	DWORD		dwRoleID;				// ������ID	
CMD_END

CMD_START(NS_Getparticipators)
	DWORD		dwErrorCode;
	INT64		nGroupPurchaseKey;		// �Ź���Ϣ��ֵ(RoleID<<32 | MallID)
	INT			nParticipatorNum;		// ��Ӧ������
	DWORD		dwParticipators[1];		// ��Ӧ���б�
CMD_END

enum ERespondBroadCast
{
	ERespondBroadCast_NULL		= 0,
	ERespondBroadCast_Launch,
	ERespondBroadCast_Success,
	ERespondBroadCast_Lose,
};

CMD_START(NS_RespondBroadCast)
	ERespondBroadCast	eType;
	DWORD				dwRoleID;
	DWORD				dwTypeID;			//��ƷTypeID
CMD_END

//--�һ�
CMD_START(NC_MallItemExchange)
	DWORD		dwMallID;				// �̳���ƷID
	INT			nPrice;					// ������Ʒ������������
	INT16		n16BuyNum;				// �һ�����
	INT			nIndexInServer;		// �ڷ������е�λ��		
CMD_END

CMD_START(NS_MallItemExchange)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_MallPackExchange)
	DWORD		dwMallID;				// �̳���ƷID
	INT			nPrice;					// ������������
	INT			nIndexInServer;		// �ڷ������е�λ��	
CMD_END

CMD_START(NS_MallPackExchange)
	DWORD		dwErrorCode;
CMD_END

//----------------------------------------------------------------------------
// Ԫ������
//----------------------------------------------------------------------------
CMD_START(NS_SynAccoutYB)				// ͬ�������˻���Ԫ����
	INT			nYuanBao;
CMD_END

CMD_START(NS_SynAccoutSilver)			// ͬ�������˻��Ľ�Ǯ��
	INT64		nSilver;
CMD_END

CMD_START(NS_SynAccout)					// ͬ�������˻�
	INT			nYuanBao;
	INT			nSilver;
CMD_END


CMD_START(NC_SaveYB2Account)			// ��������˻���Ԫ��
	INT			nNum;					// Ԫ������
CMD_END

CMD_START(NS_SaveYB2Account)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_SaveSilver2Account)		// ��������˻����Ǯ
	INT64		nNum;
CMD_END

CMD_START(NS_SaveSilver2Account)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_DepositAccountYB)			// �ӽ����˻���ȡԪ��
	INT			nNum;
CMD_END

CMD_START(NS_DepositAccountYB)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_DepositAccountSilver)		// �ӽ����˻���ȡ��Ǯ
	INT64		nNum;
CMD_END				

CMD_START(NS_DepositAccountSilver)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_SynYBTradeInfo)			// ��ȡ����Ԫ�����׼۸���б�ͽ����˻���Ϣ
CMD_END

CMD_START(NS_GetSellPriceList)
	INT				nListNum;
	tagYBPriceList	YBPriceList[1];
CMD_END

CMD_START(NS_GetBuyPriceList)
	INT				nListNum;
	tagYBPriceList	YBPriceList[1];
CMD_END

CMD_START(NS_GetYBAccount)				// ���Ԫ�������˻�
	DWORD			dwRoleID;			// ���ID
	INT				nYuanBao;			// Ԫ������
	INT64			nSilver;			// ��Ǯ����
	BOOL			bSellOrder;			// �Ƿ��г��۶���
	BOOL			bBuyOrder;			// �Ƿ����չ�����
CMD_END

CMD_START(NS_SynSellPriceList)			// ͬ�����ۼ۸��б�
	INT				nPrice;
	INT				nNum;
CMD_END

CMD_START(NS_SynBuyPriceList)			// ͬ������۸��б�
	INT				nPrice;
	INT				nNum;
CMD_END

CMD_START(NC_SubmitSellOrder)			// �ύԪ�����۶���
	INT				nNum;				// Ԫ������
	INT				nPrice;				// ����
CMD_END

CMD_START(NS_SubmitSellOrder)
	DWORD			dwErrorCode;
CMD_END

CMD_START(NC_SubmitBuyOrder)			// �ύԪ���չ�����
	INT				nNum;
	INT				nPrice;
CMD_END

CMD_START(NS_SubmitBuyOrder)
	DWORD			dwErrorCode;
CMD_END

CMD_START(NC_GetYuanbaoOrder)           // �����Ҷ���
CMD_END

CMD_START(NS_GetYuanbaoOrder)
    INT             nNum;               // ��������
    tagYuanBaoOrder Orders[1];
CMD_END

CMD_START(NC_DeleteOrder)			    // ɾ������
	DWORD			dwOrderID;			// ����ID
	EYBOTYPE		eYBOType;			// ��������
CMD_END

CMD_START(NS_DeleteOrder)
	DWORD			dwErrorCode;
CMD_END

#pragma pack(pop)