//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role.h
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: �������ݽṹ
//-----------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/move_define.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/pk_define.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/ScriptMsgInfo.h"

#include "../WorldDefine/msg_compose.h"
#include "../WorldDefine/msg_pk.h"
#include "../WorldDefine/msg_talent.h"
#include "../WorldDefine/action.h"
#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/msg_gm_cmd.h"

#include "../WorldDefine/msg_guild.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "../WorldDefine/penalty_define.h"

#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/msg_skill.h"

#include "unit.h"
#include "item_mgr.h"
#include "currency.h"
#include "db_session.h"
#include "player_session.h"
#include "exchange.h"
#include "pet_exchange.h"
#include "suit.h"
#include "quest.h"
#include "stall.h"
#include "group_mgr.h"
#include "team.h"
#include "vcard.h"
#include "clandata.h"
#include "state_mgr.h"
#include "mutex.h"
#include "longhun.h"


class PlayerSession;
class Map;
class Quest;
class Skill;
class Creature;
class Stall;
class Team;
class RoleScript;
class TitleMgr;
class PetPocket;

struct tagRoleDataLoad;
struct tagQuestDoneSave;
struct tagRoleTalent;
struct tagNDBC_SaveRole;
struct tagIMEffect;

//--------------------------------------------------------------------------------
// �������ܺ�װ���Ĵ���������
//--------------------------------------------------------------------------------
enum EPassiveSkillAndEquipTrigger
{
	EPSAET_Null				=	-1,
	EPSAET_BeAttacked		=	0,	// ������
	EPSAET_Hit				=	1,	// ����
	EPSAET_Hited			=	2,	// ������
	EPSAET_Dodged			=	3,	// ������
	EPSAET_Dodge			=	4,	// ����
	EPSAET_Blocked			=	5,	// ����
	EPSAET_Block			=	6,	// ��
	EPSAET_Crit				=	7,	// ����
	EPSAET_Crited			=	8,	// ������
	EPSAET_Die				=	9,	// ����
	EPSAET_Random			=	10,	// ���
	EPSAET_Attack			=	11,	// ����
	EPSAET_Kill				=	12,	// ��ɱ
	EPSAET_End				=	13
};

typedef VOID (Role::*pFun_RegTriggerEquipSet)(DWORD, DWORD, INT16);
//------------------------------------------------------------------------------
// ��ɫ��
//------------------------------------------------------------------------------
class Role : public Unit, public ScriptData<ESD_Role>
{
public:
	friend class PlayerSession;
	friend class GMCommandMgr;

public:
	typedef TMap<UINT16, tagQuestDoneSave*>			QuestDoneMap;
	typedef TMap<UINT16, Quest*>					QuestMap;

	typedef State<DWORD, ERoleState>				RoleState;
	typedef State<DWORD, ERoleStateEx>				RoleStateEx;

protected:
	//---------------------------------------------------------------------------
	// Constructor&Destructor
	//---------------------------------------------------------------------------
	Role(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession);
	virtual ~Role();
public:
	static Role* Create(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession);
	static VOID	Delete(Role* &pRole);

	//---------------------------------------------------------------------------
	// ��ʼ��
	//---------------------------------------------------------------------------
	VOID Init(const tagRoleDataLoad* pData);

	//---------------------------------------------------------------------------
	// ����
	//---------------------------------------------------------------------------
	VOID Online(BOOL bFirst);

	//----------------------------------------------------------------------------
	// ��һ������
	//----------------------------------------------------------------------------
	VOID VirginOnline();

	//----------------------------------------------------------------------------
	// ����Update����
	//----------------------------------------------------------------------------
	virtual VOID Update();

	//-----------------------------------------------------------------------------
	// �������ݿ�
	//-----------------------------------------------------------------------------
	VOID SaveToDB();

	//-----------------------------------------------------------------------------
	// ���뵽��Ϸ������
	//-----------------------------------------------------------------------------
	BOOL AddToWorld(BOOL bFirst);

	//-----------------------------------------------------------------------------
	// ���͵���ĳ����ͼ��
	//-----------------------------------------------------------------------------
	BOOL GotoNewMap(DWORD dwDestMapID, FLOAT fX, FLOAT fY, FLOAT fZ, DWORD dwMisc=0, BOOL bSameInstance=TRUE);

	//------------------------------------------------------------------------------
	// �س�
	//------------------------------------------------------------------------------
	VOID ReturnCity();

	//------------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------------
	VOID ReName(LPCTSTR szNewName);

	//------------------------------------------------------------------------------
	// ��Ʒ�Ƿ���Ҫ��ȴ
	//------------------------------------------------------------------------------
	BOOL ObjectCoolOff()
	{
		return m_bObjectCoolOff;
	}

	VOID SetObjectCoolMode(BOOL bMode)
	{
		m_bObjectCoolOff = bMode;
	}

	//------------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------------
	BOOL SpeakOff() { return m_bSpeakOff; }
	VOID SetSpeakOff(BOOL bFlag) { m_bSpeakOff = bFlag; }

	//-----------------------------------------------------------------------
	// ����Ƿ��ڶ�Ӧְ��NPC����
	//-----------------------------------------------------------------------
	DWORD CheckFuncNPC(DWORD dwNPCID, EFunctionNPCType eNPCType, OUT Creature **ppNPC = NULL, OUT Map **ppMap = NULL);

	//------------------------------------------------------------------------------
	// ��ǰ״̬ -- ��״̬���ת������Ҫ�ֶ���ɡ���SetState()�����Զ��������״̬λ��
	//------------------------------------------------------------------------------
	DWORD GetRoleState() const { return m_RoleState.GetState(); }

	BOOL IsInRoleState(ERoleState eState) const
	{
		return m_RoleState.IsInState(eState);
	}

	BOOL IsInRoleStateAll(DWORD dwState) const
	{
		return m_RoleState.IsInStateAll(dwState);
	}

	BOOL IsInRoleStateAny(DWORD dwState) const
	{
		return m_RoleState.IsInStateAny(dwState);
	}

