//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: combat_handler.h
// author: Aslan
// actor:
// data: 2008-09-25
// last:
// brief: ս��ϵͳ������
//-----------------------------------------------------------------------------
#pragma once

class Unit;

class CombatHandler
{
public:
	enum ETargetEffectFlag
	{
		ETEF_Hited		=	0x0001,		// ����
		ETEF_Block		=	0x0002,		// ��
		ETEF_Crited		=	0x0004,		// ����
	};

	enum EInterruptType
	{
		EIT_Null		=	-1,			// ��
		EIT_Move		=	0,			// �ƶ����
		EIT_Skill		=	1,			// ����
	};

public:
	//----------------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------------
	CombatHandler();

	//----------------------------------------------------------------------------
	// ��ʼ������ʼ�����ºͽ���
	//----------------------------------------------------------------------------
	VOID	Init(Unit* pOwner);
	INT		UseSkill(DWORD dwSkillID, DWORD dwTargetUnitID, DWORD dwSerial);
	INT		UseItem(INT64 n64ItemID, DWORD dwTargetUnitID, DWORD dwSerial, DWORD &dwTypeID, bool& bImmediate);
	VOID	Update();
	VOID	End();

	//----------------------------------------------------------------------------
	// ȡ������ʹ��
	//----------------------------------------------------------------------------
	VOID	CancelSkillUse(DWORD dwSkillID);

	//----------------------------------------------------------------------------
	// ȡ����Ʒʹ��
	//----------------------------------------------------------------------------
	VOID	CancelItemUse(INT64 n64ItemSerial);

	//----------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------
	BOOL	InterruptPrepare(EInterruptType eType, BOOL bOrdinary, BOOL bForce=FALSE);

	//----------------------------------------------------------------------------
	// ����ͷ�
	//----------------------------------------------------------------------------
	BOOL	InterruptOperate(EInterruptType eType, DWORD dwMisc, BOOL bForce=FALSE);

	//----------------------------------------------------------------------------
	// ����mod
	//----------------------------------------------------------------------------
	VOID	ModSkillPrepareModPct(INT nModPct);
	VOID	ModTargetArmorLeftPct(INT nModPct);

	//----------------------------------------------------------------------------
	// ����Get
	//----------------------------------------------------------------------------
	BOOL	IsValid()					{ return IsUseSkill() || IsUseItem(); }
	BOOL	IsUseSkill()				{ return P_VALID(m_dwSkillID); }
	BOOL	IsUseItem()					{ return P_VALID(m_n64ItemID); }
	BOOL	IsPreparing()				{ return IsSkillPreparing() || IsItemPreparing(); }
	BOOL	IsOperating()				{ return IsSkillOperating() || IsItemOperating(); }
	INT		GetSkillPrepareCountDown()	{ return m_nSkillPrepareCountDown; }
	INT		GetItemPrepareCountDown()	{ return m_nItemPrepareCountDown; }
	DWORD	GetTargetUnitID()			{ return m_dwTargetUnitID; }
	DWORD	GetSkillID()				{ return m_dwSkillID; }
	FLOAT	GetTargetArmor(Unit &target);

	//----------------------------------------------------------------------------
	// ����ʹ���ж�
	//----------------------------------------------------------------------------
	INT		CanCastSkill(Skill* pSkill, DWORD dwTargetUnitID);

	INT		CheckSkillAbility(Skill* pSkill);							// ���Լ�������
	INT		CheckOwnerLimitSkill();											// ���Լ�������������
	INT		CheckSkillLimit(Skill* pSkill);								// ���Լ��ܱ�������
	INT		CheckTargetLimit(Skill* pSkill, DWORD dwTargetUnitID);		// ����Ŀ������
	INT		CheckCostLimit(Skill* pSkill);								// ������������
	INT		CheckVocationLimit(Skill* pSkill);							// ����ְҵ����
	INT		CheckTargetLogicLimit(Skill* pSkill, Unit* pTarget);		// ��⼼�ܺ�Ŀ���
	INT		CheckMapLimit(Skill* pSkill);								// ����ͼ����

	BOOL	CheckSkillConflict(Skill* pSkill);							// ��鼼��ʹ�õĳ�ͻ

