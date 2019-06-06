//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_equip.cpp
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: ����װ��
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "pet_equip.h"

#include "../WorldDefine/msg_pet.h"
#include "../WorldDefine/msg_pet_equip.h"

#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_pet.h"

#include "pet_soul.h"
#include "pet_skill.h"
#include "pet_att.h"
#include "role.h"
#include "item_mgr.h"

//----------------------------------------------------------------------------------------------------
// ͨ��64λID��װ����Ʒ����Ʒ����ұ�����δ���Ƴ���
//----------------------------------------------------------------------------------------------------
DWORD PetEquipBar::Equip( INT64 n64ItemID, INT8 n8DstPos, BOOL bSend /*= FALSE*/ )
{
	INT16 n16DstPos = (INT16)n8DstPos;

 	MTRANS_ELSE_RET(pMaster, m_pSoul->GetMaster(), Role, E_Pets_PetEquip_Master_NotExist);

	// ��ұ����л�ȡ��Ʒ
	MTRANS_ELSE_RET(p2Equip, GetFromRoleBag(n64ItemID), tagItem, E_Pets_PetEquip_PetEquipNotInBag);

	// ���ԭ��
	MTRANS_ELSE_RET(p2EquipPetEquipProto, GetPetEquipProto(p2Equip), const tagPetEquipProto, E_Pets_PetEquip_UnknownEquipProto)

	// �������Ƴ�
	pMaster->GetItemMgr().TakeOutFromBag(n64ItemID, ELCLD_PET_ITEM_PET_EQUIP, TRUE);

	//����װ����ÿ��λ��װ��
	for (INT16 i=0; i<PetEquipContainer::GetCurSpaceSize(); ++i)
	{
		tagItem* pCurItem = PetEquipContainer::GetItem(i);
		if (!P_VALID(pCurItem)) 
			continue;

		const tagPetEquipProto* pCurProto = GetPetEquipProto(pCurItem);
		if (!P_VALID(pCurProto)) 
			continue;
		//����װ��ͬ����Ψһ�����λ��װ��������ͬ����װ��Ψһ�����λ��װ��typeid��ͬ
		if (p2EquipPetEquipProto->bTypeUnique && p2EquipPetEquipProto->nType == pCurProto->nType ||
			p2EquipPetEquipProto->bUnique && p2Equip->dwTypeID == pCurItem->dwTypeID)
		{
			//ж�ظ�λ��װ��
			UnEquip(pCurItem->GetKey(), GT_INVALID, TRUE);
		}
	}
		
	//��ָ��λ��
	if (GT_VALID(n16DstPos))
	{
		//Ŀ��λ�ò�����
		if (!PetEquipContainer::IsOnePlaceFree(n16DstPos))
		{
			//Ŀ��λ��жװ
			tagItem* pItem = PetEquipContainer::GetItem(n16DstPos);
			if (P_VALID(pItem))
			{
				UnEquip(pItem->GetKey(), GT_INVALID, TRUE);
			}
		}
	}
	//����ָ��λ��
	else
	{
		//Ŀ��λ������Ϊ�ҵ�һ������λ��
		if (PetEquipContainer::GetFreeSpaceSize() != 0)
			for (INT16 i=0; i<PetEquipContainer::GetCurSpaceSize(); ++i)
			{
				if (!PetEquipContainer::IsOnePlaceFree(i)) continue;
				n16DstPos = i;
				break;
			}
		else
		{
			n16DstPos = 0;
			tagItem* pCurItem = PetEquipContainer::GetItem(n16DstPos);
			ASSERT(P_VALID(pCurItem)) ;
			DWORD dwRtv = UnEquip(pCurItem->GetKey(), GT_INVALID, TRUE);
			ASSERT(dwRtv == E_Success);
		}
	}

	// ��׼��
	if (n16DstPos > SIZE_PET_EQUIP_BAR || n16DstPos < 0)
	{
		n16DstPos = 0;
	}
	
	AddEquip(p2Equip, n16DstPos);

	// ���뵽���ݿ�
	InsertDB(p2Equip);
	p2Equip->SetUpdate(EUDBS_Null);

	if (bSend)
	{
		// ͬ���ͻ���
		tagNS_PetEquip send;
		send.dwErrCode = E_Success;
		send.dwPetID = m_pSoul->GetID();	
		FillClientEquipData(&send.itemData, p2Equip);
		pMaster->SendMessage(&send, send.dwSize);
	}
 	return E_Pets_Success;
}

