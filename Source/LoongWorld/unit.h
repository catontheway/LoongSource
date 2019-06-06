//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: unit.h
// author: Aslan
// actor:
// data: 2008-8-5
// last:
// brief: �������NPC���������Ļ���
//-------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/move_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/msg_combat.h"

#include "move_data.h"
#include "world.h"
#include "map.h"
#include "skill.h"
#include "buff.h"
#include "combat_handler.h"
#include "state_count_mgr.h"

//------------------------------------------------------------------------------
// ��Ŀ��ĵ���������ϵ
//------------------------------------------------------------------------------
enum ETargetFriendEnemy
{
	ETFE_Friendly		=	0x0001,		// �ѷ�
	ETFE_Hostile		=	0x0002,		// �з�
	ETFE_Independent	=	0x0004,		// ����
};

//-------------------------------------------------------------------------------
// Buff�����͵��Ӽ����Ľ��
//-------------------------------------------------------------------------------
enum EBuffCounteractAndWrapResult
{
	EBCAWR_CanNotAdd		=	0,		// �����������������
	EBCAWR_CanAdd			=	1,		// ������һ��Buff�����߲���Ҫ�������������
	EBCAWR_Wraped			=	2,		// �����ӽ�һ��Buff��Ѿ����
};

//---------------------------------------------------------------------------------
// Buff����
//---------------------------------------------------------------------------------
const INT MAX_BENEFIT_BUFF_NUM		=	35;		// ���е�����buff����
const INT MAX_DEBUFF_NUM			=	15;		// ���е��к�buff����
const INT MAX_BUFF_NUM				=	MAX_BENEFIT_BUFF_NUM + MAX_DEBUFF_NUM;

//--------------------------------------------------------------------------------
// ��Ѫ�����ػ����س־����ļ��ʱ��
//--------------------------------------------------------------------------------
const INT HP_MP_REGAIN_INTER_TICK		=	3 * TICK_PER_SECOND;	// ��Ѫ�����ļ��ʱ��
const INT VITALITY_REGAIN_INTER_TICK	=	60* TICK_PER_SECOND;	// �ػ����ļ��ʱ��
const INT ENDURANCE_REGAIN_INTER_TICK	=	TICK_PER_SECOND;		// �س־�����ʱ����

//--------------------------------------------------------------------------------
// �ص��¼��ʱ��ͻָ�ֵ
//--------------------------------------------------------------------------------
const INT INPRISON_MORALE_REGAIN_INTER_TICK	= 60 * TICK_PER_SECOND;	// �ص���TICK
const INT INPRISON_MORAL_INCREASE_VAL						= 8;					// �ص���ֵ

//--------------------------------------------------------------------------------
// �����˼��ʱ��ͻָ�ֵ
//--------------------------------------------------------------------------------
const INT INJURY_REGAIN_INTER_TICK		= 1 * TICK_PER_SECOND * 60 * 10;	// �ָ�����TICK
const INT INJURY_INCREASE_VAL			= -1;								// ���˻ָ�ֵ

class Skill;
class Buff;
class Unit;

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
struct tagEnmity
{
	DWORD			dwID;						// ��˭�ĳ��
	INT				nEnmity;					// �������
	INT				nEnmityMod;					// ��������ӳ�
	BOOL			bActive;					// �Ƿ񼤻�
	tagEnmity()	{ dwID = GT_INVALID; nEnmity = 0; nEnmityMod = 0; bActive=TRUE; }
};
class PetStrengthSkill;
class Unit
{
	friend class PetStrengthSkill;
public:
	//----------------------------------------------------------------------------
	// Constructor&Destructor
	//----------------------------------------------------------------------------
	Unit(DWORD dwID, DWORD dwMapID, FLOAT fX, FLOAT fY, FLOAT fZ, FLOAT fFaceX, FLOAT fFaceY, FLOAT fFaceZ);
	virtual ~Unit();

	//-------------------------------------------------------------------------------
	// ����Update����
	//-------------------------------------------------------------------------------
	virtual VOID	Update() {}
	VOID			UpdateTimedIssue();

	//----------------------------------------------------------------------------
	// �����ʲô�Ӷ���
	//----------------------------------------------------------------------------
	BOOL			IsRole()					{ return IS_PLAYER(m_dwID); }
	BOOL			IsCreature()				{ return IS_CREATURE(m_dwID) || IS_PET(m_dwID); }

	//-------------------------------------------------------------------------------
	// ����Get
	//-------------------------------------------------------------------------------
	DWORD			GetID()	const				{ return m_dwID; }
	virtual BYTE	GetSex() const				{ return 0; }
	DWORD			GetMapID()					{ return m_dwMapID; }
	Map*			GetMap()					{ return m_pMap; }
	INT				GetLevel()					{ return m_nLevel; }
	INT				GetCurLevelExp()			{ return m_nCurLevelExp; }
	MoveData&		GetMoveData()				{ return m_MoveData; }
	CombatHandler&	GetCombatHandler()			{ return m_CombatHandler; }

	//-------------------------------------------------------------------------------
	// ����Set
	//-------------------------------------------------------------------------------
	VOID			SetMap(Map* pMap)			{ m_pMap = pMap; }
	VOID			SetMapID(DWORD dwMapID)		{ m_dwMapID = dwMapID; }
	VOID			SetLevel(INT nLevel)		{ m_nLevel = nLevel; }
	VOID			SetCurLevelExp(INT nExp)	{ m_nCurLevelExp = nExp; }
	VOID			SetSize(Vector3 vSize)					{ m_Size = vSize;	}
	//--------------------------------------------------------------------------------
	// �ƶ�״̬���
	//--------------------------------------------------------------------------------
	EMoveState		GetMoveState()				{ return m_MoveData.m_eCurMove; }
	Vector3&		GetSize()					{ return m_Size; }
	Vector3&		GetCurPos()					{ return m_MoveData.m_vPos; }
	Vector3&		GetStartPos()				{ return m_MoveData.m_vPosStart; }
	Vector3&		GetDestPos()				{ return m_MoveData.m_vDest; }
	Vector3&		GetMoveDir()				{ return m_MoveData.m_vDir; }
	Vector3&		GetFaceTo()					{ return m_MoveData.m_vFace; }
	FLOAT			GetMovePassTime()			{ return m_MoveData.m_Timer.GetElapse() - m_MoveData.m_fStartTime; }
	Vector3			GetCurPosTop()				{ Vector3 vec = m_MoveData.m_vPos; vec.y = vec.y + m_Size.y; return vec; }
	AABBox			GetAABBox() const;
	INT				GetVisTileIndex() const		{ return m_MoveData.m_nVisTileIndex; }
	VOID			SetVisTileIndex(INT nIndex)	{ m_MoveData.m_nVisTileIndex = nIndex; }

