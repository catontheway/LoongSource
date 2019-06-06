#include "stdafx.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ItemEvent.h"
#include "ToolTipCreator.h"
#include "ItemProtoData.h"
#include "..\WorldDefine\shop_define.h"

#define MIsLock(item)		false
#define MIsBind(item)		(((tagItem*)item)->byBind == EBS_Bind)
#define MIsUnvend(item)		!((tagItemProto*)item)->bCanSell
#define MIsCanUse(item)		GT_VALID(((tagItemProto*)item)->dwBuffID0)

Item::Item() :
    m_dwItemState(0),
    m_bLocked(false),
    m_dwlastItemStus(0)
{
}

Item::~Item()
{

}

/** \���캯��
	\param ��Ʒ�����ṹ
*/
Item::Item(tagItem Data)
{
	m_dwItemState = 0;
	m_dwlastItemStus = 0;
	m_bLocked = false;
	//!��������
	memcpy(&m_data, &Data, sizeof(tagItem));
	
	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(m_data.dwTypeID);
	if(!P_VALID(pProto))
		return;

	//!�洢��Ʒģ��
	m_pProto = pProto;
	
	//!��Ʒ״̬��ֵ
	m_dwItemState = 0x0;
	//!������Ʒ�����ƶ�
	if(EICT_Quest == m_data.eConType)
		SetMovable(FALSE);

	//!ֻ��װ���ſ��Դ�
	if(!MIsEquipment(m_data.dwTypeID))
		SetWearable(FALSE);
	
	//!û�й���ʹ��Ч���򲻿ɱ�ʹ��
	if(!MIsCanUse(m_pProto))
		SetUseable(FALSE);

	//!��������Ʒ���ܶ���
	if(MIsLock(&m_data))
		SetDroppable(FALSE);	
	
	//!ע���޷����۵���Ʒ������������Ʒ�޷�����
	if(MIsUnvend(m_pProto) || MIsLock(&m_data))
		SetVendible(FALSE);

	//!�Ѱ󶨻�����������Ʒ�޷�����
	if(MIsBind(&m_data) || MIsLock(&m_data))
		SetTradeable(FALSE);

	//!�ѵ���Ϊ1����ƷΪ���ɶѵ���Ʒ
	if(1 == m_pProto->n16MaxLapNum)
		SetStackable(FALSE);

}

/** \�������캯��
*/
Item::Item( const Item& Data)
{
	Item* pData = const_cast<Item*>(&Data);
	memcpy(&m_data, pData->GetData(), sizeof(tagItem));

	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(m_data.dwTypeID);
	if(!P_VALID(pProto))
		return;

	//!�洢��Ʒģ��
	m_pProto = pProto;

	//!��Ʒ״̬��ֵ
	m_dwItemState = 0x0;
	//!������Ʒ�����ƶ�
	if(EICT_Quest == m_data.eConType)
		SetMovable(FALSE);

	//!ֻ��װ���ſ��Դ�
	if(!MIsEquipment(m_data.dwTypeID))
		SetWearable(FALSE);

	//!û�й���ʹ��Ч���򲻿ɱ�ʹ��
	if(!MIsCanUse(m_pProto))
		SetUseable(FALSE);

	//!��������Ʒ���ܶ���
	if(MIsLock(&m_data))
		SetDroppable(FALSE);	

	//!ע���޷����۵���Ʒ������������Ʒ�޷�����
	if(MIsUnvend(m_pProto) || MIsLock(&m_data))
		SetVendible(FALSE);

	//!�Ѱ󶨻�����������Ʒ�޷�����
	if(MIsBind(&m_data) || MIsLock(&m_data))
		SetTradeable(FALSE);

	//!�ѵ���Ϊ1����ƷΪ���ɶѵ���Ʒ
	if(1 == m_pProto->n16MaxLapNum)
		SetStackable(FALSE);
}

/** \�Ƿ�����Ʒ
*/
BOOL Item::IsItem()
{
	BOOL bRet = P_VALID(m_data.dwTypeID) && P_VALID(m_pProto) && 
		m_data.dwTypeID == m_pProto->dwTypeID && P_VALID(m_pProto->pDisplayInfo); 
	return bRet;
}


