//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongDBLoad.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: ���ݿ����Ӧ�ò�ʵ�� -- ��ȡ�������
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/famehall_define.h"
#include "../WorldDefine/mall_define.h"

#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_vcard.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../ServerDefine/msg_strength.h"
#include "../ServerDefine/activity_define.h"
#include "../ServerDefine/strength_define.h"

#include "LoongDB.h"
#include "../Beton/Field.h"

//-------------------------------------------------------------------------------------------------------
// ��ȡ���ĳ����ɫȫ����Ϣ,�ڴ����ⲿ����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadRole(DWORD dwAccountID, DWORD dwRoleID, tagRoleDataLoad *pRoleData)
{
	// ��ʼ����ɫ -- �������ڴ�����˳�ʼ��
	ZeroMemory(pRoleData, sizeof(tagRoleDataLoad));

	LPVOID pData = pRoleData;
	
	// ��roledata���ж�ȡ�������
	if(!LoadRoleData(pData, dwAccountID, dwRoleID))
	{
		pRoleData->dwSize = 0;
		return FALSE;
	}

	DWORD dwIndexes[50] = {0};
	INT nMsgNum = 0;
	LoadLeftMsgIndexes(dwIndexes, dwRoleID, nMsgNum);
	if (nMsgNum > 0)
	{
		pRoleData->RoleDataSave.bHasLeftMsg = TRUE;		
	}
	else
	{
		pRoleData->RoleDataSave.bHasLeftMsg = FALSE;
	}

	/**************************************************************************
	* ���ζ�ȡ: �����б�״̬�б��ƺ��б���Ʒ�б�װ���б�
	*			�����б�����б������б�����������б�, ��Ʒ��ȴʱ���
	*			�Ѻöȱ�, ��������
	***************************************************************************/

	tagRoleDataSave *pDataSave = &pRoleData->RoleDataSave;
	pData = pDataSave->byData; // ��¼��ǰָ��λ��
	
	// 1.�����б�
	LoadSkill(pData, pDataSave->nSkillNum, dwRoleID);
//	pData += pDataSave->nSkillNum * sizeof(tagSkill);

	// 2.״̬�б�
	LoadRoleBuff(pData, pDataSave->nBuffNum, dwRoleID);
//	pData += pDataSave->nBuffNum * sizeof(tagBuffSave);

	// 4.�ƺ��б�
	LoadTitle(pData, pDataSave->nTitleNum, dwRoleID);
//	pData += pDataSave->nTitleNum * sizeof(tagTitleSave);
	
	// 5.��Ʒ�б� + 6.װ���б�
	LoadItem(pData, pDataSave->nItemNum, dwAccountID, dwRoleID);
//	pData += pDataSave->nItemNum * sizeof(tagItem);

	// 7.�����б�
	LoadFriend(pData, pDataSave->nFriendNum, dwRoleID);
//	pData += pDataSave->nFriendNum * sizeof(DWORD);
	
	// 8.����б�
	LoadEnemy(pData, pDataSave->nEnemyNum, dwRoleID);
//	pData += pDataSave->nEnemyNum * sizeof(DWORD);
	
	// 9.�����б�
	LoadTask(pData, pDataSave->nQuestNum, dwRoleID);
//	pData += pDataSave->nQuestNum * sizeof(tagQuestSave);

	// 10.����������б�
	LoadTaskDone(pData, pDataSave->nQuestDoneNum, dwRoleID);
//	pData += pDataSave->nQuestDoneNum * sizeof(tagQuestDoneSave);

	// 11.��Ʒ��ȴʱ���
	LoadItemCDTime(pData, pDataSave->nItemCDTimeNum, dwRoleID);

	// 12.�Ѻöȱ�
	LoadFriendship(pData, pDataSave->nFriendshipNum, dwRoleID);
	INT nFriendNumEx = 0;
	LoadFriendshipEx(pData, nFriendNumEx, dwRoleID);
	pDataSave->nFriendshipNum += nFriendNumEx;

	// 13.��������
	LoadBlackList(pData, pDataSave->nBlackNum, dwRoleID);

	// 14 ��ɫ����
	LoadVCard(pData, dwRoleID);

	// 15 ��ȡ��������
	if(!LoadClanData(pData,dwRoleID))
	{
		pRoleData->dwSize = 0;
		return FALSE;
	}

	// 16 ��ȡ����
	if (!LoadPets(pData, dwRoleID, pDataSave->nPetsNum))
	{
		pRoleData->dwSize = 0;
		return FALSE;
	}

	// �����ȡ���ݴ�С
	pRoleData->dwSize = (sizeof(tagRoleDataLoad) - 1) + ((BYTE*)pData - pDataSave->byData);
	
	return TRUE;
}


