//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: title_define.h
// author: 
// actor:
// data: 2009-01-20
// last:
// brief: �������óƺ���ؽṹ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)



//-----------------------------------------------------------------------------
// �ƺ���Ŀ
//-----------------------------------------------------------------------------
const UINT16 MAX_TITLE_NUM			= 80;

//-----------------------------------------------------------------------------
// �ƺ�ID�Ƿ���Ч
//-----------------------------------------------------------------------------
#define TITLEID_VALID( id ) ((id) >= 0 && (id) < MAX_TITLE_NUM)

//-----------------------------------------------------------------------------
// �����¼�
//-----------------------------------------------------------------------------
enum EnumTitleEvent
{	
	ETE_NULL						=-1,
	ETE_BEGIN						=0 ,	

	ETE_KILL_MONSTER				=0 ,	// ��ɱ����
	ETE_KILL_NPC					=1 ,	// ��ɱNPC
	ETE_KILL_BOSS					=2 ,	// ��ɱBoss
	ETE_KILL_ROLE					=3 ,	// ��ɱ��ɫ
	ETE_QUEST_COMPLETE				=4 ,	// �������
	ETE_QUEST_FAILED				=5 ,	// ����ʧ�� *
	ETE_ROLE_DIE					=6 ,	// ��ɫ���� *
	ETE_ROLE_KILLEDBYROLE			=7 ,	// ��ɫ��������ɫ��ɱ *
	ETE_COMPOSITE_EQUIP_SUCCESS		=8 ,	// �ϳ�װ���ɹ� *
	ETE_COMPOSITE_ITEM_SUCCESS		=9 ,	// �ϳ���Ʒ�ɹ� *
	ETE_STRENGTHEN_EQUIP_SUCCESS	=10,	// ǿ��װ���ɹ� *
	ETE_STRENGTHEN_EQUIP_FAILED		=11,	// ǿ��װ��ʧ�� *
	ETE_STRENGTHEN_EQUIP_PERFECT	=12,	// ǿ��װ������ *
	ETE_IDENTIFY_BLUE_EQUIP			=13,	// ��������ɫ����ɫ����Ʒ��
	ETE_STALL						=14,	// ��̯ *
	ETE_GATHER						=15,	// �ɼ����ջ�
	ETE_SHOUT						=16,	// ʹ�ô���
	ETE_ROLE_TRANSACTION_SUCCESS	=17,	// ���ɫ�ɹ�����
	ETE_CREATE_FACTION				=18,	// �������� *
	ETE_BE_CASTLLAN					=19,	// ��Ϊ���� *
	ETE_FRIEND_MAKE					=20,	// ��Ϊ����
	ETE_MARRY_JOIN					=21,	// ����Ѻö�
	ETE_MARRY_SEPARATION			=22,	// ��� * todo
	ETE_USE_ITEM					=23,	// ��ɫʹ�õ���
	ETE_USE_SKILL					=24,	// ��ɫʹ�ü���
	ETE_JOIN_FAMEHALL				=25,	// ��ɫ����ĳ�������� *
	ETE_ROLE_LEVEL					=26,	// ��ɫ�ﵽ����
	ETE_REPUTE_CHANGE				=27,	// �����仯 *
	ETE_FRIEND_VALUE				=28,	// �Ѻöȱ仯

	ETE_COMPOSITE_SKILL_SUCCESS		= 29,	// �ϳɳɹ�
	ETE_STRENGTHEN_SKILL_SUCCESS	= 30,	// ǿ������
	ETE_TRIGGER						= 31,	// ������
	ETE_SKILL_LEVEL					= 32,	// ��������
	ETE_ROLE_ATT					= 33,	// ��ɫ����ֵ
	ETE_BAG_ITEM					= 34,	// ��ɫ����װ����Ʒ 
	ETE_SCRIPT						= 35,	// �ű����
	ETE_MAPTRIGGER					= 36,	// ���������� * todo
	ETE_CLEARPOINT					= 37,	// ϴ�� * todo
	ETE_CLEARTALENT					= 38,	// ϴ���� * todo
	ETE_ONLINETIME					= 39,	// �����ۼ�ʱ�� * todo
	ETE_CLOTHCOLOR					= 40,	// ʱװȾɫ * todo
	ETE_MARRY						= 41,	// ��� * todo

	ETE_MAX_EVENTNUM				,	// �¼���Ŀ
	ETE_END								=ETE_MAX_EVENTNUM,
};

#define ETE_VALID( ete ) ( (ete) >= ETE_BEGIN && (ete) < ETE_END )
//-----------------------------------------------------------------------------
// �ƺ���������
//-----------------------------------------------------------------------------
enum EnumConditionType
{
	ECT_COUNT						= 0,	// ��������
	ECT_VALUE						= 1,	// ��ֵ����
	ECT_CHECK						= 2,	// ����������
};