//---------------------------------------------------
// ״̬�ж�
//--------------------------------------------------- 
VOID Item::SetOperable(BOOL bOperable)
{
	if (bOperable)//!�����Ƿ�ɲ���
	{
		m_dwItemState &= ~EIS_Inoperable; 
	}
	else
	{
		m_dwItemState |= EIS_Inoperable;
	}
}

BOOL Item::IsOperable()
{
	BOOL bRet = !(m_dwItemState & EIS_Inoperable);
	return bRet;
}

VOID Item::SetMovable(BOOL bMovable)
{
	if (bMovable)//!�����Ƿ���ƶ�
	{
		m_dwItemState &= ~EIS_Unmovable;
	} 
	else
	{
		m_dwItemState |= EIS_Unmovable;
	}
}

BOOL Item::IsMovable()
{
	//!�ж��Ƿ���ƶ���Ҫ�ж��Ƿ�ɲ���
	BOOL bRet = !(m_dwItemState & EIS_Unmovable);
	return IsOperable() && bRet;
}

VOID Item::SetWearable(BOOL bWearable)
{
	if (bWearable)//!�����Ƿ��װ��
	{
		m_dwItemState &= ~EIS_Unwearable;
	} 
	else
	{
		m_dwItemState |= EIS_Unwearable;
	}
}

BOOL Item::IsWearable()
{
	//!�ж��Ƿ��װ����Ҫ�ж��Ƿ�ɲ����Ϳ��ƶ�
	BOOL bRet = !(m_dwItemState & EIS_Unwearable);
	return IsOperable() && IsMovable() && bRet;
}

VOID Item::SetUseable(BOOL bUseable)
{
	if (bUseable)//!�����Ƿ��ʹ��
	{
		m_dwItemState &= ~EIS_Unuseable;
	} 
	else
	{
		m_dwItemState |= EIS_Unuseable;
	}
}

BOOL Item::IsUseable()
{
	//!�ж��Ƿ��ʹ����Ҫ�ж��Ƿ�ɲ���
	BOOL bRet = !(m_dwItemState & EIS_Unuseable);
	return IsOperable() && bRet;
}

VOID Item::SetDroppable(BOOL bDroppable)
{
	if (bDroppable)//!�����Ƿ�ɶ���
	{
		m_dwItemState &= ~EIS_Undroppable;
	} 
	else
	{
		m_dwItemState |= EIS_Undroppable;
	}
}

BOOL Item::IsDroppable()
{
	//!�ж��Ƿ�ɶ�����Ҫ�ж��Ƿ�ɲ����Ϳ��ƶ�
	BOOL bRet = !(m_dwItemState & EIS_Undroppable);
	return IsOperable() && IsMovable() && bRet;
}

VOID Item::SetVendible(BOOL bVendible)
{
	if (bVendible)//!�����Ƿ�ɳ���
	{
		m_dwItemState &= ~EIS_Unvendible;
	} 
	else
	{
		m_dwItemState |= EIS_Unvendible;
	}
}

BOOL Item::IsVendible()
{
	//!�ж��Ƿ�ɶ�����Ҫ�ж��Ƿ�ɲ����Ϳ��ƶ�
	BOOL bRet = !(m_dwItemState & EIS_Unvendible);
	return IsOperable() && IsMovable() && bRet;
}

VOID Item::SetTradeable(BOOL bTradeable)
{
	if (bTradeable)//!�����Ƿ�ɽ���
	{
		m_dwItemState &= ~EIS_Untradeable;
	} 
	else
	{
		m_dwItemState |= EIS_Untradeable;
	}
}

BOOL Item::IsTradeable()
{
	//!�ж��Ƿ�ɶ�����Ҫ�ж��Ƿ�ɲ����Ϳ��ƶ�
	BOOL bRet = !(m_dwItemState & EIS_Untradeable);
	return IsOperable() && IsMovable() && bRet;
}

