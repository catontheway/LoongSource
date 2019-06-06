//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_container.h
// author: Sxg
// actor:
// data: 2008-8-1
// last:
// brief: ����װ���������ҡ��ֿ�ȴ�����
//--------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "role.h"
#include "creature.h"
#include "player_session.h"
#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/base_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "map.h"


//****************************** ��װ ******************************************

//--------------------------------------------------------------------------------------------
// ����Ƿ��ܹ�����
//--------------------------------------------------------------------------------------------
DWORD Role::CanEquip(tagEquip *pEquip, EEquipPos ePosDst)
{
	// �ж�װ���Ƿ����
	/*if(!pEquip->bIdentified)*/
	if(!MIsIdentified(pEquip))
	{
		return E_Equip_NotIdentify;
	}

	// �ж�����ȼ��Ƿ����
	if(m_nLevel < pEquip->equipSpec.byMinUseLevel 
		|| m_nLevel > pEquip->equipSpec.byMaxUseLevel)
	{
		return E_Item_LevelLimit;
	}

	const tagEquipProto *pEquipProto = pEquip->pEquipProto;

	// �ж�ְҵ����
	if ( E_Success != VocationCheck( pEquipProto->dwVocationLimitWear ))
		return E_Equip_VocationLimit;

	/*INT nTmpClass =  1 << ( m_eClass - 1 );
	INT nTmpClassEx = 0;
	
	if ( (INT)m_eClassEx != (INT)EHV_Base )
	{
		nTmpClassEx = 1 << ( m_eClassEx + 8 );
	}


	if (!(( nTmpClass + nTmpClassEx) & pEquipProto->dwVocationLimitWear))
		return E_Equip_VocationLimit;*/
	
	// �жϽ�ɫ�Ա�
	if(pEquipProto->eSexLimit != ESL_Null 
		&& pEquipProto->eSexLimit != m_Avatar.bySex)
	{
		return E_Item_SexLimit;
	}

	FLOAT fFactor = 1 + pEquip->equipSpec.n16AttALimModPct / 10000.0f;

	// �ж��������� -- ԭʼһ�����Լ�ֻ������������еĳ�ʼ���ԣ������Զ��ӵ���ֶ��ӵ㲿��
	for(INT32 i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		if(GetBaseAttValue(i) < (INT)(pEquipProto->n16AttALimit[i] * fFactor + pEquip->equipSpec.n16AttALimMod))
		{
			return E_Item_AttA_Limit;
		}
	}

	// �ж�ְҵ //?? ��ʱע��
	//if(!(pEquipProto->byClassLimit[m_eClass] & m_eClassEx))
	//{
	//	return E_Item_ClassLimit;
	//}

	// �ж���������
	if(pEquipProto->eClanRepute >= ERT_BEGIN && pEquipProto->eClanRepute < ERT_END)
	{
		ECLanType eClanType = MTRANS_ERT2ECLT(pEquipProto->eClanRepute);
		if(GetClanData().RepGetVal(eClanType) < pEquipProto->nClanReputeVal)
		{
			return E_Item_ClanRepLimit;
		}
	}

	//// ������������ֵ
	//if( 0 )
	//{
	//	return E_Item_OtherClanRepLimit;
	//}

	//// ��������λ���Ƿ��ظ�
	//if(MIsEquipment(pEquip->dwTypeID))
	//{
	//	tagEquip *pOtherWeapon = GetEquipBar().GetItem((INT16)(EEP_RightHand == ePosDst ? EEP_LeftHand : EEP_RightHand));
	//	if(!P_VALID(pOtherWeapon))
	//	{
	//		return E_Success;
	//	}

	//	MTRANS_POINTER(pEquipProto, pEquip->pProtoType, tagEquipProto);
	//	MTRANS_POINTER(pOtherProto, pOtherWeapon->pProtoType, tagEquipProto);

	//	if((EWP_WaistBack == pEquipProto->eWeaponPos || EWP_Back == pEquip->pEquipProto->eWeaponPos)
	//		&& pEquip->pEquipProto->eWeaponPos == pOtherProto->eWeaponPos)
	//	{
	//		return E_Equip_WeaponPos_Overlap;
	//	}
	//}

	return E_Success;
}

