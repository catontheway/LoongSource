//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_att.h
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: ��������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/pet_define.h"
#include "../ServerDefine/pet_define.h"
#include "../WorldDefine/container_define.h"
#include "state_mgr.h"
#include "mutex.h"
struct	tagDBPetSoulCreate;
struct	tagPetProto;
struct	tagDBPetAtt;
class	PetSoul;

//-------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------
class PetAtt
{
	typedef	State<BYTE, EPetState>		PetState;
public:
	//---------------------------------------------------------------------
	// �����ʼ����洢
	//---------------------------------------------------------------------
	PetAtt(PetSoul* pSoul)
		:m_pSoul(pSoul){}
	static VOID InitCreateAtt(tagDBPetSoulCreate* pCreate, DWORD dwPetTypeID, DWORD dwOwnerID, LPCTSTR szName, INT nQuality);
	BOOL		InitAtt(const tagDBPetAtt* pAtt);
	BOOL		SaveToDB(tagDBPetAtt* pAtt);

	//---------------------------------------------------------------------
	// �������ԵĲ���
	//---------------------------------------------------------------------
	VOID		ModAttVal(INT nPetAtt, INT nMod, BOOL bSend = TRUE);
	VOID		SetAttVal(INT nPetAtt, INT nValue, BOOL bSend = TRUE);
	INT			GetAttVal(INT nPetAtt);
	VOID		ExpChange(INT nExpMod, BOOL bSend);	
	DWORD		UpStep(BOOL bSend = TRUE);
	VOID		SetLocked(BOOL bSet);
	BOOL		IsLocked()				const { return m_bLocked;	}
	
	//---------------------------------------------------------------------
	// һЩget
	//---------------------------------------------------------------------
	const tagPetProto* GetProto()		const {	return m_pProto;						}
	DWORD		GetID()					const { return m_dwPetID;						}
 	INT			GetProtoID()			const {	return m_dwProtoID;						}
	VOID		GetName(LPTSTR tszName)	const;
	INT			GetGrade()				const {	return m_nGrade;						}
	INT			GetStep()				const {	return m_nStep;							}
	INT			GetVLevel()				const {	INT nVLevel = 0;	TransStepGrade2VLevel(m_nStep, m_nGrade, nVLevel);	return nVLevel;	}
	INT			GetCurExp()				const {	return m_nExpCur;						}
	INT			GetExpLvlUp()			const {	return CalLvlUpExp(m_nStep, m_nGrade);	}
	INT64		CalPourMoney();
	BYTE		GetStateFlag()			const;
	
	//---------------------------------------------------------------------
	// ����״̬
	//---------------------------------------------------------------------
	BYTE		GetState()						const { return m_PetState.GetState();				}
	VOID		SetPetState(EPetState eState, BOOL bSync = TRUE);
	VOID		UnSetPetState(EPetState eState, BOOL bSync = TRUE);
	BOOL		IsPetInState(EPetState eState)	const {	return m_PetState.IsInState(eState);		}
	BOOL		IsPetInStateAny(BYTE byState)	const {	return m_PetState.IsInStateAny(byState);	}
	BOOL		IsPetInStateAll(BYTE byState)	const {	return m_PetState.IsInStateAll(byState);	}

private:
	//---------------------------------------------------------------------
	// ʵ��
	//---------------------------------------------------------------------
	INT			EPA2ECSPA(INT nEpa);
	static INT	CalAptitude(INT nQuality, const tagPetProto* pPetProto);
	static INT	CalSpiritMax(INT nAptitude, INT nStep, INT nGrade)		{	INT nVal = 500 + nAptitude*(nStep*10 + nGrade);	return min(nVal, 9999);}
	static INT	CalLvlUpExp(INT nStep, INT nGrade);
	static INT	CalTalentCountMax(INT nQuality, INT nStep, INT nGrade)	{	return (1 + nQuality) * 3 + nStep;				}
	static INT	CalPotential(INT nAptitude)								{	return nAptitude * 10;						}

	INT			GetAttDef(INT nPetAtt);
	VOID		OnAttChg(INT nPetAtt, INT nSrcVal, BOOL bSend = TRUE);

	VOID		SyncToClientAttChg( INT nCSPetAtt, INT nCurVal );
	VOID		SyncAllLvlUpChangeAtt();
private:
	//---------------------------------------------------------------------
	// ��Ա����
	//---------------------------------------------------------------------
	INT			m_nExpCur;			//��ǰ����	
	INT			m_nStep;			//��
	INT			m_nGrade;			//��
	PetState	m_PetState;			//״̬
	BOOL		m_bLocked;			//����

	TCHAR		m_tszName[X_SHORT_NAME];
	DWORD		m_dwPetID;
	DWORD		m_dwProtoID;

	INT			m_nAtt[EPA_NUM];

	const tagPetProto* m_pProto;
	PetSoul*	m_pSoul;

	//---------------------------------------------------------------------
	// ����id���
	//---------------------------------------------------------------------
// public:
// 	// ��ʼ�������̵߳���
// 	static VOID SetMaxPetID(DWORD dwMaxPetID)	{		m_dwMaxPetID = dwMaxPetID;	}
// 	// ����id�����̵߳���
// 	static DWORD CreatePetID()		
// 	{
// 		DWORD dwGenID = 0;
// 		m_MaxPetIDLock.Acquire();
// 		dwGenID = ++m_dwMaxPetID;
// 		m_MaxPetIDLock.Release();
// 
// 		return dwGenID;
// 	}
// 
// private:
// 	static DWORD m_dwMaxPetID;
// 	static Mutex m_MaxPetIDLock;
};