VOID Item::SetStackable(BOOL bStackable)
{
	if (bStackable)//!�����Ƿ�ɶѵ�
	{
		m_dwItemState &= ~EIS_Unstackable;
	}
	else
	{
		m_dwItemState |= EIS_Unstackable;
	}
}

BOOL Item::IsStackable(INT16 &n16Quantity)
{
	BOOL bRet = !(m_dwItemState & EIS_Unstackable);
	n16Quantity = GetItemQuantity();
	return bRet;
}

tstring Item::GetItemTips()
{
	return ToolTipCreator::Inst()->GetItemTips(this);
}

void Item::Lock()
{
    // if has been locked, return, to avoid write last status
    if (m_bLocked)
        return;

    // save current item status
    m_dwlastItemStus = m_dwItemState;

    // set all status disable
    m_dwItemState = -1;

    m_bLocked = true;
}

void Item::UnLock()
{
    // if is unlocked, return
    if (!m_bLocked)
        return;

    // load last status
    m_dwItemState = m_dwlastItemStus;

    m_bLocked = false;
}
//----------------------------------------------------------
// װ��
//----------------------------------------------------------
Equipment::Equipment()
{

}

Equipment::~Equipment()
{

}

Equipment::Equipment(tagEquip Data)
{
	m_dwItemState = 0;
	m_dwlastItemStus = 0;
	m_bLocked = false;
	//!��������
	memcpy(&m_data, &Data, sizeof(tagItem));
	INT pos = sizeof(tagItem);
	memcpy(&m_equipex, (BYTE*)(&Data)+pos, sizeof(tagEquipSpec));

	const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto(m_data.dwTypeID);
	if(!P_VALID(pProto))
		return;
	
	//!�洢��Ʒģ��
	m_pProto = pProto; 

	//!��ȡװ�����Լӳ�
	LoadEquipAttribute();

	//!װ��״̬��ֵ
	m_dwItemState = 0x0;
	//!û�й���ʹ��Ч���򲻿ɱ�ʹ��
	if(!MIsCanUse(m_pProto))
		SetUseable(FALSE);

	//!��������Ʒ���ܶ���
	if(MIsLock(&m_data))
		SetDroppable(FALSE);	

	//!ע���޷����۵���Ʒ������������Ʒ�޷�����
	if(MIsUnvend(m_pProto) || MIsLock(&m_data))
		SetVendible(FALSE);

	//!�Ѱ󶨻�����������Ʒ�޷�����
	if(MIsBind(&m_data) || MIsLock(&m_data))
		SetTradeable(FALSE);

	//װ����Ʒ���ɶѵ�
	SetStackable(FALSE);
}

Equipment::Equipment( const Equipment& Data)
{
	Equipment* pData = const_cast<Equipment*>(&Data);
	memcpy(&m_data, pData->GetData(), sizeof(tagItem));
	memcpy(&m_equipex, pData->GetEquip(), sizeof(tagEquipSpec));

	const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto(m_data.dwTypeID);
	if(!P_VALID(pProto))
		return;

	//!�洢��Ʒģ��
	m_pProto = pProto;

	//!��ȡװ�����Լӳ�
	LoadEquipAttribute();

	//!װ��״̬��ֵ
	m_dwItemState = 0x0;
	//!û�й���ʹ��Ч���򲻿ɱ�ʹ��
	if(!MIsCanUse(m_pProto))
		SetUseable(FALSE);

	//!��������Ʒ���ܶ���
	if(MIsLock(&m_data))
		SetDroppable(FALSE);	

	//!ע���޷����۵���Ʒ������������Ʒ�޷�����
	if(MIsUnvend(m_pProto) || MIsLock(&m_data))
		SetVendible(FALSE);

	//!�Ѱ󶨻�����������Ʒ�޷�����
	if(MIsBind(&m_data) || MIsLock(&m_data))
		SetTradeable(FALSE);

	//!װ�����ɶѵ�
	SetStackable(FALSE);
}


BOOL Equipment::IsItem()
{
	BOOL bRet = Item::IsItem();
	return bRet;
}

