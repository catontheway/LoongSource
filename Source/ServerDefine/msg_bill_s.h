//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_bill_s.h
// author: 
// actor:
// data: 2009-06-09
// last:
// brief: ֱ�����������Ϸ����Ľ��
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/msg_common_errorcode.h"
#include "../ServerDefine/bill_define.h"

//-----------------------------------------------------------------------------
// ��Ϸ�����½
//-----------------------------------------------------------------------------
CMD_START(NWB_Login)
	DWORD		dwWorldID;
CMD_END

CMD_START(NBW_Login)
	DWORD		dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ��֪��Ϸ����ȡ�ٱ���
//-----------------------------------------------------------------------------
CMD_START(NBW_SafePickBaiBao)
	INT64		n64Serial;
	DWORD		dwAccountID;
	EBillType	eBillType;
	INT32		nCurYuanBao;
	DWORD		dwIncYuanbao;
CMD_END