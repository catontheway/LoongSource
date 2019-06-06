//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_att.cpp
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: ��������
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "pet_att.h"
#include "../WorldDefine/msg_pet.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/pet_define.h"
#include "role.h"
#include "world.h"
#include "att_res.h"
#include "pet_define.h"
#include "pet_soul.h"

#include "att_res.h"

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
// const INT MAX_QUALITY_SCALE	= 3;
// struct 
// {
// 	INT nMin[MAX_QUALITY_SCALE];
// 	INT nMax[MAX_QUALITY_SCALE];
// 	INT nProb[MAX_QUALITY_SCALE];
// } QualityScales[EIQ_End] = 
// {
// 	{		{1,	11,	21},	{10,	20,	30},	{45,	45,	10}	},
// 	{		{21,31,	41},	{30,	40,	50},	{45,	45,	10}	},
// 	{		{41,51,	61},	{50,	60,	70},	{45,	45,	10}	},
// 	{		{61,71,	81},	{70,	80,	90},	{45,	45,	10}	},
// 	{		{90,0,	0},		{100,	0,	0},		{100,	0,	0}	},
// };

INT PetAtt::CalAptitude( INT nQuality, const tagPetProto* pPetProto)
{
	if (nQuality <= EIQ_Null || nQuality >= EIQ_End)
		return 1;

	return IUTIL->RandomInRange(pPetProto->nAptitudeMin[nQuality], pPetProto->nAptitudeMax[nQuality]);
// 	for (INT i=0; i<MAX_QUALITY_SCALE * 10; ++i)
// 	{
// 		INT nIndex = i % MAX_QUALITY_SCALE;
// 		if (IUTIL->Probability(QualityScales[nQuality].nProb[nIndex]))
// 		{
// 			return IUTIL->RandomInRange(QualityScales[nQuality].nMin[nIndex], QualityScales[nQuality].nMax[nIndex]);
// 		}
// 	}
	
//	return QualityScales[nQuality].nMin[0];
}

//-------------------------------------------------------------------------------------------------------
// ����ٶ�
//-------------------------------------------------------------------------------------------------------
INT MountSpeedStepAdd[NUM_PET_STEP] =
{
	0,		// 0��0%
	400,	// 1��1%
	800,	// 2��2%
	1200,	// 3��4%
	1600,	// 4��6%
	2000,	// 5��9%
	2400,	// 6��12%
	2800,	// 7��16%
	3200,	// 8��20%
	3600	// 9��25%
};

//-------------------------------------------------------------------------------------------------------
// ������������
//-------------------------------------------------------------------------------------------------------
INT PetAtt::CalLvlUpExp(INT nStep, INT nGrade)
{
	INT nVLevel = 0;
	TransStepGrade2VLevel(nStep, nGrade, nVLevel);
	const tagPetLvlUpProto* pLvlUpProto = g_attRes.GetPetLvlUpProto(nVLevel);
	if (!P_VALID(pLvlUpProto))
	{
		return 0;
	}

	return pLvlUpProto->nExpLvlUpNeed;
}

//-------------------------------------------------------------------------
// ��ʼ�����ݿⴴ��������Ҫ������
//-------------------------------------------------------------------------
VOID PetAtt::InitCreateAtt( tagDBPetSoulCreate* pCreate, DWORD dwPetTypeID, DWORD dwOwnerID, LPCTSTR szName, INT nQuality )
{
	MTRANS_ELSE_RET(pProto, g_attRes.GetPetProto(dwPetTypeID), const tagPetProto, );

	pCreate->dwProtoID			= dwPetTypeID;
	pCreate->dwMasterID			= dwOwnerID;
	_tcsncpy(pCreate->tszName, szName, X_SHORT_NAME);

	pCreate->nQuality			= nQuality;
	pCreate->nAptitude			= CalAptitude(nQuality, pProto);
	pCreate->nSpirte			= CalSpiritMax(pCreate->nAptitude, 0, 1);
	pCreate->nPotential			= CalPotential(pCreate->nAptitude);//���ʡ�10
}

