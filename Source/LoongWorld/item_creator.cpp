//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_creator.cpp
// author: Sxg
// actor:
// data: 2008-07-29
// last:
// brief: ��Ʒ&װ�������� -- ��Ʒ&װ������,����������(��Ʒ����������)
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/RoleDefine.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/item_define.h"

#include "att_res.h"
#include "item_creator.h"
#include "world.h"
#include "script_mgr.h"

INT64			ItemCreator::m_n64MaxSerial = 0;
INT64			ItemCreator::m_n64MinSerial = 0;
Mutex			ItemCreator::m_Mutex;

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
ItemCreator::ItemCreator()
{}

ItemCreator::~ItemCreator()
{}

//-------------------------------------------------------------------------------------------------------
// ����������Ʒ�����µĶ���Ʒ
//-------------------------------------------------------------------------------------------------------
tagItem* ItemCreator::Create(const tagItem &item, INT16 n16Num)
{
	if(n16Num > item.pProtoType->n16MaxLapNum)
	{
		ASSERT(n16Num <= item.pProtoType->n16MaxLapNum);
		return NULL;
	}
	
	tagItem *pNewItem;
	if(MIsEquipment(item.dwTypeID))
	{
		pNewItem = new tagEquip;
		IFASTCODE->MemCpy(pNewItem, &item, SIZE_EQUIP);
	}
	else
	{
		pNewItem = new tagItem;
		IFASTCODE->MemCpy(pNewItem, &item, SIZE_ITEM);
	}

	CreateItemSerial(pNewItem->n64Serial);
	pNewItem->n16Num = n16Num;

	return pNewItem;
}