	//---------------------------------------------------------------------------------
	// �ٶ����
	//---------------------------------------------------------------------------------
	FLOAT			GetXZSpeed();			//{ return m_fXZSpeed; }
	FLOAT			GetYSpeed()	const			{ return m_fYSpeed; }
	FLOAT			GetSwimXZSpeed() const		{ return m_fSwimXZSpeed; }
	FLOAT			GetDropXZSpeed() const		{ return m_fDropXZSpeed; }
	FLOAT			GetSlideSpeed() const		{ return m_fSlideSpeed; }

	//--------------------------------------------------------------------------------
	// �����ж�
	//--------------------------------------------------------------------------------
	BOOL			IsInDistance(Unit& target, FLOAT fDis);
	BOOL			IsInCombatDistance(Unit& target, FLOAT fDis);
	BOOL			IsInFrontOfTarget(Unit& target);
	BOOL			IsRayCollide(Unit& target);

	//--------------------------------------------------------------------------------
	// ���Զ�ȡ�����ü��޸�
	//--------------------------------------------------------------------------------
	INT				GetAttValue(INT nIndex);
	INT				GetBaseAttValue(INT nIndex);
	INT				GetAttModValue(INT nIndex);
	INT				GetAttModValuePct(INT nIndex);
	VOID			SetAttValue(INT nIndex, INT nValue, BOOL bSendMsg=TRUE);
	VOID			SetBaseAttValue(INT nIndex, INT nValue);
	VOID			SetAttModValue(INT nIndex, INT nValue);
	VOID			SetAttModValuePct(INT nIndex, INT nValuePct);
	VOID			ModAttValue(INT nIndex, INT nValueMod, BOOL bSendMsg=TRUE);
	VOID			ModBaseAttValue(INT nIndex, INT nValueMod);
	VOID			ModAttModValue(INT nIndex, INT nValueMod);
	VOID			ModAttModValuePct(INT nIndex, INT nValuePctMod);

	ERemoteRoleAtt	ERA2ERRA(ERoleAttribute eRA);
	ERoleAttribute	SkillDmgType2ERA(ESkillDmgType eSKillDmgType);
	ERoleAttribute	BuffResistType2ERA(EBuffResistType eBuffResistType);

	BOOL			IsTeamRemoteAtt(ERemoteRoleAtt eType);

	//------------------------------------------------------------------------------
	// ĳЩ���Ե��޸�
	//------------------------------------------------------------------------------
	VOID			ChangeHP(INT nValue, Unit* pSrc=NULL, Skill* pSkill=NULL, const tagBuffProto* pBuff=NULL, bool bCrit=false, DWORD dwSerial=GT_INVALID, DWORD dwMisc=GT_INVALID);
	VOID			ChangeMP(INT nValue);
	VOID			ChangeVitality(INT nValue);
	VOID			ChangeEndurance(INT nValue);
	VOID			ChangeRage(INT nValue);

	//------------------------------------------------------------------------------
	// ��ǰ״̬ -- ��״̬���ת������Ҫ�ֶ���ɡ���SetState()�����Զ��������״̬λ��
	//------------------------------------------------------------------------------
	DWORD			GetState()							{ return m_StateMgr.GetStateFlags(); }
	BOOL			IsInState(EState eState)			{ return m_StateMgr.IsInState(eState); }

	BOOL			IsDead()							{ return IsInState(ES_Dead); }
	virtual BOOL	IsInStateCantMove()					{ return IsInState(ES_Dead) || IsInState(ES_Dizzy) || IsInState(ES_Tie) || IsInState(ES_Spor); }
	virtual BOOL	IsInStateCantBeSkill()				{ return FALSE; }
	virtual BOOL	IsInStateCantCastSkill()			{ return IsInState(ES_Dead); }
	virtual BOOL	IsInStateInvisible()				{ return IsInState(ES_Lurk); }
	virtual BOOL	IsInStateCantBeInviteJoinTeam()		{ return FALSE; } 	//�Ƿ��ڲ����Ա�������ӵ�״̬

	VOID			ClearState()						{ m_StateMgr.Reset(); }

	VOID			SetState(EState eState, BOOL bSendMsg=TRUE);
	VOID			UnsetState(EState eState, BOOL bSendMsg=TRUE);

	VOID			OnSetState(EState eState);
	VOID			OnUnSetState(EState eState);

	//----------------------------------------------------------------------------------
	// ս��ϵͳĿ�����ͣ�״̬�����ҹ�ϵ�ж�
	//----------------------------------------------------------------------------------
	virtual DWORD	TargetTypeFlag(Unit* pTarget) = 0;
	virtual DWORD	GetStateFlag();
	virtual DWORD	FriendEnemy(Unit* pTarget) = 0;

	//----------------------------------------------------------------------------------
	// ս��ϵͳ�¼����
	//----------------------------------------------------------------------------------
	virtual VOID	OnBeAttacked(Unit* pSrc, Skill* pSkill, BOOL bHited, BOOL bBlock, BOOL bCrited) = 0;
	virtual VOID	OnDead(Unit* pSrc, Skill* pSkill=NULL, const tagBuffProto* pBuff=NULL, DWORD dwSerial=GT_INVALID, DWORD dwMisc=0) = 0;
	virtual VOID	OnKill(Unit* pTarget) = 0;