//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL PetAtt::InitAtt( const tagDBPetAtt* pAtt)
{
	m_pProto					= g_attRes.GetPetProto(pAtt->dwProtoID);
	if (!P_VALID(m_pProto) || !P_VALID(pAtt))
	{
		ASSERT(0);
		return FALSE;
	}

	//---------------------------------��ʼ�����ݿ�����-----------------------------
	_tcsncpy(m_tszName, pAtt->tszName, X_SHORT_NAME);
	m_PetState.InitState(pAtt->byPetState & 0x5);				//״̬
	m_dwProtoID					= pAtt->dwProtoID;
	m_nExpCur					= pAtt->nExpCur;		//��ǰ����	
	m_nStep						= pAtt->nStep;			//��
	m_nGrade					= pAtt->nGrade;			//��
	m_dwPetID					= pAtt->dwPetID;		//����id
	m_bLocked					= pAtt->bLocked;		//����

	m_nAtt[EPA_Quality]			= pAtt->nQuality;		//��ȡƷ��
	m_nAtt[EPA_Aptitude]		= pAtt->nAptitude;		//��ȡ����
	m_nAtt[EPA_Spirit]			= pAtt->nSpirit;		//��ȡ��ǰ����
	m_nAtt[EPA_Potential]		= pAtt->nPotential;		//��ȡ��ǰǱ��
	m_nAtt[EPA_TalentCount]		= pAtt->nTalentCount;	//��ȡ�츳����
	m_nAtt[EPA_WuXingEnergy]	= pAtt->nWuXingEnergy;	//��ȡ������

	//---------------------------------��ʼ��Ĭ��ֵ---------------------------------
	m_nAtt[EPA_SpiritMax]		= 0;					//�������ֵ
	m_nAtt[EPA_TalentCountMax]	= 0;					//��ȡ�츳����
	m_nAtt[EPA_MountNum]		= 0;					//�����Ŀ
	m_nAtt[EPA_MountNumMax]		= 0;					//�����Ŀ���ֵ

	m_nAtt[EPA_BagGrid]			= 0;					//���Ҹ���	Ĭ��4��		װ���ӳ�2~16
	m_nAtt[EPA_DeliveryConsume]	= 0;					//��ݺ�ʱ	Ĭ��0��		װ���ӳ�-��~0��ms����Ӧ���ܱ�����ʱ�䡱
	m_nAtt[EPA_SellConsume]		= 0;					//������ʱ	Ĭ��0��		װ���ӳ�-��~0��ms����Ӧ���ܱ�����ʱ�䡱
	m_nAtt[EPA_StorageConsume]	= 0;					//��ź�ʱ	Ĭ��0��		װ���ӳ�-��~0��ms����Ӧ���ܱ�����ʱ�䡱
	m_nAtt[EPA_GatherConsume]	= 0;					//�ɼ���ʱ	Ĭ��0��		װ���ӳ�-��~0��ms����Ӧ���ܱ�����ʱ�䡱
	m_nAtt[EPA_TrainResume]		= 0;					//ѵ���ָ�	Ĭ��0��		װ���ӳ�-��~0��ms����Ӧ���ܱ��ָ�ʱ�䡱
	m_nAtt[EPA_SpiritRate]		= 0;					//��������	Ĭ��100%��	װ���ӳ�-100%~0%����Ӧ���ܱ�energy_cost
	m_nAtt[EPA_ExpRate]			= 0;					//����ӳ�	Ĭ��100%��	װ���ӳ�0%~400%
	m_nAtt[EPA_PickUpResume]	= 0;					//ʰȡ�ָ�	Ĭ��0��		װ���ӳ�-20~-1
	m_nAtt[EPA_MedicineSaving]	= 0;					//��ҩ��ʡ	Ĭ��0%��	װ���ӳ�1%~5%
	m_nAtt[EPA_StrengthEffect]	= 0;					//ǿ��Ч��	Ĭ��100%��	װ���ӳ�0%~400%
	m_nAtt[EPA_WuXingConsume]	= 0;					//����������	Ĭ��100%��	װ���ӳ�-50%~0%����Ӧ���ܱ����������ġ�

	m_nAtt[EPA_MountSpeed]		= 0;					//����ٶ�	Ĭ��Ʒ�ʺͽ׼��㣬װ���ӳ�0%~100%
	
	return TRUE;
}

