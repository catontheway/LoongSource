//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: TreasureChest_mgr.cpp
// author: twan
// actor:
// data: 2009-07-29
// last:
// brief: ���������
//-------------------------------------------------------------------------------------------------------

#include <time.h>
#include "StdAfx.h"
#include "att_res.h"
#include "TreasureChest_mgr.h"
#include "../vEngine/debug/debug.h"
#include "../vEngine/globe/interface_helper.h"
#include "../vEngine/variable/var_container.h"
#include "../WorldDefine/msg_digbao.h"
#include "../WorldDefine/ItemDefine.h"
#include "world_session.h"

//-------------------------------------------------------------------------------
// ��ʼ��  ����xml�ļ�
//-------------------------------------------------------------------------------
BOOL TreasureChestMgr::Init()
{
	// ��ȡTreasureItem.xml
	list<tstring> listProto;
	tstring szTreasureItemProto = _T("server_data//TreasureItem.xml");
	TObjRef<VarContainer> varTreasureItemProto = CreateObj("TreasureItemProto", "VarContainer");
	if (!varTreasureItemProto->Load("VFS_System", szTreasureItemProto.c_str(), "TableID", &listProto))
	{
		IMSG(_T("Load TreasureItem proto file failed"));
		KillObj("TreasureItemProto");
		return FALSE;
	}

	INT nChestNum = varTreasureItemProto->GetInt(_T("TableID"), listProto.back().c_str()) / 100;
	map< int, vector<tagChestItem> >	mapChest;

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; )
	{
		
		for (INT n = 0; begin != end && n != MAX_CHEST_NUM; ++begin, ++n)
		{
			tagChestItem item;
			item.dwTypeID = varTreasureItemProto->GetDword(_T("TypeID"), begin->c_str());
			item.nTableID = varTreasureItemProto->GetInt(_T("TableID"), begin->c_str());
			item.nNum = varTreasureItemProto->GetInt(_T("Num"), begin->c_str());
			item.fBeginRate = varTreasureItemProto->GetFloat(_T("BeginRate"), begin->c_str());
			item.fRoleRate = varTreasureItemProto->GetFloat(_T("RoleRate"), begin->c_str());
			item.fServerRate = varTreasureItemProto->GetFloat(_T("ServerRate"), begin->c_str());
			item.fNormalRate = varTreasureItemProto->GetFloat(_T("NormalRate"), begin->c_str());

			map< int, vector<tagChestItem> >::iterator it = mapChest.find(item.nTableID/100 - 1);
			if (it == mapChest.end())
			{
				vector<tagChestItem> vectmp;
				vectmp.push_back(item);
				mapChest.insert(make_pair(item.nTableID/100 - 1, vectmp));
			}
			else
			{
				it->second.push_back(item);
			}
		}
	}

	for (INT i = 0; i != nChestNum; ++i)
	{
		m_vecItems.push_back(mapChest[i]);
	}

	KillObj("TreasureItemProto");
	return TRUE;
}

//-------------------------------------------------------------------------------
// ���ر�������Ʒ
//-------------------------------------------------------------------------------
vector<tagChestItem>& TreasureChestMgr::GetChest(INT nIndex)
{
	--nIndex;								//�������1
	ASSERT(nIndex < m_vecItems.size());
	return m_vecItems[nIndex];
}

//-------------------------------------------------------------------------------
// ������������Ʒ
//-------------------------------------------------------------------------------
tagChestItem* TreasureChestMgr::GetRandomItem(DWORD dwChestID, ERateType eRate, FLOAT fRand)
{
	tagItemProto* pProto = g_attRes.GetItemProto(dwChestID);
	if (!P_VALID(pProto))
	{
		return NULL;
	}

	INT32 nIndex = pProto->nSpecFuncVal1;
	INT n = 0;

	switch (eRate)
	{
	case ERTT_BeginRate:
		for (; n != MAX_CHEST_NUM && fRand >= 0.0; ++n)
		{
			fRand -= GetChest(nIndex)[n].fBeginRate;
		}
		break;

	case ERTT_RoleRate:
		for (; n != MAX_CHEST_NUM && fRand >= 0.0; ++n)
		{
			fRand -= GetChest(nIndex)[n].fRoleRate;
		}
		break;

	case ERTT_ServerRate:
		for (; n != MAX_CHEST_NUM && fRand >= 0.0; ++n)
		{
			fRand -= GetChest(nIndex)[n].fServerRate;
		}
		break;

	case ERTT_NormalRate:
		for (; n != MAX_CHEST_NUM && fRand >= 0.0; ++n)
		{
			fRand -= GetChest(nIndex)[n].fNormalRate;
		}
		break;

	default:
		IMSG(_T("Should not get here"));
		break;
	}

	return &m_vecItems[nIndex-1][n-1];		// �������������һ
}

