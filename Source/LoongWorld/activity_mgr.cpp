//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: activity.cpp
// author: wjiang
// actor:
// data: 2009-03-27
// last:
// brief: �������
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "activity_mgr.h"
#include "role_mgr.h"
#include "script_mgr.h"
#include "world.h"
#include "db_session.h"
#include "TimeChecker.h"

#include "../WorldDefine/msg_activity.h"

//----------------------------------------------------------------------------------------------------
// �̶��������
//----------------------------------------------------------------------------------------------------
CActivity::CActivity():ScriptData()
{

}

CActivity::~CActivity()
{

}

VOID CActivity::Init(const tagActInfo *pActInfo)
{
	m_pActInfo = pActInfo;
	m_bIsStart = FALSE;
	m_dwTick   = 0;
	m_setOnTimeTick.clear();

	// ��ʼ���ű�
	m_pScript = g_ScriptMgr.GetActScript(pActInfo->dwID);

	// ������ʼǰ�Ĺ㲥ʱ��
	if(pActInfo->ActTime.byStartInterval != 0)
	{
		tagDWORDTime	broad_time = pActInfo->ActTime.startTime;
		broad_time.hour = pActInfo->ActTime.byStartBroadH;
		broad_time.min  = pActInfo->ActTime.byStartBroadM;
		
		// ���������ٷ���
		DWORD dwTimeLeft = CalcTimeDiff( pActInfo->ActTime.startTime, broad_time ) / 60;
		INT	  nBroadNum = dwTimeLeft / pActInfo->ActTime.byStartInterval;

		for(INT i = 0; i < nBroadNum; ++i)
		{
			//���˴�day+1Ϊ��ֹIncreaseTime�����
			++broad_time.day;
			broad_time = IncreaseTime(broad_time, i * pActInfo->ActTime.byStartInterval * 60);
			--broad_time.day;
			m_vecStartBroadTime.push_back(broad_time);
		}
	}

	// ��������ǰ�Ĺ㲥ʱ��
	if(pActInfo->ActTime.byEndInterval != 0)
	{
		tagDWORDTime	broad_time = pActInfo->ActTime.endTime;
		broad_time.hour = pActInfo->ActTime.byEndBroadH;
		broad_time.min  = pActInfo->ActTime.byEndBroadM;

		// ���������ٷ���
		DWORD dwTimeLeft = CalcTimeDiff( pActInfo->ActTime.endTime, broad_time ) / 60;
		INT	  nBroadNum = dwTimeLeft / pActInfo->ActTime.byEndInterval;

		for(INT i = 0; i < nBroadNum; ++i)
		{
			//���˴�day+1Ϊ��ֹIncreaseTime�����
			++broad_time.day;
			broad_time = IncreaseTime(broad_time, i * pActInfo->ActTime.byEndInterval * 60);
			--broad_time.day;
			m_vecEndBroadTime.push_back(broad_time);
		}
	}

	if(P_VALID(m_pScript))
	{
		m_pScript->OnInit(pActInfo->dwID);
	}
}

//----------------------------------------------------------------------------------------------------
// ��ʼ���̶���ű�����
//----------------------------------------------------------------------------------------------------
VOID CActivity::InitScriptData(tagActivityData* pActivityData)
{
	IFASTCODE->MemCpy(m_ScriptData.dwData, pActivityData->Data.dwData, sizeof(DWORD)*ESD_Activity);
}