//-------------------------------------------------------------------------
// �޸ľ���
//-------------------------------------------------------------------------
VOID PetAtt::ExpChange( INT nExpMod, BOOL bSend )
{
	if (0 == nExpMod)
		return;
	nExpMod *= (GetAttVal(EPA_ExpRate) / 10000.0f);

	INT		nLvlUpExp = 0;
	INT		nVLevel = 0;
	BOOL	bLvlUp	= FALSE;

	// ���پ���
	if (nExpMod < 0)
	{
		m_nExpCur += nExpMod;
		m_nExpCur = m_nExpCur < 0 ? 0 : m_nExpCur;
	}
	// ���Ӿ���
	else if (nExpMod > 0)
	{
		nLvlUpExp = GetExpLvlUp();
		m_nExpCur += nExpMod;

		while (m_nExpCur >= nLvlUpExp && nLvlUpExp != 0)
		{
			// ���ﵽ��ǰ����� �� ��ע������
			if (nLvlUpExp <= m_nExpCur && MAX_PET_GRADE == m_nGrade)
			{
				m_nExpCur = nLvlUpExp;
				break;
			}

			m_nExpCur		-= nLvlUpExp;

			// �����µȼ�
			TransStepGrade2VLevel(m_nStep, m_nGrade, nVLevel);
			nVLevel		+= 1;
			TransVLevel2StepGrade(nVLevel, m_nStep, m_nGrade);

			m_pSoul->OnLevelUp();

			SyncAllLvlUpChangeAtt();

			nLvlUpExp	= GetExpLvlUp();
			bLvlUp		= TRUE;
		}
	}

	if (bSend)
	{
		// ͬ����ǰ����
		SyncToClientAttChg(ECSPA_nExpCurrent, m_nExpCur);
		if (bLvlUp)
		{
			// ͬ���������龭��
			SyncToClientAttChg(ECSPA_nExpLevelUp, nLvlUpExp);

			// ͬ������ȼ�
			SyncToClientAttChg(ECSPA_nLevel, nVLevel);
		}
	}
}

//-------------------------------------------------------------------------
// ��ȡ����ֵ
//-------------------------------------------------------------------------
INT PetAtt::GetAttVal( INT nPetAtt )
{
	return m_nAtt[nPetAtt] + GetAttDef(nPetAtt);
}

//-------------------------------------------------------------------------
// ��������ֵ
//-------------------------------------------------------------------------
VOID PetAtt::SetAttVal(INT nPetAtt, INT nValue, BOOL bSend /* = TRUE */)
{
	INT nSrcVal = m_nAtt[nPetAtt];
	m_nAtt[nPetAtt] = nValue;
	OnAttChg(nPetAtt, nSrcVal, bSend);
}

//-------------------------------------------------------------------------
// �ӳ����Ե�Ĭ��ֵ
//-------------------------------------------------------------------------
INT PetAtt::GetAttDef( INT nPetAtt )
{
	INT nDefVal = 0;
	switch(nPetAtt)
	{
	case EPA_TalentCountMax:
		nDefVal = CalTalentCountMax(m_nAtt[EPA_Quality], m_nStep, m_nGrade);
		break;
	case EPA_SpiritMax:
		nDefVal = CalSpiritMax(m_nAtt[EPA_Aptitude], m_nStep, m_nGrade);
		break;
	case EPA_BagGrid:
		nDefVal = 4;
		break;
	case EPA_SpiritRate:
	case EPA_ExpRate:
	case EPA_StrengthEffect:
	case EPA_WuXingConsume:
		nDefVal = 10000;
		break;
	case EPA_MountSpeed:
		if (GetProto()->nMountSpeed != 0)
			nDefVal = GetProto()->nMountSpeed + MountSpeedStepAdd[m_nStep];
		break;
	case EPA_MountNumMax:
		nDefVal = GetProto()->nMountable;
		break;
	default:
		nDefVal = 0;
		break;
	}

	return nDefVal;
}