	VOID SetRoleState(ERoleState eState, BOOL bSendMsg=TRUE)
	{
		m_RoleState.SetState(eState);

		if( bSendMsg )
		{
			tagNS_SetRoleState send;
			send.dwRoleID = GetID();
			send.eState = eState;
			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}

	VOID UnsetRoleState(ERoleState eState, BOOL bSendMsg=TRUE)
	{
		if( FALSE == IsInRoleState(eState) ) return;

		m_RoleState.UnsetState(eState);

		if( bSendMsg )
		{
			tagNS_UnSetRoleState send;
			send.dwRoleID = GetID();
			send.eState = eState;
			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}

	//------------------------------------------------------------------------------
	// ��չ״̬ -- ��״ֻ̬����ұ���֪���Ϳ���
	//------------------------------------------------------------------------------
	const RoleStateEx& GetRoleStateEx() const
	{
		return m_RoleStateEx;
	}
	
	VOID SetRoleStateEx(ERoleStateEx eState, BOOL bSendMsg=TRUE)
	{
		if( TRUE == m_RoleStateEx.IsInState(eState) ) return;

		m_RoleStateEx.SetState(eState);

		if( bSendMsg )
		{
			tagNS_SetRoleStateEx send;
			send.eState = eState;
			SendMessage(&send, send.dwSize);
		}
	}

	VOID UnsetRoleStateEx(ERoleStateEx eState, BOOL bSendMsg=TRUE)
	{
		if( FALSE == m_RoleStateEx.IsInState(eState) ) return;

		m_RoleStateEx.UnsetState(eState);

		if( bSendMsg )
		{
			tagNS_UnSetRoleStateEx send;
			send.eState = eState;
			SendMessage(&send, send.dwSize);
		}
	}

	//------------------------------------------------------------------------------
	// �Ƿ��ڲ����ƶ���״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantMove()
	{
		return Unit::IsInStateCantMove() || IsInRoleState(ERS_Stall) ;
	}

	//------------------------------------------------------------------------------
	// �Ƿ��ڲ���ʹ�ü��ܵ�״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantCastSkill()
	{
		return Unit::IsInStateCantCastSkill() || IsInRoleState(ERS_Stall) || IsInRoleState(ERS_Wedding);
	}

	//------------------------------------------------------------------------------
	// �Ƿ��ڲ��ɱ�����״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantBeSkill()
	{
		return Unit::IsInStateCantBeSkill() || IsInRoleState(ERS_Stall) || IsInRoleState(ERS_Wedding);
	}
	//------------------------------------------------------------------------------
	//�Ƿ��ڲ����Ա�������ӵ�״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantBeInviteJoinTeam()
	{ 
		return Unit::IsInStateCantBeInviteJoinTeam() || IsInRoleState(ERS_Wedding);
	} 
	//------------------------------------------------------------------------------
	// �Ƿ��ڲ��ɼ���״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateInvisible()
	{
		return Unit::IsInStateInvisible();
	}

	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// ������ڵ�ͼ����
	//------------------------------------------------------------------------------
	VOID CheckMapArea();
	
	//------------------------------------------------------------------------------
	// ��Ǯ����Ʒ
	//------------------------------------------------------------------------------
	CurrencyMgr&	GetCurMgr()		{ return m_CurMgr; }
	ItemMgr&		GetItemMgr()	{ return m_ItemMgr; }

	//-------------------------------------------------------------------------------
	// �������ݻ�ȡ
	//-------------------------------------------------------------------------------
	EClassType				GetClass()		const { return m_eClass; }
	EClassTypeEx			GetClassEx()	const { return m_eClassEx; }
	INT						GetCredit()		const { return m_nCredit; }
	INT						GetIdentity()	const { return m_nIdentity; }
	INT						GetVIPPoint()	const { return m_nVIPPoint; }
	PlayerSession*			GetSession()	const { return m_pSession; }
	const tagAvatarAtt*		GetAvatar()		const { return &m_Avatar; }
	const tagDisplaySet&	GetDisplaySet()	const { return m_DisplaySet; }
	ERolePKState			GetPKState()	const { return m_ePKState; }
	DWORD					GetRebornMapID()const { return m_dwRebornMapID; }
	DWORD					GetNameID()		const { return GetID(); }			//?? �ýӿ����һ������
	DWORD					GetTeamID()		const { return m_dwTeamID; }
	DWORD					GetGroupID()	const { return m_dwGroupID; }
	DWORD					GetTeamInvite()	const { return m_dwTeamInvite; }
	DWORD					GetEarnRate()	const { return GetSession()->GetFatigueGuarder().GetEarnRate();}
	virtual BYTE			GetSex()		const { return m_Avatar.bySex; }
	INT						GetOnlineTime() const { return m_nOnlineTime; }
	tagDWORDTime			GetLogoutTime() const { return m_LogoutTime; }

	DWORD					GetStrength()	const { return m_Strength.dwValue; }
	DWORD					GetErrantry()	const { return m_Strength.m_dwErrantry; }
	DWORD					GetHeroism()	const { return m_Strength.m_dwHeroism; }
	DWORD					GetValor()		const { return m_Strength.m_dwValor; }
	DWORD					GetWit()		const { return m_Strength.m_dwWit; }
	DWORD					GetStrengthNum() const {return m_nStrengthNum; }
	

	const tagRemoteOpenSet& GetRemoteOpenSet() const { return m_sRemoteOpenSet; }

	LPCTSTR					GetVNBName() const { return GetSession()->GetVNBName(); }

	const tagAvatarEquip&	GetAvatarEquip() const
	{
		return GetDisplaySet().bFashionDisplay ? m_AvatarEquipFashion : m_AvatarEquipEquip; 
	}

	const tagChestItem&		GetChestItem()	const	{return m_TreasureState.ChestItem;}
	INT64					GetChestSerial() const	{return m_TreasureState.nChestSerial;}
	INT64					GetKeySerial() const	{return m_TreasureState.nKeySerial;}
	DWORD					GetChestTypeID() const	{return m_TreasureState.dwChestTypeID;}
	DWORD					GetKeyTypeID() const	{return m_TreasureState.dwKeyTypeID;}

	//-------------------------------------------------------------------------------
	// ������������
	//-------------------------------------------------------------------------------

	VOID SetClass(EClassType eVocation)		
	{
		m_eClass = eVocation;
	}
	
	VOID SetClassEx(EClassTypeEx eHVocation)	
	{
		m_eClassEx = eHVocation;
	}

	VOID SetStrength(DWORD dwStrength, DWORD dwHeroism, DWORD dwWit, DWORD dwErrantry, DWORD dwValor)
	{
		m_Strength.dwValue = dwStrength;			// ʵ��ֵ
		m_Strength.m_dwHeroism = dwHeroism;			// Ӣ��ֵ
		m_Strength.m_dwWit = dwWit;					// ı��ֵ
		m_Strength.m_dwErrantry = dwErrantry;		// ����ֵ
		m_Strength.m_dwValor = dwValor;				// ��սֵ
	}

	VOID SetChestTypeID(DWORD dwTypeID)
	{
		m_TreasureState.dwChestTypeID = dwTypeID;
	}

	VOID SetKeyTypeID(DWORD dwTypeID)
	{
		m_TreasureState.dwKeyTypeID = dwTypeID;
	}

	VOID SetChestSerial(INT64 nSerial)
	{
		m_TreasureState.nChestSerial = nSerial;
	}

	VOID SetKeySerial(INT64 nSerial)
	{
		m_TreasureState.nKeySerial = nSerial;
	}

	VOID SetChestItem(tagChestItem item)
	{
		m_TreasureState.ChestItem = item;
	}

	VOID SetFashionMode(bool bDisplay)
	{
		m_DisplaySet.bFashionDisplay = bDisplay;
	}

	VOID SetDisplaySet(bool bHead, bool bFace, bool bBack)
	{
		m_DisplaySet.Set(bHead, bFace, bBack);
	}

	VOID SetEquipDisplay(BOOL bFashion, INT nDisplayPos, DWORD dwTypeID, BYTE byFlareVal, INT8 n8ColorID)
	{
		if(bFashion)	// ʱװ
		{
			m_AvatarEquipFashion.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
		}
		else	// ���������
		{
			m_AvatarEquipEquip.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);

			// ������������������⴦��
			if(MIsWeaponByDisplayPos(nDisplayPos))
			{
				m_AvatarEquipFashion.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
			}
		}
	}

	// ����������Ч��Ҫ��������
	VOID SetEquipEffect(INT nDisplayPos, BYTE byEquipEffect)
	{
		m_AvatarEquipEquip.Set(nDisplayPos, byEquipEffect);
		m_AvatarEquipFashion.Set(nDisplayPos, byEquipEffect);
	}

	// ȡ�ÿɼ�������ID
	LongHun& GetLongHun()				{ return m_LongHun; }

	// ���ü�¼ʽ���ͷ�������
	VOID SetItemTransportPoint(tagNetCmd* pCmd);

	// ʹ�ü�¼ʽ���ͷ�
	VOID UseNoteTakingItemTransport();
	//-------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------
	BOOL	IsInGuild()	const			{ return m_dwGuildID != GT_INVALID; }
	DWORD	GetGuildID() const			{ return m_dwGuildID; }
	VOID	SetGuildID(DWORD dwGuildID);

	//-------------------------------------------------------------------------------
	// �Ƿ���Ҫ���浽���ݿ�
	//-------------------------------------------------------------------------------
	BOOL IsNeedSave2DB() const { return m_nNeedSave2DBCountDown <= 0; }

	//-------------------------------------------------------------------------------
	// ������Ϣ
	//-------------------------------------------------------------------------------
	VOID SendMessage(LPVOID pMsg, DWORD dwSize) { if( P_VALID(GetSession()) ) { GetSession()->SendMessage(pMsg, dwSize); } }

	//---------------------------------------------------------------------------------
	// Ͷ���Ե�
	//---------------------------------------------------------------------------------
	INT BidAttPoint(const INT nAttPointAdd[X_ERA_ATTA_NUM]);

	//---------------------------------------------------------------------------------
	// ϴ���Ե�
	//---------------------------------------------------------------------------------
	INT ClearAttPoint(INT64 n64ItemID, BOOL bCheckItem=TRUE);

	//---------------------------------------------------------------------------------
	// ѧ����
	//---------------------------------------------------------------------------------
	INT	LearnSkill(DWORD dwSkillID, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// ְҵ�ж�
	//---------------------------------------------------------------------------------
	DWORD VocationCheck(DWORD dwVocationLimit);

	//---------------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------------
	INT LevelUpSkill(DWORD dwSkillID, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------------
	INT ForgetSkill(DWORD dwSkillID, DWORD dwNPCID);

	//---------------------------------------------------------------------------------
	// ϴ��
	//---------------------------------------------------------------------------------
	INT	ClearTalent(INT64 n64ItemID, BOOL bCheckItem=TRUE);

	//---------------------------------------------------------------------------------
	// ����/�ر�PK����
	//---------------------------------------------------------------------------------
	INT OpenSafeGuard(INT64 n64Item);
	INT CloseSafeGuard();

	//---------------------------------------------------------------------------------
	// ����/��������״̬
	//---------------------------------------------------------------------------------
	INT SetPK(BOOL bOnlyPK);
	VOID ResetPK();
	VOID UpdatePK();

	//---------------------------------------------------------------------------------
	// ����/����PVP״̬
	//---------------------------------------------------------------------------------
	VOID SetPVP();
	VOID UpdatePVP();

	//---------------------------------------------------------------------------------
	// ����PK״̬
	//---------------------------------------------------------------------------------
	VOID CalPKState(BOOL bSendMsg=TRUE);

	//---------------------------------------------------------------------------------
	// ��������Ϊ0ʱ�سǸ����pk����
	//---------------------------------------------------------------------------------
	VOID SetHostilitySafeGuard();
	VOID UpdateHostilitySafeGuard();

	//---------------------------------------------------------------------------------
	// ����
	//---------------------------------------------------------------------------------
	VOID BeRevived(INT nHP, INT nMP, Unit* pSrc);

	//---------------------------------------------------------------------------------
	// ͬ����������Ƿ�����
	//---------------------------------------------------------------------------------
	DWORD UpdateFriOnline();

	//---------------------------------------------------------------------------------
	// Ŀ���Ƿ���ȫ�ɼ�//??
	//---------------------------------------------------------------------------------
	BOOL CanSeeTargetEntirely(Unit *pUnit)
	{
		return (FriendEnemy(pUnit) & ETFE_Friendly) == ETFE_Friendly;
	}

	//---------------------------------------------------------------------------------
	// �������ø����
	//---------------------------------------------------------------------------------
	DWORD QuestSetRebornMap(UINT16 u16QuestID, DWORD &dwBornMapID, Vector3 &vBornPos);

	//---------------------------------------------------------------------------------
	// ��ӣ�ɾ���͸ı似�ܼ���
	//---------------------------------------------------------------------------------
	virtual VOID AddSkill(Skill* pSkill, BOOL bSendMsg=TRUE)
	{
		Unit::AddSkill(pSkill);

		if( bSendMsg )
		{
			// ���͸��Լ����һ������
			tagNS_AddSkill send;
			pSkill->GenerateMsgInfo(&send.Skill);
			SendMessage(&send, send.dwSize);

			// ���浽���ݿ�
			tagNDBC_AddSkill send1;
			send1.dwRoleID = GetID();
			send1.Skill.dwID = pSkill->GetID();
			send1.Skill.nSelfLevel = pSkill->GetSelfLevel();
			send1.Skill.nLearnLevel = pSkill->GetLearnLevel();
			send1.Skill.nProficiency = pSkill->GetProficiency();
			send1.Skill.nCoolDown = pSkill->GetCoolDownCountDown();
			g_dbSession.Send(&send1, send1.dwSize);
		}
	};

	virtual VOID RemoveSkill(DWORD dwSkillID)
	{
		Unit::RemoveSkill(dwSkillID);

		// ���͸��Լ�ɾ��һ������
		tagNS_RemoveSkill send;
		send.dwSkillID = dwSkillID;
		SendMessage(&send, send.dwSize);

		// ���͵����ݿ�ɾ��
		tagNDBC_RemoveSkill send1;
		send1.dwRoleID = GetID();
		send1.dwSkillID = dwSkillID;
		g_dbSession.Send(&send1, send1.dwSize);
	}

	virtual VOID ChangeSkillLevel(Skill* pSkill, ESkillLevelChange eType, INT nChange=1)
	{
		Unit::ChangeSkillLevel(pSkill, eType, nChange);

		// ���͸��Լ�����һ������
		tagNS_UpdateSkill send;
		pSkill->GenerateMsgInfo(&send.Skill);
		SendMessage(&send, send.dwSize);

		tagNDBC_UpdateSkill send1;
		send1.dwRoleID = GetID();
		send1.Skill.dwID = pSkill->GetID();
		send1.Skill.nSelfLevel = pSkill->GetSelfLevel();
		send1.Skill.nLearnLevel = pSkill->GetLearnLevel();
		send1.Skill.nProficiency = pSkill->GetProficiency();
		send1.Skill.nCoolDown = pSkill->GetCoolDownCountDown();
		g_dbSession.Send(&send1, send1.dwSize);
	}

	virtual VOID ChangeSkillExp(Skill *pSkill, INT nValue)
	{
		Unit::ChangeSkillExp(pSkill, nValue);
		
		// ���͸��Լ�����һ������
		tagNS_UpdateSkill send;
		pSkill->GenerateMsgInfo(&send.Skill);
		SendMessage(&send, send.dwSize);
	}

	virtual VOID StartSkillCoolDown(Skill* pSkill)
	{
		Unit::StartSkillCoolDown(pSkill);

		// ���͸��Լ����¼���CD
		tagNS_UpdateSkillCoolDown send;
		send.dwSkillID = pSkill->GetID();
		send.nCoolDown = pSkill->GetCoolDownCountDown();
		SendMessage(&send, send.dwSize);
		
	}

	virtual VOID ClearSkillCoolDown(DWORD dwSkillID)
	{
		Unit::ClearSkillCoolDown(dwSkillID);

		// ���͸��Լ����¼���CD
		tagNS_UpdateSkillCoolDown send;
		send.dwSkillID = dwSkillID;
		send.nCoolDown = 0;
		SendMessage(&send, send.dwSize);
	}

	//--------------------------------------------------------------------------------
	// ���Ըı�������������ݸı�
	//--------------------------------------------------------------------------------
	virtual VOID OnAttChange(INT nIndex);
	virtual VOID OnAttChange(bool bRecalFlag[ERA_End]);

	//--------------------------------------------------------------------------------
	// ��û���پ���
	//--------------------------------------------------------------------------------
	VOID ExpChange(INT nValue, BOOL bKill=FALSE, BOOL bForce=FALSE);

	//--------------------------------------------------------------------------------
	// �ı�ȼ�
	//--------------------------------------------------------------------------------
	VOID LevelChange(INT nValue, BOOL bKill=FALSE, BOOL bForce=FALSE);

	//--------------------------------------------------------------------------------
	// ������װ��Ч
	//--------------------------------------------------------------------------------
	VOID SetSuitEffect(DWORD dwSuitEffect)
	{ 
		m_AvatarEquipEquip.dwSuitEffectID = dwSuitEffect;
		m_AvatarEquipFashion.dwSuitEffectID = dwSuitEffect;
	}

	//--------------------------------------------------------------------------------
	// ��װ���
	//--------------------------------------------------------------------------------
	VOID  ProcEquipEffect(tagEquip* pNewEquip, tagEquip* pOldEquip, INT16 n16IndexOld, BOOL bDiscard = FALSE);
	INT32 GetEquipDisplayPos(INT16 n16EquipPos);
	DWORD Equip(INT64 n64Serial, EEquipPos ePosDst);
	VOID  ResetWeaponDmg(tagEquip& Equip);				// ո�¶ȱ仯ʱ����

	//--------------------------------------------------------------------------------
	// ��ɫ�ɼ����ܼӳ�
	//--------------------------------------------------------------------------------
	INT CalGatherRate( Creature* pCreature );

	//--------------------------------------------------------------------------------
	// ��ɫʵ�ʴ�ֻ�þ������
	//--------------------------------------------------------------------------------
	INT CalRealGainExp( INT nSrcExp );

	//----------------------------------------------------------------------------------
	// ���������õ������б����Ӧλ��
	//----------------------------------------------------------------------------------
	tagFriend GetFriend(INT nIndex)
	{
		ASSERT(nIndex < MAX_FILENAME && nIndex >= 0);
		return m_Friend[nIndex];
	}

	VOID SetFriend(INT nIndex, DWORD dwFriendID, DWORD dwFriVal = 0, BYTE byGroup = 1)
	{
		ASSERT(nIndex < MAX_FRIENDNUM && nIndex >= 0);
		ASSERT(byGroup < 5 && byGroup > 0);

		if(m_Friend[nIndex].dwFriendID == GT_INVALID && dwFriendID != GT_INVALID)
		{
			m_mapFriend.Add(dwFriendID, &m_Friend[nIndex]);
		}
		else if(m_Friend[nIndex].dwFriendID != GT_INVALID && dwFriendID == GT_INVALID)
		{
			m_mapFriend.Erase(m_Friend[nIndex].dwFriendID);
		}

		m_Friend[nIndex].dwFriendID = dwFriendID; 
		if(dwFriendID == GT_INVALID)
			m_Friend[nIndex].dwFriVal = 0;
		else
			m_Friend[nIndex].dwFriVal += dwFriVal;	
		m_Friend[nIndex].byGroup = byGroup;

		m_Friend[nIndex].dwFriVal = (m_Friend[nIndex].dwFriVal > MAX_FRIENDVAL) ? MAX_FRIENDVAL : m_Friend[nIndex].dwFriVal;
	}

	tagFriend* GetFriendPtr(DWORD dwFriendID) { return m_mapFriend.Peek(dwFriendID); }

	//----------------------------------------------------------------------------------
	// ���������õ�����������Ӧλ��
	//----------------------------------------------------------------------------------
	DWORD GetBlackList(INT nIndex)
	{
		ASSERT(nIndex < MAX_BLACKLIST && nIndex >= 0);
		return m_dwBlackList[nIndex];
	}

	VOID SetBlackList(INT nIndex, DWORD dwRoleID)
	{
		ASSERT(nIndex < MAX_BLACKLIST && nIndex >= 0);
		m_dwBlackList[nIndex] = dwRoleID;
	}

	//----------------------------------------------------------------------------------
	// ����С�Ӻ��Ŷ�ID
	//----------------------------------------------------------------------------------
	VOID SetTeamID(DWORD dwTeamID)		{ m_dwTeamID = dwTeamID; }
	VOID SetGroupID(DWORD dwGroupID)	{ m_dwGroupID = dwGroupID; }
	VOID SetTeamInvite(DWORD dwRoleID)	{ m_dwTeamInvite = dwRoleID; }

	BOOL IsTeamMate(Role* pRole)		{ return GetTeamID() != GT_INVALID && GetTeamID() == pRole->GetTeamID(); }
	BOOL IsGuildMate(Role* pRole)		{ return GetGuildID() != GT_INVALID && GetGuildID() == pRole->GetGuildID(); }
	BOOL IsLover(Role* pRole)			{ return GetLoverID() != GT_INVALID && GetLoverID() == pRole->GetID(); }

	//----------------------------------------------------------------------------------
	// ��ĳ����λ�ǲ����Ѻõ�����ϵ�����ѣ���ݣ�ʦͽ�����ޣ�ͬ�ŵȣ�
	//----------------------------------------------------------------------------------
	BOOL IsFriendlySocial(Role* pRole)	{ return IsTeamMate(pRole) || IsGuildMate(pRole) || IsLover(pRole); }

	//----------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------
	DWORD		GetOwnInstanceID()	const;
	DWORD		GetOwnInstanceMapID() const;
	DWORD		GetMyOwnInstanceID() const			{ return m_dwOwnInstanceID; }
	DWORD		GetMyOwnInstanceMapID() const		{ return m_dwOwnInstanceMapID; }
	DWORD		GetExportMapID() const				{ return m_dwExportMapID; }
	Vector3&	GetExportPoint()					{ return m_vExport; }
	VOID		SetMyOwnInstanceID(DWORD dwID)		{ m_dwOwnInstanceID = dwID; }
	VOID		SetMyOwnInstanceMapID(DWORD dwID)	{ m_dwOwnInstanceMapID = dwID; }
	VOID		SetExportMapID(DWORD dwMapID)		{ m_dwExportMapID = dwMapID; }
	VOID		SetExportPoint(Vector3 &vExport)	{ m_vExport = vExport; }

	//----------------------------------------------------------------------------------
	// �̳����
	//----------------------------------------------------------------------------------
	tagDWORDTime GetLastMallFreeTime() const { return m_LastGetMallFreeTime; }

	VOID SetLastGetMallFreeTime(DWORD dwNewLastGetTime) { m_LastGetMallFreeTime = dwNewLastGetTime; }
	
	//----------------------------------------------------------------------------------
	// ��������״̬����
	//----------------------------------------------------------------------------------
	BOOL OnActiveItemBuffTrigger(tagItem* pItem, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// ��������״̬����
	//----------------------------------------------------------------------------------
	BOOL OnPassiveSkillBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// װ����������
	//----------------------------------------------------------------------------------
	BOOL OnEquipmentBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// ע�ᴥ�������ܹ�����
	//----------------------------------------------------------------------------------
	VOID RegisterTriggerSkillSet(ETriggerEventType eEvent, DWORD dwSkillID);

	//----------------------------------------------------------------------------------
	// ��ע�ᴥ�������ܹ�����
	//----------------------------------------------------------------------------------
	VOID UnRegisterTriggerSkillSet(ETriggerEventType eType, DWORD dwSkillID);

	//----------------------------------------------------------------------------
	// ע�ᴥ����װ��������
	//----------------------------------------------------------------------------
	VOID RegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		m_bitsetEquipTrigger[eTriggerType][n16EquipPos] = true;
	}

	//----------------------------------------------------------------------------
	// ��ע�ᴥ����װ��������
	//----------------------------------------------------------------------------
	VOID UnRegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		m_bitsetEquipTrigger[eTriggerType][n16EquipPos] = false;
	}

	//----------------------------------------------------------------------------
	// ע�ᴥ�������������
	//----------------------------------------------------------------------------
	VOID RegisterTriggerLongHunSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		m_bitsetLongHunTrigger[eTriggerType][n16EquipPos] = true;
	}

	//----------------------------------------------------------------------------
	// ��ע�ᴥ�������������
	//----------------------------------------------------------------------------
	VOID UnRegisterTriggerLongHunSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		m_bitsetLongHunTrigger[eTriggerType][n16EquipPos] = false;
	}

