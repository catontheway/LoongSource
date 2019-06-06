//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongDBSave.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: ���ݿ����Ӧ�ò�ʵ�� -- �����������
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/vcard_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/clandata_define.h"
#include "../ServerDefine/item_define.h"
#include "../ServerDefine/activity_define.h"

#include "LoongDB.h"

//-------------------------------------------------------------------------------------------------------
// ɾ����Ʒװ����ض���
//-------------------------------------------------------------------------------------------------------
enum EDelItemTable
{
	EDIT_Item		= 0,	// item��
	EDIT_ItemBaiBao	= 1,	// item_baibao��
};

struct tagDelItem
{
	INT64	n64Serial;
	CHAR*	pTable;
};

struct tagNewItemEquip
{
	union
	{
		const tagItem*	pItem;
		const tagEquip* pEquip;
	};

	const CHAR*		pTable;
};

//#define MGET_TABLE(pTable, pDelItem)		\
//	char *pTable = "item";					\
//	if(EDIT_ItemBaiBao == pDelItem->eTable)	\
//		pTable = "item_baibao";

////-------------------------------------------------------------------------------------------------------
//// ɾ����ҵ�buff������item/equip��.�����������ǰ�����������ļ�¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::DeleteRecord(DWORD dwRoleID, tagRecordDel *pRecord)
//{
//	/*************************************************************************************
//	* ����ɾ��������ָ���ļ�¼: �����б�״̬�б���Ʒ�б�װ���б������б�
//	**************************************************************************************/
//	
//	BYTE *pData = pRecord->byData; // ��¼��ǰָ��λ��
//
//	// 1.�����б�
//	Update(dwRoleID, pData, pRecord->nSkillNum, sizeof(DWORD), (&CLoongDB::FormatDelRecSkill));
//	pData += pRecord->nSkillNum * sizeof(DWORD);
//
//	// 2.״̬�б�
//	Update(dwRoleID, pData, pRecord->nBuffNum, sizeof(DWORD), (&CLoongDB::FormatDelRecBuff));
//	pData += pRecord->nBuffNum * sizeof(DWORD);
//
//	//// 4.��Ʒ�б�
//	//Update(dwRoleID, pData, pRecord->nItemNum, sizeof(INT64), (&CLoongDB::FormatDelRecItem));
//	//pData += pRecord->nItemNum * sizeof(INT64);
//
//	//// 5.װ���б�
//	//Update(dwRoleID, pData, pRecord->nEquipNum, sizeof(INT64), (&CLoongDB::FormatDelRecEquip));
//	//pData += pRecord->nEquipNum * sizeof(INT64);
//
//	// 6.�����б�
//	Update(dwRoleID, pData, pRecord->nQuestNum, sizeof(DWORD), (&CLoongDB::FormatDelRecTask));
//	pData += pRecord->nQuestNum * sizeof(DWORD);
//}


//-------------------------------------------------------------------------------------------------------
// �洢��ɫ�����Ϣ -- ���輴ʱ�洢�ģ��������Ծ��ڴ˴���ʼ���洢����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveRole(DWORD dwRoleID, tagRoleDataSave *pRoleData)
{
	// �����ɫ����������Ϣ
	SaveRoleData(dwRoleID, pRoleData);

	/**************************************************************************
	* ���α���: �����б�״̬�б��ƺ��б���Ʒ�б�װ���б�
	*			�����б�����б������б�����������б�, ��Ʒ��ȴʱ���
	***************************************************************************/

	LPVOID pData = pRoleData->byData; // ��¼��ǰָ��λ��

	// 1.�����б�
	SaveSkill(dwRoleID, pData, pRoleData->nSkillNum, &pData);

	// 2.״̬�б�
	SaveRoleBuff(dwRoleID, pData, pRoleData->nBuffNum, &pData);

	// 4.�ƺ��б�
	SaveTitle(dwRoleID, pData, pRoleData->nTitleNum, &pData);

	// 5.��Ʒ�б� 6.װ���б� // ����Ϣ����
	//SaveItemEquip(dwRoleID, pData, pRoleData->nItemNum, &pData);
	UpdateItemPosAndUseRel(dwRoleID, pData, pRoleData->nItemNum, &pData);

	// 7.�����б� 
	// 8.����б� // ��2�����е������Ǽ�ʱ�����

	// 9.�����б�
	SaveTask(dwRoleID, pData, pRoleData->nQuestNum, &pData);

	// 10.����������б�
	//SaveTaskDone(dwRoleID, pData, pRoleData->nQuestDoneNum);

	// 11.��Ʒ��ȴʱ���
	ReplaceItemCDTime(dwRoleID, pData, pRoleData->nItemCDTimeNum, &pData);

	// 12.������������
	SaveClanData(dwRoleID, pData, pRoleData->bClanDataChg, &pData);

	// 13.�������
	SavePets(dwRoleID, pData, pRoleData->nPetsNum, &pData);

}