//-------------------------------------------------------------------------------------------------------
// ������Ʒ&װ��
// Jason 2009-12-1 ������ɫƷ��Ӱ�죬װ��Ǳ��ֵ�ܲ���Ӱ��
//-------------------------------------------------------------------------------------------------------
tagItem* ItemCreator::Create(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID,
							 INT16 n16Num/* = 1*/, DWORD dwCreator/* = GT_INVALID*/,
							 INT16 n16QltyModPct/* = 0*/, INT16 n16QltyModPctEx/* = 0*/, INT16 n16PotValPct/* = 10000*/, INT nPotAdding)
{
	if(n16Num <= 0)
	{
		ASSERT(n16Num > 0);
		return NULL;
	}
	
	tagItem *pRetItem;
	if(MIsEquipment(dwTypeID))
	{
		pRetItem = new tagEquip;
		if(NULL == pRetItem)
		{
			ASSERT(pRetItem != NULL);
			return NULL;
		}

		InitEquipSpec(((tagEquip*)pRetItem)->equipSpec, n16QltyModPct, n16QltyModPctEx, n16PotValPct);
		pRetItem->pProtoType = g_attRes.GetEquipProto(dwTypeID);
	}
	else
	{		
		pRetItem = new tagItem;
		if(NULL == pRetItem)
		{
			ASSERT(pRetItem != NULL);
			return NULL;
		}

		pRetItem->pProtoType = g_attRes.GetItemProto(dwTypeID);
	}

	if(!P_VALID(pRetItem->pProtoType) || pRetItem->pProtoType->n16MaxLapNum < n16Num)
	{
		MAttResCaution(_T("item or equip proto"), _T("typeid"), dwTypeID);
		ASSERT(P_VALID(pRetItem->pProtoType));
		ASSERT(P_VALID(pRetItem->pProtoType) && pRetItem->pProtoType->n16MaxLapNum >= n16Num);
		::Destroy(pRetItem);
		return NULL;
	}

	INT64 n64Serial;
	CreateItemSerial(n64Serial);

	InitItem(*pRetItem, eCreateMode, pRetItem->pProtoType, dwCreateID, n64Serial, n16Num, dwCreator, g_world.GetWorldTime(), GT_INVALID, GT_INVALID);

	// װ��
	if(MIsEquipment(pRetItem->dwTypeID))
	{
		// Jason 2009-12-1 װ��Ǳ��ֵ��������
		((tagEquip*)pRetItem)->equipSpec.nPotIncTimes	= ((tagEquip*)pRetItem)->pEquipProto->nPotIncTimes + nPotAdding;	
		// ����Ƿ�Ϊ����������ɺ�ʱ����
		if( (pRetItem->pEquipProto->bIdentifyLoot && EICM_Loot == eCreateMode)
			|| (pRetItem->pEquipProto->bIdentifyProduct && EICM_Product == eCreateMode)
			|| (EICM_LessingLoong == eCreateMode && pRetItem->pEquipProto->bIdentifyProduct ) 
			|| (EICM_GoldStone == eCreateMode && pRetItem->pEquipProto->bIdentifyProduct ) )
		{
			ItemCreator::IdentifyEquip((tagEquip*)pRetItem);
		}
	}

	return pRetItem;
}
// Jason 2009-12-1 ������ɫƷ���������
tagItem* ItemCreator::CreateByProduce(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, EItemQuality eQlty,INT nPotAdding,DWORD dwPurpleQltyIdfPct, 
							 INT16 n16Num/* = 1*/, DWORD dwCreator/* = GT_INVALID*/,
							 INT16 n16QltyModPct/* = 0*/, INT16 n16QltyModPctEx/* = 0*/, INT16 n16PotValPct/* = 10000*/)
{
	if(n16Num <= 0)
	{
		ASSERT(n16Num > 0);
		return NULL;
	}

	tagItem *pRetItem;
	if(MIsEquipment(dwTypeID))
	{
		pRetItem = new tagEquip;
		if(NULL == pRetItem)
		{
			ASSERT(pRetItem != NULL);
			return NULL;
		}

		InitEquipSpec(((tagEquip*)pRetItem)->equipSpec, n16QltyModPct, n16QltyModPctEx, n16PotValPct);
		pRetItem->pProtoType = g_attRes.GetEquipProto(dwTypeID);
	}
	else
	{		
		pRetItem = new tagItem;
		if(NULL == pRetItem)
		{
			ASSERT(pRetItem != NULL);
			return NULL;
		}

		pRetItem->pProtoType = g_attRes.GetItemProto(dwTypeID);
	}

	if(!P_VALID(pRetItem->pProtoType) || pRetItem->pProtoType->n16MaxLapNum < n16Num)
	{
		MAttResCaution(_T("item or equip proto"), _T("typeid"), dwTypeID);
		ASSERT(P_VALID(pRetItem->pProtoType));
		ASSERT(P_VALID(pRetItem->pProtoType) && pRetItem->pProtoType->n16MaxLapNum >= n16Num);
		::Destroy(pRetItem);
		return NULL;
	}

	INT64 n64Serial;
	CreateItemSerial(n64Serial);

	InitItem(*pRetItem, eCreateMode, pRetItem->pProtoType, dwCreateID, n64Serial, n16Num, dwCreator, g_world.GetWorldTime(), GT_INVALID, GT_INVALID);

	// װ��
	if(MIsEquipment(pRetItem->dwTypeID))
	{
		// Jason 2009-12-1 װ��Ǳ��ֵ��������
		((tagEquip*)pRetItem)->equipSpec.nPotIncTimes	= ((tagEquip*)pRetItem)->pEquipProto->nPotIncTimes + nPotAdding;
		((tagEquip*)pRetItem)->equipSpec.dwPurpleQltyIdfPct = dwPurpleQltyIdfPct;
		// ����Ƿ�Ϊ����������ɺ�ʱ����
		if((pRetItem->pEquipProto->bIdentifyLoot && EICM_Loot == eCreateMode)
			|| (pRetItem->pEquipProto->bIdentifyProduct && EICM_Product == eCreateMode)
			|| (EICM_LessingLoong == eCreateMode && pRetItem->pEquipProto->bIdentifyProduct ) 
			|| (EICM_GoldStone == eCreateMode && pRetItem->pEquipProto->bIdentifyProduct ) )
		{
			ItemCreator::IdentifyEquip((tagEquip*)pRetItem,eQlty);
		}
	}

	return pRetItem;
}


//-------------------------------------------------------------------------------------------------------
// �������ݿ��ȡ�����ݴ�����Ʒ
//-------------------------------------------------------------------------------------------------------
tagItem* ItemCreator::CreateItemByData( PVOID pData )
{
	MTRANS_POINTER(pItem, pData, tagItem);
	tagItem* pNewItem = new tagItem;

	IFASTCODE->MemCpy(pNewItem, pItem, sizeof(tagItem));
	pNewItem->pProtoType = g_attRes.GetItemProto(pItem->dwTypeID);

	if (P_VALID(pNewItem->pProtoType) && IsGMItemNoInit(pNewItem))
	{
		InitItem(*pNewItem, pItem->eCreateMode, pNewItem->pProtoType, pNewItem->dwCreateID, pNewItem->n64Serial, pNewItem->n16Num, pNewItem->dwCreatorID, pNewItem->dwCreateTime, pNewItem->dwAccountID, pNewItem->dwOwnerID);
	}

	return pNewItem;
}