	//----------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------
	Skill*			GetSkill(DWORD dwSkillID)			{ return m_mapSkill.Peek(dwSkillID); }
	INT				GetProduceSkillNum()				{ return m_nProduceSkillNum; }
	VOID			UpdateSkill();
	virtual VOID	StartSkillCoolDown(Skill* pSkill);
	virtual VOID	ClearSkillCoolDown(DWORD dwSkillID);
	VOID			ClearAllSkillCoodDown(ETalentType eType, DWORD dwExceptSkillID=GT_INVALID);

	//----------------------------------------------------------------------------------
	// Buff����
	//----------------------------------------------------------------------------------
	TList<DWORD>*	GetBuffModifier(DWORD dwBuffID)		{ return m_mapBuffModifier.Peek(dwBuffID); }
	VOID			RegisterBuffModifier(Skill* pSkill);
	VOID			UnRegisterBuffModifier(Skill* pSkill);
	VOID			RegisterTriggerModifier(Skill* pSkill);
	VOID			UnRegisterTriggerModifier(Skill* pSkill);

	BOOL			OnActiveSkillBuffTrigger(Skill* pSkill, TList<DWORD>& listTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);
	BOOL			OnBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//-----------------------------------------------------------------------------------
	// Buff���
	//-----------------------------------------------------------------------------------
	INT				GetBuffNum()						{ return m_mapBuff.Size(); }
	Buff*			GetBuffPtr(DWORD dwBuffID)			{ return m_mapBuff.Peek(dwBuffID); }
	BOOL			IsHaveBuff(DWORD dwBuffID)			{ return m_mapBuff.IsExist(dwBuffID); }

	VOID			GetAllBuffMsgInfo(tagBuffMsgInfo* const pBuffInfo, INT nMaxNum);
	BOOL			TryAddBuff(Unit* pSrc, const tagBuffProto* pBuffProto, const tagTriggerProto* pTriggerProto, Skill* pSkill, tagItem* pItem, BOOL bReCalAtt=TRUE, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);
	BOOL			GMTryAddBuff(Unit* pSrc, const tagBuffProto* pBuffProto, const tagTriggerProto* pTriggerProto, Skill* pSkill, tagItem* pItem, BOOL bReCalAtt=TRUE, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);
	BOOL			CancelBuff(DWORD dwBuffID);
	VOID			DispelBuff(BOOL bBenefit);
	VOID			DispelBuff(EBuffResistType eType);
	VOID			DispelBuff(DWORD dwBuffID);
	VOID			DispelBuff(INT nType, BOOL bLastOne);
	VOID			AddBuff(const tagBuffProto* pBuffProto, Unit* pSrc, DWORD dwSrcSkillID, const tagItem* pItem, INT nIndex, BOOL bRecalAtt);
	INT				RemoveBuff(DWORD dwBuffID, BOOL bRecalAtt, BOOL bSelf=FALSE);
	VOID			RemoveAllBuffBelongSkill(DWORD dwSkillID, BOOL bRecalAtt);
	VOID			RemoveAllBuff();
	VOID			RemoveAllBuff(BOOL bBenefit);
	VOID			UpdateBuff();
	VOID			OnInterruptBuffEvent(EBuffInterruptFlag eFlag, INT nMisc=GT_INVALID);
	INT				BuffCounteractAndWrap(Unit* pSrc, DWORD dwBuffID, INT nBuffLevel, INT nGroupLevel, DWORD dwGroupFlag, BOOL bBenefit, INT& nIndex);

	VOID			CalBuffInstantEffect(Unit* pSrc, EBuffEffectMode eMode, const tagBuffProto* pProto, const tagBuffMod* pMod, INT nWrapTimes=1, Unit* pTarget=NULL);
	VOID			CalBuffPersistEffect(Unit* pSrc, const tagBuffProto* pProto, const tagBuffMod* pMod, INT nWrapTimes=1, BOOL bSet=TRUE);
	VOID			WrapBuffPersistEffect(Unit* pSrc, const tagBuffProto* pProto, const tagBuffMod* pMod);
	VOID			CalBuffTargetList(Unit* pSrc, EBuffOPType eOPType, FLOAT fOPDist, FLOAT fOPRadius, EBuffFriendEnemy eFriendEnemy, DWORD dwTargetLimit, DWORD dwTargetStateLimit, TList<Unit*>& listTarget, Unit* pTarget=NULL);
	BOOL			IsBuffTargetValid(Unit* pTarget, EBuffFriendEnemy eFriendEnemy, DWORD dwTargetLimit, DWORD dwTargetStateLimit);
	Buff*			GetRelativeSkillBuff(DWORD dwSkillID, BOOL bBenefit = TRUE);

	//------------------------------------------------------------------------------------
	// ���������
	//------------------------------------------------------------------------------------
	VOID			AddEnmityCreature(Unit *pUnit);
	VOID			DelEnmityCreature(Unit *pUnit);
	VOID			ClearEnmityCreature();
	VOID			ChangeEnmityCreatureValue(DWORD dwRate);


	//------------------------------------------------------------------------------------
	// Ǳ��
	//------------------------------------------------------------------------------------
	VOID			Lurk(INT nLurkLevel);
	VOID			UnLurk();
	
	//------------------------------------------------------------------------------------
	// ͬ���ƶ���Ϣ���
	//------------------------------------------------------------------------------------
	BOOL			HasDectiveSkill()			{ return TRUE; }//??
	BOOL			IsInVisDist(Unit* pUnit);
	BOOL			IsInVisDistForOpenMacre(Unit* pUnit);
	BOOL			IsInVisList(DWORD dwUnitID);
	VOID			Add2VisList(DWORD dwUnitID);
	VOID			RemoveFromVisList(DWORD dwUnitID);

	//-----------------------------------------------------------------------------------
	// ���������
	//-----------------------------------------------------------------------------------
	BOOL			TestTrigger(Unit* pTarget, const tagTriggerProto* pProto, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);
	BOOL			TestEventTrigger(Unit* pTarget, const tagTriggerProto* pProto, const tagTriggerMod* pMod, DWORD dwEventMisc1, DWORD dwEventMisc2);
	BOOL			TestStateTrigger(Unit* pTarget, const tagTriggerProto* pProto, const tagTriggerMod* pMod);
	VOID			SetTriggerMode(BOOL bTrigger){ m_bTriggerOff = bTrigger; }