//--------------------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------------------
DWORD Role::Equip(INT64 n64Serial, EEquipPos ePosDst)
{
	// ��ô���װ��
	tagItem *pItem = GetItemMgr().GetBagItem(n64Serial);
	if(!P_VALID(pItem))
	{
		return E_Item_NotFound;
	}

	// �ж���װ����Ʒ�Ƿ�Ϊװ��
	if(!MIsEquipment(pItem->dwTypeID))
	{
		return E_Item_NotEquipment;
	}

	tagEquip *pEquip = (tagEquip *)pItem;

	// ����Ƿ����װ������
	DWORD dwErrorCode = CanEquip(pEquip, ePosDst);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ��õ�ǰװ��λ�õ�װ��
	tagEquip *pEquipOld = GetItemMgr().GetEquipBarEquip((INT16)ePosDst);

	// ��װ
	dwErrorCode = GetItemMgr().Equip(n64Serial, ePosDst);
	if(E_Success == dwErrorCode)
	{
		ProcEquipEffect(pEquip, pEquipOld, (INT16)ePosDst);
	}
	else
	{
		return dwErrorCode;
	}

	// ���񴥷�
	OnQuestEvent(EQE_Item, pItem->dwTypeID, 1, FALSE);

	return dwErrorCode;
}

//--------------------------------------------------------------------------------------------
// ���� -- Ŀ��λ��ӦΪ�գ����տͻ���Ӧ������װ����Ϣ
//--------------------------------------------------------------------------------------------
DWORD Role::Unequip(INT64 n64Serial, INT16 n16IndexDst)
{
	// ���Ŀ��λ���Ƿ�Ϊ�գ����վ�ֱ�ӷ���
	if(n16IndexDst != GT_INVALID && !GetItemMgr().IsBagOneSpaceFree(n16IndexDst))
	{
		return GT_INVALID;
	}

	// ��ô���װ��
	tagEquip *pEquipOld = GetItemMgr().GetEquipBarEquip(n64Serial);
	if(!P_VALID(pEquipOld))
	{
		return E_Item_NotFound;
	}

	INT16 n16IndexOld = pEquipOld->n16Index;

	// ��װ
	if(GT_INVALID == n16IndexDst)
	{
		n16IndexDst = GetItemMgr().GetBagOneFreeSpace();
		if(GT_INVALID == n16IndexDst)
		{
			return E_Bag_NotEnoughSpace;
		}
	}

	DWORD dwErrorCode = GetItemMgr().Unequip(n64Serial, n16IndexDst);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	ProcEquipEffect(NULL, pEquipOld, n16IndexOld);

	// ���񴥷�
	OnQuestEvent(EQE_Item, pEquipOld->dwTypeID, 1, TRUE);

	return dwErrorCode;
}

//--------------------------------------------------------------------------------------------
// �����ֶԻ�
//--------------------------------------------------------------------------------------------
DWORD Role::SwapWeapon()
{
	tagEquip *pEquipRight	= GetItemMgr().GetEquipBarEquip((INT16)EEP_RightHand);
	tagEquip *pEquipLeft	= GetItemMgr().GetEquipBarEquip((INT16)EEP_LeftHand);

	DWORD dwErrorCode = GetItemMgr().SwapWeapon();
	if(E_Success != dwErrorCode)
	{
		return dwErrorCode;
	}

	// ��װ��λ����Ϣ���͸��ͻ���
	ProcEquipEffectPos(pEquipRight, pEquipLeft, EICT_Equip, EICT_Equip);

	// ���¼����������� -- �����£�����
	if(P_VALID(pEquipRight))	// ����������
	{
		ResetWeaponDmg(*pEquipRight, FALSE);
	}

	if(P_VALID(pEquipLeft))
	{
		ResetWeaponDmg(*pEquipLeft, TRUE);
	}

	// ��������������¼��� -- ���Ѻ󴩣��ڲ�����ʱ������װ�������ظ�����������������
	//m_LongHun.Remove(n16OldPos1);
	//m_LongHun.Remove(n16OldPos2);
	//m_LongHun.Add(pRing1, n16PosDst, TRUE);
	//m_LongHun.Add(pRing2, n16OldPos1, TRUE);
	m_LongHun.Swap(pEquipRight, pEquipLeft, (INT16)EEP_RightHand, (INT16)EEP_LeftHand);

	// ���¼�����Ӱ������ﵱǰ״̬
	RecalAtt();

	// ����۷�����Χ���
	ProcEquipEffectAvatar(pEquipLeft, EEP_RightHand);
	ProcEquipEffectAvatar(pEquipRight, EEP_LeftHand);

	return E_Success;
}