//----------------------------------------------------------------------------------------------------
// ͨ��64λID��жװ����Ʒ���뵽���񱳰�
//----------------------------------------------------------------------------------------------------
DWORD PetEquipBar::UnEquip( INT64 n64ItemID, INT16 n16DstPos, BOOL bSend /*= FALSE*/ )
{
	MTRANS_ELSE_RET(pMaster, m_pSoul->GetMaster(), Role, E_Pets_PetEquip_Master_NotExist);

	//��ָ��λ��
	if (GT_VALID(n16DstPos))
	{
		//Ŀ��λ�ò�����
		if (!pMaster->GetItemMgr().IsBagOneSpaceFree(n16DstPos))
		{
			//���ش���
			return E_Pets_PetEquip_BagIndexNotFree;
		}
	}
	//����ָ��λ��
	else
	{
		//�ҵ�����Ŀ��λ��
		n16DstPos = m_pSoul->GetMaster()->GetItemMgr().GetBagOneFreeSpace();
		//���޿���λ��
		if (!GT_VALID(n16DstPos))
		{
			//���ش���	
			return E_Pets_PetEquip_BagIndexNotFree;
		}
	}

	// �ӳ���װ�����Ƴ�
	tagItem* p2UnEquip = PetEquipContainer::Remove(n64ItemID);

	//��ʱ�޹������Ƴ�
	if(!P_VALID(p2UnEquip)) return E_Pets_PetEquip_BagIndexNotFree;

	if (p2UnEquip->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(p2UnEquip->n64Serial);
	}	

	// �����ݿ��Ƴ�
	RemoveDB(p2UnEquip);
	p2UnEquip->SetUpdate(EUDBS_Null);


	// ����Ч��
	CalcEffect(m_pSoul, p2UnEquip->pProtoType->nSpecFuncVal1, FALSE, TRUE);

	DWORD dwRtv = pMaster->GetItemMgr().Add2BagByIndexAndInsertDB(p2UnEquip, ELCLD_PET_ITEM_PET_EQUIP, n16DstPos);
	if (E_Pets_Success != dwRtv)
	{
		::Destroy(p2UnEquip);
	}

	// ͬ���ͻ���
	if (E_Success == dwRtv && bSend)
	{
		tagNS_PetUnequip send;
		send.n64ItemID	= p2UnEquip->GetKey();
		send.dwErrCode = E_Success;
		pMaster->SendMessage(&send, send.dwSize);
	}

	return dwRtv ;
}

//----------------------------------------------------------------------------------------------------
// ����װ��Ч��
//----------------------------------------------------------------------------------------------------
VOID PetEquipBar::CalcEffect(PetSoul* pSoul, DWORD dwPetEquipTypeID, BOOL bEquip, BOOL bSend)
{
	const tagPetEquipProto* pProto = g_attRes.GetPetEquipProto(dwPetEquipTypeID);
	if (!P_VALID(pProto) || !P_VALID(pSoul))
	{
		ASSERT(0);
		return ;
	}

	// �Ƿ�жװ
	INT nCoef = bEquip ? 1 : -1;

	std::list<PetSkill*> listSkills;

	// ����Ч��
	for (INT i=0; i<MAX_PET_EQUIP_MOD_ATT && GT_VALID(pProto->nPetAtt[i]); ++i)
	{
		INT nPetAtt = pProto->nPetAtt[i];
		listSkills.clear();
		m_pSoul->ExportSpecSkill(nPetAtt, listSkills);

		DisablePetSkill(listSkills);
		pSoul->GetPetAtt().ModAttVal(nPetAtt, nCoef * pProto->nPetAttMod[i], bSend);
		EnablePetSkill(listSkills);
	}
}

//----------------------------------------------------------------------------------------------------
// ���캯��
//----------------------------------------------------------------------------------------------------
PetEquipBar::PetEquipBar(PetSoul* pSoul) 
	:PetEquipContainer(EICT_PetEquip, SIZE_PET_EQUIP_BAR, SIZE_PET_EQUIP_BAR), m_TimeLimitMgr(PET_EQUIP_UPDATE_TIME), m_pSoul(pSoul)
{
	ASSERT(P_VALID(m_pSoul));
}

//----------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------
VOID PetEquipBar::Update()
{
	m_TimeLimitMgr.Update();
	TList<INT64>& list2Del = m_TimeLimitMgr.GetNeedDelList();
	while (!list2Del.Empty())
	{
		// ����Ʒ���ڣ���򵥵İ��Ƴ������������ﱳ��
		INT64 n64ItemID = list2Del.PopFront();
		if(IsEquipExist(n64ItemID))
			UnEquip(n64ItemID, GT_INVALID, TRUE);
	}
}