//-------------------------------------------------------------------------------------------------------
// �������ݿ��ȡ�����ݴ���װ��
//-------------------------------------------------------------------------------------------------------
tagEquip* ItemCreator::CreateEquipByData( PVOID pData )
{
	MTRANS_POINTER(pEquip, pData, tagEquip);
	tagEquip* pNewEquip = new tagEquip;

	IFASTCODE->MemCpy(pNewEquip, pEquip, sizeof(tagEquip));
	pNewEquip->pProtoType = g_attRes.GetEquipProto(pEquip->dwTypeID);

	if (IsGMItemNoInit(pEquip))
	{
		InitItem(*pNewEquip, pNewEquip->eCreateMode, pNewEquip->pProtoType, pNewEquip->dwCreateID, pNewEquip->n64Serial, pNewEquip->n16Num, pNewEquip->dwCreatorID, pNewEquip->dwCreateTime, pNewEquip->dwAccountID, pNewEquip->dwOwnerID);

		EItemQuality eQuality = (EItemQuality)pNewEquip->equipSpec.byQuality;
		InitEquipSpec(pNewEquip->equipSpec);
		IdentifyEquip(pNewEquip, eQuality);
	}

	return pNewEquip;
}

//-------------------------------------------------------------------------------------------------------
// ��������Ψһ��(ע��Ҫ����������)
//-------------------------------------------------------------------------------------------------------
VOID ItemCreator::CreateItemSerial(INT64 &n64NewSerial)
{
	m_Mutex.Acquire();
	n64NewSerial = ++m_n64MaxSerial;
	m_Mutex.Release();
}

//-------------------------------------------------------------------------------------------------------
// ������Ʒ
//-------------------------------------------------------------------------------------------------------
VOID ItemCreator::InitItem( tagItem &item, EItemCreateMode eCreateMode, const tagItemProto *pProto, DWORD dwCreateID, INT64 n64Serial, INT16 n16Num, DWORD dwCreator, DWORD dwCreateTime, DWORD dwAccountID, DWORD dwOwnerID )
{
	ZeroMemory(&item, SIZE_ITEM);

	item.pProtoType		= pProto;
	
	item.n64Serial		= n64Serial;
	item.n16Num			= n16Num;
	item.dwTypeID		= pProto->dwTypeID;

	item.byBind			= EBS_Unknown;
	item.bLock			= FALSE;

	item.eCreateMode	= eCreateMode;
	item.dwCreateID		= dwCreateID;	// ���ɸ���Ʒ��ID,��: QuestID,GMID��
	
	item.dwCreatorID	= dwCreator;	/* ��ΪRoleID, ��ĳλ���������������Ʒ,��λ���ܱ�ʾΪ����Ҵ���;
											������Ʒͨ������ϵͳ����ʱ,���λͬdwCreateID*/
	item.dwNameID		= GT_INVALID;
	item.dwCreateTime	= dwCreateTime;

	item.dwOwnerID		= dwOwnerID;
	item.dwAccountID	= dwAccountID;
	item.eConType		= EICT_Null;
	item.n16Index		= GT_INVALID;

	item.eStatus		= EUDBS_Insert;

	item.pScript		= g_ScriptMgr.GetItemScript(pProto->dwTypeID);
}

//-------------------------------------------------------------------------------------------------------
// ����δ����װ��(װ��ר�ò�������)
//-------------------------------------------------------------------------------------------------------
VOID ItemCreator::InitEquipSpec(tagEquipSpec &equipSpec, INT16 n16QltyModPct, INT16 n16QltyModPctEx, INT16 n16PotValPct)
{
	ZeroMemory(&equipSpec, SIZE_EQUIPSPEC);

	equipSpec.byQuality		= EIQ_Null;
	equipSpec.bCanCut		= TRUE;

	for(INT32 i=0; i<MAX_ROLEATT_POSY_EFFECT; ++i)
	{
		equipSpec.PosyEffect[i].eRoleAtt = ERA_Null;
	}

	equipSpec.dwLongHunInnerID	= GT_INVALID;
	equipSpec.dwLongHunOuterID	= GT_INVALID;

	equipSpec.bySpecAtt			= EESA_Null;
	equipSpec.n8ColorID			= EC_Null;

	equipSpec.n16QltyModPct		= n16QltyModPct;
	equipSpec.n16QltyModPctEx	= n16QltyModPctEx;

	equipSpec.n16PotValModPct	= n16PotValPct;
}