//--------------------------------------------------------------------------------------------
// ����ָλ��
//--------------------------------------------------------------------------------------------
DWORD Role::MoveRing(INT64 n64SerialSrc, INT16 n16PosDst)
{
	tagEquip *pRing1 = GetItemMgr().GetEquipBarEquip(n64SerialSrc);
	tagEquip *pRing2 = GetItemMgr().GetEquipBarEquip(n16PosDst);

	INT16 n16OldPos1 = EEP_Null;
	//INT16 n16OldPos2 = EEP_Null;

	if(P_VALID(pRing1))
	{
		n16OldPos1 = pRing1->n16Index;
	}

	//if(P_VALID(pRing2))
	//{
	//	n16OldPos2 = pRing2->n16Index;
	//}

	DWORD dwErrorCode = GetItemMgr().MoveRing(n64SerialSrc, n16PosDst);
	if(E_Success != dwErrorCode)
	{
		return dwErrorCode;
	}

	// ��������������¼��� -- ���Ѻ󴩣��ڲ�����ʱ������װ�������ظ�����������������
	//m_LongHun.Remove(n16OldPos1);
	//m_LongHun.Remove(n16OldPos2);
	//m_LongHun.Add(pRing1, n16PosDst, TRUE);
	//m_LongHun.Add(pRing2, n16OldPos1, TRUE);
	m_LongHun.Swap(pRing1, pRing2, n16OldPos1, n16PosDst);

	// ��װ��λ����Ϣ���͸��ͻ���
	ProcEquipEffectPos(pRing1, pRing2, EICT_Equip, EICT_Equip);

	return E_Success;
}

//--------------------------------------------------------------------------------------------
// ���㻻װӰ��, �����͵��ͻ���
//--------------------------------------------------------------------------------------------
VOID Role::ProcEquipEffect(tagEquip *pNewEquip, tagEquip *pOldEquip, INT16 n16IndexOld, BOOL bDiscard/* = FALSE*/)
{
	// ��װ��λ����Ϣ���͸��ͻ���
	if(!bDiscard)
	{
		ProcEquipEffectPos(pNewEquip, pOldEquip, EICT_Equip, EICT_Bag);
	}

	// ����װ��Ӱ���������� -- ע��Ҫ�����ٴ�
	if(P_VALID(pOldEquip))
	{
		ProcEquipEffectAtt(pOldEquip, FALSE, n16IndexOld);
		m_Suit.Remove(pOldEquip, n16IndexOld);
		m_LongHun.Remove(n16IndexOld);
	}

	if(P_VALID(pNewEquip))
	{
		ProcEquipEffectAtt(pNewEquip, TRUE, n16IndexOld);
		m_Suit.Add(pNewEquip, n16IndexOld);
		m_LongHun.Add(pNewEquip, n16IndexOld, TRUE);
	}

	// ���¼�����Ӱ������ﵱǰ״̬
	RecalAtt();

	// ����۷�����Χ���
	ProcEquipEffectAvatar(pNewEquip, n16IndexOld);
}

