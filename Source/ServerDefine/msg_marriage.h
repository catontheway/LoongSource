//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_marriage.h
// author: 
// actor:
// data: 2009-09-10
// last:
// brief: �������
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
//-----------------------------------------------------------------------------

#include "../WorldDefine/msg_common_errorcode.h"

//���
CMD_START(NDBC_RoleGetMarriage)
	DWORD dwRoleID1;
	DWORD dwRoleID2;
CMD_END;

//���
CMD_START(NDBC_RoleBreakMarriage)
	DWORD dwRoleID1;
	DWORD dwRoleID2;
CMD_END;

//-----------------------------------------------------------------------------
#pragma pack(pop)