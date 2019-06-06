#include "stdafx.h"

#include "unit.h"
#include "role.h"
#include "title_mgr.h"
#include "buff.h"
#include "buff_effect.h"

//----------------------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------------------
Buff::Buff()
: m_pOwner(NULL), m_nIndex(GT_INVALID), m_dwSrcUnitID(GT_INVALID), m_dwSrcSkillID(GT_INVALID), m_n64ItemID(GT_INVALID),
  m_dwItemTypeID(GT_INVALID), m_dwID(GT_INVALID), m_nLevel(0), m_nPersistTick(0), m_nCurTick(0),
  m_nWrapTimes(0), m_pProto(NULL), m_pMod(NULL), m_eState(EBS_Idle)
{

}

//----------------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------------
Buff::~Buff()
{
	SAFE_DEL(m_pMod);
}

//----------------------------------------------------------------------------------------------------
// ��ʼ������������Ϸ����ʱ���Buff
//----------------------------------------------------------------------------------------------------
VOID Buff::Init(const tagBuffProto* pBuffProto, Unit* pTarget, Unit* pSrc, DWORD dwSrcSkillID, const tagItem* pItem, INT nIndex, TList<DWORD>* listModifier)
{
	ASSERT( P_VALID(pBuffProto) );
	ASSERT( P_VALID(pTarget) );

	BeginInit();				// ����Ŀǰ���ڳ�ʼ��

	// ���þ�̬����
	m_pProto = pBuffProto;

	// ���Ҵ�����
	m_pTrigger = g_attRes.GetTriggerProto(pBuffProto->dwOPTrigger);

	// ������������
	m_pOwner = pTarget;
	m_nIndex = nIndex;

	m_dwSrcUnitID = (P_VALID(pSrc) ? pSrc->GetID() : GT_INVALID);
	m_dwSrcSkillID = dwSrcSkillID;

	if( P_VALID(pItem) )
	{
		m_n64ItemID = pItem->n64Serial;
		m_dwItemTypeID = pItem->dwTypeID;
	}
	else
	{
		m_n64ItemID = GT_INVALID;
		m_dwItemTypeID = GT_INVALID;
	}

	m_dwID = GetIDFromTypeID(pBuffProto->dwID);
	m_nLevel = GetLevelFromTypeID(pBuffProto->dwID);

	m_nPersistTick = 0;
	m_nCurTick = 0;
	m_nWrapTimes = 1;

	// ����BuffӰ��
	if( P_VALID(listModifier) && FALSE == listModifier->Empty() )
	{
		if( P_VALID(m_pMod) )	m_pMod->Clear();
		else					m_pMod = new tagBuffMod;

		// Buffʩ��������������Ӱ���buff�ļ���
		TList<DWORD>::TListIterator it = listModifier->Begin();
		DWORD dwSkillTypeID = GT_INVALID;

		while( listModifier->PeekNext(it, dwSkillTypeID) )
		{
			const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(dwSkillTypeID);
			if( !P_VALID(pSkillProto) ) continue;

			m_pMod->SetMod(pSkillProto);
		}
	}

	// ����Buff�ĳ�ʼ˲ʱЧ��
	if( !P_VALID(m_pTrigger) )
	{
		m_pOwner->CalBuffInstantEffect(pSrc, EBEM_Instant, m_pProto, m_pMod);
	}

	// ����Buf�ĳ�����Ч��
	m_pOwner->CalBuffPersistEffect(pSrc, m_pProto, m_pMod);

	EndInit();				// ��ʼ������
}

