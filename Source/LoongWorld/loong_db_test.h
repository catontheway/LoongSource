//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loong_db_test.h
// author: Sxg
// actor:
// data: 2008-12-5
// last:
// brief: ������Ϣ��LoongDB���������������ݿ⴦��Ч��
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/role_data.h"

//-----------------------------------------------------------------------------
// ��ȡ��ɫ(����: 1, 100, 10 -- ���: �ӽ�ɫidΪ1��100֮�������roledata���ж�ȡ10����¼)
//-----------------------------------------------------------------------------
DWORD WINAPI TestLoadRole(LPCTSTR strNamePostfix, LPCTSTR szNum, LPCTSTR szTestNum)
{
	INT nPostfix = _ttoi(strNamePostfix);
	INT nNum = _ttoi(szNum);
	INT nTestNum = _ttoi(szTestNum);

	if(nPostfix < 0 || nNum <= 0 || nTestNum <= 0)
	{
		return GT_INVALID;
	}
	
	tagNDBC_TestLoadRole send;

	while(nTestNum > 0)
	{
		send.dwAccountID = nPostfix + IUTIL->Rand()%nNum;
		send.dwRoleID = send.dwAccountID;

		g_dbSession.Send(&send, send.dwSize);

		--nTestNum;
	}
	
	return 0;
}

//-----------------------------------------------------------------------------
// ���½�ɫ(����: 1, 100, 10 -- ���: �ӽ�ɫidΪ1��100֮���������roledata����10����¼)
//-----------------------------------------------------------------------------
DWORD WINAPI TestUpdateRole(LPCTSTR strNamePostfix, LPCTSTR szNum, LPCTSTR szTestNum)
{
	INT nPostfix = _ttoi(strNamePostfix);
	INT nNum = _ttoi(szNum);
	INT nTestNum = _ttoi(szTestNum);

	if(nPostfix < 0 || nNum <= 0 || nTestNum <= 0)
	{
		return GT_INVALID;
	}
	
	tagNDBC_SaveRole *pSend = new tagNDBC_SaveRole;

	pSend->RoleData.LoginTime = 0;
	pSend->RoleData.LogoutTime = 0;
	pSend->RoleData.nOnlineTime = 0;
	pSend->RoleData.nCurOnlineTime = 0;

	ZeroMemory(&pSend->RoleData.AvatarEquip, SIZE_AVATAR_EQUIP);
	pSend->RoleData.DisplaySet = 0;
	pSend->RoleData.dwMapID = 3618849499;
	pSend->RoleData.dwRebornMapID = 3618849499;
	pSend->RoleData.fCoordinate[0] = 100.0f;
	pSend->RoleData.fCoordinate[1] = 100.0f;
	pSend->RoleData.fCoordinate[2] = 100.0f;
	pSend->RoleData.fFace[0] = 100.0f;
	pSend->RoleData.fFace[1] = 100.0f;
	pSend->RoleData.fFace[2] = 100.0f;

	pSend->RoleData.eClass = EV_Base;
	pSend->RoleData.eClassEx = EHV_Base;
	pSend->RoleData.nLevel = 1;
	pSend->RoleData.nCurExp = 0;
	pSend->RoleData.nHP = 100;
	pSend->RoleData.nMP = 100;
	pSend->RoleData.nAttPoint = 0;
	pSend->RoleData.nTalentPoint = 0;
	pSend->RoleData.nRage = 0;
	pSend->RoleData.nEndurance = 20;

	ZeroMemory(pSend->RoleData.nAttPointAdd, sizeof(pSend->RoleData.nAttPointAdd));

	pSend->RoleData.nVitality = 0;
	pSend->RoleData.nInjury = 0;
	pSend->RoleData.nKnowledge = 0;
	pSend->RoleData.nMorality = 0;
	pSend->RoleData.nCulture = 0;
	pSend->RoleData.nCredit = 0;
	pSend->RoleData.nIdentity = 0;
	pSend->RoleData.nVIPPoint = 0;

	pSend->RoleData.n16BagSize = 20;
	pSend->RoleData.nBagGold = 999;
	pSend->RoleData.nBagSilver = 9999;
	pSend->RoleData.nBagYuanBao = 100;

	pSend->RoleData.bSafeGuard = ERS_Safeguard;
	pSend->RoleData.CloseSafeGuardTime = 0;
	ZeroMemory(pSend->RoleData.talent, sizeof(pSend->RoleData.talent));

	/*********************************************************************************************************
	*�����б�״̬�б��ƺ��б���Ʒ�б�װ���б������б�����б������б�����������б�
	*��Ʒ��ȴʱ���
	*���밴˳��
	*********************************************************************************************************/
	LPVOID pData = pSend->RoleData.byData;

	// 1.�����б�
	pSend->RoleData.nSkillNum		= 0;

	// 2.״̬�б�
	pSend->RoleData.nBuffNum		= 0;

	// 3.�ƺ��б� -- ��: ʵʱ,����������Ϣ
	pSend->RoleData.nTitleNum		= 0;

	// 4.��Ʒ�б� 5.װ���б�
	pSend->RoleData.nItemNum		= 0;

	// 6.�����б� -- ��: ʵʱ,����������Ϣ
	pSend->RoleData.nFriendNum		= 0;

	// 7.����б� -- ��: ʵʱ,����������Ϣ
	pSend->RoleData.nEnemyNum		= 0;

	// 8.�����б�
	pSend->RoleData.nQuestNum		= 0;

	// 9.����������б� -- ��: ʵʱ,����������Ϣ
	pSend->RoleData.nQuestDoneNum	= 0;

	// 10.��Ʒ��ȴʱ��
	pSend->RoleData.nItemCDTimeNum	= 0;

	// ���¼�����Ϣ��С
	pSend->dwSize = (DWORD)((BYTE*)pData - (BYTE*)pSend);

	while(nTestNum > 0)
	{
		pSend->dwRoleID = nPostfix + IUTIL->Rand()%nNum;

		g_dbSession.Send(pSend, pSend->dwSize);

		--nTestNum;
	}

	delete pSend;

	return 0;
}