void Equipment::UpdateEquipment( tagEquipSpec Data )
{
	//!��������
	memcpy(&m_equipex, (BYTE*)(&Data), sizeof(tagEquipSpec));
	
	//!���¶�ȡװ�����Լӳ�
	LoadEquipAttribute();
}

VOID Equipment::LoadEquipAttribute()
{
	m_mapEquipAtt.Clear();
	m_mapEquipAttPct.Clear();
	
	const tagEquipProto* pProto = (const tagEquipProto*)m_pProto;

	INT i;
	//--�ֶ���д������
	for(i=0; i<MAX_ROLEATT_BASE_EFFECT; ++i)
	{
		AddEquipAttribute(pProto->BaseEffect[i].eRoleAtt, pProto->BaseEffect[i].nValue);
	}
	//--���������һ������
	for(i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		AddEquipAttribute((ERoleAttribute)(ERA_AttA_Start + i), m_equipex.nRoleAttEffect[i]);
	}
	//--�Կ����ӵ�����
	for(i=0; i<MAX_ROLEATT_ENGRAVE_EFFECT; ++i)
	{
		AddEquipAttribute((ERoleAttribute)(ERA_EngraveAtt_Start + i), m_equipex.nEngraveAtt[i]);
	}
	//--�������ӵ�����
	for(i=0; i<MAX_ROLEATT_POSY_EFFECT; ++i)
	{
		AddEquipAttribute(m_equipex.PosyEffect[i].eRoleAtt, m_equipex.PosyEffect[i].nValue);
	}
	//--��ӡ���ӵ�����
	for(i=0; i<X_ERA_DERATE_NUM; ++i)
	{
		AddEquipAttribute((ERoleAttribute)(ERA_Derate_Start + i), m_equipex.nBrandAtt[i]);
	}
	//--ʱװ���ӵ�����
	AddEquipAttribute(ERA_Appearance, m_equipex.n16Appearance);
	AddEquipAttribute(ERA_Rein,		  m_equipex.byRein);
	AddEquipAttribute(ERA_Savvy,	  m_equipex.bySavvy);
	AddEquipAttribute(ERA_Fortune,	  m_equipex.byFortune);
}

VOID Equipment::AddEquipAttribute( ERoleAttribute eAtt, INT nInc )
{
	//--������Ϊ����������
	if(0 == nInc)
		return;

	if(MIsValuePct(nInc))
	{
		INT nValue = m_mapEquipAttPct.Peek(eAtt);
		if(m_mapEquipAttPct.IsExist(eAtt))
		{
			m_mapEquipAttPct.ChangeValue(eAtt, nValue + MValuePctTrans(nInc));
		}
		else 
		{
			m_mapEquipAttPct.Add(eAtt, MValuePctTrans(nInc));
		}
	}
	else
	{
		INT nValue = m_mapEquipAtt.Peek(eAtt);
		if(m_mapEquipAtt.IsExist(eAtt))
		{
			m_mapEquipAtt.ChangeValue(eAtt, nValue + nInc);
		}
		else 
		{
			m_mapEquipAtt.Add(eAtt, nInc);
		}
	}
}

INT Equipment::GetItemPrice()
{
	FLOAT fFactor;
	MCalPriceFactor(fFactor, m_equipex.byQuality);
	INT nPrice = (INT)(m_pProto->nBasePrice * fFactor);
	if(nPrice < 0)
	{
		ASSERT(0);
		nPrice = 0;
	}
	return nPrice;
}

INT16 Equipment::GetEquipAttLimit( ERoleAttribute eType )
{
	if(eType >= ERA_AttA_Start && eType <= ERA_AttA_End)
	{
		const tagEquipProto* pProto = (const tagEquipProto*)m_pProto;
		INT16 n16Ret  = pProto->n16AttALimit[eType];
		INT16 n16Mod  = m_equipex.n16AttALimMod;
		FLOAT fModPct = m_equipex.n16AttALimModPct / 10000.0f;
		n16Ret = (INT16)((n16Ret + n16Mod) * (1 + fModPct));
		return n16Ret;
	}
	return 0;
}
