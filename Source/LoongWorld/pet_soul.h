//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_soul.h
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: �������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "pet_att.h"
#include "pet_equip.h"

class Pet;
class Role;
class PetTracker;
class PetSkill;

struct tagItem;
struct tagDBPet;
struct tagPetSkillCmdParam;

//----------------------------------------------------------------------------------------------------
// �������
// ˵����
//		����id		dwPetSkillID;
//		����lvl		dwPetSkillLvl;
//		����typeid	dwPetSkillTypeID;
//		dwPetSkillTypeID = dwPetSkillID * 100 + dwPetSkillLvl;
//----------------------------------------------------------------------------------------------------
class PetSoul
{
	friend class PetAtt;
	typedef TMap<DWORD, PetSkill*>		PetSkillMap;	

public:
	//----------------------------------------------------------------------------------------------------
	// ������ɾ��
	//----------------------------------------------------------------------------------------------------
	static VOID		CreateDBSoul( DWORD dwTypeID,LPCTSTR tszName,Role* pMaster, INT nQualiry, INT64 n64EggSerial );
	static PetSoul* CreateSoulByDBData(const BYTE* &pData, BOOL bCreate);
	static VOID		DeleteSoul(PetSoul* pSoul, BOOL bFromDB = FALSE);

	//----------------------------------------------------------------------------------------------------
	// Log
	//----------------------------------------------------------------------------------------------------
	static VOID		LogPet(DWORD dwMasterID, DWORD dwPetID, DWORD dwCmdID);

	//----------------------------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------------------------
	VOID			Update();

	//----------------------------------------------------------------------------------------------------
	// �������ͼ
	//----------------------------------------------------------------------------------------------------
	DWORD			BodyEnterMap();

	BOOL			IsPetInMap();
	DWORD			BodyLeaveMap();

	//----------------------------------------------------------------------------------------------------
	// ��ȡ
	//----------------------------------------------------------------------------------------------------
	Role*			GetMaster()		const	{return m_pMaster;	}
	Pet*			GetBody()		const	{return m_pBody;	}
	VOID			SetBody(Pet* pBody)		{m_pBody = pBody;	}
	DWORD			GetID()			const;
	DWORD			GetProtoID()	const;
	PetAtt&			GetPetAtt()				{ return m_PetAtt;	}
	const tagPetProto* GetProto();

	//----------------------------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------------------------
	BOOL			IsLocked()		const	{ return m_PetAtt.IsLocked();	}
	VOID			SetLocked(BOOL bSet);

	VOID			SendSaveAtt2DB();
	VOID			FillClientPetAtt(tagPetInitAttr* pInitAtt);
	BOOL			SaveToDB(IN LPVOID pData, OUT LPVOID &pOutPointer);

	//----------------------------------------------------------------------------------------------------
	// ���������
	//----------------------------------------------------------------------------------------------------
	BOOL			IntegrateInPet(Pet* pPet);
	VOID			DetachFromPet();
	BOOL			IntegrateInRole(Role* pRole);
	VOID			DetachFromRole();

	//----------------------------------------------------------------------------------------------------
	//	����״̬���
	//----------------------------------------------------------------------------------------------------
	BOOL			IsInState(EPetState eState);

	//----------------------------------------------------------------------------------------------------
	// ��Ԧ���
	//----------------------------------------------------------------------------------------------------
	BOOL			IsMounted();
	BOOL			CanSetMount(BOOL bSet);
	BOOL			SetMount(BOOL bSet);

	//----------------------------------------------------------------------------------------------------
	// �ٻ����
	//----------------------------------------------------------------------------------------------------
	BOOL			IsCalled();
	BOOL			CanSetCalled(BOOL bSet);
	BOOL			SetCalled(BOOL bSet, BOOL bSync = TRUE);

	//----------------------------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------------------------
	BOOL			IsWorking();
	BOOL			CanSetWroking( BOOL bSet, DWORD dwPetSkillTypeID );
	BOOL			SetWorking(BOOL bSet, DWORD dwPetSkillTypeID);
	BOOL			GetWorkingSkillTypeID()const {return m_dwWorkingSkillTypeID;}