//----------------------------------------------------------------------------------------------------
// ��Ƿ���Կ�ʼ
//----------------------------------------------------------------------------------------------------
BOOL CActivity::CanStart(tagDWORDTime cur_time)
{
	// ����Ѿ������������ٿ���
	if( IsStart())
		return FALSE;

	switch (m_pActInfo->eActMode)
	{
	case EAM_Day:				// ÿ�ջ
		{

		}
		break;
	case EAM_Week:				// ÿ�ܻ
		{
			// ���ȸ������ڼ��������ж�
			DWORD dwWeekDay = WhichWeekday(cur_time);
			if( dwWeekDay != (DWORD)m_pActInfo->ActTime.eWeekday )
				return FALSE;
		}
		break;
	case EAM_Month:				// ÿ�»
		{
			if(cur_time.day != m_pActInfo->ActTime.startTime.day)
				return FALSE;
		}
		break;
	case EAM_Festival:			// ���ջ
		{
			INT nDiffMonth = m_pActInfo->ActTime.endTime.month - m_pActInfo->ActTime.startTime.month;
			INT nDiffDay = m_pActInfo->ActTime.endTime.day - m_pActInfo->ActTime.startTime.day;
			if((cur_time.month > m_pActInfo->ActTime.startTime.month + nDiffMonth) || (cur_time.month < m_pActInfo->ActTime.startTime.month))
				return FALSE;

			if((cur_time.day > m_pActInfo->ActTime.startTime.day + nDiffDay) || (cur_time.day < m_pActInfo->ActTime.startTime.day))
				return FALSE;
		}
		break;
	}

	// �������һ�죬���жϵ�ǰ��ʱ���ǲ��Ƿ�������
	if( cur_time.hour < m_pActInfo->ActTime.startTime.hour	||
		(cur_time.hour == m_pActInfo->ActTime.startTime.hour&&
		cur_time.min < m_pActInfo->ActTime.startTime.min)	||
		cur_time.hour > m_pActInfo->ActTime.endTime.hour	||
		(cur_time.hour == m_pActInfo->ActTime.endTime.hour  &&
		cur_time.min >= m_pActInfo->ActTime.endTime.min ))
		return FALSE;

	// ���ϲ���ȫ��ͨ��
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// ��Ƿ�ʼ������
//----------------------------------------------------------------------------------------------------
BOOL CActivity::IsStartBroad(tagDWORDTime cur_time)
{
	// ����Ѿ������������ٷ�����ʼ����
	if( IsStart())
		return FALSE;

	switch (m_pActInfo->eActMode)
	{
	case EAM_Day:				// ÿ�ջ
		{

		}
		break;
	case EAM_Week:				// ÿ�ܻ
		{
			// ���ȸ������ڼ��������ж�
			DWORD dwWeekDay = WhichWeekday(cur_time);
			if( dwWeekDay != (DWORD)m_pActInfo->ActTime.eWeekday )
				return FALSE;
		}
		break;
	case EAM_Month:				// ÿ�»
		{
			if(cur_time.day != m_pActInfo->ActTime.startTime.day)
				return FALSE;
		}
		break;
	case EAM_Festival:			// ���ջ
		{
			if(cur_time.month != m_pActInfo->ActTime.startTime.month)
				return FALSE;

			if(cur_time.day != m_pActInfo->ActTime.startTime.day)
				return FALSE;
		}
		break;
	}

	std::vector<tagDWORDTime>::iterator it;
	for(it = m_vecStartBroadTime.begin(); it != m_vecStartBroadTime.end(); ++it)
	{
		if(cur_time.hour == it->hour && cur_time.min == it->min)
			return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------------------------------
// ��Ƿ����
//----------------------------------------------------------------------------------------------------
BOOL CActivity::IsEnd(tagDWORDTime cur_time)
{
	// �����û�п������������������
	if( ! IsStart() )
		return FALSE;

	switch (m_pActInfo->eActMode)
	{
	case EAM_Day:				// ÿ�ջ
		{

		}
		break;
	case EAM_Week:				// ÿ�ܻ
		{
			// ���ȸ������ڼ��������ж�
			DWORD dwWeekDay = WhichWeekday(cur_time);
			if( dwWeekDay != (DWORD)m_pActInfo->ActTime.eWeekday )
				return TRUE;
		}
		break;
	case EAM_Month:				// ÿ�»
		{
			if(cur_time.day != m_pActInfo->ActTime.endTime.day)
				return TRUE;
		}
		break;
	case EAM_Festival:			// ���ջ
		{
			INT nDiffMonth = m_pActInfo->ActTime.endTime.month - m_pActInfo->ActTime.startTime.month;
			INT nDiffDay = m_pActInfo->ActTime.endTime.day - m_pActInfo->ActTime.startTime.day;
			if( (cur_time.month > m_pActInfo->ActTime.startTime.month + nDiffMonth) || (cur_time.month < m_pActInfo->ActTime.startTime.month) )
				return TRUE;

			if( (cur_time.day > m_pActInfo->ActTime.startTime.day + nDiffDay) || (cur_time.day < m_pActInfo->ActTime.startTime.day) )
				return TRUE;
		}
		break;
	}

	// �����ͬһ�죬���⵱ǰʱ���ǲ��ǹ��˽�ֹʱ��
	if( cur_time.hour > m_pActInfo->ActTime.endTime.hour ||
		(cur_time.hour == m_pActInfo->ActTime.endTime.hour &&
		cur_time.min >= m_pActInfo->ActTime.endTime.min ))
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------------------------
// ���������
//----------------------------------------------------------------------------------------------------
BOOL CActivity::IsEndBroad(tagDWORDTime cur_time)
{
	// �����û�п������������������
	if( ! IsStart() )
		return FALSE;

	switch (m_pActInfo->eActMode)
	{
	case EAM_Day:				// ÿ�ջ
		{

		}
		break;
	case EAM_Week:				// ÿ�ܻ
		{
			// ���ȸ������ڼ��������ж�
			DWORD dwWeekDay = WhichWeekday(cur_time);
			if( dwWeekDay != (DWORD)m_pActInfo->ActTime.eWeekday )
				return FALSE;
		}
		break;
	case EAM_Month:				// ÿ�»
		{
			if(cur_time.day != m_pActInfo->ActTime.endTime.day)
				return FALSE;
		}
		break;
	case EAM_Festival:			// ���ջ
		{
			if(cur_time.month != m_pActInfo->ActTime.endTime.month)
				return FALSE;

			if(cur_time.day != m_pActInfo->ActTime.endTime.day)
				return FALSE;
		}
		break;
	}

	std::vector<tagDWORDTime>::iterator it;
	for(it = m_vecEndBroadTime.begin(); it != m_vecEndBroadTime.end(); ++it)
	{
		if(cur_time.hour == it->hour && cur_time.min == it->min)
			return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------------------------------
// �㲥���ǰ״̬�����������������
// 0-������ʼ 1-��ʼ 2-�������� 3-���� 
//----------------------------------------------------------------------------------------------------
VOID CActivity::BroadActivityState(EActBroadMode eState)
{
/*	tagNS_ActivityTaskState	send;
	send.nState = nState;
	_tcsncpy(send.szName, m_pActInfo->szName, X_SHORT_NAME);
	if( nState == 0 || nState == 1 )	// ������ʼ��ʼ
	{
		send.byHour = m_pActInfo->ActTime.startTime.hour;
		send.byMinute = m_pActInfo->ActTime.startTime.min;
	}
	else								// �������������
	{
		send.byHour = m_pActInfo->ActTime.endTime.hour;
		send.byMinute = m_pActInfo->ActTime.endTime.min;
	}

	g_roleMgr.SendWorldMsg(&send, send.dwSize);*/

	if( P_VALID(m_pScript) ) 
	{
		m_pScript->BroadActivityState(eState);
	}
}


//----------------------------------------------------------------------------------------------------
// ���ʼ
//----------------------------------------------------------------------------------------------------
VOID CActivity::Start()
{ 
	m_bIsStart = TRUE;

	if (P_VALID(m_pScript))
	{
		m_pScript->OnActStart(m_pActInfo->dwID);
	}
}

//----------------------------------------------------------------------------------------------------
// �����
//----------------------------------------------------------------------------------------------------
VOID CActivity::End() 
{
	m_bIsStart = FALSE; 
	m_dwTick   = 0;

	if (P_VALID(m_pScript))
	{
		m_pScript->OnActEnd(m_pActInfo->dwID);
	}

	// �洢��ű�����
	tagNDBC_SaveActivityData	sendDB;
	sendDB.Activity.dwID = m_pActInfo->dwID;
	IFASTCODE->MemCpy(sendDB.Activity.Data.dwData, m_ScriptData.dwData, sizeof(DWORD)*ESD_Activity);
	g_dbSession.Send(&sendDB, sendDB.dwSize);
}

//----------------------------------------------------------------------------------------------------
// ������ʱ�䴥���¼��Ĵ���ʱ���б�
//----------------------------------------------------------------------------------------------------
VOID CActivity::AddEventTime(DWORD dwTime)
{
	m_setOnTimeTick.insert(dwTime * TICK_PER_SECOND);
}

//----------------------------------------------------------------------------------------------------
// minute update
//----------------------------------------------------------------------------------------------------
VOID CActivity::OnMinute(tagDWORDTime cur_time)
{
	if( !IsStart() )
	{
		// ����û�п�����������Ƿ��ܹ�����
		if( CanStart(cur_time) )
		{
			Start();
			BroadActivityState(EABM_Start);		// �Ѿ���ʼ

			return;
		}
		else if(IsStartBroad(cur_time))			// ��ʼǰ�Ĺ㲥
		{
			BroadActivityState(EABM_AboutStart);
			return;
		}
	}
	else
	{
		if(P_VALID(m_pScript))
		{
			m_pScript->OnTimerMin(m_pActInfo->dwID);
		}

		if( IsEnd(cur_time) )
		{
			End();
			BroadActivityState(EABM_End);		// �Ѿ�����
			return;
		}
		else if(IsEndBroad(cur_time))			// ����ǰ�Ĺ㲥
		{
			BroadActivityState(EABM_AboutEnd);
			return;
		}
	}
}

//----------------------------------------------------------------------------------------------------
// update
//----------------------------------------------------------------------------------------------------
VOID CActivity::Update()
{
	if(!IsStart())
		return;

	// �Ƿ��л��ʱ�䴥���¼�
	std::set<DWORD>::iterator it = m_setOnTimeTick.begin();
	while (it != m_setOnTimeTick.end())
	{
		if(m_dwTick == *it)
		{
			if (P_VALID(m_pScript))
			{
				m_pScript->OnTimer(m_pActInfo->dwID, (m_dwTick * TICK_TIME) / 1000);
			}
		}
		++it;
	}

	++m_dwTick;
}

//----------------------------------------------------------------------------------------------------
// �洢����ݵ����ݿ�
//----------------------------------------------------------------------------------------------------
VOID CActivity::SaveActivityData2DB()
{
	// �洢��ű�����
	tagNDBC_SaveActivityData	sendDB;
	sendDB.Activity.dwID = m_pActInfo->dwID;
	IFASTCODE->MemCpy(sendDB.Activity.Data.dwData, m_ScriptData.dwData, sizeof(DWORD)*ESD_Activity);
	g_dbSession.Send(&sendDB, sendDB.dwSize);
}



//----------------------------------------------------------------------------------------------------
// �̶��������
//----------------------------------------------------------------------------------------------------
CActivityMgr::~CActivityMgr()
{
	Destroy();
}

VOID CActivityMgr::Destroy()
{
	tagActInfo	*pActInfo = (tagActInfo*)GT_INVALID;
	m_mapActInfo.ResetIterator();
	while (m_mapActInfo.PeekNext(pActInfo))
	{
		SAFE_DEL(pActInfo);
	}
	m_mapActInfo.Clear();

	CActivity *pActivity = (CActivity*)GT_INVALID;
	m_mapActivity.ResetIterator();
	while (m_mapActivity.PeekNext(pActivity))
	{
		SAFE_DEL(pActivity);
	}
	m_mapActivity.Clear();
}

//----------------------------------------------------------------------------------------------------
// ���ع̶����̬����
//----------------------------------------------------------------------------------------------------
BOOL CActivityMgr::LoadActivityData()
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// �����ļ�
	if(!m_pVar->Load(NULL, _T("data/system/attdata/activity_proto.xml"), "id", &ListField))
	{
		return FALSE;
	}

	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		tagActInfo* pActInfo = new tagActInfo;
		if(!P_VALID(pActInfo))
			return FALSE;

		// ��ʼ��
		pActInfo->dwID = m_pVar->GetDword(_T("id"), it->c_str());
		_tcscpy( pActInfo->szName, m_pVar->GetString(_T("name"), it->c_str(), _T("")) );
		pActInfo->eActMode = (EActMode)m_pVar->GetDword(_T("activitymode"), it->c_str(), 0);
		pActInfo->ActTime.eWeekday = (EWeekday)m_pVar->GetDword(_T("weekday"), it->c_str(), 0);
		pActInfo->ActTime.startTime.year = (BYTE)m_pVar->GetDword(_T("startyear"), it->c_str(), 0);
		pActInfo->ActTime.startTime.month = (BYTE)m_pVar->GetDword(_T("startmonth"), it->c_str(), 0);
		pActInfo->ActTime.startTime.day = (BYTE)m_pVar->GetDword(_T("startday"), it->c_str(), 0);
		pActInfo->ActTime.startTime.hour= (BYTE)m_pVar->GetDword(_T("starthour"), it->c_str(), 0);
		pActInfo->ActTime.startTime.min = (BYTE)m_pVar->GetDword(_T("startminute"), it->c_str(), 0);
		pActInfo->ActTime.endTime.year = (BYTE)m_pVar->GetDword(_T("endyear"), it->c_str(), 0);
		pActInfo->ActTime.endTime.month = (BYTE)m_pVar->GetDword(_T("endmonth"), it->c_str(), 0);
		pActInfo->ActTime.endTime.day = (BYTE)m_pVar->GetDword(_T("endday"), it->c_str(), 0);
		pActInfo->ActTime.endTime.hour = (BYTE)m_pVar->GetDword(_T("endhour"), it->c_str(), 0);
		pActInfo->ActTime.endTime.min = (BYTE)m_pVar->GetDword(_T("endminute"), it->c_str(), 0);
		pActInfo->ActTime.byStartBroadH = (BYTE)m_pVar->GetDword(_T("startbroadhour"), it->c_str(), 0);
		pActInfo->ActTime.byStartBroadM = (BYTE)m_pVar->GetDword(_T("startbroadminute"), it->c_str(), 0);
		pActInfo->ActTime.byStartInterval = (BYTE)m_pVar->GetDword(_T("startinterval"), it->c_str(), 0);
		pActInfo->ActTime.byEndBroadH = (BYTE)m_pVar->GetDword(_T("endbroadhour"), it->c_str(), 0);
		pActInfo->ActTime.byEndBroadM = (BYTE)m_pVar->GetDword(_T("endbroadminute"), it->c_str(), 0);
		pActInfo->ActTime.byEndInterval = (BYTE)m_pVar->GetDword(_T("endinterval"), it->c_str(), 0);
		// ����map��
		m_mapActInfo.Add(pActInfo->dwID, pActInfo);
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// �̶���������ʼ��
//----------------------------------------------------------------------------------------------------
BOOL CActivityMgr::Init()
{
	CreateObj("AttResContainer", "VarContainer");
	m_pVar = "AttResContainer";

	// ���ع̶����̬����
	if (FALSE == LoadActivityData())
	{
		return FALSE;
	}

	KillObj("AttResContainer");

	// �����
	tagActInfo	*pActInfo = (tagActInfo*)GT_INVALID;
	CActivity	*pActivity = (CActivity*)GT_INVALID;
	tagDWORDTime cur_time = GetCurrentDWORDTime();

	m_mapActInfo.ResetIterator();
	while (m_mapActInfo.PeekNext(pActInfo))
	{
		pActivity = new CActivity;
		if(!P_VALID(pActivity))
			return FALSE;

		m_mapActivity.Add(pActInfo->dwID, pActivity);

		pActivity->Init(pActInfo);
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// minute update
//----------------------------------------------------------------------------------------------------
VOID CActivityMgr::OnMinute(DWORD cur_time)
{
	CActivity* pActivity = (CActivity*)GT_INVALID;

	// ���α������еĹ̶��������и���
	m_mapActivity.ResetIterator();
	while( m_mapActivity.PeekNext(pActivity) )
	{
		pActivity->OnMinute(cur_time);
	}

}

//----------------------------------------------------------------------------------------------------
// update
//----------------------------------------------------------------------------------------------------
VOID CActivityMgr::Update()
{
	CHECK_TIME();
	CActivity* pActivity = (CActivity*)GT_INVALID;

	// ���α������еĹ̶��������и���
	m_mapActivity.ResetIterator();
	while( m_mapActivity.PeekNext(pActivity) )
	{
		pActivity->Update();
	}
}

//----------------------------------------------------------------------------------------------------
// �Ƿ��л�ڽ�����
//----------------------------------------------------------------------------------------------------
BOOL CActivityMgr::HaveActivitStart()
{
	CActivity* pActivity = (CActivity*)GT_INVALID;

	// ���α������еĹ̶��������и���
	m_mapActivity.ResetIterator();
	while( m_mapActivity.PeekNext(pActivity) )
	{
		if(pActivity->IsStart())
			return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------------------------------
// �洢���л���ݵ����ݿ�
//----------------------------------------------------------------------------------------------------
VOID CActivityMgr::SaveAllActivityData2DB()
{
	CActivity* pActivity = (CActivity*)GT_INVALID;

	// ���α������еĹ̶��������и���
	m_mapActivity.ResetIterator();
	while( m_mapActivity.PeekNext(pActivity) )
	{
		pActivity->SaveActivityData2DB();
	}
}

CActivityMgr	g_activityMgr;






















