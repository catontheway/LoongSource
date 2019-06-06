//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_issue.h
// author: 
// actor:
// data: 2008-07-21
// last:
// brief: ��ҹ��ڵ�ͼ��������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

enum
{
	E_BindRebornMap_NPCInvalid	=	1,	// ְ��NPC�Ƿ�
	E_BindRebornMap_MapInvalid	=	2,	// ��ͼ�Ƿ���������ͨ��ͼ��
	E_BindRebornMap_Already		=	3,	// �Ѿ����ڸõ�ͼ
	E_BindRebornMap_NPCNotFound	=	4,	// û���ҵ�ָ����NPC
	E_BindRebornMap_TooFar		=	5,	// ����̫Զ

	E_Instance_Not_Exit			=	11,	// ����������
	E_Instance_Role_Not_Exit	=	12,	// ��Ҳ�����
	E_Instance_Role_Full		=	13,	// ���������ѵ�����
	E_Instance_Level_Down_Limit =	14,	// ������͵ȼ�����
	E_Instance_Level_Up_Limit	=	15,	// ������ߵȼ�����
	E_Instance_Not_Same_Team	=	16,	// ����ͬһС��
	E_Instance_Role_Lack		=	17,	// ��������δ������
	E_Instance_End_Delete		=	18,	// �����Ѿ����������Ѿ��ر�
	E_Instance_Already			=	19,	// �Ѿ���������������
	E_Instance_Full				=	20,	// ����������������
};

enum E_SET_TRANSPORT_ERROR_CODE
{	
	EST_SuccessSet				=	E_Success,	//���óɹ�
	EST_InInstance				=	1,			//���ڸ�����
	EST_LackNeedItem			=	2,			//ȱ����Ҫ����Ʒ
	EST_SpecFunError			=	3,			//������Ʒ���ʹ���
	EST_SelfStateLimit			=	4,			//����״̬����
	EST_ErrEnd
};

//------------------------------------------------------------------------------
// �ȵ���ͼ������
//------------------------------------------------------------------------------
CMD_START(NC_RoleMapTrigger)
	DWORD	dwTriggerID;
	DWORD	dwMisc;			// �������ͨ������������������Ѷȣ�
CMD_END

//------------------------------------------------------------------------------
// ���븱��
//------------------------------------------------------------------------------
CMD_START(NS_EnterInstance)
	DWORD	dwTimeLimit;	// ʱ�޸�����ʣʱ�䣬��λ����( ��ʱ�޸���ΪGT_INVALID��
	DWORD	dwErrorCode;
CMD_END;

//-----------------------------------------------------------------------------
// ȥ�µ�ͼ
//-----------------------------------------------------------------------------
CMD_START(NS_GotoNewMap)
	DWORD	dwMapID;
	Vector3	pos;
	Vector3	faceTo;
CMD_END

//-----------------------------------------------------------------------------
// ֪ͨ��������Ƿ��ٻ�����
//-----------------------------------------------------------------------------
CMD_START(NS_InstanceNofity)
CMD_END

CMD_START(NC_InstanceNotify)	
	bool	bNotify;
CMD_END

//-----------------------------------------------------------------------------
// ֪ͨ���ж����Ƿ���븱��
//-----------------------------------------------------------------------------
CMD_START(NS_InstanceAgree)
	DWORD   dwInviterID;
	DWORD	dwMapID;
	DWORD	dwMisc;
CMD_END

CMD_START(NC_InstanceAgree)
	bool	bAgree;
CMD_END

//-----------------------------------------------------------------------------
// ��������������
//-----------------------------------------------------------------------------
CMD_START(NS_InstanceComplete)
CMD_END

//-----------------------------------------------------------------------------
// �����뿪����
//-----------------------------------------------------------------------------
CMD_START(NC_LeaveInstance)
CMD_END

//-----------------------------------------------------------------------------
// ͬ����������ʱ
//-----------------------------------------------------------------------------
CMD_START(NS_SyncInstanceTime)
	DWORD	dwTimeLeft;			// ����ʣ��ʱ�䣺��λ����
CMD_END

//-----------------------------------------------------------------------------
// �󶨸����
//-----------------------------------------------------------------------------
CMD_START(NC_BindRebornMap)
	DWORD	dwNPCID;		// ְ��NPCID
CMD_END

CMD_START(NS_BindRebornMap)
	DWORD	dwErrorCode;
	DWORD	dwBornMapID;
	Vector3	vBornPos;
CMD_END

//-----------------------------------------------------------------------------
// ���ÿɼ�¼���ͷ�
//-----------------------------------------------------------------------------
CMD_START(NC_SetTransportMap)
INT64   n64ItemID;      // �ɼ�¼���ͷ�
CMD_END

CMD_START(NS_SetTransportMap)
E_SET_TRANSPORT_ERROR_CODE  dwErrorCode;
DWORD                       dwTransportMapID;
Vector3                     vTransportPoint;
CMD_END

//-----------------------------------------------------------------------------
// ʹ�ÿɼ�¼���ͷ�
//-----------------------------------------------------------------------------
CMD_START(NC_UseTransportMap)
INT64		n64ItemID;			//��Ʒ64λID
CMD_END

#pragma pack(pop)