	//----------------------------------------------------------------------------------------------------
	// Ԥ����Ԧ���
	//----------------------------------------------------------------------------------------------------
	BOOL			IsPreparing();
	BOOL			CanSetPreparing( BOOL bSet);
	BOOL			SetPreparing(BOOL bSet);
	
	//----------------------------------------------------------------------------------------------------
	// ����������
	//----------------------------------------------------------------------------------------------------
	BOOL			CanAddPassenger(Role* pPassenger);
	BOOL			CanRemovePassenger(Role* pPassenger);
	DWORD			AddPassenger(Role* pPassenger);
	DWORD			RemovePassenger(Role* pPassenger);
	Role*			GetPassenger()	{	return m_pPassenger;	}

	//----------------------------------------------------------------------------------------------------
	//	���＼�����
	//----------------------------------------------------------------------------------------------------
	DWORD			LearnBookSkill(INT64 n64ItemID);
	DWORD			LearnNormalSkill(INT nLevel);

	DWORD			LearnSkill( DWORD dwPetSkillTypeID );
	DWORD			ForgetSkill(DWORD dwPetSkillID);
	
	DWORD			AddSkill(PetSkill* pSkill);
	DWORD			AddSkillOnLoad(PetSkill* pSkill);

	DWORD			RemoveSkill(PetSkill* pSkill);
	PetSkill*		GetSkill(DWORD dwPetSkillID);
	INT				ExportSpecSkill(INT nPetAtt, std::list<PetSkill*> &listModSkill);
	INT				GetModSkillType(INT nPetAtt);
	DWORD			HandleSkillCmd( DWORD dwPetSkillTypeID, PVOID pSkillData, DWORD dwSkillDataSize );
	DWORD			StopSkillWorking(DWORD dwPetSkillID);
	VOID			UpdateAllSkill();

	//----------------------------------------------------------------------------------------------------
	//	����װ�����
	//----------------------------------------------------------------------------------------------------
	DWORD			Equip(INT64 n64ItemID, INT8 n8DstPos, BOOL bSend = FALSE);
	DWORD			UnEquip(INT64 n64ItemID, INT16 n16DstPos, BOOL bSend = FALSE);
	DWORD			EquipSwapPos(INT64 n64ItemID, INT8 n8DstPos, BOOL bSend = FALSE);
	VOID			GetEquipInfo(INT64 n64ItemID, tagPetEquipMsgInfo* pMsgInfo);
	BOOL			HasEquip();

	//----------------------------------------------------------------------------------------------------
	// ����������
	//----------------------------------------------------------------------------------------------------
	DWORD			PourExp(INT64 n64ItemID);
	VOID			OnLevelUp();
	DWORD			Enhance(INT nAptitudeAdd);
	DWORD			UpStep(DWORD dwPetSkillTypeID);

	//----------------------------------------------------------------------------------------------------
	// ιʳ
	//----------------------------------------------------------------------------------------------------
	DWORD Feed(INT nSpritMod);

private:
	//----------------------------------------------------------------------------------------------------
	// ʵ��
	//----------------------------------------------------------------------------------------------------
	BOOL			SetMaster(Role* pRole)	{	m_pMaster = pRole;	return TRUE;}

	//----------------------------------------------------------------------------------------------------
	// ����������
	//----------------------------------------------------------------------------------------------------
	PetSoul();
	virtual	~PetSoul();

	//----------------------------------------------------------------------------------------------------
	// ���ֳ�ʼ��
	//----------------------------------------------------------------------------------------------------
	BOOL			Init(const tagDBPet* &pSoulData, BOOL bCreate);
	VOID			InitPetEquip(IN PVOID pData, OUT PVOID& pDataOut, IN INT nNum);
	VOID			InitPetSkill(IN PVOID pData, OUT PVOID& pDataOut, IN INT nNum);

private:
	Role*			m_pMaster;				// ����
	Role*			m_pPassenger;			// ����
	Pet*			m_pBody;				// ����

	PetAtt			m_PetAtt;				// ��������
	PetSkillMap		m_mapPetSkill;			// ���＼��
	DWORD			m_dwWorkingSkillTypeID;	// ��ǰ��������
	PetEquipBar		m_EquipBar;				// ����װ����

	INT				nCount;					// �ظ���������
};