//--------------------------------------------------------------------------------------------
// ���㻻װӰ��, ��װ��λ����Ϣ���͸��ͻ���
//--------------------------------------------------------------------------------------------
VOID Role::ProcEquipEffectPos(tagEquip *pNewEquip, tagEquip *pOldEquip, EItemConType eConTypeNewDst, EItemConType eConTypeNewSrc)
{
	tagNS_ItemPosChangeEx sendPos;
	sendPos.eConTypeSrc1 = eConTypeNewSrc;
	sendPos.eConTypeSrc2 = eConTypeNewDst;
	sendPos.eConTypeDst1 = eConTypeNewDst;
	sendPos.eConTypeDst2 = eConTypeNewSrc;
	sendPos.n64Serial1 = GT_INVALID;
	sendPos.n64Serial2 = GT_INVALID;
	sendPos.n16Num1 = 1;
	sendPos.n16Num2 = 1;
	sendPos.n16PosDst1 = GT_INVALID;
	sendPos.n16PosDst2 = GT_INVALID;

	if(P_VALID(pNewEquip))
	{
		sendPos.n64Serial1 = pNewEquip->n64Serial;
		sendPos.n16PosDst1 = pNewEquip->n16Index;
	}

	if(P_VALID(pOldEquip))
	{
		sendPos.n64Serial2 = pOldEquip->n64Serial;
		sendPos.n16PosDst2 = pOldEquip->n16Index;
	}

	m_pSession->SendMessage(&sendPos, sendPos.dwSize);
}

//--------------------------------------------------------------------------------------------
// ���㻻װӰ����������
//--------------------------------------------------------------------------------------------
VOID Role::ProcEquipEffectAtt(tagEquip *pEquip, bool bEquip, const INT16 n16Index)
{
	ASSERT(P_VALID(pEquip));
	
	INT32 nFactor = 1;
	pFun_RegTriggerEquipSet pRegTriggerEquipSet = &Role::RegisterTriggerEquipSet;
	if(!bEquip)
	{
		nFactor = -1;
		pRegTriggerEquipSet = &Role::UnRegisterTriggerEquipSet;
	}

	const tagEquipProto *pEquipProto = pEquip->pEquipProto;

	// ����
	if(EIT_Weapon == pEquipProto->eType)
	{
		// ֻ�������������˺�
		if(n16Index == pEquipProto->eEquipPos)
		{
			// ������������������(�Ѱ��Կ�Ӱ���������)
			ResetWeaponDmg(*pEquip, bEquip);
		}
	}
	else
	{
		// �Կ�
		ChangeRoleAtt(pEquip->equipSpec.nEngraveAtt, MAX_ROLEATT_ENGRAVE_EFFECT, ERA_EngraveAtt_Start, nFactor);
	}

	// ���߻���
	ModAttModValue(ERA_Armor, pEquip->equipSpec.n16Armor * nFactor);

	// ��Ʒ���޹ص����Լӳ�
	ChangeRoleAtt(pEquipProto->BaseEffect, MAX_ROLEATT_BASE_EFFECT, nFactor);
	
	// ����
	ChangeRoleAtt(pEquip->equipSpec.PosyEffect, MAX_ROLEATT_POSY_EFFECT, nFactor);

	// һ������
	ChangeRoleAtt(pEquip->equipSpec.nRoleAttEffect, X_ERA_ATTA_NUM, ERA_AttA_Start, nFactor);

	// ��ӡ -- �˺������������
	ChangeRoleAtt(pEquip->equipSpec.nBrandAtt, X_ERA_DERATE_NUM, ERA_Derate_Start, nFactor);

	// ��Ƕ
	ChangeRoleAtt(pEquip->equipSpec.dwHoleGemID, pEquip->equipSpec.byHoleNum, nFactor);

	// ���� -- ��������,������.���������
	
	// �Ƿ��б�,����������(GT_INVALID��ʾû��) -- ��ר�Ź���������㴦��(ͬsuit)

	// �������� -- ����ֱ��Ӱ�컻װ

	// װ��Buff��
	(this->*pRegTriggerEquipSet)(pEquipProto->dwTriggerID0, pEquipProto->dwBuffID0, n16Index);
	(this->*pRegTriggerEquipSet)(pEquipProto->dwTriggerID1, pEquipProto->dwBuffID1, n16Index);
	(this->*pRegTriggerEquipSet)(pEquipProto->dwTriggerID2, pEquipProto->dwBuffID2, n16Index);

	// ʱװ����
	if(pEquip->equipSpec.n16Appearance > 0)
	{
		ModAttModValue(ERA_Appearance, pEquip->equipSpec.n16Appearance * nFactor);
	}

	if(pEquip->equipSpec.byRein != 0)
	{
		ModAttModValue(ERA_Rein, pEquip->equipSpec.byRein * nFactor);
	}

	if(pEquip->equipSpec.bySavvy != 0)
	{
		ModAttModValue(ERA_Savvy, pEquip->equipSpec.bySavvy * nFactor);
	}

	if(pEquip->equipSpec.byFortune != 0)
	{
		ModAttModValue(ERA_Fortune, pEquip->equipSpec.byFortune * nFactor);
	}
}