	//----------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------
	BOOL HitFly(Unit* pTarget);

	//----------------------------------------------------------------------------------
	// �����͹رպ�״̬���������ҵ����⴦��
	//----------------------------------------------------------------------------------
	BOOL OpenMacroToDealWithLuckState();
	BOOL CloseMacroToDealWithLuckState();

protected:
	//----------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------
	VOID			SetAttRecalFlag(INT nIndex);
	bool			GetAttRecalFlag(INT nIndex);
	VOID			ClearAttRecalFlag();
	BOOL			NeedRecalAtt()							{ return m_bNeedRecal; }	
	VOID			RecalAtt(BOOL bSendMsg=TRUE);
	INT				CalAttMod(INT nBase, INT nIndex);
	VOID			SendAttChange(INT nIndex);

	virtual VOID	OnAttChange(INT nIndex) = 0;
	virtual VOID	OnAttChange(bool bRecalFlag[ERA_End]) = 0;		// �ǵ�Ҫ�������浱ǰ���Ժ����

	//----------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------
	VOID			AddSkillMod(Skill* pSkill);
	VOID			AddSkillBeMod(Skill* pSkill);
	VOID			RemoveSkillMod(Skill* pSkill);
	virtual VOID	AddSkill(Skill* pSkill, BOOL bSendMsg=TRUE);
	virtual VOID	RemoveSkill(DWORD dwSkillID);
	virtual VOID	ChangeSkillLevel(Skill* pSkill, ESkillLevelChange eType, INT nChange=1);
	virtual VOID	ChangeSkillExp(Skill *pSkill, INT nValue);



public:
	// ��ʱ�����ã������װ��move_data��
	FLOAT						m_fXZSpeed;							// ��ǰXZ�����ٶ�
	FLOAT						m_fYSpeed;							// ��ǰY�����ٶ�
	FLOAT						m_fSwimXZSpeed;						// ��Ӿ��XZ�����ٶ�
	FLOAT						m_fJumpXZSpeed;						// ��Ծ��XZ�����ٶȣ�����Ծ��ʼʱ��ֵ����Ծ�����в��ı�
	FLOAT						m_fJumpYSpeed;						// ��Ծ��Y�����ٶȣ�����Ծ��ʼʱ��ֵ������Ծ�����в��ı�
	FLOAT						m_fDropXZSpeed;						// ����ʱ��XZ�����ٶ�
	FLOAT						m_fSlideSpeed;						// �����ٶ�
	FLOAT						m_fMountXZSpeed;					// ��ǰ���XZ�����ٶ�

protected:
	DWORD						m_dwID;								// ID
	INT							m_nLevel;							// �ȼ�
	INT							m_nCurLevelExp;						// ��ǰ��������
	DWORD						m_dwMapID;							// ��ͼID
	DWORD						m_dwInstanceID;						// ����ID
	Map*						m_pMap;								// ��ͼָ��

	MoveData					m_MoveData;							// �ƶ�״̬
	Vector3						m_Size;								// �ߴ�
	Vector3						m_SizeMount;						// ���ʱ�ĳߴ�

	//------------------------------------------------------------------------------------
	// ״̬
	//------------------------------------------------------------------------------------
	CountStateMgr<ES_End>		m_StateMgr;							// ����״̬��������ѣ�Σ��⼼������ȵȣ�

	//------------------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------------------
	INT							m_nAtt[ERA_End];					// ��ǰ����
	INT							m_nBaseAtt[ERA_End];				// �������ԣ������������ܣ�װ���Լ�״̬Ч���������ڣ�
	INT							m_nAttMod[ERA_End];					// ��ֵ�ӳɣ��������ܣ�װ���Լ�״̬Ч���ܵĶ�ֵ�ӳ�ֵ��
	INT							m_nAttModPct[ERA_End];				// �ٷֱȼӳɣ��������ܣ�װ���Լ�״̬Ч���ܵİٷֱȼӳ�ֵ��

	bool						m_bAttRecalFlag[ERA_End];			// ������Ҫ���¼���ı�־λ
	BOOL						m_bNeedRecal;						// �Ƿ������Ըı�

	//-------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------
	TMap<DWORD, Skill*>			m_mapSkill;					// �����б�
	TList<DWORD>				m_listSkillCoolDown;		// ��ȴ�����б�
	CombatHandler				m_CombatHandler;			// ��ǰ����ʩչ�ļ���
	INT							m_nProduceSkillNum;			// ��ǰ�����������ܵ�����

	TMap<DWORD, TList<DWORD>*>	m_mapBuffModifier;			// �������ܶ�buff��Ӱ���б�
	TMap<DWORD, TList<DWORD>*>	m_mapTriggerModifier;		// �������ܶ�trigger��Ӱ���б�

	BOOL						m_bTriggerOff;			// �Ƿ���Ҫ�жϴ�����

	//-------------------------------------------------------------------------------------
	// Buff
	//-------------------------------------------------------------------------------------
	Buff						m_Buff[MAX_BUFF_NUM];		// ��ǰ���ϵ�����Buff
	TMap<DWORD, Buff*>			m_mapBuff;					// ��ǰ���ϵ�buff�б�ע�⣺���ܶԸ�map������ѯ������
	TList<DWORD>				m_listDestroyBuffID;		// Ҫɾ����Buff�б�

	//-------------------------------------------------------------------------------------
	// ӵ�и���ҳ�޵Ĺ����б�
	//-------------------------------------------------------------------------------------
	TMap<DWORD, DWORD>			m_mapEnmityCreature;

	//-------------------------------------------------------------------------------------
	// ���ӵ�Ǳ��״̬��λ�б�
	//-------------------------------------------------------------------------------------
	TList<DWORD>				m_listLurkUnit;

