//-----------------------------------------------------------------------------
// filename: msg_reputation.h
// author: client/server hyu/xlguo
// actor:
// data: 2009-02-19
// last: 
// brief: ������Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push,1)

#include "reputation.h"

//----------------------------------------------------------------------------
// ��ȡ��ɫ������Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_GetReputation)
	DWORD					dwRoleID;		//��ɫID
CMD_END

CMD_START(NS_GetReputation)
	DWORD					dwRoleID;		//��ɫID
	tagReputationData		ReputeData;		//��������
CMD_END

#pragma pack(pop)