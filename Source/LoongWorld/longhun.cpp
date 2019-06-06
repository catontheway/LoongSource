//------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: longhun.cpp
// author: Sxg
// actor:
// data: 2009-3-17
// last:
// brief: ��������������ƴ�����
//------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/ItemDefine.h"
#include "../ServerDefine/base_define.h"

#include "longhun.h"
#include "att_res.h"
#include "role.h"
#include "map.h"
//------------------------------------------------------------------------
// ����&����
//------------------------------------------------------------------------
LongHun::LongHun(Role *pRole)
{
	m_pRole			= pRole;
	m_n8NumPassive	= 0;
	m_n8NumActive	= 0;
	m_listActive.Clear();

	for(INT i=0; i<EEP_MaxEquip; ++i)
	{
		m_dwPassive[i]	= GT_INVALID;
		m_dwActive[i]	= GT_INVALID;
	}
}

LongHun::~LongHun()
{
}

//------------------------------------------------------------------------
// ����&ɾ��
//------------------------------------------------------------------------
VOID LongHun::Add(const tagEquip *pEquip, const INT16 n16EquipPos, BOOL bSend2Client)
{
	if(!P_VALID(pEquip))
	{
		return;
	}
	
	// ʱװû����������
	if(MIsFashion(pEquip))
	{
		return;
	}

	// ע�⣺�ô�������ʱװ�жϵ���˳��
	ASSERT(n16EquipPos >= EEP_Equip_Start && n16EquipPos <= EEP_Equip_End);

	// ���øü�װ���ϵ���������
	Set(pEquip, n16EquipPos, bSend2Client);

	// �ü�װ��Ӱ���װ��
	for(INT i=0; i<X_LONGHUN_EFFECT_POS_NUM; ++i)
	{
		INT16 n16Pos = g_sLongHunCanActivePos[n16EquipPos].eEquipPos[i];
		if(n16Pos < EEP_Equip_Start || n16Pos > EEP_Equip_End)	// (EEP_NULL == n16Pos)
		{
			continue;
		}

		// ����Ƿ��Ѿ�����
		if(GetLongHunID(n16Pos) != GT_INVALID)
		{
			continue;
		}

		// ��ȡװ�����϶�Ӧ��λ��װ��
		tagEquip *pEquip1 = m_pRole->GetItemMgr().GetEquipBarEquip(n16Pos);
		if(!P_VALID(pEquip1))
		{
			continue;
		}

		// ����װ���ϵ���������
		Set(pEquip1, n16Pos, bSend2Client);

		// ������Ч
		if(bSend2Client && MIsWeapon(pEquip1))
		{
			SendEquipEffect2Client(n16Pos);
		}
	}
}

VOID LongHun::Remove(const INT16 n16EquipPos)
{
	// ���Ƿ�ʱװװ��
	if(n16EquipPos < EEP_Equip_Start || n16EquipPos > EEP_Equip_End)
	{
		return;
	}

	// ��ǰλ���Ƿ��������걻����
	if(GetLongHunID(n16EquipPos) != GT_INVALID)
	{
		Unset(n16EquipPos, TRUE);
	}

	// �ü�װ��Ӱ���װ��
	for(INT i=0; i<X_LONGHUN_EFFECT_POS_NUM; ++i)
	{
		INT16 n16Pos = g_sLongHunCanActivePos[n16EquipPos].eEquipPos[i];
		if(n16Pos < EEP_Equip_Start || n16Pos > EEP_Equip_End)	// (EEP_NULL == n16Pos)
		{
			continue;
		}

		// ����Ƿ��Ѿ�����
		const DWORD dwLongHunID = GetLongHunID(n16Pos);
		if(GT_INVALID == dwLongHunID)
		{
			continue;
		}

		// ȡ������
		Unset(n16Pos, FALSE);

		// ��ȡװ�����϶�Ӧ��λ��װ��
		tagEquip *pEquip1 = m_pRole->GetItemMgr().GetEquipBarEquip(n16Pos);
		if(!P_VALID(pEquip1))
		{
			// ��ִ�е��ˣ�˵���������������߼���©��
			ASSERT(P_VALID(pEquip1));
			continue;
		}

		// ����װ���ϵ���������
		Set(pEquip1, n16Pos, FALSE);

		// �ж��Ƿ���Ҫ���ͻ��˷���Ϣ
		if(GetLongHunID(n16Pos) == GT_INVALID)
		{
			SendRemove2Client(n16Pos);
			
			if(MIsWeaponByEquipPos(n16Pos))
			{
				SendEquipEffect2Client(n16Pos);
			}
		}
	}
}