	//-------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------
	INT							m_nHPMPRegainTickCountDown;				// ��Ѫ�����ĵ���ʱ
	INT							m_nVitalityRegainTickCountDown;			// �ػ����ĵ���ʱ
	INT							m_nEnduranceRegainTickCountDown;		// �س־����ĵ���ʱ
	INT							m_nInPrisonMoraleRegainTickCountDown;	// ��������ֵ������ʱ
	INT							m_nOutPrisonMoraleRegainTickCountDown;	// ����������ֵ������ʱ
	INT							m_nInjuryRegainTickCountDown;			// �ָ����˵���ʱ
};


//----------------------------------------------------------------------------------
// �õ�������
//----------------------------------------------------------------------------------
inline AABBox Unit::GetAABBox() const
{
	AABBox box;
	FLOAT fHalfX = m_Size.x / 2.0f;
	FLOAT fHalfZ = m_Size.z / 2.0f;
	box.min.x = m_MoveData.m_vPos.x - fHalfX;
	box.min.z = m_MoveData.m_vPos.z - fHalfZ;
	box.min.y = m_MoveData.m_vPos.y;
	box.max.x = m_MoveData.m_vPos.x + fHalfX;
	box.max.z = m_MoveData.m_vPos.z + fHalfZ;
	box.max.y = m_MoveData.m_vPos.y + m_Size.y;

	return box;
}

//----------------------------------------------------------------------------------
// �ж�Ŀ���Ƿ���������Χ
//----------------------------------------------------------------------------------
inline BOOL Unit::IsInDistance(Unit& target, FLOAT fDis) 
{
	const Vector3& vSrc = GetCurPos();
	const Vector3& vDest = target.GetCurPos();

	FLOAT fDistSq = Vec3DistSq(vSrc, vDest);

	return fDis * fDis >= fDistSq;
}

//----------------------------------------------------------------------------------
// �ж���Ŀ���ս�������Ƿ����㣬fDisΪ���ܻ���Ʒ��ʹ�þ���
//----------------------------------------------------------------------------------
inline BOOL Unit::IsInCombatDistance(Unit& target, FLOAT fDis)
{
	const Vector3& vSrc = GetCurPos();
	const Vector3& vDest = target.GetCurPos();

	FLOAT fDistSq = Vec3DistSq(vSrc, vDest);

	// ս������Ҫ����˫�����Եİ뾶���ټ���һ���������Ա��������ӳ�
	FLOAT fRealDist = fDis + (m_Size.z + target.m_Size.z) * 0.5f + 100.0f;

	return fRealDist * fRealDist >= fDistSq;
}

//----------------------------------------------------------------------------------
// �ж��Լ��Ƿ���Ŀ����ǰ������������
//----------------------------------------------------------------------------------
inline BOOL Unit::IsInFrontOfTarget(Unit& target)
{
	Vector3& vSelfPos = GetCurPos();
	Vector3& vTargetPos = target.GetCurPos();

	// ���Ŀ���泯����ʸ��A
	Vector3 vTargetFace = target.GetFaceTo();

	// ���Ŀ�굽�Լ���ʸ��B
	Vector3 vDir = vSelfPos - vTargetPos;

	// ���ǵĵ������|A|*|B|*cos��,cos��Ϊ�Ǹ���ʾ�н�С�ڵ���90��
	// ����ʾ������Ŀ������
	if( vTargetFace.x * vDir.x + vTargetFace.z * vDir.z >= 0 )
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------
// �ж������Ŀ��֮������߼���Ƿ�ͨ��
//----------------------------------------------------------------------------------
inline BOOL Unit::IsRayCollide(Unit& target)
{
	Map* pMap = GetMap();
	if( !P_VALID(pMap) || !P_VALID(pMap->GetNavMap()) )
		return FALSE;

	Vector3 vSrc = GetCurPosTop();
	Vector3 vDest = target.GetCurPosTop();

	if( pMap->IsRayCollide(vSrc, vDest) )
	{
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------------
// ��ȡ��ǰ����
//-----------------------------------------------------------------------------------
inline INT Unit::GetAttValue(INT nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	return m_nAtt[nIndex];
}

//-----------------------------------------------------------------------------------
// ��ȡ��������
//-----------------------------------------------------------------------------------
inline INT Unit::GetBaseAttValue(INT nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	return m_nBaseAtt[nIndex];
}

//-----------------------------------------------------------------------------------
// ��ȡ����ƽֱ�ӳ�
//-----------------------------------------------------------------------------------
inline INT Unit::GetAttModValue(INT nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	return m_nAttMod[nIndex];
}

//-----------------------------------------------------------------------------------
// ��ȡ���԰ٷֱȼӳ�
//-----------------------------------------------------------------------------------
inline INT Unit::GetAttModValuePct(INT nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	return m_nAttModPct[nIndex];
}

//------------------------------------------------------------------------------------
// ���õ�ǰ����
//------------------------------------------------------------------------------------
inline VOID Unit::SetAttValue(INT nIndex, INT nValue, BOOL bSendMsg)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	m_nAtt[nIndex] = nValue;

	// ȡ������
	if( m_nAtt[nIndex] < g_attRes.GetAttMin(nIndex) ) m_nAtt[nIndex] = g_attRes.GetAttMin(nIndex);
	if( m_nAtt[nIndex] > g_attRes.GetAttMax(nIndex) ) m_nAtt[nIndex] = g_attRes.GetAttMax(nIndex);

	OnAttChange(nIndex);

	if( bSendMsg ) SendAttChange(nIndex);
}

//-----------------------------------------------------------------------------------
// ���û�������
//-----------------------------------------------------------------------------------
inline VOID Unit::SetBaseAttValue(INT nIndex, INT nValue)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	m_nBaseAtt[nIndex] = nValue;
	SetAttRecalFlag(nIndex);
}

//-----------------------------------------------------------------------------------
// ��������ƽֱ�ӳ�
//-----------------------------------------------------------------------------------
inline VOID Unit::SetAttModValue(INT nIndex, INT nValue)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	m_nAttMod[nIndex] = nValue;
	SetAttRecalFlag(nIndex);
}

