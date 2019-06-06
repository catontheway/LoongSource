//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: TreasureChest_mgr.h
// author: twan
// actor:
// data: 2009-07-29
// last:
// brief: ���������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/TreasureChest_define.h"
#include "role.h"

class TreasureChestMgr
{
private:
	vector< vector<tagChestItem> >	m_vecItems;						// ������б������Ʒ��һ��������16����Ʒ
	
public:
	BOOL						Init();								// ��ʼ����������Ʒ
	vector<tagChestItem>&		GetChest(INT nIndex);				// ���һ�ֱ��������
	BOOL						CanOpenChest(DWORD dwChestID, DWORD dwKeyID);	//�ж��ܷ�������
	VOID						Destroy();							// ����
	tagChestItem*				GetRandomItem(DWORD dwChestID, ERateType eRate, FLOAT fRand); //������������Ʒ
	ERateType					GetRate(Role* pRole);				// �õ���ǰ�������
	// ��ͻ��˷�����Ϣ
	BOOL						SendMsg2Client(Role* pRole, DWORD dwChestID, const std::string strMsgName, 
											BOOL bOpened, BOOL bDestroy, DWORD dwItemID, INT nItemNum, DWORD dwErrorCode);		
	
};

extern TreasureChestMgr g_TreasureChestMgr;