//-----------------------------------------------------------------------------
// ����װ��(û��Ʒ����Ʒ���Ƿ��������¼���Ʒ��)
//-----------------------------------------------------------------------------
VOID ItemCreator::IdentifyEquip(IN OUT tagEquip *pEquip, 
								EItemQuality eQlty/* = EIQ_Null*/, 
								const tagIMEffect *pIMEffect/* = NULL*/)
{
	ASSERT(P_VALID(pEquip));
	ASSERT(P_VALID(pEquip->pEquipProto));
	ASSERT(!MIsIdentified(pEquip));

	// ����Ƿ����Ѽ�����װ��
	if(MIsIdentified(pEquip))
	{
		return;
	}
	
	INT32 nQuality = eQlty;
	
	// û��Ʒ����Ʒ���Ƿ��������¼���Ʒ��
	bool bPurpleQltyIdf = false;
	if( pEquip->equipSpec.dwPurpleQltyIdfPct )
	{
		FLOAT fRnd = ( (FLOAT) IUTIL->RandomInRange(0,100) ) / 100;
		FLOAT fPct = (FLOAT)pEquip->equipSpec.dwPurpleQltyIdfPct / 100;
		if( fRnd <= fPct )//�ɹ�
			bPurpleQltyIdf = true;
	}
	
	if( eQlty <= EIQ_Start || eQlty >= EIQ_End ) 
	{
		if( !bPurpleQltyIdf ) //��ɫ֮��ģ�����ԭƷ�����ɷ�ʽ
		{
			// ����װ��Ʒ�����ɼ�������Ʒ��
			nQuality = GenBaseEquipQlty(pEquip->dwTypeID);

			// ��������Ʒ��������������Ʒ��
			nQuality = ModEquipQltyByProduce(pEquip, nQuality);
		}
		else
			nQuality = EIQ_Purple;
	}

	// ��ʼ��Ʒ��
	pEquip->equipSpec.byQuality = (BYTE)nQuality;

	// ��ʼ���ȼ�����
	pEquip->equipSpec.byMinUseLevel = pEquip->pEquipProto->byMinUseLevel;	// �ȼ�����
	pEquip->equipSpec.byMaxUseLevel = pEquip->pEquipProto->byMaxUseLevel;	// �ȼ�����

	// ����Ʒ�������������
	if(!MIsFashion(pEquip))
	{
		CreateEquipQltyRel(pEquip, pEquip->pEquipProto, (EItemQuality)nQuality);
	}
	else
	{
		CreateFashionQltyRel(pEquip, pEquip->pEquipProto, (EItemQuality)nQuality, pIMEffect);
	}
}

//-----------------------------------------------------------------------------
// ����װ��Ʒ�����ɼ�������Ʒ��
//-----------------------------------------------------------------------------
INT32 ItemCreator::GenBaseEquipQlty(DWORD dwTypeID)
{
	tagEquipQltyPct *pEquipQltyPct = g_attRes.GetEquipQltyPct(dwTypeID);
	if(!P_VALID(pEquipQltyPct))
	{
		ASSERT(P_VALID(pEquipQltyPct));
		return EIQ_White;
	}

	INT32 nEquipQltySumPct = 0;
	INT32 nRandPct = IUTIL->Rand() % 10000;
	// Jason 2009-11-30 �����Զ�������ɫƷ��װ��
	// ע�⣬��ɫƷ��װ���ǲ����Զ����ɵġ�
	// ���att_res.cpp�е�InitOneEquipQltyPct������Ҫ�Ķ���
	for(INT32 i=EIQ_Purple-1/*EIQ_End - 1*/; i>EIQ_Start; --i)
	{
		nEquipQltySumPct += pEquipQltyPct->nEquipQltyPct[i]; // Jason nEquipQltyPct��ά����EIQ_End - EIQ_Start - 1������������װ����ˣ�����������������
		if(nRandPct < nEquipQltySumPct)
		{
			return i;
		}
	}

	return EIQ_White;
}