//----------------------------------------------------------------------------------------------------
// ��ʼ�����������������ʱ���buff
//----------------------------------------------------------------------------------------------------
VOID Buff::Init(Unit* pTarget, const tagBuffSave* pBuffSave, INT nIndex)
{
	if( !P_VALID(pTarget) || !P_VALID(pBuffSave) )
		return;

	BeginInit();

	m_pProto = g_attRes.GetBuffProto(Buff::GetTypeIDFromIDAndLevel(pBuffSave->dwBuffID, pBuffSave->n8Level));
	if( !P_VALID(m_pProto) )
	{
		IMSG(_T("Can not find the buff proto when loading the buff: id=%u, roleid=%u\r\n"), pBuffSave->dwBuffID, pTarget->GetID());
		m_dwID = GT_INVALID;
		EndInit();
		return;
	}

	m_pTrigger = g_attRes.GetTriggerProto(m_pProto->dwOPTrigger);

	m_dwID = pBuffSave->dwBuffID;
	m_nLevel = pBuffSave->n8Level;

	m_pOwner = pTarget;
	m_nIndex = nIndex;

	m_dwSrcUnitID = pBuffSave->dwSrcUnitID;
	m_dwSrcSkillID = pBuffSave->dwSrcSkillID;
	m_n64ItemID = pBuffSave->n64Serial;
	m_dwItemTypeID = pBuffSave->dwItemTypeID;

	m_nPersistTick = pBuffSave->nPersistTick;
	m_nCurTick = 0;

	m_nWrapTimes = pBuffSave->n8CurLapTimes;

	// ����BuffӰ��
	if( pBuffSave->n8ModifierNum > 0 )
	{
		if( P_VALID(m_pMod) )	m_pMod->Clear();
		else					m_pMod = new tagBuffMod;

		DWORD* pdwSkillTypeID = (DWORD*)pBuffSave->byData;

		for(INT n = 0; n < pBuffSave->n8ModifierNum; ++n)
		{
			const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(pdwSkillTypeID[n]);
			if( !P_VALID(pSkillProto) ) continue;

			m_pMod->SetMod(pSkillProto);
		}
	}

	// ����Buff�ĳ�����Ч�����������ô�õ�Buffʩ���ߵ�ָ�룿����
	m_pOwner->CalBuffPersistEffect(NULL, m_pProto, m_pMod, m_nWrapTimes);

	EndInit();
}

//----------------------------------------------------------------------------------------------------
// ��ʼ��tagBuffSave�ṹ
//----------------------------------------------------------------------------------------------------
VOID Buff::InitBuffSave(OUT tagBuffSave *pBuffSave, OUT INT32 &nSize)
{
	pBuffSave->dwBuffID				= m_dwID;
	pBuffSave->nPersistTick			= m_nPersistTick;;
	pBuffSave->n8Level				= m_nLevel;

	pBuffSave->n8CurLapTimes		= m_nWrapTimes;

	pBuffSave->dwSrcUnitID			= m_dwSrcUnitID;
	pBuffSave->dwSrcSkillID			= m_dwSrcSkillID;
	pBuffSave->n64Serial			= m_n64ItemID;
	pBuffSave->dwItemTypeID			= m_dwItemTypeID;

	// Buff�ļ���Ӱ��
	MTRANS_POINTER(pModifier, pBuffSave->byData, DWORD);
	INT n = 0;

	if( P_VALID(m_pMod) && FALSE == m_pMod->listModifier.Empty() )
	{	
		TList<DWORD>::TListIterator it = m_pMod->listModifier.Begin();
		DWORD dwSkillTypeID = GT_INVALID;
		while( m_pMod->listModifier.PeekNext(it, dwSkillTypeID) )
		{
			pModifier[n] = dwSkillTypeID;
			++n;
		}
	}
	pBuffSave->n8ModifierNum = n;

	nSize = (INT32)((size_t)&pModifier[n] - (size_t)pBuffSave);
}