//--------------------------------------------------------------------------------------------
// ���㻻װӰ���������, �����͵��ͻ���
//--------------------------------------------------------------------------------------------
VOID Role::ProcEquipEffectAvatar(tagEquip *pNewEquip, INT16 n16IndexOld)
{
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(pMap);
		return;
	}

	// ����װ����ʾ
	INT32 nPos = ResetOneEquipDisplay(pNewEquip, n16IndexOld);
	if(!GT_VALID(nPos))
	{
		return;
	}

	tagNS_AvatarEquipChange send;
	send.dwRoleID		= GetID();
	send.EquipDisplay	= GetAvatarEquip().AvatarEquip[nPos];
	
	pMap->SendBigVisTileMsg(this, &send, send.dwSize);
}

//--------------------------------------------------------------------------------------------
// ����װ����ʾ����������ʾ��λö��ֵ��GT_INVALID��ʾ��װ��������ʾ
//--------------------------------------------------------------------------------------------
INT32 Role::ResetOneEquipDisplay(tagEquip* pEquip, INT16 n16Index)
{
	INT32 nPos = GetEquipDisplayPos(n16Index);
	if(!GT_VALID(nPos))
	{
		return GT_INVALID;
	}

	// �ж���ʱװ������װ��
	bool bFashion = true;
	if(n16Index >= EEP_Equip_Start && n16Index <= EEP_Equip_End)
	{
		bFashion = FALSE;
	}

	if(!P_VALID(pEquip))
	{		
		SetEquipDisplay(bFashion, nPos, GT_INVALID, X_DEFAULT_FLARE_VAL, EC_Null);
	}
	else
	{
		// �������Ч�� -- ���꼤����Ч�����ڹ������д���
		SetEquipDisplay(bFashion, nPos, pEquip->dwTypeID, 
						pEquip->equipSpec.byFlareVal, pEquip->equipSpec.n8ColorID);
	}

	// �жϸ�����Ƿ���Ҫ��ʾ
	if(GetDisplaySet().bFashionDisplay != bFashion && !MIsWeaponByDisplayPos(nPos))
	{
		nPos = GT_INVALID;
	}

	return nPos;
}

//--------------------------------------------------------------------------------------------
// �ж�װ����λ����װ���Ƿ���Ҫ��ʾ
//--------------------------------------------------------------------------------------------
INT32 Role::GetEquipDisplayPos(INT16 n16EquipPos)
{
	// ��Ʒ����Ҫ��ʾ
	if(EEP_Finger1 == n16EquipPos || EEP_Finger2 == n16EquipPos
		|| EEP_Waist == n16EquipPos || EEP_Neck == n16EquipPos)
	{
		return GT_INVALID;
	}

	// ת��Ϊ��ʾλ��
	switch(n16EquipPos)
	{
	// ����
	case EEP_RightHand:
		return EAE_RWeapon;
		break;
	case EEP_LeftHand:
		return EAE_LWeapon;
		break;

	// ʱװ
	case EEP_FashionHead:
		return EAE_Head;
		break;
	case EEP_FashionFace:
		return EAE_Face;
		break;
	case EEP_FashionBody:
		return EAE_Upper;
		break;
	case EEP_FashionLegs:
		return EAE_Lower;
		break;
	case EEP_FashionBack:
		return EAE_Back;
		break;
	case EEP_FashionWrist:
		return EAE_Glove;
		break;
	case EEP_FashionFeet:
		return EAE_Boot;
		break;

	// ����
	case EEP_Head:
		return EAE_Head;
		break;
	case EEP_Face:
		return EAE_Face;
		break;
	case EEP_Body:
		return EAE_Upper;
		break;
	case EEP_Legs:
		return EAE_Lower;
		break;
	case EEP_Back:
		return EAE_Back;
		break;
	case EEP_Wrist:
		return EAE_Glove;
		break;
	case EEP_Feet:
		return EAE_Boot;
		break;
	}

	return GT_INVALID;
}