	//----------------------------------------------------------------------------
	// ���㼼��Ч��
	//----------------------------------------------------------------------------
	DWORD	CalculateSkillEffect(Skill* pSkill, Unit* pTarget);
	VOID	CalSkillTargetList();
	BOOL	CalculateHit(Skill* pSkill, Unit* pTarget);
	BOOL	CalculateBlock(Skill* pSkill, Unit* pTarget);
	BOOL	CalculateCritRate(Skill* pSkill, Unit* pTarget);
	FLOAT	CalculateCritAmount(Skill* pSkill, Unit* pTarget);	
	VOID	CalculateDmg(Skill* pSkill, Unit* pTarget);
	VOID	CalculateCost(Skill* pSkill);

	//----------------------------------------------------------------------------
	// ��Ʒʹ���ж�
	//----------------------------------------------------------------------------
	INT		CanUseItem(tagItem* pItem);
	INT		CheckItemAbility(tagItem* pItem);							// �����Ʒ����
	INT		CheckOwnerLimitItem();										// �������
	INT		CheckRoleProtoLimit(tagItem* pItem);						// ���������������
	INT		CheckRoleStateLimit(tagItem* pItem);						// �������״̬����
	INT		CheckRoleVocationLimit(tagItem* pItem);						// �������ְҵ����
	INT		CheckMapLimit(tagItem* pItem);								// ����ͼ����
	BOOL	CheckItemConflict(tagItem* pItem);

	//-----------------------------------------------------------------------------
	// ������ƷЧ��
	//-----------------------------------------------------------------------------
	VOID	CalUseItemTargetList();
	
private:
	//-----------------------------------------------------------------------------
	// ����Get
	//-----------------------------------------------------------------------------
	bool	IsSkillPreparing()			{ return m_bSkillPreparing; }
	bool	IsSkillOperating()			{ return m_bSkillOperating; }
	bool	IsItemPreparing()			{ return m_bItemPreparing; }
	bool	IsItemOperating()			{ return m_bItemOperating; }

	//-----------------------------------------------------------------------------
	// Mod�ײ���ú���
	//-----------------------------------------------------------------------------
	VOID	ModPct(IN OUT FLOAT &fDstPct, IN INT nModPct);

	//-----------------------------------------------------------------------------
	// ���²���
	//-----------------------------------------------------------------------------
	VOID	UpdateSkillPrepare();
	VOID	UpdateSkillOperate();
	VOID	UpdateItemPrepare();
	VOID	UpdateItemOperate();

	//-----------------------------------------------------------------------------
	// ��������
	//-----------------------------------------------------------------------------
	VOID	EndUseSkill();
	VOID	EndUseItem();

	//----------------------------------------------------------------------------
	// �˺���ʽ��ϵ������
	//----------------------------------------------------------------------------
	FLOAT	CalBaseDmg(Skill* pSkill);
	FLOAT	CalAttackDefenceCoef(Skill* pSkill, Unit* pTarget);
	FLOAT	CalMoraleCoef(Unit* pTarget);
	FLOAT	CalDerateCoef(Skill* pSkill, Unit* pTarget);
	FLOAT	CalInjuryCoef();
	FLOAT	CalLevelCoef(Skill* pSkill, Unit* pTarget);

private:
	Unit*			m_pOwner;					// ������

	DWORD			m_dwSkillID;				// �����ļ���ID
	INT64			m_n64ItemID;				// ʹ����Ʒ64λID
	DWORD			m_dwTargetUnitID;			// ���ܷ�������Ŀ��ID
	DWORD			m_dwTargetEffectFlag;		// ��Ŀ�������Ч��
	DWORD			m_dwSkillSerial;			// ���ܹ������к�
	DWORD			m_dwItemSerial;				// ��Ʒʹ�����к�
	DWORD			m_dwTargetUnitIDItem;		// ʹ����Ʒ��Ŀ��ID

	bool			m_bSkillPreparing;			// �����Ƿ�������
	bool			m_bItemPreparing;			// ��Ʒ��������
	bool			m_bSkillOperating;			// �����Ƿ��ڷ���
	bool			m_bItemOperating;			// ��Ʒ�Ƿ��ڷ���

	INT				m_nSkillPrepareCountDown;	// �������ֵ���ʱ�����룩
	INT				m_nItemPrepareCountDown;	// ��Ʒ���ֵ���ʱ�����룩

	FLOAT			m_fSkillPrepareModPct;		// ��������ʱ��Ӱ��ٷֱ�
	FLOAT			m_fTargetArmorLeftPct;		// Ŀ�껤��������ʣ��ٷֱȣ�1.0f - �����ٷֱȣ�

	INT				m_nSkillOperateTime;		// ���ܲ�����ʱ�䣬���ڼ�������˺������룩
	INT				m_nSkillCurDmgIndex;		// ��ǰҪ����ڼ����˺�

	TList<DWORD>	m_listTargetID;				// ����Ŀ���б�
	TList<DWORD>	m_listHitedTarget;			// �������е�Ŀ��
	TList<DWORD>	m_listDodgedTarget;			// �������ܵ�Ŀ��
	TList<DWORD>	m_listBlockedTarget;		// ���ܸ񵲵�Ŀ��
	TList<DWORD>	m_listCritedTarget;			// ���ܱ�����Ŀ��
};

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
inline CombatHandler::CombatHandler()
: m_pOwner(NULL), m_dwSkillID(GT_INVALID), m_n64ItemID(GT_INVALID), m_dwTargetUnitID(GT_INVALID),
  m_dwTargetEffectFlag(0), m_dwSkillSerial(0), m_dwItemSerial(0), m_bSkillPreparing(false), m_bItemPreparing(false),
  m_bSkillOperating(false), m_bItemOperating(false), m_nSkillPrepareCountDown(0), m_nItemPrepareCountDown(0),
  m_fSkillPrepareModPct(1.0f), m_fTargetArmorLeftPct(1.0f), m_nSkillOperateTime(0), m_nSkillCurDmgIndex(0)
{
}

//-----------------------------------------------------------------------------
// ��ǰ���ܳ�ʼ��
//-----------------------------------------------------------------------------
inline VOID CombatHandler::Init(Unit* pOwner)
{
	m_pOwner	=	pOwner;
}

//------------------------------------------------------------------------------------------
// ����ʹ�ü���
//------------------------------------------------------------------------------------------
inline VOID CombatHandler::EndUseSkill()
{
	m_dwSkillID					=	GT_INVALID;
	m_dwTargetUnitID			=	GT_INVALID;
	m_dwTargetEffectFlag		=	0;
	m_dwSkillSerial				=	0;

	m_bSkillPreparing			=	false;
	m_bSkillOperating			=	false;

	m_nSkillPrepareCountDown	=	0;
	m_nSkillOperateTime			=	0;
	m_nSkillCurDmgIndex			=	0;
}

//-------------------------------------------------------------------------------------------
// ����ʹ����Ʒ
//-------------------------------------------------------------------------------------------
inline VOID CombatHandler::EndUseItem()
{
	m_n64ItemID					=	GT_INVALID;
	m_dwItemSerial				=	0;
	m_bItemPreparing			=	false;
	m_bItemOperating			=	false;
	m_nItemPrepareCountDown		=	0;
}

//--------------------------------------------------------------------------------------------
// ����ս��ϵͳ���ж���
//--------------------------------------------------------------------------------------------
inline VOID CombatHandler::End()
{
	EndUseSkill();
	EndUseItem();
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
inline VOID CombatHandler::Update()
{
	// �������ʹ�ü���
	if( IsUseSkill() )
	{
		if( IsSkillPreparing() )		UpdateSkillPrepare();
		else if( IsSkillOperating() )	UpdateSkillOperate();
		else							EndUseSkill();
	}

	// �������ʹ����Ʒ
	if( IsUseItem() )
	{
		if( IsItemPreparing() )			UpdateItemPrepare();
		else if( IsItemOperating() )	UpdateItemOperate();
		else							EndUseItem();
	}
}

//-----------------------------------------------------------------------------
// Mod�ײ���ú���
//-----------------------------------------------------------------------------
inline VOID CombatHandler::ModPct(IN OUT FLOAT &fDstPct, IN INT nModPct)
{
	fDstPct += (FLOAT)nModPct / 10000.0f;

	if(fDstPct < 0.0f)
	{
		// ���ٷֱȱ�Ϊ��ֵʱ���޷����з������
		ASSERT(fDstPct >= 0.0f);
		fDstPct = 0.0f;
	}
}

//----------------------------------------------------------------------------
// ��������ʱ��Ӱ��ٷֱ�
//----------------------------------------------------------------------------
inline VOID CombatHandler::ModSkillPrepareModPct(INT nModPct)
{
	ModPct(m_fSkillPrepareModPct, nModPct);
}

//----------------------------------------------------------------------------
// ���ö�Ŀ�껤��Ӱ��ٷֱ�
//----------------------------------------------------------------------------
inline VOID CombatHandler::ModTargetArmorLeftPct(INT nModPct)
{
	ModPct(m_fTargetArmorLeftPct, nModPct);
}