	//----------------------------------------------------------------------------------
	// ע�ᴥ������װ������
	//----------------------------------------------------------------------------------
	VOID RegisterTriggerSuitSet(DWORD dwTriggerID, DWORD dwBuffID, DWORD dwSuitID)
	{
		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		m_setSuitTrigger[eTriggerType].insert(dwSuitID);
	}

	//----------------------------------------------------------------------------------
	// ��ע�ᴥ������װ������
	//----------------------------------------------------------------------------------
	VOID UnRegisterTriggerSuitSet(DWORD dwTriggerID, DWORD dwBuffID, DWORD dwSuitID)
	{
		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		m_setSuitTrigger[eTriggerType].erase(dwSuitID);
	}
	
	//----------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------
	VOID	UpdateLoverID(DWORD dwLoverID)	{ m_dwLoverID = dwLoverID; }
	DWORD	GetLoverID()					{ return m_dwLoverID; }

	BOOL IsRoleMarried() { return (m_dwLoverID != GT_INVALID) ? TRUE:FALSE ; } //�Ƿ��ѻ�
	BOOL IsRoleWedded() { return m_bHaveWedding; } //�Ƿ���й�����


private:
	//----------------------------------------------------------------------------------
	// װ�����buffԤ��������ȡtigger����
	//----------------------------------------------------------------------------------
	EPassiveSkillAndEquipTrigger PreRegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, BOOL bEquip);

	//----------------------------------------------------------------------------------
	// װ����ر���trigger��������
	//----------------------------------------------------------------------------------
	VOID OnEquipmentBuffTriggerCommon(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);
	VOID OnEquipmentBuffTriggerLongHun(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);
	VOID OnEquipmentBuffTriggerSuit(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);