//--------------------------------------------------------------------------------------------
// ����ĥ��������˺�Ӱ��.����ֵΪ�������˺�Ӱ��İٷֱ�ֵ.
//--------------------------------------------------------------------------------------------
FLOAT Role::CalAbrasionEffect(const tagEquip &Equip)
{
	INT32 nCurNewness = Equip.pEquipProto->n16Newness 
							- Equip.nUseTimes / ABRASION2USETIMES;
	nCurNewness = max(nCurNewness, 0);

	/*	AΪ������ǰո�¶ȣ�BΪ����ԭʼ�˺�
		A=200-999	�����˺�����꣩�ӳ� = 8% * B
		A=121-199	�����˺�����꣩�ӳ� =��A-120��* 0.1% * B
		A=80-120	�����˺�����꣩�ӳ� = 0
		A=50-79		�����˺�����꣩�ӳ� = -5% * B
		A=20-49		�����˺�����꣩�ӳ� = -15% * B
		A=5-19		�����˺�����꣩�ӳ� = -25% * B
		A=0-4		�����˺�����꣩�ӳ� = -50% * B	*/

	if(nCurNewness >= 200)
	{
		return 0.08f;
	}

	if(nCurNewness >= 121)
	{
		return (nCurNewness - 120) * 0.001f;
	}

	if(nCurNewness >= 80)
	{
		return 0.0f;
	}

	if(nCurNewness >= 50)
	{
		return -0.05f;
	}

	if(nCurNewness >= 20)
	{
		return -0.15f;
	}

	if(nCurNewness >= 5)
	{
		return -0.25f;
	}

	return -0.5f;
}

//--------------------------------------------------------------------------------------------
// ���������˺� -- ��װʱ����.
//--------------------------------------------------------------------------------------------
VOID Role::ResetWeaponDmg(const tagEquip &Equip, BOOL bEquip)
{
	if(bEquip)	// ����
	{
		FLOAT fDmgPct = 1.0f + CalAbrasionEffect(Equip);

		SetBaseAttValue(ERA_WeaponDmgMin, (INT)(Equip.equipSpec.n16MinDmg * fDmgPct));
		SetBaseAttValue(ERA_WeaponDmgMax, (INT)(Equip.equipSpec.n16MaxDmg * fDmgPct));
		SetBaseAttValue(ERA_WeaponSoul, (INT)(Equip.equipSpec.n16WuHun * fDmgPct));

		// �Կ�
		ChangeRoleAtt(Equip.equipSpec.nEngraveAtt, MAX_ROLEATT_ENGRAVE_EFFECT, ERA_EngraveAtt_Start, 1);
	}
	else	// ����
	{
		SetBaseAttValue(ERA_WeaponDmgMin, 0);
		SetBaseAttValue(ERA_WeaponDmgMax, 0);
		SetBaseAttValue(ERA_WeaponSoul, 0);

		// �Կ�
		ChangeRoleAtt(Equip.equipSpec.nEngraveAtt, MAX_ROLEATT_ENGRAVE_EFFECT, ERA_EngraveAtt_Start, -1);
	}
}