//-----------------------------------------------------------------------------
// ��������Ʒ��������������Ʒ��
// ������������ɫƷ��������
// �����Զ�������ɫװ������ˣ���EIQ_END��ΪEIQ_Purple
//-----------------------------------------------------------------------------
INT32 ItemCreator::ModEquipQltyByProduce(const tagEquip *pEquip, INT32 nQuality)
{
	if(pEquip->equipSpec.n16QltyModPctEx != 0
		&& (IUTIL->Rand() % 10000) < pEquip->equipSpec.n16QltyModPctEx)	// ��������
	{
		return min(nQuality + 2, EIQ_Purple - 1);
	}
	else	// һ������
	{
		if(pEquip->equipSpec.n16QltyModPct > 0)
		{
			if((IUTIL->Rand() % 10000) < pEquip->equipSpec.n16QltyModPct)
			{
				return min(nQuality + 1, EIQ_Purple - 1);
			}
		}
		else if(pEquip->equipSpec.n16QltyModPct < 0)
		{
			if((IUTIL->Rand() % 10000) < -pEquip->equipSpec.n16QltyModPct)
			{
				if( nQuality >= EIQ_Purple )
					nQuality = EIQ_Purple - 1 ;
				return max(nQuality - 1, EIQ_White);
			}
		}
	}

	return nQuality;
}

//-----------------------------------------------------------------------------
// ����ָ��Ʒ������װ���������
//-----------------------------------------------------------------------------
DWORD ItemCreator::GenLongHunRandID(INT nLongHunType, INT nEquipPos, INT nQlty)
{
	if(nEquipPos < EEP_Equip_Start || nEquipPos > EEP_Equip_End)
	{
		ASSERT(0);
		return GT_INVALID;
	}
	
	const std::vector<DWORD> vectLongHun 
		= g_attRes.GetLongHunSpecVect(nLongHunType, g_eEquipGenLongHunTypeEx[nEquipPos], nQlty);

	INT nSzVect = vectLongHun.size();
	if(nSzVect > 0)
	{
		INT nRandPct = IUTIL->Rand() % nSzVect;
		return vectLongHun[nRandPct];
	}

	return GT_INVALID;
}

