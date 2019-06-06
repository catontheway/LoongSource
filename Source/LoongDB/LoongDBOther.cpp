//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongDBSave.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: ���ݿ����Ӧ�ò�ʵ�� -- ���룬ɾ�������ݵȲ���
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/RoleDefine.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_quest.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../WorldDefine/mall_define.h"
#include "LoongDB.h"

//-------------------------------------------------------------------------------------------------------
// ���ѵ�׷��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertFriend(DWORD dwRoleID, LPVOID pFriendSave)
{
	return Update(dwRoleID, pFriendSave, 1, GT_INVALID, (&CLoongDB::FormatInsertFriend));
}

//-------------------------------------------------------------------------------------------------------
// ���ѵĸ���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateFriend(DWORD dwRoleID, LPVOID pFriendSave)
{
	return Update(dwRoleID, pFriendSave, 1, GT_INVALID, (&CLoongDB::FormatUpdateFriend));
}

//-------------------------------------------------------------------------------------------------------
// ���ѵ�ɾ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteFriend(DWORD dwRoleID, DWORD dwFriendID)
{
	return Update(dwRoleID, &dwFriendID, 1, GT_INVALID, (&CLoongDB::FormatDeleteFriend));
}

//-------------------------------------------------------------------------------------------------------
// ɾ�����ѱ��к͸ý�ɫ��ص����м�¼
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteAllFriRel(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, GT_INVALID, (&CLoongDB::FormatDeleteAllFriRel));
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�׷��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertFriendship(DWORD dwRoleID, LPVOID pFriendshipSave)
{
	return Update(dwRoleID, pFriendshipSave, 1, GT_INVALID, (&CLoongDB::FormatInsertFriendship));
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵĸ���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateFriendship(DWORD dwRoleID, LPVOID pFriendshipSave)
{
	return Update(dwRoleID, pFriendshipSave, 1, GT_INVALID, (&CLoongDB::FormatUpdateFriendship));
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�ɾ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteFriendship(DWORD dwRoleID, DWORD dwFriendID)
{
	return Update(dwRoleID, &dwFriendID, 1, GT_INVALID, (&CLoongDB::FormatDeleteFriendship));
}

//-------------------------------------------------------------------------------------------------------
// ɾ�����Ѷȱ��к͸ý�ɫ��ص����м�¼
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteAllFriValRel(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, GT_INVALID, (&CLoongDB::FormatDeleteAllFriValRel));
}

//-------------------------------------------------------------------------------------------------------
// ������������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertBlack(DWORD dwRoleID, DWORD dwBlackID)
{
	return Update(dwRoleID, &dwBlackID, 1, GT_INVALID, (&CLoongDB::FormatInsertBlack));
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteBlack(DWORD dwRoleID, DWORD dwBlackID)
{
	return Update(dwRoleID, &dwBlackID, 1, GT_INVALID, (&CLoongDB::FormatDeleteBlack));
}

//-------------------------------------------------------------------------------------------------------
// ��е�׷��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertEnemy(DWORD dwRoleID, DWORD dwEnemyID)
{
	return Update(dwRoleID, &dwEnemyID, 1, GT_INVALID, (&CLoongDB::FormatInsertEnemy));
}

//-------------------------------------------------------------------------------------------------------
// ��е�ɾ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteEnemy(DWORD dwRoleID, DWORD dwEnemyID)
{
	return Update(dwRoleID, &dwEnemyID, 1, GT_INVALID, (&CLoongDB::FormatDeleteEnemy));
}

//-------------------------------------------------------------------------------------------------------
// �ƺŵ�׷��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertTitle(DWORD dwRoleID, LPVOID pData)
{
	return Update(dwRoleID, pData, 1, GT_INVALID, (&CLoongDB::FormatInsertTitle));
}
//-------------------------------------------------------------------------------------------------------
// �ƺŵ�ɾ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::RemoveTitle(DWORD dwRoleID, LPVOID pData)
{
	return Update(dwRoleID, pData, 1, GT_INVALID, (&CLoongDB::FormatRemoveTitle));
}
//-------------------------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteTask(DWORD dwRoleID, DWORD dwTaskID)
{
	return Update(dwRoleID, &dwTaskID, 1, GT_INVALID, (&CLoongDB::FormatDeleteTask));
}

//-------------------------------------------------------------------------------------------------------
// ����һ��Ԫ�������˻�
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertYBAccount(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, GT_INVALID, (&CLoongDB::FormatInsertYBAccount));
} 

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻���Ԫ������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateAccountYB(DWORD dwRoleID, INT32 nYuanBao)
{
	return Update(dwRoleID, &nYuanBao, 1, GT_INVALID, (&CLoongDB::FormatAccountYB));
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻��Ľ�Ǯ����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateAccountSilver(DWORD dwRoleID, LPVOID pMoney)
{
	return Update(dwRoleID, pMoney, 1, GT_INVALID, (&CLoongDB::FormatAccountSilver));
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻����۶������ύ״̬
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSubmitSell(DWORD dwRoleID, bool bSellOrder)
{
	return Update(dwRoleID, &bSellOrder, 1, GT_INVALID, (&CLoongDB::FormatSubmitSell));
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻��չ��������ύ״̬
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSubmitBuy(DWORD dwRoleID, bool bBuyOrder)
{
	return Update(dwRoleID, &bBuyOrder, 1, GT_INVALID, (&CLoongDB::FormatSubmitBuy));
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertOrder(DWORD dwRoleID, LPVOID pYBOrder)
{
	return Update(dwRoleID, pYBOrder, 1, GT_INVALID, (&CLoongDB::FormatInsertOrder));
}

//-------------------------------------------------------------------------------------------------------
// ������ر�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::CompleteOrder(DWORD dwRoleID, LPVOID pComOrder)
{
	return Update(dwRoleID, pComOrder, 1, GT_INVALID, (&CLoongDB::FormatCompleteOrder));
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::RefreshOrder(DWORD dwRoleID, LPVOID pRefreshOrder)
{
	return Update(dwRoleID, pRefreshOrder, 1, GT_INVALID, (&CLoongDB::FormatRefreshOrder));
}

//-------------------------------------------------------------------------------------------------------
// ɾ�����ݿ���ʱ�䳬����������ж���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteYBOrder(DWORD dwRoleID, LPVOID pDeleteTime)
{
	return Update(dwRoleID, pDeleteTime, 1, GT_INVALID, (&CLoongDB::FormatDeleteYBOrder));
}

//-------------------------------------------------------------------------------------------------------
// ׷��һ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::AddGPInfo(tagGPInfo* pGPInfo)
{
	return Update(GT_INVALID, pGPInfo, 1, GT_INVALID, (&CLoongDB::FormatAddGPInfo));
}

//-------------------------------------------------------------------------------------------------------
// ɾ��һ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::RemoveGPInfo(tagGPInfoKey* pGPInfoKey)
{
	return Update(GT_INVALID, pGPInfoKey, 1, GT_INVALID, (&CLoongDB::FormatRemoveGPInfo));
}

//-------------------------------------------------------------------------------------------------------
// ɾ��ָ���������е��Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::RemoveGuildGPInfo(DWORD dwGuildID)
{
	return Update(dwGuildID, NULL, 1, GT_INVALID, (&CLoongDB::FormatRemoveGuildGPInfo));
}

//-------------------------------------------------------------------------------------------------------
// �����Ź�����Ӧ���б�
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateGPInfo(tagGPInfoKey* pGPInfoKey, DWORD dwNewParticipator)
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	// ��ʽ������
	pStream->SetSelect("group_purchase", "ParticipatorNum,ParticipatorList");
	pStream->SetWhere();
	pStream->FillString("GuildID=") << pGPInfoKey->dwGuildID;
	pStream->FillString(" and RoleID=") << pGPInfoKey->dwRoleID;
	pStream->FillString(" and MallID=") << pGPInfoKey->dwMallID;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);

#ifdef _DEBUG
	IMSG(_T("%s\r\n"), m_pUtil->AnsiToUnicode(pStream->GetBuf()));
	IMSG(_T("Results Row Count: %d\n"), P_VALID(pResult) ? pResult->GetRowCount() : 0);
#endif

	// �ͷ���������
	m_pDBLoong->ReturnConnection(pCon);
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		// �����µ���Ӧ���б�
		INT16 nParticipatorNum = (*pResult)[0].GetShort();

		BYTE *pBuffer = new BYTE[sizeof(tagGPInfo)+nParticipatorNum*sizeof(DWORD)];
		tagGPInfo *pGPInfo = (tagGPInfo*)pBuffer;
		(*pResult)[1].GetBlob(pGPInfo->dwParticipators, (*pResult)[1].GetLen());
		pGPInfo->dwParticipators[nParticipatorNum] = dwNewParticipator;
		pGPInfo->nParticipatorNum = nParticipatorNum + 1;
		pGPInfo->dwGuildID = pGPInfoKey->dwGuildID;
		pGPInfo->dwRoleID = pGPInfoKey->dwRoleID;
		pGPInfo->dwMallID = pGPInfoKey->dwMallID;

		// ���µ����ݿ�
		Update(GT_INVALID, pGPInfo, 1, GT_INVALID, (&CLoongDB::FormatUpdateGPInfo));

		SAFE_DEL_ARRAY(pBuffer);
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// �����Ź�����Ӧ���б�
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateGPTime()
{
	// ���µ����ݿ�
	Update(GT_INVALID, NULL, 1, GT_INVALID, (&CLoongDB::FormatUpdateGPTime));

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ����VIP̯λ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ApplyVIPStall(tagVipStall2DB* pInfo)
{
	// ���µ����ݿ�
	Update(GT_INVALID, pInfo, 1, GT_INVALID, (&CLoongDB::FormatApplyVIPStall));

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ����VIP̯λʣ��ʱ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateVIPStallTime()
{
	// ���µ����ݿ�
	Update(GT_INVALID, NULL, 1, GT_INVALID, (&CLoongDB::FormatUpdateVIPStallTime));

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ�����������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateTreasureSum(DWORD dwRoleID, INT& nSum)
{
	// ���µ����ݿ�
	Update(dwRoleID, &nSum, 1, GT_INVALID, (&CLoongDB::FormatUpdateTreasureSum));

	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
// ���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::RoleGetMarriage(DWORD dwRoleID1, DWORD dwRoleID2)
{
	return Update(dwRoleID1, &dwRoleID2, 1, GT_INVALID, (&CLoongDB::FormatRoleGetMarriage));
}
//-------------------------------------------------------------------------------------------------------
// ���
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::RoleBreakMarriage(DWORD dwRoleID1, DWORD dwRoleID2)
{
	return Update(dwRoleID1, &dwRoleID2, 1, GT_INVALID, (&CLoongDB::FormatRoleBreakMarriage));
}
//*********************************************************************************************************

//-------------------------------------------------------------------------------------------------------
// ����sql
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatRoleGetMarriage(Beton::MyStream *pStream, DWORD dwRoleID1, LPVOID pData)
{
	MTRANS_POINTER(p, pData, DWORD);
	pStream->Clear();

	pStream->SetUpdate("roledata");
	pStream->FillString("LoverID=") << *p;
	pStream->FillString(",HaveWedding=0");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID1;
}
//-------------------------------------------------------------------------------------------------------
// ����sql
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatRoleBreakMarriage(Beton::MyStream *pStream, DWORD dwRoleID1, LPVOID pData)
{
	MTRANS_POINTER(p, pData, DWORD);
	pStream->Clear();

	pStream->SetUpdate("roledata");
	pStream->FillString("LoverID = 4294967295");
	pStream->FillString(",HaveWedding=0");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID1;
}
//-------------------------------------------------------------------------------------------------------
// ���ѵ�׷��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertFriend(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pFriendSave)
{
	MTRANS_POINTER(p, pFriendSave, tagFriendSave);
	
	pStream->SetInsert("friend");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",FriendID=") << p->dwFriendID;
	pStream->FillString(",GroupID=") << p->nGroupID;
}

//-------------------------------------------------------------------------------------------------------
// ���ѵĸ���SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateFriend(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pFriendSave)
{
	MTRANS_POINTER(p, pFriendSave, tagFriendSave);
	
	pStream->SetUpdate("friend");
	pStream->FillString("GroupID=") << p->nGroupID;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and FriendID=") << p->dwFriendID;
}

//-------------------------------------------------------------------------------------------------------
// ���ѵ�ɾ��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteFriend(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pFriendID)
{
	pStream->SetDelete("friend");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and FriendID=") << (*(DWORD *)pFriendID);
}

//-------------------------------------------------------------------------------------------------------
// ɾ�����Ժ͸ý�ɫ��صļ�¼SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteAllFriRel(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDummy)
{
	pStream->SetDelete("friend");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" or FriendID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�׷��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertFriendship(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pFriendshipSave)
{
	MTRANS_POINTER(p, pFriendshipSave, tagFriendshipSave);

	ASSERT(dwRoleID < p->dwFriendID);

	pStream->SetInsert("friendship");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",FriendID=") << p->dwFriendID;
	pStream->FillString(",FriVal=") << p->nFriVal;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵĸ���SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateFriendship(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pFriendshipSave)
{
	MTRANS_POINTER(p, pFriendshipSave, tagFriendshipSave);

	ASSERT(dwRoleID < p->dwFriendID);

	pStream->SetUpdate("friendship");
	pStream->FillString("FriVal=") << p->nFriVal;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and FriendID=") << p->dwFriendID;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�ɾ��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteFriendship(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pFriendID)
{
	ASSERT(dwRoleID < (*(DWORD *)pFriendID));

	pStream->SetDelete("friendship");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and FriendID=") << (*(DWORD *)pFriendID);
}

//-------------------------------------------------------------------------------------------------------
// �Ӻ��Ѷȱ���ɾ���͸ý�ɫ��ص����м�¼SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteAllFriValRel(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDummy)
{
	pStream->SetDelete("friendship");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" or FriendID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�ɾ��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertBlack(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pBlackID)
{
	pStream->SetInsert("blacklist");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",BlackID=") << (*(DWORD *)pBlackID);
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�ɾ��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteBlack(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pBlackID)
{
	pStream->SetDelete("blacklist");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and BlackID=") << (*(DWORD *)pBlackID);
}

//-------------------------------------------------------------------------------------------------------
// ��е�׷��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertEnemy(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEnemyID)
{
	pStream->SetInsert("enemy");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",EnemyID=") << (*(DWORD *)pEnemyID);
}

//-------------------------------------------------------------------------------------------------------
// ��е�ɾ��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteEnemy(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEnemyID)
{
	pStream->SetDelete("enemy");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and EnemyID=") << (*(DWORD *)pEnemyID);
}

//-------------------------------------------------------------------------------------------------------
// �ƺŵĲ���SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertTitle(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagTitleSave);
	
	pStream->SetReplace("title");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",ID=") << p->u16TitleID;
	pStream->FillString(",Count=") << p->dwStateMark;
}
//-------------------------------------------------------------------------------------------------------
// ɾ���ƺŵ�sql���
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatRemoveTitle(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagTitleSave);
	
	pStream->SetDelete("title");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and ID=") << p->u16TitleID;
}
//-------------------------------------------------------------------------------------------------------
// SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertTask(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pAcceptQuest)
{
	MTRANS_POINTER(p, pAcceptQuest, tagAcceptQuest);
	
	pStream->SetInsert("task");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",StartTime=") << p->dwAcceptTime;
}

//-------------------------------------------------------------------------------------------------------
// ��е�ɾ��SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteTask(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTaskID)
{
	pStream->SetDelete("task");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and TaskID=") << (*(DWORD *)pTaskID);
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ���˻���SQL����ʽ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertYBAccount(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetInsert("ybaccount");
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻���Ԫ������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatAccountYB(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("ybaccount");
	pStream->FillString("YuanBao=") <<(*(INT32*)pData);

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻��Ľ�Ǯ����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatAccountSilver(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pMoney)
{
	MTRANS_POINTER(p, pMoney, tagMoney);

	pStream->SetUpdate("ybaccount");
	pStream->FillString("Gold=") <<p->nGold;
	pStream->FillString(",Silver=") <<p->nSilver;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻����۶������ύ״̬
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSubmitSell(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("ybaccount");
	pStream->FillString("bSellOrder=") << (*(BYTE*)pData);

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻��չ��������ύ״̬
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSubmitBuy(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("ybaccount");
	pStream->FillString("bBuyOrder=") << (*(BYTE*)pData);

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertOrder(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagYBOrder);

	pStream->SetInsert("yuanbaoorder");
	pStream->FillString("dwID=") << p->dwID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",OrderType=") << p->nType;
	pStream->FillString(",Price=") << p->nPrice;
	pStream->FillString(",Number=") << p->nNum;
	pStream->FillString(",OrderMode=") << p->nYBMode;
	pStream->FillString(",StartTime=") << p->dwStartTime;
}

//-------------------------------------------------------------------------------------------------------
// ������ر�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatCompleteOrder(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagCompleteOrder);

	pStream->SetUpdate("yuanbaoorder");
	pStream->FillString("OrderMode=") << p->nYBOMode;
	pStream->FillString(",EndTime=") << p->dwEndTime;

	pStream->SetWhere();
	pStream->FillString("dwID=") << p->dwID;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatRefreshOrder(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagRefresh);

	pStream->SetUpdate("yuanbaoorder");
	pStream->FillString("AvgPrice=") << p->nAvgPrice;
	pStream->FillString(",DealNum=") << p->nDealNum;

	pStream->SetWhere();
	pStream->FillString("dwID=") << p->dwID;
}

//-------------------------------------------------------------------------------------------------------
// ɾ��Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteYBOrder(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetDelete("yuanbaoorder");
	pStream->SetWhere();
	pStream->FillString("StartTime<") << (*(DWORD*)pData);
}


//-------------------------------------------------------------------------------------------------------
// ��ʽ������Log���
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertBaiBaoLog(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagBaiBaoRecord);

	// ��ȡ����
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetInsert("log_baibao");
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(",RoleID=") << p->dwRoleID;

	// LoongDB����ʱ��
	if(DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwTime))
	{
		pStream->FillString(",LogTime='").FillString(m_szTime);
		pStream->FillString("'");
	}
	else
	{
		IMSG(_T("Error: Func DwordTime2DataTime() run error in CLoongDB::CreateRole()!!!!!\r\n"));
		ASSERT(0);
	}

	pStream->FillString(",n16Type=") << p->n16Type;
	pStream->FillString(",TypeID=") << p->dwTypeID;


	//����
	pStream->FillString(",LeaveWords='");
	INT nLen = _tcsnlen(p->szWords, Max_LeaveWord_Length);
	if (nLen > 0)
	{
		pStream->FillBlob(p->szWords, p->n16Size-sizeof(tagBaiBaoRecord)+sizeof(p->szWords), pCon);
	}
	pStream->FillString("'");

	// �ͷ�����
	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ��ʽ��ɾ��Log���
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDeleteBaiBaoLog(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	ASSERT(GT_VALID(dwAccountID));

	MTRANS_POINTER(p, pData, tagBaiBaoRecord);

	pStream->SetDelete("log_baibao");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(" and RoleID=") << p->dwRoleID;
	pStream->FillString(" and n16Type=") << p->n16Type;
	pStream->FillString(" and TypeID=") << p->dwTypeID;

	if(DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwTime))
	{
		pStream->FillString(" and LogTime='").FillString(m_szTime);
		pStream->FillString("'");
	}
	else
	{
		IMSG(_T("Error: Func DwordTime2DataTime() run error in CLoongDB::CreateRole()!!!!!\r\n"));
		ASSERT(0);
	}
}

//-------------------------------------------------------------------------------------------------------
// ɾ������İٱ�����ʷ��¼
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResCountBaiBaoLog(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	INT32 nNum = pRes->GetRowCount();

	if(nNum < MAX_BAIBAO_LOG_NUM) return;

	DWORD dwAccountID = GT_INVALID;
	tagBaiBaoRecord tmpLogInfo = {0};
	DWORD tmpLogTime;
	tmpLogTime = tmpLogInfo.dwTime = GetCurrentDWORDTime();

	for(INT32 i=0; i<nNum; ++i)
	{
		DataTime2DwordTime((*(tagDWORDTime*)&tmpLogTime), (*pRes)[2].GetString(), (*pRes)[2].GetLen());
		if (tmpLogTime <= tmpLogInfo.dwTime)
		{
			dwAccountID = (*pRes)[0].GetDword();
			tmpLogInfo.dwRoleID = (*pRes)[1].GetDword();
			tmpLogInfo.dwTypeID = (*pRes)[4].GetDword();
			tmpLogInfo.n16Type = (*pRes)[3].GetShort();
			DwordTime2DataTime(m_szTime, sizeof(m_szTime), tmpLogTime);
			tmpLogInfo.dwTime = tmpLogTime;
		}
		pRes->NextRow();
	}

	Update(dwAccountID, (LPVOID)&tmpLogInfo, 1, GT_INVALID, &CLoongDB::FormatDeleteBaiBaoLog);
}
//-------------------------------------------------------------------------------------------------------
// ׷��һ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatAddGPInfo(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagGPInfo);

	// ��ȡ����
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetInsert("group_purchase");
	pStream->FillString("GuildID=") << p->dwGuildID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",MallID=") << p->dwMallID;
	pStream->FillString(",ItemPrice=") << p->nPrice;
	pStream->FillString(",ParticipatorNum=") << p->nParticipatorNum;
	pStream->FillString(",RequireNum=") << p->nRequireNum;
	pStream->FillString(",RemainTime=") << p->nRemainTime;

	//��Ӧ���б���ʱֻ�з�����
	pStream->FillString(",ParticipatorList='");
	pStream->FillBlob(p->dwParticipators, sizeof(p->dwParticipators), pCon);
	pStream->FillString("'");

	// �ͷ�����
	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ɾ��һ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatRemoveGPInfo(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagGPInfoKey);

	pStream->SetDelete("group_purchase");
	pStream->SetWhere();
	pStream->FillString("GuildID=") << p->dwGuildID;
	pStream->FillString(" and RoleID=") << p->dwRoleID;
	pStream->FillString(" and MallID=") << p->dwMallID;
}

//-------------------------------------------------------------------------------------------------------
// ɾ��ָ���������е��Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatRemoveGuildGPInfo(Beton::MyStream *pStream, DWORD dwGuildID, LPVOID pData)
{
	pStream->SetDelete("group_purchase");
	pStream->SetWhere();
	pStream->FillString("GuildID=") << dwGuildID;
}

//-------------------------------------------------------------------------------------------------------
// �����Ź���Ϣ��Ӧ���б�
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateGPInfo(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagGPInfo);

	// ��ȡ����
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetUpdate("group_purchase");

	pStream->FillString("ParticipatorNum=") << p->nParticipatorNum;

	//��Ӧ���б�
	pStream->FillString(",ParticipatorList='");
	pStream->FillBlob(p->dwParticipators, p->nParticipatorNum * sizeof(DWORD), pCon);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("GuildID=") << p->dwGuildID;
	pStream->FillString(" and RoleID=") << p->dwRoleID;
	pStream->FillString(" and MallID=") << p->dwMallID;

	// �ͷ�����
	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// �����Ź�ʣ��ʱ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateGPTime(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("group_purchase");

	pStream->FillString("RemainTime=RemainTime-60");
	pStream->SetWhere();
	pStream->FillString("RemainTime>0");
}

//-------------------------------------------------------------------------------------------------------
// ����VIP̯λ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatApplyVIPStall(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagVipStall2DB);

	pStream->Clear();
	pStream->SetUpdate("vip_stall");
	pStream->FillString("RoleID=") << p->dwOwnerID;
	pStream->FillString(",RemainTime=") <<p->nRemainTime;
	pStream->SetWhere();
	pStream->FillString("StallID=") << p->byStallIndex;
}

//-------------------------------------------------------------------------------------------------------
// ����VIP̯λʣ��ʱ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateVIPStallTime(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->Clear();

	pStream->SetUpdate("vip_stall");
	pStream->FillString("RemainTime=RemainTime-60");
	pStream->SetWhere();
	pStream->FillString("RemainTime>0");
}

//-------------------------------------------------------------------------------------------------------
// ����һ��̯λ��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertVIPStall(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagVipStall2DB);

	pStream->SetInsert("vip_stall");
	pStream->FillString("StallID=") << p->byStallIndex;
	pStream->FillString(",RoleID=") << p->dwOwnerID;
	pStream->FillString(",RemainTime=") << p->nRemainTime;
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ�����������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateTreasureSum(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, DWORD);
	pStream->Clear();

	pStream->SetUpdate("roledata");
	pStream->FillString("TreasureSum=") << *p;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ���� -- ���������Ϊ"t1,t2,t3"
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LockTables(LPCSTR szTableNames)
{
	return Update(GT_INVALID, (LPVOID)szTableNames, 1, 0, (&CLoongDB::FormatLockTables));
}

VOID CLoongDB::FormatLockTables(Beton::MyStream *pStream, DWORD dwDummy, LPVOID pData)
{
	pStream->Clear();

	pStream->FillString("Lock Tables ");
	pStream->FillString((LPCSTR)pData);
	pStream->FillString(" WRITE");
}

//-------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UnlockTables()
{
	return Update(GT_INVALID, NULL, 1, 0, (&CLoongDB::FormatUnlockTables));
}

VOID CLoongDB::FormatUnlockTables(Beton::MyStream *pStream, DWORD dwDummy, LPVOID pData)
{
	pStream->Clear();

	pStream->FillString("Unlock Tables");
}
