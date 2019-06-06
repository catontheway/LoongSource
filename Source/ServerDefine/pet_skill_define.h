#pragma once
#include "../WorldDefine/pet_skill_define.h"

//-----------------------------------------------------------------------------
// ���＼��Ӱ������������Ŀ
//-----------------------------------------------------------------------------
const INT MAX_PET_SKILL_MOD_ATT		= 6;

//-----------------------------------------------------------------------------
// ���＼��ԭ��(�Ѹ�Ϊ�ͻ��ˣ����������ֶ���)
//-----------------------------------------------------------------------------
struct tagPetSkillProto
{
	DWORD				dwTypeID;			// ���＼��ID
	EPetskillType		eType;				// ���＼������
	EPetskillType2		eType2;				// ���＼������2
	INT					nType3;				// ���＼������3

	EPetskillCastType	eCastType;			// �ͷ�����
	INT					nSkillLevel;		// ���ܵȼ�
	BYTE				byCast_condition;	// ʩ��״̬
	INT					nCooldownTick;		// �ָ�ʱ��
	INT					nWorkTimeTick;			// ����ʱ��
	INT					nWuxing_cost;		// ����������
	INT					nSpirit_cost;		// ��������
	INT					nWuxing_add;		// ����������
	INT					nBuffid;			// ״̬
	BOOL	            bLearn_condition;	// ѧϰ����
	INT					nLearn_prob;		// ѧϰ����
	INT					nLearn_step;		// ѧϰ��
	INT					nLearn_grade;		// ѧϰ��
	INT					nLearn_PontentialCost;	// ѧϰ��

	INT					nPetLvlLim;			// ��Ӧ����ԭ���������ȼ�����

	INT					AttIndexs[MAX_PET_SKILL_MOD_ATT];	// �ӳ���������
	INT					AttMods[MAX_PET_SKILL_MOD_ATT];		// �ӳ�ֵ

	INT					nPetAttIndex;
	INT					nPetAttMod;
};

const INT	MAX_WUXING_ITEM_NUM		= 10;

struct tagPetWuXingProto
{
	DWORD				dwTypeID;
	DWORD				dwItemTypeID[MAX_WUXING_ITEM_NUM];
	INT					nProb[MAX_WUXING_ITEM_NUM];
	INT					nNum[MAX_WUXING_ITEM_NUM];
	BOOL				bNotice[MAX_WUXING_ITEM_NUM];
};

struct tagPetSkillCmdParam
{
	tagPetSkillCmdParam(PVOID pInputBuf, PVOID pOutBuf, DWORD& dwSize)
		:m_pInputBuf(pInputBuf), m_pOutBuf(pOutBuf), m_dwOutSize(dwSize){	}

	PVOID	m_pInputBuf;
	PVOID	m_pOutBuf;
	DWORD&	m_dwOutSize;
};