//--------------------------------------------------------------------------------------------
// ���������˺� -- ո�¶ȱ仯ʱ����.
//--------------------------------------------------------------------------------------------
VOID Role::ResetWeaponDmg(tagEquip &Equip)
{
	// ����ո�¶ȱ仯֮ǰ�������˺�Ӱ��
	--Equip.nUseTimes;
	FLOAT fDmgPctPre = CalAbrasionEffect(Equip);

	// ��ǰո�¶ȶ������˺�Ӱ��
	++Equip.nUseTimes;
	FLOAT fDmgPct = CalAbrasionEffect(Equip);

	// Ӱ�첻ͬ������������
	if(fDmgPctPre != fDmgPct)
	{
		fDmgPct += 1.0f;

		SetBaseAttValue(ERA_WeaponDmgMin, (INT)(Equip.equipSpec.n16MinDmg * fDmgPct));
		SetBaseAttValue(ERA_WeaponDmgMax, (INT)(Equip.equipSpec.n16MaxDmg * fDmgPct));
		SetBaseAttValue(ERA_WeaponSoul, (INT)(Equip.equipSpec.n16WuHun * fDmgPct));
	}
}

//--------------------------------------------------------------------------------------------
// ����װ������Ӱ��,���޸Ķ�Ӧ�����Լӳ�ֵ.
//--------------------------------------------------------------------------------------------
VOID Role::ChangeRoleAtt(const tagRoleAttEffect Effect[], INT32 nArraySz, INT32 nFactor)
{
	for(INT32 i=0; i<nArraySz; ++i)
	{
		if(Effect[i].eRoleAtt <= ERA_Null || Effect[i].eRoleAtt >= ERA_End)
		{
			//ASSERT(Effect[i].eRoleAtt == ERA_Null);
			return;
		}

		if(MIsValuePct(Effect[i].nValue))		// �ٷ���
		{
			ModAttModValuePct(Effect[i].eRoleAtt, MValuePctTrans(Effect[i].nValue) * nFactor);
		}
		else	// ��ֵ
		{
			ModAttModValue(Effect[i].eRoleAtt, Effect[i].nValue * nFactor);
		}
	}
}

//--------------------------------------------------------------------------------------------
// ����װ������Ӱ��,���޸Ķ�Ӧ�����Լӳ�ֵ.
//--------------------------------------------------------------------------------------------
VOID Role::ChangeRoleAtt(const INT32 nValue[], INT32 nArraySz, INT32 nAttStart, INT32 nFactor)
{
	ASSERT(nAttStart > ERA_Null && nAttStart + nArraySz < ERA_End);
	
	for(INT32 i=0; i<nArraySz; ++i)
	{
		if(MIsValuePct(nValue[i]))		// �ٷ���
		{
			ModAttModValuePct(nAttStart + i, MValuePctTrans(nValue[i]) * nFactor);
		}
		else	// ��ֵ
		{
			ModAttModValue(nAttStart + i, nValue[i] * nFactor);
		}
	}
}

//--------------------------------------------------------------------------------------------
// ����װ������Ӱ��,���޸Ķ�Ӧ�����Լӳ�ֵ. -- ��ʯӰ��
//--------------------------------------------------------------------------------------------
VOID Role::ChangeRoleAtt(const DWORD dwValue[], INT32 nArraySz, INT32 nFactor)
{
	ASSERT(nArraySz <= MAX_EQUIPHOLE_NUM);

	for(INT32 i=0; i<nArraySz; ++i)
	{
		if(0 == dwValue[i] || ERA_Null == dwValue[i])
		{
			break;
		}

		const tagConsolidateItem *pGemProto = g_attRes.GetConsolidateProto(dwValue[i]);
		if(!P_VALID(pGemProto))
		{
			ASSERT(P_VALID(pGemProto));
			continue;
		}

		ChangeRoleAtt((tagRoleAttEffect*)pGemProto->tagRoleAtt, MAX_CONSOLIDATE_ROLEATT, nFactor);
	}
}

