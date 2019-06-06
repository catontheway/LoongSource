//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_item.h
// author: 
// actor:
// data: 2008-09-04
// last:
// brief: ��Ʒ���
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
//-----------------------------------------------------------------------------


#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/ItemDefine.h"
#include "item_define.h"
//-----------------------------------------------------------------------------
// ö��
//-----------------------------------------------------------------------------
enum
{
	
};

//-----------------------------------------------------------------------------
// ��Ϣ�ṹ
//-----------------------------------------------------------------------------
CMD_START(NDBC_GetItemInfo)	// �����Ϸ������ȫ����Ʒ�����Ϣ
CMD_END;

CMD_START(NDBS_GetItemInfo)
	DWORD	dwDummy;
	INT64	n64MaxSerial;
	INT64	n64MinSerial;
CMD_END;

CMD_START(NDBC_NewItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_NewEquip)
	tagEquip	equip;
CMD_END;

CMD_START(NDBC_DelItem)
	INT64		n64Serial;
CMD_END;

CMD_START(NDBC_DelEquip)
	INT64		n64Serial;
CMD_END;

CMD_START(NDBC_NewBaiBaoItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_NewBaiBaoEquip)
	tagEquip	equip;
CMD_END;

CMD_START(NDBC_DelBaiBaoItem)
	INT64		n64Serial;
CMD_END;

CMD_START(NDBC_DelBaiBaoEquip)
	INT64		n64Serial;
CMD_END;

//CMD_START(NDBC_UpdateItemPos)	// ������Ʒλ��
//	DWORD		dwAccountID;
//	INT64		n64Serial;
//	DWORD		dwRoleID;
//	INT32		nUseTimes;		// ��Ʒ�ϼ���ʹ�ô���
//	INT16		n16Num;			// ��Ʒ����
//	INT16		n16Index;		// ������λ��
//	BYTE		byConType;		// ��������
//CMD_END;

CMD_START(NDBC_UpdateEquipAtt)	// ����װ�����ԣ���װ�����Ա�
	tagEquipSpecUpdate	equipSpecUpdate;
CMD_END;


//CMD_START(NDBC_ReplaceItemCDTime)	// �洢��Ʒ��ȴʱ��
//	DWORD			dwRoleID;
//	INT32			nNum;			// ����
//	BYTE			byCDTime[1];	// tagCDTime
//CMD_END;


//-----------------------------------------------------------------------------
// ѹ��ר��
//-----------------------------------------------------------------------------
CMD_START(NDBC_TestSaveItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_TestSaveEquip)
	tagEquip	equip; 
CMD_END;

//-----------------------------------------------------------------------------
#pragma pack(pop)