private:
	//------------------------------------------------------------------------------
	// ���Ϳͻ��˳�ʼ����
	//------------------------------------------------------------------------------
	VOID SendInitStateAtt();
	VOID SendInitStateSkill();
	VOID SendInitStateItem();	// ��Ʒ&װ��
	VOID SendInitStateSuit();	// ��װ
	VOID SendInitStateLongHun();
	VOID SendInitStateCompleteQuest();
	VOID SendInitStateIncompleteQuest();
	VOID SendInitStateMoney();
	VOID SendInitStateReputation();
	VOID SendFriendAndEnemy();
	VOID SendInitStateGuild();				// ����
	VOID SendFatigueGuardInfo(BYTE byCode);	// ������

	//------------------------------------------------------------------------------
	// ��ʼ�������ݿ��ж�ȡ���б�����
	//------------------------------------------------------------------------------
	VOID InitAtt(const tagRoleDataSave* pData);
	VOID InitSkill(const BYTE* &pData, INT32 nNum);
	VOID InitBuff(const BYTE* &pData, INT32 nNum);
	VOID InitItem(const BYTE* &pData, INT32 nNum);
	VOID InitItemCDTime(const BYTE* &pData, INT32 nNum);
	VOID InitFriend(const BYTE* &pData, INT32 nNum);
	VOID InitFriendValue(const BYTE* &pData, INT32 nNum);
	VOID InitBlackList(const BYTE* &pData, INT32 nNum);

	DWORD Put2Container(tagItem* pItem);

	VOID CalInitAtt();

	VOID CalInitState();

	//------------------------------------------------------------------------------
	// ��ʼ���豣�����ݿ������
	//------------------------------------------------------------------------------
	VOID SaveSkill2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID SaveBuff2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID SaveQuest2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	//------------------------------------------------------------------------------
	// ���ñ��浽���ݿ�ʱ�䵹��ʱ
	//------------------------------------------------------------------------------
	VOID ResetNeedSave2DBCD() { m_nNeedSave2DBCountDown = MIN_ROLE_SAVE2DB_INTERVAL; }

	//------------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------------
	DWORD SetRebornMap(DWORD dwNPCID, DWORD &dwBornMapID, Vector3 &vBornPos);
	DWORD Revive(ERoleReviveType eType, INT64 n64ItemID, BOOL bNeedItem=TRUE);

	DWORD CityRevive();
	DWORD LocusRevive(INT64 n64ItemID, BOOL bNeedItem=TRUE);
	DWORD AcceptRevive();
	DWORD PrisonRevive();
	//------------------------------------------------------------------------------
	// ��װ
	//------------------------------------------------------------------------------
	DWORD	CanEquip(tagEquip* pEquip, EEquipPos ePosDst);
	DWORD	Unequip(INT64 n64Serial, INT16 n16IndexDst);
	DWORD	SwapWeapon();
	DWORD	MoveRing(INT64 n64SerialSrc, INT16 n16PosDst);
	VOID	ProcEquipEffectPos(tagEquip* pNewEquip, tagEquip* pOldEquip, EItemConType eConTypeNewDst, EItemConType eConTypeNewSrc);
	VOID	ProcEquipEffectAtt(tagEquip* pEquip, bool bEquip, const INT16 n16Index);
	VOID	ProcEquipEffectAvatar(tagEquip* pNewEquip, INT16 n16IndexOld);
	INT32	ResetOneEquipDisplay(tagEquip* pEquip, INT16 n16Index);
	VOID	ChangeRoleAtt(const tagRoleAttEffect Effect[], INT32 nArraySz, INT32 nFactor);
	VOID	ChangeRoleAtt(const INT32 nValue[], INT32 nArraySz, INT32 nAttStart, INT32 nFactor);
	VOID	ChangeRoleAtt(const DWORD dwValue[], INT32 nArraySz, INT32 nFactor);
	VOID	ProcEquipBuffTrigger(DWORD dwBuffID, BOOL bEquip);

	//------------------------------------------------------------------------------
	// �����˺���ؼ���
	//------------------------------------------------------------------------------
	/* ����ĥ��������˺�Ӱ��.����ֵΪ�������˺�Ӱ��İٷֱ�ֵ.*/
	FLOAT	CalAbrasionEffect(const tagEquip& Equip);
	VOID	ResetWeaponDmg(const tagEquip& Equip, BOOL bEquip);	// ��װʱ����

	//------------------------------------------------------------------------------
	// ��Ҽ佻�����
	//------------------------------------------------------------------------------
	ExchangeMgr& GetExchMgr() { return m_ExchMgr; }
	
	BOOL	IsExchanging()					{ return IsInRoleState(ERS_Exchange); }
	BOOL	CanExchange()
	{
		return !IsInRoleStateAny(ERS_Exchange | ERS_Shop | ERS_Stall | ERS_Wedding) 
				&& (GetExchMgr().GetTgtRoleID() == GT_INVALID);
	}
	
	VOID	BeginExchange(DWORD dwTgtRoleID)
	{
		SetRoleState(ERS_Exchange);
		GetExchMgr().CreateData();
		GetExchMgr().SetTgtRoleID(dwTgtRoleID);
	}

	VOID	EndExchange()
	{
		UnsetRoleState(ERS_Exchange);
		GetExchMgr().DeleteData();
		GetExchMgr().SetTgtRoleID(GT_INVALID);
	}

	DWORD	ProcExchangeReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD	ProcExchangeReqRes(OUT Role* &pApplicant, DWORD dwTgtRoleID, DWORD dwErrorCode);
	DWORD	ProcExchangeAdd(OUT Role* &pTarget, OUT tagItem* &pItem, INT32 &nInsIndex, INT64 n64Serial);
	DWORD	ProcExchangeDec(OUT Role* &pTarget, INT64 n64Serial);
	DWORD	ProcExchangeMoney(OUT Role* &pTarget, INT64 n64Silver);
	DWORD	ProcExchangeLock(OUT Role* &pTarget);
	DWORD	ProcExchangeCancel(OUT Role* &pTarget);
	DWORD	ProcExchangeVerify(OUT Role* &pTarget, OUT DWORD &dwFailedRoleID);

	DWORD	VerifyExchangeData(OUT tagItem* pItem[]);
	DWORD	ProcExchange();

	DWORD	ProcPrepareExchange(OUT Role* &pTarget);

	//---------------------------------------------------------------------------------
	// �̵����
	//---------------------------------------------------------------------------------
	DWORD	GetShopItems(DWORD dwNPCID, BYTE byShelf);
	DWORD	GetShopEquips(DWORD dwNPCID, BYTE byShelf);
	DWORD	BuyShopItem(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT16 n16ItemNum);
	DWORD	BuyShopEquip(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT64 n64Serial);
	DWORD	SellToShop(DWORD dwNPCID, INT64 n64Serial);

	VOID	SendShopFeedbackMsg(DWORD dwErrorCode, DWORD dwNPCID);

	//-----------------------------------------------------------------------------
	// �̳����
	//-----------------------------------------------------------------------------
	DWORD	GetMallAll(OUT DWORD &dwMallTime);
	DWORD	UpdateMallAll(OUT DWORD &dwNewMallTime, IN DWORD dwOldMallTime);
	DWORD	BuyMallItem(DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex);
	DWORD	BuyMallPack(DWORD dwID, INT nUnitPrice, BYTE byIndex);
	DWORD	BuyMallItem(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
							DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex);
	DWORD	BuyMallPack(DWORD dwTgtRoleID, LPCTSTR szLeaveWord,
							DWORD dwID, INT nUnitPrice, BYTE byIndex);
	DWORD	GetMallFreeItem(DWORD dwID);

	//-----------------------------------------------------------------------------
	// �������
	//-----------------------------------------------------------------------------
	//���
	DWORD ProcGetMarriageReq(OUT Role* &pTarget, DWORD dwTgtRoleID, DWORD dwNpcID);
	DWORD ProcGetMarriageReqRes(OUT Role* &pApplicant, DWORD dwApplicantRoleID, DWORD dwErrorCode);

	//���
	DWORD ProcBreakMarriageReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD ProcBreakMarriageReqRes(OUT Role* &pApplicant, DWORD dwApplicantRoleID, DWORD dwErrorCode);

	//����
	DWORD ProcCelebrateWeddingReq(OUT Role* &pTarget,OUT Role* &pOfficiatorRole,DWORD dwOfficiatorNameCrc,DWORD dwNpcID);
	DWORD ProcCelebrateWeddingReqRes(OUT Role* &pApplicant, DWORD dwErrorCode);