//-----------------------------------------------------------------------------------
// �������԰ٷֱȼӳ�
//-----------------------------------------------------------------------------------
inline VOID Unit::SetAttModValuePct(INT nIndex, INT nValuePct)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	m_nAttModPct[nIndex] = nValuePct;
	SetAttRecalFlag(nIndex);
}

//------------------------------------------------------------------------------------
// �޸ĵ�ǰ����
//------------------------------------------------------------------------------------
inline VOID Unit::ModAttValue(INT nIndex, INT nValueMod, BOOL bSendMsg)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );

	INT nOriginValue = m_nAtt[nIndex];		// �õ���ʼֵ

	m_nAtt[nIndex] += nValueMod;			// ���õ�ǰֵ

	// ȡ������
	if( m_nAtt[nIndex] < g_attRes.GetAttMin(nIndex) ) m_nAtt[nIndex] = g_attRes.GetAttMin(nIndex);
	if( m_nAtt[nIndex] > g_attRes.GetAttMax(nIndex) ) m_nAtt[nIndex] = g_attRes.GetAttMax(nIndex);

	OnAttChange(nIndex);

	// �����ֵȷʵ�ı䲢����Ҫ������Ϣ������Ϣ
	if( nOriginValue != m_nAtt[nIndex] && bSendMsg ) SendAttChange(nIndex);
}

//-----------------------------------------------------------------------------------
// �޸Ļ�������
//-----------------------------------------------------------------------------------
inline VOID Unit::ModBaseAttValue(INT nIndex, INT nValueMod)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	m_nBaseAtt[nIndex] += nValueMod;
	SetAttRecalFlag(nIndex);
}

//-----------------------------------------------------------------------------------
// �޸�����ƽֱ�ӳ�
//-----------------------------------------------------------------------------------
inline VOID Unit::ModAttModValue(INT nIndex, INT nValueMod)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	m_nAttMod[nIndex] += nValueMod;
	SetAttRecalFlag(nIndex);
}

//-----------------------------------------------------------------------------------
// �޸����԰ٷֱȼӳ�
//-----------------------------------------------------------------------------------
inline VOID Unit::ModAttModValuePct(INT nIndex, INT nValuePctMod)
{
	ASSERT( nIndex >= 0 && nIndex < ERA_End );
	m_nAttModPct[nIndex] += nValuePctMod;
	SetAttRecalFlag(nIndex);
}

//----------------------------------------------------------------------------------
// �������¼����ʶλ����
//----------------------------------------------------------------------------------
inline VOID Unit::SetAttRecalFlag(INT nIndex)
{
	ASSERT( nIndex > ERA_Null && nIndex < ERA_End );
	m_bAttRecalFlag[nIndex] = true;
	if( !m_bNeedRecal ) m_bNeedRecal = TRUE;
}

//----------------------------------------------------------------------------------
// �õ�ĳ�������Ƿ���Ҫ����
//----------------------------------------------------------------------------------
inline bool Unit::GetAttRecalFlag(INT nIndex)
{
	ASSERT( nIndex > ERA_Null && nIndex < ERA_End );
	return m_bAttRecalFlag[nIndex];
}

//----------------------------------------------------------------------------------
// ������������־λ
//----------------------------------------------------------------------------------
inline VOID Unit::ClearAttRecalFlag()
{
	ZeroMemory(m_bAttRecalFlag, sizeof(m_bAttRecalFlag));
	m_bNeedRecal = FALSE;
}

//----------------------------------------------------------------------------------
// ����������Զ������ӳ��
//----------------------------------------------------------------------------------
inline ERemoteRoleAtt Unit::ERA2ERRA(ERoleAttribute eRA)
{
	switch(eRA)
	{
	case ERA_MaxHP:
		return ERRA_MaxHP;
		break;

	case ERA_HP:
		return ERRA_HP;
		break;

	case ERA_MaxMP:
		return ERRA_MaxMP;
		break;

	case ERA_MP:
		return ERRA_MP;
		break;

	case ERA_Rage:
		return ERRA_Rage;
		break;

	case ERA_Speed_XZ:
		return ERRA_Speed_XZ;
		break;

	case ERA_Speed_Mount:
		return ERRA_Speed_Mount;
		break;

	case ERA_Speed_Y:
		return ERRA_Speed_Y;
		break;

	case ERA_Shape:
		return ERRA_Shape;
		break;

	case ERA_Hostility:
		return ERRA_Hostility;
		break;

	default:
		return ERRA_Null;
		break;
	}

	return ERRA_Null;
}

//----------------------------------------------------------------------------------
// ���ݼ����˺�������ȷ����Ӧ�������˺���������
//----------------------------------------------------------------------------------
inline ERoleAttribute Unit::SkillDmgType2ERA(ESkillDmgType eSKillDmgType)
{
	switch(eSKillDmgType)
	{
	case ESDGT_Ordinary:
		return ERA_Derate_Ordinary;
		break;

	case ESDGT_Bleeding:
		return ERA_Derate_Bleeding;
		break;

	case ESDGT_Brunt:
		return ERA_Derate_Brunt;
		break;

	case ESDGT_Bang:
		return ERA_Derate_Bang;
		break;

	case ESDGT_Poison:
		return ERA_Derate_Poison;
		break;

	case ESDGT_Thinker:
		return ERA_Derate_Thinker;
		break;

	case ESDGT_Injury:
		return ERA_Derate_Injury;
		break;

	case ESDGT_Stunt:
		return ERA_Derate_Stunt;
		break;

	default:
		return ERA_Null;
		break;
	}
}

//----------------------------------------------------------------------------------
// ����Buff�Ŀ������͵õ���Ӧ�����￹������
//----------------------------------------------------------------------------------
inline ERoleAttribute Unit::BuffResistType2ERA(EBuffResistType eBuffResistType)
{
	switch(eBuffResistType)
	{
	case EBRT_Bleeding:
		return ERA_Resist_Bleeding;
		break;

	case EBRT_Weak:
		return ERA_Resist_Weak;
		break;

	case EBRT_Choas:
		return ERA_Resist_Choas;
		break;

	case EBRT_Special:
		return ERA_Resist_Special;
		break;

	case EBRT_Regain:
		return ERA_Regain_Addtion;
		break;

	default:
		return ERA_Null;
		break;
	}
}

