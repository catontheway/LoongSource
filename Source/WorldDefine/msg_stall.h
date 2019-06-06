//-----------------------------------------------------------------------------
//!\file msg_stall.h
//!
//!\date 2008-12-26
//!
//!\brief ��̯��Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "stall_define.h"

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// ��̯�߷��͵���Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_StallStart)			// �򿪰�̯����
CMD_END

CMD_START(NS_StallStart)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_StallSetGoods)			// ��Ʒ�ϼ�
	INT64	n64Serial;
	INT64	n64UnitPrice;
	BYTE	byIndex;				// �����ϵ���������0��ʼ
CMD_END

CMD_START(NS_StallSetGoods)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_StallUnsetGoods)		// ��Ʒ�¼�
	BYTE	byIndex;
CMD_END

CMD_START(NS_StallUnsetGoods)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_StallSetTitle)			// ���ñ���
	TCHAR	szData[STALL_MAX_TITLE_NUM];// ����
CMD_END

CMD_START(NS_StallSetTitle)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_StallSetAdText)		// ���ù��
	TCHAR	szData[STALL_MAX_ADTEXT_NUM];// ���
CMD_END

CMD_START(NS_StallSetAdText)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_StallSetAdFlag)		// ���ù��㲥��־
	bool	bFlag;					// 1��ʾ�㲥��0��ʾ���㲥
CMD_END

CMD_START(NS_StallSetAdFlag)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_StallSetFinish)		// ��ɰ�̯���ã�������Χ���
CMD_END

CMD_START(NS_StallSetFinish)
	DWORD	dwErrorCode;
	DWORD	dwRoleID;				// ��̯��ID
	BYTE	byStallLevel;			// ̯λģ�͵ȼ�
CMD_END

CMD_START(NC_StallClose)			// ��̯
CMD_END

CMD_START(NS_StallClose)
	DWORD	dwErrorCode;
	DWORD	dwStallRoleID;			// ��̯��ID
CMD_END

CMD_START(NS_StallBuyRefresh)
	DWORD	dwStallRoleID;			// ��̯��ID
	INT16	n16Num;					// ���º�̯λ����Ʒ����
	BYTE	byIndex;				// ̯λ����Ʒ��λ������
CMD_END

CMD_START(NS_StallSetRefresh)		// ��Ʒ�ϼܹ㲥
	DWORD	dwStallRoleID;			// ��̯��ID
	BYTE	byIndex;				// ̯λ����Ʒ��λ������
CMD_END

CMD_START(NS_StallUnsetRefresh)		// ��Ʒ�¼ܹ㲥
	DWORD	dwStallRoleID;			// ��̯��ID
	BYTE	byIndex;
CMD_END

//-----------------------------------------------------------------------------
// �����߷��͵���Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_StallGet)
	DWORD	dwStallRoleID;			// ��̯��ID
CMD_END

CMD_START(NS_StallGet)
	DWORD	dwErrorCode;
	DWORD	dwStallRoleID;			// ��̯��ID
	BYTE	byNum;					// ��Ʒ����
	BYTE	byData[1];				// tagMsgStallGoods
CMD_END

CMD_START(NC_StallGetTitle)
	DWORD	dwStallRoleID;			// ��̯��ID
CMD_END

CMD_START(NS_StallGetTitle)
	DWORD	dwErrorCode;
	DWORD	dwStallRoleID;			// ��̯��ID
	TCHAR	szTitle[STALL_MAX_TITLE_NUM];
CMD_END

CMD_START(NC_StallBuy)				// ������Ʒ
	INT64	n64Serial;				// Ҫ�������Ʒ64λID
	INT64	n64UnitPrice;			// ��Ʒ����(�����Ʒ�Ƿ��й�������)
	DWORD	dwStallRoleID;			// ��̯��ID
	INT16	n16Num;					// ��Ʒ����
	BYTE	byIndex;				// ����̯λ�ĸ�������(��0��ʼ)
CMD_END

CMD_START(NS_StallBuy)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_StallGetSpec)			// ��ȡָ��̯λ�ϵ���Ʒ
	DWORD	dwStallRoleID;			// ��̯��ID
	BYTE	byIndex;				// ̯λ����Ʒ��λ������
CMD_END

CMD_START(NS_StallGetSpec)
	DWORD	dwErrorCode;
	DWORD	dwStallRoleID;			// ��̯��ID
	BYTE	byData[1];				// tagMsgStallGoods
CMD_END

//-----------------------------------------------------------------------------
// VIP̯λ��Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_GetAllVIPStallInfo)
CMD_END

CMD_START(NS_GetAllVIPStallInfo)
	DWORD		dwErrorCode;
	DWORD		dwRequestTime;
	tagVIPStall	VIPStallInfo[VIP_STALL_MAX_NUM];
CMD_END

CMD_START(NC_UpdateVIPStallInfo)
	DWORD		dwRequestTime;		// VIP̯λ��Ϣ����ʱ��
CMD_END

CMD_START(NS_UpdateVIPStallInfo)
	DWORD		dwErrorCode;
	DWORD		dwRequestTime;		// VIP̯λ��Ϣ����ʱ��
	INT32		nInfoNum;			// ����̯λ��Ϣ��Ŀ��
	tagVIPStall	VIPStallInfo[1];	// ������Ϣ
CMD_END

CMD_START(NC_ApplyVIPStall)
	INT32		nRent;				// ̯λ���
	BYTE		byIndex;			// ̯λ���
CMD_END

CMD_START(NS_ApplyVIPStall)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_SpecVIPStallGet)
	BYTE		byIndex;			// VIP̯λ���
CMD_END

CMD_START(NS_SpecVIPStallGet)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_VIPStallBuy)
	INT64	n64Serial;				// Ҫ�������Ʒ64λID
	INT64	n64UnitPrice;			// ��Ʒ����(�����Ʒ�Ƿ��й�������)
	DWORD	dwStallRoleID;			// ��̯��ID
	INT16	n16Num;					// ��Ʒ����
	BYTE	byIndex;				// ����̯λ�ĸ�������(��0��ʼ)
CMD_END

CMD_START(NS_VIPStallBuy)
	DWORD		dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
#pragma pack(pop)