public:
	DWORD	MallItemExchange(DWORD dwMallID, INT nPrice, INT16 n16BuyNum, BYTE byIndex);
	DWORD	MallPackExchange(DWORD dwMallID, INT nPrice, BYTE byIndex);

public:
	DWORD	RoleSetVocation(DWORD dwNPCID, EClassType eVocation);
	DWORD	RoleSetHeroVocation(DWORD dwNPCID, EClassTypeEx eHVocation);
	DWORD	ChangeVocation(DWORD dwNPCID, EClassType eVocation);

private:

	//-----------------------------------------------------------------------------
	// �̳�Ԫ���������
	//-----------------------------------------------------------------------------
	DWORD	SaveYB2Account(DWORD dwID, INT nNum);
	DWORD 	SaveSilver2Account(DWORD dwID, INT64 nNum);
	DWORD 	DepositYBAccout(DWORD dwID, INT nNum);
	DWORD 	DepositSilverAccount(DWORD dwID, INT64 nNum);
	DWORD 	GetYBTradeInfo();
	DWORD 	SubmitSellOrder(DWORD dwRoleID, INT nNum, INT nPrice);
	DWORD 	SubmitBuyOrder(DWORD dwRole, INT nNum, INT nPrice);
	DWORD 	DeleteOrder(DWORD dwRoleID, DWORD dwOrderID, EYBOTYPE eYBOType);
	DWORD 	GetYBOrder(DWORD dwRoleID);

public:
	//------------------------------------------------------------------------
	// ��̯���
	//------------------------------------------------------------------------
	DWORD	StartStall()
	{
		DWORD dwRtv = m_pStall->Init(0);
		if (E_Success == dwRtv)
		{
			StopMount();
		}
		return dwRtv;//?? ����˰��
	}

	DWORD	SetStallGoods(INT64 n64Serial, INT64 n64UnitPrice, BYTE byIndex)
	{
		return m_pStall->SetGoods(n64Serial, n64UnitPrice, byIndex);
	}

	DWORD	UnsetStallGoods(const BYTE byIndex)
	{
		return m_pStall->UnSetGoods(byIndex);
	}

	DWORD	SetStallTitle(LPCTSTR strTitle)
	{
		return m_pStall->SetTitle(strTitle);
	}

	DWORD	SetStallAdText(LPCTSTR strAdText)
	{
		return m_pStall->SetAdText(strAdText);
	}

	DWORD	SetStallAdFlag(bool bAdFlag)
	{
		return m_pStall->SetAdFlag(bAdFlag);
	}

	DWORD	SetStallFinish()
	{
		return m_pStall->SetFinish();
	}

	DWORD	CloseStall()
	{
		return m_pStall->Destroy();
	}

	DWORD	GetStallTitle(OUT LPTSTR pSzTitle)
	{
		return m_pStall->GetStallTitle(pSzTitle);
	}

	DWORD	GetStallSpecGoods(BYTE byIndex, OUT LPVOID pData, OUT INT &nGoodsSz)
	{
		return m_pStall->GetSpecGoods(byIndex, pData, nGoodsSz);
	}
	
	DWORD	GetStallGoods(OUT LPVOID pData, OUT BYTE &byGoodsNum, OUT INT &nGoodsSz)
	{
		return m_pStall->GetGoods(pData, byGoodsNum, nGoodsSz);
	}

	DWORD	BuyStallGoods(Role *pBuyer, INT64 n64Serial, INT64 n64UnitPrice, 
							INT16 n16Num, BYTE byIndex, OUT INT16 &n16RemainNum)
	{
		return m_pStall->Sell(pBuyer, n64UnitPrice, n64Serial, byIndex, n16Num, n16RemainNum);
	}

	BYTE	GetStallModeLevel() const
	{
		return m_pStall->GetModeLevel();
	}

	//INT8	GetStallGoodsNum() const
	//{
	//	return m_pStall->GetGoodsNum();
	//}

	BOOL	IsNoGoodsInStall() const
	{
		return m_pStall->IsEmpty();
	}

	BOOL	IsSetGoodsFinish() const
	{
		return IsInRoleState(ERS_Stall);
	}

	INT32	CalStallGoodsMemUsed() const	// ����̯λ����Ʒ����ṹtagMsgStallGoodsʱ�Ĵ�С
	{
		return m_pStall->CalMemSzGoodsUsed();
	}

	DWORD	SendCloseStall();

	DWORD	GainStallExp(INT32 nExp)
	{
		return m_pStall->GainExp(nExp);
	}