//-----------------------------------------------------------------------------
// ������鷵��ֵ
//-----------------------------------------------------------------------------
enum EnumCheckResult
{
	ECR_ACTIVE						= 0,	// ����
	ECR_COUNTDOWN					= 1,	// ����
	ECR_NOAFECT						= 2,	// ��Ӱ��
};

struct tagTitleProto;


//-----------------------------------------------------------------------------
// �ƺ���������
//-----------------------------------------------------------------------------
class Condition
{
public:
	Condition(UINT16 u16TitleID, DWORD dwPara1, DWORD dwPara2)
		:m_u16TitleID(u16TitleID), m_dwPara1(dwPara1), m_dwPara2(dwPara2){}
	const tagTitleProto* GetTitleProto();
	bool IsCountCond() const;
	EnumCheckResult Check(DWORD dwArg1, DWORD dwArg2) {	return DoCheck(dwArg1, dwArg2);	}
	void SetPara2(DWORD dwPara2)	{	m_dwPara2 = dwPara2; }
	DWORD GetPara2() const {return m_dwPara2;}
	DWORD GetPara1() const {return m_dwPara1;}
protected:
//	Role* GetRole() const {return m_pRole;}
private:
	virtual EnumCheckResult DoCheck(DWORD dwArg1, DWORD dwArg2) = 0;

protected:
	DWORD	m_dwPara1;
	DWORD	m_dwPara2;
private:
	UINT16	m_u16TitleID;
//	Role*	m_pRole;
};


//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
class CountCondition: public Condition
{
public:
	CountCondition(UINT16 u16TitleID, DWORD dwPara1, DWORD dwPara2)
		:Condition(u16TitleID, dwPara1, dwPara2){}

private:
	virtual EnumCheckResult DoCheck(DWORD dwSubType, DWORD dwArg2)
	{
		if ( GT_VALID(m_dwPara1) && dwSubType != m_dwPara1) return ECR_NOAFECT; 
		if ((--m_dwPara2) <= 0)
		{
			return ECR_ACTIVE;
		}
		else
		{
			return ECR_COUNTDOWN;
		}
	}
};

//-----------------------------------------------------------------------------
// ��ֵ����
//-----------------------------------------------------------------------------
class ValueCondition: public Condition
{
public:
	ValueCondition(UINT16 u16TitleID, DWORD dwPara1, DWORD dwPara2)
		:Condition(u16TitleID, dwPara1, dwPara2){}
private:
	virtual EnumCheckResult DoCheck(DWORD dwSubType, DWORD dwValue)
	{
		if ( GT_VALID(m_dwPara1) && dwSubType != m_dwPara1) return ECR_NOAFECT; 
		if (dwValue >= m_dwPara2)
		{
			return ECR_ACTIVE;
		}
		else
		{
			return ECR_NOAFECT; 
		}
	}
};

//-----------------------------------------------------------------------------
// ���Ӽ������
//-----------------------------------------------------------------------------
class CheckCondition:public Condition
{
public:
	CheckCondition(UINT16 u16TitleID, DWORD dwPara1, DWORD dwPara2)
		:Condition(u16TitleID, dwPara1, dwPara2){}
private:
	virtual EnumCheckResult DoCheck(DWORD dwArg1, DWORD dwArg2)
	{
//		Role* pRole = GetRole();

		return ECR_NOAFECT;//pRole->CheckCondition();
	}
};

//-----------------------------------------------------------------------------
// �ƺ�ԭ��
//-----------------------------------------------------------------------------
struct tagTitleProto
{
	enum
	{
		EVENTSNUM	= 1,
	};

	UINT16				m_u16ID;
	DWORD				m_dwBuffID;

	EnumConditionType 	m_CondType;
	DWORD				m_dwPara1;
	DWORD				m_dwPara2;

	EnumTitleEvent		m_Events[EVENTSNUM];

	// ��������
	Condition* MakeCond(DWORD dwPara1, DWORD dwPara2) const 
	{
		dwPara1 = GT_VALID(dwPara1) ? dwPara1 : m_dwPara1;
		dwPara2 = GT_VALID(dwPara2) ? dwPara2 : m_dwPara2;
		switch (m_CondType)
		{
		case ECT_COUNT:
			return new CountCondition(m_u16ID, dwPara1, dwPara2);
			break;
		case ECT_VALUE:
			return new ValueCondition(m_u16ID, dwPara1, dwPara2);
			break;
		case ECT_CHECK:
			return new CheckCondition(m_u16ID, dwPara1, dwPara2);
			break;
		default:
			ASSERT(0);
			return NULL;
			break;
		}
	}
};

#pragma pack(pop)