//********************************************************************************************************
//-------------------------------------------------------------------------------------------------------
// ��������Ʒ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertItem(const tagItem &item, const CHAR* pTable)
{
	tagNewItemEquip sNewItem;
	sNewItem.pItem	= &item;
	sNewItem.pTable	= pTable;
	return Update(GT_INVALID, (LPVOID)&sNewItem, 1, SIZE_ITEM, (&CLoongDB::FormatInsertItem));
}

//-------------------------------------------------------------------------------------------------------
// ������װ��ר������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertEquipSpec(const tagEquip &equip, const CHAR* pTable)
{
	tagNewItemEquip sNewItem;
	sNewItem.pEquip	= &equip;
	sNewItem.pTable	= pTable;
	return Update(GT_INVALID, (LPVOID)&sNewItem, 1, SIZE_EQUIP, (&CLoongDB::FormatInsertEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// ������װ�� -- װ��Ҫ����item���equip����Ҫ��֤���ݵ�������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertEquip(const tagEquip &equip, BOOL bBaiBao/* = FALSE*/)
{
	CHAR* pTableItem	= "item";
	CHAR* pTableEquip	= "equip";

	if(bBaiBao)
	{
		pTableItem	= "item_baibao";
		pTableEquip = "equip_baibao";
	}
	
	BOOL bRes = InsertItem(equip, pTableItem);
	if(!bRes)
	{
		return bRes;
	}

	bRes = InsertEquipSpec(equip, pTableEquip);
	if(!bRes)
	{
		// equip�����ʧ�ܣ���item����ɾ����Ӧ��¼
		DelItem(equip.n64Serial, pTableItem);
	}

	return bRes;
}

//-------------------------------------------------------------------------------------------------------
// ��Ʒ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveItem(const tagItem &item)
{
	return Update(GT_INVALID, (LPVOID)&item, 1, SIZE_ITEM, (&CLoongDB::FormatSaveItem));
}

//-------------------------------------------------------------------------------------------------------
// װ��ר������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveEquipSpec(const tagEquip &equip)
{
	return Update(GT_INVALID, (LPVOID)&equip, 1, SIZE_EQUIP, (&CLoongDB::FormatSaveEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// װ��ר������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateEquipSpec(const tagEquipSpecUpdate &equipSpecUpdate)
{
	return Update(GT_INVALID, (LPVOID)&equipSpecUpdate, 1, sizeof(tagEquipSpecUpdate), (&CLoongDB::FormatUpdateEquipSpec));
}


//-------------------------------------------------------------------------------------------------------
// װ�� -- װ��Ҫ����item���equip����Ҫ��֤���ݵ�������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveEquip(const tagEquip &equip)
{
	BOOL bRes = SaveItem(equip);
	if(!bRes)
	{
		return bRes;
	}

	bRes = SaveEquipSpec(equip);
	if(!bRes)
	{
		// equip�����ʧ�ܣ���item����ɾ����Ӧ��¼
		//DelItem(equip.n64Serial);//??
	}

	return bRes;
}

//-------------------------------------------------------------------------------------------------------
// ɾ����Ʒ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelItem(INT64 n64Serial, CHAR *pTable/* = "item"*/)
{
	tagDelItem delItem;
	delItem.n64Serial	= n64Serial;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelItem));
}

//-------------------------------------------------------------------------------------------------------
// ��װ������ɾ��װ��ר������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelEquipSpec(INT64 n64Serial, CHAR *pTable/* = "equip"*/)
{
	tagDelItem delItem;
	delItem.n64Serial	= n64Serial;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// ɾ��װ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelEquip(INT64 n64Serial)
{
	if ( !(SaveDelEquipInfo(n64Serial) && SaveDelItemInfo(n64Serial)) )
		return FALSE;	

	if(DelItem(n64Serial))
	{
		return DelEquipSpec(n64Serial);
	}

	return FALSE;
}

BOOL CLoongDB::SaveDelEquipInfo(INT64 n64Serial)
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	ASSERT(P_VALID(pStream));
	ASSERT(P_VALID(pCon));

	// ɾ������
	pStream->SetCopyItem("equip_del", "*", "equip");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���������
	m_pDBLoong->ReturnConnection(pCon);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::SaveDelItemInfo(INT64 n64Serial)
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	ASSERT(P_VALID(pStream));
	ASSERT(P_VALID(pCon));

	// ɾ������
	pStream->SetCopyItem("item_del", "*", "item");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���������
	m_pDBLoong->ReturnConnection(pCon);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveRoleData(DWORD dwRoleID, const tagRoleDataSave *pRoleData)
{
	return Update(dwRoleID, (LPVOID)pRoleData, 1, GT_INVALID, (&CLoongDB::FormatSaveRoleData));
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveClanData(DWORD dwRoleID, IN LPVOID pData, BOOL bChange, OUT LPVOID* ppOutPointer)
{
	if (!bChange)
	{
		*ppOutPointer = pData;
		return TRUE;
	}
	*ppOutPointer = reinterpret_cast<BYTE*>(pData) + sizeof(tagDBReputeData);
	return Update(dwRoleID, pData, 1, sizeof(tagDBReputeData), (&CLoongDB::FormatUpdateClanData));
}


//-------------------------------------------------------------------------------------------------------
// ������Ʒ��Ϣ
//-------------------------------------------------------------------------------------------------------
//BOOL CLoongDB::SaveItemEquip(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
//{
//	tagItem *pItem;
//	BYTE	*pCurPointer = (BYTE*)pData;
//	for(INT32 i=0; i<nNum; ++i)
//	{
//		pItem = (tagItem*)pCurPointer;
//		if(MIsEquipment(pItem->dwTypeID))
//		{
//			SaveEquip(*(tagEquip*)pItem);
//			pCurPointer += SIZE_EQUIP;
//		}
//		else
//		{
//			SaveItem(*pItem);
//			pCurPointer += SIZE_ITEM;
//		}
//	}
//
//	*ppOutPointer = pCurPointer;
//
//	return TRUE;
//}

//-------------------------------------------------------------------------------------------------------
// ������Ʒλ�ü�ʹ���������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateItemPosAndUseRel(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagItemUpdate), (&CLoongDB::FormatUpdateItemPosAndUseRel), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertSkill(DWORD dwRoleID, tagSkillSave *pData)
{
	return Update(dwRoleID, pData, 1, sizeof(tagSkillSave), (&CLoongDB::FormatInsertSkill));
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelSkill(DWORD dwRoleID, DWORD dwSkillID)
{
	return Update(dwRoleID, &dwSkillID, 1, sizeof(DWORD), (&CLoongDB::FormatDelSkill));
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSkill(DWORD dwRoleID, tagSkillSave *pData)
{
	LPVOID pSkillData = pData;
	return SaveSkill(dwRoleID, pSkillData, 1, &pSkillData);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveSkill(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagSkillSave), (&CLoongDB::FormatSaveSkill), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫbuff��Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveRoleBuff(DWORD dwRoleID, LPVOID pBuff, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	// ��ɾ��buff
	Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelAllRoleBuff));

	BYTE		*pCurPointer = (BYTE*)pBuff;
	tagBuffSave *pBuffSave;
	for(INT32 i=0; i<nNum; ++i)
	{
		pBuffSave = (tagBuffSave *)pCurPointer;
		Update(dwRoleID, pBuffSave, 1, sizeof(tagBuffSave), (&CLoongDB::FormatSaveRoleBuff));
		pCurPointer = pBuffSave->byData + pBuffSave->n8ModifierNum * sizeof(DWORD);
	}

	*ppOutPointer = pCurPointer;
	
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ����ƺ���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTitle(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagTitleSave), (&CLoongDB::FormatSaveTitle), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertTask(DWORD dwRoleID, LPVOID pAcceptQuest)
{
	return Update(dwRoleID, pAcceptQuest, 1, sizeof(tagAcceptQuest), (&CLoongDB::FormatInsertTask));
}


//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTask(DWORD dwRoleID, LPVOID pTask, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pTask, nNum, sizeof(tagQuestSave), (&CLoongDB::FormatSaveTask), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// ���������������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTaskDone(DWORD dwRoleID, LPVOID pTask, INT32 nNum)
{
	return Update(dwRoleID, pTask, nNum, sizeof(tagQuestDoneSave), (&CLoongDB::FormatSaveTaskDone));
}

//-------------------------------------------------------------------------------------------------------
// ������ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertVCard(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatInsertVCard));
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateVCard(DWORD dwRoleID, const PVOID pData)
{
	return Update(dwRoleID, pData, 1, sizeof(tagVCardData), (&CLoongDB::FormatUpdateVCard));
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelVCard(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelVCard));
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertClanData(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatInsertClanData));
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
// BOOL CLoongDB::UpdateClanData(DWORD dwRoleID, const PVOID pData)
// {
// 	return Update(dwRoleID, pData, 1, sizeof(tagDBReputeData), (&CLoongDB::FormatUpdateClanData));
// }

//-------------------------------------------------------------------------------------------------------
// ɾ��������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelClanData(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelClanData));
}

BOOL CLoongDB::SaveActivityData(DWORD dwID, LPVOID pActivityData)
{
	return Update(dwID, pActivityData, 1, sizeof(tagActivityData), (&CLoongDB::FormatSaveActivity));
}

//********************************************************************************************************
//-------------------------------------------------------------------------------------------------------
// ��������Ʒ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(pEx, pData, tagNewItemEquip);
	const tagItem *p = pEx->pItem;

	pStream->SetInsert(pEx->pTable);

	pStream->FillString("SerialNum=") << p->n64Serial;
	pStream->FillString(",TypeID=") << p->dwTypeID;

	pStream->FillString(",Num=") << p->n16Num;
	pStream->FillString(",Bind=") << p->byBind;
	pStream->FillString(",LockStat=") << p->bLock;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	
	pStream->FillString(",CreateMode=") << p->eCreateMode;
	pStream->FillString(",CreateID=") << p->dwCreateID;

	pStream->FillString(",CreatorID=") << p->dwCreatorID;
	pStream->FillString(",NameID=") << p->dwNameID;

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwCreateTime);
	pStream->FillString(",CreateTime='").FillString(m_szTime);
	pStream->FillString("'");

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dw1stGainTime);
	pStream->FillString(",FirstGainTime='").FillString(m_szTime);
	pStream->FillString("'");

	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->eConType;
	pStream->FillString(",Suffix=") << p->n16Index;
}

//-------------------------------------------------------------------------------------------------------
// ��ʽ��װ������ͨ�ò���(insert,update)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatCommonEquipSpec(Beton::MyStream *pStream, const tagEquipSpec *pEquipSpec)
{
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->FillString("Quality=") << pEquipSpec->byQuality;
	pStream->FillString(",MinUseLevel=") << pEquipSpec->byMinUseLevel;
	pStream->FillString(",MaxUseLevel=") << pEquipSpec->byMaxUseLevel;
	pStream->FillString(",AttALimMod=") << pEquipSpec->n16AttALimMod;
	pStream->FillString(",AttALimModPct=") << pEquipSpec->n16AttALimModPct;

	pStream->FillString(",WuHun=") << pEquipSpec->n16WuHun;
	pStream->FillString(",MinDmg=") << pEquipSpec->n16MinDmg;
	pStream->FillString(",MaxDmg=") << pEquipSpec->n16MaxDmg;
	pStream->FillString(",Armor=") << pEquipSpec->n16Armor;
	pStream->FillString(",PotVal=") << pEquipSpec->nPotVal;
	pStream->FillString(",PotValUsed=") << pEquipSpec->nPotValUsed;
	// Jason 2009-12-1װ��Ǳ��ֵ����������Ĭ��0
	pStream->FillString(",PotIncTimes=") << pEquipSpec->nPotIncTimes;
	// Jason 2009-12-2 ����װ����ɫƷ���������ʣ�Ĭ��0
	pStream->FillString  (",PurpleQltyIdfPct=") << pEquipSpec->dwPurpleQltyIdfPct;

	pStream->FillString(",PosyTimes=") << pEquipSpec->byPosyTimes;
	pStream->FillString(",EngraveTimes=") << pEquipSpec->byEngraveTimes;
	pStream->FillString(",HoleNum=") << pEquipSpec->byHoleNum;
	pStream->FillString(",BrandLevel=") << pEquipSpec->byBrandLevel;
	pStream->FillString(",LongFuLevel=") << pEquipSpec->byLongfuLevel;

	pStream->FillString(",CanCut=") << pEquipSpec->bCanCut;
	pStream->FillString(",LongInnerID=") << pEquipSpec->dwLongHunInnerID;
	pStream->FillString(",LongOuterID=") << pEquipSpec->dwLongHunOuterID;
	pStream->FillString(",SpecAtt=") << pEquipSpec->bySpecAtt;
	pStream->FillString(",FlareVal=") << pEquipSpec->byFlareVal;
	pStream->FillString(",QltyModPct=") << pEquipSpec->n16QltyModPct;
	pStream->FillString(",QltyModPctEx=") << pEquipSpec->n16QltyModPctEx;
	pStream->FillString(",PotValModPct=") << pEquipSpec->n16PotValModPct;

	// ʱװ����
	pStream->FillString(",Appearance=") << pEquipSpec->n16Appearance;
	pStream->FillString(",Rein=") << pEquipSpec->byRein;
	pStream->FillString(",Savvy=") << pEquipSpec->bySavvy;
	pStream->FillString(",Fortune=") << pEquipSpec->byFortune;
	pStream->FillString(",ColorID=") << pEquipSpec->n8ColorID;

	pStream->FillString(",RoleAttEffect='").FillBlob(pEquipSpec->nRoleAttEffect, sizeof(pEquipSpec->nRoleAttEffect), pCon);
	pStream->FillString("',PosyEffect='").FillBlob(pEquipSpec->PosyEffect, sizeof(pEquipSpec->PosyEffect), pCon);
	pStream->FillString("',EngraveAtt='").FillBlob(pEquipSpec->nEngraveAtt, sizeof(pEquipSpec->nEngraveAtt), pCon);
	pStream->FillString("',HoleGemID='").FillBlob(pEquipSpec->dwHoleGemID, sizeof(pEquipSpec->dwHoleGemID), pCon);
	pStream->FillString("',DerateVal='").FillBlob(pEquipSpec->nBrandAtt, sizeof(pEquipSpec->nBrandAtt), pCon);
	pStream->FillString("',WuXing='").FillBlob(pEquipSpec->byLongfuAtt, sizeof(pEquipSpec->byLongfuAtt), pCon);
	pStream->FillString("'");

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ������װ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNewItemEquip);

	pStream->SetInsert(p->pTable);
	
	pStream->FillString("SerialNum=") << p->pEquip->n64Serial;
	pStream->FillString(",");
	FormatCommonEquipSpec(pStream, &p->pEquip->equipSpec);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ����Ʒ��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pItem)
{
	MTRANS_POINTER(p, pItem, tagItem);

	pStream->SetUpdate("item");

	//pStream->FillString("SerialNum=") << p->n64Serial;
	//pStream->FillString(",TypeID=") << p->dwTypeID;

	pStream->FillString("Num=") << p->n16Num;
	pStream->FillString(",Bind=") << p->byBind;
	pStream->FillString(",LockStat=") << p->bLock;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	
	//pStream->FillString(",CreateMode=") << p->eCreateMode;
	//pStream->FillString(",CreateID=") << p->dwCreateID;

	//pStream->FillString(",CreatorID=") << p->dwCreatorID;

	//DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwCreateTime);
	//pStream->FillString(",CreateTime='").FillString(m_szTime);
	//pStream->FillString("'");

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dw1stGainTime);
	pStream->FillString(",FirstGainTime='").FillString(m_szTime);
	pStream->FillString("'");

	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->eConType;
	pStream->FillString(",Suffix=") << p->n16Index;

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// ����װ����Ϣ -- �޸�equip��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEquip)
{
	MTRANS_POINTER(p, pEquip, tagEquip);

	pStream->SetUpdate("equip");
	FormatCommonEquipSpec(pStream, &p->equipSpec);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// ����װ������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEquipSpecUpdate)
{
	MTRANS_POINTER(p, pEquipSpecUpdate, tagEquipSpecUpdate);

	pStream->SetUpdate("equip");
	FormatCommonEquipSpec(pStream, &p->equipSpec);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItem);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItem);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}


//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveRoleData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	tagRoleDataSave *p = (tagRoleDataSave *)pData;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	
	pStream->SetUpdate("roledata");
	pStream->FillString("AvatarEquip='").FillBlob(&p->AvatarEquip, SIZE_AVATAR_EQUIP, pCon);
	pStream->FillString("',DisplaySet=")<< (BYTE)p->DisplaySet;
	pStream->FillString(",MapID=") << p->dwMapID;
	pStream->FillString(",X=") << p->fCoordinate[0];// ����
	pStream->FillString(",Y=") << p->fCoordinate[1];
	pStream->FillString(",Z=") << p->fCoordinate[2];
	pStream->FillString(",FaceX=") << p->fFace[0];// ����
	pStream->FillString(",FaceY=") << p->fFace[1];
	pStream->FillString(",FaceZ=") << p->fFace[2];

	pStream->FillString(",RebornMapID=") << p->dwRebornMapID;

	pStream->FillString(",Class=") << (INT32)p->eClass;
	pStream->FillString(",ClassEx=") << p->eClassEx;
	pStream->FillString(",Level=") << p->nLevel;
	pStream->FillString(",ExpCurLevel=") << p->nCurExp;

	pStream->FillString(",HP=") << p->nHP;
	pStream->FillString(",MP=") << p->nMP;
	pStream->FillString(",Rage=") << p->nRage;
	pStream->FillString(",Endurance=") << p->nEndurance;
	pStream->FillString(",Vitality=") << p->nVitality;
	pStream->FillString(",Injury=") << p->nInjury;
	pStream->FillString(",Knowledge=") << p->nKnowledge;
	pStream->FillString(",Morale=") << p->nMorale;
	pStream->FillString(",Morality=") << p->nMorality;
	pStream->FillString(",Culture=") << p->nCulture;
	pStream->FillString(",Hostility=") << p->nHostility;
	pStream->FillString(",Credit=") << p->nCredit;
	pStream->FillString(",Identity=") << p->nIdentity;
	pStream->FillString(",VIPPoint=") << p->nVIPPoint;

	pStream->FillString(",AttPtAvail=") << p->nAttPoint;
	pStream->FillString(",TalentPtAvail=") << p->nTalentPoint;

	pStream->FillString(",Strength=") << (p->Strength.m_dwHeroism + p->Strength.m_dwWit + p->Strength.m_dwErrantry + p->Strength.m_dwValor);

	pStream->FillString(",PhysiqueAdded=") << p->nAttPointAdd[ERA_Physique];
	pStream->FillString(",StrengthAdded=") << p->nAttPointAdd[ERA_Strength];
	pStream->FillString(",PneumaAdded=") << p->nAttPointAdd[ERA_Pneuma];
	pStream->FillString(",InnerforceAdded=") << p->nAttPointAdd[ERA_InnerForce];
	pStream->FillString(",TechniqueAdded=") << p->nAttPointAdd[ERA_Technique];
	pStream->FillString(",AgilityAdded=") << p->nAttPointAdd[ERA_Agility];

	pStream->FillString(",TalentType1=") << p->talent[0].eType;
	pStream->FillString(",TalentType2=") << p->talent[1].eType;
	pStream->FillString(",TalentType3=") << p->talent[2].eType;
	pStream->FillString(",TalentType4=") << p->talent[3].eType;
	pStream->FillString(",TalentVal1=") << p->talent[0].nPoint;
	pStream->FillString(",TalentVal2=") << p->talent[1].nPoint;
	pStream->FillString(",TalentVal3=") << p->talent[2].nPoint;
	pStream->FillString(",TalentVal4=") << p->talent[3].nPoint;

	pStream->FillString(",SGFlag=") << p->bSafeGuard;
	
	pStream->FillString(",BagSize=") << p->n16BagSize;		// ����
	pStream->FillString(",BagGold=") << p->nBagGold;
	pStream->FillString(",BagSilver=") << p->nBagSilver;
	pStream->FillString(",BagYuanBao=") << p->nBagYuanBao;
	pStream->FillString(",ExchangeVolume=") << p->nExVolume;

	//pStream->FillString(",WareSize=") << p->n16WareSize;		// �˺Ųֿ�
	//pStream->FillString(",WareGold=") << p->nWareGold;
	//pStream->FillString(",WareSilver=") << p->nWareSilver;
	//pStream->FillString(",WareYuanBao=") << p->nWareYuanBao;

	pStream->FillString(",TotalTax=") << p->nTotalTax;	// ��˰�ܶ�
	pStream->FillString(",RemoteOpenSet=") << (DWORD)p->sRemoteOpenSet;
	pStream->FillString(",CurTitleID=") << p->u16ActiveTitleID;

	if(DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->LoginTime))
	{
		pStream->FillString(",LoginTime='").FillString(m_szTime);
		pStream->FillString("'");

		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->LogoutTime);
		pStream->FillString(",LogoutTime='").FillString(m_szTime);
		pStream->FillString("'");
		
		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->CloseSafeGuardTime);
		pStream->FillString(",CloseSGTime='").FillString(m_szTime);
		pStream->FillString("'");

		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwTimeGetMallFree);
		pStream->FillString(",GetMallFreeTime='").FillString(m_szTime);
		pStream->FillString("'");
	}
	else
	{
		IMSG(_T("Error: Func DwordTime2DataTime() run error in CLoongDB::FormatSaveRoleData()!!!!!\r\n"));
		ASSERT(0);
	}

	pStream->FillString(",OnlineTime=") << p->nOnlineTime;
	pStream->FillString(",CurOnlineTime=") << p->nCurOnlineTime;

	pStream->FillString(",ScriptData='");
	pStream->FillBlob(p->Data.dwData, sizeof(DWORD)*ESD_Role, pCon);
	pStream->FillString("'");

	pStream->FillString(",StallLevel=") << p->byStallLevel;
	pStream->FillString(",StallDailyExp=") << p->nStallDailyExp;
	pStream->FillString(",StallCurExp=") << p->nStallCurExp;
	pStream->FillString(",StallLastTime=") << p->dwLastStallTime;

	pStream->FillString(",DeadUnSetSafeGuardCountDown=") << p->nDeadUnSetSafeGuardCountDown;
	pStream->FillString(",NeedPrisonRevive=") << p->bNeedPrisonRevive;

	pStream->FillString(",LoverID=") << p->dwLoverID;
	pStream->FillString(",HaveWedding=") << p->bHaveWedding;

	// Jason 2009-12-7 ���������͸�����ֶ�
	pStream->FillString(",LastLessingTime=") << (DWORD)p->timeLastLessing;
	pStream->FillString(",LastLessingLevel=") << p->nLessingLevel;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ������ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertVCard(Beton::MyStream *pStream, DWORD dwRoleID, PVOID pDum)
{
	ASSERT(!P_VALID(pDum));

	pStream->SetInsert("visiting_card");
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateVCard(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagVCardData);
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetUpdate("visiting_card");

	pStream->FillString("Level=") << (INT32)p->dwLevel;
	pStream->FillString(",Job=") << (INT32)p->dwJob;
	pStream->FillString(",MateRoleID=") << (INT32)p->dwMateRoleID;
	pStream->FillString(",FactionID=") << (INT32)p->dwFactionID;
	pStream->FillString(",Position=") << (INT32)p->dwPosition;

	pStream->FillString(",Visibility=") << p->customVCardData.bVisibility;
	pStream->FillString(",Sex=") << p->customVCardData.bySex;
	pStream->FillString(",Constellation=") << p->customVCardData.byConstellation;
	pStream->FillString(",ChineseZodiac=") << p->customVCardData.byChineseZodiac;
	pStream->FillString(",Area=") << p->customVCardData.byArea;
	pStream->FillString(",Province=") << p->customVCardData.byProvince;
	pStream->FillString(",City='").FillString(p->customVCardData.chCity, pCon).FillString("'");
	pStream->FillString(",HeadUrl='").FillString(reinterpret_cast<TCHAR *>(p->customVCardData.byData), pCon).FillString("'");
	pStream->FillString(",Signature='").FillString(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData[p->customVCardData.byHeadUrlSize])), pCon).FillString("'");

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelVCard(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));
	pStream->SetDelete("visiting_card");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ������ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));
	
	pStream->SetInsert("clan_data");
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	ASSERT(P_VALID(pData));
	MTRANS_POINTER(pDBRepData, pData, tagDBReputeData);

	pStream->SetUpdate("clan_data");

	pStream->FillString("RepXuanYuan=")		<< pDBRepData->m_nReputation[ECLT_XuanYuan];
	pStream->FillString(",ConXuanYuan=")	<< pDBRepData->m_nContribution[ECLT_XuanYuan];
	pStream->FillString(",ActCntXuanYuan=")	<< pDBRepData->m_n8ActCount[ECLT_XuanYuan];

	pStream->FillString(",RepShenNong=")	<< pDBRepData->m_nReputation[ECLT_ShenNong];
	pStream->FillString(",ConShenNong=")	<< pDBRepData->m_nContribution[ECLT_ShenNong];
	pStream->FillString(",ActCntShenNong=")	<< pDBRepData->m_n8ActCount[ECLT_ShenNong];

	pStream->FillString(",RepFuXi=")		<< pDBRepData->m_nReputation[ECLT_FuXi];
	pStream->FillString(",ConFuXi=")		<< pDBRepData->m_nContribution[ECLT_FuXi];
	pStream->FillString(",ActCntFuXi=")		<< pDBRepData->m_n8ActCount[ECLT_FuXi];

	pStream->FillString(",RepSanMiao=")		<< pDBRepData->m_nReputation[ECLT_SanMiao];
	pStream->FillString(",ConSanMiao=")		<< pDBRepData->m_nContribution[ECLT_SanMiao];
	pStream->FillString(",ActCntSanMiao=")	<< pDBRepData->m_n8ActCount[ECLT_SanMiao];

	pStream->FillString(",RepJiuLi=")		<< pDBRepData->m_nReputation[ECLT_JiuLi];
	pStream->FillString(",ConJiuLi=")		<< pDBRepData->m_nContribution[ECLT_JiuLi];
	pStream->FillString(",ActCntJiuLi=")	<< pDBRepData->m_n8ActCount[ECLT_JiuLi];

	pStream->FillString(",RepYueZhi=")		<< pDBRepData->m_nReputation[ECLT_YueZhi];
	pStream->FillString(",ConYueZhi=")		<< pDBRepData->m_nContribution[ECLT_YueZhi];
	pStream->FillString(",ActCntYueZhi=")	<< pDBRepData->m_n8ActCount[ECLT_YueZhi];

	pStream->FillString(",RepNvWa=")		<< pDBRepData->m_nReputation[ECLT_NvWa];
	pStream->FillString(",ConNvWa=")		<< pDBRepData->m_nContribution[ECLT_NvWa];
	pStream->FillString(",ActCntNvWa=")		<< pDBRepData->m_n8ActCount[ECLT_NvWa];

	pStream->FillString(",RepGongGong=")	<< pDBRepData->m_nReputation[ECLT_GongGong];
	pStream->FillString(",ConGongGong=")	<< pDBRepData->m_nContribution[ECLT_GongGong];
	pStream->FillString(",ActCntGongGong=")	<< pDBRepData->m_n8ActCount[ECLT_GongGong];

	pStream->FillString(",FameMask=")		<< pDBRepData->m_u16FameMask;
	pStream->FillString(",RepRstTimeStamp=")<< pDBRepData->m_dwtLastResetTime;

	pStream->SetWhere();
	pStream->FillString("RoleID=")			<< dwRoleID;

}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));

	pStream->SetDelete("clan_data");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateItemPosAndUseRel(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagItemUpdate);

	pStream->SetUpdate("item");
	pStream->FillString("Num=") << p->n16Num;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->byConType;
	pStream->FillString(",Suffix=") << p->n16Index;
	pStream->FillString(",Bind=") << p->byBindType;
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagSkillSave);

	pStream->SetInsert("skill");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",ID=") << p->dwID;
	pStream->FillString(",BiddenLevel=") << p->nLearnLevel;
	pStream->FillString(",SelfLevel=") << p->nSelfLevel;
	pStream->FillString(",Proficiency=") << p->nProficiency;
	pStream->FillString(",CoolDown=") << p->nCoolDown;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetDelete("skill");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and ID=") << (*(DWORD*)pData);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagSkillSave);

	pStream->SetReplace("skill");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",ID=") << p->dwID;
	pStream->FillString(",BiddenLevel=") << p->nLearnLevel;
	pStream->FillString(",SelfLevel=") << p->nSelfLevel;
	pStream->FillString(",Proficiency=") << p->nProficiency;
	pStream->FillString(",CoolDown=") << p->nCoolDown;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫbuff��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveRoleBuff(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pBuff)
{
	MTRANS_POINTER(p, pBuff, tagBuffSave);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetInsert("buff");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",SrcUnitID=") << p->dwSrcUnitID;
	pStream->FillString(",SrcSkillID=") << p->dwSrcSkillID;
	pStream->FillString(",ItemTypeID=") << p->dwItemTypeID;
	pStream->FillString(",ItemSerialID=") << p->n64Serial;

	pStream->FillString(",BuffID=") << p->dwBuffID;
	pStream->FillString(",CurTick=") << p->nPersistTick;
	pStream->FillString(",Level=") << p->n8Level;
	pStream->FillString(",CurLapTimes=") << p->n8CurLapTimes;

	if(p->n8ModifierNum > 0)
	{
		pStream->FillString(",EffectSkillID='");
		pStream->FillBlob(p->byData, p->n8ModifierNum * sizeof(DWORD), pCon);
		pStream->FillString("'");
	}

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ����ƺ���Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTitle(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagTitleSave);

	pStream->SetUpdate("title");
	pStream->FillString("Count=") << p->dwStateMark;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and ID=") << p->u16TitleID;
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTask(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTask)
{
	tagQuestSave *p = (tagQuestSave *)pTask;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetReplace("task");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",TaskFlag=") << p->dwQuestFlag;
	pStream->FillString(",StartTime=") << p->dwAcceptTime;

	for(INT32 i=0; i<QUEST_CREATURES_COUNT; ++i)
	{
		pStream->FillString(",Monster") << i;
		pStream->FillString("Num=") << (INT16)p->n16MonsterNum[i];
	}

	pStream->FillString(",ScriptData='");
	pStream->FillBlob(p->Data.dwData, sizeof(DWORD)*ESD_Quest, pCon);
	pStream->FillString("'");

	if(p->DynamicTarget.eTargetType != EQTT_NULL)
	{
		pStream->FillString(",DynamicTarget='");
		pStream->FillBlob(&p->DynamicTarget, sizeof(tagQuestDynamicTarget), pCon);
		pStream->FillString("'");
	}
	
	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ���������������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTaskDone(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTask)
{
	tagQuestDoneSave *p = (tagQuestDoneSave *)pTask;

	pStream->SetReplace("task_done");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",StartTime=") << p->dwStartTime;
	pStream->FillString(",Times=") << p->nTimes;
}