private:
	//---------------------------------------------------------------------------------
	// ����ְ��NPC����
	//---------------------------------------------------------------------------------
	DWORD	ProcDak(DWORD dwNPCID, INT32 nIndex, DWORD dwMapID);

	//---------------------------------------------------------------------------------
	// ʹ��ĥʯ
	//---------------------------------------------------------------------------------
	DWORD	AbraseWeapon(INT64 n64AbraserSerial, INT64 n64WeaponSerial, OUT INT32 &nAttackTimes);

	//---------------------------------------------------------------------------------
	// ��ͼ������
	//---------------------------------------------------------------------------------
	BOOL	MapTrigger(DWORD dwTriggerID, DWORD dwMisc);

	//---------------------------------------------------------------------------------
	// ֪ͨ���ѽ��븱��
	//---------------------------------------------------------------------------------
	DWORD	InstanceNotify(BOOL bNotify);

	//---------------------------------------------------------------------------------
	// ����Ƿ�ͬ��������ҽ��븱��������
	//---------------------------------------------------------------------------------
	DWORD	InstanceAgree(BOOL bAgree);

	//---------------------------------------------------------------------------------
	// ��������뿪����
	//---------------------------------------------------------------------------------
	DWORD	LeaveInstance();

	//---------------------------------------------------------------------------------
	// ǿ��װ��
	//---------------------------------------------------------------------------------
	// ����
	DWORD	PosyEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);	
	DWORD	GMPosyEquip(DWORD dwFormulaID, INT16 n16ItemIndex);	
	// �Կ�
	DWORD	EngraveEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	GMEngraveEquip(DWORD dwFormulaID, INT16 n16ItemIndex);
	// ��Ƕ
	DWORD	InlayEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID);
	// ��ӡ
	DWORD	BrandEquip(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD dwCmdID);
	// ����
	DWORD	LoongSoulEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID);
	DWORD	GMLoongSoulEquip(INT64 n64SrcItemID, INT16 n16ItemIndex);
	// ���
	DWORD	QuenchEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	// ����
	DWORD	ChiselEquip(INT64 n64SrcItemID, INT64 n64SuffID, DWORD dwCmdID);
	// ʱװȾɫ
	DWORD	DyeFashion(INT64 n64DyeSN, INT64 n64EquipSerial, DWORD dwCmdID);
	// ����B�����Զ�ǿ���ɹ��ʵ�Ӱ��
	FLOAT	CalSpecAttEffectSuc(EEquipSpecAtt eSpecAtt, FLOAT fProp, EEquipSpecAtt eSpecType);
	// ����B�����Զ�ǿ�������ʵ�Ӱ��
	FLOAT	CalSpecAttEffectPef(EEquipSpecAtt eSpecAtt, FLOAT fPerfect);
	// ������ֿ�
	DWORD	OpenSideWare(INT64 n64ItemSerial);
	
	//---------------------------------------------------------------------------------
	// �����ϳ���Ʒ
	//---------------------------------------------------------------------------------
	DWORD	ProduceItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	ProduceEquip(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	CheckProduceLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, const tagProduceProtoSer* &pProduceProto, INT64 n64StuffID[], INT32 nArraySz);
	DWORD	DeComposeItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64Item, DWORD dwCmdID);
	DWORD	CheckDeComposeLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, const tagDeComposeProtoSer* &pDeComposeProto, INT64 n64Item, tagEquip *pEquip);
	EProduceType Skill2ProduceType(ESkillTypeEx2 eSkillType);	
	tagItem*	 CreateItem(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num, DWORD dwCreator);
	VOID	CalIMEffect(EConsolidateTypeSer eConType, tagIMEffect &IMEffect, INT64 n64IMID, const LPVOID pProto);
	VOID	CalEquipFlare(tagEquip* pEquip);
	// Jason 2009-12-6 ����װ��Ǳ��ֵ���ߴ���
	DWORD RaiseEquipPotVal(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD & dwPotValAdded);

	// Jason v1.3.1 date:2009-12-21
	DWORD GetGemRemovalInfo(DWORD dwNPCID,INT64 equipID,INT8 & num,DWORD dwGemIDs[MAX_EQUIPHOLE_NUM]);
	DWORD RemoveGemFromEquip(DWORD dwNPCID,INT64 equipID,INT8 num,INT64 SignIDs[MAX_EQUIPHOLE_NUM]);

	//---------------------------------------------------------------------------------
	// ��ѯĳ���������ڵ�λ�ã����û�У�����GT_INVALD��
	//---------------------------------------------------------------------------------
	INT FindTalentIndex(ETalentType eType)
	{
		INT nBegin = X_MAIN_TALENT_START, nEnd = X_ASS_TALENT_START;	// ������ʼ����յ�

		// ����Ǹ������ʣ���ȡ��벿��
		if( ETT_Action == eType || ETT_Jugglery == eType )
		{
			nBegin	=	X_ASS_TALENT_START;
			nEnd	=	X_MAX_TALENT_PER_ROLE;
		}

		// ����������������ʣ���
		for(INT n = nBegin; n < nEnd; n++)
		{
			if( m_Talent[n].eType == eType )
				return n;
		}

		return GT_INVALID;
	}

	//---------------------------------------------------------------------------------
	// �ҵ�ĳ���ɲ��������λ�ã����û�У�����GT_INVALID��
	//---------------------------------------------------------------------------------
	INT FindBlankTalentIndex(ETalentType eType)
	{
		INT nBegin = X_MAIN_TALENT_START, nEnd = X_ASS_TALENT_START;	// ������ʼ����յ�

		// ����Ǹ������ʣ���ȡ��벿��
		if( ETT_Action == eType || ETT_Jugglery == eType )
		{
			nBegin	=	X_ASS_TALENT_START;
			nEnd	=	X_MAX_TALENT_PER_ROLE;
		}

		for(INT n = nBegin; n < nEnd; n++)
		{
			if( ETT_Null == m_Talent[n].eType )
				return n;
		}

		return GT_INVALID;
	}

	//---------------------------------------------------------------------------------
	// ����һ������
	//---------------------------------------------------------------------------------
	VOID AddTalent(INT nIndex, ETalentType eType, INT nPoint=1)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null == m_Talent[nIndex].eType );
		ASSERT( nPoint > 0 );

		m_Talent[nIndex].eType = eType;
		m_Talent[nIndex].nPoint = nPoint;

		// ͬ��
		tagNS_AddTalent send;
		send.eType = eType;
		send.nPoint = nPoint;
		SendMessage(&send, send.dwSize);
	}

	//---------------------------------------------------------------------------------
	// ɾ��һ������
	//---------------------------------------------------------------------------------
	VOID RemoveTalent(INT nIndex)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );
		
		ETalentType eType = m_Talent[nIndex].eType;

		m_Talent[nIndex].eType = ETT_Null;
		m_Talent[nIndex].nPoint = 0;

		// ͬ��
		tagNS_RemoveTalent send;
		send.eType = eType;
		SendMessage(&send, send.dwSize);
	}


	//---------------------------------------------------------------------------------
	// �õ�ĳ����������Ӧ�����ʵ�ǰͶ����
	//---------------------------------------------------------------------------------
	INT GetTalentPoint(INT nIndex)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );

		return m_Talent[nIndex].nPoint;
	}

	//---------------------------------------------------------------------------------
	// �����ʼӵ�
	//---------------------------------------------------------------------------------
	VOID AddTalentPoint(INT nIndex, INT nPoint=1)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );
		ASSERT( nPoint > 0 );

		m_Talent[nIndex].nPoint += nPoint;

		// ͬ��
		tagNS_UpdateTalent send;
		send.eType = m_Talent[nIndex].eType;
		send.nPoint = m_Talent[nIndex].nPoint;
		SendMessage(&send, send.dwSize);
	}

	//---------------------------------------------------------------------------------
	// ������
	//---------------------------------------------------------------------------------
	virtual VOID OnBeAttacked(Unit* pSrc, Skill* pSkill, BOOL bHited, BOOL bBlock, BOOL bCrited);

	//---------------------------------------------------------------------------------
	// ����
	//---------------------------------------------------------------------------------
	virtual VOID OnDead(Unit* pSrc, Skill* pSkill=NULL, const tagBuffProto* pBuff=NULL, DWORD dwSerial=GT_INVALID, DWORD dwMisc=0);

	//---------------------------------------------------------------------------------
	// ��ɱ
	//---------------------------------------------------------------------------------
	virtual VOID OnKill(Unit* pSrc);

	//---------------------------------------------------------------------------------
	// �Ƿ���������ͷ�
	//---------------------------------------------------------------------------------
	BOOL IsDeadPenalty(EDeadPenaltyMode eMode);

	//---------------------------------------------------------------------------------
	// �����ͷ�
	//---------------------------------------------------------------------------------
	VOID DeadPenalty(Unit* pSrc, EDeadPenaltyMode eMode);

	//---------------------------------------------------------------------------------
	// �Ƿ�����������ͷ�
	//---------------------------------------------------------------------------------
	VOID CanCancelPenalty(Unit* pSrc, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommercePenalty);

	//---------------------------------------------------------------------------------
	// Ŀ������ͱ�־
	//---------------------------------------------------------------------------------
	virtual DWORD TargetTypeFlag(Unit* pTarget);

	//---------------------------------------------------------------------------------
	// ״̬��־
	//---------------------------------------------------------------------------------
	virtual DWORD GetStateFlag()
	{
		DWORD dwStateFlag = Unit::GetStateFlag();

		dwStateFlag |= ( IsInRoleState(ERS_Mount)		?	ESF_Mount	:	ESF_NoMount );
		dwStateFlag |= ( IsInRoleState(ERS_PrisonArea)	?	ESF_Prison	:	ESF_NoPrison );
		dwStateFlag |= ( IsInRoleState(ERS_Commerce)	?	ESF_Commerce:	ESF_NoCommerce );

		return dwStateFlag;
	}

	//---------------------------------------------------------------------------------
	// ��Ŀ��ĵ��������ж�
	//---------------------------------------------------------------------------------
	virtual DWORD FriendEnemy(Unit* pTarget);

	//----------------------------------------------------------------------------------
	// ���������Ͷ�Ӧ�������ܺ�װ������������
	//----------------------------------------------------------------------------------
	EPassiveSkillAndEquipTrigger TriggerTypeToPassiveSkillAndEquipTriggerType(ETriggerEventType eType)
	{   
		switch(eType)
		{
		case ETEE_BeAttacked:
			return EPSAET_BeAttacked;
			break;

		case ETEE_Hit:
			return EPSAET_Hit;
			break;

		case ETEE_Hited:
			return EPSAET_Hited;
			break;

		case ETEE_Dodged:
			return EPSAET_Dodged;
			break;

		case ETEE_Dodge:
			return EPSAET_Dodge;
			break;

		case ETEE_Blocked:
			return EPSAET_Blocked;
			break;

		case ETEE_Block:
			return EPSAET_Block;

		case ETEE_Crit:
			return EPSAET_Crit;
			break;

		case ETEE_Crited:
			return EPSAET_Crited;
			break;

		case ETEE_Die:
			return EPSAET_Die;
			break;

		case ETEE_Random:
			return EPSAET_Random;
			break;

		case ETEE_Attack:
			return EPSAET_Attack;
			break;

		case ETEE_Kill:
			return EPSAET_Kill;

		default:
			return EPSAET_Null;
			break;
		}
	}
	
private:
	DWORD TargetTypeFlag(Role* pTarget);
	DWORD TargetTypeFlag(Creature* pTarget);
	DWORD FriendEnemy(Role* pTarget);
	DWORD FriendEnemy(Creature* pCreature);

private:
	//---------------------------------------------------------------------------------
	// ����ɫ���ݱ��浽���ݿ����
	//---------------------------------------------------------------------------------
	class SaveRole
	{
	public:
		SaveRole();
		~SaveRole();

		VOID Init();
		operator tagNDBC_SaveRole*() { return m_pSaveRole; }

	private:
		tagNDBC_SaveRole*	m_pSaveRole;
	};

	static SaveRole	m_SaveRole;
	static Mutex	m_SaveRoleLock;
	
	//---------------------------------------------------------------------------------
	// �������
	//---------------------------------------------------------------------------------
	struct tagRevive
	{
		INT		nHP;
		INT		nMP;

		DWORD	dwMapID;
		FLOAT	fX;
		FLOAT	fY;
		FLOAT	fZ;

		tagRevive() { ZeroMemory(this, sizeof(*this)); }
	};

	tagRevive m_Revive;

private:
	//---------------------------------------------------------------------------------
	// ��Ӧ��session
	//---------------------------------------------------------------------------------
	PlayerSession*				m_pSession;

