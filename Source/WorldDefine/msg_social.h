//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_social.h
// author: 
// actor:
// data: 2008-12-04
// last:
// brief: ����ϵ�����Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

// ������
enum
{
	E_Friend_Target_Not_Exist			=	1,		// Ŀ�겻����
	E_Friend_Target_Not_Sel				=	2,		// ���ܼ��Լ�Ϊ����
	E_Friend_Target_Black_List			=	3,		// �������ڵ�����޷������Ϊ����
	E_Friend_Target_Already_Exit		=	4,		// �ú����Ѵ����ں����б�
	E_Friend_Max_Num					=	5,		// �����б������Ƿ�ﵽ����100��
	E_Friend_Group_Not_Exit				=	6,		// ���鲻����
	E_Friend_Not_Friend					=	7,		// ���Ǹ���Һ���
	E_Friend_CanNotDel_Spouse			=	8,		// ����ɾ������

	E_Black_Targer_Not_Sel				=	20,		// ���ܼ��Լ���������
	E_Black_Max_Num						=	21,		// ����������
	E_Black_Target_Already_Exit			=	22,		// ������ں�������
	E_Black_Target_Not_Exit				=	23,		// ��Ҳ��ں�������

	E_Gift_Not_Exit						=	30,		// ���ﲻ����
	E_Gift_Not_Both_Friend				=	31,		// ����˫�����
	E_Gift_Friend_Not_Online			=	32,		// ���Ѳ�����
	E_Gift_Not_Gift						=	33,		// ���͵���Ʒ��������
};

CMD_START(NS_LoginToFriend)		// �������ʱ֪ͨ����
	DWORD	dwRoleID;
CMD_END;

CMD_START(NS_LogoutToFriend)	// �������ʱ֪ͨ����
	DWORD	dwRoleID;
CMD_END;

CMD_START(NC_RoleMakeFriend)	// ���˭�ύΪ����
	DWORD	dwDestRoleID;	
CMD_END;

CMD_START(NS_RoleMakeFriend)
	DWORD	dwDestRoleID;
	BOOL	bOnline;
	INT		nLevel;				// ��ɫ�ȼ�
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_RoleCancelFriend)	// ȡ�����ѹ�ϵ
	DWORD   dwDestRoleID;
CMD_END;

CMD_START(NS_RoleCancelFriend)	
	DWORD   dwDestRoleID;
	BYTE	byGroup;
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_UpdateFriGroup)	// ���ѷ���
	DWORD	dwDestRoleID;
	BYTE	byGroup;
CMD_END;

CMD_START(NS_UpdateFriGroup)
	DWORD	dwDestRoleID;
	BYTE	byOldGroup;			// 1-4�������б�
	BYTE	byNewGroup;
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_MoveToBlackList)	// ������ҵ�������
	DWORD	dwDestRoleID;
CMD_END;

CMD_START(NS_MoveToBlackList)
	DWORD	dwDestRoleID;
	BYTE	byOldGroup;
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_DeleteBlackList)	// ����ҴӺ�����ɾ��
	DWORD	dwDestRoleID;
CMD_END;

CMD_START(NS_DeleteBlackList)
	DWORD	dwDestRoleID;
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_SendGift)			// ������
	DWORD	dwDestRoleID;		// ���������
	INT64	n64ItemID;			// ����ID
CMD_END;

CMD_START(NS_SendGiftToFri)		// ������
	DWORD	dwSrcRoleID;		// ���﷢����
	DWORD	dwTypeID;			// ��Ʒ����ID
CMD_END;

CMD_START(NS_SendGiftToSender)	// ������
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_SendGiftReply)		// �յ������ظ�
	DWORD	dwSrcRoleID;		// ���﷢����
	DWORD	dwTypeID;			// ����TypeID
	BOOL	bResult;			// �Ƿ����
CMD_END;

CMD_START(NS_SendGiftBroadcast)	// �㲥
	DWORD	dwSrcRoleID;
	DWORD	dwDestRoleID;
	DWORD	dwTypeID;
	BOOL	bResult;
CMD_END;

CMD_START(NS_SendBlackList)		// ���ͺ�������Ϣ
	DWORD	dwRolesID[MAX_BLACKLIST];
CMD_END;

CMD_START(NC_UpdateFriState)
CMD_END;

CMD_START(NS_UpdateFriState)
	INT				nNum;
	tagFriUpdate	FriUpdate[1];
CMD_END;

CMD_START(NS_MakeFriNotice)
	DWORD	dwRoleID;			
CMD_END;

CMD_START(NS_UpdateFriValue)
	DWORD   dwRoleID;
	INT		nFriVal;
CMD_END;

#pragma pack(pop)