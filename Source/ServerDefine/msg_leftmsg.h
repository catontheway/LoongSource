#pragma once

#pragma pack(push)
#pragma pack(1)

//-------------------------------------------------------------------------
// ������һ��һ�����棬һ���������
//-------------------------------------------------------------------------
struct tagDBLeftMsg
{
	DWORD dwDateTime;
	DWORD dwRoleID;
	BYTE byData[1];
};

const INT MAX_LEFTMSG_NUM = 50;

//-------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------
CMD_START(NDBC_InsertLeftMsg)
	tagDBLeftMsg data;
CMD_END;

CMD_START(NDBS_InsertLeftMsg)
CMD_END;

//-------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------
// CMD_START(NDBC_UpdateLeftMsg)
// 	DWORD dwOldDateTime;
// 	tagDBLeftMsg data;
// CMD_END;
// 
// CMD_START(NDBS_UpdateLeftMsg)
// CMD_END;


//-------------------------------------------------------------------------
// ��ȡ����
//-------------------------------------------------------------------------
CMD_START(NDBC_LoadLeftMsg)	
	DWORD dwRoleID;
CMD_END;

CMD_START(NDBS_LoadLeftMsg)
	DWORD dwErrCode;
	DWORD dwRoleID;
	BYTE byData[1];
CMD_END;

//-------------------------------------------------------------------------
// �������
//-------------------------------------------------------------------------
CMD_START(NDBC_ClearLeftMsg)	
	DWORD dwRoleID;
CMD_END;

CMD_START(NDBS_ClearLeftMsg)
	DWORD dwErrCode;
CMD_END;


#pragma pack(pop)