//-------------------------------------------------------------------------
// ����ֵ�仯����
//-------------------------------------------------------------------------
VOID PetAtt::OnAttChg( INT nPetAtt, INT nSrcVal, BOOL bSend /*= TRUE*/ )
{
	// ����֪ͨ
	// ECSPA_nLevel
	// ECSPA_nExpCurrent
	// ECSPA_nExpLevelUp
	// ECSPA_PetState	

	switch(nPetAtt)
	{
	case EPA_Spirit:
		if (m_nAtt[EPA_Spirit] > GetAttVal(EPA_SpiritMax))
		{
			m_nAtt[EPA_Spirit] = GetAttVal(EPA_SpiritMax);
		}
		else if (m_nAtt[EPA_Spirit] < 0)
		{
			m_nAtt[EPA_Spirit] = 0;
		}
		break;
	case EPA_TalentCount:
		if (m_nAtt[EPA_TalentCount] > GetAttVal(EPA_TalentCountMax))
		{
			m_nAtt[EPA_TalentCount] = GetAttVal(EPA_TalentCountMax);
		}
		else if (m_nAtt[EPA_TalentCount] < 0)
		{
			m_nAtt[EPA_TalentCount] = 0;
		}
		break;
	case EPA_MountNum:
		if (m_nAtt[EPA_MountNum] > GetAttVal(EPA_MountNumMax))
		{
			m_nAtt[EPA_MountNum] = GetAttVal(EPA_MountNumMax);
		}
		else if (m_nAtt[EPA_MountNum] < 0)
		{
			m_nAtt[EPA_MountNum] = 0;
		}
		break;
	case EPA_Aptitude:
		//m_nAtt[EPA_Spirit]		= CalSpiritMax(m_nAtt[EPA_Aptitude], m_nStep, m_nGrade);
		//m_nAtt[EPA_Potential]	= CalPotential(m_nAtt[EPA_Aptitude]);
		if(m_nAtt[EPA_Aptitude] > GetProto()->nAptitudeMax[GetAttVal(EPA_Quality)])
		{
			m_nAtt[EPA_Aptitude] = GetProto()->nAptitudeMax[GetAttVal(EPA_Quality)];
		}
		else if (m_nAtt[EPA_Aptitude] < 0)
		{
			m_nAtt[EPA_Aptitude] = 0;
		}
		break;
	case EPA_MountSpeed:
		Role* pMaster = m_pSoul->GetMaster();
		if (P_VALID(pMaster) && m_pSoul->IsMounted())
		{
			pMaster->ModMountSpeed(m_nAtt[EPA_MountSpeed] - nSrcVal);
		}
		break;
	}

	if (bSend)
	{
		INT nSCPet = EPA2ECSPA(nPetAtt);	
		if (nSCPet != ECSPA_Null)
		{
			SyncToClientAttChg(nSCPet, GetAttVal(nPetAtt));
		}
	}
}

//-------------------------------------------------------------------------
// �޸�����ֵ
//-------------------------------------------------------------------------
VOID PetAtt::ModAttVal( INT nPetAtt, INT nMod, BOOL bSend /*= TRUE*/ )
{
	INT nSrcVal = m_nAtt[nPetAtt];
	m_nAtt[nPetAtt] += nMod;
	OnAttChg(nPetAtt, nSrcVal, bSend);
}

//-------------------------------------------------------------------------
// ��ȡ����
//-------------------------------------------------------------------------
VOID PetAtt::GetName( LPTSTR tszName ) const
{
	_tcsncpy(tszName, m_tszName, X_SHORT_NAME);

}

//-------------------------------------------------------------------------
// �����ݿ�
//-------------------------------------------------------------------------
BOOL PetAtt::SaveToDB( tagDBPetAtt* pAtt )
{
	if (!P_VALID(m_pSoul) || !P_VALID(m_pProto))
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD dwMasterID		= P_VALID(m_pSoul->GetMaster()) ? m_pSoul->GetMaster()->GetID() : GT_INVALID;

	GetName(pAtt->tszName);
	
	pAtt->dwPetID			= m_dwPetID;						// ����ID
	pAtt->dwProtoID			= m_pProto->dwTypeID;				// ԭ��ID
	pAtt->dwMasterID		= dwMasterID;						// ����ID
	pAtt->byPetState		= m_PetState.GetState();			// ����״̬	

	pAtt->nQuality			= m_nAtt[EPA_Quality];				// Ʒ��
	pAtt->nAptitude			= m_nAtt[EPA_Aptitude];				// ����
	pAtt->nPotential		= m_nAtt[EPA_Potential];			// ��ǰǱ��
	pAtt->nSpirit			= m_nAtt[EPA_Spirit];				// ��ǰ����
	pAtt->nWuXingEnergy		= m_nAtt[EPA_WuXingEnergy];			// ������
	pAtt->nTalentCount		= m_nAtt[EPA_TalentCount];			// �츳����
	

	pAtt->nExpCur			= m_nExpCur;						// ��ǰ����
	pAtt->nStep				= m_nStep;							// ��
	pAtt->nGrade			= m_nGrade;							// ��
	pAtt->bLocked			= m_bLocked;						// ����

	return TRUE;
}

//-------------------------------------------------------------------------
// ö��ӳ��
//-------------------------------------------------------------------------
INT PetAtt::EPA2ECSPA( INT nEpa )
{
	switch(nEpa)
	{
	case EPA_Quality:
		return ECSPA_nQuality;
	case EPA_Aptitude:
		return ECSPA_nAptitude;
	case EPA_SpiritMax:
		return ECSPA_nSpiritMax;
	case EPA_Spirit:
		return ECSPA_nSpirit;
	case EPA_Potential:
		return ECSPA_nPotential;
	case EPA_WuXingEnergy:
		return ECSPA_nWuXingEnergy;
	case EPA_MountSpeed:
		return ECSPA_nMountSpeed;
	case EPA_BagGrid:
		return ECSPA_BagGrid;
	case EPA_MountNum:
		return ECSPA_nMountNum;
	case EPA_MountNumMax:
		return ECSPA_nMountNumMax;
	default:
		return ECSPA_Null;
	}
}