//-----------------------------------------------------------------------------
// ��ȡ��Ʒ&װ��(����: 1, 100, 10 -- ���: �ӽ�ɫidΪ1��100֮�������ȡ10����ɫ����Ʒ&װ��)
//-----------------------------------------------------------------------------
DWORD WINAPI TestLoadItem(LPCTSTR strNamePostfix, LPCTSTR szNum, LPCTSTR szTestNum)
{
	INT nPostfix = _ttoi(strNamePostfix);
	INT nNum = _ttoi(szNum);
	INT nTestNum = _ttoi(szTestNum);

	if(nPostfix < 0 || nNum <= 0 || nTestNum <= 0)
	{
		return GT_INVALID;
	}

	tagNDBC_TestLoadRoleItem send;

	while(nTestNum > 0)
	{
		send.dwAccountID = (nPostfix + IUTIL->Rand()%nNum)/10 * 10 + 1;
		send.dwRoleID = send.dwAccountID;

		g_dbSession.Send(&send, send.dwSize);

		--nTestNum;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ������Ʒ(����: 1, 100, 10, 999 -- ���:��SerialΪ1��100֮�����Ʒ�������ȡһ��,����typeidΪ999����Ʒ��Ϣ����)
//-----------------------------------------------------------------------------
DWORD WINAPI TestUpdateItem(LPCTSTR szSerialStart, LPCTSTR szNum, LPCTSTR szTestNum, LPCTSTR szItemTypeID)
{
	INT64	n64SerialStart = _ttoi64(szSerialStart);
	INT		nNum = _ttoi(szNum);
	INT		nTestNum = _ttoi(szTestNum);
	DWORD	dwTypeID	= (DWORD)_ttoi(szItemTypeID);

	if(n64SerialStart <= 0 || nNum <= 0 || nTestNum <= 0)
	{
		return GT_INVALID;
	}

	if(MIsEquipment(dwTypeID))
	{
		return GT_INVALID;
	}
	
	tagItem *pItem = ItemCreator::Create(EICM_Null, GT_INVALID, dwTypeID);
	if(!P_VALID(pItem))
	{
		ASSERT(P_VALID(pItem));
		return GT_INVALID;
	}

	pItem->n16Index = IUTIL->Rand()%10;
	
	while(nTestNum > 0)
	{
		pItem->n64Serial = n64SerialStart + IUTIL->Rand()%nNum;
		pItem->dwOwnerID = IUTIL->Rand()%50000;
		pItem->dwAccountID = pItem->dwOwnerID;
		pItem->eConType	= (EItemConType)(IUTIL->Rand()%5);

		tagNDBC_TestSaveItem send;
		IFASTCODE->MemCpy(&send.item, pItem, SIZE_ITEM);

		g_dbSession.Send(&send, send.dwSize);

		--nTestNum;
	}

	delete pItem;
	
	return 0;
}

//-----------------------------------------------------------------------------
// ����װ��(����: 1, 100, 10, 999 -- ���:��SerialΪ1��100֮�����Ʒ�������ȡһ��,����typeidΪ999��װ����Ϣ����)
//-----------------------------------------------------------------------------
DWORD WINAPI TestUpdateEquip(LPCTSTR szSerialStart, LPCTSTR szNum, LPCTSTR szTestNum, LPCTSTR szEquipTypeID)
{
	INT64	n64SerialStart = _ttoi64(szSerialStart);
	INT		nNum = _ttoi(szNum);
	INT		nTestNum = _ttoi(szTestNum);
	DWORD	dwTypeID	= (DWORD)_ttoi(szEquipTypeID);

	if(n64SerialStart <= 0 || nNum <= 0 || nTestNum <= 0)
	{
		return GT_INVALID;
	}

	if(!MIsEquipment(dwTypeID))
	{
		return GT_INVALID;
	}

	tagEquip *pEquip = (tagEquip*)ItemCreator::Create(EICM_Null, GT_INVALID, dwTypeID);
	if(!P_VALID(pEquip))
	{
		ASSERT(P_VALID(pEquip));
		return GT_INVALID;
	}

	while(nTestNum > 0)
	{
		ItemCreator::IdentifyEquip(pEquip);

		pEquip->n64Serial = n64SerialStart + IUTIL->Rand()%nNum;
		pEquip->n16Index = IUTIL->Rand()%10 + 10;

		tagNDBC_TestSaveEquip send;
		IFASTCODE->MemCpy(&send.equip, pEquip, SIZE_EQUIP);

		g_dbSession.Send(&send, send.dwSize);

		--nTestNum;
	}

	delete pEquip;

	return 0;
}