//-----------------------------------------------------------------------------
// ����ָ��Ʒ������װ���������
//-----------------------------------------------------------------------------
VOID ItemCreator::CreateEquipQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, EItemQuality eQlty)
{
	ASSERT(eQlty > EIQ_Start && eQlty < EIQ_End);
	
	// �õ�ָ��Ʒ��װ�����Բ���
	const tagEquipQltyEffect *pEquipQltyEffect = g_attRes.GetEquipQltyEffect(eQlty);
	if(!P_VALID(pEquipQltyEffect))
	{
		ASSERT(P_VALID(pEquipQltyEffect));
		return;
	}
	
	// ����ϵ��
	FLOAT fValueModFactor;

	// װ���������� -- "������ԭʼ�༭ֵ; ���ߣ�ԭʼ�༭ֵ"
	fValueModFactor = pEquipQltyEffect->fWeaponFactor;
	pEquip->equipSpec.n16WuHun	= (INT16)(pEquipProto->n16WuHun * fValueModFactor);		// �ڹ��˺�������
	pEquip->equipSpec.n16MinDmg	= (INT16)(pEquipProto->n16MinDmg * fValueModFactor);	// ������С�˺�
	pEquip->equipSpec.n16MaxDmg	= (INT16)(pEquipProto->n16MaxDmg * fValueModFactor);	// ��������˺�

	fValueModFactor = pEquipQltyEffect->fArmorFactor;
	pEquip->equipSpec.n16Armor	= (INT16)(pEquipProto->n16Armor * fValueModFactor);		// ���߻���

	// ��ɫһ������	--	A4 ��ֵ4=0.5+�ȼ���0.05	����ֵ4�������������1��2����3��һ������֮��							
	ZeroMemory(pEquip->equipSpec.nRoleAttEffect, sizeof(pEquip->equipSpec.nRoleAttEffect));
	
	fValueModFactor = pEquipQltyEffect->fAttAFactor;
	INT32 nValueModLeft = (INT32)(pEquipQltyEffect->nAttAFactor + pEquipProto->byLevel * fValueModFactor);
	INT32 nValueMod, nAttAIndex, nAttANumMod = 0;

	while(nValueModLeft != 0)
	{
		++nAttANumMod;
		nValueMod = IUTIL->Rand() % nValueModLeft + 1;
		nAttAIndex = IUTIL->Rand() % X_ERA_ATTA_NUM;

		if(nAttANumMod >= pEquipQltyEffect->nAttANumEffect)
		{
			nValueMod = nValueModLeft;
		}

		pEquip->equipSpec.nRoleAttEffect[nAttAIndex] += nValueMod;

		nValueModLeft -= nValueMod;
	}

	// װ��Ǳ��ֵ -- ԭʼ�༭ֵ(��10%����)��4
	fValueModFactor = (1.0f + FLOAT(IUTIL->Rand() % 21 - 10) / 100.0f) * pEquipQltyEffect->fPotFactor * (FLOAT)(pEquip->equipSpec.n16PotValModPct) / 10000.0f;
	pEquip->equipSpec.nPotVal = (INT)(pEquipProto->nPotVal * fValueModFactor);
	// Jason 2009-12-22 ����bug����Ʒ����ʱ���ᳬ�����Ǳ��ֵ�������ɫƷ��
	if( pEquip->equipSpec.nPotVal > pEquip->pEquipProto->nMaxPotVal )
		pEquip->equipSpec.nPotVal = pEquip->pEquipProto->nMaxPotVal;
	else if( pEquip->equipSpec.nPotVal < 0 )
		pEquip->equipSpec.nPotVal = 0;

	
	// �����[0,10000)
	INT32 nRandPct;
	
	// ��Ƕ������ -- �޿�(0%) 1��(40%) 2��(30%) 3��(20%) 4��(0%) 5��(0%)
	pEquip->equipSpec.byHoleNum = 0;

	INT32 nHolePctSum = 0;
	nRandPct = IUTIL->Rand() % 10000;
	for(INT32 i=MAX_EQUIPHOLE_NUM; i>0; --i)
	{
		nHolePctSum += pEquipQltyEffect->nHoleNumPct[i];
		if(nRandPct < nHolePctSum)
		{
			pEquip->equipSpec.byHoleNum = i;
			break;
		}
	}

	// ��������	-- 	"50%���ʳ��ֱ���������, 25%���ʳ������������"
	pEquip->equipSpec.dwLongHunInnerID	= GT_INVALID;
	pEquip->equipSpec.dwLongHunOuterID	= GT_INVALID;

	// .1����ֻ�����һ�� -- ������
	nRandPct = IUTIL->Rand() % 10000;
	if(nRandPct < pEquipQltyEffect->nLonghunPct[ELHT_Inner])
	{
		pEquip->equipSpec.dwLongHunInnerID = GenLongHunRandID(ELHT_Inner, pEquipProto->eEquipPos, eQlty);
	}
	else
	{
		nRandPct = IUTIL->Rand() % 10000;
		if(nRandPct < pEquipQltyEffect->nLonghunPct[ELHT_Outer])
		{
			pEquip->equipSpec.dwLongHunOuterID = GenLongHunRandID(ELHT_Outer, pEquipProto->eEquipPos, eQlty);
		}
	}

	// װ���������� -- 5%����1��B������
	nRandPct = IUTIL->Rand() % 10000;
	if(nRandPct < pEquipQltyEffect->nSpecAttPct)
	{
		nRandPct = IUTIL->Rand() % 10000;
		INT32 nSpecAttPctSum = 0;
		for(INT32 i=0; i<X_EQUIP_SPECATT_NUM; ++i)
		{
			nSpecAttPctSum += g_nEquipSpecAttPct[i];
			if(nRandPct < nSpecAttPctSum)
			{
				pEquip->equipSpec.bySpecAtt = i;
				break;
			}
		}

		if(pEquip->equipSpec.bySpecAtt <= (BYTE)EESA_EquipSpec_RelEnd)
		{
			// �����������Զ�װ�����Ե�Ӱ��
			ProcEquipAttBySpecAtt(pEquip);
		}
		else
		{
			// ��װ��ǿ��Ӱ�����������Ӱ�� -- ���������ʱ����
		}
	}
}