//-------------------------------------------------------------------------------
// ��ͻ��˷�����Ϣ
//-------------------------------------------------------------------------------
BOOL TreasureChestMgr::SendMsg2Client(Role *pRole, DWORD dwChestID, const std::string strMsgName, 
									  BOOL bOpened, BOOL bDestroy, DWORD dwItemID, INT nItemNum, DWORD dwErrorCode)
{
	if (strMsgName == "OpenChest")
	{
		tagNS_TreasureChest send;
		send.dwChestTypeID = dwChestID;
		if (bOpened)
		{
			tagItemProto* pItemProto = g_attRes.GetItemProto(dwChestID);
			if (!P_VALID(pItemProto))
			{
				return 0;
			}

			//�õ�������Ʒ����id
			for (INT i = 0; i != MAX_CHEST_NUM; ++i)
			{
				send.dwTypeID[i] = g_TreasureChestMgr.GetChest(pItemProto->nSpecFuncVal1)[i].dwTypeID;
				send.nNum[i] = g_TreasureChestMgr.GetChest(pItemProto->nSpecFuncVal1)[i].nNum;
			}
		}

		send.dwErrorCode = dwErrorCode;
		// ������Ϣ���ͻ���
		pRole->GetSession()->SendMessage(&send, send.dwSize);
		return TRUE;
	}
	else if (strMsgName == "StopChest")
	{
		tagNS_StopTreasureChest send;
		send.dwTypeID = dwItemID;
		send.nNum = nItemNum;
		send.dwErrorCode = dwErrorCode;

		if (dwErrorCode == E_Success)
		{
			// ��¼��������Ʒ
			tagChestItem item;
			item.dwTypeID = dwItemID;
			item.nNum = nItemNum;
			item.nTableID = 0;
			item.fBeginRate = 0;
			item.fNormalRate = 0;
			item.fRoleRate = 0;
			item.fServerRate = 0;

			pRole->SetChestItem(item);
		}

		// ������Ϣ���ͻ���
		pRole->GetSession()->SendMessage(&send, send.dwSize);
		return TRUE;
	}
	else if (strMsgName == "AgainChest")
	{
		tagNS_AgainTreasureChest send;
		send.byDestroy = bDestroy ? 1 : 0;
		send.dwErrorCode = dwErrorCode;
		// ��ս�ɫ��󿪳���Ʒ�ļ�¼
		tagChestItem item;
		item.dwTypeID = 0;
		item.nNum = 0;
		item.nTableID = 0;
		item.fBeginRate = 0;
		item.fNormalRate = 0;
		item.fRoleRate = 0;
		item.fServerRate = 0;
		pRole->SetChestItem(item);
		// ������Ϣ���ͻ���
		pRole->GetSession()->SendMessage(&send, send.dwSize);
		return TRUE;
	}

	else if (strMsgName == "GetItem")					// "GetItem"
	{
		tagNS_ChestGetItem send;
		send.dwErrorCode = dwErrorCode;
		// ��ս�ɫ��󿪳���Ʒ�ļ�¼
		tagChestItem item;
		item.dwTypeID = 0;
		item.nNum = 0;
		item.nTableID = 0;
		item.fBeginRate = 0;
		item.fNormalRate = 0;
		item.fRoleRate = 0;
		item.fServerRate = 0;
		pRole->SetChestItem(item);

		// ��ս�ɫ�����Կ��ID��¼
		pRole->SetChestSerial(0);
		pRole->SetKeySerial(0);
		// ������Ϣ���ͻ���
		pRole->GetSession()->SendMessage(&send, send.dwSize);
		return TRUE;
	}

	else
	{
		return FALSE;
	}
	
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID TreasureChestMgr::Destroy()
{
	m_vecItems.clear();
}

TreasureChestMgr g_TreasureChestMgr;