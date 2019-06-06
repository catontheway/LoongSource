//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_exchange.h
// author: 
// actor:
// data: 2008-08-21
// last:
// brief: ��Ҽ佻����Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "ItemDefine.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	E_Exchange_Accept				= E_Success,	// �Է�ͬ�⽻��
	E_Exchange_Refuse				= 1,	// �Է��ܾ�		
	E_Exchange_NotEnough_BagSpace	= 2,	// �����ռ䲻��
	E_Exchange_RoleBusy				= 3,	// �����뷽æ	
	E_Exchange_WithMore				= 4,	// ���ܺͶ���ͬʱ����
	E_Exchange_OutOfRange			= 5,	// ���׾��뷶Χ֮��
	E_Exchange_NotInSame_Map		= 6,	// ����ͬһ����
	E_Exchange_ItemCanNot_Exchange	= 7,	// ��Ʒ���ܽ���
	E_Exchange_ItemCanNot_Find		= 8,	// û���ҵ�ָ����������Ʒ
	E_Exchange_NotEnough_Money		= 9,	// ���׵Ľ�Ǯ����
	E_Exchange_ApplicantTgt_Change	= 10,	// ��������Ľ���Ŀ�귢���仯
	E_Exchange_NotAnswer			= 11,	// �Է�û����Ӧ
	E_Exchange_SelfBusy				= 12,	// ����Լ�æ
};




//----------------------------------------------------------------------------
// ��Ϣ�ṹ
//----------------------------------------------------------------------------
CMD_START(NC_ExchangeReq)		// ������󽻻���Ʒ
	DWORD	dwDstID;	// ��ϢĿ��ID
CMD_END;

CMD_START(NS_ExchangeReq)		// ������ת����Ŀ�����
	DWORD	dwSrcID;	// ��Ϣ������ID
CMD_END;

CMD_START(NC_ExchangeReqRes)	// Ŀ����ҽ���������
	DWORD	dwErrorCode;
	DWORD	dwDstID;
CMD_END;

CMD_START(NS_ExchangeReqRes)	// ������ת����Ŀ����ҽ���������
	DWORD	dwErrorCode;
	DWORD	dwSrcID;
	DWORD	dwDstID;
CMD_END;


CMD_START(NC_ExchangeAdd)		// ��ӽ��׵���Ʒװ��
//	DWORD	dwDstID;
	INT64	n64Serial;	// ֻ��Ҫ64λ���,�����������ʼ����Ʒ������Ϣ
CMD_END;

CMD_START(NS_ExchangeAdd_2Src)
	DWORD	dwErrorCode;
	DWORD	dwDstID;
	INT64	n64Serial;
	INT32	nIndex;
CMD_END;

CMD_START(NS_ExchangeAdd_2Dst)
	DWORD	dwSrcID;
	INT32	nIndex;
	BYTE	byData[SIZE_EQUIP];	// ��Ʒ��װ��(��Ҫ֪ͨ�Է�װ������ϸ��Ϣ)
CMD_END;

CMD_START(NC_ExchangeDec)		// ȡ�����׵���Ʒװ��
//	DWORD	dwDstID;
	INT64	n64Serial;
CMD_END;

CMD_START(NS_ExchangeDec)
	DWORD	dwErrorCode;
	DWORD	dwSrcID;
	DWORD	dwDstID;
	INT64	n64Serial;
CMD_END;

CMD_START(NC_ExchangeMoney)		// ��Ǯ�����ı� 
//	DWORD	dwDstID;
	INT64	n64Money;
CMD_END

CMD_START(NS_ExchangeMoney)
	DWORD	dwErrorCode;	
	INT64	n64Money;
	DWORD	dwSrcID;
CMD_END


CMD_START(NC_ExchangeLock)		// ����������Ʒ
//	DWORD	dwDstID;
CMD_END;

CMD_START(NS_ExchangeLock)
	DWORD	dwErrorCode;
	DWORD	dwSrcID;
CMD_END;

CMD_START(NC_ExchangeCancel)	// ȡ������
CMD_END;

CMD_START(NS_ExchangeCancel)
	DWORD	dwSrcID;
CMD_END;

CMD_START(NC_ExchangeVerify)	// ���ȷ�Ͻ���
CMD_END;

CMD_START(NS_ExchangeFinish)	// �������
	DWORD	dwErrorCode;
	DWORD	dwFailedRoleID;		// ����ʧ����ID
CMD_END;



//----------------------------------------------------------------------------
#pragma pack(pop)