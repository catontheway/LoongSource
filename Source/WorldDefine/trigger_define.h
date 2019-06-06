//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: trigger_define.h
// author: Aslan
// actor:
// data: 2008-09-08
// last:
// brief: ��������̬����
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//------------------------------------------------------------------------------
// �������¼�����
//------------------------------------------------------------------------------
enum ETriggerEventType
{
	ETEE_Null					=	0,
	ETEE_Use					=	1,		// ʹ�ã���Ʒʹ�ã���������ʹ��
	ETEE_Equip					=	2,		// װ����װ��װ��ʱ
	ETEE_BeAttacked				=	3,		// ��������ĳЩ�������ܴ�����װ��buff������AI����
	ETEE_Hit					=	4,		// ���У�������������Ŀ��ʱ�������������ܺ�װ������
	ETEE_Hited					=	5,		// �����У��������ܺ�װ��������AI����
	ETEE_Dodged					=	6,		// �����ܣ��������ܱ�Ŀ������ʱ�������������ܻ�װ��������ʱ����
	ETEE_Dodge					=	7,		// ���ܣ������˹���ʱ����
	ETEE_Blocked				=	8,		// ���񵲣��������ܱ�Ŀ������ʱ�������������ܻ�װ��������ʱ����
	ETEE_Block					=	9,		// �񵲣����˹���ʱ����
	ETEE_Crit					=	10,		// �������������ܲ�������ʱ�������������ܺ�װ������
	ETEE_Crited					=	11,		// ��������������ʱ����
	ETEE_Die					=	12,		// ����������ʱ������ĳЩ�������ܺ�װ���ᴥ����AI����
	ETEE_Random					=	13,		// ������������ܻ���װ��������AI����
	ETEE_InterCombat			=	14,		// ���뱸ս״̬�������AI��
	ETEE_DetectTarget			=	19,		// ����Ŀ�꣺AI��
	ETEE_InterCombatTime		=	20,		// ����ս����AI��
	ETEE_InterCombatInterTime	=	21,		// ����ս��ÿ��һ��ʱ�䣺AI��  ?????
	ETEE_TeammateDie			=	22,		// ����������AI��
	ETEE_Attack					=	23,		// ������ĳЩ�������ܴ�����װ��buff����
	ETEE_Kill					=	24,		// ��ɱ��ĳЩ�������ܴ�����װ��buff����
};

//------------------------------------------------------------------------------
// ������״̬����
//------------------------------------------------------------------------------
enum ETriggerStateType
{
	ETST_Null				=	0,
	ETST_HPLower			=	1,		// ��������
	ETST_HPHigher			=	2,		// ��������
	ETST_MPLower			=	3,		// ��������
	ETST_MPHigher			=	4,		// ��������
	ETST_RageLower			=	5,		// ŭ������
	ETST_RageHigher			=	6,		// ŭ������
	ETST_TeammateHPLower	=	7,		// ͬ�ӳ�Ա��������һ��ֵ
	ETST_TeammateMPLower	=	8,		// ͬ�ӳ�Ա��������һ��ֵ
	ETST_SelfHaveBuff		=	9,		// ����ӵ��ĳ״̬
	ETST_TargetHaveBuff		=	10,		// Ŀ��ӵ��ĳ״̬
	ETST_EnduranceLower		=	11,		// �־�������
	ETST_EnduranceHigher	=	12,		// �־�������
	ETST_Distance			=	13,		// �͹���Ŀ��ľ����Ƿ���ĳһ��Χ֮��
};

enum ECreatureTargetFriendEnemy
{
	ECFE_NULL				=	0,		// û��Ŀ�꣨Ⱥ�弼�ܣ�
	ECFE_Self				=	1,		// �Լ�
	ECFE_Leader				=	2,		// �ӳ�

	ECFE_CurPlayer			=	3,		// ����ĵ�ǰĿ�����
	ECFE_RandPlayer			=	4,		// �������б������һ�����
	ECFE_MinEnimty			=	5,		// �������б��ڳ����С�����
};


//------------------------------------------------------------------------------
// ��������̬����
//------------------------------------------------------------------------------
struct tagTriggerProto
{
	DWORD				dwID;
	ETriggerEventType	eEventType;		// �¼���������
	DWORD				dwEventMisc1;	// �¼���������
	DWORD				dwEventMisc2;	// �¼���������
	INT					nEventProp;		// �¼���������
	ETriggerStateType	eStateType;		// ״̬��������
	DWORD				dwStateMisc1;	// ״̬��������
	DWORD				dwStateMisc2;	// ״̬��������
};

#pragma pack(pop)