protected:
	//-------------------------------------------------------------------------------------
	// ʱ�����
	//-------------------------------------------------------------------------------------
	tagDWORDTime		m_CreatTime;						// ����ʱ��
	tagDWORDTime		m_LoginTime;						// ��½ʱ��
	tagDWORDTime		m_LogoutTime;						// �ǳ�ʱ��
	INT					m_nOnlineTime;						// �ۼ�����ʱ�䣨�룩
	INT					m_nCurOnlineTime;					// ��������ʱ�䣨�룩

	tagDWORDTime		m_CloseSafeGuardTime;				// ��һ�ιر�PK������ʱ��
	INT					m_nUnSetPKTickCountDown;			// �ر��������״̬��Tick����ʱ
	INT					m_nUnSetPVPTickCountDown;			// �ر����PVP״̬��Tick����ʱ
	INT					m_nNeedSave2DBCountDown;			// ���Ա������ݿ⵹��ʱ
	INT					m_nDeadUnSetSafeGuardCountDown;		// �ر��������Ϊ0ʱ�سǸ��������ϵ�pk��������tick

	DWORD				m_dwItemTransportMapID;				//��ǰ��¼���ͷ��󶨵�ͼID
	FLOAT				m_fItemTransportX;					//��ǰ��¼���ͷ��󶨵�x����
	FLOAT				m_fItemTransportZ;					//��ǰ��¼���ͷ��󶨵�z����
	FLOAT				m_fItemTransportY;					//��ǰ��¼���ͷ��󶨵�y����

	tagDWORDTime		m_LastGetMallFreeTime;				// ��һ�δ��̳ǻ�ȡ�����Ʒʱ��

	//-------------------------------------------------------------------------------------
	// ��ҵ�ǰ״̬
	//-------------------------------------------------------------------------------------
	RoleState			m_RoleState;						// ���״̬	-- �仯����֪ͨ��Χ���
	RoleStateEx			m_RoleStateEx;						// ���״̬ -- �仯��ֻ��Ҫ֪ͨ�Լ�
	ERolePKState		m_ePKState;							// ��ǰPK״̬

	//-------------------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------------------
	tagAvatarAtt		m_Avatar;							// �������
	tagAvatarEquip		m_AvatarEquipEquip;					// װ�����
	tagAvatarEquip		m_AvatarEquipFashion;				// ʱװ���
	tagDisplaySet		m_DisplaySet;						// װ����ʾ����
	EClassType			m_eClass;							// ְҵ
	EClassTypeEx		m_eClassEx;							// ��չְҵ
	INT					m_nCredit;							// ���ö�
	INT					m_nIdentity;						// ���
	INT					m_nVIPPoint;						// ��Ա����
	DWORD				m_dwGuildID;						// ��������ID
	DWORD				m_dwLoverID;						// ���˶�Ӧ��roleid û������GT_INVALID
	BOOL				m_bHaveWedding;						// �Ƿ���й�����
	BOOL				m_bNeedPrisonRevive;				// �Ƿ���Ҫ��������
	//-------------------------------------------------------------------------------------
	// ��Ʒ��ȴ
	//-------------------------------------------------------------------------------------
	BOOL				m_bObjectCoolOff;					// �����ڵ���Ʒ�Ƿ���ȴ

	//-------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------
	BOOL				m_bSpeakOff;						// ����Ƿ񱻽���

	//-------------------------------------------------------------------------------------
	// ���Ե�ͼ��ܵ�
	//-------------------------------------------------------------------------------------
	INT					m_nAttPointAdd[X_ERA_ATTA_NUM];		// Ͷ�����һ�����Ե����Ե�
	tagRoleTalent		m_Talent[X_MAX_TALENT_PER_ROLE];	// ���ʼ��������ʵ�

	//-------------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------------
	DWORD				m_dwRebornMapID;					// �����ͼID

	//-------------------------------------------------------------------------------------
	// �������ܵĴ�����������
	//-------------------------------------------------------------------------------------
	std::set<DWORD>		m_setPassiveSkillTrigger[EPSAET_End];

	//-------------------------------------------------------------------------------------
	// װ���Ĵ�����������
	//-------------------------------------------------------------------------------------
	typedef std::bitset<X_EQUIP_BAR_SIZE>	BitSetEquipPos;		/*n16EquipPos*/
	typedef std::set<DWORD>					SetSuitTrigger;		/*dwSuitID*/

	BitSetEquipPos		m_bitsetEquipTrigger[EPSAET_End];
	BitSetEquipPos		m_bitsetLongHunTrigger[EPSAET_End];
	SetSuitTrigger		m_setSuitTrigger[EPSAET_End];

	//-------------------------------------------------------------------------------------
	// ��Ǯ
	//-------------------------------------------------------------------------------------
	CurrencyMgr			m_CurMgr;

	//-------------------------------------------------------------------------------------
	// ��Ʒ������ -- �������ҡ�����ҳ��װ��������ɫ�ֿ�Ͱٱ�����
	//-------------------------------------------------------------------------------------
	ItemMgr				m_ItemMgr;							// ��Ʒ������

	//-------------------------------------------------------------------------------------
	// ��װ����
	//-------------------------------------------------------------------------------------
	Suit				m_Suit;

	//-------------------------------------------------------------------------------------
	// �����������
	//-------------------------------------------------------------------------------------
	LongHun				m_LongHun;

	//-------------------------------------------------------------------------------------
	// ��Ҽ佻�����
	//-------------------------------------------------------------------------------------
	ExchangeMgr			m_ExchMgr;

	//-------------------------------------------------------------------------------------
	// ��̯
	//-------------------------------------------------------------------------------------
	Stall				*m_pStall;

	//-------------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------------
	tagFriend						m_Friend[MAX_FRIENDNUM];				// �����б�	
	TMap<DWORD, tagFriend*>			m_mapFriend;
	DWORD							m_dwBlackList[MAX_BLACKLIST];			// ������

	//-------------------------------------------------------------------------------------
	// �Ŷ����
	//-------------------------------------------------------------------------------------
	DWORD				m_dwTeamID;											// С��ID
	DWORD				m_dwGroupID;										// �Ŷ�ID
	DWORD				m_dwTeamInvite;										// ������ID
	BOOL				m_bTeamSyn;											// С�����״̬ͬ����־

	//-------------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------------
	DWORD				m_dwOwnInstanceMapID;								// ����������ĸ����ĵ�ͼID
	DWORD				m_dwOwnInstanceID;									// ��Ҵ����ĸ���ID
	DWORD				m_dwExportMapID;									// ��������ʱ�ĵ�ͼID
	Vector3				m_vExport;											// ��������ʱ��ͼ������		

	//-------------------------------------------------------------------------------------
	// ��Զ����ҹ�����Ϣ����
	//-------------------------------------------------------------------------------------
	tagRemoteOpenSet	m_sRemoteOpenSet;

	//-------------------------------------------------------------------------------------
	// ��ҽű�
	//-------------------------------------------------------------------------------------
	const RoleScript*	m_pScript;											// ��ҽű�

	//-------------------------------------------------------------------------------------
	// ��ɫ�����������
	//-------------------------------------------------------------------------------------
	INT					m_nTreasureSum;										// ����ѿ����ı�����
	
	//-------------------------------------------------------------------------------------
	// ������״̬
	//-------------------------------------------------------------------------------------
	tagChestState		m_TreasureState;

	//-------------------------------------------------------------------------------------
	// �Ƿ���30���ѹر�pk����
	//-------------------------------------------------------------------------------------
	BOOL				m_bLevelUpClosedSafeGuard;

	// ʵ��ֵ
	tagStrength				m_Strength;											// ���ʵ��ֵ
	INT						m_nStrengthNum;										// ��ʵ�����а��е�����

public:
	//-------------------------------------------------------------------------------------
	// ��ʼ����ǰ����
	//-------------------------------------------------------------------------------------
	VOID InitCurrentQuest(const BYTE* &pData, INT32 nNum);

	//-------------------------------------------------------------------------------------
	// ��ʼ���������������
	//-------------------------------------------------------------------------------------
	VOID InitCompleteQuest(const BYTE* &pData, INT32 nNum);

	//-------------------------------------------------------------------------------------
	// ͨ��NPC����ȡ����
	//-------------------------------------------------------------------------------------
	INT AcceptQuestFromNPC(UINT16 u16QuestID, DWORD dwNPCID);

	//-------------------------------------------------------------------------------------
	// ͨ����������ȡ����
	//-------------------------------------------------------------------------------------
	INT AcceptQuestFromMapTrigger(UINT16 u16QuestID, DWORD dwMapTriggerID);

	//-------------------------------------------------------------------------------------
	// �Ƿ���Խ�ȡ���񣬷��ز���Ϊ����ɲμӣ���Ӧ������
	//-------------------------------------------------------------------------------------
	INT CanAcceptQuest(UINT16 u16QuestID, INT& nIndex, Creature* pNPC=NULL);

	//-------------------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------------------
	BOOL AddQuest(const tagQuestProto* pProto, INT nIndex);

	//-------------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------------
	INT CompleteQuest(UINT16 u16QuestID, DWORD dwNPCID, INT nChoiceItemIndex, UINT16& u16NexitQuestID);

	//-------------------------------------------------------------------------------------
	// �Ƿ���Խ�����
	//-------------------------------------------------------------------------------------
	INT CanCompleteQuest(Quest* pQuest, DWORD dwNPCID, INT nChoiceItemIndex);

	//-------------------------------------------------------------------------------------
	// ������
	//-------------------------------------------------------------------------------------
	VOID RewardQuest(Quest* pQuest, INT32 nChoiceItemIndex);

	//-------------------------------------------------------------------------------------
	// ɾ������
	//-------------------------------------------------------------------------------------
	INT DeleteQuest(UINT16 u16QuestID);

	//-------------------------------------------------------------------------------------
	// ɾ��һ������
	//-------------------------------------------------------------------------------------
	VOID RemoveQuest(UINT16 u16QuestID, BOOL bComplete);

	//-------------------------------------------------------------------------------------
	// ����NPC�Ի�����״̬
	//-------------------------------------------------------------------------------------
	VOID UpdateQuestNPCTalk(UINT16 u16QuestID, DWORD dwNPCID);

	//-------------------------------------------------------------------------------------
	// ����Trigger����״̬
	//-------------------------------------------------------------------------------------
	VOID UpdateQuestTrigger(UINT16 u16QuestID, DWORD dwTriggerID);

	//-------------------------------------------------------------------------------------
	// ���������¼�
	//-------------------------------------------------------------------------------------
	VOID OnQuestEvent(EQuestEvent eQuestType, DWORD dwEventMisc1=0, DWORD dwEventMisc2=0, DWORD dwEventMisc3=0);

	//------------------------------------------------------------------------------------
	// �õ��������ĸ���
	//------------------------------------------------------------------------------------
	INT GetCompleteQuestCount() { return m_mapCompleteQuests.Size(); }

	//------------------------------------------------------------------------------------
	// ����������������
	//------------------------------------------------------------------------------------
	VOID ClearCompleteQuest()
	{
		tagQuestDoneSave* pDoneQuest = NULL;
		QuestDoneMap::TMapIterator it = m_mapCompleteQuests.Begin();
		while( m_mapCompleteQuests.PeekNext(it, pDoneQuest) )
		{
			// ��ͻ��˷�����Ϣ
			tagNS_GMQuestState send;
			send.u16QuestID = pDoneQuest->u16QuestID;
			send.bDone = FALSE;
			SendMessage(&send, send.dwSize);

			SAFE_DEL(pDoneQuest);
		}
		m_mapCompleteQuests.Clear();
	}

	//------------------------------------------------------------------------------------
	// ��ӻ�ɾ��һ�����������
	//------------------------------------------------------------------------------------
	VOID AddDelCompleteQuest(UINT16 u16QuestID, BOOL bAddOrDel)
	{
		// ��ָ������������������
		if( bAddOrDel )
		{
			if( m_mapCompleteQuests.IsExist(u16QuestID) )
				return;

			tagQuestDoneSave* pQuestDone = new tagQuestDoneSave;
			pQuestDone->u16QuestID = u16QuestID;
			pQuestDone->nTimes = 1;
			pQuestDone->dwStartTime = g_world.GetWorldTime();

			m_mapCompleteQuests.Add(u16QuestID, pQuestDone);

			// ��ͻ��˷�����Ϣ
			tagNS_GMQuestState send;
			send.u16QuestID = u16QuestID;
			send.bDone = TRUE;
			SendMessage(&send, send.dwSize);
		}
		else // ��ָ������Ϊδ�������
		{		
			tagQuestDoneSave* pQuestDone = m_mapCompleteQuests.Peek(u16QuestID);
			if( P_VALID(pQuestDone) )
			{
				m_mapCompleteQuests.Erase(u16QuestID);
				SAFE_DEL(pQuestDone);
			}

			// ��ͻ��˷�����Ϣ
			tagNS_GMQuestState send;
			send.u16QuestID = u16QuestID;
			send.bDone = FALSE;
			SendMessage(&send, send.dwSize);
		}
	}

	//------------------------------------------------------------------------------------
	// �õ��������ĸ���
	//------------------------------------------------------------------------------------
	INT GetCurrentQuestCount() { return m_mapCurrentQuests.Size(); }

	//-------------------------------------------------------------------------------------
	// �õ�ָ��������ɵĴ���
	//-------------------------------------------------------------------------------------
	INT GetQuestCompleteTimes(UINT16 u16QuestID)
	{
		tagQuestDoneSave* pDoneQuest = m_mapCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneQuest) ) return GT_INVALID;

		return pDoneQuest->nTimes;
	}

	//-------------------------------------------------------------------------------------
	// �õ�ָ��������ɵĽ�ȡʱ��
	//-------------------------------------------------------------------------------------
	DWORD GetQuestCompleteAcceptTime(UINT16 u16QuestID)
	{
		tagQuestDoneSave* pDoneQuest = m_mapCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneQuest) ) return GT_INVALID;

		return pDoneQuest->dwStartTime;
	}

	//-------------------------------------------------------------------------------------
	// �õ�ָ������
	//-------------------------------------------------------------------------------------
	Quest* GetQuest(UINT16 u16QuestID) { return m_mapCurrentQuests.Peek(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// �õ�ָ�������Ƿ����
	//-------------------------------------------------------------------------------------
	BOOL IsHaveQuest(UINT16 u16QuestID) { return m_mapCurrentQuests.IsExist(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// ����Ƿ�����ĳ����
	//-------------------------------------------------------------------------------------
	BOOL IsHaveDoneQuest(UINT16 u16QuestID) { return m_mapCompleteQuests.IsExist(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// �õ����Ѹ���
	//-------------------------------------------------------------------------------------
	INT  GetFriendCount() { return m_mapFriend.Size(); }

	//-------------------------------------------------------------------------------------
	// �������ɿضԻ���ȱʡ�¼�
	//-------------------------------------------------------------------------------------
	VOID OnDlgDefaultEvent(EMsgUnitType	eDlgTarget, DWORD dwTargetID, EDlgOption eDlgOption);

private:
	Quest			m_Quests[QUEST_MAX_COUNT];		// ��ҵ�ǰ������
public:
	BOOL			QuestValid(INT nIndex){return m_Quests[nIndex].IsValid();}
private:
	QuestMap		m_mapCurrentQuests;				// ��ҵ�ǰ������
	QuestDoneMap	m_mapCompleteQuests;			// �Ѿ���ɵ����񼯺�
// �ƺ����
public:
	TitleMgr*		GetTitleMgr() { return m_pTitleMgr; }
private:
	TitleMgr*		m_pTitleMgr;
//��ɫ�������
public:
	tagRoleVCard&	GetVCard() { return m_VCard; }
private:
	tagRoleVCard	m_VCard;
//������Ϣ
public:
	ClanData& GetClanData(){	return m_ClanData;}
private:
	ClanData	m_ClanData;
// �����
public:
	PetPocket*	GetPetPocket() { return m_pPetPocket; }
private:
	PetPocket*	m_pPetPocket;
public:
	VOID OnLeaveMap(BOOL bChgMap = TRUE);

	void BreakMount();
	VOID OnEnterMap();
public:
	// ���纰����������ʱ��
	BOOL TalkToWorld();
private:
	INT m_nWorldTalkCounter;
	VOID UpdateTalkToWorld();

public:
	DWORD CanPickUpItem(INT64 n64GroundID, BOOL bRole = TRUE);
	// ʰȡ��Ʒ
	DWORD PickUpItem(INT64 n64GroundID, BOOL bRole = TRUE);
	// ������Ʒ
	DWORD PutDownItem(INT64 n64Serial);
public:
	// �������״̬
	VOID SetMount(BOOL bSet, INT nMountSpeed, const tagPetProto* pPetProto);

	// ��������ٶ�
	VOID ModMountSpeed(INT nMod);

	// �������
	VOID SetPassenger(Role* pHost);

	typedef MoveData::EMoveRet (MoveData::*PFMoveAction2P)(const Vector3& v1, const Vector3& v2);
	typedef MoveData::EMoveRet (MoveData::*PFMoveAction1P)(const Vector3& v);

	MoveData::EMoveRet	MoveAction(PFMoveAction2P pAction, Vector3& v1, Vector3& v2);
	MoveData::EMoveRet	MoveAction(PFMoveAction1P pAction, Vector3& v);

	//------------------------------------------------------------------------------
	// ��Ҽ���ｻ�����
	//------------------------------------------------------------------------------
	PetExchangeMgr& GetPetExchMgr() { return m_PetExchMgr; }

	BOOL	IsPetExchanging()					{ return IsInRoleState(ERS_PetExchange); }
	BOOL	CanPetExchange()
	{
		return !IsInRoleStateAny(ERS_Exchange | ERS_Shop | ERS_Stall | ERS_PetExchange) 
			&& (GetPetExchMgr().GetTgtRoleID() == GT_INVALID);
	}

	VOID	BeginPetExchange(DWORD dwTgtRoleID)
	{
		SetRoleState(ERS_PetExchange);
		GetPetExchMgr().CreateData();
		GetPetExchMgr().SetTgtRoleID(dwTgtRoleID);
	}

	VOID	EndPetExchange()
	{
		UnsetRoleState(ERS_PetExchange);
		GetPetExchMgr().DeleteData();
		GetPetExchMgr().SetTgtRoleID(GT_INVALID);
	}

	DWORD	ProcPetExchangeReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD	ProcPetExchangeReqRes(OUT Role* &pApplicant, DWORD dwTgtRoleID, DWORD dwErrorCode);
	DWORD ProcPetExchangeAdd(OUT Role* &pTarget, DWORD dwPetID);
	DWORD ProcPetExchangeDec(OUT Role* &pTarget, DWORD dwPetID);
	DWORD	ProcPetExchangeMoney(OUT Role* &pTarget, INT64 n64Silver);
	DWORD	ProcPetExchangeLock(OUT Role* &pTarget);
	DWORD	ProcPetExchangeCancel(OUT Role* &pTarget);
	DWORD	ProcPetExchangeVerify(OUT Role* &pTarget, OUT DWORD &dwFailedRoleID);

	DWORD VerifyPetExchangeData();
	DWORD	ProcPetExchange();

	DWORD	ProcPreparePetExchange(OUT Role* &pTarget);

private:
	PetExchangeMgr m_PetExchMgr;
	//	��ɫ��������������
public:

	//	��ȡ��ɫ�����������
	INT GetTreasureSum()		{ return m_nTreasureSum; }
	//	���ӱ������
	VOID IncTreasureSum();
	//	��ʼ���������
	VOID InitChestState();
	//	���ñ��俪����
	VOID SetTreasureSum(INT nSum);

	VOID StopMount();
	VOID ContinueMount();

public:
	// �������ɾ���ӳ�
	FLOAT	GetVNBExpRate();
	// �������ɵ��ʼӳ�
	FLOAT	GetVNBLootRate();
	
public:
	// GM������õļ�������
	DWORD AddSuit(DWORD dwSuitID, INT nQlty);
	DWORD AddEquip(INT nType, INT nLevel, INT nQlty);
public:
	//�����Ƿ���������Ϣ
	VOID	ResetHasLeftMsg()
	{
		m_bHasLeftMsg = FALSE;
	}
private:
	BOOL	m_bHasLeftMsg;

public:
	// �Ƿ���Բɼ�
	INT CanGather(Creature* pRes);

public:
	//�Ƿ���Բ��Ÿ��Զ���
	DWORD	CanCastMotion(Unit* pDest, DWORD dwActionID);
	// ���Ÿ��Զ���
	DWORD	CastMotion(Unit* pDest, DWORD dwActionID);

	// �ж�dwRoleID��û��Ȩ�����н���
	BOOL	GetMotionInviteStatus(DWORD dwRoleID);
	// ����Ƿ���Խ��ܻ���������
	VOID	SetMotionInviteStatus(BOOL bSet, DWORD dwRoleID);
	// ����״̬
	VOID	UpdateMotionInviteState();
private:
	INT		m_nMotionCounter;
	DWORD	m_dwPartnerID;
public:
	//���澭�鵽db
	VOID	SaveExp2DB();
	//�������Ե㵽db
	VOID SaveAttPoint2DB();
	//�������ʵ㵽db
	VOID	SaveTalentPoint2DB(INT nIndex);
	//����ȼ���db
	VOID	SaveLevel2DB();
	//�������ֵ�db
	VOID	SaveNewName2DB();

private:
	// Jason 2009-12-7 �����͸�
	VOID	UpdateBlessingOfLoong(VOID);

	// Jason 2009-12-7 �����͸�
	DWORD	m_timeLastLessing;			// �ϴ������͸�ʱ�䣬ע�⣬��ֵ��������ʱ�䵥λһ�£���λ��
	INT				m_nLessingLevel;				// �͸��������ROLE_MAX_LESSLEVEL_OF_LOONG��
	BOOL			m_bFiredOnceOfLessingLoong;

	BOOL IsValidOfLessingLoong(VOID);

public:
	VOID NotifyClientToReceiveLessingOfLoong(VOID); // ֪ͨ�������һ��ȡ�����͸�
	VOID NotifyClientIniLessingOfLoong(VOID);		// ֪ͨ��������͸��������ͻ�����ع��ܽ���

	DWORD ReceiveLessingOfLoong(VOID);				// ��ȡ�����͸�������

	// ����������ص����ۼƵ�������
	DWORD GetCurrentOnlineTime(VOID);
	DWORD ProcessGoldStoneMsg( INT64 GoldStoneItemID, INT64 destItemID);
};