//-------------------------------------------------------------------------
// ͬ�����ͻ���
//-------------------------------------------------------------------------
void PetAtt::SyncToClientAttChg( INT nCSPetAtt, INT nCurVal )
{
	if (!P_VALID(m_pSoul->GetMaster()))
		return;

	tagNS_PetAttrChange send;
	send.dwPetID		= m_dwPetID;
	send.u32NewValue	= nCurVal;
	send.u8AttrType		= nCSPetAtt;
	m_pSoul->GetMaster()->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------
// ���ó���״̬
//-------------------------------------------------------------------------
VOID PetAtt::SetPetState( EPetState eState, BOOL bSync )
{
	if (!m_PetState.IsInState(eState))
	{
		m_PetState.SetState(eState);
		if (bSync)
		{
			SyncToClientAttChg(ECSPA_PetState, m_PetState.GetState());
		}		
	}
}

//-------------------------------------------------------------------------
// ȡ����������
//-------------------------------------------------------------------------
VOID PetAtt::UnSetPetState( EPetState eState, BOOL bSync )
{
	if (m_PetState.IsInState(eState))
	{
		m_PetState.UnsetState(eState);
		if (bSync)
		{
			SyncToClientAttChg(ECSPA_PetState, m_PetState.GetState());
		}		
	}
}

INT64 PetAtt::CalPourMoney()
{
	INT nVLevel = 0;
	TransStepGrade2VLevel(m_nStep, m_nGrade, nVLevel);
	const tagPetLvlUpProto* pLvlUpProto = g_attRes.GetPetLvlUpProto(nVLevel);
	if (!P_VALID(pLvlUpProto))
	{
		return 0;
	}

	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(m_pSoul->GetMaster()->GetLevel());

	return INT64(pEffect->nExpLevelUp / 10000.0f * (pLvlUpProto->nMoneyRatePourExpNeed) / 5.0f);
}

BYTE PetAtt::GetStateFlag() const
{
	BYTE byState = 0;
	byState |= ( m_PetState.IsInState(EPS_Called)		? EPSF_Called :		EPSF_UnCalled);
	byState |= ( m_PetState.IsInState(EPS_Working)		? EPSF_Working :	EPSF_UnWorking);
	byState |= ( m_PetState.IsInState(EPS_Preparing)	? EPSF_Preparing :	EPSF_UnPreparing);
	byState |= ( m_PetState.IsInState(EPS_Mounting)		? EPSF_Mounting:	EPSF_UnMounting);

	return byState;
}

VOID PetAtt::SyncAllLvlUpChangeAtt()
{
	SyncToClientAttChg(EPA2ECSPA(EPA_TalentCountMax), GetAttVal(EPA_TalentCountMax));
	SyncToClientAttChg(EPA2ECSPA(EPA_MountSpeed), GetAttVal(EPA_MountSpeed));
	SyncToClientAttChg(EPA2ECSPA(EPA_SpiritMax), GetAttVal(EPA_SpiritMax));
}

DWORD PetAtt::UpStep( BOOL bSend /*= TRUE*/ )
{
	INT nLvlUpExp = GetExpLvlUp();
	INT nVLevel = 0;
	BOOL bLvlUp = FALSE;
	if (m_nExpCur >= nLvlUpExp && nLvlUpExp != 0 && GetGrade() == MAX_PET_GRADE)
	{
		TransStepGrade2VLevel(m_nStep, m_nGrade, nVLevel);
		nVLevel		+= 1;
		TransVLevel2StepGrade(nVLevel, m_nStep, m_nGrade);

		m_pSoul->OnLevelUp();
		SyncAllLvlUpChangeAtt();

		nLvlUpExp	= GetExpLvlUp();
		bLvlUp		= TRUE;
	}

	if (bLvlUp && bSend)
	{
		// ͬ����ǰ����
		SyncToClientAttChg(ECSPA_nExpCurrent, m_nExpCur);

		// ͬ���������龭��
		SyncToClientAttChg(ECSPA_nExpLevelUp, nLvlUpExp);

		// ͬ������ȼ�
		SyncToClientAttChg(ECSPA_nLevel, nVLevel);
	}

	return E_Pets_Success;
}

VOID PetAtt::SetLocked( BOOL bSet )
{
	if (bSet == m_bLocked)
	{
		return;
	}
	m_bLocked = bSet;

	SyncToClientAttChg(ECSPA_bLocked, m_bLocked);
}