//------------------------------------------------------------------------
// ��ʼ�����͸��ͻ��˵�����
//------------------------------------------------------------------------
VOID LongHun::InitSendInitState(OUT INT16 n16Data[], INT8 n8Sz) const
{
	ASSERT(GetActiveNum() + GetPassiveNum() == n8Sz);

	if(0 == n8Sz)
	{
		return;
	}
	
	INT nIndex = 0;
	for(INT i=0; i<EEP_MaxEquip; ++i)
	{
		if(GetLongHunID(i) != GT_INVALID)
		{
			n16Data[nIndex++] = i;
			if(nIndex >= n8Sz)
			{
				return;
			}
		}
	}
}

//------------------------------------------------------------------------
// ������������ -- ����������
//------------------------------------------------------------------------
VOID LongHun::CalLongfuAtt(OUT BYTE byLongfuAtt[], const INT nSz, INT16 n16EquipPos)
{
	const tagEquip *pEquip = m_pRole->GetItemMgr().GetEquipBarEquip(n16EquipPos);
	if(P_VALID(pEquip))
	{
		for(INT i=0; i<nSz; ++i)
		{
			byLongfuAtt[i] += pEquip->GetLongfuAtt(i);
		}
	}
}

//------------------------------------------------------------------------
// �жϸü�װ���ǿɱ��񼤻�
//------------------------------------------------------------------------
BOOL LongHun::IsNeedCount(const INT16 n16EquipPos, 
						  const tagLongHunProto *pLongHunProto,
						  const BOOL bOuterLongHun)
{
	ASSERT(P_VALID(pLongHunProto));

	BYTE byLongfuAtt[X_WUXING_TYPE_NUM];
	ZeroMemory(byLongfuAtt, sizeof(byLongfuAtt));
	
	if(bOuterLongHun)
	{
		CalLongfuAtt(byLongfuAtt, X_WUXING_TYPE_NUM, g_sLongHunActivePos[n16EquipPos].eOuter);
	}
	else
	{
		CalLongfuAtt(byLongfuAtt, X_WUXING_TYPE_NUM, g_sLongHunActivePos[n16EquipPos].eInner[0]);
		CalLongfuAtt(byLongfuAtt, X_WUXING_TYPE_NUM, g_sLongHunActivePos[n16EquipPos].eInner[1]);
	}

	for(INT i=0; i<X_WUXING_TYPE_NUM; ++i)
	{
		if(byLongfuAtt[i] < pLongHunProto->byWuXingActive[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}

//------------------------------------------------------------------------
// ������������
//------------------------------------------------------------------------
VOID LongHun::Set(const tagEquip *pEquip, const INT16 n16EquipPos, BOOL bSend2Client)
{
	// ���Ѽ���򲻿��ٱ�����
	if(GetLongHunID(n16EquipPos) != GT_INVALID)
	{
		return;
	}

	// ��ȡװ���ϵ���������������
	DWORD	dwLongHunID = GT_INVALID;
	BOOL	bOuterLongHun = TRUE;

	pEquip->GetLongHunID(dwLongHunID, bOuterLongHun);
	
	// ָ��װ����û����������
	if(GT_INVALID == dwLongHunID)
	{
		return;
	}

	// ��ȡ����������̬����
	const tagLongHunProto *pLongHunProto = g_attRes.GetLongHunProto(dwLongHunID);
	if(!P_VALID(pLongHunProto))
	{
		MAttResCaution(_T("longhun_proto.xml"), _T("dwID"), dwLongHunID);
		ASSERT(0);
		return;
	}

	// �ж��Ƿ����㼤������
	if(!IsNeedCount(n16EquipPos, pLongHunProto, bOuterLongHun))
	{
		return;
	}

	// ��ȡ����������trigger������
	const tagTriggerProto *pTriggerProto = g_attRes.GetTriggerProto(pLongHunProto->dwTriggerID);
	if(!P_VALID(pTriggerProto))
	{
		MAttResCaution(_T("trigger_proto.xml"), _T("dwTriggerID"), pLongHunProto->dwTriggerID);
		ASSERT(0);
		return;
	}

	// ����trigger����������������
	switch(pTriggerProto->eEventType)
	{
	case ETEE_Use:			// ʹ��
		m_dwActive[n16EquipPos] = dwLongHunID;
		m_listActive.PushBack(dwLongHunID);
		++m_n8NumActive;
		break;
	default:
		m_dwPassive[n16EquipPos] = dwLongHunID;
		++m_n8NumPassive;

		// trigger,buffע���
		m_pRole->RegisterTriggerLongHunSet(pLongHunProto->dwTriggerID, pLongHunProto->dwBuffID1, n16EquipPos);
		m_pRole->RegisterTriggerLongHunSet(pLongHunProto->dwTriggerID, pLongHunProto->dwBuffID2, n16EquipPos);
		break;
	}

	// ��������������Ч -- ��������
	if(MIsWeapon(pEquip))
	{
		INT nDisplayPos = m_pRole->GetEquipDisplayPos(n16EquipPos);
		ASSERT(nDisplayPos != GT_INVALID);
		
		if(bOuterLongHun)
		{
			m_pRole->SetEquipEffect(nDisplayPos, EEE_Simple);
		}
		else
		{
			m_pRole->SetEquipEffect(nDisplayPos, EEE_Complex);
		}
	}

	// ��ͻ��˷�����Ϣ
	if(bSend2Client)
	{
		SendAdd2Client(n16EquipPos);
	}
}

//------------------------------------------------------------------------
// ȡ����Ӧλ����������������
//------------------------------------------------------------------------
VOID LongHun::Unset(const INT16 n16EquipPos, BOOL bSend2Client)
{
	DWORD dwLongHunID = GetLongHunID(n16EquipPos);

	ASSERT(dwLongHunID != GT_INVALID);

	// ��ȡ����������̬����
	const tagLongHunProto *pLongHunProto = g_attRes.GetLongHunProto(dwLongHunID);
	if(!P_VALID(pLongHunProto))
	{
		MAttResCaution(_T("longhun_proto.xml"), _T("dwID"), dwLongHunID);
		ASSERT(0);
		return;
	}

	// ��ȡ����������trigger������
	const tagTriggerProto *pTriggerProto = g_attRes.GetTriggerProto(pLongHunProto->dwTriggerID);
	if(!P_VALID(pTriggerProto))
	{
		MAttResCaution(_T("trigger_proto.xml"), _T("dwTriggerID"), pLongHunProto->dwTriggerID);
		ASSERT(0);
		return;
	}

	// ����trigger���ͷ�������������
	switch(pTriggerProto->eEventType)
	{
	case ETEE_Use:			// ʹ��
		m_dwActive[n16EquipPos] = GT_INVALID;
		m_listActive.Erase(dwLongHunID);
		--m_n8NumActive;
		break;
	default:
		m_dwPassive[n16EquipPos] = GT_INVALID;
		--m_n8NumPassive;

		// trigger,buff��ע���
		m_pRole->UnRegisterTriggerLongHunSet(pLongHunProto->dwTriggerID, pLongHunProto->dwBuffID1, n16EquipPos);
		m_pRole->UnRegisterTriggerLongHunSet(pLongHunProto->dwTriggerID, pLongHunProto->dwBuffID2, n16EquipPos);
		break;
	}

	// ȡ����������������Ч -- ��������
	if(MIsWeaponByEquipPos(n16EquipPos))
	{
		m_pRole->SetEquipEffect(m_pRole->GetEquipDisplayPos(n16EquipPos), EEE_Null);
	}

	if(bSend2Client)
	{
		SendRemove2Client(n16EquipPos);
	}
}

//------------------------------------------------------------------------
// ��������������״̬�仯���͸��ͻ���
//------------------------------------------------------------------------
VOID LongHun::SendAdd2Client(const INT16 n16EquipPos)
{
	tagNS_LongHunOn send;
	send.n16EquipPos = n16EquipPos;
	m_pRole->SendMessage(&send, send.dwSize);
}

VOID LongHun::SendRemove2Client(const INT16 n16EquipPos)
{
	tagNS_LongHunOff send;
	send.n16EquipPos = n16EquipPos;
	m_pRole->SendMessage(&send, send.dwSize);
}

VOID LongHun::SendEquipEffect2Client(const INT16 n16EquipPos)
{
	Map *pMap = m_pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return;
	}

	INT nDisplayPos = m_pRole->GetEquipDisplayPos(n16EquipPos);
	ASSERT(EAE_RWeapon == nDisplayPos || EAE_LWeapon == nDisplayPos);

	tagNS_EquipEffectChange send;
	send.dwRoleID		= m_pRole->GetID();
	send.byDisplayPos	= (BYTE)nDisplayPos;
	send.byEquipEffect	= m_pRole->GetAvatarEquip().AvatarEquip[nDisplayPos].byEquipEffect;
	pMap->SendBigVisTileMsg(m_pRole, &send, send.dwSize);
}