//----------------------------------------------------------------------------------------------------
// �����ﱳ���л�ȡ����װ��
//----------------------------------------------------------------------------------------------------
tagItem* PetEquipBar::GetFromRoleBag( INT64 n64ItemID )
{
	MTRANS_ELSE_RET(pMaster, m_pSoul->GetMaster(), Role, NULL);
	tagItem* pToGet = pMaster->GetItemMgr().GetBagItem(n64ItemID);
	if (!P_VALID(pToGet))
	{
		ASSERT(0);
	}
	return pToGet;
}

//----------------------------------------------------------------------------------------------------
// �ӳ������ϻ�ȡ����װ��
//----------------------------------------------------------------------------------------------------
tagItem* PetEquipBar::GetFromPet( INT64 n64ItemID )
{
	tagItem* pToGet = PetEquipContainer::GetItem(n64ItemID);
	if (!P_VALID(pToGet))
	{
		ASSERT(0);	
	}
	return pToGet;
}

//----------------------------------------------------------------------------------------------------
// ��ȡ�ͻ���װ���ṹ
//----------------------------------------------------------------------------------------------------
VOID PetEquipBar::FillClientEquipData( tagPetEquipMsgInfo* pMsgInfo, tagItem* pItem)
{
	if (P_VALID(pItem))
	{
		pMsgInfo->n64ItemID		= pItem->GetKey();
		pMsgInfo->dwItemProtoID	= pItem->dwTypeID;
		pMsgInfo->n8Pos			= (INT8)pItem->n16Index;
	}
}

//----------------------------------------------------------------------------------------------------
// ����װ��λ�ý���
//----------------------------------------------------------------------------------------------------
DWORD PetEquipBar::SwapEquipPos( INT64 n64ItemID, INT8 n8DstPos, BOOL bSend /*= FALSE*/ )
{
	
	INT16 n16DstPos = (INT16)n8DstPos;
	tagItem* pItem = GetFromPet( n64ItemID);
	if (!P_VALID(pItem))
	{
		return E_Pets_PetEquip_EquipIsNotOnPet;
	}else if (n16DstPos < 0 || n16DstPos >= PetEquipContainer::GetCurSpaceSize())
	{
		return E_Pets_PetEquip_InvalidDstPos;
	}
	else if (pItem->n16Index == n16DstPos)
	{
		return E_Pets_PetEquip_EquipAlreadyInPos;
	}

	INT16 n16SrcPos = pItem->n16Index;

	if (PetEquipContainer::IsOnePlaceFree(n16DstPos))
	{
		// ֱ�ӷŵ����λ��
		tagItem* pItem = PetEquipContainer::Remove(n64ItemID);
		ASSERT(P_VALID(pItem));
		PetEquipContainer::Add(pItem, n16DstPos);
	}
	else
	{
		// ����
		PetEquipContainer::Swap(pItem->n16Index, n16DstPos);
	}

	if (bSend)
	{
		tagNS_PetEquipPosSwap send;
		send.dwErrCode = E_Success;
		send.n64ItemID = pItem->GetKey();
		send.n8PosSrc = (INT8)n16SrcPos;
		send.n8PosDst = (INT8)pItem->n16Index;
		m_pSoul->GetMaster()->SendMessage(&send, send.dwSize);
	}

	return E_Pets_Success;
}

//----------------------------------------------------------------------------------------------------
// �����ݿ�
//----------------------------------------------------------------------------------------------------
VOID PetEquipBar::SaveToDB( OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum )
{
	nNum = 0;

	MTRANS_POINTER(pCurPointer, pData, tagItemUpdate);

	tagItem	*pTemp	= NULL;
	for(INT16 i=0; i<PetEquipContainer::GetCurSpaceSize(); ++i)
	{
		pTemp = PetEquipContainer::GetItem(i);
		if(P_VALID(pTemp) && pTemp->eStatus != EUDBS_Null)
		{
			pCurPointer[nNum].n64Serial		= pTemp->n64Serial;
			pCurPointer[nNum].dwOwnerID		= pTemp->dwOwnerID;
			pCurPointer[nNum].nUseTimes		= pTemp->nUseTimes;
			pCurPointer[nNum].n16Num		= pTemp->n16Num;
			pCurPointer[nNum].n16Index		= pTemp->n16Index;
			pCurPointer[nNum].byConType		= pTemp->eConType;

			pTemp->SetUpdate(EUDBS_Null);

			++nNum;
		}
	}

	pOutPointer = &pCurPointer[nNum];
}

