//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: world_event_mgr.cpp
// author: wjiang
// actor:
// data: 2009-04-2
// last:
// brief: ��Ϸ�����¼�������
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "world_event_mgr.h"
#include "script_mgr.h"
#include "activity_mgr.h"
#include "role_mgr.h"
#include "world.h"
#include "role.h"

BOOL CWorldEventMgr::Init()
{
	// ��ʼ���ű�
	m_pScript = g_ScriptMgr.GetWorldScript();
	if(!P_VALID(m_pScript))
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����Update
//-----------------------------------------------------------------------------
VOID CWorldEventMgr::OnClock(BYTE byHour)
{
	OnAdventure();
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
VOID CWorldEventMgr::OnAdventure()
{
	// �Ƿ��й̶���ڽ�����
	if(TRUE == g_activityMgr.HaveActivitStart())
		return;

	// ���ѡȡ3��������һ������
	Role	*pRole = (Role*)GT_INVALID;
	std::vector<Role*>	vecRole;
	INT		nNum	= 0;
	FLOAT	fProp	= 0.0f;
	BOOL	bResult	= FALSE;

	// ��һ�γ�ȡ500�Σ������ɫ��Ե����Ӱ��
	for(INT i = 0; i < 500; ++i)
	{
		// �Ѿ��ҵ��������
		if(nNum == 3)
			break;

		pRole = g_roleMgr.GetRandRolePtr();
		if(!P_VALID(pRole))
			continue;

		// С��15�����²����ȡ
		if(pRole->GetLevel() < 15)
			continue;

		// ��������Ҳ����ȡ
		

		// �����������ĸ���
		fProp = 1.0f / 20.0f * (1.0f + (FLOAT)pRole->GetAttValue(ERA_Fortune) / 10.0f);
		bResult = IUTIL->Rand() % 100 <= (INT)fProp;
		if(bResult)
		{
			vecRole.push_back(pRole);
			++nNum;
		}
	}

	// ��ȡ�����С��3��������еڶ��ֳ�ȡ
	if(nNum < 3)
	{
		for(INT n = 0; n < 500; ++n)
		{
			// �Ѿ��ҵ��������
			if(nNum == 3)
				break;

			pRole = g_roleMgr.GetRandRolePtr();
			if(!P_VALID(pRole))
				continue;

			// С��15�����²����ȡ
			if(pRole->GetLevel() < 15)
				continue;

			// ��������Ҳ����ȡ


			vecRole.push_back(pRole);
			++nNum;
		}
	}

	// �Գ�ȡ����Ҳ�������
	std::vector<Role*>::iterator it = vecRole.begin();
	while (it != vecRole.end())
	{
		if (P_VALID(m_pScript))
		{
			m_pScript->OnAdventure(*it);
		}
		++it;
	}
}