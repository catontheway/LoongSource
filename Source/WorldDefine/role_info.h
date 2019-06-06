//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_info.h
// author: 
// actor:
// data: 2008-08-25
// last:
// brief: ���ݽ�ɫID�õ���ɫ���ƻ��ߣ����ݽ�ɫ����CRCֵ�õ���ɫID
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

CMD_START(NC_RoleGetID)		// �õ�ĳ��ɫID
	DWORD	dwRoleNameCrc;	// ��ɫ����crc32
CMD_END;

CMD_START(NS_RoleGetID)		// ����ĳ��ɫID	
	DWORD	dwRoleNameCrc;	// ��ɫ����crc32
	DWORD	dwRoleID;		// ����Ŀ����ҵ�roleid, = GT_INVALID��ʾ������
	TCHAR	szName[X_SHORT_NAME];	// name
CMD_END;

CMD_START(NC_GetNamebyNameID)	// �õ�name
	DWORD	dwNameID;			// NameID
CMD_END;

CMD_START(NS_GetNamebyNameID)		// ����name
	BOOL	bResult;				// TRUE = �ɹ�
	DWORD	dwNameID;				// ����NameID
	TCHAR	szName[X_SHORT_NAME];	// name
CMD_END;


// һ��������ID��Ӧ������
CMD_START(NC_RoleGetSomeName)
	INT		nUserData;
	INT		nNum;		// id������
	DWORD	dwAllID[1];	// �䳤����,���dwNum��id
CMD_END


// ���ID�����ֶ�Ӧ�Ľṹ
struct tagRoleIDName
{
	DWORD dwID;
	TCHAR szName[X_SHORT_NAME];
};

CMD_START(NS_RoleGetSomeName)
	INT				nUserData;
	INT				nNum;				// ��������
	tagRoleIDName	IdName[1];
CMD_END

#pragma pack(pop)



