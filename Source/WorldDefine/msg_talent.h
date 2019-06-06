//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_talent.h
// author: 
// actor:
// data: 2008-10-20
// last:
// brief: ���ʼ��������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "RoleDefine.h"
#include "skill_define.h"

#pragma pack(push, 1)

enum
{
	E_LearnSkill_Existed				=		1,		// �ü����Ѿ�����
	E_LearnSkill_ProtoNotFound			=		2,		// û���ҵ��ü��ܶ�Ӧ�ľ�̬����
	E_LearnSkill_NPCNotEixst			=		3,		// NPC������
	E_LearnSkill_NPCTooFar				=		4,		// NPC����̫Զ
	E_LearnSkill_NPCCanNotTeach			=		5,		// NPC���ܽ��ڸü���
	E_LearnSkill_ItemNotFound			=		6,		// ��Ʒ������
	E_LearnSkill_ItemInvalid			=		7,		// ��Ʒ���Ǽ�����
	E_LearnSkill_NoPreSkill				=		8,		// û��ǰ�ü���
	E_LearnSkill_NeedMoreLevel			=		9,		// ����ȼ�����
	E_LearnSkill_NeedMoreTalentPoint	=		10,		// �ü�����Ҫ�����ʵ�Ͷ��������
	E_LearnSkill_ClassLimit				=		11,		// ְҵ����
	E_LearnSkill_NoTalentType			=		12,		// �ü�������������
	E_LearnSkill_NoTalentPoint			=		13,		// �����ʵ��Ͷ
	E_LearnSkill_ExceedTalentMaxNum		=		14,		// ���������Ͷ��������������
	E_LearnSkill_ProduceSkillFull		=		15,		// Ŀǰ������ϵ�������������

	E_LevelUpSkill_NotExist				=		21,		// ���ܲ�����
	E_LevelUpSkill_NotFixed				=		22,		// ���ܲ��ǹ̶�������ʽ
	E_LevelUpSkill_NoPreSkill			=		23,		// û��ǰ�ü���
	E_LevelUpSkill_ExceedMaxLevel		=		24,		// �������ܵȼ�����
	E_LevelUpSkill_ExceedMaxLearnLevel	=		25,		// �������ܿ�ѧ����
	E_LevelUpSkill_NeedMoreLevel		=		26,		// ����ȼ�����
	E_LevelUpSkill_NeedMoreTalentPoint	=		27,		// �ü�����Ҫ�����ʵ�Ͷ��������
	E_LevelUpSkill_ClassLimit			=		28,		// ְҵ����
	E_LevelUpSkill_NoTalentType			=		29,		// �ü�������������
	E_LevelUpSkill_NoTalentPoint		=		30,		// �����ʵ��Ͷ
	E_LevelUpSkill_ProtoNotFound		=		31,		// û���ҵ��ü��ܶ�Ӧ�ľ�̬����
	E_LevelUpSkill_NPCNotEixst			=		32,		// NPC������
	E_LevelUpSkill_NPCTooFar			=		33,		// NPC����̫Զ
	E_LevelUpSkill_NPCCanNotTeach		=		34,		// NPC���ܽ��ڸü���

	E_ForgetSkill_NotExist				=		51,		// ���ܲ�����
	E_ForgetSkill_CanNotBeForgotten		=		52,		// ���ּ��ܲ��ܱ�����
	E_ForgetSkill_NPCNotValid			=		53,		// NPC���Ϸ�

	E_ClearTalent_NoTalent				=		71,		// ����û�и�����
	E_ClearTalent_NoItem				=		72,		// ��ϴ����Ʒ
};


//------------------------------------------------------------------------------
// ѧϰһ���¼��ܣ�����Ϊ�����飬NPC�������飩
//------------------------------------------------------------------------------
CMD_START(NC_LearnSkill)
	DWORD	dwSkillID;				// ����ID����ID��
	DWORD	dwNPCID;				// NPC��ȫ��ID���������ͨ��NPC��ΪGT_INVALID��
	INT64	n64ItemID;				// ��������Ʒ��64λID���������ͨ�������飬��ΪGT_INVALID��
CMD_END

CMD_START(NS_LearnSkill)
	DWORD	dwErrorCode;			// ������
	DWORD	dwSkillID;				// ����ID����ID��
CMD_END

CMD_START(NS_AddSkill)
	tagSkillMsgInfo	Skill;			// ���һ������
CMD_END

//-------------------------------------------------------------------------------
// ����һ������
//-------------------------------------------------------------------------------
CMD_START(NC_LevelUpSkill)
	DWORD	dwSkillID;				// ����ID����ID��
	DWORD	dwNPCID;				// NPC��ȫ��ID���������ͨ��NPC��ΪGT_INVALID��
	INT64	n64ItemID;				// ��������Ʒ��64λID���������ͨ�������飬��ΪGT_INVALID��
CMD_END

CMD_START(NS_LevelUpSkill)
	DWORD	dwErrorCode;			// ������
	DWORD	dwSkillID;				// ����ID����ID��
CMD_END

CMD_START(NS_UpdateSkill)
	tagSkillMsgInfo	Skill;			// ����
CMD_END

CMD_START(NS_UpdateSkillCoolDown)
	DWORD			dwSkillID;		// ����ID
	INT				nCoolDown;		// �µ���ȴʱ��
CMD_END

//-------------------------------------------------------------------------------
// ����һ������
//-------------------------------------------------------------------------------
CMD_START(NC_ForgetSkill)
	DWORD	dwSkillID;		// ����ID����ID��
	DWORD	dwNPCID;		// NPCID
CMD_END

CMD_START(NS_ForgetSkill)
	DWORD	dwErrorCode;	// ������
	DWORD	dwSkillID;		// ����ID����ID��
CMD_END

//-------------------------------------------------------------------------------
// ϴ��
//-------------------------------------------------------------------------------
CMD_START(NC_ClearTalent)
	ETalentType	eType;		// ��������
	INT64		n64ItemID;	// ʹ�õĵ���
CMD_END

CMD_START(NS_ClearTalent)
	DWORD		dwErrorCode;// ������
	ETalentType	eType;		// ��������
CMD_END

CMD_START(NS_RemoveSkill)
	DWORD	dwSkillID;
CMD_END

//-------------------------------------------------------------------------------
// ����һ������
//------------------------------------------------------------------------------
CMD_START(NS_AddTalent)
	ETalentType	eType;
	INT			nPoint;
CMD_END

//------------------------------------------------------------------------------
// ɾ��һ������
//------------------------------------------------------------------------------
CMD_START(NS_RemoveTalent)
	ETalentType	eType;
CMD_END

//------------------------------------------------------------------------------
// ����һ������
//-----------------------------------------------------------------------------
CMD_START(NS_UpdateTalent)
	ETalentType	eType;
	INT			nPoint;
CMD_END

#pragma pack(pop)