//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ҽ�����Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllRoleInfo(LPVOID pData, INT &nRoleNum)
{
	return Load(pData, &nRoleNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadAllRoleInfo, &CLoongDB::ProcResLoadAllRoleInfo);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡĳ���ʺŵ����н�ɫ������Ϣ,�ڴ����ⲿ����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadSimRole(DWORD dwAccountID, INT &nRoleNum, LPVOID pSimRole)
{
	return Load(pSimRole, &nRoleNum, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadSimRole, &CLoongDB::ProcResLoadSimRole);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadRoleData(LPVOID &pData, DWORD dwAccountID, DWORD dwRoleID)
{
	return Load(pData, NULL, dwAccountID, dwRoleID, &CLoongDB::FormatLoadRoleData, &CLoongDB::ProcResLoadRoleData);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadSkill(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadSkill, &CLoongDB::ProcResLoadSkill);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫbuff��Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadRoleBuff(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadRoleBuff, &CLoongDB::ProcResLoadRoleBuff);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ƺ���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadTitle(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadTitle, &CLoongDB::ProcResLoadTitle);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ��Ʒ&װ����Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadItem(LPVOID &pData, INT32 &nNum, DWORD dwAccountID, DWORD dwRoleID)
{
	return Load(pData, &nNum, dwAccountID, dwRoleID, &CLoongDB::FormatLoadItem, &CLoongDB::ProcResLoadItem);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadFriend(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadFriend, &CLoongDB::ProcResLoadFriend);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�����Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadEnemy(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadEnemy, &CLoongDB::ProcResLoadEnemy);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadTask(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadTask, &CLoongDB::ProcResLoadTask);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�����������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadTaskDone(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadTaskDone, &CLoongDB::ProcResLoadTaskDone);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��������������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadSimTaskDone(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadSimTaskDone, &CLoongDB::ProcResLoadSimTaskDone);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ʒ��ȴʱ����Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadItemCDTime(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadItemCDTime, &CLoongDB::ProcResLoadItemCDTime);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�Ѻö���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadFriendship(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadFriendship, &CLoongDB::ProcResLoadFriendship);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�Ѻö���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadFriendshipEx(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadFriendshipEx, &CLoongDB::ProcResLoadFriendship);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�Ѻö���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadBlackList(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadBlackList, &CLoongDB::ProcResLoadBlackList);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadVCard(LPVOID &pData, DWORD dwRoleID)
{
	INT32 nNum = 0;
	BOOL bRtv = Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadVCard, &CLoongDB::ProcResLoadVCard);
// 	if (0 == nNum)
// 	{
// 		InsertVCard(dwRoleID);
// 		bRtv = Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadVCard, &CLoongDB::ProcResLoadVCard);
// 		ASSERT(1 == nNum);
// 	}
	return bRtv;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadClanData(LPVOID &pData, DWORD dwRoleID)
{
	INT32 nNum = 0;
	BOOL bRtv = Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadClanData, &CLoongDB::ProcResLoadClanData);
	if (1 != nNum)
	{
		InsertClanData(dwRoleID);
		bRtv = Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadClanData, &CLoongDB::ProcResLoadClanData);
		ASSERT(1 == nNum);
	}
	return bRtv;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ�����Ʒ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadItemBaiBao(DWORD dwAccountID, INT32 nNumLoad, INT64 n64Serial, INT32 &nNumLoaded, INT32 &nDataSize, LPVOID pData)
{
	ResetLoadNumLim(nNumLoad);
	ResetLoadSerial(n64Serial);

	LPVOID	pTmpData = pData;
	BOOL	bRet = Load(pTmpData, &nNumLoaded, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadItemBaiBao, &CLoongDB::ProcResLoadItem);

	if(bRet)
	{
		nDataSize = (INT32)((BYTE*)pTmpData - (BYTE*)pData);
	}

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ���Ԫ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadBaiBaoYuanBao(DWORD dwAccountID, INT32& nYuanBao)
{
	LPVOID pData = (LPVOID)&nYuanBao;
	return Load(pData, NULL, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadBaiBaoYuanBao, &CLoongDB::ProcResLoadBaiBaoYuanBao);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ�����ƷLog
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadBaibaoLogInfo(DWORD dwAccountID, INT32 &nNumLoaded, INT32 &nDataSize, LPVOID pData)
{
	LPVOID	pTmpData = pData;
	BOOL	bRet = Load(pData, &nNumLoaded, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadBaiBaoLog, &CLoongDB::ProcResLoadBaiBaoLog);

	if(bRet)
	{
		nDataSize = (INT32)((BYTE*)pData - (BYTE*)pTmpData);
	}

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ����һ���ٱ���Log��Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertBaibaoLogInfo(DWORD dwAccountID, LPVOID pData)
{
	if (!GT_VALID(dwAccountID))
		return FALSE;

	// �жϼ�¼��������, ɾ������ļ�¼
	Load(pData, NULL, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadBaiBaoLog, &CLoongDB::ProcResCountBaiBaoLog);

	// �����¼
	return Update(dwAccountID, pData, 1, GT_INVALID, &CLoongDB::FormatInsertBaiBaoLog);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���log����ƷTypeID
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadItemNeedLog(LPVOID pTypeID, INT32 &nItemNum)
{
	return Load(pTypeID, &nItemNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadItemNeedLog, &CLoongDB::ProcResLoadItemNeedLog);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�������Ԫ�������˻���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllYBAccount(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadYBAccount, &CLoongDB::ProcResLoadYBAccount);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������ҵ�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllYBOrder(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadYBOrder, &CLoongDB::ProcResLoadYBOrder);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡһ����ҵ�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadRoleAllYBOrder(LPVOID pData, INT&nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadRoleYBOrder, &CLoongDB::ProcResLoadRoleOrder);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���а����Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllGPInfo(OUT PVOID pData, INT32 &nInfoNum)
{
	return Load(pData, &nInfoNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadAllGPInfo, &CLoongDB::ProcResLoadAllGPInfo);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�̶������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadActivityData(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatActivityData, &CLoongDB::ProcResLoadActivityData);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡʵ�����а�
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadStrengthRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatStrengthRankings, &CLoongDB::ProcResLoadStrengthRankings);
}

//-------------------------------------------------------------------------------------------------------
// ����ʵ�����а�
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateStrengthRankings()
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	
	if ( !P_VALID(pStream) || !P_VALID(pCon) )
	{
		return FALSE;
	}

	// ��roladata��ȡ������ֶΣ�д��strength_rankings����(����֮ǰ�������)
	pStream->SetEmpty("strength_rankings");
	m_pDBLoong->Execute(pStream);
	pStream->SetCopyItem("strength_rankings(PreNum,Strength,RoleName)", "StrengthNum,Strength,RoleName", "roledata");
	m_pDBLoong->Execute(pStream);

	// strength_rankings����Strengthֵ�Ľ�������
	pStream->SetSelect("strength_rankings", "*");
	pStream->SetOrder("Strength", 1);
	
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if (!P_VALID(pResult))
	{
		// �ͷ���������
		m_pDBLoong->ReturnConnection(pCon);
		m_pDBLoong->ReturnStream(pStream);
		return FALSE;
	}

	INT nCount = pResult->GetRowCount();
	if(nCount > 0)
	{
		for(int i = 0; i < nCount; ++i)
		{
			const char* szRoleName = (*pResult)[3].GetString();

			pStream->SetUpdate("strength_rankings");
			pStream->FillString("Num=")<<i+1;
			pStream->SetWhere();
			pStream->FillString("RoleName='").FillString(szRoleName).FillString("'");
			m_pDBLoong->Execute(pStream);

			pResult->NextRow();
		}
	}
	m_pDBLoong->FreeQueryResult(pResult);

	// ȥ��StrengthֵС��100�ģ���ȡǰ500��
	pStream->SetDelete("strength_rankings");
	pStream->SetWhere();
	pStream->FillString("Strength<100");
	m_pDBLoong->Execute(pStream);

	pStream->SetDelete("strength_rankings");
	pStream->SetWhere();
	pStream->FillString("Num>500");
	m_pDBLoong->Execute(pStream);

	// ��roledata�и���ɫ��StrengthNumֵ��0
	pStream->SetUpdate("roledata");
	pStream->FillString("StrengthNum=0");
	m_pDBLoong->Execute(pStream);

	// ��Ŀǰ��strength_rankings�а����Ľ�ɫ������ֵд��ý�ɫ��roledata�е�StrengthNum�ֶ�
	
	pStream->SetSelect("strength_rankings","Num,RoleName");

	pResult = m_pDBLoong->Query(pStream);
	INT nNum = pResult->GetRowCount();

	if(P_VALID(pResult) && nNum > 0)
	{
		Beton::MyStream* pStreamF = m_pDBLoong->GetStream();
		Beton::Connection* pConF = m_pDBLoong->GetFreeConnection();

		for( INT i = 0; i < nNum; i++)
		{
			INT nStrengthNum = (*pResult)[0].GetInt();
			const char* szRoleName = (*pResult)[1].GetString();
		
			pStreamF->SetUpdate("roledata");
			pStreamF->FillString("StrengthNum=") << nStrengthNum;
			pStreamF->SetWhere();
			pStreamF->FillString("RoleName='").FillString(szRoleName).FillString("'");
			m_pDBLoong->Execute(pStreamF);

			pResult->NextRow();
		}

		// �ͷ���������
		m_pDBLoong->ReturnConnection(pConF);
		m_pDBLoong->ReturnStream(pStreamF);
	}

	// �ͷ���������
	m_pDBLoong->ReturnConnection(pCon);
	m_pDBLoong->ReturnStream(pStream);

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����VIP̯λ��Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::GetAllVIPStallInfo(OUT PVOID pData)
{
	INT32 nVIPStallNum = 0;

	// ���µ����ݿ�
	Load(pData, &nVIPStallNum, GT_INVALID, GT_INVALID, (&CLoongDB::FormatGetAllVIPStallInfo), (&CLoongDB::ProcResGetAllVIPStallInfo));

	if (nVIPStallNum < VIP_STALL_MAX_NUM)
	{
		tagVipStall2DB* pInfo = (tagVipStall2DB*)pData;

		for (int n=0; n<VIP_STALL_MAX_NUM; n++)
		{
			if (pInfo[n].byStallIndex == (BYTE)GT_INVALID)
			{
				pInfo[n].byStallIndex	= n;
				pInfo[n].dwOwnerID		= GT_INVALID;
				pInfo[n].nRemainTime	= GT_INVALID;
				Update(GT_INVALID, &pInfo[n], 1, GT_INVALID, (&CLoongDB::FormatInsertVIPStall));
			}
		}
	}

	return TRUE;
}

//**********************************************************************************************************

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ҽ�����ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadAllRoleInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("roledata", "AccountID,RoleID,RoleName,RoleNameCrc,Sex,MapID,Level,LogoutTime,Class,LoverID");
	pStream->SetWhere();
	pStream->FillString("RemoveFlag=") << 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ҽ�����Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadAllRoleInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRoleInfo, pData, tagRoleInfo);
	*pNum = pRes->GetRowCount();

	// RoleID,RoleName,RoleNameCrc,Sex,MapID,Level
	for(INT i=0; i<*pNum; ++i)
	{
		(*pRes)[2].GetTCHAR(pRoleInfo[i].szRoleName, X_SHORT_NAME);
		pRoleInfo[i].dwAccountID = (*pRes)[0].GetDword();
		pRoleInfo[i].dwRoleID = (*pRes)[1].GetDword();
		pRoleInfo[i].dwRoleNameCrc = (*pRes)[3].GetDword();
		pRoleInfo[i].bySex = (*pRes)[4].GetByte();
		pRoleInfo[i].dwMapID = (*pRes)[5].GetDword();
		pRoleInfo[i].byLevel = (*pRes)[6].GetByte();
		DataTime2DwordTime(pRoleInfo[i].dwTimeLastLogout, (*pRes)[7].GetString(), (*pRes)[7].GetLen());
		pRoleInfo[i].byClass = (*pRes)[8].GetByte(); //ְҵ
		pRoleInfo[i].dwLoverID = (*pRes)[9].GetDword(); //����
		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�������Ԫ�������˻�SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadYBAccount(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("ybaccount", "RoleID,YuanBao,Gold,Silver,bSellOrder,bBuyOrder");
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�������Ԫ�����׶���SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadYBOrder(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("yuanbaoorder", "dwID,RoleID,OrderType,Price,Number,OrderMode,StartTime,EndTime,AvgPrice,DealNum");
	pStream->SetWhere();
	pStream->FillString("OrderMode=") << 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡһ�����Ԫ�����׶���SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadRoleYBOrder(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("yuanbaoorder", "dwID,RoleID,OrderType,Price,Number,OrderMode,StartTime,EndTime,AvgPrice,DealNum");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�������Ԫ�������˻������鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadYBAccount(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pYBAccount, pData, tagYBAccount);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pYBAccount[i].dwRoleID = (*pRes)[0].GetDword();
		pYBAccount[i].nYuanBao = (*pRes)[1].GetDword();
		pYBAccount[i].nGold = (*pRes)[2].GetInt();
		pYBAccount[i].nSilver = (*pRes)[3].GetInt();
		pYBAccount[i].bSellOrder = (*pRes)[4].GetBool();
		pYBAccount[i].bBuyOrder = (*pRes)[5].GetBool();

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�������Ԫ�����׶��������鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadYBOrder(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pYBOrder, pData, tagYuanBaoOrder);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pYBOrder[i].dwID = (*pRes)[0].GetDword();
		pYBOrder[i].dwRoleID = (*pRes)[1].GetDword();
		pYBOrder[i].eYBOType = (EYBOTYPE)(*pRes)[2].GetInt();
		pYBOrder[i].nPrice = (*pRes)[3].GetInt();
		pYBOrder[i].nNum = (*pRes)[4].GetInt();
		pYBOrder[i].eYBOMode = (EYBOMODE)(*pRes)[5].GetInt();
		pYBOrder[i].dwStartTime = (*pRes)[6].GetDword();
		pYBOrder[i].dwEndTime =  (*pRes)[7].GetDword();
		pYBOrder[i].nAvgPrice = (*pRes)[8].GetInt();
		pYBOrder[i].nDealNum = (*pRes)[9].GetInt();

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡһ�����Ԫ�����׶��������鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadRoleOrder(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pYBOrder, pData, tagYuanBaoOrder);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pYBOrder[i].dwID = (*pRes)[0].GetDword();
		pYBOrder[i].dwRoleID = (*pRes)[1].GetDword();
		pYBOrder[i].eYBOType = (EYBOTYPE)(*pRes)[2].GetInt();
		pYBOrder[i].nPrice = (*pRes)[3].GetInt();
		pYBOrder[i].nNum = (*pRes)[4].GetInt();
		pYBOrder[i].eYBOMode = (EYBOMODE)(*pRes)[5].GetInt();
		pYBOrder[i].dwStartTime = (*pRes)[6].GetDword();
		pYBOrder[i].dwEndTime =  (*pRes)[7].GetDword();
		pYBOrder[i].nAvgPrice = (*pRes)[8].GetInt();
		pYBOrder[i].nDealNum = (*pRes)[9].GetInt();

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡĳ���ʺŵ����н�ɫ������ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadSimRole(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("roledata", 
		"RoleName,RoleID,MapID,Level,"
		"HairModelID,HairColorID,FaceModelID,FaceDetailID,DressModelID,Sex,AvatarEquip,DisplaySet");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(" and RemoveFlag=") << 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡĳ���ʺŵ����н�ɫ������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadSimRole(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	
	MTRANS_POINTER(pSimRole, pData, tagSimRole);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_ROLENUM_ONEACCOUNT)
	{
		// ���ط���Ϣ //??

		ASSERT(*pNum <= MAX_ROLENUM_ONEACCOUNT);
		*pNum = MAX_ROLENUM_ONEACCOUNT;
	}

	// RoleName,RoleID,MapID,SafeCodeCrc,Level,
	// Status,HairModelID,HairColorID,FaceModelID,FaceDetailID,DressModelID
	for(INT i=0; i<*pNum; ++i)
	{
		(*pRes)[0].GetTCHAR(pSimRole[i].szRoleName, X_SHORT_NAME);
		pSimRole[i].dwRoleID = (*pRes)[1].GetDword();

		pSimRole[i].dwMapID = (*pRes)[2].GetDword();
		pSimRole[i].byLevel = (*pRes)[3].GetByte();

		pSimRole[i].Avatar.wHairMdlID = (*pRes)[4].GetShort();
		pSimRole[i].Avatar.wHairTexID = (*pRes)[5].GetShort();
		pSimRole[i].Avatar.wFaceMdlID = (*pRes)[6].GetShort();
		pSimRole[i].Avatar.wFaceDetailTexID = (*pRes)[7].GetShort();
		pSimRole[i].Avatar.wDressMdlID = (*pRes)[8].GetShort();
		pSimRole[i].Avatar.bySex = (BYTE)(*pRes)[9].GetShort();
		pSimRole[i].sDisplaySet = (*pRes)[11].GetByte();

		ZeroMemory(&pSimRole[i].AvatarEquip, SIZE_AVATAR_EQUIP);
		if(!(*pRes)[10].IsNull())
		{
			//IMSG(_T("Role<id: %u> AvatarEquip size: %d\r\n"), pSimRole[i].dwRoleID, (*pRes)[10].GetLen());
			//ASSERT(SIZE_AVATAR_EQUIP == (*pRes)[9].GetLen());
			(*pRes)[10].GetBlob(&pSimRole[i].AvatarEquip, min((*pRes)[10].GetLen(), SIZE_AVATAR_EQUIP));
		}
		

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������ϢSQL����ʽ��//??0
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadRoleData(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("roledata",	"RoleName,"			"RoleNameCrc,"		"DisplaySet,"		"Sex,"				"HairModelID,"
									"HairColorID,"		"FaceModelID,"		"FaceDetailID,"		"DressModelID,"		"MapID,"	
									"X,"				"Y,"				"Z,"				"FaceX,"			"FaceY,"	
									"FaceZ,"			"Class,"			"ClassEx,"			"Level,"			"ExpCurLevel,"	
									"HP,"				"MP,"				"AttPtAvail,"		"TalentPtAvail,"	"PhysiqueAdded,"	
									"StrengthAdded,"	"PneumaAdded,"		"InnerforceAdded,"	"TechniqueAdded,"	"AgilityAdded,"	
									"Injury,"			"Vitality,"			"Knowledge,"		"Morality,"			"Culture,"	
									"Credit,"			"Identity,"			"VIPPoint,"			"BagSize,"			"BagGold,"	
									"BagSilver,"		"BagYuanBao,"		"GuildID,"			"0,"				"0,"	
									"0,"				"CreateTime,"		"LoginTime,"		"LogoutTime,"		"OnlineTime,"	
									"CurOnlineTime,"	"Rage,"				"Endurance,"		"SGFlag,"			"CloseSGTime,"
									"TalentType1,"		"TalentType2,"		"TalentType3,"		"TalentType4,"		"TalentVal1,"
									"TalentVal2,"		"TalentVal3,"		"TalentVal4,"		"RebornMapID,"		"TotalTax,"	
									"Morale,"			"GetMallFreeTime,"	"RemoteOpenSet,"	"CurTitleID,"		"ExchangeVolume,"
									"ScriptData,"		"TreasureSum,"		"StallLevel,"		"StallDailyExp,"	"StallCurExp,"	
									"StallLastTime,"	"Hostility,"		"DeadUnSetSafeGuardCountDown,"	"ItemTransportMapID,"
									"ItemTransportX,"	"ItemTransportZ,"	"ItemTransportY,"	"Strength," 		"StrengthNum,"	
									"Heroism,"			"Wit,"				"Errantry,"			"Valor,"			"LoverID,"			
									"HaveWedding,"		"LastLessingTime,"	"LastLessingLevel,"	"NeedPrisonRevive");

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;
	pStream->FillString(" and RemoveFlag=") << 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadRoleData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(NULL == pNum);
	
	MTRANS_POINTER(pRoleData, pData, tagRoleDataLoad);
	tagRoleDataConst	*pRoleDataConst = &pRoleData->RoleDataConst;
	tagRoleDataSave		*pRoleDataSave	= &pRoleData->RoleDataSave;

	if(pRes->GetRowCount() != 1)
	{
		// ���ط���Ϣ//??

		ASSERT(pRes->GetRowCount() == 1);
	}

	(*pRes)[0].GetTCHAR(pRoleDataConst->szRoleName, X_SHORT_NAME);
	pRoleDataConst->dwRoleNameCrc				= (*pRes)[1].GetDword();
	pRoleDataConst->Avatar.bySex				= (*pRes)[3].GetByte();
	pRoleDataConst->Avatar.wHairMdlID			= (*pRes)[4].GetShort();
	pRoleDataConst->Avatar.wHairTexID			= (*pRes)[5].GetShort();
	pRoleDataConst->Avatar.wFaceMdlID			= (*pRes)[6].GetShort();
	pRoleDataConst->Avatar.wFaceDetailTexID		= (*pRes)[7].GetShort();
	pRoleDataConst->Avatar.wDressMdlID			= (*pRes)[8].GetShort();
	
	pRoleDataSave->DisplaySet					= (*pRes)[2].GetByte();
	pRoleDataSave->dwMapID						= (*pRes)[9].GetDword();
	pRoleDataSave->fCoordinate[0]				= (*pRes)[10].GetFloat();
	pRoleDataSave->fCoordinate[1]				= (*pRes)[11].GetFloat();
	pRoleDataSave->fCoordinate[2]				= (*pRes)[12].GetFloat();
	pRoleDataSave->fFace[0]						= (*pRes)[13].GetFloat();
	pRoleDataSave->fFace[1]						= (*pRes)[14].GetFloat();
	pRoleDataSave->fFace[2]						= (*pRes)[15].GetFloat();

	pRoleDataSave->eClass						= (EClassType)(*pRes)[16].GetInt();
	pRoleDataSave->eClassEx						= (EClassTypeEx)(*pRes)[17].GetInt();
	pRoleDataSave->nLevel						= (*pRes)[18].GetShort();
	pRoleDataSave->nCurExp						= (*pRes)[19].GetInt();
	pRoleDataSave->nHP							= (*pRes)[20].GetInt();
	pRoleDataSave->nMP							= (*pRes)[21].GetInt();

	pRoleDataSave->nAttPoint					= (*pRes)[22].GetInt();
	pRoleDataSave->nTalentPoint					= (*pRes)[23].GetInt();

	pRoleDataSave->nAttPointAdd[ERA_Physique]	= (*pRes)[24].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_Strength]	= (*pRes)[25].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_Pneuma]		= (*pRes)[26].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_InnerForce]	= (*pRes)[27].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_Technique]	= (*pRes)[28].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_Agility]	= (*pRes)[29].GetInt();

	pRoleDataSave->nInjury						= (*pRes)[30].GetInt();
	pRoleDataSave->nVitality					= (*pRes)[31].GetInt();
	pRoleDataSave->nKnowledge					= (*pRes)[32].GetInt();
	pRoleDataSave->nMorality					= (*pRes)[33].GetInt();
	pRoleDataSave->nCulture						= (*pRes)[34].GetInt();
	pRoleDataSave->nCredit						= (*pRes)[35].GetInt();
	pRoleDataSave->nIdentity					= (*pRes)[36].GetInt();
	pRoleDataSave->nVIPPoint					= (*pRes)[37].GetInt();

	pRoleDataSave->n16BagSize					= (*pRes)[38].GetShort();
	pRoleDataSave->nBagGold						= (*pRes)[39].GetInt();
	pRoleDataSave->nBagSilver					= (*pRes)[40].GetInt();
	pRoleDataSave->nBagYuanBao					= (*pRes)[41].GetInt();

	pRoleDataSave->dwGuildID					= (*pRes)[42].GetDword();
	//pRoleDataSave->nWareGold					= (*pRes)[43].GetInt();
	//pRoleDataSave->nWareSilver					= (*pRes)[44].GetInt();
	//pRoleDataSave->nWareYuanBao					= (*pRes)[45].GetInt();

	if(!DataTime2DwordTime(pRoleDataConst->CreateTime,	(*pRes)[46].GetString(), (*pRes)[46].GetLen()))
	{
		IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::FormatSaveRoleData()!!!!!\r\n"));
		IMSG(_T("Error: Role Name CRC32 is %u!!!!!\r\n"), pRoleDataConst->dwRoleNameCrc);
		ASSERT(0);
	}

	DataTime2DwordTime(pRoleDataSave->LoginTime,	(*pRes)[47].GetString(), (*pRes)[47].GetLen());
	DataTime2DwordTime(pRoleDataSave->LogoutTime,	(*pRes)[48].GetString(), (*pRes)[48].GetLen());

	pRoleDataSave->nOnlineTime					= (*pRes)[49].GetDword();
	pRoleDataSave->nCurOnlineTime				= (*pRes)[50].GetDword();

	pRoleDataSave->nRage						= (*pRes)[51].GetInt();
	pRoleDataSave->nEndurance					= (*pRes)[52].GetInt();
	pRoleDataSave->bSafeGuard					= (*pRes)[53].GetInt();

	//pRoleDataSave->CloseSafeGuardTime			= (*pRes)[54].GetDword();
	DataTime2DwordTime(pRoleDataSave->CloseSafeGuardTime,	(*pRes)[54].GetString(), (*pRes)[54].GetLen());

	pRoleDataSave->talent[0].eType				= (ETalentType)(*pRes)[55].GetInt();
	pRoleDataSave->talent[1].eType				= (ETalentType)(*pRes)[56].GetInt();
	pRoleDataSave->talent[2].eType				= (ETalentType)(*pRes)[57].GetInt();
	pRoleDataSave->talent[3].eType				= (ETalentType)(*pRes)[58].GetInt();
	pRoleDataSave->talent[0].nPoint				= (*pRes)[59].GetInt();
	pRoleDataSave->talent[1].nPoint				= (*pRes)[60].GetInt();
	pRoleDataSave->talent[2].nPoint				= (*pRes)[61].GetInt();
	pRoleDataSave->talent[3].nPoint				= (*pRes)[62].GetInt();

	pRoleDataSave->dwRebornMapID				= (*pRes)[63].GetDword();
	pRoleDataSave->nTotalTax					= (*pRes)[64].GetInt();
	pRoleDataSave->nMorale						= (*pRes)[65].GetInt();

	DataTime2DwordTime(*(tagDWORDTime*)&pRoleDataSave->dwTimeGetMallFree, (*pRes)[66].GetString(), (*pRes)[66].GetLen());
	pRoleDataSave->sRemoteOpenSet				= (*pRes)[67].GetDword();
	pRoleDataSave->u16ActiveTitleID				= (*pRes)[68].GetInt();
	pRoleDataSave->nExVolume					= (*pRes)[69].GetInt();
	(*pRes)[70].GetBlob(pRoleDataSave->Data.dwData, (*pRes)[70].GetLen());
	pRoleDataSave->nTreasureSum					= (*pRes)[71].GetInt();

	pRoleDataSave->byStallLevel					= (*pRes)[72].GetByte();
	pRoleDataSave->nStallDailyExp				= (*pRes)[73].GetInt();
	pRoleDataSave->nStallCurExp					= (*pRes)[74].GetInt();
	pRoleDataSave->dwLastStallTime				= (*pRes)[75].GetDword();
	pRoleDataSave->nHostility					= (*pRes)[76].GetInt();
	pRoleDataSave->nDeadUnSetSafeGuardCountDown = (*pRes)[77].GetInt();

	pRoleDataSave->dwItemTransportMapID			= (*pRes)[78].GetDword();
	pRoleDataSave->fItemTransportX			= (*pRes)[79].GetFloat();
	pRoleDataSave->fItemTransportZ			= (*pRes)[80].GetFloat();
	pRoleDataSave->fItemTransportY			= (*pRes)[81].GetFloat();

	pRoleDataSave->Strength.dwValue				= (*pRes)[82].GetDword();
	pRoleDataSave->nStrengthNum					= (*pRes)[83].GetInt();
	pRoleDataSave->Strength.m_dwHeroism			= (*pRes)[84].GetDword();
	pRoleDataSave->Strength.m_dwWit				= (*pRes)[85].GetDword();
	pRoleDataSave->Strength.m_dwErrantry		= (*pRes)[86].GetDword();
	pRoleDataSave->Strength.m_dwValor			= (*pRes)[87].GetDword();

	pRoleDataSave->dwLoverID					= (*pRes)[88].GetDword();
	pRoleDataSave->bHaveWedding					= (*pRes)[89].GetBool();

	// Jason 2009-12-7 �����͸�
	pRoleDataSave->timeLastLessing					= (*pRes)[90].GetDword();
	pRoleDataSave->nLessingLevel					= (*pRes)[91].GetInt();
	pRoleDataSave->bNeedPrisonRevive				= (*pRes)[92].GetBool();

	// ���ֶ��ڶ�ȡ��ɫ������Ϣʱ���ö�ȡ
	//ZeroMemory(&pRoleDataSave->DisplaySet, sizeof(tagDisplaySet));
	//if(!(*pRes)[2].IsNull())
	//{
	//	(*pRes)[2].GetBlob(&pRoleDataSave->DisplaySet, (*pRes)[2].GetLen());
	//}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadSkill(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("skill", "ID,BiddenLevel,SelfLevel,CoolDown,Proficiency");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadSkill(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	
	MTRANS_POINTER(pSkill, pData, tagSkillSave);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pSkill[i].dwID			= (*pRes)[0].GetDword();
		pSkill[i].nSelfLevel	= (*pRes)[2].GetInt();
		pSkill[i].nLearnLevel	= (*pRes)[1].GetInt();
		pSkill[i].nCoolDown	    = (*pRes)[3].GetInt();
		pSkill[i].nProficiency	= (*pRes)[4].GetInt();

		pRes->NextRow();
	}

	pData = &pSkill[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫbuff��ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadRoleBuff(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("buff", 
		"SrcUnitID,"		"SrcSkillID,"			"ItemTypeID,"		"ItemSerialID,"		"BuffID,"
		"CurTick,"			"Level,"				"CurLapTimes,"		"EffectSkillID");

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;


}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫbuff��Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadRoleBuff(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pCurPointer, pData, BYTE);
	tagBuffSave *pRoleSave;
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pRoleSave = (tagBuffSave*)pCurPointer;

		pRoleSave->dwSrcUnitID			= (*pRes)[0].GetDword();
		pRoleSave->dwSrcSkillID			= (*pRes)[1].GetDword();
		pRoleSave->dwItemTypeID			= (*pRes)[2].GetDword();
		pRoleSave->n64Serial			= (*pRes)[3].GetLong();

		pRoleSave->dwBuffID				= (*pRes)[4].GetDword();
		pRoleSave->nPersistTick			= (*pRes)[5].GetInt();
		pRoleSave->n8Level				= (INT8)(*pRes)[6].GetInt();
		pRoleSave->n8CurLapTimes		= (INT8)(*pRes)[7].GetInt();


		pRoleSave->n8ModifierNum		= (INT8)((*pRes)[8].GetLen()/sizeof(DWORD));

		pCurPointer = pRoleSave->byData;
		if(pRoleSave->n8ModifierNum > 0)
		{
			(*pRes)[8].GetBlob(pCurPointer, (*pRes)[8].GetLen());
			pCurPointer += (*pRes)[8].GetLen();
		}

		pRes->NextRow();
	}

	pData = pCurPointer;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ƺ���ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadTitle(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("title", "ID,Count");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ƺ���Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadTitle(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	*pNum = pRes->GetRowCount();

	MTRANS_POINTER(p, pData, tagTitleSave);
	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i].u16TitleID		= (UINT16)(*pRes)[0].GetDword();
		p[i].dwStateMark	= (*pRes)[1].GetDword();

		pRes->NextRow();
	}

	pData = &p[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// ��ʼ����Ʒ�����ֶ�//??0
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::InitItemLoad(Beton::MyStream *pStream)
{
	pStream->FillString(
		"SerialNum,"	"Num,"				"TypeID,"		"Bind,"				"LockStat,"
		"UseTimes,"		"FirstGainTime,"	"CreatorID,"	"CreateTime,"		"OwnerID,"
		"AccountID,"	"ContainerTypeID,"	"Suffix,"		"Quality,"			"PotValUsed,"
		"MinUseLevel,"	"MaxUseLevel,"		"WuHun,"		"MinDmg,"			"MaxDmg,"		
		"Armor,"		"PotVal,"			"RoleAttEffect,""PosyTimes,"		"PosyEffect,"
		"EngraveTimes,"	"EngraveAtt,"		"HoleNum,"		"HoleGemID,"		"BrandLevel,"
		"DerateVal,"	"LongFuLevel,"		"WuXing,"		"CanCut,"			"LongInnerID,"
		"LongOuterID,"	"0,"				"0,"			"SpecAtt,"			"Appearance,"
		"Rein,"			"Savvy,"			"Fortune,"		"ColorID,"			"QltyModPct,"
		"QltyModPctEx,"	"CreateMode,"		"CreateID,"		"AttALimMod,"		"AttALimModPct,"
		"FlareVal,"		"NameID,"			"PotValModPct,"	"PotIncTimes,"		"PurpleQltyIdfPct");
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ��Ʒ��ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadItem(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->FillString("select ");
	InitItemLoad(pStream);
	pStream->FillString(" from item left join equip using(SerialNum)");

	pStream->SetWhere();
	pStream->FillString("OwnerID=") << dwRoleID;
	pStream->FillString(" or (AccountID=") << dwAccountID;
	pStream->FillString(" and (ContainerTypeID=") << EICT_Baibao;
	pStream->FillString(" or ContainerTypeID=") << EICT_RoleWare;
	pStream->FillString("))");
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ��Ʒ��Ϣ�����鼰����//??
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadItem(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		MTRANS_POINTER(pItem, pData, tagItem);

		pItem->n64Serial 					= (*pRes)[0].GetLong();
		pItem->n16Num 						= (*pRes)[1].GetShort();
		pItem->dwTypeID 					= (*pRes)[2].GetDword();
		pItem->byBind 						= (*pRes)[3].GetInt();
		pItem->bLock 						= (*pRes)[4].GetBool();

		pItem->nUseTimes 					= (*pRes)[5].GetInt();
		pItem->dwCreatorID 					= (*pRes)[7].GetDword();
		pItem->dwOwnerID 					= (*pRes)[9].GetDword();
		pItem->dwAccountID 					= (*pRes)[10].GetDword();

		pItem->eConType 					= (EItemConType)(*pRes)[11].GetInt();
		pItem->n16Index 					= (*pRes)[12].GetShort();
		pItem->eCreateMode					= (EItemCreateMode)(*pRes)[46].GetInt();
		pItem->dwCreateID					= (*pRes)[47].GetDword();

		pItem->dwNameID						=(*pRes)[51].GetDword();

		if(!DataTime2DwordTime(pItem->dw1stGainTime, (*pRes)[6].GetString(), (*pRes)[6].GetLen()))
		{
			// ���ݿ��м�¼���ڵ��ַ�����ʽ����.Ӧ����: 2008-11-03 11:47:00
			IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::ProcResLoadItem()!!!!!\r\n"));
			IMSG(_T("Error: Item Serial Number is %lld!!!!!\r\n"), pItem->n64Serial);
			ASSERT(0);
		}

		if(!DataTime2DwordTime(pItem->dwCreateTime, (*pRes)[8].GetString(), (*pRes)[8].GetLen()))
		{
			// ���ݿ��м�¼���ڵ��ַ�����ʽ����.Ӧ����: 2008-11-03 11:47:00
			IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::ProcResLoadItem()!!!!!\r\n"));
			IMSG(_T("Error: Item Serial Number is %lld!!!!!\r\n"), pItem->n64Serial);
			ASSERT(0);
		}
		
		if(!MIsEquipment(pItem->dwTypeID))
		{
			pData = &pItem[1];
			pRes->NextRow();
			continue;
		}

		MTRANS_POINTER(pEquip, pData, tagEquip);

		// װ������
		pEquip->equipSpec.byQuality 				= (*pRes)[13].GetByte();
		pEquip->equipSpec.nPotValUsed 				= (*pRes)[14].GetInt();
		pEquip->equipSpec.byMinUseLevel 			= (*pRes)[15].GetByte();
		pEquip->equipSpec.byMaxUseLevel 			= (*pRes)[16].GetByte();
		pEquip->equipSpec.n16WuHun 					= (*pRes)[17].GetShort();
		pEquip->equipSpec.n16MinDmg 				= (*pRes)[18].GetShort();
		pEquip->equipSpec.n16MaxDmg 				= (*pRes)[19].GetShort();
		pEquip->equipSpec.n16Armor 					= (*pRes)[20].GetShort();
		pEquip->equipSpec.nPotVal 					= (*pRes)[21].GetInt();
	
		pEquip->equipSpec.byPosyTimes 				= (*pRes)[23].GetByte();
		
		pEquip->equipSpec.byEngraveTimes 			= (*pRes)[25].GetByte();
		
		pEquip->equipSpec.byHoleNum 				= (*pRes)[27].GetByte();
		
		pEquip->equipSpec.byBrandLevel 				= (*pRes)[29].GetByte();
		
		pEquip->equipSpec.byLongfuLevel				= (*pRes)[31].GetByte();

		pEquip->equipSpec.bCanCut 					= (*pRes)[33].GetBool();
		pEquip->equipSpec.dwLongHunInnerID			= (*pRes)[34].GetDword();
		pEquip->equipSpec.dwLongHunOuterID			= (*pRes)[35].GetDword();
		//pEquip->equipSpec.dwLongHunOuterTriggerID	= (*pRes)[36].GetDword();
		//pEquip->equipSpec.dwLongHunOuterBuffID		= (*pRes)[37].GetDword();
		pEquip->equipSpec.bySpecAtt					= (*pRes)[38].GetInt();

		// ʱװ����
		pEquip->equipSpec.n16Appearance				= (*pRes)[39].GetInt();
		pEquip->equipSpec.byRein					= (*pRes)[40].GetByte();
		pEquip->equipSpec.bySavvy					= (*pRes)[41].GetByte();
		pEquip->equipSpec.byFortune					= (*pRes)[42].GetByte();
		pEquip->equipSpec.n8ColorID					= (INT8)(*pRes)[43].GetInt();

		pEquip->equipSpec.n16QltyModPct				= (*pRes)[44].GetShort();
		pEquip->equipSpec.n16QltyModPctEx			= (*pRes)[45].GetShort();
		pEquip->equipSpec.n16PotValModPct			= (*pRes)[52].GetShort();
		// Jason 2009-12-1 װ��Ǳ��ֵ��������Ӱ��
		pEquip->equipSpec.nPotIncTimes				= (*pRes)[53].GetShort();
		// Jason 2009-12-2 ����װ����ɫ�������ʣ�Ĭ��0
		pEquip->equipSpec.dwPurpleQltyIdfPct		= (*pRes)[54].GetDword();
		
		pEquip->equipSpec.n16AttALimMod				= (*pRes)[48].GetInt();
		pEquip->equipSpec.n16AttALimModPct			= (*pRes)[49].GetInt();
		pEquip->equipSpec.byFlareVal				= (*pRes)[50].GetByte();
		
		(*pRes)[22].GetBlob(pEquip->equipSpec.nRoleAttEffect, min((*pRes)[22].GetLen(), sizeof(pEquip->equipSpec.nRoleAttEffect)));
		(*pRes)[24].GetBlob(pEquip->equipSpec.PosyEffect, min((*pRes)[24].GetLen(), sizeof(pEquip->equipSpec.PosyEffect)));
		(*pRes)[26].GetBlob(pEquip->equipSpec.nEngraveAtt, min((*pRes)[26].GetLen(), sizeof(pEquip->equipSpec.nEngraveAtt)));
		(*pRes)[28].GetBlob(pEquip->equipSpec.dwHoleGemID, min((*pRes)[28].GetLen(), sizeof(pEquip->equipSpec.dwHoleGemID)));
		(*pRes)[30].GetBlob(pEquip->equipSpec.nBrandAtt, min((*pRes)[30].GetLen(), sizeof(pEquip->equipSpec.nBrandAtt)));
		(*pRes)[32].GetBlob(pEquip->equipSpec.byLongfuAtt, min((*pRes)[32].GetLen(), sizeof(pEquip->equipSpec.byLongfuAtt)));

		pData = &pEquip[1];
		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadFriend(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("friend", "FriendID,GroupID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadFriend(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagFriendSave);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_FRIENDNUM)
	{
		// ���ط���Ϣ//??

		ASSERT(*pNum <= MAX_FRIENDNUM);

		*pNum = MAX_FRIENDNUM; // �������ֻȡǰ���
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i].dwFriendID = (*pRes)[0].GetDword();
		p[i].nGroupID	= (*pRes)[1].GetInt();

		pRes->NextRow();
	}

	pData = &p[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�Ѻö���ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadFriendship(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("friendship", "FriendID,FriVal");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�Ѻö���ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadFriendshipEx(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("friendship", "RoleID,FriVal");
	pStream->SetWhere();
	pStream->FillString("FriendID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�Ѻö���Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadFriendship(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagFriendshipSave);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_FRIENDNUM)
	{
		// ���ط���Ϣ//??

		ASSERT(*pNum <= MAX_FRIENDNUM);

		*pNum = MAX_FRIENDNUM; // �������ֻȡǰ���
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i].dwFriendID = (*pRes)[0].GetDword();
		p[i].nFriVal	= (*pRes)[1].GetInt();

		pRes->NextRow();
	}

	pData = &p[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��������ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadBlackList(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("blacklist", "BlackID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadBlackList(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, DWORD);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_BLACKLIST)
	{
		// ���ط���Ϣ//??

		ASSERT(*pNum <= MAX_BLACKLIST);

		*pNum = MAX_BLACKLIST; // �������ֻȡǰ���
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i] = (*pRes)[0].GetDword();

		pRes->NextRow();
	}

	pData = &p[*pNum];
}


//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadVCard(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("visiting_card", "Level,Job,MateRoleID,FactionID,Position,Visibility,Sex,Constellation,ChineseZodiac,Area,Province,City,HeadUrl,Signature, RoleID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}


//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɫ������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadVCard(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagVCardData);
	*pNum = pRes->GetRowCount();

	ASSERT(1 == *pNum);

	p->dwLevel			= (*pRes)[0].GetInt();
	p->dwJob			= (*pRes)[1].GetInt();
	p->dwMateRoleID		= (*pRes)[2].GetInt();
	p->dwFactionID		= (*pRes)[3].GetInt();
	p->dwPosition		= (*pRes)[4].GetInt();

	p->customVCardData.bVisibility		= (*pRes)[5].GetBool();
	p->customVCardData.bySex			= (*pRes)[6].GetByte();
	p->customVCardData.byConstellation	= (*pRes)[7].GetByte();
	p->customVCardData.byChineseZodiac	= (*pRes)[8].GetByte();
	p->customVCardData.byArea			= (*pRes)[9].GetByte();
	p->customVCardData.byProvince		= (*pRes)[10].GetByte();
	(*pRes)[11].GetTCHAR(p->customVCardData.chCity, LEN_CITY_NAME);
	p->customVCardData.byHeadUrlSize	= (*pRes)[12].GetTCHAR(reinterpret_cast<TCHAR *>(p->customVCardData.byData), 200) * sizeof(TCHAR);
	p->customVCardData.bySignatureSize	= (*pRes)[13].GetTCHAR(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData[p->customVCardData.byHeadUrlSize])), 200) * sizeof(TCHAR);

	p->dwRoleID		= (*pRes)[14].GetInt();

	DWORD dwVCardSize = sizeof(tagVCardData) - 1 + p->customVCardData.byHeadUrlSize + p->customVCardData.bySignatureSize;

	pData = reinterpret_cast<BYTE *>(pData) + dwVCardSize;

}
//-------------------------------------------------------------------------------------------------------
// ��ȡ������ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadClanData(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("clan_data", "RepXuanYuan,ConXuanYuan,ActCntXuanYuan,"
									"RepShenNong,ConShenNong,ActCntShenNong,"
									"RepFuXi,ConFuXi,ActCntFuXi,"
									"RepSanMiao,ConSanMiao,ActCntSanMiao,"
									"RepJiuLi,ConJiuLi,ActCntJiuLi,"
									"RepYueZhi,ConYueZhi,ActCntYueZhi,"
									"RepNvWa,ConNvWa,ActCntNvWa,"
									"RepGongGong,ConGongGong,ActCntGongGong,"
									"FameMask, RepRstTimeStamp");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}




//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadClanData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	MTRANS_POINTER(p, pData, tagDBReputeData);

	*pNum = pRes->GetRowCount();
	ASSERT(1 == *pNum);

	
	for(INT nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		p->m_nReputation[nClanType]		= (*pRes)[nClanType * 3].GetInt();
		p->m_nContribution[nClanType]	= (*pRes)[nClanType * 3 + 1].GetInt();
		p->m_n8ActCount[nClanType]		= (*pRes)[nClanType * 3 + 2].GetByte();
		
	}
	p->m_u16FameMask	= (*pRes)[ECLT_NUM * 3].GetShort();
	p->m_dwtLastResetTime= (*pRes)[ECLT_NUM * 3 + 1].GetDword();

	pData = reinterpret_cast<BYTE *>(pData) + sizeof(tagDBReputeData);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�����ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadEnemy(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("enemy", "EnemyID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�����Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadEnemy(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pEnemyID, pData, DWORD);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_ENEMYNUM)
	{
		// ���ط���Ϣ//??

		ASSERT(*pNum <= MAX_ENEMYNUM);

		*pNum = MAX_ENEMYNUM; // �������ֻȡǰ���
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		pEnemyID[i] = (*pRes)[0].GetDword();

		pRes->NextRow();
	}

	pData = &pEnemyID[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadTask(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("task", "TaskID,StartTime,TaskFlag,Monster0Num,Monster1Num,Monster2Num,"
						"Monster3Num,Monster4Num,Monster5Num,ScriptData, DynamicTarget");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadTask(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pQuest, pData, tagQuestSave);
	*pNum = pRes->GetRowCount();

	if(*pNum > QUEST_MAX_COUNT)
	{
		// ���ط���Ϣ//??

		ASSERT(*pNum <= QUEST_MAX_COUNT);
		*pNum = QUEST_MAX_COUNT;	// ֻ��ȡǰ�������
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		pQuest[i].u16QuestID	= (UINT16)(*pRes)[0].GetDword();
		pQuest[i].dwAcceptTime	= (*pRes)[1].GetDword();
		pQuest[i].dwQuestFlag	= (*pRes)[2].GetDword();
		
		for(INT32 j=0; j<QUEST_CREATURES_COUNT; ++j)
		{
			pQuest[i].n16MonsterNum[j] = (INT16)(*pRes)[j+3].GetInt();
		}

		(*pRes)[QUEST_CREATURES_COUNT+3].GetBlob(pQuest[i].Data.dwData, (*pRes)[QUEST_CREATURES_COUNT+3].GetLen());
		if((*pRes)[QUEST_CREATURES_COUNT+4].GetLen() > 0)
			(*pRes)[QUEST_CREATURES_COUNT+4].GetBlob(&pQuest[i].DynamicTarget, (*pRes)[QUEST_CREATURES_COUNT+4].GetLen());

		pRes->NextRow();
	}

	pData = &pQuest[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�����������ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadTaskDone(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("task_done", "TaskID,Times,StartTime");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�����������Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadTaskDone(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pQuest, pData, tagQuestDoneSave);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pQuest[i].u16QuestID = (UINT16)(*pRes)[0].GetDword();
		pQuest[i].dwStartTime = (*pRes)[2].GetDword();
		pQuest[i].nTimes = (*pRes)[1].GetInt();

		pRes->NextRow();
	}

	pData = &pQuest[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��������������ϢSQL����ʽ��(��ҵ�½ʱ��)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadSimTaskDone(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("task_done", "TaskID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��������������Ϣ�����鼰����(��ҵ�½ʱ��)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadSimTaskDone(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pQuest, pData, tagQuestDoneSimSave);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pQuest[i].u16QuestID = (UINT16)(*pRes)[0].GetDword();

		pRes->NextRow();
	}

	pData = &pQuest[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadItemCDTime(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("item_cdtime", "CDTime");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadItemCDTime(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	if((*pRes)[0].IsNull())
	{
		*pNum = 0;
		return;
	}

	*pNum = (*pRes)[0].GetLen() / sizeof(tagCDTime);
	(*pRes)[0].GetBlob(pData, (*pRes)[0].GetLen());	

	pData = (BYTE*)pData + (*pRes)[0].GetLen();
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ����е���Ʒ��ϢSQL����ʽ��(��ҵ�½ʱ��)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadItemBaiBao(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->FillString("select ");
	InitItemLoad(pStream);
	pStream->FillString(" from item_baibao left join equip_baibao using(SerialNum)");
		
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
	if (GT_VALID(m_n64LoadSerial))
	{
		pStream->FillString(" and SerialNum=") << m_n64LoadSerial;
	}
	pStream->SetLimit(m_nLoadNumLim);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ����е�Ԫ������SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadBaiBaoYuanBao(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("account_common", "BaiBaoYuanBao");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ����е�Ԫ������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadBaiBaoYuanBao(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	INT32* nYuanBao = (INT32*)pData;

	INT32 nNum	= pRes->GetRowCount();
	*nYuanBao	= 0;

	if (nNum != 0)
	{
		*nYuanBao = (*pRes)[0].GetInt();
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ����е���Ʒ��¼SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadBaiBaoLog(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("log_baibao", "AccountID,RoleID,LogTime,n16Type,TypeID,LeaveWords");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//-------------------------------------------------------------------------------------------------------
// �����ȡ���İٱ���Log
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadBaiBaoLog(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pBaibaoLog, pData, tagBaiBaoRecord);
	*pNum = pRes->GetRowCount();

	for(INT16 i=0; i<*pNum; ++i)
	{
		pBaibaoLog->dwRoleID = (*pRes)[1].GetDword();
		pBaibaoLog->dwTypeID = (*pRes)[4].GetDword();
		pBaibaoLog->n16Type = (*pRes)[3].GetShort();
		DataTime2DwordTime(*(tagDWORDTime*)&(pBaibaoLog->dwTime), (*pRes)[2].GetString(), (*pRes)[2].GetLen());
		(*pRes)[5].GetBlob(pBaibaoLog->szWords, (*pRes)[5].GetLen());

		pBaibaoLog->n16Size = (INT16)(sizeof(tagBaiBaoRecord) + (*pRes)[5].GetLen() - sizeof(pBaibaoLog->szWords));

		pBaibaoLog = (tagBaiBaoRecord*)((BYTE*)pBaibaoLog + pBaibaoLog->n16Size);
		pRes->NextRow();
	}

	pData = pBaibaoLog;
}

////-------------------------------------------------------------------------------------------------------
//// ��ȡ�ٱ����е���Ʒ��Ϣ�����鼰����
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::ProcResLoadItemBaiBao(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
//{
//	ASSERT(pNum != NULL);
//
//	*pNum = pRes->GetRowCount();
//
//	for(INT32 i=0; i<*pNum; ++i)
//	{
//		MTRANS_POINTER(pItem, pData, tagItem);
//
//		pItem->n64Serial 					= (*pRes)[0].GetLong();
//		pItem->n16Num 						= (*pRes)[1].GetShort();
//		pItem->dwTypeID 					= (*pRes)[2].GetDword();
//		pItem->byBind 						= (*pRes)[3].GetInt();
//		pItem->bLock 						= (*pRes)[4].GetBool();
//
//		pItem->nUseTimes 					= (*pRes)[5].GetInt();
//		pItem->dwCreatorID 					= (*pRes)[7].GetDword();
//		pItem->dwOwnerID 					= (*pRes)[9].GetDword();
//		pItem->dwAccountID 					= (*pRes)[10].GetDword();
//
//		pItem->eConType 					= (EItemConType)(*pRes)[11].GetInt();
//		pItem->n16Index 					= (*pRes)[12].GetShort();
//		pItem->eCreateMode					= (EItemCreateMode)(*pRes)[45].GetInt();
//		pItem->dwCreateID					= (*pRes)[46].GetDword();
//
//		if(!DataTime2DwordTime(pItem->dw1stGainTime, (*pRes)[6].GetString(), (*pRes)[6].GetLen()))
//		{
//			// ���ݿ��м�¼���ڵ��ַ�����ʽ����.Ӧ����: 2008-11-03 11:47:00
//			IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::ProcResLoadItem()!!!!!\r\n"));
//			IMSG(_T("Error: Item Serial Number is %lld!!!!!\r\n"), pItem->n64Serial);
//			ASSERT(0);
//		}
//
//		if(!DataTime2DwordTime(pItem->dwCreateTime, (*pRes)[8].GetString(), (*pRes)[8].GetLen()))
//		{
//			// ���ݿ��м�¼���ڵ��ַ�����ʽ����.Ӧ����: 2008-11-03 11:47:00
//			IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::ProcResLoadItem()!!!!!\r\n"));
//			IMSG(_T("Error: Item Serial Number is %lld!!!!!\r\n"), pItem->n64Serial);
//			ASSERT(0);
//		}
//
//		if(!MIsEquipment(pItem->dwTypeID))
//		{
//			pData = &pItem[1];
//			pRes->NextRow();
//			continue;
//		}
//
//		MTRANS_POINTER(pEquip, pData, tagEquip);
//
//		// װ������
//		pEquip->equipSpec.byQuality 				= (*pRes)[13].GetByte();
//		pEquip->equipSpec.nPotValUsed 				= (*pRes)[14].GetInt();
//		pEquip->equipSpec.byMinUseLevel 			= (*pRes)[15].GetByte();
//		pEquip->equipSpec.byMaxUseLevel 			= (*pRes)[16].GetByte();
//		pEquip->equipSpec.n16WuHun 					= (*pRes)[17].GetShort();
//		pEquip->equipSpec.n16MinDmg 				= (*pRes)[18].GetShort();
//		pEquip->equipSpec.n16MaxDmg 				= (*pRes)[19].GetShort();
//		pEquip->equipSpec.n16Armor 					= (*pRes)[20].GetShort();
//		pEquip->equipSpec.nPotVal 					= (*pRes)[21].GetInt();
//
//		pEquip->equipSpec.byPosyTimes 				= (*pRes)[23].GetByte();
//
//		pEquip->equipSpec.byEngraveTimes 			= (*pRes)[25].GetByte();
//
//		pEquip->equipSpec.byHoleNum 				= (*pRes)[27].GetByte();
//
//		pEquip->equipSpec.byBrandLevel 				= (*pRes)[29].GetByte();
//
//		pEquip->equipSpec.byLongfuLevel				= (*pRes)[31].GetByte();
//
//		pEquip->equipSpec.bCanCut 					= (*pRes)[33].GetBool();
//		pEquip->equipSpec.dwLongHunInnerTriggerID	= (*pRes)[34].GetDword();
//		pEquip->equipSpec.dwLongHunInnerBuffID		= (*pRes)[35].GetDword();
//		pEquip->equipSpec.dwLongHunOuterTriggerID	= (*pRes)[36].GetDword();
//		pEquip->equipSpec.dwLongHunOuterBuffID		= (*pRes)[37].GetDword();
//		pEquip->equipSpec.bySpecAtt					= (*pRes)[38].GetInt();
//		//pEquip->equipSpec.dwSpecBuffID				= (*pRes)[39].GetDword();
//		pEquip->equipSpec.byFlareType				= (*pRes)[40].GetInt();
//		pEquip->equipSpec.byFlareEffect				= (*pRes)[41].GetInt();
//		pEquip->equipSpec.n16QltyModPct				= (*pRes)[42].GetShort();
//		pEquip->equipSpec.n16QltyModPctEx			= (*pRes)[43].GetShort();
//		pEquip->equipSpec.dwColor					= (*pRes)[44].GetDword();
//		
//		(*pRes)[22].GetBlob(pEquip->equipSpec.nRoleAttEffect, min((*pRes)[22].GetLen(), sizeof(pEquip->equipSpec.nRoleAttEffect)));
//		(*pRes)[24].GetBlob(pEquip->equipSpec.PosyEffect, min((*pRes)[24].GetLen(), sizeof(pEquip->equipSpec.PosyEffect)));
//		(*pRes)[26].GetBlob(pEquip->equipSpec.nEngraveAtt, min((*pRes)[26].GetLen(), sizeof(pEquip->equipSpec.nEngraveAtt)));
//		(*pRes)[28].GetBlob(pEquip->equipSpec.dwHoleGemID, min((*pRes)[28].GetLen(), sizeof(pEquip->equipSpec.dwHoleGemID)));
//		(*pRes)[30].GetBlob(pEquip->equipSpec.nBrandAtt, min((*pRes)[30].GetLen(), sizeof(pEquip->equipSpec.nBrandAtt)));
//		(*pRes)[32].GetBlob(pEquip->equipSpec.byLongfuAtt, min((*pRes)[32].GetLen(), sizeof(pEquip->equipSpec.byLongfuAtt)));
//
//		pData = &pEquip[1];
//		pRes->NextRow();
//	}
//}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ҫ��¼log����ƷTypeID��ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadItemNeedLog(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("item_needlog", "TypeID");
	pStream->SetWhere();
	pStream->FillString("NeedLog=") << TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ҫ��¼log����ƷTypeID��Ϣ�����鼰����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadItemNeedLog(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pTypeID, pData, DWORD);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_ITEM_NEEDLOG_NUM)
	{
		ASSERT(*pNum <= MAX_ITEM_NEEDLOG_NUM);
		*pNum = MAX_ITEM_NEEDLOG_NUM;
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		pTypeID[i] = (*pRes)[0].GetDword();

		pRes->NextRow();
	}

	pData = &pTypeID[*pNum];
}
//-------------------------------------------------------------------------------------------------------
// ��ȡ�����Ź���ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadAllGPInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("group_purchase", "GuildID,RoleID,MallID,ItemPrice,"
		"ParticipatorNum,RequireNum,RemainTime,ParticipatorList");
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�����Ź���Ϣ��ѯ�������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadAllGPInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pGPInfo, pData, tagGPInfo);
	*pNum = pRes->GetRowCount();

	for(INT16 i=0; i<*pNum; ++i)
	{
		pGPInfo->dwGuildID			= (*pRes)[0].GetDword();
		pGPInfo->dwRoleID			= (*pRes)[1].GetDword();
		pGPInfo->dwMallID			= (*pRes)[2].GetDword();
		pGPInfo->nPrice				= (*pRes)[3].GetDword();
		pGPInfo->nParticipatorNum	= (*pRes)[4].GetShort();
		pGPInfo->nRequireNum		= (*pRes)[5].GetShort();
		pGPInfo->nRemainTime		= (*pRes)[6].GetInt();

		(*pRes)[7].GetBlob(pGPInfo->dwParticipators, (*pRes)[7].GetLen());

		pGPInfo = (tagGPInfo*)((BYTE*)pGPInfo + sizeof(tagGPInfo) + (pGPInfo->nParticipatorNum-1)*sizeof(DWORD));
		pRes->NextRow();
	}

	pData = pGPInfo;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����VIP̯λ��ϢSQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatGetAllVIPStallInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("vip_stall", "StallID,RoleID,RemainTime");
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����VIP̯λ��Ϣ��ѯ�������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResGetAllVIPStallInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pVIPStallInfo, pData, tagVipStall2DB);

	*pNum = pRes->GetRowCount();

	for(INT16 i=0; i<*pNum; ++i)
	{
		BYTE index = (*pRes)[0].GetByte();
		pVIPStallInfo[index].byStallIndex	= index;
		pVIPStallInfo[index].dwOwnerID		= (*pRes)[1].GetDword();
		pVIPStallInfo[index].nRemainTime	= (*pRes)[2].GetInt();

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���й̶������SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatActivityData(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("activity", "dwID, ScriptData");
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���й̶������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadActivityData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pActivityData, pData, tagActivityData);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pActivityData[i].dwID = (*pRes)[0].GetDword();
		(*pRes)[1].GetBlob(pActivityData[i].Data.dwData, (*pRes)[1].GetLen());

		pRes->NextRow();
	}
}


VOID CLoongDB::FormatStrengthRankings(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("strength_rankings", "Num, PreNum, Strength, RoleName");
}

VOID CLoongDB::ProcResLoadStrengthRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagStrengthRoleNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<X_STRENGTH_RANKINGS_LEN; ++i)
	{
		// ����ȡ��������¼
		pRankingsNode[i].nNum = (*pRes)[0].GetInt();
		pRankingsNode[i].nPreNum = (*pRes)[1].GetInt();
		pRankingsNode[i].dwStrength = (*pRes)[2].GetDword();
		//(*pRes)[3].SetType(pRes->EDBT_STRING);
		(*pRes)[3].GetTCHAR(pRankingsNode[i].szRoleName, X_SHORT_NAME);
		
		pRes->NextRow();
	}
}