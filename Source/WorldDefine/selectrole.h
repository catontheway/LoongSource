#pragma once

#pragma pack(push)
#pragma pack(1)

#include "msg_common_errorcode.h"
#include "RoleDefine.h"
#define MAX_ROLE_NUM		5
enum										// ���ﴴ����ɾ����ѡ����صĴ������
{
	E_JoinGame_AccountOnline			=	1,
	E_JoinGame_VerifyCodeError			=	2,

	E_CreateRole_RoleCreating			=	11,
	E_CreateRole_RoleEnumNotDone		=	12,
	E_CreateRole_RoleDeleting			=	13,
	E_CreateRole_RoleNumFull			=	14,
	E_CreateRole_NameExist				=	15,
	E_CreateRole_NameInvalid			=	16,
	E_CreateRole_SafeCodeInvalid		=	17,
	E_CreateRole_InWorld				=	18,
	E_CreateRole_RoleLoading			=	19,
	E_CreateRole_RoleEnumNotSuccess		=	20,

	E_DelRole_RoleNotExist				=	21,
	E_DelRole_RoleDeleting				=	22,
	E_DelRole_RoleEnumNotDone			=	23,
	E_DelRole_RoleCreating				=	24,
	E_DelRole_SafeCodeIncorrect			=	25,
	E_DelRole_InWorld					=	26,
	E_DelRole_RoleLoading				=	27,
	E_DelRole_LeaveGuild1st				=	28,	// �����������
	E_DelRole_BreakMarriage1st			=	29,	// �������

	E_EnumRole_RoleEnumed				=	31,
	E_EnumRole_RoleEnuming				=	32,
	E_EnumRole_InWorld					=	33,

	E_SelectRole_RoleEnumNotDone		=	41,
	E_SelectRole_RoleCreating			=	42,
	E_SelectRole_RoleDeleting			=	43,
	E_SelectRole_RoleNotExist			=	44,
	E_SelectRole_InWorld				=	45,
	E_SelectRole_RoleLoading			=	46,
};

//-----------------------------------------------------------------------------
// ������Ϸ
//-----------------------------------------------------------------------------
CMD_START(NC_JoinGame)
	DWORD		dwAccountID;		// �ʺ�ID
	DWORD		dwVerifyCode;		// ��½���������͸��ͻ��˵���֤��
CMD_END

CMD_START(NS_JoinGame)
	DWORD		dwErrorCode;
	INT			nGMPrivilege;		// GMȨ��
CMD_END

//-----------------------------------------------------------------------------
// �����ɫ�б�
//-----------------------------------------------------------------------------
CMD_START(NC_EnumRole)
CMD_END

CMD_START(NS_EnumRole)
	DWORD			dwErrorCode;
	DWORD			dwSafeCodeCrc;
	tagDWORDTime	dwTimeReset;
	tagDWORDTime	dwTimeLastLogin;	//�ϴε�¼ʱ��
	DWORD			dwIPLast;			//�ϴε�¼IP
	DWORD			dwIP;				//���ε�¼IP
	INT				nNum;
	BYTE			bySimpleInfo[1];
CMD_END


//������ȫ��
CMD_START(NC_SafeCode)
	DWORD			dwSafeCode;
CMD_END

CMD_START(NS_SafeCode)
	DWORD			dwErrorCode;
CMD_END

//���ð�ȫ��
CMD_START(NC_ResetSafeCode)
CMD_END

CMD_START(NS_ResetSafeCode)
	tagDWORDTime	dwTimeReset;
CMD_END

//ȡ����ȫ��
CMD_START(NC_CancelSafeCodeReset)
CMD_END

CMD_START(NS_CancelSafeCodeReset)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ������ɫ
//-----------------------------------------------------------------------------
CMD_START(NC_CreateRole)	
	TCHAR			szRoleName[X_SHORT_NAME];
	EPreChapStatus	eStatus;
	tagAvatarAtt	AvatarAtt;
CMD_END

CMD_START(NS_CreateRole)	
	DWORD			dwErrorCode;
	BYTE			bySimRoleInfo[1];
CMD_END

//-----------------------------------------------------------------------------
// ɾ����ɫ
//-----------------------------------------------------------------------------
CMD_START(NC_DeleteRole)
	DWORD			dwRoleID;						
	DWORD			dwSafeCodeCrc;
CMD_END

CMD_START(NS_DeleteRole)
	DWORD			dwErrorCode;
	DWORD			dwRoleID;
CMD_END

//------------------------------------------------------------------------------
// ѡ���ɫ
//------------------------------------------------------------------------------
CMD_START(NC_SelectRole)
	DWORD			dwRoleID;
CMD_END

CMD_START(NS_SelectRole)
	DWORD			dwErrorCode;
	DWORD			dwRoleID;
CMD_END

#pragma pack(pop)