//--------------------------------------------------------------------------------------------
// װ����buff����
//--------------------------------------------------------------------------------------------
VOID Role::ProcEquipBuffTrigger(DWORD dwBuffID, BOOL bEquip)
{
	INT nTmp;
	ERoleAttribute	eTmp;
	INT nFactor = (bEquip) ? 1 : -1;
	
	const tagBuffProto *pBuffProto = g_attRes.GetBuffProto(dwBuffID);
	if( !P_VALID(pBuffProto) )
	{
		MAttResCaution(_T("buff proto"), _T("BuffID"), dwBuffID);
		return;
	}

	if( pBuffProto->mapRoleAttMod.Size() > 0 )
	{
		TMap<ERoleAttribute, INT>::TMapIterator iter = pBuffProto->mapRoleAttMod.Begin();
		while( pBuffProto->mapRoleAttMod.PeekNext(iter, eTmp, nTmp) )
		{
			ModAttModValue(eTmp, nTmp * nFactor);
		}
	}

	if( pBuffProto->mapRoleAttModPct.Size() > 0 )
	{
		TMap<ERoleAttribute, INT>::TMapIterator iter = pBuffProto->mapRoleAttModPct.Begin();
		while( pBuffProto->mapRoleAttModPct.PeekNext(iter, eTmp, nTmp) )
		{
			ModAttModValuePct(eTmp, nTmp * nFactor);
		}
	}
}

// ��ɫ�ɼ����ܼӳ�
INT Role::CalGatherRate( Creature* pCreature )
{
	Skill* pGatherSkill = NULL;
	if ( pCreature->IsNatuRes() )
		pGatherSkill = GetSkill(GATHER_SKILL_MINING);
	else if ( pCreature->IsManRes() )
		pGatherSkill = GetSkill(GATHER_SKILL_HARVEST);
	if ( !P_VALID(pGatherSkill) )
		return 0;

	INT nSkillLvl = pGatherSkill->GetLevel();

	if (nSkillLvl <= 0)
		return 0;

	// �õ��ջ��ɫ��Ե 	
	FLOAT fFortune = (FLOAT)GetAttValue(ERA_Fortune);

	// ��������ʼӳ�	�������ʼӳ�=5%�����ջ��ܵȼ�-1��/10+��ɫ��Ե/1000
	// Jason 2009-11-28
	// �������ʼӳ�=20%�������ɼ��ܵȼ�-1��/10+��ɫ��Ե/1000
	return 0.2 * (nSkillLvl - 1) * 0.1 + fFortune / 1000;
	//INT nAddRatPct = INT((5.0f * FLOAT(nSkillLvl - 1) + fFortune) * 10.0f);

	//return nAddRatPct;
};

// ����ʵ��ֵ���������־���ӳ����Լ��㣩
INT Role::CalRealGainExp( INT nSrcExp )
{
	// ������������
	FLOAT fEarnRate		= GetEarnRate() / 10000.0f;

	// gm������
	FLOAT fGMRate		= GetMap()->GetExpRate();

	// role��������
	FLOAT fRoleRate		= 1 + GetAttValue(ERA_Exp_Add_Rate)/10000.0f + GetVNBExpRate();

	// ԭ����/��Ǯ/���� * (1+�ӳ�/10000)
	FLOAT fExpRate	= max(fGMRate, fRoleRate);

	return INT(nSrcExp * fEarnRate * fExpRate);
	
}

// ������ֿ�
DWORD Role::OpenSideWare(INT64 n64ItemSerial)
{
	tagItem *pItem = GetItemMgr().GetBagItem(n64ItemSerial);
	if(!P_VALID(pItem))
	{
		return E_UseItem_ItemNotExist;
	}

	if(pItem->pProtoType->eSpecFunc != EISF_PortableWare)
	{
		return E_UseItem_SpecFunError;
	}

	// ������Ʒ
	//DWORD dwErrorCode = GetItemMgr().ItemUsedFromBag(n64ItemSerial, 1, ELCLD_Item_Use);
	//if(dwErrorCode != E_Success)
	//{
	//	return E_UseSkill_CostLimit;
	//}

	return E_Success;
}
