//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_vip_netbar.h
// author: xlguo
// actor:
// data: 2009-8-3
// last:
// brief: �������ɷ�������Ϣ
//-------------------------------------------------------------------------------
#pragma once
#include "vip_netbar_define.h"
#pragma pack(push,1)

//-------------------------------------------------------------------------
// ��ȡ�˺���ʷ����
//-------------------------------------------------------------------------
CMD_START(NDBC_GetVNBData)
	DWORD dwDate;
CMD_END

CMD_START(NDBS_GetVNBData)
	tagDBVNBPlayers players;	
CMD_END

//-------------------------------------------------------------------------
// ���½����������
//-------------------------------------------------------------------------
CMD_START(NDBC_UpdateVNBPlayer)
	DWORD	dwAccountID;
	DWORD	dwLoginTime;
CMD_END

#pragma pack(pop)