//-----------------------------------------------------------------------------
// �����������Զ�װ�����Ե�Ӱ��
//-----------------------------------------------------------------------------
VOID ItemCreator::ProcEquipAttBySpecAtt(tagEquip *pEquip)
{
	tagEquipSpec &equipSpec = pEquip->equipSpec;

	switch(equipSpec.bySpecAtt)
	{
	case EESA_LevelLim_Simple:
		// ����:��װ���ȼ�����-5����Ϳɼ�����0
		equipSpec.byMinUseLevel = (equipSpec.byMinUseLevel > 5 ? equipSpec.byMinUseLevel - 5 : 0);
		break;
	case EESA_LevelLim_Fine:
		// ����		��װ���ȼ�����-10����Ϳɼ�����0
		equipSpec.byMinUseLevel = (equipSpec.byMinUseLevel > 10 ? equipSpec.byMinUseLevel - 10 : 0);
		break;
	case EESA_LevelLim_None:
		// �޼���	��װ���޵ȼ�����
		equipSpec.byMinUseLevel = 0;
		break;

	case EESA_AttALim_Simple:
		// ���		��װ���������Ƽ���10%��ȡ��
		equipSpec.n16AttALimModPct = -1000;
		break;
	case EESA_AttALim_Comfort:
		// ����		��װ���������Ƽ���25%��ȡ��
		equipSpec.n16AttALimModPct = -2500;
		break;
	case EESA_AttALim_Light:
		// ��ӯ		��װ���������Ƽ���50%��ȡ��
		equipSpec.n16AttALimModPct = -5000;
		break;

	case EESA_Potential_YinFeng:
		// ����		��װ���ĳ�ʼǱ��ֵ+200
		equipSpec.nPotVal = min(equipSpec.nPotVal + 200, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_YinHuang:
		// ����		��װ���ĳ�ʼǱ��ֵ+400
		equipSpec.nPotVal = min(equipSpec.nPotVal + 400, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_FeiFeng:
		// �ɷ�		��װ���ĳ�ʼǱ��ֵ+800
		equipSpec.nPotVal = min(equipSpec.nPotVal + 800, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_MingHuang:
		// ����		��װ���ĳ�ʼǱ��ֵ+1200
		equipSpec.nPotVal = min(equipSpec.nPotVal + 1200, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_WoLong:
		// ����		װ���ĳ�ʼǱ��ֵ���5%
		equipSpec.nPotVal = (INT32)min(equipSpec.nPotVal * 1.05, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_CangLong:
		// ����		װ���ĳ�ʼǱ��ֵ���10%
		equipSpec.nPotVal = (INT32)min(equipSpec.nPotVal * 1.1, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_FuLong:
		// ����		װ���ĳ�ʼǱ��ֵ���20%
		equipSpec.nPotVal = (INT32)min(equipSpec.nPotVal * 1.2, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_ShengLong:
		// ����		װ���ĳ�ʼǱ��ֵ���30%
		equipSpec.nPotVal = (INT32)min(equipSpec.nPotVal * 1.3, pEquip->pEquipProto->nMaxPotVal);
		break;
	}
}

//-----------------------------------------------------------------------------
// ����ָ��Ʒ������ʱװ�������
//-----------------------------------------------------------------------------
BOOL ItemCreator::CreateFashionQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, 
									EItemQuality eQlty, const tagIMEffect *pIMEffect/* = NULL*/)
{
	ASSERT(eQlty > EIQ_Start && eQlty < EIQ_End);

	// ��ȡʱװ���������Դ
	const tagFashionGen *pGen = g_attRes.GetFashionQltyEffect(eQlty);
	if(!P_VALID(pGen))
	{
		MAttResCaution(_T("fashion_qlty_effect"), _T("Quality"), (INT)eQlty);
		return FALSE;
	}

	const tagFashionColorPct *pColor = g_attRes.GetFashionColorPct(eQlty);
	if(!P_VALID(pColor))
	{
		MAttResCaution(_T("fashion_color_pct"), _T("Quality"), (INT)eQlty);
		return FALSE;
	}

	// �����������
	INT nRand;
	
	// ��ɫ����
	INT nColor		= EC_Null;
	INT nColorPct	= 0;
	if(pIMEffect != NULL && EIME_Color == pIMEffect->eEffect)
	{
		nColor		= (INT)pIMEffect->dwParam1;
		nColorPct	= (INT)pIMEffect->dwParam2;
	}

	// .0ԭ��������ɫ������ԭ������ɫ
	if(pEquipProto->dwColor <= EC_End)
	{
		pEquip->equipSpec.n8ColorID = (INT8)pEquipProto->dwColor;
	}
	// .1���������Ƿ�Ϊ100%
	else if(10000 == nColorPct && nColor >= EC_Start && nColor <= EC_End)
	{
		pEquip->equipSpec.n8ColorID = nColor;
	}
	// .2������ɫ��������������ɫ
	else
	{
		INT nPctSum = 0;
		nRand = IUTIL->Rand() % 10000;
		pEquip->equipSpec.n8ColorID = EC_Val0;
		for(INT i=X_COLOR_NUM-1; i>=0; --i)
		{
			nPctSum += pColor->n16ColorPct[i];
			if(nColor == i)
			{
				nPctSum += nColorPct;
			}

			if(nRand < nPctSum)
			{
				pEquip->equipSpec.n8ColorID = i;
				break;
			}
		}
	}

	// ��װû�����Լӳ�
	if(EIQ_White == eQlty)
	{
		return TRUE;
	}

	// �������� -- ԭʼ�༭ֵ��1.5
	if(ERA_Appearance == pEquipProto->BaseEffect[0].eRoleAtt)
	{
		pEquip->equipSpec.n16Appearance = (INT16)(pEquipProto->BaseEffect[0].nValue * (pGen->fAppearanceFactor - 1));
	}

	// ͳ�����Լӳ� -- 10%���ʳ���, ֵ=��Ʒ�ȼ���5[ȡ��]
	nRand = IUTIL->Rand() % 10000;
	if(nRand < pGen->n16ReinPct && pGen->n8ReinVal > 0)
	{
		pEquip->equipSpec.byRein = pEquip->pEquipProto->byLevel / pGen->n8ReinVal;
	}

	// �������Լӳ� -- 10%���ʳ���, ֵ=��Ʒ�ȼ���20[ȡ��]
	nRand = IUTIL->Rand() % 10000;
	if(nRand < pGen->n16SavvyPct && pGen->n8SavvyVal > 0)
	{
		pEquip->equipSpec.bySavvy = pEquip->pEquipProto->byLevel / pGen->n8SavvyVal;
	}

	// ��Ե���Լӳ� -- 1%���ʳ���, ֵ=5�����¸���20%��+װ���ȼ�/30
	nRand = IUTIL->Rand() % 10000;
	if(nRand < pGen->n16FortunePct && pGen->n8FortuneVal2 > 0)
	{
		FLOAT fFactor = 1.0f + (FLOAT(IUTIL->Rand() % 41 - 20) / 100.0f);
		pEquip->equipSpec.byFortune 
			= (BYTE)(pGen->n8FortuneVal1 * fFactor + (DOUBLE)pEquipProto->byLevel / pGen->n8FortuneVal2);
	}

	// ʱװ�����ɿ���
	pEquip->equipSpec.bCanCut = FALSE;

	return TRUE;
}

tagItem* ItemCreator::CreateTreasure(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num /*= 1*/, DWORD dwCreator /*= GT_INVALID*/, INT16 n16QltyModPct /*= 0*/, INT16 n16QltyModPctEx /*= 0*/ )
{
	ASSERT(GT_VALID(dwNameID));
	tagItem* pNew = Create(eCreateMode, dwCreateID, dwTypeID, n16Num, dwCreator, n16QltyModPct, n16QltyModPctEx);
	if (P_VALID(pNew))
	{
		pNew->dwNameID = dwNameID;
	}	
	return pNew;
}

tagItem* ItemCreator::CreateTreasureEx(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, 
								 INT16 n16Num/* = 1*/, EItemQuality eQlty/* = EIQ_Null*/, DWORD dwCreator/* = GT_INVALID*/, 
								 const tagIMEffect *pIMEffect/* = NULL*/)
{
	ASSERT(GT_VALID(dwNameID));
	tagItem* pNew = CreateEx(eCreateMode, dwCreateID, dwTypeID, n16Num, eQlty, dwCreator, pIMEffect);
	if (P_VALID(pNew))
	{
		pNew->dwNameID = dwNameID;
	}
	return pNew;
}

BOOL ItemCreator::IsGMItemNoInit(tagItem* pTmpItem)
{
	ASSERT(P_VALID(pTmpItem));

	if (EICT_Baibao == pTmpItem->eConType 
		&& !GT_VALID(pTmpItem->dwCreateID)
		&& !GT_VALID(pTmpItem->dwCreatorID))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL ItemCreator::InitGMItem(tagItem* pTmpItem)
{
	// ��ʼ����Ʒ����

	// ��ʼ��װ������
	if (MIsEquipment(pTmpItem->dwTypeID))
	{
		MTRANS_POINTER(pEquip, pTmpItem, tagEquip);
		ItemCreator::IdentifyEquip(pEquip, EItemQuality(pEquip->equipSpec.byQuality));
	}

	return TRUE;
}
