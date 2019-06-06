//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongProcMsg.cpp
// author: Sxg
// actor:
// data: 2008-06-04
// last:
// brief: �������Ϸ���������յ�����Ϣ(�����Ϣ���ݵĺϷ��ԡ�����DB��������)
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/clandata_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/msg_quest.h"
#include "../ServerDefine/msg_skill.h"
#include "../ServerDefine/msg_title.h"
#include "../ServerDefine/msg_vcard.h"
#include "../ServerDefine/msg_famehall.h"
#include "../ServerDefine/msg_yuanbao.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../ServerDefine/msg_strength.h"
#include "../ServerDefine/msg_marriage.h"
#include "../ServerDefine/activity_define.h"
#include "../ServerDefine/pet_define.h"
#include "LoongDB.h"
#include "Loong.h"

#include "../ServerDefine/msg_sexternal_links.h"


const INT MAX_ROLEINFO_NUM_PER_MSG		= 100000;	// ÿ����Ϣ��roleinfo��������
const INT MAX_ROLEYBORDER_NUM_PER_MSG   = 100;		// ÿ����Ϣ��tagyuanbaoorder��������
const INT MAX_GROUPPURCHASE_CAPACITY_PER_MSG	= 1000000;	// ÿ����Ϣ��tagGPInfo��ռ�ĵ��������
const INT MAX_ACTIVITY_NUM_PER_MSG = 50;			// ÿ����Ϣ��tagActivityData��������
//-------------------------------------------------------------------------------------------------------
// ��ʼ����Ϸ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InitWorldOK(DWORD pDBMsg, DWORD dwDummy)
{
	// ����Ϸ����������
	SendMsg( (LPVOID)pDBMsg, ((tagN_Certification*)pDBMsg)->dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��Ϸ����ر�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::WorldShutDown(DWORD pDBMsg, DWORD dwDummy)
{
	// ����Ϸ����������
	tagNDBS_WorldShutDown send;

	SendMsg( &send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��LoongWorld���س�ʼ�����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoongWorldInitOK(DWORD pDBMsg, DWORD dwDummy)
{
	tagNDBS_LoongWorldInitOK	send;
	SendMsg( &send, send.dwSize); 
	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����������Ҽ�����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadAllRoleInfo(DWORD pDBMsg, DWORD dwDummy)
{
	//	MGET_MSG(pRecv, pDBMsg, NDBC_LoadAllRoleInfo);

	INT32 nRoleNum = m_pDB->GetRoleCount();
	if(0 == nRoleNum)
	{
		tagNDBS_LoadAllRoleInfo Load;
		Load.dwErrorCode = E_Success;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagRoleInfo *pRoleInfo = new tagRoleInfo[nRoleNum];
	tagRoleInfo *pCur = pRoleInfo;
	INT nNum;
	if(!m_pDB->LoadAllRoleInfo(pRoleInfo, nNum))
	{
		delete []pRoleInfo;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagRoleInfo) + sizeof(tagNDBS_LoadAllRoleInfo)];

	// ��ʼ����Ϣͷ
	tagNDBS_LoadAllRoleInfo LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_LoadAllRoleInfo *pLoad = (tagNDBS_LoadAllRoleInfo *)pBuff;
	pLoad->dwErrorCode = E_Success;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	//if(nRoleNum != nNum)
	//{
	//	// ���ط���Ϣ //??
	//	
	//	ASSERT(nRoleNum == nNum);
	//}

	// �ְ�(�������ݰ�ͷ4���ֽ��еĸ�8λ�������ܣ����Զ԰��Ĵ�С������)
	while(nRoleNum > 0)
	{
		if(nRoleNum > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(pLoad->RoleInfo, pCur, MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagRoleInfo));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(tagRoleInfo) + sizeof(tagNDBS_LoadAllRoleInfo);
			pLoad->nNum = MAX_ROLEINFO_NUM_PER_MSG;

			nRoleNum -= MAX_ROLEINFO_NUM_PER_MSG;
			pCur = &pCur[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->RoleInfo, pCur, nRoleNum * sizeof(tagRoleInfo));

			// ����������Ϣ��С
			pLoad->dwSize = (nRoleNum - 1) 
				* sizeof(tagRoleInfo) + sizeof(tagNDBS_LoadAllRoleInfo);
			pLoad->nNum = nRoleNum;

			nRoleNum = 0;
			pCur = NULL;
		}

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pRoleInfo;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������ҵ�Ԫ�������˻�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadAllYBAccount(DWORD pDBMsg, DWORD dwDummy)
{
	INT nAccountNum = m_pDB->GetAccountCount();
	if(0 == nAccountNum)
	{
		tagNDBS_LoadAllYBAccount Load;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagYBAccount *pYBAccount = new tagYBAccount[nAccountNum];
	tagYBAccount *pCur = pYBAccount;
	INT nNum;
	if(!m_pDB->LoadAllYBAccount(pYBAccount, nNum))
	{
		delete []pYBAccount;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYBAccount) + sizeof(tagNDBS_LoadAllYBAccount)];

	// ��ʼ����Ϣͷ
	tagNDBS_LoadAllYBAccount LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_LoadAllYBAccount *pLoad = (tagNDBS_LoadAllYBAccount*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	while(nAccountNum > 0)
	{
		if(nAccountNum > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(pLoad->Account, pCur, MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYBAccount));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(tagYBAccount) + sizeof(tagNDBS_LoadAllYBAccount);
			pLoad->nNum = MAX_ROLEINFO_NUM_PER_MSG;

			nAccountNum -= MAX_ROLEINFO_NUM_PER_MSG;
			pCur = &pCur[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->Account, pCur, nAccountNum * sizeof(tagYBAccount));

			// ����������Ϣ��С
			pLoad->dwSize = (nAccountNum - 1) 
				* sizeof(tagYBAccount) + sizeof(tagNDBS_LoadAllYBAccount);
			pLoad->nNum = nAccountNum;

			nAccountNum = 0;
			pCur = NULL;
		}

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pYBAccount;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���е�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadAllYBOrder(DWORD pDBMsg, DWORD dwDummy)
{
	INT nOrderNum = m_pDB->GetOrderCount();
	if(0 == nOrderNum)
	{
		tagNDBS_LoadAllOrder Load;
		Load.nNum = 0;
		Load.dwMaxOrderID = m_pDB->GetMaxOrderID();
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagYuanBaoOrder *pYBOrder = new tagYuanBaoOrder[nOrderNum];
	tagYuanBaoOrder *pCur = pYBOrder;
	INT nNum;
	if(!m_pDB->LoadAllYBOrder(pYBOrder, nNum))
	{
		delete []pYBOrder;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_LoadAllOrder)];

	// ��ʼ����Ϣͷ
	tagNDBS_LoadAllOrder LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_LoadAllOrder *pLoad = (tagNDBS_LoadAllOrder*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	while(nOrderNum > 0)
	{
		if(nOrderNum > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(pLoad->YBOrder, pCur, MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYuanBaoOrder));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_LoadAllOrder);
			pLoad->nNum = MAX_ROLEINFO_NUM_PER_MSG;

			nOrderNum -= MAX_ROLEINFO_NUM_PER_MSG;
			pCur = &pCur[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->YBOrder, pCur, nOrderNum * sizeof(tagYuanBaoOrder));

			// ����������Ϣ��С
			pLoad->dwSize = (nOrderNum - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_LoadAllOrder);
			pLoad->nNum = nOrderNum;

			nOrderNum = 0;
			pCur = NULL;
		}
		pLoad->dwMaxOrderID	= m_pDB->GetMaxOrderID();

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pYBOrder;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ѯһ������ҽ��׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetRoleYBOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetRoleYBOrder);
	INT nOrderNum = 0;
	m_pDB->GetRoleYBOrderNum(pRecv->dwRoleID, nOrderNum);

	if(0 == nOrderNum)
	{
		tagNDBS_GetRoleYBOrder Load;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagYuanBaoOrder *pYBOrder = new tagYuanBaoOrder[nOrderNum];
	tagYuanBaoOrder *pCur = pYBOrder;
	INT nNum;
	if(!m_pDB->LoadRoleAllYBOrder(pYBOrder, nNum, pRecv->dwRoleID))
	{
		delete []pYBOrder;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[nOrderNum * sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_GetRoleYBOrder)];

	// ��ʼ����Ϣͷ
	tagNDBS_GetRoleYBOrder LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_GetRoleYBOrder *pLoad = (tagNDBS_GetRoleYBOrder*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	while(nOrderNum > 0)
	{
		if(nOrderNum > MAX_ROLEYBORDER_NUM_PER_MSG)
		{
			memcpy(pLoad->YBOrder, pCur, MAX_ROLEYBORDER_NUM_PER_MSG * sizeof(tagYuanBaoOrder));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ROLEYBORDER_NUM_PER_MSG - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_GetRoleYBOrder);
			pLoad->nNum = MAX_ROLEYBORDER_NUM_PER_MSG;

			nOrderNum -= MAX_ROLEYBORDER_NUM_PER_MSG;
			pCur = &pCur[MAX_ROLEYBORDER_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->YBOrder, pCur, nOrderNum * sizeof(tagYuanBaoOrder));

			// ����������Ϣ��С
			pLoad->dwSize = (nOrderNum - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_GetRoleYBOrder);
			pLoad->nNum = nOrderNum;

			nOrderNum = 0;
			pCur = NULL;
		}

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pYBOrder;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ(�������ȼ������κ͵�ǰ���ڵ�ͼ��)
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadSimRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadSimRole);

	m_pLoadSimRole->dwErrorCode = E_Success;
	m_pLoadSimRole->dwAccountID = pRecv->dwAccountID;
	m_pLoadSimRole->sAccountCommon.Init();

	if(!m_pDB->LoadSimRole(m_pLoadSimRole->dwAccountID, m_pLoadSimRole->nNum, m_pLoadSimRole->SimRole))
	{
		// û�н�ɫʱ��Ҳ��ִ�е��ô�
		//m_pLoadSimRole->dwErrorCode = E_DBLoad_SimRoleFailed;
		m_pLoadSimRole->nNum = 0;		// ��ȡ����ʱ����С������С
	}

	// ����
	m_pDB->LockAccountCommon();

	// ��ȡ��ȫ��(accountrel���б�����ؼ�¼������ȡʧ�ܣ���˵�����ݿ����ӳ�������)
	if(!m_pDB->LoadAccountCommon(&m_pLoadSimRole->sAccountCommon, pRecv->dwAccountID))
	{
		// ��accountrel���в����¼
		if(!m_pDB->InsertAccountCommon(pRecv->dwAccountID, pRecv->szAccount))
		{
			// ���ط�����Ϣ//??


			// ����
			IMSG(_T("\n\n\n///////////////////////////////////////\n"));
			IMSG(_T("Caution: \n\tCritical Error! DB connect lost! May be need to restart server!\n"));
			IMSG(_T("///////////////////////////////////////\n\n\n"));

			m_pLoadSimRole->dwErrorCode = E_DBLoad_SimRoleFailed;
		}

		// �ٴζ�ȡ
		m_pDB->LoadAccountCommon(&m_pLoadSimRole->sAccountCommon, pRecv->dwAccountID);
	}

	// ����
	m_pDB->UnlockTables();

	// ����������Ϣ��С
	m_pLoadSimRole->dwSize = sizeof(tagNDBS_LoadSimRole) 
		- sizeof(tagSimRole) * (MAX_ROLENUM_ONEACCOUNT - m_pLoadSimRole->nNum);

	// ��������Ϸ������
	SendMsg( m_pLoadSimRole, m_pLoadSimRole->dwSize);

	return m_pLoadSimRole->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ��Ϣ(ѡ����ɫ������Ϣ)
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadRole);

	ZeroMemory(m_pBuffer, X_ROLE_BUFF_SIZE);
	m_pLoadRole->dwID = m_pLoadRole->Crc32("NDBS_LoadRole");
	m_pLoadRole->dwAccountID = pRecv->dwAccountID;
	m_pLoadRole->dwErrorCode = E_Success;
	m_pLoadRole->dwRoleID = pRecv->dwRoleID;
	if(!m_pDB->LoadRole(pRecv->dwAccountID, pRecv->dwRoleID, &m_pLoadRole->RoleData))
	{
		m_pLoadRole->dwErrorCode = E_DBLoad_RoleDataFailed;
	}

	// ���¼���m_LoadRole�Ĵ�С
	m_pLoadRole->dwSize = sizeof(tagNDBS_LoadRole) + m_pLoadRole->RoleData.dwSize - sizeof(m_pLoadRole->RoleData);

	// ��������Ϸ������
	SendMsg( m_pLoadRole, m_pLoadRole->dwSize);

	return m_pLoadRole->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ����洢��ɫ����
//-------------------------------------------------------------------------------------------------------
VOID CLoong::SaveRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveRole);

	m_pDB->SaveRole(pRecv->dwRoleID, &pRecv->RoleData);
}

////-------------------------------------------------------------------------------------------------------
//// ɾ����ҵ�buff������item/equip��.����������ݿ�����Ҷ���ļ�¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoong::DeleteRecord(DWORD pDBMsg, DWORD dwDummy)
//{
//	MGET_MSG(pRecv, pDBMsg, NDBC_RecordDel);
//
//	m_pDB->DeleteRecord(pRecv->dwRoleID, &pRecv->Record);
//}

//-------------------------------------------------------------------------------------------------------
// ���󴴽���ɫ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::CreateRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CreateRole);

	m_pCreateRole->dwErrorCode = E_Success;
	m_pCreateRole->dwAccountID = pRecv->dwAccountID;
	memcpy(m_pCreateRole->SimRole.szRoleName, 
		pRecv->RoleInfo.szRoleName, sizeof(m_pCreateRole->SimRole.szRoleName));


	DWORD dwRoleNameCrc = GT_INVALID;
	DWORD dwNewRoleID = GT_INVALID;
	if(!m_pDB->CreateRole(dwNewRoleID, pRecv->dwAccountID, pRecv->RoleInfo, dwRoleNameCrc))
	{
		m_pCreateRole->dwErrorCode = E_DBInsert_RoleFailed;
	}
	else // ������ɫ�ɹ�
	{
		// ����������Ϣ
		m_pDB->InsertClanData(dwNewRoleID);

		// ������ɫ����
		m_pDB->InsertVCard(dwNewRoleID);

		// ��ʼ����ɫ������Ϣ
		memcpy(&m_pCreateRole->SimRole.Avatar, &pRecv->RoleInfo.Avatar, sizeof(m_pCreateRole->SimRole.Avatar));
		m_pCreateRole->SimRole.dwRoleID = dwNewRoleID;
		m_pCreateRole->SimRole.dwMapID = pRecv->RoleInfo.dwMapID;
		m_pCreateRole->SimRole.byLevel = pRecv->RoleInfo.byLevel;

		// ��ʼ��tagRoleInfo
		memcpy(m_pCreateRole->RoleInfo.szRoleName, 
			pRecv->RoleInfo.szRoleName, sizeof(m_pCreateRole->RoleInfo.szRoleName));
		m_pCreateRole->RoleInfo.dwAccountID = pRecv->dwAccountID;
		m_pCreateRole->RoleInfo.dwRoleID = dwNewRoleID;
		m_pCreateRole->RoleInfo.dwRoleNameCrc = dwRoleNameCrc;
		m_pCreateRole->RoleInfo.dwMapID = pRecv->RoleInfo.dwMapID;
		m_pCreateRole->RoleInfo.byLevel = pRecv->RoleInfo.byLevel;
		m_pCreateRole->RoleInfo.bySex = pRecv->RoleInfo.Avatar.bySex;
		m_pCreateRole->RoleInfo.dwLoverID = GT_INVALID;
		m_pCreateRole->RoleInfo.byClass = 1; //Ĭ������
	}

	// ��������Ϸ������
	SendMsg( m_pCreateRole, m_pCreateRole->dwSize);

	return m_pLoadSimRole->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ����ɾ����ɫ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelRole);

	//// ��������������
	//if(m_pDB->BackupRole(pRecv->dwAccountID, pRecv->dwRoleID) != E_Success)
	//{
	//	// ���ط���Ϣ
	//}

	// ɾ������������
	//SendDelRole.dwErrorCode = m_pDB->DelRole(pRecv->dwAccountID, pRecv->dwRoleID);
	//m_pDB->DelClanData(pRecv->dwRoleID);
	//m_pDB->DelVCard(pRecv->dwRoleID);
	//
	//if(SendDelRole.dwErrorCode != E_Success)
	//{
	//	// ���ط���Ϣ
	//}
	//else
	//{
	//	// ��¼��������Ϣ -- ��֤��,�ü�¼��ʧ��,�������⴦��
	//	m_pDB->RecordInfoDelRole(pRecv->dwAccountID, pRecv->dwRoleID, pRecv->szRoleName, pRecv->szIP);
	//}
	// �ֿ����Ʒ
	DWORD dwMiscArg = (EICT_RoleWare & 0xff);
	dwMiscArg |= (EBS_Bind & 0xff) << 16;

	tagItemData ItemSerials[1000] = {0};
	PVOID pItemSerials = ItemSerials;
	INT nNum = 0;
	if (m_pDB->LoadRoleItemSerial(pRecv->dwRoleID, pItemSerials, nNum, dwMiscArg))
	{
		for(INT i=0; i<nNum; ++i)
		{
			if (MIsEquipment(ItemSerials[i].dwTypeID))
			{
				m_pDB->DelEquip(ItemSerials[i].n64Serial);
			}
			else
			{
				m_pDB->DelItem(ItemSerials[i].n64Serial);
			}
		}
	}

	DWORD dwPetIDs[MAX_PETSOUL_NUM * 2] = {GT_INVALID};
	INT nPetIDNum = 0;
	if (m_pDB->LoadRolePetIDs(dwPetIDs, pRecv->dwRoleID, nPetIDNum))
	{
		for(INT i=0; i<nPetIDNum && GT_VALID(dwPetIDs[i]); ++i)
		{
			// tbd:ɾ�����＼��
			m_pDB->DelPetSkill(GT_INVALID, dwPetIDs[i]);
			// ɾ������װ��
			m_pDB->DelPetEquip(dwPetIDs[i]);
			// ɾ����������
			m_pDB->DeletePetSoul(dwPetIDs[i]);
		}
	}

	// ���ý�ɫ����ɾ����־λ
	m_pDB->SetRoleRemoveFlag(pRecv->dwRoleID, pRecv->dwTime);

	// ��¼��������Ϣ -- ��֤��,�ü�¼��ʧ��,�������⴦��
	m_pDB->RecordInfoDelRole(pRecv->dwAccountID, pRecv->dwRoleID, pRecv->szRoleName, pRecv->szIP);

	// ��������Ϸ������
	tagNDBS_DelRole SendDelRole;
	SendDelRole.dwErrorCode = E_Success;
	SendDelRole.dwAccountID = pRecv->dwAccountID;
	SendDelRole.dwRoleID = pRecv->dwRoleID;

	// ��������Ϸ������ 
	SendMsg( &SendDelRole, SendDelRole.dwSize);

	return SendDelRole.dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ����ָ���ɫ//??
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ResumeRole(DWORD pDBMsg, DWORD dwDummy)
{
	return 0;
}

//-------------------------------------------------------------------------------------------------------
// �����޸Ľ�ɫ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ChangeRoleName(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ChangeRoleName);

	tagNDBS_ChangeRoleName	Send;	// ���ô���Ӧ�ò���ܶ࣬��������ʱ��������
	Send.dwAccountID = pRecv->dwAccountID;
	Send.dwRoleID = pRecv->dwRoleID;
	Send.dwErrorCode = E_Success;
	memcpy(Send.szNewRoleName, pRecv->szNewRoleName, sizeof(Send.szNewRoleName));

	if(!m_pDB->ChangeRoleName(pRecv->dwAccountID, pRecv->dwRoleID, pRecv->szNewRoleName))
	{
		Send.dwErrorCode = E_ChangeRoleNameFailed;
	}

	// ��������Ϸ������
	SendMsg( &Send, Send.dwSize);

	return Send.dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// �����޸Ľ�ɫ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ChangeRoleGuild(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ChangeRoleGuild);

	if(!m_pDB->ChangeRoleGuild(pRecv->dwRoleID, pRecv->dwGuildID))
	{
		ASSERT(0);
		IMSG(_T("\n\nCaution:\n\tRole<id: %u> guildid change failed!\n"), pRecv->dwRoleID);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �޸Ľ�ɫDressMdID
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ChangeRoleDressMdID(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearRoleDressMdID);

	if(!m_pDB->ChangeRoleDressMdID(pRecv->dwRoleID, pRecv->wNewVal))
	{
		ASSERT(0);
		IMSG(_T("\n\nCaution:\n\tRole<id: %u> dressmdid change failed!\n"), pRecv->dwRoleID);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���ѵ�׷��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertFriend(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertFriend);

	if(!m_pDB->InsertFriend(pRecv->dwRoleID, &pRecv->FriendSave))
	{
		return E_DBInsert_FriendFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���ѵĸ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateFriend(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateFriend);

	if(!m_pDB->UpdateFriend(pRecv->dwRoleID, &pRecv->FriendSave))
	{
		return E_DBInsert_FriendFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���ѵ�ɾ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteFriend(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteFriend);

	if(!m_pDB->DeleteFriend(pRecv->dwRoleID, pRecv->dwFriendID))
	{
		return E_DBDelete_FriendFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ӻ��ѱ���ɾ�����к͸ý�ɫ��صļ�¼
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteAllFriRel(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelAllFriRel);

	if(!m_pDB->DeleteAllFriRel(pRecv->dwRoleID))
	{
		return E_DBDelete_FriendFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�׷��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertFriendship(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertFriVal);

	if(!m_pDB->InsertFriendship(pRecv->dwRoleID, &pRecv->FriendshipSave))
	{
		return E_DBInsert_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵĸ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateFriendship(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateFriVal);

	if(!m_pDB->UpdateFriendship(pRecv->dwRoleID, &pRecv->FriendshipSave))
	{
		return E_DBInsert_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�ɾ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteFriendship(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteFriend);

	if(!m_pDB->DeleteFriendship(pRecv->dwRoleID, pRecv->dwFriendID))
	{
		return E_DBDelete_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ӻ��Ѷ��б���ɾ��������ý�ɫ��صļ�¼
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteAllFriValRel(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelAllFriValRel);

	if(!m_pDB->DeleteAllFriValRel(pRecv->dwRoleID))
	{
		return E_DBDelete_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������׷��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertBlack(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertBlack);

	if(!m_pDB->InsertBlack(pRecv->dwRoleID, pRecv->dwBlackID))
	{
		return E_DBInsert_BlackFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteBlack(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteBlack);


	if(!m_pDB->DeleteBlack(pRecv->dwRoleID, pRecv->dwBlackID))
	{
		return E_DBDelete_BlackFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��е�׷��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertEnemy(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertEnemy);

	tagNDBS_InsertEnemy Send;
	Send.dwErrorCode = E_Success;
	Send.dwRoleID = pRecv->dwRoleID;
	Send.dwEnemyID = pRecv->dwEnemyID;

	if(!m_pDB->InsertEnemy(pRecv->dwRoleID, pRecv->dwEnemyID))
	{
		Send.dwErrorCode = E_DBInsert_EnemyFailed;
	}

	// ��������Ϸ������
	SendMsg( &Send, Send.dwSize);

	return Send.dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��е�ɾ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteEnemy(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteEnemy);

	tagNDBS_DeleteEnemy Send;
	Send.dwErrorCode = E_Success;
	Send.dwRoleID = pRecv->dwRoleID;
	Send.dwEnemyID = pRecv->dwEnemyID;

	if(!m_pDB->DeleteEnemy(pRecv->dwRoleID, pRecv->dwEnemyID))
	{
		Send.dwErrorCode = E_DBDelete_EnemyFailed;
	}

	// ��������Ϸ������
	SendMsg( &Send, Send.dwSize);

	return Send.dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::AcceptQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_AcceptQuest);

	if(!m_pDB->InsertTask(pRecv->dwRoleID, &pRecv->acceptQuest))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::CompleteQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CompleteQuest);

	if(!m_pDB->SaveTaskDone(pRecv->dwRoleID, &pRecv->QuestDone, 1)
		|| !m_pDB->DeleteTask(pRecv->dwRoleID, pRecv->QuestDone.u16QuestID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DiscardQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DiscardQuest);

	if(!m_pDB->DeleteTask(pRecv->dwRoleID, pRecv->u16QuestID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ʒ64λ��ŵ������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetItemInfo(DWORD pDBMsg, DWORD dwDummy)
{
	//MGET_MSG(pRecv, pDBMsg, NDBC_GetItemInfo);

	tagNDBS_GetItemInfo send;

	m_pDB->GetMaxMinSerial(send.n64MaxSerial, send.n64MinSerial);

	// ��������Ϸ������
	SendMsg( &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����Ʒ���ٱ�����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadNewItem2BaiBao(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadBaiBao);

	if(pRecv->nFreeSpaceSize <=0 || pRecv->nFreeSpaceSize > SPACE_BAIBAO_BAG)
	{
		return GT_INVALID;
	}

	m_pLoadBaiBao->dwAccountID = pRecv->dwAccountID;
	m_pLoadBaiBao->dwErrorCode = E_Success;

	INT32 nTmpSize = 0;

	if(!m_pDB->LoadItemBaiBao(pRecv->dwAccountID, pRecv->nFreeSpaceSize, pRecv->n64Serial, m_pLoadBaiBao->nRetNum, nTmpSize, m_pLoadBaiBao->byData))
	{
		m_pLoadBaiBao->dwErrorCode = E_DBLoad_BaiBao_Failed;
	}

	// ���¼�����Ϣ��С
	m_pLoadBaiBao->dwSize = sizeof(tagNDBS_LoadBaiBao) + nTmpSize - sizeof(m_pLoadBaiBao->byData);

	// ��������Ϸ������
	SendMsg( m_pLoadBaiBao, m_pLoadBaiBao->dwSize);

	return m_pLoadBaiBao->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ�����Ԫ����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadBaiBaoYuanBaoInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadBaiBaoYuanBao);

	INT32 nYuanBao;
	if (!m_pDB->LoadBaiBaoYuanBao(pRecv->dwAccountID, nYuanBao))
	{
		return GT_INVALID;
	}

	tagNDBS_LoadBaiBaoYuanBao send;
	send.dwAccountID	= pRecv->dwAccountID;
	send.nBaiBaoYuanBao	= nYuanBao;

	// ��������Ϸ������
	SendMsg( &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��item_baibao����ɾ��ָ����Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelBaiBaoItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelBaiBaoItem);

	if(!m_pDB->DelItem(pRecv->n64Serial, "item_baibao"))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��item_baibao����ɾ��ָ��װ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelBaiBaoEquip(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelBaiBaoEquip);

	if(m_pDB->DelItem(pRecv->n64Serial, "item_baibao"))
	{
		return m_pDB->DelEquipSpec(pRecv->n64Serial, "equip_baibao");
	}

	return FALSE;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ҫ��¼����ƷTypeID
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadItemNeedLog(DWORD pDBMsg, DWORD dwDummy)
{
	//	MGET_MSG(pRecv, pDBMsg, NDBC_LoadItemNeedLog);

	tagNDBS_LoadItemNeedLog Send;
	Send.nItemNum		= 0;
	Send.dwErrorCode	= E_Success;

	if(!m_pDB->LoadItemNeedLog(Send.dwTypeID, Send.nItemNum))
	{
		Send.dwErrorCode = E_DBLoad_ItemNeedLog_Failed;
	}

	// ���¼����С
	Send.dwSize += (Send.nItemNum - MAX_ITEM_NEEDLOG_NUM) * sizeof(DWORD);

	// ��������Ϸ������
	SendMsg( &Send, Send.dwSize);

	return 0;
}


//-------------------------------------------------------------------------------------------------------
// ����Ʒ���в�������Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewItem);

	if(!m_pDB->InsertItem(p->item))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��װ�������Ʒ���в�����װ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertEquip(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewEquip);

	if(!m_pDB->InsertEquip(p->equip))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ����ɾ����Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DelItem);

	if(!m_pDB->DelItem(p->n64Serial))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��װ�������Ʒ��ɾ��װ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelEquip(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DelEquip);

	if(!m_pDB->DelEquip(p->n64Serial))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����װ��ר������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateEquipSpec(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateEquipAtt);

	if(!m_pDB->UpdateEquipSpec(p->equipSpecUpdate))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ���в�������Ʒ -- item_baibao
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertBaiBaoItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewBaiBaoItem);

	if(!m_pDB->InsertItem(p->item, "item_baibao"))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��װ�������Ʒ���в�����װ�� -- item_baibao,equip_baibao
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertBaiBaoEquip(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewBaiBaoEquip);

	if(!m_pDB->InsertEquip(p->equip, TRUE))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ƺű��в����³ƺż�¼
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertTitle(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_TitleInsert);

	if(!m_pDB->InsertTitle(p->dwRoleID, &p->titleSave))
	{
		return GT_INVALID;
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------------
// ɾ���ƺ�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RemoveTitle(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_TitleDelete);

	if(!m_pDB->RemoveTitle(p->dwRoleID, &p->titleSave))
	{
		return GT_INVALID;
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateVCard(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_VCardChg);

	if (!m_pDB->UpdateVCard(p->dwRoleID, &(p->VCardData)))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadVCard(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetOffLineVCard);

	MCREATE_MSG(pSend, 2000, NDBS_GetOffLineVCard);

	pSend->dwRoleID = pRecv->dwRoleID;
	pSend->dwQueryID = pRecv->dwQueryID;
	PVOID pDataLoad = pSend->byData;
	
	if (m_pDB->LoadVCard(pDataLoad, pRecv->dwRoleID))
	{
		pSend->dwErrCode = E_VCard_Success;
		DWORD dwVCardSize = (BYTE*)pDataLoad - pSend->byData;
		pSend->dwSize = sizeof(tagNDBS_GetOffLineVCard) - 1 + dwVCardSize;
	}
	else
	{
		pSend->dwErrCode = E_VCard_Failed;
		pSend->dwSize = sizeof(tagNDBS_GetOffLineVCard);
	}

	SendMsg( pSend, pSend->dwSize);		
	MDEL_MSG(pSend);

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��Ӽ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertSkill(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_AddSkill);

	if(!m_pDB->InsertSkill(p->dwRoleID, &p->Skill))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteSkill(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_RemoveSkill);

	if(!m_pDB->DelSkill(p->dwRoleID, p->dwSkillID))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateSkill(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateSkill);

	PVOID pData = &(p->Skill);

	if(!m_pDB->UpdateSkill(p->dwRoleID, &p->Skill))
	{
		return GT_INVALID;
	}

	return 0;
}


//-------------------------------------------------------------------------------------------------------
// ������Ʒ��ȴʱ��
//-------------------------------------------------------------------------------------------------------
//DWORD CLoong::ReplaceItemCDTime(DWORD pDBMsg, DWORD dwDummy)
//{
//	MGET_MSG(p, pDBMsg, NDBC_ReplaceItemCDTime);
//
//	if(!m_pDB->ReplaceItemCDTime(p->dwRoleID, p->byCDTime, p->nNum))
//	{
//		return GT_INVALID;
//	}
//
//	return 0;
//}

//-------------------------------------------------------------------------------------------------------
// ����һ��Ԫ�������˻�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertYBAccount(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertAccount);

	if(!m_pDB->InsertYBAccount(pRecv->dwRoleID))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻���Ԫ������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateAccountYB(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SynAccountYB);

	if(!m_pDB->UpdateAccountYB(pRecv->dwRoleID, pRecv->nYuanBao))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻��Ľ�Ǯ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateAccountSilver(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SynAccountSilver);

	if(!m_pDB->UpdateAccountSilver(pRecv->dwRoleID, &pRecv->Money))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻����۶������ύ״̬
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateSubmitSell(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SynSubmitSell);

	if(!m_pDB->UpdateSubmitSell(pRecv->dwRoleID, pRecv->bSellOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻��չ��������ύ״̬
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateSubmitBuy(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SynSubmitBuy);

	if (!m_pDB->UpdateSubmitBuy(pRecv->dwRoleID, pRecv->bBuyOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertOrder);

	if(!m_pDB->InsertOrder(pRecv->YBOrder.dwRoleID, &pRecv->YBOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ������ر�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::CompleteOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CompleteOrder);

	if(!m_pDB->CompleteOrder(GT_INVALID, &pRecv->CompleteOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RefreshOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RefreshOrder);

	if(!m_pDB->RefreshOrder(GT_INVALID, &pRecv->RefreshOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ɾ�����ݿ���ʱ�䳬����������ж���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteYBOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteYBOrder);
	if(!m_pDB->DeleteYBOrder(GT_INVALID, &pRecv->dwDeleteTime))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ȡ�����ݿ���ָ���˺ŵİٱ���Log��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetBaibaoLogInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadBaiBaoLog);

	ZeroMemory(m_pBuffer, X_ROLE_BUFF_SIZE);
	m_pLoadBaiBaoLog->dwID = m_pLoadBaiBaoLog->Crc32("NDBS_LoadBaiBaoLog");
	m_pLoadBaiBaoLog->dwAccountID = pRecv->dwAccountID;
	m_pLoadBaiBaoLog->dwErrorCode = E_Success;

	INT32 nTmpSize = 0;

	if (!m_pDB->LoadBaibaoLogInfo(pRecv->dwAccountID, m_pLoadBaiBaoLog->nLogNum, nTmpSize, m_pLoadBaiBaoLog->byData))
	{
		m_pLoadBaiBaoLog->dwErrorCode = E_DBLoad_BaiBaoLog_Failed;
	}

	// ���¼�����Ϣ��С
	m_pLoadBaiBaoLog->dwSize = sizeof(tagNDBS_LoadBaiBaoLog) + nTmpSize - sizeof(m_pLoadBaiBaoLog->byData);

	// ��������Ϸ������
	SendMsg( m_pLoadBaiBaoLog, m_pLoadBaiBaoLog->dwSize);

	return m_pLoadBaiBaoLog->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��ָ���˻��в���һ��Log��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertBaibaoLog(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_AddBaiBaoLog);

	return m_pDB->InsertBaibaoLogInfo(pRecv->dwAccountID, (LPVOID)&pRecv->LogInfo);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���а����Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadAllGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	INT nGPInfoNum = m_pDB->GetGPInfoCount();
	if(0 == nGPInfoNum)
	{
		tagNDBS_GetAllGPInfo Load;
		Load.dwErrorCode = E_Success;
		Load.nGPInfoNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagGPInfo *pGPInfo = (tagGPInfo*)(new BYTE[m_pDB->GetGPInfoSize()]);
	tagGPInfo *pCur = pGPInfo;

	if (!m_pDB->LoadAllGPInfo(pGPInfo, nGPInfoNum))
	{
		tagNDBS_GetAllGPInfo Load;
		Load.dwErrorCode = E_DBLoad_GroupPurchase_Faild;
		Load.nGPInfoNum = 0;
		SendMsg( &Load, Load.dwSize);		

		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_GROUPPURCHASE_CAPACITY_PER_MSG + sizeof(tagNDBS_GetAllGPInfo)];
	ZeroMemory(pBuff, MAX_GROUPPURCHASE_CAPACITY_PER_MSG + sizeof(tagNDBS_GetAllGPInfo));

	// ��ʼ����Ϣͷ
	tagNDBS_GetAllGPInfo LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_GetAllGPInfo *pLoad = (tagNDBS_GetAllGPInfo*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize - sizeof(tagGPInfo);
	pLoad->nGPInfoNum = 0;

	tagGPInfo *pTmpGPInfo = pCur;
	while (nGPInfoNum > 0)
	{
		pLoad->dwSize = LoadTemp.dwSize - sizeof(tagGPInfo);
		pLoad->nGPInfoNum = 0;

		INT32 nTmpSize = 0;

		while (nTmpSize < MAX_GROUPPURCHASE_CAPACITY_PER_MSG)
		{
			INT32 nCurSize = pTmpGPInfo->Size();

			// һ���Ź���Ϣ�����ܳ����������������������ѭ��
			ASSERT(nCurSize <= MAX_GROUPPURCHASE_CAPACITY_PER_MSG);

			if (nTmpSize + nCurSize > MAX_GROUPPURCHASE_CAPACITY_PER_MSG)
			{
				break;
			}
			else
			{
				nTmpSize += nCurSize;
				pTmpGPInfo = (tagGPInfo*)((BYTE*)pTmpGPInfo + nCurSize);

				pLoad->nGPInfoNum++;

				nGPInfoNum--;
				if (nGPInfoNum <= 0)
				{
					break;
				}
			}
		}

		pLoad->dwSize += nTmpSize;
		memcpy(pLoad->GPInfoData, pCur, nTmpSize);
		pCur = pTmpGPInfo;

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	SAFE_DEL_ARRAY(pBuff);
	SAFE_DEL_ARRAY(pGPInfo);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ׷��һ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::AddGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_AddGPInfo);

	return m_pDB->AddGPInfo(&(pRecv->GPInfo));
}

//-------------------------------------------------------------------------------------------------------
// ɾ��һ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RemoveGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RemoveGPInfo);

	return m_pDB->RemoveGPInfo(&(pRecv->GpInfoKey));
}

//-------------------------------------------------------------------------------------------------------
// ɾ��ָ���������е��Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RemoveGuildGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RemoveGuildGPInfo);

	return m_pDB->RemoveGuildGPInfo(pRecv->dwGuildID);
}

//-------------------------------------------------------------------------------------------------------
// �����Ź���Ӧ���б�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateGPInfo);

	return m_pDB->UpdateGPInfo(&(pRecv->GpInfoKey), pRecv->dwNewParticipator);
}

//-------------------------------------------------------------------------------------------------------
// �����Ź�ʣ��ʱ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateGPTime(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateGPTime);

	return m_pDB->UpdateGPTime();
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����VIP̯λ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetAllVIPStallInfo(DWORD pDBMsg, DWORD dwDummy)
{
	tagNDBS_GetAllVIPStallInfo send;

	// ��ʼ������VIP̯λ����
	for (int i=0; i<VIP_STALL_MAX_NUM; i++)
	{
		send.VipStallInfo[i].byStallIndex	= GT_INVALID;
		send.VipStallInfo[i].dwOwnerID		= GT_INVALID;
		send.VipStallInfo[i].nRemainTime	= GT_INVALID;
	}

	if (!m_pDB->GetAllVIPStallInfo(send.VipStallInfo))
	{
		return GT_INVALID;
	}

	SendMsg( &send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����һ��VIP̯λ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ApplyVIPStall(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ApplyVIPStall);

	return m_pDB->ApplyVIPStall(&(pRecv->VipStallInfo));
}

//-------------------------------------------------------------------------------------------------------
// ����VIP̯λʣ��ʱ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateVIPStall(DWORD pDBMsg, DWORD dwDummy)
{
	return m_pDB->UpdateVIPStallTime();
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�̶������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadActivityData(DWORD pDBMsg, DWORD dwDummy)
{
	INT nActivityNum = m_pDB->GetActivityCount();

	if(0==nActivityNum)
	{
		tagNDBS_LoadActivityData Load;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagActivityData *pActivityData = new tagActivityData[nActivityNum];
	tagActivityData *pCur = pActivityData;

	INT nNum;
	if(!m_pDB->LoadActivityData(pActivityData, nNum))
	{
		delete []pActivityData;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_ACTIVITY_NUM_PER_MSG * sizeof(tagActivityData) + sizeof(tagNDBS_LoadActivityData)];

	// ��ʼ����Ϣͷ
	tagNDBS_LoadActivityData LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_LoadActivityData *pLoad = (tagNDBS_LoadActivityData*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	while(nActivityNum > 0)
	{
		if(nActivityNum > MAX_ACTIVITY_NUM_PER_MSG)
		{
			memcpy(pLoad->Activity, pCur, MAX_ACTIVITY_NUM_PER_MSG * sizeof(tagActivityData));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ACTIVITY_NUM_PER_MSG - 1) 
				* sizeof(tagActivityData) + sizeof(tagNDBS_LoadActivityData);
			pLoad->nNum = MAX_ACTIVITY_NUM_PER_MSG;

			nActivityNum -= MAX_ACTIVITY_NUM_PER_MSG;
			pCur = &pCur[MAX_ACTIVITY_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->Activity, pCur, nActivityNum * sizeof(tagActivityData));

			// ����������Ϣ��С
			pLoad->dwSize = (nActivityNum - 1) 
				* sizeof(tagActivityData) + sizeof(tagNDBS_LoadActivityData);
			pLoad->nNum = nActivityNum;

			nActivityNum = 0;
			pCur = NULL;
		}

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pActivityData;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �洢�̶���ű�����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::SaveActivityData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveActivityData);

	if(!m_pDB->SaveActivityData(pRecv->Activity.dwID, &pRecv->Activity))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ���¿����������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateTreasureSum(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateTreasureSum);

	if(!m_pDB->UpdateTreasureSum(pRecv->dwRoleID, pRecv->nSum))
	{
		return GT_INVALID;
	}

	return E_Success;
}


//-------------------------------------------------------------------------------------------------------
// ����ʵ�����а�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadStrengthRankings(DWORD pDBMsg, DWORD dwDummy)
{
	tagStrengthRoleNode* pRankingsNode = new tagStrengthRoleNode[500];
	tagStrengthRoleNode* pCur = pRankingsNode;
	INT nNum;

	if (!m_pDB->LoadStrengthRankings(pRankingsNode, nNum))
	{
		delete []pRankingsNode;
		return GT_INVALID;
	}

	// ��֯����Ϣ��������Ϸ����
	tagNDBS_StrengthRankings send;
	send.nSum = nNum;
	for (INT i=0; i<nNum && i<X_STRENGTH_RANKINGS_LEN; ++i)
	{
		send.Rankings[i].nNum = pRankingsNode[i].nNum;
		send.Rankings[i].nPreNum = pRankingsNode[i].nPreNum;
		_tcscpy(send.Rankings[i].szRoleName, pRankingsNode[i].szRoleName);
		//send.Rankings[i].dwRoleID = pRankings[i].dwRoleID;
		send.Rankings[i].dwStrength = pRankingsNode[i].dwStrength;
	}
	
	SendMsg(&send, send.dwSize);

	delete []pRankingsNode;

	return E_Success;
	
}

//-------------------------------------------------------------------------------------------------------
// ����ʵ�����а�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateStrengthRankings(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateRankings);

	DWORD dwErrorCode = E_Success;
	if(!m_pDB->UpdateStrengthRankings())
	{
		dwErrorCode = E_DBUpdate_StrengthRankingsFailed;
	}

	tagNDBS_UpdateRankings ret;
	ret.dwErrorCode = dwErrorCode;

	SendMsg(&ret, ret.dwSize);
	return E_Success;
}
//-------------------------------------------------------------------------------------------------------
// ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RoleGetMarriage(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RoleGetMarriage);

	//����role1��db
	if(!m_pDB->RoleGetMarriage(pRecv->dwRoleID1, pRecv->dwRoleID2))
	{
		return GT_INVALID;
	}
	//����role2��db
	if(!m_pDB->RoleGetMarriage(pRecv->dwRoleID2, pRecv->dwRoleID1))
	{
		return GT_INVALID;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------
// ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RoleBreakMarriage(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RoleBreakMarriage);

	//����role1��db
	if(!m_pDB->RoleBreakMarriage(pRecv->dwRoleID1, pRecv->dwRoleID2))
	{
		return GT_INVALID;
	}
	//����role2��db
	if(!m_pDB->RoleBreakMarriage(pRecv->dwRoleID2, pRecv->dwRoleID1))
	{
		return GT_INVALID;
	}
	return 0;
}

DWORD CLoong::LoadExtLinks(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetExternalLinks);

	if( P_VALID(pRecv) )
	{
		tagExternalLink * p = NULL;
		int num = 0;
		if( m_pDB->LoadExtLinks( p , num ) )
		{
			int size = sizeof(tagNDBS_GetExternalLinks) + (num - 1) * sizeof(tagExternalLink);
			MCREATE_MSG(pSend,size,NDBS_GetExternalLinks);
			pSend-> dwErrorCode = 0;
			pSend-> dwElementNum = num;
			memcpy(pSend->links , p, num * sizeof(tagExternalLink));
			SendMsg(pSend,pSend->dwSize);
			//g_MemPoolSafe.Free  (pSend);
			g_MemPoolSafe.Free  (p);
			return 0;
		}
	}
	tagNDBS_GetExternalLinks msg;
	msg.dwErrorCode = GT_INVALID;
	msg.dwElementNum = 0;
	memset(msg.links,0,sizeof(tagExternalLink));
	SendMsg(&msg,msg.dwSize);
	return GT_INVALID;
}