//----------------------------------------------------------------------------------------------------
// ���£��������True�����ʾ��BuffҪɾ����
//----------------------------------------------------------------------------------------------------
BOOL Buff::Update()
{
	if( !IsValid() ) return FALSE;

	BeginUpdate();

	// ����Ǽ�����õ�buff������ʱ���Ƿ���
	if( IsInterOP() )
	{
		if( ++m_nCurTick >= m_pProto->nInterOPTick )
		{
			// ����������Ч��
			Unit* pSrc = NULL;
			if( P_VALID(m_pOwner->GetMap()) )
			{
				pSrc = m_pOwner->GetMap()->FindUnit(m_dwSrcUnitID);
			}
			m_pOwner->CalBuffInstantEffect(pSrc, EBEM_Inter, m_pProto, m_pMod, m_nWrapTimes);
			m_nCurTick = 0;
		}
	}

	// ���buff�ĳ���ʱ��
	if( !IsPermanent() )
	{
		if( ++m_nPersistTick >= GetMaxPersistTick() )
		{
			// ����ʱ�䵽��
			EndUpdate();
			return TRUE;
		}
        //���ڷ������˼����ӳ� ���� ÿ407Tick �� 5 Tick
        

		if( (m_nPersistTick + 407)%407 == 0 )
		{
			m_nPersistTick+= 5;
			if( GetMaxPersistTick() >= 6000.0f)
			{
				m_nPersistTick++;
			}
		}

		

	}

	EndUpdate();

	return FALSE;
}

//----------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------
VOID Buff::Destroy(BOOL bSelf)
{
	BeginDestroy();

	Unit* pSrc = NULL;
	if( P_VALID(m_pOwner->GetMap()) )
	{
		pSrc = m_pOwner->GetMap()->FindUnit(m_dwSrcUnitID);
	}
	// ����Buff�Ľ���ʱЧ����ֻ����ʱ�䵽���������ʱ�ż���buff����ʱЧ��
	if( bSelf )
	{
		m_pOwner->CalBuffInstantEffect(pSrc, EBEM_Finish, m_pProto, m_pMod, m_nWrapTimes);
	}

	// ȡ��Buff�ĳ�����Ч��
	m_pOwner->CalBuffPersistEffect(pSrc, m_pProto, m_pMod, m_nWrapTimes, FALSE);

	// ���Buff
	m_dwID = GT_INVALID;
	m_pOwner = NULL;
	m_pProto = NULL;

	// ���Mod��ֵ�����ﲻ�ͷŵ������ǽ�������գ��Ա��´���ʹ�ø�Buffʱ�����ٷ����ڴ�
	if( P_VALID(m_pMod) )
	{
		m_pMod->Clear();
	}

	EndDestroy();
}

//----------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------
VOID Buff::Wrap()
{
	BeginUpdate();

	Unit* pSrc = NULL;
	if( P_VALID(m_pOwner->GetMap()) )
	{
		pSrc = m_pOwner->GetMap()->FindUnit(m_dwSrcUnitID);
	}

	++m_nWrapTimes;
	m_nPersistTick = 0;

	// ��������һ��Buff��һ��˲��Ч��
	m_pOwner->CalBuffInstantEffect(pSrc, EBEM_Instant, m_pProto, m_pMod);

	// �鿴�Ƿ���Ҫ���ۼ�һ�γ�����Ч��
	if( m_nWrapTimes > GetMaxWrapTimes() )
	{
		m_nWrapTimes = GetMaxWrapTimes();
	}
	else
	{
		// ��û�г��������Ӵ��������ټ���һ��Buff����Ч��
		m_pOwner->WrapBuffPersistEffect(pSrc, m_pProto, m_pMod);
	}

	EndUpdate();
}

