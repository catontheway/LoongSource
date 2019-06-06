//-----------------------------------------------------------------------------
//!\file msg_show_item.h
//!
//!\date 2008-11-05
//!
//!\brief �ͻ����������֮�����չʾװ��,��Ʒ����Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "ItemDefine.h"
#include "container_define.h"

#pragma pack(push, 1)

//������
enum
{
	E_RoleShowItem_ForbidChat			    = 1,				//��ֹչʾ
	E_RoleShowItem_ServerNotReady			= 2,				//������û��׼����
	E_RoleShowItem_SendFailure			    = 3,				//������Ʒʧ��
	E_RoleShowItem_Frequently			    = 4,				//չʾ����

	E_RoleShowItem_World_NoItem				= 20,				//����,û�е���
	E_RoleShowItem_World_NoMoney			= 21,				//����,û���㹻����

	E_RoleShowItem_Pvt_RemoteRoleLeave		= 30,				//˽��,�Է�������
	E_RoleShowItem_Pvt_NoRoleName			= 31,				//˽��,û��Ŀ����� 
	E_RoleShowItem_Pvt_NoSelf				= 32,				//˽��,�Լ��������Լ�˽��

	E_RoleShowItem_Guild_NoJoin				= 40,				//����,û������

	E_RoleShowItem_Troop_NoJoin				= 50,				//����,û�ж���
};

//-----------------------------------------------------------------------------
//	չʾװ��
//-----------------------------------------------------------------------------
CMD_START(NC_RoleShowEquip)
	DWORD		 dwDestRoleID;	// ��˭չʾ
	BYTE		 byChannel;		// չʾƵ��
	EItemConType eConType;      // װ��λ��(�����ȵ�)
	INT64		 n64Serial;		// װ��64λID
CMD_END;

CMD_START(NS_RoleShowEquip)	
	DWORD		dwSrcRoleID;	// ��˭չʾ GT_INVALID����ϵͳ����
	DWORD		dwDestRoleID;	// �Ƕ�˭չʾ
	BYTE		byChannel;		// չʾƵ��
	DWORD		dwErrorCode;	// ������
	TCHAR	    szEquip[1];		// װ���ṹ tagEquip
CMD_END;

//-----------------------------------------------------------------------------
//	չʾ��Ʒ
//-----------------------------------------------------------------------------
CMD_START(NC_RoleShowItem)
	DWORD		 dwDestRoleID;	// ��˭չʾ
	BYTE		 byChannel;		// չʾƵ��
	EItemConType eConType;		// ��Ʒλ��(�����ȵ�)
	INT64		 n64Serial;		// ��Ʒ64λID
CMD_END;

CMD_START(NS_RoleShowItem)
	DWORD		dwSrcRoleID;	// ��˭չʾ GT_INVALID����ϵͳ����
	DWORD		dwDestRoleID;	// �Ƕ�˭չʾ
	BYTE		byChannel;		// չʾƵ��
	DWORD		dwErrorCode;	// ������
	DWORD	    dwTypeID;		// ��ƷTypeID tagItem
CMD_END;

// Jason 2009-11-28
// �ڶ���չʾʰȡ�ĵ�����Ʒ
// ֻ�����ڵĳ�Ա�㲥��Ϣ
CMD_START(NS_RoleShowDroptItemInTeam)
	DWORD		dwSrcRoleID;	// ��˭չʾ GT_INVALID����ϵͳ����
	BYTE		byChannel;		// չʾƵ��
	DWORD	    dwTypeID;		// ��ƷTypeID
	TCHAR		szPickerName[X_SHORT_NAME];
CMD_END


#pragma pack(pop)