//-------------------------------------------------------------------------------------------------------
// ��״̬����ɾ��ָ����¼
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelAllRoleBuff(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDummy)
{
	pStream->SetDelete("buff");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ����̶���ű�����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveActivity(Beton::MyStream *pStream, DWORD dwID, LPVOID pActivityData)
{
	tagActivityData *p = (tagActivityData *)pActivityData;
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	pStream->SetReplace("activity");
	pStream->FillString("dwID=") << dwID;
	pStream->FillString(",ScriptData='");
	pStream->FillBlob(&p->Data.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	m_pDBLoong->ReturnConnection(pCon);
}
////-------------------------------------------------------------------------------------------------------
//// ����Ʒ����ɾ��ָ����¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	INT64 *p = (INT64 *)pData;
//
//	pStream->SetDelete("item");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and SerialNum=") << *p;
//}
//
////-------------------------------------------------------------------------------------------------------
//// ��װ������ɾ��ָ����¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecEquip(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	INT64 *p = (INT64 *)pData;
//
//	pStream->SetDelete("equip");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and SerialNum=") << *p;
//}
//
////-------------------------------------------------------------------------------------------------------
//// ���������ɾ��ָ����¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecTask(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	DWORD *p = (DWORD *)pData;
//
//	pStream->SetDelete("task");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and TaskID=") << *p;
//}