//------------------------------------------------------------------------------------------------------
// ���
//------------------------------------------------------------------------------------------------------
BOOL Buff::Interrupt(EBuffInterruptFlag eFlag, INT nMisc)
{
	if( !IsValid() ) return FALSE;

	// ���buff������flag�еĴ�Ϸ�ʽ������ʧ��
	if( !(eFlag & m_pProto->dwInterruptFlag) ) return FALSE;

	// ����ĳЩ������жϣ�Ҫ�����ж�
	switch(eFlag)
	{
		// ��������
	case EBIF_HPLower:
		{
			INT nHPLimit = m_pProto->nHPInterruptLimit;
			if( nHPLimit < 100000 )
			{
				return m_pOwner->GetAttValue(ERA_HP) < nHPLimit;
			}
			else if( m_pOwner->GetAttValue(ERA_MaxHP) > 0 )
			{
				return m_pOwner->GetAttValue(ERA_HP) * 10000 / m_pOwner->GetAttValue(ERA_MaxHP) < (nHPLimit - 100000);
			}
			else
			{
				return FALSE;
			}
		}
		break;

		// ��������
	case EBIF_MPLower:
		{
			INT nMPLimit = m_pProto->nMPInterruptLimit;
			if( nMPLimit < 100000 )
			{
				return m_pOwner->GetAttValue(ERA_MP) < nMPLimit;
			}
			else if( m_pOwner->GetAttValue(ERA_MaxMP) > 0 )
			{
				return m_pOwner->GetAttValue(ERA_MP) * 10000 / m_pOwner->GetAttValue(ERA_MaxMP) < (nMPLimit - 100000);
			}
			else
			{
				return FALSE;
			}
		}
		break;

		// ŭ������
	case EBIF_RageLower:
		{
			INT nRageLimit = m_pProto->nRageInterruptLimit;
			return m_pOwner->GetAttValue(ERA_Rage) < nRageLimit;
		}
		break;

		// �־�������
	case EBIF_EnduranceLower:
		{
			INT nEnduranceLimit = m_pProto->nEnduranceInterruptLimit;
			if( nEnduranceLimit < 100000 )
			{
				return m_pOwner->GetAttValue(ERA_Endurance) < nEnduranceLimit;
			}
			else if( m_pOwner->GetAttValue(ERA_MaxEndurance) > 0 )
			{
				return m_pOwner->GetAttValue(ERA_Endurance) * 10000 / m_pOwner->GetAttValue(ERA_MaxEndurance) < (nEnduranceLimit - 100000);
			}
			else
			{
				return FALSE;
			}
		}
		break;

		// ��������
	case EBIF_VitalityLower:
		{
			INT nVitalityLimit = m_pProto->nVitalityInterruptLimit;

			if( nVitalityLimit < 100000 )
			{
				return m_pOwner->GetAttValue(ERA_Vitality) < nVitalityLimit;
			}
			else if( m_pOwner->GetAttValue(ERA_MaxVitality) > 0 )
			{
				return m_pOwner->GetAttValue(ERA_Vitality) * 10000 / m_pOwner->GetAttValue(ERA_MaxVitality) < (nVitalityLimit - 100000);
			}
			else
			{
				return FALSE;
			}
		}
		break;

		// ������
	case EBIF_BeAttacked:
		{
			INT nProp = m_pProto->nAttackInterruptRate + ( P_VALID(m_pMod) ? m_pMod->nAttackInterruptRateMod : 0 );

			if( IUTIL->Rand() % 10000 > nProp ) return FALSE;
		}
		break;

	default:
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------------------------------------------
BOOL Buff::OnTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1/* =GT_INVALID */, DWORD dwEventMisc2/* =GT_INVALID */)
{
	if( !IsValid() ) return FALSE;

	if( m_eState != EBS_Idle ) return FALSE;

	// ��ⴥ������������
	if( !P_VALID(m_pTrigger) || m_pTrigger->eEventType != eEvent ) return FALSE;

	// ����Trigger�Ƿ�����
	if( !m_pOwner->TestTrigger(pTarget, m_pTrigger, dwEventMisc1, dwEventMisc2) ) return FALSE;

	// �ƺ��¼�����
	Role *pRole = dynamic_cast<Role *>(pTarget);
	if (NULL != pRole)
	{
		pRole->GetTitleMgr()->SigEvent(ETE_TRIGGER, m_pTrigger->dwID, GT_INVALID);
	}

	// �������ڸ���
	BeginUpdate();

	// ����Buff˲ʱЧ��
	Unit* pSrc = NULL;
	if( P_VALID(m_pOwner->GetMap()) )
	{
		pSrc = m_pOwner->GetMap()->FindUnit(m_dwSrcUnitID);
	}
	m_pOwner->CalBuffInstantEffect(pSrc, EBEM_Instant, m_pProto, m_pMod, m_nWrapTimes, pTarget);

	// ����ֹͣ����
	EndUpdate();

	return TRUE;
}

