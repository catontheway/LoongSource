#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

//----------------------------------------------------------
// ֪ͨ�ͻ���ɾ��ĳ��Զ����һ����
//----------------------------------------------------------
CMD_START(NS_RemoveRemote)
	DWORD	dwRoleID[1];
CMD_END

#pragma pack(pop)