//----------------------------------------------------------------------------------------------------
// �������ݿ�
//----------------------------------------------------------------------------------------------------
VOID PetEquipBar::InsertDB( tagItem* p2Equip )
{
	tagNDBC_NewItem send;
	IFASTCODE->MemCpy(&send.item, p2Equip, SIZE_ITEM);
	g_dbSession.Send(&send, send.dwSize);
}

//----------------------------------------------------------------------------------------------------
// �����ݿ��Ƴ�
//----------------------------------------------------------------------------------------------------
VOID PetEquipBar::RemoveDB( tagItem* p2UnEquip )
{
	tagNDBC_DelItem send;
	send.n64Serial = p2UnEquip->n64Serial;
	g_dbSession.Send(&send, send.dwSize);
}

//----------------------------------------------------------------------------------------------------
// ��ȡ����װ��ԭ��
//----------------------------------------------------------------------------------------------------
const tagPetEquipProto* PetEquipBar::GetPetEquipProto( tagItem* pItem )
{
	if (!P_VALID(pItem)) return NULL;
	MTRANS_ELSE_RET(pItemProto, pItem->pProtoType, const tagItemProto, NULL);
	if (EISF_PetEquipment != pItemProto->eSpecFunc) return NULL;
	return g_attRes.GetPetEquipProto(pItem->pProtoType->nSpecFuncVal1);
}

//----------------------------------------------------------------------------------------------------
// ���װ��
//----------------------------------------------------------------------------------------------------
VOID PetEquipBar::AddEquip( tagItem* p2Equip, INT16 n16DstPos, BOOL bOnLoad)
{
	// ���ԭ��
	MTRANS_ELSE_RET(p2EquipPetEquipProto, GetPetEquipProto(p2Equip), const tagPetEquipProto, );

	//��ӵ�װ����
	PetEquipContainer::Add(p2Equip, n16DstPos);
	p2Equip->SetOwner(m_pSoul->GetID(), GT_INVALID);

	// ����ʱ����Ʒ
	if (p2Equip->IsTimeLimit())
	{
		m_TimeLimitMgr.Add2TimeLeftList(p2Equip->n64Serial, p2Equip->pProtoType->dwTimeLimit, p2Equip->dw1stGainTime);
	}	

	// ������ƷЧ��
	CalcEffect(m_pSoul, p2EquipPetEquipProto->dwTypeID, TRUE, !bOnLoad);
}

BOOL PetEquipBar::HasEquip()
{
	return  PetEquipContainer::GetMaxSpaceSize() - PetEquipContainer::GetFreeSpaceSize() != 0;
}

VOID PetEquipBar::DisablePetSkill( std::list<PetSkill*> &listSkill)
{
	if (!m_pSoul->IsCalled())
	{
		return;
	}
	std::list<PetSkill*>::iterator itr = listSkill.begin();
	while (itr != listSkill.end())
	{
		PassiveSkill* pPassive = dynamic_cast<PassiveSkill*>(*itr);
		if (P_VALID(pPassive))
		{
			pPassive->DeActive();
		}
		EnhanceSkill* pEnhance = dynamic_cast<EnhanceSkill*>(*itr);
		if (P_VALID(pEnhance))
		{
			pEnhance->Close();
		}
		++itr;
	}
}

VOID PetEquipBar::EnablePetSkill( std::list<PetSkill*> &listSkill)
{
	if (!m_pSoul->IsCalled())
	{
		return;
	}
	std::list<PetSkill*>::iterator itr = listSkill.begin();
	while (itr != listSkill.end())
	{
		PassiveSkill* pPassive = dynamic_cast<PassiveSkill*>(*itr);
		if (P_VALID(pPassive))
		{
			pPassive->Active();
		}
		EnhanceSkill* pEnhance = dynamic_cast<EnhanceSkill*>(*itr);
		if (P_VALID(pEnhance))
		{
			pEnhance->Open();
		}
		++itr;
	}
}

BOOL PetEquipBar::IsEquipExist( INT64 n64Id )
{
	//����װ����ÿ��λ��װ��
	for (INT16 i=0; i<PetEquipContainer::GetCurSpaceSize(); ++i)
	{
		tagItem* pCurItem = PetEquipContainer::GetItem(i);
		if (P_VALID(pCurItem)) 
			return TRUE;
	}

	return FALSE;
}