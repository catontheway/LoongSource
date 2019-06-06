//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: buff.h
// author: Aslan
// actor:
// data: 2008-9-12
// last:
// brief: ״̬
//-------------------------------------------------------------------------------
#pragma once

class Unit;

//--------------------------------------------------------------------------------
// ��ӵ����ϵ�buff
//--------------------------------------------------------------------------------
class Buff
{
public:
	enum EBuffState				// Buff״̬
	{
		EBS_Idle		=	0,	// ����
		EBS_Initing		=	1,	// ��ʼ��
		EBS_Updating	=	2,	// ����
		EBS_Destroying	=	3,	// ɾ��
	};

public:
	Buff();
	~Buff();

	//----------------------------------------------------------------------------
	// ��ʼ�������º����ٺͱ���
	//----------------------------------------------------------------------------
	VOID			Init(const tagBuffProto* pBuffProto, Unit* pTarget, Unit* pSrc, DWORD dwSrcSkillID, 
						const tagItem* pItem, INT nIndex, TList<DWORD>* listModifier=NULL);
	VOID			Init(Unit* pTarget, const tagBuffSave* pBuffSave, INT nIndex);
	VOID			InitBuffSave(OUT tagBuffSave *pBuffSave, OUT INT32 &nSize);
	BOOL			Update();
	VOID			Destroy(BOOL bSelf);

	//----------------------------------------------------------------------------
	// ���Ӻʹ�Ϻʹ���
	//----------------------------------------------------------------------------
	VOID			Wrap();
	BOOL			Interrupt(EBuffInterruptFlag eFlag, INT nMisc=GT_INVALID);
	BOOL			OnTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------
	// ����Get
	//----------------------------------------------------------------------------
	BOOL			IsValid()				{ return P_VALID(m_dwID); }
	DWORD			GetID()					{ return m_dwID; }
	DWORD			GetTypeID()				{ return m_pProto->dwID; }
	INT				GetLevel()				{ return m_nLevel; }
	INT				GetGroupLevel()			{ return m_pProto->nLevel; }
	INT				GetIndex()				{ return m_nIndex; }
	DWORD			GetSrcUnitID()			{ return m_dwSrcUnitID; }
	DWORD			GetSrcSkillID()			{ return m_dwSrcSkillID; }
	DWORD			GetGroupFlag()			{ return m_pProto->dwGroupFlag; }
	DWORD			GetBuffInterruptID()	{ return m_pProto->dwBuffInterruptID; }
	INT				GetWrapTimes()			{ return m_nWrapTimes; }
	EBuffResistType	GetResistType()			{ return m_pProto->eResistType; }
	EBuffEffectType	GetEffectType(EBuffEffectMode eMod)			{ return m_pProto->eEffect[eMod]; }
	EBuffState		GetState()				{ return m_eState; }

	INT				GetPersistTimeLeft()	{ return ( !IsPermanent() ? ((GetMaxPersistTick() - m_nPersistTick) * TICK_TIME) : GT_INVALID); }
	INT				GetPersistTime()		{ return ( !IsPermanent() ? GetMaxPersistTick() * TICK_TIME : GT_INVALID); }

	//-----------------------------------------------------------------------------
	// һЩ����ľ�̬����
	//-----------------------------------------------------------------------------
	static DWORD	GetIDFromTypeID(DWORD dwTypeID)						{ return dwTypeID / 100; }
	static INT		GetLevelFromTypeID(DWORD dwTypeID)					{ return dwTypeID % 100; }
	static DWORD	GetTypeIDFromIDAndLevel(DWORD dwID, INT nLevel)		{ return dwID * 100 + nLevel; }

private:
	//-----------------------------------------------------------------------------
	// ״̬�л�
	//-----------------------------------------------------------------------------
	VOID			BeginInit()			{ m_eState = EBS_Initing; }
	VOID			EndInit()			{ m_eState = EBS_Idle; }
	VOID			BeginUpdate()		{ m_eState = EBS_Updating; }
	VOID			EndUpdate()			{ m_eState = EBS_Idle; }
	VOID			BeginDestroy()		{ m_eState = EBS_Destroying; }
	VOID			EndDestroy()		{ m_eState = EBS_Idle; }

	//----------------------------------------------------------------------------
	// һЩGet����
	//----------------------------------------------------------------------------
	INT				GetMaxPersistTick();
	INT				GetMaxWrapTimes();
	INT				GetAttackInterrupt();

	//----------------------------------------------------------------------------
	// ������Buff�ͼ��������buff�ж�
	//----------------------------------------------------------------------------
	INT				IsPermanent();
	INT				IsInterOP();

	//----------------------------------------------------------------------------
	// ��������ID
	//----------------------------------------------------------------------------
	DWORD			CreateTypeID(DWORD dwID, INT nLevel)	{ return dwID * 100 + nLevel; }

private:
	EBuffState						m_eState;						// ��ǰ״̬
	DWORD							m_dwID;							// ID
	INT								m_nLevel;						// �ȼ�
	INT								m_nIndex;						// ��Ӧ��Buff�е�����
	DWORD							m_dwSrcUnitID;					// ԴUnit��ID
	DWORD							m_dwSrcSkillID;					// ���ĸ����ܲ�������ID��
	INT64							m_n64ItemID;					// ���ĸ���Ʒ��װ������
	DWORD							m_dwItemTypeID;					// ��Ʒ��װ��������ID

	INT								m_nPersistTick;					// ��ǰ�ĳ���ʱ��
	INT								m_nCurTick;						// ��ǰ����ʱ�� 
	INT								m_nWrapTimes;					// ��ǰ���Ӵ���

	Unit*							m_pOwner;						// ��˭����
	const tagBuffProto*				m_pProto;						// ��̬����
	const tagTriggerProto*			m_pTrigger;						// Ч��������
	tagBuffMod*						m_pMod;							// ���Լӳ�


	//--------------------------------------------------------------------------------------
	//ʱ���ӳټ���  
	//--------------------------------------------------------------------------------------
	DWORD                             m_dwTimeCountFlags;  

};

//-------------------------------------------------------------------------------------
// �õ�Buff��������ʱ��
//-------------------------------------------------------------------------------------
inline INT Buff::GetMaxPersistTick()
{
	INT nPersistTick = m_pProto->nPersistTick;
	if( P_VALID(nPersistTick) && P_VALID(m_pMod) )
	{
		nPersistTick = nPersistTick + m_pMod->nPersistTickMod;
	}
	return nPersistTick;
}

//-------------------------------------------------------------------------------------
// �õ�Buff�������Ӵ���
//-------------------------------------------------------------------------------------
inline INT Buff::GetMaxWrapTimes()
{
	return m_pProto->nWarpTimes + ( P_VALID(m_pMod) ? m_pMod->nWarpTimesMod : 0 );
}

//--------------------------------------------------------------------------------------
// �õ�Buff�Ĺ�������ϼ���
//--------------------------------------------------------------------------------------
inline INT Buff::GetAttackInterrupt()
{
	return m_pProto->nAttackInterruptRate + (P_VALID(m_pMod) ? m_pMod->nAttackInterruptRateMod : 0);
}

//--------------------------------------------------------------------------------------
// Buff�Ƿ�Ϊ������Buff
//--------------------------------------------------------------------------------------
inline INT Buff::IsPermanent()
{
	return !P_VALID(m_pProto->nPersistTick);
}

//--------------------------------------------------------------------------------------
// Buff�Ƿ�Ϊ���������Buff
//--------------------------------------------------------------------------------------
inline INT Buff::IsInterOP()
{
	return m_pProto->nInterOPTick > 0;
}