//----------------------------------------------------------------------------------------
// ���ĳ�������ǲ��Ƕ���ͬ��������
//----------------------------------------------------------------------------------------
inline BOOL Unit::IsTeamRemoteAtt(ERemoteRoleAtt eType)
{
	if(eType == ERRA_MaxHP || eType == ERRA_HP || eType == ERRA_MaxMP || eType == ERRA_MP)
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------------------------------------
// �ı�Ѫ��
//-----------------------------------------------------------------------------------------
inline VOID Unit::ChangeHP(INT nValue, Unit* pSrc/* =NULL */, Skill* pSkill/* =NULL */, const tagBuffProto* pBuff/* =NULL */, 
						   bool bCrit/* =false */, DWORD dwSerial/* =GT_INVALID */, DWORD dwMisc/* =GT_INVALID */)
{
	if( 0 == nValue ) return;

	ModAttValue(ERA_HP, nValue);

	// �����ս��ϵͳ��ɵ�Ѫ���ı䣬����ս��������Ϣ
	if( P_VALID(pSkill) || P_VALID(pBuff) )
	{
		tagNS_RoleHPChange send;
		send.dwRoleID = GetID();
		send.bBlocked = false;
		send.bMiss = false;
		send.bCrit = bCrit;
		send.nHPChange = nValue;
		send.dwMisc2 = dwMisc;
		send.dwSerial = dwSerial;

		if( P_VALID(pSrc) )
		{
			send.dwSrcRoleID = pSrc->GetID();
		}
		else
		{
			send.dwSrcRoleID = GT_INVALID;
		}

		if( P_VALID(pSkill) )
		{
			send.eCause = ERHPCC_SkillDamage;
			send.dwMisc = pSkill->GetTypeID();
			send.dwMisc2 = dwMisc;
			send.dwSerial = dwSerial;
		}
		else
		{
			send.eCause = ERHPCC_BuffDamage;
			send.dwMisc = pBuff->dwID;
		}

		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}

	// ����Ѫ�¼�
	if( nValue < 0 )
	{
		OnInterruptBuffEvent(EBIF_HPLower);		// ���Buff�¼�

		if( GetAttValue(ERA_HP) <= 0 )
		{
			OnDead(pSrc, pSkill, pBuff, dwSerial, dwMisc);	// ����
		}
	}
}

//-----------------------------------------------------------------------------------------
// �ı�����
//-----------------------------------------------------------------------------------------
inline VOID Unit::ChangeMP(INT nValue)
{
	if( 0 == nValue ) return;

	ModAttValue(ERA_MP, nValue);

	if( nValue < 0 )
	{
		OnInterruptBuffEvent(EBIF_MPLower);		// ���Buff�¼�
	}
}

//-----------------------------------------------------------------------------------------
// �ı�ŭ��
//-----------------------------------------------------------------------------------------
inline VOID Unit::ChangeRage(INT nValue)
{
	if( 0 == nValue ) return;

	ModAttValue(ERA_Rage, nValue);

	if( nValue < 0 )
	{
		OnInterruptBuffEvent(EBIF_RageLower);		// ���Buff�¼�
	}
}

//-----------------------------------------------------------------------------------------
// �ı����
//-----------------------------------------------------------------------------------------
inline VOID Unit::ChangeVitality(INT nValue)
{
	if( 0 == nValue ) return;

	ModAttValue(ERA_Vitality, nValue);

	if( nValue < 0 )
	{
		OnInterruptBuffEvent(EBIF_VitalityLower);		// ���Buff�¼�
	}
}

//-----------------------------------------------------------------------------------------
// �ı�־���
//-----------------------------------------------------------------------------------------
inline VOID Unit::ChangeEndurance(INT nValue)
{
	if( 0 == nValue ) return;

	ModAttValue(ERA_Endurance, nValue);

	if( nValue < 0 )
	{
		OnInterruptBuffEvent(EBIF_EnduranceLower);		// ���Buff�¼�
	}
}

//-----------------------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------------------
inline VOID Unit::SetState(EState eState, BOOL bSendMsg)
{
	BOOL bIn = IsInState(eState);

	m_StateMgr.SetState(eState);
	OnSetState(eState);
	
	DWORD nFlag = m_StateMgr.GetStateFlags();
	if( !bIn && bSendMsg )
	{
		tagNS_SetState send;
		send.dwRoleID = GetID();
		send.eState = eState;
		if( GetMap() )
		{
			GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
		}
	}
}

//-----------------------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------------------
inline VOID Unit::UnsetState(EState eState, BOOL bSendMsg)
{
	if( !IsInState(eState) ) return;

	m_StateMgr.UnsetState(eState);

	if( !IsInState(eState) )
	{
		OnUnSetState(eState);
		if( bSendMsg )
		{
			tagNS_UnSetState send;
			send.dwRoleID = GetID();
			send.eState = eState;
			if( GetMap() )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
// ��״̬������ʱ
//-----------------------------------------------------------------------------------------
inline VOID Unit::OnSetState(EState eState)
{
	switch(eState)
	{
		// ѣ��
	case ES_Dizzy:
		{
			GetMoveData().StopMoveForce();	// ǿ��ֹͣ�ƶ�
			GetCombatHandler().End();		// ֹͣ��ǰ����
		}
		break;

		// ����
	case ES_Tie:
		{
			GetMoveData().StopMoveForce();	// ǿ��ֹͣ�ƶ�
		}
		break;

		// ��˯
	case ES_Spor:
		{
			GetMoveData().StopMoveForce();	// ǿ��ֹͣ�ƶ�
			GetCombatHandler().End();		// ֹͣ��ǰ����
		}
		break;

		// �޵� 
	case ES_Invincible:
		{
			RemoveAllBuff(FALSE);			// �����������debuff
		}
		break;

		// ����
	case ES_Lurk:
		{
			
		}
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------
// ��״̬������ʱ
//-----------------------------------------------------------------------------------------
inline VOID Unit::OnUnSetState(EState eState)
{
	switch(eState)
	{
	case ES_Lurk:
		{

		}
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------
inline VOID Unit::UpdateSkill()
{
	if( m_listSkillCoolDown.Empty() ) return;

	DWORD dwSkillID = GT_INVALID;
	TList<DWORD>::TListIterator it = m_listSkillCoolDown.Begin();
	while( m_listSkillCoolDown.PeekNext(it, dwSkillID) )
	{
		Skill* pSkill = m_mapSkill.Peek(dwSkillID);
		if( P_VALID(pSkill) && pSkill->CountDownCoolDown() )
		{
			m_listSkillCoolDown.Erase(dwSkillID);
		}
	}
}

//----------------------------------------------------------------------------------
// ��ռ���CD
//----------------------------------------------------------------------------------
inline VOID Unit::ClearAllSkillCoodDown(ETalentType eType, DWORD dwExceptSkillID)
{
	if( m_listSkillCoolDown.Empty() ) return;

	if( ETT_Null == eType )
	{
		// ���м�����ȴ
		DWORD dwSkillID = GT_INVALID;
		TList<DWORD>::TListIterator it = m_listSkillCoolDown.Begin();
		while( m_listSkillCoolDown.PeekNext(it, dwSkillID) )
		{
			if( dwSkillID == dwExceptSkillID ) continue;

			ClearSkillCoolDown(dwSkillID);
		}
	}
	else
	{
		if( eType >= ETT_End ) return;

		// ĳ�����ʼ�����ȴ
		DWORD dwSkillID = GT_INVALID;
		TList<DWORD>::TListIterator it = m_listSkillCoolDown.Begin();
		while( m_listSkillCoolDown.PeekNext(it, dwSkillID) )
		{
			if( dwSkillID == dwExceptSkillID ) continue;

			Skill* pSkill = GetSkill(dwSkillID);
			if( pSkill->GetTalentType() != eType ) continue;

			ClearSkillCoolDown(dwSkillID);
		}
	}
}

//----------------------------------------------------------------------------------
// ������Buffд���ⲿ��Ϣ
//----------------------------------------------------------------------------------
inline VOID Unit::GetAllBuffMsgInfo(tagBuffMsgInfo* const pBuffInfo, INT nMaxNum)
{
	if( nMaxNum <= 0 ) return;

	INT nIndex = 0;	// д���Index

	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		pBuffInfo[nIndex].dwBuffTypeID = m_Buff[n].GetTypeID();
		pBuffInfo[nIndex].nWarpTimes = m_Buff[n].GetWrapTimes();
		pBuffInfo[nIndex].nMaxPersistTime = m_Buff[n].GetPersistTime();
		pBuffInfo[nIndex].nPersistTimeLeft = m_Buff[n].GetPersistTimeLeft();

		if( ++nIndex > nMaxNum ) break;
	}
}

//----------------------------------------------------------------------------------
// Ǳ��
//----------------------------------------------------------------------------------
inline VOID Unit::Lurk(INT nLurkLevel)
{
	Map *pMap = GetMap();
	if( !P_VALID(pMap) )
	{
		return;
	}
	
	// ���õ�λ״̬
	SetState(ES_Lurk);

	// ͬ��
	pMap->Lurk(this);
}

//----------------------------------------------------------------------------------
// ����Ǳ��״̬
//----------------------------------------------------------------------------------
inline VOID Unit::UnLurk()
{
	Map *pMap = GetMap();
	if( !P_VALID(pMap) )
	{
		return;
	}

	// ���õ�λ״̬
	UnsetState(ES_Lurk);

	// ͬ��
	if( !IsInState(ES_Lurk) )
	{
		pMap->UnLurk(this);
	}
}

//----------------------------------------------------------------------------------
// �Ƿ��ڿ���Ǳ�е�λ�б�
//----------------------------------------------------------------------------------
inline BOOL Unit::IsInVisList(DWORD dwUnitID)
{
	return m_listLurkUnit.IsExist(dwUnitID);
}

//----------------------------------------------------------------------------------
// �Ƿ��ڿ��ӷ�Χ��
//----------------------------------------------------------------------------------
inline BOOL Unit::IsInVisDist(Unit* pUnit)
{
	// ������
	FLOAT fDistSQ = Vec3DistSq(GetCurPos(), pUnit->GetCurPos());
	FLOAT fLurkDist = (FLOAT)GetAttValue(ERA_Inspiration) - (FLOAT)pUnit->GetAttValue(ERA_Lurk);
	if(fDistSQ <= fLurkDist )	return TRUE;

	return FALSE;
}

//----------------------------------------------------------------------------------
// ������Ǳ�еĿ��Ӿ����ж�
//----------------------------------------------------------------------------------
inline BOOL Unit::IsInVisDistForOpenMacre(Unit* pUnit)
{
	// 1�����ӵľ���
	FLOAT fDistAbs = FLOAT(1 * TILE_SCALE);	// ���Ծ���

	// ������
	FLOAT fDistSQ = Vec3DistSq(GetCurPos(), pUnit->GetCurPos());
	if ( fDistSQ < fDistAbs)
		return TRUE;
	else
		return FALSE;
	
}
//----------------------------------------------------------------------------------
// �������Ǳ�е�λ�б�
//----------------------------------------------------------------------------------
inline VOID Unit::Add2VisList(DWORD dwUnitID)
{
	ASSERT(!m_listLurkUnit.IsExist(dwUnitID));

	m_listLurkUnit.PushBack(dwUnitID);
}

//----------------------------------------------------------------------------------
// �ӿ���Ǳ�е�λ�б����޳�
//----------------------------------------------------------------------------------
inline VOID Unit::RemoveFromVisList(DWORD dwUnitID)
{
	ASSERT(m_listLurkUnit.IsExist(dwUnitID));
	
	m_listLurkUnit.Erase(dwUnitID);
}
